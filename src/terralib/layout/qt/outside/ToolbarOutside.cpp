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
  \file ToolbarOutside.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ToolbarOutside.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/AbstractScene.h"
#include "../core/Scene.h"
#include "terralib/geometry/Envelope.h"
#include "../../core/enum/Enums.h"
#include "../../core/ContextObject.h"
#include "ToolbarController.h"

//STL
#include <string>

// Qt
#include <QToolBar>
#include <QToolButton>
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QMenuBar>
#include <QAction>
#include <QPushButton>
#include <QUndoStack>
#include <QLineEdit>

te::layout::ToolbarOutside::ToolbarOutside(AbstractOutsideController* controller, Scene* scene, QWidget* parent) :
  QToolBar(parent),
  AbstractOutsideView(controller),
  m_comboZoom(0),
  m_actionMapDefault("map_default"),
  m_actionLegendDefault("legend_default"),
  m_actionScale("scale_object"),
  m_actionMapZoomIn("map_zoom_in"),
  m_actionMapZoomOut("map_zoom_out"),
  m_actionMapPan("map_pan"),
  m_actionMapSystematicScale("map_systematic_scale"),
  m_actionMapCreateTextGrid("map_text_grid"),
  m_actionMapCreateMapText("map_text_map"),
  m_actionMapCreateLegendChildAsObject("legend_child"),
  m_actionNorth("map_north"),
  m_actionThreeNorth("map_three_north"),
  m_actionMapLocation("map_location"),
  m_actionRectangle("geometry_rectangle"),
  m_actionPoint("geometry_point"),
  m_actionEllipse("geometry_ellipse"),
  m_actionArrow("geometry_arrow"),
  m_actionLine("geometry_line"),
  m_actionPolygon("geometry_polygon"),
  m_actionSVG("geometry_svg"),
  m_actionViewPan("view_pan"),
  m_actionViewZoomIn("view_zoom_in"),
  m_actionViewZoomOut("view_zoom_out"),
  m_actionGroup("items_group"),
  m_actionUngroup("items_ungroup"),
  m_actionSceneZoom("scene_zoom"),
  m_actionRemoveObject("remove_item"),
  m_actionTextDefault("text_default"),
  m_actionImage("text_image"),
  m_actionStringGrid("text_stringGrid"),
  m_actionTitle("text_title"),
  m_actionBalloon("text_balloon"),
  m_actionBarCode("text_barCode"),
  m_actionAlignLeft("align_left"),
  m_actionAlignRight("align_right"),
  m_actionAlignTop("align_top"),
  m_actionAlignBottom("align_bottom"),
  m_actionAlignCenterHorizontal("align_center_horizontal"),
  m_actionAlignCenterVertical("align_center_vertical"),
  m_actionUndo("undo"),
  m_actionRedo("redo"),
  m_actionDrawMap("draw_map"),
  m_actionObjectToImage("object_to_image"),
  m_actionExit("exit"),
  m_actionExportToPDF("export_to_pdf"),
  m_mapToolButton(0),
  m_mapToolsToolButton(0),
  m_geometryToolButton(0),
  m_viewAreaToolButton(0),
  m_arrowCursorButton(0),
  m_itemTools(0),
  m_bringToFrontToolButton(0),
  m_sendToBackToolButton(0),
  m_bringForwardToolButton(0),
  m_sendBackwardToolButton(0),
  m_recomposeToolButton(0),
  m_textToolButton(0),
  m_alignLeftToolButton(0),
  m_alignRightToolButton(0),
  m_alignTopToolButton(0),
  m_alignBottomToolButton(0),
  m_alignCenterHorizontalToolButton(0),
  m_alignCenterVerticalToolButton(0),
  m_removeObjectToolButton(0),
  m_undoToolButton(0),
  m_redoToolButton(0),
  m_drawMapToolButton(0),
  m_objectToImageButton(0),
  m_exitButton(0),
  m_exportToPDFButton(0),
  m_actionComboZoom(0),
  m_actionMapToolButton(0),
  m_actionMapToolsToolButton(0),
  m_actionGeometryToolButton(0),
  m_actionViewAreaToolButton(0),
  m_actionArrowCursorButton(0),
  m_actionItemTools(0),
  m_actionBringToFrontToolButton(0),
  m_actionSendToBackToolButton(0),
  m_actionBringForwardToolButton(0),
  m_actionSendBackwardToolButton(0),
  m_actionRecomposeToolButton(0),
  m_actionTextToolButton(0),
  m_actionAlignLeftToolButton(0),
  m_actionAlignRightToolButton(0),
  m_actionAlignTopToolButton(0),
  m_actionAlignBottomToolButton(0),
  m_actionAlignCenterHorizontalToolButton(0),
  m_actionAlignCenterVerticalToolButton(0),
  m_actionRemoveObjectToolButton(0),
  m_actionUndoToolButton(0),
  m_actionRedoToolButton(0),
  m_actionDrawMapToolButton(0),
  m_actionObjectToImageButton(0),
  m_actionExitButton(0),
  m_actionExportToPDFButton(0),
  m_scene(scene)
{
  setVisible(false);
  setWindowTitle(tr("Layout - Toolbar"));
  setMinimumSize(200, 10);
}

te::layout::ToolbarOutside::~ToolbarOutside()
{

}

void te::layout::ToolbarOutside::setPosition( const double& x, const double& y )
{
  move(x,y);
  refresh();
}

te::gm::Coord2D te::layout::ToolbarOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

void te::layout::ToolbarOutside::createToolbar()
{
  createArrowCursorButton();
  createDrawMapToolButton();
  this->addSeparator();

  createRecomposeToolButton();
  this->addSeparator();

  // Undo/Redo Actions
  createUndoToolButton();
  createRedoToolButton();
  this->addSeparator();
  
  createViewAreaToolButton();

  //Due to the refactrtoing, all the map tools need some revision.For this reason, this toolbutton is being temporarily disabled
  //createMapToolsToolButton();
  this->addSeparator();

  createMapToolButton();  
  createGeometryToolButton();
  createTextToolButton();
  this->addSeparator();

  createItemTools();
  this->addSeparator();

  createRemoveObjectToolButton();
  this->addSeparator();

  createBringForwardToolButton();
  createSendBackwardToolButton();
  createBringToFrontToolButton();
  createSendToBackToolButton();
  this->addSeparator();

  createAlignLeftToolButton();
  createAlignTopToolButton();
  createAlignBottomToolButton();
  createAlignRightToolButton();
  this->addSeparator();

  createAlignCenterHorizontalToolButton();
  createAlignCenterVerticalToolButton();
  this->addSeparator();

  createObjectToImageButton();
  this->addSeparator();

  createExportToPDFButton();
  this->addSeparator();

  createSceneZoomCombobox();
  this->addSeparator();

  createExitButton();
  this->addSeparator();
}

QToolButton* te::layout::ToolbarOutside::createMapToolButton()
{
  if (m_controller == 0)
  {
    return 0;
  }

  ToolbarController* toolbarController = dynamic_cast<ToolbarController*>(m_controller);
  if (toolbarController == 0)
  {
    return 0;
  }

  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();
  const ItemIconManager& iconManager = toolbarController->getIconManager();

  QToolButton *btnMap = createToolButton(tr("Map"), tr("Map Objects"), "");

  QMenu* menu = new QMenu(btnMap);

  QAction* actionDefaultMenu = createAction(tr("Default Map Object"), m_actionMapDefault, iconManager.getIconNameAsQString(itemType->getMapCompositionItem()->getName()), "", menu);
  menu->addAction(actionDefaultMenu);
  
  QAction* actionLegend = createAction(tr("Default Legend"), m_actionLegendDefault, iconManager.getIconNameAsQString(itemType->getLegendItem()->getName()), "", menu);
  menu->addAction(actionLegend);

  QAction* actionScale = createAction(tr("Scale Object"), m_actionScale, iconManager.getIconNameAsQString(itemType->getScaleItem()->getName()), "", menu);
  menu->addAction(actionScale);

  QAction* actionNorth = createAction(tr("North Object"), m_actionNorth, iconManager.getIconNameAsQString(itemType->getNorthItem()->getName()), "", menu);
  menu->addAction(actionNorth);

  QAction* actionThreeNorth = createAction(tr("Three North Object"), m_actionThreeNorth, iconManager.getIconNameAsQString(itemType->getNorthItem()->getName()), "", menu);
  menu->addAction(actionThreeNorth);

  QAction* actionMapLocation = createAction(tr("Map Location Object"), m_actionMapLocation, iconManager.getIconNameAsQString(itemType->getMapLocationItem()->getName()), "", menu);
  menu->addAction(actionMapLocation);
  
  btnMap->setMenu(menu);
  btnMap->setPopupMode(QToolButton::MenuButtonPopup);
  btnMap->setDefaultAction(actionDefaultMenu);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btnMap, SIGNAL(triggered(QAction*)), controller, SLOT(onMapTriggered(QAction*)));
  m_actionMapToolButton = this->addWidget(btnMap);

  m_mapToolButton = btnMap;

  return btnMap;
}

QToolButton* te::layout::ToolbarOutside::createMapToolsToolButton()
{
  QToolButton *btnMapTools = createToolButton(tr("Map Tools"), tr("Map Tools"), "");

  QMenu* menu = new QMenu(btnMapTools);

  QAction* actionPan = createAction(tr("Pan Tool"), m_actionMapPan, "layout-map-pan", "", menu);
  menu->addAction(actionPan);

  QAction* actionZoomIn = createAction(tr("Zoom In"), m_actionMapZoomIn, "layout-map-zoom-in", "", menu);
  menu->addAction(actionZoomIn);

  QAction* actionZoomOut = createAction(tr("Zoom Out"), m_actionMapZoomOut, "layout-map-zoom-out", "", menu);
  menu->addAction(actionZoomOut);

  QAction* actionSystematic = createAction(tr("Systematic Scale"), m_actionMapSystematicScale, "layout-systematic-map", "", menu);
  menu->addAction(actionSystematic);

  QAction* actionTextGrid = createAction(tr("Text Grid as Object"), m_actionMapCreateTextGrid, "layout-createtext-as-obj", "", menu);
  menu->addAction(actionTextGrid);

  QAction* actionTextMap = createAction(tr("Text Map as Object"), m_actionMapCreateMapText, "layout-createmap-text-as-objs", "", menu);
  menu->addAction(actionTextMap);

  QAction* actionLegend = createAction(tr("Legend as Object"), m_actionMapCreateLegendChildAsObject, "layout-legend-child", "", menu);
  menu->addAction(actionLegend);

  btnMapTools->setMenu(menu);
  btnMapTools->setPopupMode(QToolButton::MenuButtonPopup);
  btnMapTools->setDefaultAction(actionPan);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btnMapTools, SIGNAL(triggered(QAction*)), controller, SLOT(onMapToolsTriggered(QAction*)));
  m_actionMapToolsToolButton = this->addWidget(btnMapTools);

  m_mapToolsToolButton = btnMapTools;

  return btnMapTools;
}

QToolButton* te::layout::ToolbarOutside::createGeometryToolButton()
{
  if (m_controller == 0)
  {
    return 0;
  }

  ToolbarController* toolbarController = dynamic_cast<ToolbarController*>(m_controller);
  if (toolbarController == 0)
  {
    return 0;
  }

  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();
  const ItemIconManager& iconManager = toolbarController->getIconManager();

  QToolButton *btnGeometry = createToolButton(tr("Geometry Tools"), tr("Geometries Objects"), "");

  QMenu* menu = new QMenu(btnGeometry);

  QAction* actionRectagle = createAction(tr("Rectangle Object"), m_actionRectangle, iconManager.getIconNameAsQString(itemType->getRectangleItem()->getName()), "", menu);
  menu->addAction(actionRectagle);

  QAction* actionArrow = createAction(tr("Arrow Object"), m_actionArrow, iconManager.getIconNameAsQString(itemType->getArrowItem()->getName()), "", menu);
  menu->addAction(actionArrow);

  QAction* actionEllipse = createAction(tr("Ellipse Object"), m_actionEllipse, iconManager.getIconNameAsQString(itemType->getEllipseItem()->getName()), "", menu);
  menu->addAction(actionEllipse);

  QAction* actionPoint = createAction(tr("Point Object"), m_actionPoint, iconManager.getIconNameAsQString(itemType->getPointItem()->getName()), "", menu);
  menu->addAction(actionPoint);

  QAction* actionLine = createAction(tr("Line Object"), m_actionLine, iconManager.getIconNameAsQString(itemType->getLineItem()->getName()), "", menu);
  menu->addAction(actionLine);

  QAction* actionPolygon = createAction(tr("Polygon Object"), m_actionPolygon, iconManager.getIconNameAsQString(itemType->getPolygonItem()->getName()), "", menu);
  menu->addAction(actionPolygon);

  QAction* actionSVG = createAction(tr("SVG Object"), m_actionSVG, iconManager.getIconNameAsQString(itemType->getSVGItem()->getName()), "", menu);
  menu->addAction(actionSVG);

  btnGeometry->setMenu(menu);
  btnGeometry->setPopupMode(QToolButton::MenuButtonPopup);
  btnGeometry->setDefaultAction(actionRectagle);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btnGeometry, SIGNAL(triggered(QAction*)), controller, SLOT(onGeometryTriggered(QAction*)));
  m_actionGeometryToolButton = this->addWidget(btnGeometry);

  m_geometryToolButton = btnGeometry;

  return btnGeometry;
}

QToolButton* te::layout::ToolbarOutside::createViewAreaToolButton()
{
  QToolButton *btnViewArea = createToolButton(tr("Paper Tools"), tr("Paper Tools"), "");

  QMenu* menu = new QMenu(btnViewArea);

  QAction* actionPan = createAction(tr("Pan Tool"), m_actionViewPan, "layout-paper-pan", "", menu);
  menu->addAction(actionPan);
  
  QAction* actionZoomIn = createAction(tr("Zoom In"), m_actionViewZoomIn, "layout-paper-zoom-in", "", menu);
  menu->addAction(actionZoomIn);
  
  QAction* actionZoomOut = createAction(tr("Zoom Out"), m_actionViewZoomOut, "layout-paper-zoom-out", "", menu);
  menu->addAction(actionZoomOut);

  btnViewArea->setMenu(menu);
  btnViewArea->setPopupMode(QToolButton::MenuButtonPopup);
  btnViewArea->setDefaultAction(actionPan);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btnViewArea, SIGNAL(triggered(QAction*)), controller, SLOT(onViewAreaTriggered(QAction*)));
  m_actionViewAreaToolButton = this->addWidget(btnViewArea);

  m_viewAreaToolButton = btnViewArea;

  return btnViewArea;
}

QToolButton* te::layout::ToolbarOutside::createArrowCursorButton()
{
  QToolButton *btnArrowCursor = createToolButton(tr("Arrow Cursor"), tr("Arrow Cursor"), "layout-default-cursor");
  btnArrowCursor->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btnArrowCursor, SIGNAL(clicked(bool)), controller, SLOT(onArrowCursorClicked(bool)));

  m_actionArrowCursorButton = this->addWidget(btnArrowCursor);

  m_arrowCursorButton = btnArrowCursor;

  return btnArrowCursor;
}

QToolButton* te::layout::ToolbarOutside::createItemTools()
{
  if (m_controller == 0)
  {
    return 0;
  }

  ToolbarController* toolbarController = dynamic_cast<ToolbarController*>(m_controller);
  if (toolbarController == 0)
  {
    return 0;
  }

  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();
  const ItemIconManager& iconManager = toolbarController->getIconManager();

  QToolButton *btnTools = createToolButton(tr("Item Tools"), tr("Item Tools"), "");

  QMenu* menu = new QMenu(btnTools);

  QAction* actionGroup = createAction(tr("Group"), m_actionGroup, iconManager.getIconNameAsQString(itemType->getItemGroup()->getName()), "", menu);
  menu->addAction(actionGroup);

  QAction* actionUngroup = createAction(tr("Ungroup"), m_actionUngroup, "layout-ungroup", "", menu);
  menu->addAction(actionUngroup);

  btnTools->setMenu(menu);
  btnTools->setPopupMode(QToolButton::MenuButtonPopup);
  btnTools->setDefaultAction(actionGroup);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btnTools, SIGNAL(triggered(QAction*)), controller, SLOT(onItemToolsTriggered(QAction*)));
  m_actionItemTools = this->addWidget(btnTools);

  m_itemTools = btnTools;

  return btnTools;
}

QComboBox* te::layout::ToolbarOutside::createSceneZoomCombobox()
{
  m_comboZoom = new QComboBox(this);
  m_comboZoom->setObjectName(m_actionSceneZoom);

  m_comboZoom->setEditable(true);
  m_comboZoom->setInsertPolicy(QComboBox::NoInsert);
  m_comboZoom->addItem("42%", 0.42);
  m_comboZoom->addItem("50%", 0.5);
  m_comboZoom->addItem("70%", 0.7);
  m_comboZoom->addItem("100%", 1.);
  m_comboZoom->addItem("150%", 1.5);
  m_comboZoom->addItem("200%", 2.);
  m_comboZoom->addItem("300%", 3.); 

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(m_comboZoom, SIGNAL(activated(const QString &)), controller, SLOT(onComboZoomActivated()));
  connect(m_comboZoom->lineEdit(), SIGNAL(returnPressed()), controller, SLOT(onComboZoomActivated()));

  ContextObject context = m_scene->getContext();

  int zoom = context.getZoom();

  controller->onZoomChanged(zoom);

  m_actionComboZoom = this->addWidget(m_comboZoom);
  
  return m_comboZoom;
}

QToolButton* te::layout::ToolbarOutside::createBringToFrontToolButton()
{
  QToolButton *btn = createToolButton(tr("Bring to front"), tr("Bring to front"), "layout-in-front");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onBringToFrontClicked(bool)));

  m_actionBringToFrontToolButton = this->addWidget(btn);

  m_bringToFrontToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createSendToBackToolButton()
{
  QToolButton *btn = createToolButton(tr("Send to back"), tr("Send to back"), "layout-to-back");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onSendToBackClicked(bool)));

  m_actionSendToBackToolButton = this->addWidget(btn);

  m_sendToBackToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createBringForwardToolButton()
{
  QToolButton *btn = createToolButton(tr("Bring Forward"), tr("Bring Forward"), "layout-bring-forward");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onBringForwardClicked(bool)));

  m_actionBringForwardToolButton = this->addWidget(btn);

  m_bringForwardToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createSendBackwardToolButton()
{
  QToolButton *btn = createToolButton(tr("Send Backward"), tr("Send Backward"), "layout-send-backward");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onSendBackwardClicked(bool)));

  m_actionSendBackwardToolButton = this->addWidget(btn);

  m_sendBackwardToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createRecomposeToolButton()
{
  QToolButton *btn = createToolButton(tr("Recompose"), tr("Recompose"), "layout-recompose");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onRecomposeClicked(bool)));

  m_actionRecomposeToolButton = this->addWidget(btn);

  m_recomposeToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createTextToolButton()
{
  if (m_controller == 0)
  {
    return 0;
  }

  ToolbarController* toolbarController = dynamic_cast<ToolbarController*>(m_controller);
  if (toolbarController == 0)
  {
    return 0;
  }

  EnumObjectType* itemType = Enums::getInstance().getEnumObjectType();
  const ItemIconManager& iconManager = toolbarController->getIconManager();


  QToolButton *btn = createToolButton(tr("Text Tools"), tr("Text Tools"), "");

  QMenu* menu = new QMenu(btn);

  QAction* actionTxtDefault = createAction(tr("Default Text Object"), m_actionTextDefault, iconManager.getIconNameAsQString(itemType->getTextItem()->getName()), "", menu);
  menu->addAction(actionTxtDefault);
  
  QAction* actionTitle = createAction(tr("Title Object"), m_actionTitle, iconManager.getIconNameAsQString(itemType->getTitleItem()->getName()), "", menu);
  menu->addAction(actionTitle);

  /*QAction* actionStringGrid = createAction(tr("Text Grid Object"), m_actionStringGrid, "layout-grid", "", menu);
  menu->addAction(actionStringGrid);*/

  QAction* actionImage = createAction(tr("Image Object"), m_actionImage, iconManager.getIconNameAsQString(itemType->getImageItem()->getName()), "", menu);
  menu->addAction(actionImage);

  /*QAction* actionBalloon = createAction(tr("Balloon Object"), m_actionBalloon, "layout-ballon", "", menu);
  menu->addAction(actionBalloon);

  QAction* actionBarCode = createAction(tr("BarCode Object"), m_actionBarCode, "layout-barcode", "", menu);
  menu->addAction(actionBarCode);*/

  btn->setMenu(menu);
  btn->setPopupMode(QToolButton::MenuButtonPopup);
  btn->setDefaultAction(actionTxtDefault);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(triggered(QAction*)), controller, SLOT(onTextToolsTriggered(QAction*)));
  m_actionTextToolButton = this->addWidget(btn);

  m_textToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignLeftToolButton()
{
  QToolButton *btn = createToolButton(tr("Align Left"), tr("Align Left"), "layout-alignleft");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onAlignLeftClicked(bool)));

  m_actionAlignLeftToolButton = this->addWidget(btn);

  m_alignLeftToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignRightToolButton()
{
  QToolButton *btn = createToolButton(tr("Align Right"), tr("Align Right"), "layout-alignright");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onAlignRightClicked(bool)));

  m_actionAlignRightToolButton = this->addWidget(btn);

  m_alignRightToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignTopToolButton()
{
  QToolButton *btn = createToolButton(tr("Align Top"), tr("Align Top"), "layout-aligntop");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onAlignTopClicked(bool)));

  m_actionAlignTopToolButton = this->addWidget(btn);

  m_alignTopToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignBottomToolButton()
{
  QToolButton *btn = createToolButton(tr("Align Bottom"), tr("Align Bottom"), "layout-alignbottom");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onAlignBottomClicked(bool)));

  m_actionAlignBottomToolButton = this->addWidget(btn);

  m_alignBottomToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignCenterHorizontalToolButton()
{
  QToolButton *btn = createToolButton(tr("Align Center Horizontal"), tr("Align Center Horizontal"), "layout-alignhrzcenter");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onAlignCenterHorizontalClicked(bool)));

  m_actionAlignCenterHorizontalToolButton = this->addWidget(btn);

  m_alignCenterHorizontalToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createAlignCenterVerticalToolButton()
{
  QToolButton *btn = createToolButton(tr("Align Center Vertical"), tr("Align Center Vertical"), "layout-alignvrtcenter");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onAlignCenterVerticalClicked(bool)));

  m_actionAlignCenterVerticalToolButton = this->addWidget(btn);

  m_alignCenterVerticalToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createRemoveObjectToolButton()
{
  QToolButton *btn = createToolButton(tr("Remove Object"), tr("Remove Object"), "layout-empty-trash");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onRemoveObjectClicked(bool)));

  m_actionRemoveObjectToolButton = this->addWidget(btn);

  m_removeObjectToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createUndoToolButton()
{
  QToolButton *btn = createToolButton(tr("Undo"), tr("Undo"), "");

  QMenu* menu = new QMenu(btn);

  QUndoStack* undoStack = m_scene->getUndoStack();

  if(!undoStack)
    return 0;

  QAction* actionUndo = undoStack->createUndoAction(menu, tr("&Undo"));
  actionUndo->setObjectName(m_actionUndo);
  actionUndo->setIcon(QIcon::fromTheme("layout-undo"));
  actionUndo->setToolTip(tr("Undo Action"));
  btn->setDefaultAction(actionUndo);

  m_actionUndoToolButton = this->addWidget(btn);
  
  m_undoToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createRedoToolButton()
{
  QToolButton *btn = createToolButton(tr("Redo"), tr("Redo"), "");

  QMenu* menu = new QMenu(btn);

  QUndoStack* undoStack = m_scene->getUndoStack();

  if (!undoStack)
    return 0;

  QAction* actionRedo = undoStack->createRedoAction(menu, tr("&Redo"));
  actionRedo->setObjectName(m_actionRedo);
  actionRedo->setIcon(QIcon::fromTheme("layout-redo"));
  actionRedo->setToolTip(tr("Redo Action"));
  btn->setDefaultAction(actionRedo);

  m_actionUndoToolButton = this->addWidget(btn);

  m_redoToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createDrawMapToolButton()
{
  QToolButton *btn = createToolButton(tr("Redraw Selection Map"), tr("Redraw Selection Map"), "layout-draw-map");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onDrawMapClicked(bool)));

  m_actionDrawMapToolButton = this->addWidget(btn);

  m_drawMapToolButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createObjectToImageButton()
{
  QToolButton *btn = createToolButton(tr("Object To Image"), tr("Export all selected objects to image"), "layout-object-to-image");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onObjectToImageClicked(bool)));

  m_actionObjectToImageButton = this->addWidget(btn);

  m_objectToImageButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createExitButton()
{
  QToolButton *btn = createToolButton(tr("Exit"), tr("Exit"), "layout-close");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onExitClicked(bool)));

  m_actionExitButton = this->addWidget(btn);

  m_exitButton = btn;

  return btn;
}

QToolButton* te::layout::ToolbarOutside::createExportToPDFButton()
{
  QToolButton *btn = createToolButton(tr("Export To PDF"), tr("Export to PDF"), "layout-pdf");
  btn->setCheckable(false);

  ToolbarController* controller = dynamic_cast<ToolbarController*>(m_controller);
  connect(btn, SIGNAL(clicked(bool)), controller, SLOT(onExportToPDFClicked(bool)));

  m_actionExportToPDFButton = this->addWidget(btn);

  m_exportToPDFButton = btn;

  return btn;
}

void te::layout::ToolbarOutside::notifyChangedZoom(int newZoom)
{
  emit zoomChangedInComboBox(newZoom);
}

QToolButton* te::layout::ToolbarOutside::createToolButton(QString text, QString tooltip, QString icon)
{
  QToolButton *btn = new QToolButton(this);
  btn->setText(text);
  btn->setGeometry(0,0,10,10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip);
  
  if(!icon.isEmpty())
  btn->setIcon(QIcon::fromTheme(icon));

  return btn;
}

QPushButton* te::layout::ToolbarOutside::createPushButton(QString text, QString tooltip, QString icon)
{
  QPushButton *btn = new QPushButton(this);
  btn->setText(text);
  btn->setGeometry(0,0,10,10);
  btn->setCheckable(true);
  btn->setToolTip(tooltip);

  btn->setIcon(QIcon::fromTheme(icon));

  return btn;
}

QAction* te::layout::ToolbarOutside::createAction(QString text, QString objName, QString icon, QString tooltip, QWidget* parent)
{
  QWidget* parentToUse = parent;
  if(parentToUse == 0)
  {
  parentToUse = this;
  }

  QAction *actionMenu = new QAction(text, parentToUse);
  actionMenu->setObjectName(objName);

  actionMenu->setIcon(QIcon::fromTheme(icon));
  actionMenu->setToolTip(tooltip);

  return actionMenu;
}

void te::layout::ToolbarOutside::changeAction(EnumType* mode)
{
  emit changeMode(mode);
}

QComboBox* te::layout::ToolbarOutside::getComboBoxZoom()
{
  return m_comboZoom;
}

QToolButton* te::layout::ToolbarOutside::getMapToolButton()
{
  return m_mapToolButton;
}

QToolButton* te::layout::ToolbarOutside::getMapToolsToolButton()
{
  return m_mapToolsToolButton;
}

QToolButton* te::layout::ToolbarOutside::getGeometryToolButton()
{
  return m_geometryToolButton;
}

QToolButton* te::layout::ToolbarOutside::getViewAreaToolButton()
{
  return m_viewAreaToolButton;
}

QToolButton* te::layout::ToolbarOutside::getArrowCursorButton()
{
  return m_arrowCursorButton;
}

QToolButton* te::layout::ToolbarOutside::getItemTools()
{
  return m_itemTools;
}

QToolButton* te::layout::ToolbarOutside::getBringToFrontToolButton()
{
  return m_bringToFrontToolButton;
}

QToolButton* te::layout::ToolbarOutside::getSendToBackToolButton()
{
  return m_sendToBackToolButton;
}

QToolButton* te::layout::ToolbarOutside::getBringForwardToolButton()
{
  return m_bringForwardToolButton;
}

QToolButton* te::layout::ToolbarOutside::getSendBackwardToolButton()
{
  return m_sendBackwardToolButton;
}

QToolButton* te::layout::ToolbarOutside::getRecomposeToolButton()
{
  return m_recomposeToolButton;
}

QToolButton* te::layout::ToolbarOutside::getTextToolButton()
{
  return m_textToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignLeftToolButton()
{
  return m_alignLeftToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignRightToolButton()
{
  return m_alignRightToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignTopToolButton()
{
  return m_alignTopToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignBottomToolButton()
{
  return m_alignBottomToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignCenterHorizontalToolButton()
{
  return m_alignCenterHorizontalToolButton;
}

QToolButton* te::layout::ToolbarOutside::getAlignCenterVerticalToolButton()
{
  return m_alignCenterVerticalToolButton;
}

QToolButton* te::layout::ToolbarOutside::getRemoveObjectToolButton()
{
  return m_removeObjectToolButton;
}

QToolButton* te::layout::ToolbarOutside::getUndoToolButton()
{
  return m_undoToolButton;
}

QToolButton* te::layout::ToolbarOutside::getRedoToolButton()
{
  return m_redoToolButton;
}

QToolButton* te::layout::ToolbarOutside::getDrawMapToolButton()
{
  return m_drawMapToolButton;
}

QToolButton* te::layout::ToolbarOutside::getObjectToImageButton()
{
  return m_objectToImageButton;
}

QToolButton* te::layout::ToolbarOutside::getExitButton()
{
  return m_exitButton;
}

QString te::layout::ToolbarOutside::getActionMapDefault()
{
  return m_actionMapDefault;
}

QString te::layout::ToolbarOutside::getActionLegendDefault()
{
  return m_actionLegendDefault;
}

QString te::layout::ToolbarOutside::getActionScale()
{
  return m_actionScale;
}

QString te::layout::ToolbarOutside::getActionMapZoomIn()
{
  return m_actionMapZoomIn;
}

QString te::layout::ToolbarOutside::getActionMapZoomOut()
{
  return m_actionMapZoomOut;
}

QString te::layout::ToolbarOutside::getActionMapPan()
{
  return m_actionMapPan;
}

QString te::layout::ToolbarOutside::getActionMapSystematicScale()
{
  return m_actionMapSystematicScale;
}

QString te::layout::ToolbarOutside::getActionMapCreateTextGrid()
{
  return m_actionMapCreateTextGrid;
}

QString te::layout::ToolbarOutside::getActionMapCreateMapText()
{
  return m_actionMapCreateMapText;
}

QString te::layout::ToolbarOutside::getActionMapCreateLegendChildAsObject()
{
  return m_actionMapCreateLegendChildAsObject;
}

QString te::layout::ToolbarOutside::getActionNorth()
{
  return m_actionNorth;
}

QString te::layout::ToolbarOutside::getActionThreeNorth()
{
  return m_actionThreeNorth;
}

QString te::layout::ToolbarOutside::getActionMapLocation()
{
  return m_actionMapLocation;
}

QString te::layout::ToolbarOutside::getActionRectangle()
{
  return m_actionRectangle;
}

QString te::layout::ToolbarOutside::getActionPoint()
{
  return m_actionPoint;
}

QString te::layout::ToolbarOutside::getActionEllipse()
{
  return m_actionEllipse;
}

QString te::layout::ToolbarOutside::getActionArrow()
{
  return m_actionArrow;
}

QString te::layout::ToolbarOutside::getActionLine()
{
  return m_actionLine;
}

QString te::layout::ToolbarOutside::getActionPolygon()
{
  return m_actionPolygon;
}

QString te::layout::ToolbarOutside::getActionViewPan()
{
  return m_actionViewPan;
}

QString te::layout::ToolbarOutside::getActionViewZoomIn()
{
  return m_actionViewZoomIn;
}

QString te::layout::ToolbarOutside::getActionViewZoomOut()
{
  return m_actionViewZoomOut;
}

QString te::layout::ToolbarOutside::getActionGroup()
{
  return m_actionGroup;
}

QString te::layout::ToolbarOutside::getActionUngroup()
{
  return m_actionUngroup;
}

QString te::layout::ToolbarOutside::getActionSceneZoom()
{
  return m_actionSceneZoom;
}

QString te::layout::ToolbarOutside::getActionRemoveObject()
{
  return m_actionRemoveObject;
}

QString te::layout::ToolbarOutside::getActionTextDefault()
{
  return m_actionTextDefault;
}

QString te::layout::ToolbarOutside::getActionImage()
{
  return m_actionImage;
}

QString te::layout::ToolbarOutside::getActionStringGrid()
{
  return m_actionStringGrid;
}

QString te::layout::ToolbarOutside::getActionTitle()
{
  return m_actionTitle;
}

QString te::layout::ToolbarOutside::getActionBalloon()
{
  return m_actionBalloon;
}

QString te::layout::ToolbarOutside::getActionBarCode()
{
  return m_actionBarCode;
}

QString te::layout::ToolbarOutside::getActionAlignLeft()
{
  return m_actionAlignLeft;
}

QString te::layout::ToolbarOutside::getActionAlignRight()
{
  return m_actionAlignRight;
}

QString te::layout::ToolbarOutside::getActionAlignTop()
{
  return m_actionAlignTop;
}

QString te::layout::ToolbarOutside::getActionAlignBottom()
{
  return m_actionAlignBottom;
}

QString te::layout::ToolbarOutside::getActionAlignCenterHorizontal()
{
  return m_actionAlignCenterHorizontal;
}

QString te::layout::ToolbarOutside::getActionAlignCenterVertical()
{
  return m_actionAlignCenterVertical;
}

QString te::layout::ToolbarOutside::getActionUndo()
{
  return m_actionUndo;
}

QString te::layout::ToolbarOutside::getActionRedo()
{
  return m_actionRedo;
}

QString te::layout::ToolbarOutside::getActionDrawMap()
{
  return m_actionDrawMap;
}

QString te::layout::ToolbarOutside::getActionObjectToImage()
{
  return m_actionObjectToImage;
}

QString te::layout::ToolbarOutside::getActionExit()
{
  return m_actionExit;
}

QString te::layout::ToolbarOutside::getActionExportToPDF()
{
  return m_actionExportToPDF;
}

QAction* te::layout::ToolbarOutside::getActionMapToolButton()
{
  return m_actionMapToolButton;
}

QString te::layout::ToolbarOutside::getActionSVG()
{
  return m_actionSVG;
}

QAction* te::layout::ToolbarOutside::getActionMapToolsToolButton()
{
  return m_actionMapToolsToolButton;
}

QAction* te::layout::ToolbarOutside::getActionGeometryToolButton()
{
  return m_actionGeometryToolButton;
}

QAction* te::layout::ToolbarOutside::getActionViewAreaToolButton()
{
  return m_actionViewAreaToolButton;
}

QAction* te::layout::ToolbarOutside::getActionArrowCursorButton()
{
  return m_actionArrowCursorButton;
}

QAction* te::layout::ToolbarOutside::getActionItemTools()
{
  return m_actionItemTools;
}

QAction* te::layout::ToolbarOutside::getActionBringToFrontToolButton()
{
  return m_actionBringToFrontToolButton;
}

QAction* te::layout::ToolbarOutside::getActionSendToBackToolButton()
{
  return m_actionSendToBackToolButton;
}

QAction* te::layout::ToolbarOutside::getActionBringForwardToolButton()
{
  return m_actionBringForwardToolButton;
}

QAction* te::layout::ToolbarOutside::getActionSendBackwardToolButton()
{
  return m_actionSendBackwardToolButton;
}

QAction* te::layout::ToolbarOutside::getActionRecomposeToolButton()
{
  return m_actionRecomposeToolButton;
}

QAction* te::layout::ToolbarOutside::getActionTextToolButton()
{
  return m_actionTextToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignLeftToolButton()
{
  return m_actionAlignLeftToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignRightToolButton()
{
  return m_actionAlignRightToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignTopToolButton()
{
  return m_actionAlignTopToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignBottomToolButton()
{
  return m_actionAlignBottomToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignCenterHorizontalToolButton()
{
  return m_actionAlignCenterHorizontalToolButton;
}

QAction* te::layout::ToolbarOutside::getActionAlignCenterVerticalToolButton()
{
  return m_actionAlignCenterVerticalToolButton;
}

QAction* te::layout::ToolbarOutside::getActionRemoveObjectToolButton()
{
  return m_actionRemoveObjectToolButton;
}

QAction* te::layout::ToolbarOutside::getActionUndoToolButton()
{
  return m_actionUndoToolButton;
}

QAction* te::layout::ToolbarOutside::getActionRedoToolButton()
{
  return m_actionRedoToolButton;
}

QAction* te::layout::ToolbarOutside::getActionDrawMapToolButton()
{
  return m_actionDrawMapToolButton;
}

QAction* te::layout::ToolbarOutside::getActionObjectToImageButton()
{
  return m_actionObjectToImageButton;
}

QAction* te::layout::ToolbarOutside::getActionExitButton()
{
  return m_actionExitButton;
}

QAction* te::layout::ToolbarOutside::getActionComboBoxZoom()
{
  return m_actionComboZoom;
}

QAction* te::layout::ToolbarOutside::getActionExportToPDFButton()
{
  return m_actionExportToPDFButton;
}

