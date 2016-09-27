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
  \file BuildGraphicsItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildGraphicsItem.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"


#include "../../core/pattern/singleton/Context.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "pattern/factory/item/ItemFactory.h"
#include "pattern/command/AddCommand.h"
#include "Scene.h"
#include "ItemUtils.h"
#include "../item/ImageItem.h"
#include "View.h"

// STL
#include <sstream>
#include <string>

// Qt
#include <QGraphicsItem>
#include <QUndoCommand> 

te::layout::BuildGraphicsItem::BuildGraphicsItem(Scene* scene, QObject* parent) :
  QObject(parent),
  m_scene(scene)
{
  if (m_scene)
  {
    View* view = m_scene->getView();
    view->connect(this, SIGNAL(showDialogWindow(EnumType*, QList<QGraphicsItem*>)), view, SLOT(onShowDialogWindow(EnumType*, QList<QGraphicsItem*>)));
  }
}

te::layout::BuildGraphicsItem::~BuildGraphicsItem()
{
 
}

QGraphicsItem* te::layout::BuildGraphicsItem::buildItem(te::layout::Properties props, bool addUndo, bool isCopy)
{
  QGraphicsItem* item = 0;

  if(props.getProperties().empty())
    return item;

  clear();

  m_props = props;
  m_coord = findCoordinate(props);
  m_width = findWidth(props);
  m_height = findHeight(props);
  
  EnumType* type = props.getTypeObj();

  item = createItem(type, addUndo, isCopy);

  clear();
  
  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItem(te::layout::EnumType* itemType, const te::gm::Coord2D& coordinate, 
  double width, double height, bool addUndo)
{
  QGraphicsItem* item = 0;

  clear();

  m_coord = coordinate;
  m_width = width;
  m_height = height;

  item = createItem(itemType, addUndo);

  clear();

  return item;
}

QGraphicsItem* te::layout::BuildGraphicsItem::createItem(te::layout::EnumType* itemType, bool addUndo, bool isCopy)
{
  QGraphicsItem* item = 0;

  if (!itemType)
  {
    return item;
  }

  ItemFactoryParamsCreate params = createParams(itemType, isCopy);

  std::string name = itemType->getName();

  if (isCopy == true)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property idProp = m_props.getProperty("id");

    idProp.setValue(m_id, dataType->getDataTypeInt());
    m_props.updateProperty(idProp);

    Property nameProp = m_props.getProperty("name");
    nameProp.setValue(nameItem(itemType), dataType->getDataTypeString());
    m_props.updateProperty(nameProp);

  }

  AbstractItemView* abstractItem = te::layout::ItemFactory::make(name, params);
  item = dynamic_cast<QGraphicsItem*>(abstractItem);

  if (!item)
  {
    return item;
  }

  if (m_scene)
  {
    m_scene->insertItem(item);
  }

  if (m_props.getProperties().empty() == false)
  { 
    abstractItem->getController()->setProperties(m_props);
  }
  
  afterBuild(item, addUndo);

  showImgDlg(item);

  return item;
}

std::string te::layout::BuildGraphicsItem::nameItem( te::layout::EnumType* type )
{
  std::string name = "";
  
  QList<QGraphicsItem*> graphicsItems;
  std::stringstream ss;//create a stringstream

  m_id = -1;
  if (m_scene)
  {
    graphicsItems = m_scene->items();

    ItemUtils iUtils = m_scene->getItemUtils();
    m_id = iUtils.maxTypeId(type);
  }  

  if(m_id < 0)
    m_id = 0;
  else
    m_id+= 1;

  ss << m_id;

  name = type->getName();
  name+="_";
  name+=ss.str();

  return name;
}

void te::layout::BuildGraphicsItem::afterBuild(QGraphicsItem* item, bool addUndo)
{
  if (!item)
  {
    return;
  }

  QPointF pointInSceneCS(m_coord.x, m_coord.y);

  // tool for create items could be with size
  if (m_width == 0 && m_height == 0)
  {
    double width = item->boundingRect().width();
    double height = item->boundingRect().height();

    QPointF pointInItemCS = item->mapFromScene(pointInSceneCS);
    pointInItemCS.setX(pointInItemCS.x() - (width / 2.));
    pointInItemCS.setY(pointInItemCS.y() - (height / 2.));
    pointInSceneCS = item->mapToScene(pointInItemCS);
  }

  item->setPos(pointInSceneCS);

  if (!m_props.getProperties().empty())
  {
    int zValue = findZValue(m_props);
    if (zValue > -1)
    {
      item->setZValue(zValue);
    }
  }

  if (item)
  {
    if (m_scene)
    {
      if (addUndo)
      {
        QUndoCommand* command = new AddCommand(item);
        m_scene->addUndoStack(command);
      }
    }
  }
}

te::layout::ItemFactoryParamsCreate te::layout::BuildGraphicsItem::createParams(te::layout::EnumType* type, bool isCopy)
{
  std::string strName = nameItem(type);

  if (isCopy == true)
  {
    m_name = strName;
    return ItemFactoryParamsCreate(strName, m_id, m_coord, m_width, m_height);
  }

  if (m_props.getProperties().empty())
  {
    m_name = strName;
    return ItemFactoryParamsCreate(strName, m_id, m_coord, m_width, m_height);
  }

  std::string name = findName(m_props);
  if (name.empty())
  {
    m_name = strName;
    Properties pEmpty;
    return ItemFactoryParamsCreate(strName, m_coord, pEmpty);
  }  

  Properties pEmpty;
  m_name = name;
  return ItemFactoryParamsCreate(name, m_id, m_coord, m_width, m_height);
}

void te::layout::BuildGraphicsItem::showImgDlg(QGraphicsItem* item)
{
  te::layout::AbstractItem* abstractItem = dynamic_cast<te::layout::AbstractItem*> (item);
  if (abstractItem == NULL)
  {
    return;
  }
  EnumDataType* enumData = Enums::getInstance().getEnumDataType();
  te::layout::EnumObjectType* enumObj = te::layout::Enums::getInstance().getEnumObjectType();
  te::layout::EnumType* enumType = abstractItem->getController()->getProperties().getTypeObj();
  if (enumType == enumObj->getImageItem())
  {
    ImageItem* imageItem = dynamic_cast<ImageItem*> (item);
    if (imageItem)
    {
      Property prop = abstractItem->getController()->getProperty("file_name");

      if (prop.getValue()->toString().compare("") != 0)
        return;

      QList<QGraphicsItem*> imageItemList;
      imageItemList.append(item);

      View* view = m_scene->getView();
      if (view)
      {
        // call file dialog window
        emit showDialogWindow(enumData->getDataTypeImage(), imageItemList);
      }
    }
  }
}


std::string te::layout::BuildGraphicsItem::getNewName()
{
  return m_name;
}