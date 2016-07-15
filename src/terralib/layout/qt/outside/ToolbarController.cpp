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
  \file ToolbarController.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "../core/Scene.h"
#include "../../core/pattern/singleton/Context.h"
#include "../core/View.h"
#include "ToolbarController.h"
#include "ToolbarOutside.h"

// Qt
#include <QToolButton>
#include <QAction>
#include <QComboBox>

te::layout::ToolbarController::ToolbarController(AbstractOutsideModel* o, Scene* scene) :
  AbstractOutsideController(o),
  m_scene(scene)
{
  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();

  //map related items
  m_iconManager.setIcon(itemType->getLegendItem()->getName(), "layout-default-legend");
  m_iconManager.setIcon(itemType->getMapItem()->getName(),"layout-default-map");
  m_iconManager.setIcon(itemType->getMapCompositionItem()->getName(), "layout-default-map");
  m_iconManager.setIcon(itemType->getMapLocationItem()->getName(), "layout-map-location");
  m_iconManager.setIcon(itemType->getNorthItem()->getName(), "layout-north");
  m_iconManager.setIcon(itemType->getScaleItem()->getName(), "layout-scale");
  m_iconManager.setIcon(itemType->getThreeNorthItem()->getName(), "layout-three-north");

  //geometry related items
  m_iconManager.setIcon(itemType->getRectangleItem()->getName(), "layout-square");
  m_iconManager.setIcon(itemType->getArrowItem()->getName(), "layout-arrow");
  m_iconManager.setIcon(itemType->getEllipseItem()->getName(), "layout-ellipse");
  m_iconManager.setIcon(itemType->getPointItem()->getName(), "layout-point");
  m_iconManager.setIcon(itemType->getLineItem()->getName(), "layout-drawline");
  m_iconManager.setIcon(itemType->getPolygonItem()->getName(), "layout-polygon");
  m_iconManager.setIcon(itemType->getSVGItem()->getName(), "layout-svg");

  //text related items
  m_iconManager.setIcon(itemType->getTextItem()->getName(), "layout-default-text");
  m_iconManager.setIcon(itemType->getTitleItem()->getName(), "layout-title");
  m_iconManager.setIcon(itemType->getImageItem()->getName(), "layout-image");

  //group related items
  m_iconManager.setIcon(itemType->getItemGroup()->getName(), "layout-group");
}

te::layout::ToolbarController::~ToolbarController()
{

}

const te::layout::ItemIconManager& te::layout::ToolbarController::getIconManager() const
{
  return m_iconManager;
}

void te::layout::ToolbarController::onMapTriggered(QAction* action)
{ 
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }
  
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);
  
  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();

  View* viewport = m_scene->getView();

  if (action->objectName().compare(toolbar->getActionMapDefault()) == 0)
  {
    viewport->createItem(itemType->getMapCompositionItem());
  }
  else if (action->objectName().compare(toolbar->getActionLegendDefault()) == 0)
  {
    viewport->createItem(itemType->getLegendItem());
  }
  else if (action->objectName().compare(toolbar->getActionScale()) == 0)
  {
    viewport->createItem(itemType->getScaleItem());
  }
  else if (action->objectName().compare(toolbar->getActionNorth()) == 0)
  {
    viewport->createItem(itemType->getNorthItem());
  }
  else if (action->objectName().compare(toolbar->getActionMapLocation()) == 0)
  {
    viewport->createItem(itemType->getMapLocationItem());
  }
  else if (action->objectName().compare(toolbar->getActionThreeNorth()) == 0)
  {
    viewport->createItem(itemType->getThreeNorthItem());
  }
}

void te::layout::ToolbarController::onMapToolsTriggered(QAction* action)
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }

  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  EnumModeType* type = Enums::getInstance().getEnumModeType();

  if (action->objectName().compare(toolbar->getActionMapPan()) == 0)
  {
    toolbar->changeAction(type->getModeMapPan());
  }
  else if (action->objectName().compare(toolbar->getActionMapZoomIn()) == 0)
  {
    toolbar->changeAction(type->getModeMapZoomIn());
  }
  else if (action->objectName().compare(toolbar->getActionMapZoomOut()) == 0)
  {
    toolbar->changeAction(type->getModeMapZoomOut());
  }
  else if (action->objectName().compare(toolbar->getActionMapSystematicScale()) == 0)
  {
    toolbar->changeAction(type->getModeSystematicScale());
  }
  else if (action->objectName().compare(toolbar->getActionMapCreateTextGrid()) == 0)
  {
    //do nothing
  }
  else if (action->objectName().compare(toolbar->getActionMapCreateMapText()) == 0)
  {
    //do nothing
  }
  else if (action->objectName().compare(toolbar->getActionMapCreateLegendChildAsObject()) == 0)
  {
    //do nothing
  }
}

void te::layout::ToolbarController::onGeometryTriggered(QAction* action)
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }

  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);
  
  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();

  View* viewport = m_scene->getView();

  if (action->objectName().compare(toolbar->getActionRectangle()) == 0)
  {
    viewport->createItem(itemType->getRectangleItem());
  }
  else if (action->objectName().compare(toolbar->getActionArrow()) == 0)
  {
    viewport->createItem(itemType->getArrowItem());
  }
  else if (action->objectName().compare(toolbar->getActionEllipse()) == 0)
  {
    viewport->createItem(itemType->getEllipseItem());
  }
  else if (action->objectName().compare(toolbar->getActionPoint()) == 0)
  {
    viewport->createItem(itemType->getPointItem());
  }
  else if (action->objectName().compare(toolbar->getActionLine()) == 0)
  {
    viewport->createLineItem();
  }
  else if (action->objectName().compare(toolbar->getActionPolygon()) == 0)
  {
    viewport->createPolygonItem();
  }
  else if (action->objectName().compare(toolbar->getActionSVG()) == 0)
  {
    viewport->createItem(itemType->getSVGItem());
  }
}

void te::layout::ToolbarController::onViewAreaTriggered(QAction* action)
{
  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }

  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);
  
  View* viewport = m_scene->getView();

  if (action->objectName().compare(toolbar->getActionViewPan()) == 0)
  {
    viewport->pan();
  }
  else if (action->objectName().compare(toolbar->getActionViewZoomIn()) == 0)
  {
    viewport->zoomArea();
  }
  else if (action->objectName().compare(toolbar->getActionViewZoomOut()) == 0)
  {
    viewport->zoomOut();
  }
}

void te::layout::ToolbarController::onArrowCursorClicked(bool checked)
{
  View* viewport = m_scene->getView();
  viewport->arrowCursor();
}

void te::layout::ToolbarController::onItemToolsTriggered(QAction* action)
{
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }

  View* viewport = m_scene->getView();

  if (action->objectName().compare(toolbar->getActionGroup()) == 0)
  {    
    viewport->createItemGroup();
  }
  else if (action->objectName().compare(toolbar->getActionUngroup()) == 0)
  {
    viewport->destroyItemGroup();
  }
}

void te::layout::ToolbarController::onComboZoomActivated()
{
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  QString text = toolbar->getComboBoxZoom()->currentText();
  if (text.isEmpty() == true)
  {
    return;
  }

  QString textCopy = text;
  textCopy.replace(QString("%"), QString(""));

  bool converted = false;
  int newZoom = textCopy.toInt(&converted);
  if (converted == false)
  {
    toolbar->getComboBoxZoom()->setEditText("");
    return;
  }

  toolbar->notifyChangedZoom(newZoom);
}

void te::layout::ToolbarController::onZoomChanged(int zoom)
{
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  if (zoom <= 0)
  {
    return;
  }

  QString value = QString::number(zoom) + "%";
  if (toolbar->getComboBoxZoom()->currentText() == value)
  {
    return;
  }

  toolbar->getComboBoxZoom()->setEditText(value);
}

void te::layout::ToolbarController::onBringToFrontClicked(bool checked)
{    
  bool result = m_scene->getAlignItems()->bringToFront();
  if (result)
  {
    View* viewport = m_scene->getView();
    if (viewport)
    {
      viewport->refreshAllProperties();
    }
  }
}

void te::layout::ToolbarController::onSendToBackClicked(bool checked)
{
  bool result = m_scene->getAlignItems()->sendToBack();
  if (result)
  {
    View* viewport = m_scene->getView();
    if (viewport)
    {
      viewport->refreshAllProperties();
    }
  }
}

void te::layout::ToolbarController::onBringForwardClicked(bool checked)
{
  bool result = m_scene->getAlignItems()->bringForward();
  if (result)
  {
    View* viewport = m_scene->getView();
    if (viewport)
    {
      viewport->refreshAllProperties();
    }
  }
}

void te::layout::ToolbarController::onSendBackwardClicked(bool checked)
{
  bool result = m_scene->getAlignItems()->sendBackward();
  if (result)
  {
    View* viewport = m_scene->getView();
    if (viewport)
    {
      viewport->refreshAllProperties();
    }
  }
}

void te::layout::ToolbarController::onRecomposeClicked(bool checked)
{
  ContextObject context = m_scene->getContext();

  int zoom = context.getZoom();
  onZoomChanged(zoom);
  onComboZoomActivated();

  View* viewport = m_scene->getView();
  viewport->recompose();
}

void te::layout::ToolbarController::onTextToolsTriggered(QAction* action)
{
  ToolbarOutside* toolbar = dynamic_cast<ToolbarOutside*>(m_view);

  QToolButton* button = dynamic_cast<QToolButton*>(sender());
  if (button)
  {
    button->setDefaultAction(action);
  }
  
  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();

  View* viewport = m_scene->getView();

  if (action->objectName().compare(toolbar->getActionTextDefault()) == 0)
  {
    viewport->createItem(itemType->getTextItem());
  }
  else if (action->objectName().compare(toolbar->getActionStringGrid()) == 0)
  {
    viewport->createItem(itemType->getTextGridItem());
  }
  else if (action->objectName().compare(toolbar->getActionTitle()) == 0)
  {
    viewport->createItem(itemType->getTitleItem());
  }
  else if (action->objectName().compare(toolbar->getActionImage()) == 0)
  {
    viewport->createItem(itemType->getImageItem());
  }
  else if (action->objectName().compare(toolbar->getActionBalloon()) == 0)
  {
    viewport->createItem(itemType->getBalloonItem());
  }
  else if (action->objectName().compare(toolbar->getActionBarCode()) == 0)
  {
    viewport->createItem(itemType->getBarCodeItem());
  }
}

void te::layout::ToolbarController::onAlignLeftClicked(bool checked)
{
  m_scene->getAlignItems()->alignLeft();
}

void te::layout::ToolbarController::onAlignRightClicked(bool checked)
{
  m_scene->getAlignItems()->alignRight();
}

void te::layout::ToolbarController::onAlignTopClicked(bool checked)
{
  m_scene->getAlignItems()->alignTop();
}

void te::layout::ToolbarController::onAlignBottomClicked(bool checked)
{
  m_scene->getAlignItems()->alignBottom();
}

void te::layout::ToolbarController::onAlignCenterHorizontalClicked(bool checked)
{  
  m_scene->getAlignItems()->alignCenterHorizontal();
}

void te::layout::ToolbarController::onAlignCenterVerticalClicked(bool checked)
{
  m_scene->getAlignItems()->alignCenterVertical();
}

void te::layout::ToolbarController::onRemoveObjectClicked(bool checked)
{
  m_scene->removeSelectedItems();
}

void te::layout::ToolbarController::onDrawMapClicked(bool checked)
{
  m_scene->redrawSelectionMap();
}

void te::layout::ToolbarController::onObjectToImageClicked(bool checked)
{
  View* viewport = m_scene->getView();
  viewport->exportItemsToImage();
}

void te::layout::ToolbarController::onExitClicked(bool checked)
{
  View* viewport = m_scene->getView();
  viewport->close();
}

void te::layout::ToolbarController::onExportToPDFClicked(bool checked)
{
  View* viewport = m_scene->getView();
  viewport->exportToPDF();
}

