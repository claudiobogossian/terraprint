/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file Scene.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Scene.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/enum/Enums.h"
#include "pattern/command/DeleteCommand.h"
#include "pattern/command/UngroupCommand.h"
#include "../item/ItemGroup.h"
#include "BuildGraphicsItem.h"
#include "pattern/command/AddCommand.h"
#include "pattern/command/AddGroupCommand.h"
#include "../../core/template/TemplateEditor.h"
#include "../../core/template/AbstractTemplate.h"
#include "../../core/property/Properties.h"
#include "VisualizationArea.h"
#include "terralib/qt/widgets/Utils.h"
#include "../../core/Utils.h"
#include "terralib/common/STLUtils.h"
#include "../item/MapItem.h"
#include "../item/PaperItem.h"
#include "../../core/property/SharedProperties.h"
#include "../../core/PaperConfig.h"
#include "View.h"
#include "../../core/ContextObject.h"
#include "../../core/WorldTransformer.h"
#include "ItemUtils.h"
#include "../../core/ItemInputProxy.h"
#include "../../core/Utils.h"


// STL
#include <algorithm>
#include <complex>

// Qt
#include <QUndoCommand>
#include <QUndoStack>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPrinter>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QColor>
#include <QPen>
#include <QKeyEvent>
#include <QToolBar>

te::layout::Scene::Scene( QObject* object): 
  QGraphicsScene(object),
  AbstractScene(),
  m_undoStack(0),
  m_align(0),
  m_moveOrResizeWatched(false),
  m_paperConfig(0),
  m_currentItemEdition(0),
  m_isEditionMode(false),
  m_context(0,0,0,0),
  m_increasedUnprintableArea(40.),
  m_itemInputProxy(0)
{
  m_backgroundColor = QColor(109,109,109);
  setBackgroundBrush(QBrush(m_backgroundColor));
  
  m_paperConfig = new PaperConfig;
  m_align = new AlignItems(this, m_paperConfig);
  
  m_undoStack = new QUndoStack(this);
  connect(m_undoStack, SIGNAL(indexChanged(int)), this, SLOT(onUndoStackHasChanged()));

  m_itemInputProxy = new te::layout::ItemInputProxy(this);
}

te::layout::Scene::Scene( AlignItems* align, PaperConfig* paperConfig, QObject* object ) :
  QGraphicsScene(object),
  m_undoStack(0),
  m_align(align),
  m_moveOrResizeWatched(false),
  m_paperConfig(paperConfig),
  m_currentItemEdition(0),
  m_isEditionMode(false),
  m_context(0, 0, 0, 0),  
  m_increasedUnprintableArea(40.),
  m_itemInputProxy(0)
{
  m_itemInputProxy = new te::layout::ItemInputProxy(this);
}

te::layout::Scene::~Scene()
{
  m_moveWatches.clear();
  
  if(m_undoStack)
  {
    delete m_undoStack;
    m_undoStack = 0;
  }

  destroyItemsWithoutScene();
  
  if(m_align)
  {
    delete m_align;
    m_align = 0;
  }
  
  if(m_paperConfig)
  {
    delete m_paperConfig;
    m_paperConfig = 0;
  }

  for (std::map<std::string, ValueBase*>::iterator it = m_contextValues.begin(); it != m_contextValues.end(); ++it)
  {
    if (it->second != 0)
    {
      delete it->second;
      it->second = 0;
    }
  }

  delete m_itemInputProxy;
}

void te::layout::Scene::insertItem(AbstractItemView* item)
{
  if (!item)
  {
    return;
  }

  QGraphicsItem* qitem = dynamic_cast<QGraphicsItem*>(item);
  insertItem(qitem);
}

void te::layout::Scene::insertItem(QGraphicsItem* item)
{
  if(item == 0)
  {
    return;
  }

  if(item->scene() == this)
  {
    return;
  }

  
  AbstractItemView* abstractItem = dynamic_cast<AbstractItemView*>(item);
  if (abstractItem == 0)
  {
    return;
  }

  this->addItem(item); 
  
  abstractItem->refresh();

  removeItemStackWithoutScene(item);

  emit addItemFinalized(item);
}

void te::layout::Scene::init( double screenWMM, double screenHMM, ContextObject context)
{
  m_context = context;
  calculateSceneMeasures(screenWMM, screenHMM);
  
  if(!m_paperConfig)
  {
    m_paperConfig = new PaperConfig;
  }

  if(!m_align)
  {
    m_align = new AlignItems(this, m_paperConfig);
  }
}

void te::layout::Scene::calculateMatrixViewScene()
{
  double llx = m_box.m_llx;
  double lly = m_box.m_lly;
  double urx = m_box.m_urx;
  double ury = m_box.m_ury;

  ContextObject context = getContext();

  double dpiX = context.getDpiX();
  double dpiY = context.getDpiY();

  double factorX = (dpiX / 25.4);
  double factorY = (dpiY / 25.4);

  //inverted Y-Axis
  m_matrix = QTransform().scale(factorX, -factorY).translate(-llx, -ury);
  //World coordinate - mm
  setSceneRect(QRectF(QPointF(llx, lly), QPointF(urx, ury)));
}

void te::layout::Scene::calculateWindow( double wMM, double hMM )
{
  double ppSizeWMM = 0;
  double ppSizeHMM = 0;

  m_paperConfig->getPaperSize(ppSizeWMM, ppSizeHMM);

  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;

  double paddingX = 0;
  double paddingY = 0;

  //X-Axis
  double minX = std::min(ppSizeWMM, wMM);
  double maxX = std::max(ppSizeWMM, wMM);

  paddingX = (maxX - minX) / 2;
  x1 = - paddingX;
  x2 = maxX - paddingX;

  //Y-Axis
  double minY = std::min(ppSizeHMM, hMM);
  double maxY = std::max(ppSizeHMM, hMM);

  paddingY = (maxY - minY) / 2;
  y1 = - paddingY;
  y2 = maxY - paddingY;

  m_box.m_llx = x1;
  m_box.m_lly = y1;
  m_box.m_urx = x2;
  m_box.m_ury = y2;
}

QTransform te::layout::Scene::sceneTransform()
{
  return m_matrix;
}

void te::layout::Scene::deleteItems()
{
  std::vector<std::string> names;

  QList<QGraphicsItem*> graphicsItems = selectedItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    deleteItem(item);
  }

  if(!names.empty())
    emit deleteFinalized(names);
}

void te::layout::Scene::removeSelectedItems()
{
  std::vector<std::string> names;

  QList<QGraphicsItem*> graphicsItems = selectedItems();

  if(graphicsItems.empty())
    return;

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      AbstractItemView* abstractItem = dynamic_cast<AbstractItemView*>(item);
      if (abstractItem)
      {
        const Property& pName = abstractItem->getProperty("name");
        names.push_back(pName.getValue()->toString());
      }
    }
  }

  QUndoCommand* command = new DeleteCommand(this, graphicsItems);
  addUndoStack(command);

  if(!names.empty())
    emit deleteFinalized(names);
}

bool te::layout::Scene::removeItemByName(std::string name)
{
  bool result = false;

  std::vector<std::string> names;

  AbstractItemView* abstractItem = getItem(name);
  if (!abstractItem)
    return result;

  QList<QGraphicsItem*> graphicsItems;
  if (abstractItem)
  {
    const Property& pName = abstractItem->getProperty("name");
    names.push_back(pName.getValue()->toString());
    result = true;
  }

  QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(abstractItem);
  if (!item || names.empty())
    return result;

  graphicsItems.push_back(item);

  QUndoCommand* command = new DeleteCommand(this, graphicsItems);
  addUndoStack(command);

  emit deleteFinalized(names);

  return result;
}

void te::layout::Scene::addUndoStack( QUndoCommand* command )
{
  m_undoStack->push(command);
}

QUndoStack* te::layout::Scene::getUndoStack()
{
  return m_undoStack;
}

QList<QGraphicsItem*> te::layout::Scene::getListUngroupedItems(const QList<QGraphicsItem *> & items, EnumType* groupType)
{
  QList<QGraphicsItem*> listUngroupedItems;
  if (!groupType)
  {
    return listUngroupedItems;
  }

  EnumObjectType* object = Enums::getInstance().getEnumObjectType();
  size_t size = items.size();
  for (int i = 0; i < items.size(); ++i)
  {
    QGraphicsItem* item = items[i];
    AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
    if (view == 0)
    {
			/*It can be a map with custom items, 
			which can be loaded into another plugin that uses layout module. 
			It may be that in the plugin version, there is no custom item. Example: a different Legend Item. */
      continue;
    }
    
    if (view->getProperties().getTypeObj() == groupType)
    {
      QList<QGraphicsItem*> childItems = item->childItems();
      foreach(QGraphicsItem* childItem, childItems)
      {
        listUngroupedItems.append(childItem);
      }

      this->removeItem(item);
      removeItemGroup((te::layout::ItemGroup*)item);
    }
    else
    {
      listUngroupedItems.append(item);
    }
  }
  return listUngroupedItems;
}

QGraphicsItem* te::layout::Scene::createGroup(EnumType* groupType)
{
  QGraphicsItem* item = 0;

  Properties properties;

  QList<QGraphicsItem *> list = selectedItems();
  item = createItemGroup(list, 0, groupType);

  if (m_undoStack)
  {
    ItemGroup* group = dynamic_cast<ItemGroup*>(item);
    QUndoCommand* command = new AddGroupCommand(group);
    addUndoStack(command);
  }

  return item;
}

bool te::layout::Scene::removeGroup(te::layout::ItemGroup* group)
{
  bool result = true;
  if (m_undoStack)
  {
    QUndoCommand* command = new UngroupCommand(this, group);
    addUndoStack(command);
  }
  else
  {
    result = removeItemGroup(group);
  }
  return result;
}

QGraphicsItem* te::layout::Scene::createItemGroup(const QList<QGraphicsItem *> & items, QGraphicsItem* itemGroup, EnumType* groupType)
{
  this->clearSelection();

  EnumObjectType* object = Enums::getInstance().getEnumObjectType();

  if (!groupType)
  {
    groupType = object->getItemGroup();
  }

  QList<QGraphicsItem*> listUngroupedItems = getListUngroupedItems(items, groupType);
  sortByZValue(listUngroupedItems);
  
  //we need to reorder the z values in order to make the group be correctly placed on the map
  std::vector<int> vecZValues;
  double x = 0.;
  double y = 0.;
  for(int i = 0; i <  listUngroupedItems.size(); ++i)
  {
    QGraphicsItem* currentItem = listUngroupedItems[i];
    vecZValues.push_back(currentItem->zValue());

    if(i == 0)
    {
      x = currentItem->pos().x();
      y = currentItem->pos().y();
      continue;
    }
    
    if(currentItem->pos().x() < x)
    {
      x = currentItem->pos().x();
    }
    if(currentItem->pos().y() < y)
    {
      y = currentItem->pos().y();
    }
  }
  
  // The group component must be initialized with a position (setPos).
  te::gm::Coord2D coord(x, y);

  QGraphicsItem* item = 0;

  if (!itemGroup)
  {
    bool addUndo = false; // force the BuildGraphicsItem not create a AddCommand, since the group has a different command for add
    //The scene create a new group with important restriction
    BuildGraphicsItem build(this);
    item = build.createItem(groupType, coord, 0, 0, addUndo);
  }
  else
  {
    item = itemGroup;
    insertItem(item);
  }

  ItemGroup* group = dynamic_cast<ItemGroup*>(item);

  vecZValues.push_back(item->zValue());

  group->setUndoEnabled(false);

  if(group)
  {
    item->setZValue(vecZValues[0]);

    for (int i = 0; i < listUngroupedItems.size(); ++i)
    {
      AbstractItemView* iView = dynamic_cast<AbstractItemView*>(listUngroupedItems[i]);
      iView->setUndoEnabled(false);

      listUngroupedItems[i]->setZValue(vecZValues[i+1]);
      group->addToGroup(listUngroupedItems[i]);
    }

    for (int i = 0; i < listUngroupedItems.size(); ++i)
    {
      AbstractItemView* iView = dynamic_cast<AbstractItemView*>(listUngroupedItems[i]);
      iView->setUndoEnabled(true);
    }
  }
  group->setUndoEnabled(true);

  emit addItemFinalized(group);
  
  return group;
}

bool te::layout::Scene::removeItemGroup(te::layout::ItemGroup* group )
{
  if (!group)
  {
    return false;
  }
  
  QList<QGraphicsItem*> listUngroupedItems;
  QList<QGraphicsItem*> childItems = group->childItems();
  
  group->setHandlesChildEvents(false);
  group->setUndoEnabled(false);

  std::vector<int> vecZValues;
  vecZValues.push_back(group->zValue());
  for (int i = 0; i < childItems.size(); ++i)
  {
    vecZValues.push_back(childItems[i]->zValue());
    
    AbstractItemView* iView = dynamic_cast<AbstractItemView*>(childItems[i]);
    iView->setUndoEnabled(false);

    childItems[i]->setZValue(vecZValues[i]);
        
    group->removeFromGroup(childItems[i]);
  }

  for (int i = 0; i < childItems.size(); ++i)
  {
    AbstractItemView* iView = dynamic_cast<AbstractItemView*>(childItems[i]);

    iView->setUndoEnabled(true);
  }

  group->setUndoEnabled(true);
  
  std::vector<std::string> vecNames;
  AbstractItemView* abstractItem = dynamic_cast<AbstractItemView*>(group);
  if (abstractItem)
  {
    const Property& pName = abstractItem->getProperty("name");
    vecNames.push_back(pName.getValue()->toString());
  }

  QList<QGraphicsItem*> listItems;
  listItems.push_back(group);

  removeItem(group);
  addItemStackWithoutScene(group);
  
  if (!vecNames.empty())
    emit deleteFinalized(vecNames);

  return true;
}

void te::layout::Scene::calculateSceneMeasures(double widthMM, double heightMM)
{
  // increases the unprintable area for better visualization of the edges of the paper
  increasedUnprintableArea(widthMM, heightMM);

  calculateWindow(widthMM, heightMM);

  double w = m_box.getWidth();
  double h = m_box.getHeight();

  calculateWindow(w,h);

  calculateMatrixViewScene();
}

bool te::layout::Scene::exportPropertiesToTemplate( EnumType* type, std::string fileName )
{
  bool is_export = false;

  if(fileName.compare("") == 0)
  {
    return is_export;
  }
  
  std::vector<te::layout::Properties> properties;
  std::map< std::string, std::vector<std::string> > mapGroups;
  getItemsProperties(properties, mapGroups);

  if (properties.empty())
    return is_export;
  
  TemplateEditor editor(type, fileName);

  AbstractTemplate* jtemplate = editor.getTemplate();

  if(!jtemplate)
    return is_export;

  is_export = jtemplate->exportTemplate(*m_paperConfig, properties, mapGroups);

  return is_export;
}

bool te::layout::Scene::importTemplateToProperties(EnumType* type, std::string fileName, std::vector<te::layout::Properties>& properties, std::map< std::string, std::vector<std::string> >& mapGroups)
{
  if(fileName.compare("") == 0)
  {
    return false;
  }
  
  TemplateEditor editor(type, fileName);

  AbstractTemplate* jtemplate = editor.getTemplate();

  if(!jtemplate)
    return false;

  te::layout::PaperConfig paperConfig;
  jtemplate->importTemplate(paperConfig, properties, mapGroups);

  double w = 0;
  double h = 0;

  m_paperConfig->setPaperOrientation(paperConfig.getPaperOrientantion());
  paperConfig.getPaperSize(w, h);
  m_paperConfig->setPaperSizeCustom(w, h);
  m_paperConfig->setPaperType(paperConfig.getPaperType());

  return true;
}

bool te::layout::Scene::getItemsProperties(std::vector<te::layout::Properties>& properties, std::map< std::string, std::vector<std::string> >& mapGroups)
{
  EnumObjectType* objectType = Enums::getInstance().getEnumObjectType();

  QList<QGraphicsItem*> graphicsItems = items();
  graphicsItems = sortItemsByDependency(graphicsItems);

  foreach(QGraphicsItem *qItem, graphicsItems)
  {
    AbstractItemView* absItem = dynamic_cast<AbstractItemView*>(qItem);
    if (absItem == 0)
    {
      continue;
    }

    //I am inside a group
    if (qItem->parentItem() != 0)
    {
      const std::string& absItemName = absItem->getProperty("name").getValue()->toString();

      AbstractItemView* absParentItem = dynamic_cast<AbstractItemView*>(qItem->parentItem());
      if (absParentItem == 0)
      {
        continue;
      }

      //we need to check if the parent is a group. If it is not, we dont register anything in the map
      std::string objectTypeName = absParentItem->getProperties().getTypeObj()->getName();
      if (objectTypeName == objectType->getItemGroup()->getName())
      {
        const std::string& absParentName = absParentItem->getProperty("name").getValue()->toString();
        mapGroups[absParentName].push_back(absItemName);
      }
      else
      {
        //we must not export items that are inside parent items that arent groups. These items must handle themselves
        continue;
      }
    }

    //Adds the properties of the item to the vector
    AbstractItemView* lItem = dynamic_cast<AbstractItemView*>(qItem);
    if(lItem)
    {
      const Property& pIsPrintable = lItem->getProperty("printable");
      if(te::layout::Property::GetValueAs<bool>(pIsPrintable) == false)
        continue;
        
      properties.push_back(lItem->getProperties());
    }
  }

  return true;
}

void te::layout::Scene::onUndoStackHasChanged()
{
  emit undoStackHasChanged();
}


QList<QGraphicsItem*> te::layout::Scene::sortItemsByDependency(const QList<QGraphicsItem*>& listItems)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::map< std::string, std::string > mapDependency;
  std::map< std::string, QGraphicsItem* > mapItems;

  //we first analyse all the item to detect which item depends on which 
  foreach(QGraphicsItem *qItem, listItems)
  {
    AbstractItemView* absItem = dynamic_cast<AbstractItemView*>(qItem);
    if (absItem == 0)
    {
      continue;
    }

    const std::string& absItemName = te::layout::Property::GetValueAs<std::string>(absItem->getProperty("name"));

    mapItems[absItemName] = qItem;

    
    const Properties& properties = absItem->getProperties();
    const std::vector<Property>& vecProperty = properties.getProperties();
    for (size_t i = 0; i < vecProperty.size(); ++i)
    {
      const Property& property = vecProperty[i];
      if (property.getType() == dataType->getDataTypeItemObserver())
      {
        const std::string& parentItemName = te::layout::Property::GetValueAs<std::string>(property);
        mapDependency[absItemName] = parentItemName;
      }
    }
  }

  QList<QGraphicsItem*> sortedList = listItems;

  //and now we sort the list to ensure that the items that have dependency are listed after the ones that they depend on
  std::map< std::string, std::string >::const_iterator it = mapDependency.begin();
  while (it != mapDependency.end())
  {
    const std::string& itemName = it->first;
    const std::string& parentName = it->second;

    //QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(getItem(itemName));
    //QGraphicsItem* parent = dynamic_cast<QGraphicsItem*>(getItem(parentName));
    
    QGraphicsItem* item = mapItems[itemName];
    QGraphicsItem* parent = mapItems[parentName];

    int itemIndex = sortedList.indexOf(item);
    int parentIndex = sortedList.indexOf(parent);

    if (parentIndex > itemIndex)
    {
      sortedList.move(itemIndex, parentIndex);

      //as we can have several levels in the dependency tree, we must restart the analysis to ensure that the sort is correct
      it = mapDependency.begin();
      continue;
    }

    ++it;
  }

  return sortedList;

}


void te::layout::Scene::reset()
{
  m_currentItemEdition = 0;

  if(!m_undoStack)
    return;

  if(m_undoStack->count() > 0)
    m_undoStack->clear();

  clear();
}

bool te::layout::Scene::buildTemplate( VisualizationArea* vzArea, EnumType* type, std::string fileName )
{
  BuildGraphicsItem build(this);
  
  std::vector<te::layout::Properties> properties;
  std::map< std::string, std::vector<std::string> > mapGroups;
  if (importTemplateToProperties(type, fileName, properties, mapGroups) == false)
  {
    return false;
  }

  if (properties.empty())
    return false;

  reset();

  std::vector<te::layout::Properties>::iterator it;

  te::gm::Envelope boxW = getSceneBox();
  vzArea->changeBoxArea(boxW);
  vzArea->build();

  getView()->onChangeConfig();

  te::layout::EnumType* groupType = Enums::getInstance().getEnumObjectType()->getItemGroup();
  
  //we create the items
  std::map<std::string, te::layout::Properties> mapProperties;
  for (it = properties.begin(); it != properties.end(); ++it)
  {
    const te::layout::Properties& proper = (*it);

    if(proper.getProperties().empty())
      continue;

    if (proper.getTypeObj() == groupType)
    {
      mapProperties[te::layout::Property::GetValueAs<std::string>(proper.getProperty("name"))] = proper;
      continue;
    }
    
    build.buildItem(proper, false);
  }

  //then we create the groups
  std::map< std::string, std::vector<std::string> >::const_iterator itGroups = mapGroups.begin();
  while (itGroups != mapGroups.end())
  {
    const std::string& groupName = itGroups->first;
    const std::vector<std::string>& vecItemNames = itGroups->second;

    QList<QGraphicsItem*> listItems;
    for (size_t i = 0; i < vecItemNames.size(); ++i)
    {
      const std::string& itemName = vecItemNames[i];
      QGraphicsItem* qItem = dynamic_cast<QGraphicsItem*>(this->getItem(itemName));

      listItems.append(qItem);
    }

    const Properties& groupProperties = mapProperties[groupName];

    QGraphicsItem* qItemGroup = createItemGroup(listItems, 0, groupProperties.getTypeObj());
    AbstractItemView* itemView = dynamic_cast<AbstractItemView*>(qItemGroup);
    if (itemView != 0)
    {
      changeUndoEnable(listItems, false);

      itemView->setUndoEnabled(false);
      itemView->setProperties(groupProperties);
      itemView->setUndoEnabled(true);

      changeUndoEnable(listItems, true);
    }

    ++itGroups;
  }

  return true;
}

void te::layout::Scene::buildItem(te::layout::Properties props,std::string &name, bool isCopy)
{
  BuildGraphicsItem build(this);

  build.buildItem(props, false, isCopy);
  name = build.getNewName();

}

void te::layout::Scene::redrawSelectionMap()
{
  QList<QGraphicsItem*> selected = selectedItems();

  QGraphicsItem* subSelectedItem = this->getSubSelectedItem();
  if (subSelectedItem != 0)
  {
    selected.append(subSelectedItem);
  }

  foreach(QGraphicsItem *item, selected) 
  {
    MapItem* mapItem = dynamic_cast<MapItem*>(item);
    if(mapItem)
    {
      mapItem->redraw();
    }
  }
}

void te::layout::Scene::exportItemsToImage(std::string dir)
{
  const int dpi = 96;
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();
  ContextObject context(100, dpi, dpi, enumMode->getModePrinter());
  
  ContextObject oldContext = this->getContext();
  this->setContext(context);

  QList<QGraphicsItem*> selected = selectedItems();
  foreach(QGraphicsItem *item, selected) 
  {
    AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
    if (view == 0)
    {
      continue;
    }

    const std::string& name = Property::GetValueAs<std::string>(view->getProperty("name"));

    QRectF rectInItemCS = item->boundingRect();
    QRectF rectInSceneCS = item->mapRectToScene(rectInItemCS); //rect in Millimeters

    //we first calculate the size in pixels
    double widthPixels = Utils::mm2pixel(rectInSceneCS.width(), dpi);
    double heightPixels = Utils::mm2pixel(rectInSceneCS.height(), dpi);

    double scaleX = widthPixels / rectInSceneCS.width();
    double scaleY = heightPixels / rectInSceneCS.height();

    QImage image(widthPixels, heightPixels, QImage::Format_ARGB32);
    image.fill(Qt::white); // background

    QStyleOptionGraphicsItem styleOption;
    QPainter painter;
    painter.begin(&image);
    painter.scale(scaleX, -scaleY);
    painter.translate(0, -rectInSceneCS.height());

    //draws the item
    item->paint(&painter, &styleOption);

    painter.end();

    std::string dirName = dir + "/" + name + ".png";
    image.save(dirName.c_str());
  }

  this->setContext(oldContext);
}

void te::layout::Scene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  if (m_isEditionMode && m_currentItemEdition) // Don't have move or resize event in edition mode
  {
    //if an item is in edition mode, we restrict the events to its position
    QPointF pointScene = mouseEvent->scenePos();
    QGraphicsItem* gItem = dynamic_cast<QGraphicsItem*>(m_currentItemEdition);

    if (gItem->contains(gItem->mapFromScene(pointScene)))
    {
      QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    return;
  }

  QGraphicsScene::mouseMoveEvent(mouseEvent);

  QGraphicsItem* item = mouseGrabberItem();
  if (item) // MoveCommand and ChangePropertyCommand block
  {
    m_moveOrResizeWatched = true;
  }
}

void te::layout::Scene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  if (m_isEditionMode && m_currentItemEdition) // Don't have move or resize event in edition mode
  {
    //if an item is in edition mode, we restrict the events to its position
    QPointF pointScene = mouseEvent->scenePos();
    QGraphicsItem* gItem = dynamic_cast<QGraphicsItem*>(m_currentItemEdition);
    
    if (gItem->contains(gItem->mapFromScene(pointScene)))
    {
      QGraphicsScene::mousePressEvent(mouseEvent);
    }
    return;
  }

  QGraphicsItem* currentSubSelectedItem = getSubSelectedItem();
  QGraphicsScene::mousePressEvent(mouseEvent);
  QGraphicsItem* newSubSelectedItem = getSubSelectedItem();

  if (currentSubSelectedItem != newSubSelectedItem)
  {
    emit selectionChanged();
  }

  QGraphicsItem* item = mouseGrabberItem();
  if (item) // MoveCommand and ChangePropertyCommand block
  {
    searchSelectedItemsInResizeMode();
    if (m_resizeWatches.empty())
    {
      searchSelectedItemsInMoveMode();
    }
  }
}

void te::layout::Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  if (m_isEditionMode && m_currentItemEdition) // Don't have move or resize event in edition mode
  {
    //if an item is in edition mode, we restrict the events to its position
    QPointF pointScene = mouseEvent->scenePos();
    QGraphicsItem* gItem = dynamic_cast<QGraphicsItem*>(m_currentItemEdition);

    if (gItem->contains(gItem->mapFromScene(pointScene)))
    {
      QGraphicsScene::mouseReleaseEvent(mouseEvent);
    }
    return;
  }
  
  QGraphicsScene::mouseReleaseEvent(mouseEvent);

  m_moveWatches.clear();
  m_resizeWatches.clear();

  if (m_moveOrResizeWatched)
  {
    update();
  }

  m_moveOrResizeWatched = false;
}

void te::layout::Scene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
  QGraphicsScene::mouseDoubleClickEvent(mouseEvent);

  if (m_currentItemEdition && m_isEditionMode)
  {
    QPointF pt = mouseEvent->scenePos();
    QGraphicsItem* qCurrentItem = dynamic_cast<QGraphicsItem*>(m_currentItemEdition);
    pt = qCurrentItem->mapFromScene(pt);
    if (qCurrentItem->contains(pt))
    {
      return; // the same item continues edition
    }
  }

  if (m_isEditionMode == true)
  {
    closeDock();
    setEditionMode(true); 
    if (!m_isEditionMode)
    {
      setEditionMode(false); //Edition off
    }
    else
    {
      showDock();
    }
    return;
  }

  setEditionMode(true);
  if (m_isEditionMode)
  {
    showDock();
  }
  else
  {
    closeDock();
    setEditionMode(false); //Edition off
  }
}

void te::layout::Scene::keyPressEvent(QKeyEvent * keyEvent)
{
  QGraphicsScene::keyPressEvent(keyEvent);
  if (keyEvent->isAccepted() == true)
  {
    return;
  }

  if (keyEvent->key() == Qt::Key_Escape)
  {
    closeDock();
    setEditionMode(false); //Edition off
    keyEvent->setAccepted(true);
  }
  else if (keyEvent->matches(QKeySequence::Undo))
  {
    m_undoStack->undo();
    keyEvent->setAccepted(true);
  }
  else if (keyEvent->matches(QKeySequence::Redo))
  {
    m_undoStack->redo();
    keyEvent->setAccepted(true);
  }
}

void te::layout::Scene::drawForeground(QPainter * painter, const QRectF & rect)
{
  QGraphicsScene::drawForeground(painter, rect);

  if (m_isEditionMode)
  {
    if (!m_currentItemEdition)
      return;

    if (m_currentItemEdition->isEditionMode() == true)
    {
      QGraphicsItem* item = dynamic_cast<QGraphicsItem*>(m_currentItemEdition);
      if (!item)
        return;

      painter->save();

      QRectF rec = item->sceneBoundingRect();

      QPainterPath outerPath;
      outerPath.setFillRule(Qt::WindingFill);
      outerPath.addRect(rect); //rectangle outside

      QPainterPath innerPath;
      innerPath.addRect(rec); // rectangle inside
      QPainterPath fillPath = outerPath.subtracted(innerPath);

      //config painter
      QColor backgroundColor(0, 0, 0, 80);
      painter->setRenderHint(QPainter::Antialiasing);
      painter->fillPath(fillPath, backgroundColor);

      painter->restore();
    }
  }
}

void te::layout::Scene::selectItem(std::string name)
{
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems) 
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if(it)
      {
        const Property& pItemName = it->getProperty("name");
        const std::string& itemName = te::layout::Property::GetValueAs<std::string>(pItemName);
        if(itemName.compare(name) == 0)
        {
          item->setSelected(true);
        }
        else
        {
          item->setSelected(false);
        }
      }
    }
  }
}

void te::layout::Scene::selectItem( QGraphicsItem* item )
{
  if (item)
  {
    item->setSelected(true);
  }
}

void te::layout::Scene::selectItems( std::vector<std::string> names )
{
  foreach(std::string name, names)
  {
    this->selectItem(name);
  }
}

void te::layout::Scene::selectItems( QList<QGraphicsItem*> items )
{
  foreach(QGraphicsItem* item, items)
  {
    this->selectItem(item);
  }
}

void te::layout::Scene::redrawItems()
{
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems) 
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if(it)
      {
        const Property& pIsPrintable = it->getProperty("printable");
        if(te::layout::Property::GetValueAs<bool>(pIsPrintable) == true)
        {
          it->refresh();
        }
      }
    }
  }
}

bool te::layout::Scene::addItemStackWithoutScene( QGraphicsItem* item )
{
  if(!item)
  {
    return false;
  }

  if(item->scene() == this)
  {
    return false;
  }

  if(m_itemStackWithoutScene.contains(item))
  {
    return false;
  }

  m_itemStackWithoutScene.push_back(item);

  return true;
}

bool te::layout::Scene::removeItemStackWithoutScene( QGraphicsItem* item )
{
  if(!item)
  {
    return false;
  }

  return m_itemStackWithoutScene.removeOne(item);
}

void te::layout::Scene::deletePaperItem()
{
  QGraphicsItem* item = getPaperItem();
  if(!item)
  {
    return;
  }
  deleteItem(item);     
}

bool te::layout::Scene::deleteItem( QGraphicsItem *item )
{
  bool result = false;

  if (item)
  {
    removeItem(item);
    if(item)
    {
      delete item;
      result = true;
    }
  }

  return result;
}

QGraphicsItem* te::layout::Scene::getPaperItem()
{
  QGraphicsItem* paper = 0;

  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems)
  {
    if(item)
    {
      PaperItem* pItem = dynamic_cast<PaperItem*>(item);
      if(pItem)
      {
        paper = item;
        break;
      }
    }
  }

  return paper;
}

void te::layout::Scene::applyPaperProportion(QSize oldPaper, QSize newPaper)
{
  applyProportionAllItems(oldPaper, newPaper);
}

void te::layout::Scene::applyProportionAllItems( QSize oldPaper, QSize newPaper )
{
  QGraphicsItem* paper = getPaperItem(); 

  Utils utils = this->getUtils();

  te::gm::Envelope oldPp(0, 0, oldPaper.width(), oldPaper.height());
  te::gm::Envelope newPp(0, 0, newPaper.width(), newPaper.height());

  WorldTransformer transf = utils.getTransformGeo(oldPp, newPp);
  transf.setMirroring(false);
  
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems)
  {
    if(item)
    {
      if(item != paper)
      {
        AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
        if(it)
        {           
          double x = item->scenePos().x();
          double y = item->scenePos().y();
          if (item->parentItem())
          {
            x = item->pos().x();
            y = item->pos().y();
          }

          double width = item->boundingRect().width();
          double height = item->boundingRect().height();

          // map position to new position
          transf.system1Tosystem2(x, y, x, y);

          te::gm::Envelope box;
          box.m_llx = x;
          box.m_lly = y;
          box.m_urx = (box.m_llx + width);
          box.m_ury = (box.m_lly + height);
          
          // calculate proportion
          te::gm::Envelope boxProportion = calculateProportion(box, oldPaper, newPaper);

          box.m_llx = x;
          box.m_lly = y;
          box.m_urx = (box.m_llx + boxProportion.getWidth());
          box.m_ury = (box.m_lly + boxProportion.getHeight());
                    
          updateBoxFromProperties(box, it);
        }
      }
    }
  }
}

void te::layout::Scene::updateBoxFromProperties(te::gm::Envelope box, AbstractItemView* view)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  Properties props;
  
  double x = box.m_llx;
  double y = box.m_lly;
  double width = box.getWidth();
  double height = box.getHeight();

  Property pro_x;
  pro_x.setName("x");
  pro_x.setValue(x, dataType->getDataTypeDouble());
  props.addProperty(pro_x);

  Property pro_y;
  pro_y.setName("y");
  pro_y.setValue(y, dataType->getDataTypeDouble());
  props.addProperty(pro_y);

  Property pro_width;
  pro_width.setName("width");
  pro_width.setValue(width, dataType->getDataTypeDouble());
  props.addProperty(pro_width);

  Property pro_height;
  pro_height.setName("height");
  pro_height.setValue(height, dataType->getDataTypeDouble());
  props.addProperty(pro_height);

  view->setProperties(props);
}

te::gm::Envelope te::layout::Scene::calculateProportion(te::gm::Envelope box, QSize oldPaper, QSize newPaper)
{
  // The comparison is always made with the paper in portrait.
  double w = 0;
  if (oldPaper.width() > oldPaper.height())
  {
    w = oldPaper.width();
    oldPaper.setWidth(oldPaper.height());
    oldPaper.setHeight(w);
  }

  if (newPaper.width() > newPaper.height())
  {
    w = newPaper.width();
    newPaper.setWidth(newPaper.height());
    newPaper.setHeight(w);
  }

  // calculate proportion
  te::gm::Envelope boxCopy = box;
  boxCopy.m_llx = ((box.m_llx * newPaper.width()) / oldPaper.width());
  boxCopy.m_lly = ((box.m_lly * newPaper.height()) / oldPaper.height());
  boxCopy.m_urx = ((box.m_urx * newPaper.width()) / oldPaper.width());
  boxCopy.m_ury = ((box.m_ury * newPaper.height()) / oldPaper.height());

  return boxCopy;
}

const te::layout::ContextObject& te::layout::Scene::getContext() const
{
  return m_context;
}

void te::layout::Scene::updateAllItemsContext()
{
  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems)
  {
    if(item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if(it)
      {
        it->contextUpdated(m_context);
      }
    }
  }
}

te::layout::AlignItems* te::layout::Scene::getAlignItems()
{
  return m_align;
}

te::layout::View* te::layout::Scene::getView()
{
  View* view = 0;
  QList<QGraphicsView*> viewList = views();

  QGraphicsView* gview = viewList.first();
  if(!gview)
  {
    return view;
  }

  view = dynamic_cast<View*>(gview);
  if(!view)
  {
    return view;
  }
  return view;
}

void te::layout::Scene::deselectAllItems()
{
  QList<QGraphicsItem*> graphicsItems = items();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      item->setSelected(false);
    }
  }
}

te::layout::PaperConfig* te::layout::Scene::getPaperConfig()
{
  return m_paperConfig;
}

void te::layout::Scene::setEditionMode(bool editionMode)
{
  if (editionMode == true)
  {
    m_isEditionMode = enterEditionMode();
  }
  else
  {
    leaveEditionMode();
    m_isEditionMode = false;
  }
}

te::layout::ItemUtils te::layout::Scene::getItemUtils()
{
  return te::layout::ItemUtils(this);
}

te::layout::Utils te::layout::Scene::getUtils()
{
  return te::layout::Utils(this);
}

bool te::layout::Scene::isEditionMode()
{
  return m_isEditionMode;
}

bool te::layout::Scene::enterEditionMode()
{
  QGraphicsItem* item = mouseGrabberItem();

  QGraphicsItem* subSelectedItem = this->getSubSelectedItem();
  if (subSelectedItem != 0)
  {
    item = subSelectedItem;
  }
  if (item == 0)
  {
    return false;
  }

  AbstractItemView* absItem = dynamic_cast<AbstractItemView*>(item);
  if (!absItem)
    return false;

  const Property& property = absItem->getProperty("editable");
  if (te::layout::Property::GetValueAs<bool>(property) == false)
  {
    return false;
  }

  if (m_currentItemEdition)
    m_currentItemEdition->setEditionMode(false);

  m_currentItemEdition = absItem;
  m_isEditionMode = true;
  if (getView())
  {
    getView()->setDragMode(QGraphicsView::NoDrag);
  }
  m_currentItemEdition->setEditionMode(true);
  update();

  emit editionInitialized();

  return true;
}

void te::layout::Scene::leaveEditionMode()
{
  if (!m_currentItemEdition)
  {
    return;
  }
  m_isEditionMode = false;
  if (getView())
  {
    getView()->setDragMode(QGraphicsView::RubberBandDrag);
  }
  m_currentItemEdition->setEditionMode(false);
  update();
  emit editionFinalized();
}

void te::layout::Scene::setContext(ContextObject context)
{
  if (context.isValid())
  {
    m_context = context;

    contextUpdated();

    emit updateAllItemsContext();
  }
}

QGraphicsItem* te::layout::Scene::getSubSelectedItem() const
{
  QList<QGraphicsItem*> allItems = this->items();
  QList<QGraphicsItem*>::const_iterator it = allItems.begin();
  while (it != allItems.end())
  {
    AbstractItemView* item = dynamic_cast<AbstractItemView*>(*it);
    if (item != 0)
    {
      if (item->isSubSelected() == true)
      {
        return *it;
      }
    }
    ++it;
  }

  return 0;
}

te::layout::AbstractItemView* te::layout::Scene::getItem(const std::string& name)
{
  AbstractItemView* abstractItem = 0;

  QList<QGraphicsItem*> allItems = items();
  foreach(QGraphicsItem *item, allItems)
  {
    if (item)
    {
      AbstractItemView* it = dynamic_cast<AbstractItemView*>(item);
      if (it)
      {
        const Property& property = it->getProperty("name");
        if (name.compare(te::layout::Property::GetValueAs<std::string>(property)) == 0)
        {
          abstractItem = it;
          break;
        }
      }
    }
  }

  return abstractItem;
}

void te::layout::Scene::setProxyProject(AbstractProxyProject* proxyProject)
{
  if (!proxyProject)
    return;

  ValueBase* value = new Value<AbstractProxyProject*>(proxyProject);
  m_contextValues["proxy_project"] = value;
}

void te::layout::Scene::showDock()
{
  if (m_currentItemEdition)
  {
    View* view = getView();
    if (view)
    {
      EnumType* itemType = m_currentItemEdition->getProperties().getTypeObj();
      if (itemType)
      {
        view->showToolbar(itemType, m_currentItemEdition);
      }
    }
  }
}

void te::layout::Scene::closeDock()
{
  if (m_currentItemEdition)
  {
    View* view = getView();
    if (view)
    {
      if (m_currentItemEdition)
      {
        view->closeToolbar();
      }
    }
  }
}

void te::layout::Scene::increasedUnprintableArea(double& screenWMM, double& screenHMM)
{
  double ppSizeWMM = 0;
  double ppSizeHMM = 0;

  m_paperConfig->getPaperSize(ppSizeWMM, ppSizeHMM);

  double wmaxMM = qMax(ppSizeWMM, screenWMM);
  double wminMM = qMin(ppSizeWMM, screenWMM);
  double hmaxMM = qMax(ppSizeHMM, screenHMM);
  double hminMM = qMin(ppSizeHMM, screenHMM);

  screenWMM += (wmaxMM - wminMM) + m_increasedUnprintableArea;
  screenHMM += (hmaxMM - hminMM) + m_increasedUnprintableArea;
}

void te::layout::Scene::searchSelectedItemsInResizeMode()
{
  QList<QGraphicsItem*> its = selectedItems();
  m_resizeWatches.clear();

  foreach(QGraphicsItem *itm, its)
  {
    AbstractItemView* itemView = dynamic_cast<AbstractItemView*>(itm);
    if (itemView)
    {
      searchSelectedChildItemsInResizeMode(itm);
    }
  }
}

void te::layout::Scene::searchSelectedChildItemsInResizeMode(QGraphicsItem* item)
{
  QList<QGraphicsItem*> its = item->childItems();
  if (its.isEmpty())
  {
    AbstractItemView* itemView = dynamic_cast<AbstractItemView*>(item);
    // Undo/Redo for Resize (ChangePropertyCommand)
    if (itemView->getCurrentAction() == te::layout::RESIZE_ACTION)
    {
      m_resizeWatches[item] = itemView->getProperties();
    }
    return;
  }

  foreach(QGraphicsItem *itm, its)
  {
    AbstractItemView* itemView = dynamic_cast<AbstractItemView*>(itm);
    if (itemView)
    {
      if (!itm->childItems().isEmpty())
      {
        searchSelectedChildItemsInResizeMode(itm);
      }
      // Undo/Redo for Resize (ChangePropertyCommand)
      if (itemView->getCurrentAction() == te::layout::RESIZE_ACTION)
      {
        m_resizeWatches[itm] = itemView->getProperties();
      }
    }
  }
}

void te::layout::Scene::searchSelectedItemsInMoveMode()
{
  QList<QGraphicsItem*> its = selectedItems();
  m_moveWatches.clear();
  
  foreach(QGraphicsItem *itm, its)
  {
    QPointF pt = itm->scenePos();
    m_moveWatches[itm] = pt;
  }
}

void te::layout::Scene::sortByZValue(QList<QGraphicsItem *> & listItems)
{
  std::vector<QGraphicsItem*> vecItems;
  for (int i = 0; i < listItems.size(); ++i)
  {
    vecItems.push_back(listItems[i]);
  }

  // using function as comparison
  // orders of lower zValue to the higher zValue
  std::sort(vecItems.begin(), vecItems.end(), zValueLessThan);

  listItems.clear();
  for (int i = 0; i < vecItems.size(); ++i)
  {
    listItems.append(vecItems[i]);
  }
}

bool te::layout::Scene::zValueLessThan(QGraphicsItem* item1, QGraphicsItem* item2)
{
  return item1->zValue() < item2->zValue();
}

void te::layout::Scene::addChangePropertiesCommandToStack(QList<QGraphicsItem*> items, const Properties& properties)
{
  if (items.count() > 1)
  {
    m_undoStack->beginMacro("Block: Change Items Properties"); // begin only one block of commands on stack
  }

  foreach(QGraphicsItem* item, items)
  {
    if (item)
    {
      AbstractItemView* lItem = dynamic_cast<AbstractItemView*>(item);
      if (lItem)
      {
        /* Each item, at the end of the setProperties method (AbstractItemView), 
        adds a Property Change command to the Undo/Redo stack, via the scene. */
        lItem->setProperties(properties);
      }
    }
  }

  if (items.count() > 1)
  {
    m_undoStack->endMacro(); // end only one block of commands on stack
  }
}

void te::layout::Scene::addChangePropertiesCommandToStack(const std::map<QGraphicsItem*, te::layout::Properties>& map)
{
  if (map.size() > 1)
  {
    m_undoStack->beginMacro("Block: Change Items Properties"); // begin only one block of commands on stack
  }

  std::map<QGraphicsItem*, te::layout::Properties>::const_iterator it;

  for (it = map.begin(); it != map.end(); ++it)
  {
    QGraphicsItem* item = it->first;
    Properties properties = it->second;
    if (item)
    {
      AbstractItemView* lItem = dynamic_cast<AbstractItemView*>(item);
      if (lItem)
      {
        /* Each item, at the end of the setProperties method (AbstractItemView),
        adds a Property Change command to the Undo/Redo stack, via the scene. */
        lItem->setProperties(properties);
      }
    }
  }

  if (map.size() > 1)
  {
    m_undoStack->endMacro(); // end only one block of commands on stack
  }
}

te::layout::ItemInputProxy* te::layout::Scene::getInputItemProxy()
{
  return m_itemInputProxy;
}

void te::layout::Scene::changeUndoEnable(const QList<QGraphicsItem *> & listItems, bool enable)
{
  foreach(QGraphicsItem* item, listItems)
  {
    if (item)
    {
      AbstractItemView* iView = dynamic_cast<AbstractItemView*>(item);
      if (iView)
      {
        iView->setUndoEnabled(enable);
      }
    }
  }
}

void te::layout::Scene::destroyItemsWithoutScene()
{
  foreach(QGraphicsItem *item, m_itemStackWithoutScene)
  {
    if (item->scene() != this)
    {
      ItemGroup* group = dynamic_cast<ItemGroup*>(item);
      if (group)
      {
        QList<QGraphicsItem*> childItems = item->childItems();
        removeItemGroup(group); // Remove children from group
        foreach(QGraphicsItem* child, childItems)
        {
          if ((!m_itemStackWithoutScene.contains(child)) && (child->scene() != this))
          {
            delete child;
            child = 0;
          }
        }
      }
      delete item;
      item = 0;
    }
  }
}
