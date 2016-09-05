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
  \file View.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "View.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/enum/Enums.h"
#include "terralib/geometry/Envelope.h"
#include "VisualizationArea.h"
#include "../item/ItemGroup.h"
#include "tools/ZoomClickTool.h"
#include "HorizontalRuler.h"
#include "VerticalRuler.h"
#include "PrintScene.h"
#include "../../core/enum/EnumTemplateType.h"
#include "ItemUtils.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "pattern/factory/tool/ToolFactoryParamsCreate.h"
#include "pattern/factory/tool/ToolFactory.h"
#include "../../core/ContextObject.h"
#include "Scene.h"
#include "BuildGraphicsOutside.h"
#include "../inside/ToolbarItemInside.h"
#include "../inside/DialogItemToolbar.h"
#include "../../core/property/SharedProperties.h"
#include "tempDataStorage/TempDataStorageEditor.h"
#include "tempDataStorage/TempFileInfo.h"

// Qt
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QGraphicsItem>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainterPath>
#include <QEvent>
#include <QToolBar>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QScrollBar>
#include <QLayout>
#include <QDir>
#include <QString>

// STL
#include <memory>

te::layout::View::View( QWidget* widget) : 
  QGraphicsView(new QGraphicsScene, widget),
  m_visualizationArea(0),
  m_currentTool(0),
  m_pageSetupOutside(0),
  m_systematicOutside(0),
  m_selectionChange(false),
  m_width(-1),
  m_height(-1),
  m_isMoving(false),
  m_movingItemGroup(0),
  m_updateItemPos(false),
  m_mouseEvent(false),
  m_dialogItemToolbar(0),
  m_currentToolbarInsideType(0),
  m_midButtonClicked(false),
  m_showContextMenu(true),
  m_clipboard(QApplication::clipboard()),
  m_cutObject(false),
  m_tempDataStorageEditor(0),
  m_fullTempPath(""),
  m_tempFileName("~layoutTemp.xml")
{
  setDragMode(RubberBandDrag);

  m_horizontalRuler = new HorizontalRuler;
  m_verticalRuler = new VerticalRuler;

  m_dialogItemToolbar = new DialogItemToolbar(this->viewport());
}

te::layout::View::~View()
{
  QList<ToolbarItemInside*> toolbars = m_itemToolbars.values();
  foreach(ToolbarItemInside *inside, toolbars)
  {
    if (inside)
    {
      delete inside;
      inside = 0;
    }
  }

  m_itemToolbars.clear();

  if (m_currentTool)
  {
    viewport()->removeEventFilter(m_currentTool);
    delete m_currentTool;
    m_currentTool = 0;
  }

  for (size_t i = 0; i < m_lateRemovalVec.size(); ++i)
  {
    delete m_lateRemovalVec[i];
  }
  m_lateRemovalVec.clear();

  if(m_visualizationArea)
  {
    delete m_visualizationArea;
    m_visualizationArea = 0;
  }

  if(m_pageSetupOutside)
  {
    delete m_pageSetupOutside;
    m_pageSetupOutside = 0;
  }

  if(m_systematicOutside)
  {
    delete m_systematicOutside;
    m_systematicOutside = 0;
  }

  if(m_verticalRuler)
  {
    delete m_verticalRuler;
    m_verticalRuler = 0;
  }

  if(m_horizontalRuler)
  {
    delete m_horizontalRuler;
    m_horizontalRuler = 0;
  }
  
  if (m_tempDataStorageEditor)
  {
    delete m_tempDataStorageEditor;
    m_tempDataStorageEditor = 0;
  }

  m_clipboard->clear();
}

void te::layout::View::mousePressEvent( QMouseEvent * event )
{
  m_mouseEvent = true;

  Scene* sc = dynamic_cast<Scene*>(scene());
  if (!sc)
    return;

  if (event->button() == Qt::MidButton && !sc->isEditionMode()) // Pan
  {
    m_midButtonClicked = true;
    pan();
    /* The pan is made by default with the left mouse button (QGraphicsView), 
    so we need to resubmit the event, as if this button had been clicked */
    QMouseEvent resubmit(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
    QGraphicsView::mousePressEvent(&resubmit);
  }
  else
  {
    QGraphicsView::mousePressEvent(event);
  }

  QPointF scenePos = mapToScene(event->pos());
  te::gm::Coord2D coord(scenePos.x(), scenePos.y());

  if (m_isMoving == false)
  {
    QList<QGraphicsItem*> selectedItems = sc->selectedItems();

    if (selectedItems.size() > 1)
    {
      bool isInvertedMatrix = false;
      foreach(QGraphicsItem* item, selectedItems)
      {
        AbstractItemView* observer = dynamic_cast<AbstractItemView*> (item);
        if (observer)
        {
          if (observer->isInverted() == true)
          {
            isInvertedMatrix = true;
          }
        }
      }

      if (isInvertedMatrix == true)
      {
        m_movingItemGroup = sc->createMovingItemGroup(selectedItems);
        m_isMoving = true;
      }
    }
  }
}

void te::layout::View::mouseMoveEvent( QMouseEvent * event )
{
  m_mouseEvent = true;

  Scene* sc = dynamic_cast<Scene*>(scene());

  if (!sc)
    return;

  if (event->button() == Qt::MidButton && !sc->isEditionMode()) // Pan
  {
    /* The pan is made by default with the left mouse button (QGraphicsView),
    so we need to resubmit the event, as if this button had been clicked */
    QMouseEvent resubmit(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
    QGraphicsView::mouseMoveEvent(&resubmit);
  }
  else
  {
    QGraphicsView::mouseMoveEvent(event);
  }

  if(!scene()->selectedItems().empty())
  {
    m_updateItemPos = true;
  }
  else
  {
    m_updateItemPos = false;
  }

  QPointF pt = mapToScene(event->pos());
   
  emit changeSceneCoordMouse(pt);
}

void te::layout::View::mouseReleaseEvent( QMouseEvent * event )
{
  m_mouseEvent = false;

  Scene* sc = dynamic_cast<Scene*>(scene());

  if (!sc)
    return;
  
  if (event->button() == Qt::MidButton && !sc->isEditionMode()) // Pan
  {
    /* The pan is made by default with the left mouse button (QGraphicsView),
    so we need to resubmit the event, as if this button had been clicked */
    QMouseEvent resubmit(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
    QGraphicsView::mouseReleaseEvent(&resubmit);
  }
  else
  {
    QGraphicsView::mouseReleaseEvent(event);
  }

  if (m_isMoving == true)
  {
    QList<QGraphicsItem*> selectedItems = m_movingItemGroup->childItems();

    sc->destroyItemGroup(m_movingItemGroup);
    m_movingItemGroup = 0;
    m_isMoving = false;

    sc->clearSelection();

    foreach (QGraphicsItem* item, selectedItems)
    {
      if (item->isSelected())
      {
        item->setSelected(false);
      }
    }

    sc->selectItems(selectedItems);
  }
  
  if (m_midButtonClicked)
  {
    m_midButtonClicked = false;
    resetDefaultConfig();
  }

  /* The Properties only load when selection change and mouse release */
  if(!m_selectionChange && !m_updateItemPos)
    return;

  if (!sc->isEditionMode()) // If scene in edition mode the reload will happen in double click event
  {
    if (m_updateItemPos && !m_selectionChange)
      reload();
    else
      load();
  }
}

void te::layout::View::mouseDoubleClickEvent(QMouseEvent * event)
{
  QGraphicsView::mouseDoubleClickEvent(event);

  Scene* sc = dynamic_cast<Scene*>(scene());
  if (!sc)
    return;

  if (sc->isEditionMode()) // If scene in edition mode the reload will happen in double click event
  {
    reload();
  }
}

void te::layout::View::wheelEvent(QWheelEvent *event)
{  
  Scene* scne = dynamic_cast<Scene*>(scene());
  if (!scne)
  {
    return;
  }

  /*In edit mode not apply the zoom, 
    but part of the current item characteristics for this event.
    Example: Zoom on the map.*/
  if (scne->isEditionMode())
  {
    QGraphicsView::wheelEvent(event);
    return;
  }

  ViewportUpdateMode mode = viewportUpdateMode();
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  int zoom = 0; 

  // Zoom in / Zoom Out with mouse scroll
  if (event->delta() > 0)
  {
    //Zooming In
    zoom = nextZoom();
  }
  else
  {
    zoom = previousZoom();
  }

  setZoom(zoom);

  if (isLimitExceeded(zoom) == false)
  {
    QGraphicsView::wheelEvent(event);
  }
  
  setViewportUpdateMode(mode);
}

void te::layout::View::keyPressEvent( QKeyEvent* keyEvent )
{
  Scene* scne = dynamic_cast<Scene*>(scene());

  EnumModeType* mode = Enums::getInstance().getEnumModeType();

  if((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->key() == Qt::Key_P))
  {
    print();
  }
  else if((keyEvent->modifiers() & Qt::AltModifier) && (keyEvent->key() == Qt::Key_0))
  {
    pan();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_R))
  {
    recompose();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_A))
  {
    zoomArea();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_O))
  {
    zoomOut();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_G))
  {
    createItemGroup();
  }
  else if((keyEvent->modifiers() == Qt::AltModifier) & (keyEvent->key() == Qt::Key_U))
  {
    destroyItemGroup();
  }
  else if(keyEvent->key() == Qt::Key_Delete)
  {
    if(getCurrentMode() != mode->getModeTextEditorInteraction())
    {
      if (scne->isEditionMode() == false)
      {
        scne->removeSelectedItems();
      }
    }
  }
  else if(keyEvent->key() == Qt::Key_Escape)
  {
    if (m_currentTool != NULL)
    {
      m_currentTool->keyPressEvent(keyEvent);
    }
  }
  else if ((keyEvent->modifiers() == Qt::ControlModifier) & (keyEvent->key() == Qt::Key_C))
  {
    copyToClipboard();
  }
  else if ((keyEvent->modifiers() == Qt::ControlModifier) & (keyEvent->key() == Qt::Key_V))
  {
    paste();
  }
  else if ((keyEvent->modifiers() == Qt::ControlModifier) & (keyEvent->key() == Qt::Key_X))
  {
    cutSelectedItens();
  }
  QGraphicsView::keyPressEvent(keyEvent);
}

void te::layout::View::copyToClipboard()
{
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();
  std::vector<Properties> propertiesList;
 

  foreach(QGraphicsItem* item, graphicsItems)
  {
    AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
    Properties itemProperties = view->getController()->getProperties();
    propertiesList.push_back(itemProperties);

  }

  QMimeData* data = convert2MimeData(propertiesList);

  m_clipboard->setMimeData(data);

  m_cutObject = false;
 
}
void te::layout::View::removeSelectedItem()
{
  Scene* scne = dynamic_cast<Scene*>(scene());

  EnumModeType* mode = Enums::getInstance().getEnumModeType();
  if (getCurrentMode() != mode->getModeTextEditorInteraction())
  {
    if (scne->isEditionMode() == false)
    {
      scne->removeSelectedItems();
    }
  }
}

void te::layout::View::paste()
{
  const QMimeData *clipboardData = m_clipboard->mimeData();

  if (!clipboardData->hasFormat("application/layout-item"))
  {
    return;
  }

  QByteArray arrayData = clipboardData->data("application/layout-item");

  qulonglong dataValue = arrayData.toULongLong();

  Scene* nscene = dynamic_cast<Scene*>(scene());

  if (!nscene)
    return;

  std::auto_ptr< std::vector<Properties> > changed(reinterpret_cast<std::vector<Properties>* >(dataValue));

  std::map<std::string, std::string> newNames;
  std::map<int, int> newIds;

  std::vector<Properties> p = *changed.get();

  std::vector<Properties> mapItens;
  std::vector<Properties> otherItens;

  te::layout::EnumType* mapCompositionType = Enums::getInstance().getEnumObjectType()->getMapCompositionItem();

  for (int i = 0; i < p.size(); i++){

    Properties prop;
    prop = p.at(i);

    if (prop.getTypeObj() == mapCompositionType)
    {
      mapItens.push_back(prop);
    }
    else
    {
      otherItens.push_back(prop);
    }

    
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property pX = prop.getProperty("x");
    Property pY = prop.getProperty("y");

    double x = pX.getValue().toDouble();
    double y = pY.getValue().toDouble();

    Variant varX;
    Variant varY;

    varX.setValue(x + 20.0, dataType->getDataTypeDouble());
    pX.setValue(varX);
    prop.updateProperty(pX);

    varY.setValue(y - 20.0, dataType->getDataTypeDouble());
    pY.setValue(varY);
    prop.updateProperty(pY);


  }

  for (int i = 0; i < mapItens.size(); i++)
  {
    Properties prop;
    prop = mapItens.at(i);
    Property pro_name = prop.getProperty("name");
    Property pro_id = prop.getProperty("id");

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    if (m_cutObject == false)
    {

      Property pX = prop.getProperty("x");
      Property pY = prop.getProperty("y");

      double x = pX.getValue().toDouble();
      double y = pY.getValue().toDouble();

      Variant varX;
      Variant varY;

      varX.setValue(x + 20.0, dataType->getDataTypeDouble());
      pX.setValue(varX);
      prop.updateProperty(pX);

      varY.setValue(y - 20.0, dataType->getDataTypeDouble());
      pY.setValue(varY);
      prop.updateProperty(pY);

    }


    SharedProperties sharedProps;

    std::string oldName;
    int oldId;

    oldName = pro_name.getValue().toString();
    oldId = pro_id.getValue().toInt();

    Variant var;
    pro_name.setValue(var);
    prop.updateProperty(pro_name);
    std::string newName;

    nscene->buildItem(prop, newName, true);

    newNames[oldName] = newName;
  }


  for (int i = 0; i < otherItens.size(); i++)
  {
    Properties prop;
    prop = otherItens.at(i);
    Property pro_name = prop.getProperty("name");
    Property pro_id = prop.getProperty("id");


    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    if (m_cutObject == false)
    {
      Property pX = prop.getProperty("x");
      Property pY = prop.getProperty("y");

      double x = pX.getValue().toDouble();
      double y = pY.getValue().toDouble();

      Variant varX;
      Variant varY;

      varX.setValue(x + 5.0, dataType->getDataTypeDouble());
      pX.setValue(varX);
      prop.updateProperty(pX);

      varY.setValue(y - 5.0, dataType->getDataTypeDouble());
      pY.setValue(varY);
      prop.updateProperty(pY);

    }
    

    SharedProperties sharedProps;
    Property connect = prop.getProperty(sharedProps.getItemObserver());

    Variant varConnect;
    std::string currentNameConnect = connect.getValue().toString();
    currentNameConnect = currentNameConnect.substr(0, currentNameConnect.find("_Map"));
    std::string nameConnect = newNames[currentNameConnect];
    varConnect.setValue(nameConnect + "_Map", dataType->getDataTypeItemObserver());
    connect.setValue(varConnect);
    prop.updateProperty(connect);

    std::string oldName;
    int oldId;

    oldName = pro_name.getValue().toString();
    oldId = pro_id.getValue().toInt();

    Variant var;
    pro_name.setValue(var);
    prop.updateProperty(pro_name);
    std::string newName;

    nscene->buildItem(prop, newName, true);

  }
  if (m_cutObject ==  false)
  {

    QMimeData* data = convert2MimeData(p);
    m_clipboard->setMimeData(data);
  
  }
  else
  {
    m_clipboard->setText("");
  }

}

void te::layout::View::cutSelectedItens()
{
  copyToClipboard();

  Scene* scne = dynamic_cast<Scene*>(scene());
  
  if (scne->isEditionMode() == false)
  {
    scne->removeSelectedItems();
  }
  else
  {
    return;
  }

  m_cutObject = true;

}

QMimeData* te::layout::View::convert2MimeData(const  std::vector<Properties>& properties)
{
  QMimeData *mimeData = new QMimeData();
  std::vector<Properties>* aux = new std::vector<Properties>();

  foreach(Properties prop, properties)
  {
    aux->push_back(prop);
  }

  qulonglong longValue = (qulonglong)aux;

  QString s;
  s.setNum((qulonglong)aux);

  QByteArray encodedData(s.toStdString().c_str());

  mimeData->setData("application/layout-item", encodedData);

  return mimeData;

}

void te::layout::View::config()
{  
  //Calculate matrix and centralizes the scene

  Scene* nscene = dynamic_cast<Scene*>(scene());

  if(!nscene)
    return;
    
  double sw = viewport()->widthMM();
  double sh = viewport()->heightMM();

  if(m_width == -1 || m_height == -1)
  {
    m_width = sw;
    m_height = sh;
  }

  nscene->init(m_width, m_height, getContext());

  QTransform mtrx = nscene->sceneTransform();

  setTransform(mtrx);

  te::gm::Envelope box = nscene->getSceneBox();
  centerOn(QPointF(box.m_llx, box.m_ury));
      
  int zoom = getDefaultZoom();
  double newScale = zoom / 100.;
  scale(newScale, newScale); //Initial zoom out
  setZoom(zoom);

  //----------------------------------------------------------------------------------------------
  if(!m_visualizationArea)
  {
    m_visualizationArea = new VisualizationArea(nscene, box);
  }

  if(!getCurrentMode())
  {
    if(Enums::getInstance().getEnumModeType())
    {
      setCurrentMode(Enums::getInstance().getEnumModeType()->getModeNone());
    }
  }
    
  connect(scene(), SIGNAL(editionFinalized()), this, SLOT(onEditionFinalized()));

  //scrollbars
  connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onScrollBarValueChanged(int)));
  connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(onScrollBarValueChanged(int)));
}

void te::layout::View::resizeEvent(QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event);
  m_foreground = QPixmap();
}

void te::layout::View::onToolbarChangeMode( te::layout::EnumType* newMode )
{
  changeMode(newMode);
}

void te::layout::View::createItemGroup()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  if (graphicsItems.isEmpty())
  {
    return;
  }

  if(sc)
  {
    QGraphicsItemGroup* group = sc->createItemGroup(graphicsItems);

    if(!group)
      return;
          
    ItemGroup* layoutGroup = dynamic_cast<ItemGroup*>(group);
      
    if(!layoutGroup)
      return;

    group->setSelected(true);
    reload(); // load item gro1up properties
  }
}

void te::layout::View::destroyItemGroup()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  QList<QGraphicsItem*> graphicsItems = this->scene()->selectedItems();

  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if (item)
    {
      QGraphicsItemGroup* group = dynamic_cast<QGraphicsItemGroup*>(item);
      if(group)
      {
        if(sc)
        {
          sc->destroyItemGroup(group);
        }
      }
    }
  }
}

void te::layout::View::resetDefaultConfig(bool toolLateRemoval)
{
  //Use ScrollHand Drag Mode to enable Panning
  //You do need the enable scroll bars for that to work.
  setDragMode(RubberBandDrag);

  //Whole view not interactive while in ScrollHandDrag Mode
  setInteractive(true);
  setCursor(Qt::ArrowCursor);
  setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

  for (size_t i = 0; i < m_lateRemovalVec.size(); ++i)
  {
    delete m_lateRemovalVec[i];
  }
  m_lateRemovalVec.clear();

  if(m_currentTool)
  {
    viewport()->removeEventFilter(m_currentTool);
    
    if(toolLateRemoval == true)
    {
      m_currentTool->setParent(0);
      m_lateRemovalVec.push_back(m_currentTool);
      m_currentTool = 0;
    }
    else
    {
      delete m_currentTool;
      m_currentTool = 0;
    }    
  }
}

void te::layout::View::onMainMenuChangeMode( te::layout::EnumType* newMode )
{
  changeMode(newMode);
}

void te::layout::View::changeMode( EnumType* newMode )
{
  if(newMode == getCurrentMode())
  {
    return;
  }

  setCurrentMode(newMode);

  resetDefaultConfig();

  Scene* sc = dynamic_cast<Scene*>(scene());

  if(!sc)
    return;

  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();
  ItemUtils iUtils = sc->getItemUtils();

  EnumType* mode = getCurrentMode();
  
  if(mode == enumMode->getModeMapPan())
  {
    iUtils.setCurrentToolInSelectedMapItems(enumMode->getModeMapPan());
  }
  else if(mode == enumMode->getModeMapZoomIn())
  {
    iUtils.setCurrentToolInSelectedMapItems(enumMode->getModeMapZoomIn());
  }
  else if(mode == enumMode->getModeMapZoomOut()) 
  {
    iUtils.setCurrentToolInSelectedMapItems(enumMode->getModeMapZoomOut());
  }

  sc->setContext(getContext());

  emit changeContext();
}

void te::layout::View::hideEvent( QHideEvent * event )
{
  QGraphicsView::hideEvent(event);
  if (m_tempDataStorageEditor)
  {
    m_tempDataStorageEditor->stop();
  }
  emit hideView();
}

void te::layout::View::closeEvent( QCloseEvent * event )
{
  closeToolbar();
  
  QGraphicsView::closeEvent(event);
  emit closeView();
}

void te::layout::View::showEvent( QShowEvent * event )
{
  QGraphicsView::showEvent(event);
  emit showView();
}

void te::layout::View::showPageSetup()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  if (!sc)
    return;

  if(!m_pageSetupOutside)
  {
    BuildGraphicsOutside build;
    EnumObjectType* type = Enums::getInstance().getEnumObjectType();
    QWidget* outside = build.createOutside(type->getPageSetup(), sc, (QWidget*) this->parent());
    m_pageSetupOutside = dynamic_cast<PageSetupOutside*>(outside);
    connect(m_pageSetupOutside, SIGNAL(changeConfig()), this, SLOT(onChangeConfig()));
  }
  m_pageSetupOutside->load();
  m_pageSetupOutside->show();
}

void te::layout::View::onChangeConfig()
{ 
  Scene* sc = dynamic_cast<Scene*>(scene());
  if(!sc)
    return;

  QGraphicsItem* oldItem = sc->getPaperItem();
  QRectF oldBoundingRect = oldItem->boundingRect();

  QSize oldSize(oldBoundingRect.width(), oldBoundingRect.height());

  sc->deletePaperItem();

  config();

  te::gm::Envelope boxW = sc->getSceneBox();
  m_visualizationArea->changeBoxArea(boxW);

  m_visualizationArea->build();

  QGraphicsItem* newItem = sc->getPaperItem();
  QRectF newBoundingRect = newItem->boundingRect();

  QSize newSize(newBoundingRect.width(), newBoundingRect.height());
    
  sc->applyPaperProportion(oldSize, newSize);

  recompose();
}

void te::layout::View::showSystematicScale()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  if (!sc)
    return;

  if(!m_systematicOutside)
  {
    BuildGraphicsOutside build;
    EnumObjectType* type = Enums::getInstance().getEnumObjectType();
    QWidget* outside = build.createOutside(type->getSystematicScale(), sc, (QWidget*) this->parent());
    m_systematicOutside = dynamic_cast<SystematicScaleOutside*>(outside);
    connect(m_systematicOutside, SIGNAL(systematicApply(double,SystematicScaleType)), this, SLOT(onSystematicApply(double,SystematicScaleType)));
  }

  m_systematicOutside->show();
}

void te::layout::View::closeOutsideWindows()
{
  if(m_pageSetupOutside)
  {
    m_pageSetupOutside->close();
  }
}

void te::layout::View::onSystematicApply(double scale, SystematicScaleType type)
{
 
}

void te::layout::View::contextMenuEvent( QContextMenuEvent * event )
{
  if (!m_showContextMenu)
  {
    m_showContextMenu = true;
    return;
  }

  if(event->reason() != QContextMenuEvent::Mouse)
    return;

  QPointF pt = mapToScene(event->pos());
  QPointF ptGlobal = event->globalPos();
  emit showContextMenu(pt, ptGlobal);
}

QImage te::layout::View::createImage()
{
  QImage ig;
  Scene* sc = dynamic_cast<Scene*>(scene());
  if(!sc)
    return ig;

  te::gm::Envelope env = sc->getSceneBox();

  QRectF rtv(0, 0, width(), height());
  QRectF rts(env.m_llx, env.m_lly, env.m_urx, env.m_ury);

  QImage img(rtv.width(), rtv.height(), QImage::Format_ARGB32);
  QPainter ptr(&img);
  ptr.setRenderHint(QPainter::Antialiasing);
  this->render(&ptr, QRect(), QRect(), Qt::IgnoreAspectRatio);

  return img;
}

QCursor te::layout::View::createCursor( std::string pathIcon )
{
  QString qPathIcon = ItemUtils::convert2QString(pathIcon);

  QIcon ico(QIcon::fromTheme(qPathIcon));

  //search icon size
  QList<QSize> sizes = ico.availableSizes();
  int maximum = sizes[0].width();
  for (int i = 1; i < sizes.size(); ++i)
  {
    maximum = qMax(maximum, sizes[i].width());
  }

  QSize sz(maximum, maximum);
  QPixmap pixmap = ico.pixmap(sz);

  QCursor cur(pixmap);

  return cur;
}

void te::layout::View::reload()
{
  m_selectionChange = false;
  emit reloadProperties();
}

void te::layout::View::load()
{
  m_selectionChange = false;
  emit loadProperties();
}

void te::layout::View::resetView()
{
  Scene* scne = dynamic_cast<Scene*>(scene());

  if(!scne)
    return;

  QTransform mtrx = scne->sceneTransform();
  setTransform(mtrx);

  te::gm::Envelope box = scne->getSceneBox();
  QPointF pt(box.m_llx, box.m_ury);
  centerOn(pt);

  int zoom = getDefaultZoom();
  double zoomFactor = zoom / 100.;
  scale(zoomFactor, zoomFactor); //Initial zoom out
}

void te::layout::View::pan()
{
  //Use ScrollHand Drag Mode to enable Panning
  resetDefaultConfig();

  m_foreground = QPixmap();

  //The entire viewport is redrawn to avoid traces
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

  setInteractive(false);
  setDragMode(QGraphicsView::ScrollHandDrag); // Pan Mode
}

void te::layout::View::zoomArea()
{
  resetDefaultConfig();
   
  EnumToolType* tools = Enums::getInstance().getEnumToolType();

  std::string toolName = tools->getZoomAreaTool()->getName();
  ToolFactoryParamsCreate params(this);

  m_currentTool = te::layout::ToolFactory::make(toolName, params);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::zoomOut()
{
  resetDefaultConfig();

  // #Active ZoomClick (Out) Tool

  QCursor curOut = createCursor("layout-paper-zoom-out");
  m_currentTool = new ZoomClickTool(this, curOut);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::print()
{
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();

  Scene* scne = dynamic_cast<Scene*>(scene());
  if(!scne)
  {
    return;
  }

  emit aboutToPerformIO();

  setCurrentMode(enumMode->getModePrinter());

  resetDefaultConfig();
  
  // No update Widget while print is running
  // Rulers aren't print
  disableUpdate();
  
  PrintScene printer(scne);
  //printer.showPrintPreviewDialog();
  printer.showPrintDialog();

  enableUpdate();

  setCurrentMode(enumMode->getModeNone());

  emit endedPerformingIO();
}

void te::layout::View::exportToPDF()
{
  emit aboutToPerformIO();

  Scene* scne = dynamic_cast<Scene*>(scene());

  resetDefaultConfig();

  // No update Widget while print is running
  // Rulers aren't print
  disableUpdate();

  ContextObject oldContext = scne->getContext();

  PrintScene printer(scne);
  printer.exportToPDF();

  scne->setContext(oldContext);

  enableUpdate();

  emit endedPerformingIO();
}

void te::layout::View::recompose()
{
  resetDefaultConfig();

  QRectF sceneRectV = sceneRect();

  fitZoom(sceneRectV);

  int zoom = getCurrentZoom();

  setZoom(zoom*0.95);
}

void te::layout::View::arrowCursor()
{
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();
  ItemUtils iUtils = ((Scene*) scene())->getItemUtils();

  setCurrentMode(enumMode->getModeNone());
  resetDefaultConfig();
}

void te::layout::View::newTemplate()
{
  Scene* sc = dynamic_cast<Scene*>(scene());
  sc->reset();
  m_visualizationArea->build();
}

void te::layout::View::fitZoom(const QRectF& rect)
{
  //clears the foreground pixmap
  m_foreground = QPixmap();

  double scaleOld = this->transform().m11();
  this->fitInView(rect, Qt::KeepAspectRatio);
  double scaleNew = this->transform().m11();

  double scaleFactor = scaleNew / scaleOld;

  int currentZoom = getCurrentZoom();
  int newZoom = (int)(currentZoom * scaleFactor);

  if(newZoom > 0)
  {
    setCurrentZoom(newZoom);

    if (isLimitExceeded(newZoom) == true)
    {
      setZoom(this->getMaxZoomLimit());
      return;
    }

    Scene* sce = dynamic_cast<Scene*>(scene());
    if(sce)
    {
      sce->setContext(getContext());
    }

    emit zoomChanged(newZoom);
  }
}

void te::layout::View::setZoom(int newZoom)
{
  //clears the foreground pixmap
  m_foreground = QPixmap();

  int currentZoom = getCurrentZoom();

  if(newZoom == currentZoom)
    return;

  if(isLimitExceeded(newZoom) == true)
    return;

  Scene* scne = dynamic_cast<Scene*>(scene());

  double currentHorizontalScale = this->viewportTransform().m11();
  double currentVerticalScale = this->viewportTransform().m22();


  double origHorizontalScale = scne->sceneTransform().m11();
  double origVerticalScale = scne->sceneTransform().m22();


  double hZoomScale = origHorizontalScale / (100./newZoom);
  double vZoomScale = origVerticalScale / (100./newZoom);

  double zoomScaleFactorH = hZoomScale / currentHorizontalScale;
  double zoomScaleFactorV = vZoomScale / currentVerticalScale;


  if((zoomScaleFactorH > 0)
      && (zoomScaleFactorV > 0))
  {
    setCurrentZoom(newZoom);

    applyScale(zoomScaleFactorH, zoomScaleFactorV);

    Scene* sce = dynamic_cast<Scene*>(scene());
    if(sce)
    {
      sce->setContext(getContext());
    }

    this->update();

    emit zoomChanged(newZoom);
  }
}

void te::layout::View::createLineItem()
{
  resetDefaultConfig();
  m_showContextMenu = false;

  EnumToolType* tools = Enums::getInstance().getEnumToolType();

  std::string toolName = tools->getCreateLineItemTool()->getName();
  ToolFactoryParamsCreate params(this);

  m_currentTool = te::layout::ToolFactory::make(toolName, params);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::createPolygonItem()
{
  resetDefaultConfig();
  m_showContextMenu = false;

  EnumToolType* tools = Enums::getInstance().getEnumToolType();

  std::string toolName = tools->getCreatePolygonItemTool()->getName();
  ToolFactoryParamsCreate params(this);

  m_currentTool = te::layout::ToolFactory::make(toolName, params);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::createItem(EnumType* itemType)
{
  resetDefaultConfig();

  EnumToolType* tools = Enums::getInstance().getEnumToolType();

  std::string toolName = tools->getCreateItemTool()->getName();
  ToolFactoryParamsCreate params(this, itemType);

  m_currentTool = te::layout::ToolFactory::make(toolName, params);

  setInteractive(false);
  viewport()->installEventFilter(m_currentTool);
}

void te::layout::View::applyScale(double horizontalScale, double verticalScale)
{
  if((horizontalScale <= 0)||(verticalScale <= 0))
  {
    return;
  }

  scale(horizontalScale, verticalScale);
}

void te::layout::View::drawForeground( QPainter * painter, const QRectF & rect )
{
  if ( !painter )
    return;

  QGraphicsView::drawForeground(painter, rect);
  if(!m_visibleRulers)
    return;

  QGraphicsScene* scene = this->scene();
  if(scene == 0)
  {
    return;
  }

  double scale = transform().m11();


  if (m_foreground.isNull())
  {
    m_foreground = QPixmap(painter->device()->width(), painter->device()->height());
    m_foreground.fill(Qt::transparent);
    QPainter painter2(&m_foreground);
    painter2.setTransform(painter->transform());

    m_horizontalRuler->drawRuler(this, &painter2, scale);
    m_verticalRuler->drawRuler(this, &painter2, scale);

    painter2.end();

    m_foreground = QPixmap::fromImage(m_foreground.toImage().mirrored());
  }

  QRect rectView(0, 0, this->viewport()->width(), this->viewport()->height());
  QPolygonF polygonScene = this->mapToScene(rectView);

  painter->drawPixmap(polygonScene.boundingRect(), m_foreground, m_foreground.rect());
}

bool te::layout::View::exportProperties( EnumType* type )
{
  Scene* scne = dynamic_cast<Scene*>(scene());
  if (!scne)
    return false;

  emit aboutToPerformIO();

  bool is_export = false;

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), 
    QDir::currentPath(), tr("XML Files (*.xml)"));

  if(fileName.isEmpty())
  {
    emit endedPerformingIO();
    return is_export;
  }
  if (fileName.endsWith(".xml") == false)
  {
    fileName.append(".xml");
  }

  std::string j_name = ItemUtils::convert2StdString(fileName);

  is_export = scne->exportPropertiesToTemplate(type, j_name);

  QMessageBox msgBox;

  if(is_export)
  {
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(tr("Template exported successfully!"));    
  }
  else
  {
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(tr("Error exporting template!"));
  }

  emit endedPerformingIO();

  msgBox.exec();

  return is_export;
}

bool te::layout::View::importTemplate( EnumType* type )
{  
  Scene* scne = dynamic_cast<Scene*>(scene());
  if(!scne)
    return false;

  emit aboutToPerformIO();

  QString fileName = QFileDialog::getOpenFileName(this, tr("Import File"), 
    QDir::currentPath(), tr("XML Files (*.xml)"));

  if(fileName.isEmpty())
  {
    emit endedPerformingIO();
    return false;
  }

  std::string j_name = ItemUtils::convert2StdString(fileName); 

  bool result = scne->buildTemplate(m_visualizationArea, type, j_name);

  emit endedPerformingIO();

  return result;
}

void te::layout::View::exportItemsToImage()
{
  Scene* scne = dynamic_cast<Scene*>(scene());
  if(!scne)
    return;

  QMessageBox msgBox;

  if(scne->selectedItems().empty())
  {
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(tr("Select at least one component!")); 
    msgBox.exec();
    return;
  }

  QFileDialog dialog(this);
  dialog.setGeometry(QRect(this->width()/4, this->height()/4, this->width()/2, this->height()/2));
  QString dir = dialog.getExistingDirectory(this, tr("Open Directory"), 
    QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  if(dir.isEmpty())
    return;

  std::string dirName = ItemUtils::convert2StdString(dir);

  scne->exportItemsToImage(dirName);

  msgBox.setIcon(QMessageBox::Information);
  msgBox.setText(tr("Successfully exported images!")); 
  msgBox.exec();
}

void te::layout::View::refreshAllProperties()
{
  reload();
}

void te::layout::View::disableUpdate()
{
  // No update Widget while print is running
  setUpdatesEnabled(false);
  // Rulers aren't print
  m_visibleRulers = false;
}

void te::layout::View::enableUpdate()
{
  m_visibleRulers = true;
  setUpdatesEnabled(true);
}

te::layout::ContextObject te::layout::View::getContext()
{
  double dpiX = logicalDpiX();
  double dpiY = logicalDpiY();

  ItemUtils::setTextDPI(dpiY);

  int zoom = getCurrentZoom();
  EnumType* mode = getCurrentMode();
  return ContextObject(zoom, dpiX, dpiY, mode);
}

void te::layout::View::onEditionFinalized()
{
  reload();
}

te::layout::Scene* te::layout::View::getScene()
{
  return dynamic_cast<te::layout::Scene*> (this->scene());
}

bool te::layout::View::addToolbarItemInside(EnumType* itemType, ToolbarItemInside* toolbarInside)
{
  if (!itemType)
    return false;

  if (!toolbarInside)
    return false;

  toolbarInside->getToolbar()->setVisible(false);

  m_itemToolbars[itemType] = toolbarInside;
  return true;
}

te::layout::ToolbarItemInside* te::layout::View::getToolbarInside(EnumType* itemType)
{
  ToolbarItemInside* toolbarInside = 0;
  if (!itemType)
    return toolbarInside;

  if (m_itemToolbars.find(itemType) != m_itemToolbars.end())
  {
    QMap<EnumType*, ToolbarItemInside*>::iterator it = m_itemToolbars.find(itemType);
    toolbarInside = it.value();
  }
  return toolbarInside;
}

void te::layout::View::showToolbar(EnumType* itemType, AbstractItemView* item)
{
  if (!m_dialogItemToolbar)
    return;

  closeToolbar();

  if (itemType)
  {
    ToolbarItemInside* toolbarInside = getToolbarInside(itemType);
    if (toolbarInside)
    {
      QMap<EnumType*, ToolbarItemInside*>::iterator it = m_itemToolbars.find(itemType);
      toolbarInside = it.value();
      if (toolbarInside)
      {
        toolbarInside->setItem(item);
        QToolBar* toolbar = toolbarInside->getToolbar();
        toolbar->setParent(m_dialogItemToolbar);
        toolbar->setVisible(true);

        QSize size = toolbar->size();
        m_dialogItemToolbar->resize(size);

        QString title = toolbar->windowTitle();
        m_dialogItemToolbar->setWindowTitle(title);

        positioningToolbarOnTheScreen(item);

        m_dialogItemToolbar->setFixedSize(m_dialogItemToolbar->size());

        m_dialogItemToolbar->setVisible(true);
        m_currentToolbarInsideType = itemType;
      }
    }
  }
}

void te::layout::View::closeToolbar()
{
  if (!m_dialogItemToolbar)
    return;

  if (m_currentToolbarInsideType)
  {
    ToolbarItemInside* toolbarInside = getToolbarInside(m_currentToolbarInsideType);
    if (toolbarInside)
    {
      toolbarInside->clear();
      toolbarInside->setItem(0);
      m_dialogItemToolbar->setVisible(false);
      QList<QToolBar *> allPButtons = m_dialogItemToolbar->findChildren<QToolBar *>();
      QToolBar* toolbar = 0;
      if (!allPButtons.isEmpty())
      {
        toolbar = allPButtons.first();
        if (toolbar)
        {
          toolbar->setParent(0);
        }
      }
      m_currentToolbarInsideType = 0;
    }
  }
}

void te::layout::View::positioningToolbarOnTheScreen(AbstractItemView* item)
{
  if (!item)
  {
    return;
  }

  /* Positioning the dock (toolbar) on top of the item  */

  // space between the item and dock (toolbar)
  double space = 20;

  QGraphicsItem* qitem = dynamic_cast<QGraphicsItem*>(item);
  if (qitem)
  {
    // Required if the item has the inverted matrix
    QRectF boundRect = qitem->boundingRect();
    boundRect = qitem->mapRectToScene(boundRect);

    // Mapping to screen coordinates
    QPointF pos(qitem->scenePos().x(), qitem->scenePos().y() + boundRect.height());
    QPoint itemPos = mapFromScene(pos);
    QPoint ptGlobal = viewport()->mapToGlobal(itemPos);

    // total size with margins and borders
    QSize dockSize = m_dialogItemToolbar->size();

    // Place the dock on top of the item
    QRect rect(ptGlobal.x(), ptGlobal.y() - (m_dialogItemToolbar->height() + space), dockSize.width(), dockSize.height());
    m_dialogItemToolbar->setGeometry(rect);
  }
}

void te::layout::View::onScrollBarValueChanged(int value)
{
  // moved scrollbar so update foreground
  m_foreground = QPixmap();
  viewport()->update();
}

void te::layout::View::onShowDialogWindow(EnumType* type, QList<QGraphicsItem*> itemList)
{
  emit showDialogWindow(type, itemList);
}

void te::layout::View::configLayoutWithTempFile()
{
  // init or read temporary file data storage

  Scene* scene = getScene();

  // User folder path
  // Whether a directory separator is added to the end or not, depends on the operating system.
  QString newPath = QDir(QDir::tempPath()).filePath("TerraPrint");
  
  QDir dir(newPath);
  if (!dir.exists())
  {
    emit aboutToPerformIO();
    bool result = dir.mkpath(newPath); // create a new path or directory
    emit endedPerformingIO();
    if (!result)
    {
      return;
    }
  }
  else
  {
    QString pathToLoad = newPath + "/" + m_tempFileName;
    QFile file(pathToLoad);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      file.close();
      te::layout::EnumTemplateType* enumTemplate = te::layout::Enums::getInstance().getEnumTemplateType();
      importTempFile(enumTemplate->getXmlType(), pathToLoad); // load temp file
    }
  }

  m_fullTempPath = newPath + "/" + m_tempFileName;
  configTempFileDataStorage(m_fullTempPath); // init temp data storage 
}

void te::layout::View::configTempFileDataStorage(const QString& fullNewPath)
{
  Scene* scene = getScene();

  std::string path = ItemUtils::convert2StdString(fullNewPath);
  EnumTempDataStorageType* type = Enums::getInstance().getEnumTempDataStorageType();
  TempFileInfo* info = new TempFileInfo(scene, path);
  m_tempDataStorageEditor = new TempDataStorageEditor(scene->getUndoStack(), type->getTempFileType(), info);

  connect(m_tempDataStorageEditor, SIGNAL(requestIOEnterAccess()), this, SLOT(onRequestIOEnterAccessTempDataStorage()));
  connect(m_tempDataStorageEditor, SIGNAL(requestIOEndAccess()), this, SLOT(onRequestIOEndAccessTempDataStorage()));
}

void te::layout::View::onRequestIOEnterAccessTempDataStorage()
{
  emit aboutToPerformIO();
}

void te::layout::View::onRequestIOEndAccessTempDataStorage()
{
  emit endedPerformingIO();
}

QString te::layout::View::getFullTempPath()
{
  return m_fullTempPath;
}

QString te::layout::View::getTempFileName()
{
  return m_tempFileName;
}

bool te::layout::View::importTempFile(EnumType* type, const QString& fullTempPath)
{
  Scene* scne = dynamic_cast<Scene*>(scene());
  if (!scne)
    return false;

  emit aboutToPerformIO();

  if (fullTempPath.isEmpty())
  {
    emit endedPerformingIO();
    return false;
  }

  std::string j_name = ItemUtils::convert2StdString(fullTempPath);

  bool result = scne->buildTemplate(m_visualizationArea, type, j_name);

  emit endedPerformingIO();

  return result;
}

