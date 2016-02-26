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
  \file ToolbarOutside.h
   
  \brief Standard Toolbar for Layout. Includes Qt components for interaction between the user and the plugin/app, via events, made from the Layout module. 
    Ex .: button for create Map, activate ZoomOut function, align selected components to the left, etc. 
    He is also the son of OutsideObserver, so it can become observer of a model (Observable). "View" part of widget.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_OUTSIDE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TOOLBAR_OUTSIDE_H

// Qt
#include <QToolBar>

// TerraLib
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "terralib/geometry/Envelope.h"
#include "../../core/Config.h"

// STL
#include <string>

// Qt
#include <QString>

class QGraphicsItem;
class QWidget;
class QGroupBox;
class QAction;
class QToolButton;
class QMenu;
class QPushButton;
class QComboBox;

namespace te
{
  namespace layout
  {
    class EnumType;
    class Scene;
    class AbstractOutsideController;

    /*!
    \brief Standard Toolbar for Layout. Includes Qt components for interaction between the user and the plugin/app, via events, made from the Layout module. 
       Ex .: button for create Map, activate ZoomOut function, align selected components to the left, etc. 
       This is also son of OutsideObserver, so it can become observer of a model (Observable). "View" part of widget.
    
      \ingroup layout

      \sa te::layout::OutsideObserver
    */
    class TELAYOUTEXPORT ToolbarOutside : public QToolBar, public AbstractOutsideView
    {
      Q_OBJECT //for slots/signals

    public:

      ToolbarOutside(AbstractOutsideController* controller, Scene* scene, QWidget* parent);

      virtual ~ToolbarOutside();
      
      virtual void setPosition(const double& x, const double& y);

      virtual te::gm::Coord2D getPosition();

      virtual void createToolbar();

      virtual void notifyChangedZoom(int newZoom);

      virtual void changeAction(EnumType* mode);

      virtual QToolButton* getMapToolButton();

      virtual QToolButton* getMapToolsToolButton();

      virtual QToolButton* getGeometryToolButton();

      virtual QToolButton* getViewAreaToolButton();

      virtual QToolButton* getArrowCursorButton();

      virtual QToolButton* getItemTools();
            
      virtual QToolButton* getBringToFrontToolButton();

      virtual QToolButton* getSendToBackToolButton();

      virtual QToolButton* getBringForwardToolButton();

      virtual QToolButton* getSendBackwardToolButton();

      virtual QToolButton* getRecomposeToolButton();

      virtual QToolButton* getTextToolButton();

      virtual QToolButton* getAlignLeftToolButton();

      virtual QToolButton* getAlignRightToolButton();

      virtual QToolButton* getAlignTopToolButton();

      virtual QToolButton* getAlignBottomToolButton();

      virtual QToolButton* getAlignCenterHorizontalToolButton();

      virtual QToolButton* getAlignCenterVerticalToolButton();

      virtual QToolButton* getRemoveObjectToolButton();

      virtual QToolButton* getUndoToolButton();

      virtual QToolButton* getRedoToolButton();

      virtual QToolButton* getDrawMapToolButton();

      virtual QToolButton* getObjectToImageButton();

      virtual QToolButton* getExitButton();

      virtual QComboBox* getComboBoxZoom();

      /* Actions (Widget's) */

      virtual QAction* getActionMapToolButton();

      virtual QAction* getActionMapToolsToolButton();

      virtual QAction* getActionGeometryToolButton();

      virtual QAction* getActionViewAreaToolButton();

      virtual QAction* getActionArrowCursorButton();

      virtual QAction* getActionItemTools();
      
      virtual QAction* getActionBringToFrontToolButton();

      virtual QAction* getActionSendToBackToolButton();

      virtual QAction* getActionBringForwardToolButton();

      virtual QAction* getActionSendBackwardToolButton();

      virtual QAction* getActionRecomposeToolButton();

      virtual QAction* getActionTextToolButton();

      virtual QAction* getActionAlignLeftToolButton();

      virtual QAction* getActionAlignRightToolButton();

      virtual QAction* getActionAlignTopToolButton();

      virtual QAction* getActionAlignBottomToolButton();

      virtual QAction* getActionAlignCenterHorizontalToolButton();

      virtual QAction* getActionAlignCenterVerticalToolButton();

      virtual QAction* getActionRemoveObjectToolButton();

      virtual QAction* getActionUndoToolButton();

      virtual QAction* getActionRedoToolButton();

      virtual QAction* getActionDrawMapToolButton();

      virtual QAction* getActionObjectToImageButton();

      virtual QAction* getActionExitButton();

      virtual QAction* getActionComboBoxZoom();

      virtual QAction* getActionExportToPDFButton();

      /* Actions names */

      /* Map Menu */

      QString getActionMapDefault();

      QString getActionLegendDefault();

      QString getActionScale();

      /* Map Tools */

      QString getActionMapZoomIn();

      QString getActionMapZoomOut();

      QString getActionMapPan();

      QString getActionMapSystematicScale();

      QString getActionMapCreateTextGrid();

      QString getActionMapCreateMapText();

      QString getActionMapCreateLegendChildAsObject();
      
      QString getActionGridPlanar();

      QString getActionGridGeodesic();

      QString getActionNorth();

      QString getActionThreeNorth();

      QString getActionMapLocation();

      /* Geometry Menu */

      QString getActionRectangle();

      QString getActionPoint();

      QString getActionEllipse();

      QString getActionArrow();

      QString getActionLine();

      QString getActionPolygon();

      QString getActionSVG();

      /* View Area */

      QString getActionViewPan();

      QString getActionViewZoomIn();

      QString getActionViewZoomOut();

      /* Items Tools */

      QString getActionGroup();

      QString getActionUngroup();
      
      QString getActionSceneZoom();

      QString getActionRemoveObject();

      /* Text Tools */

      QString getActionTextDefault();

      QString getActionImage();

      QString getActionStringGrid();

      QString getActionTitle();

      QString getActionBalloon();

      QString getActionBarCode();


      /* Align */

      QString getActionAlignLeft();

      QString getActionAlignRight();

      QString getActionAlignTop();

      QString getActionAlignBottom();

      QString getActionAlignCenterHorizontal();

      QString getActionAlignCenterVertical();

      /* Undo/Redo */

      QString getActionUndo();

      QString getActionRedo();

      QString getActionDrawMap();

      QString getActionObjectToImage();

      QString getActionExit();

      QString getActionExportToPDF();
                  
    signals:
      
      void changeMode(te::layout::EnumType* newMode);

      void zoomChangedInComboBox(int zoom);

    protected:

      virtual QToolButton* createMapToolButton();

      virtual QToolButton* createMapToolsToolButton();

      virtual QToolButton* createGeometryToolButton();

      virtual QToolButton* createViewAreaToolButton();

      virtual QToolButton* createArrowCursorButton();

      virtual QToolButton* createItemTools();
            
      virtual QToolButton* createBringToFrontToolButton();

      virtual QToolButton* createSendToBackToolButton();

      virtual QToolButton* createBringForwardToolButton();

      virtual QToolButton* createSendBackwardToolButton();

      virtual QComboBox* createSceneZoomCombobox();

      virtual QToolButton* createRecomposeToolButton();

      virtual QToolButton* createTextToolButton();

      virtual QToolButton* createAlignLeftToolButton();

      virtual QToolButton* createAlignRightToolButton();

      virtual QToolButton* createAlignTopToolButton();

      virtual QToolButton* createAlignBottomToolButton();

      virtual QToolButton* createAlignCenterHorizontalToolButton();

      virtual QToolButton* createAlignCenterVerticalToolButton();

      virtual QToolButton* createRemoveObjectToolButton();

      virtual QToolButton* createUndoToolButton();

      virtual QToolButton* createRedoToolButton();

      virtual QToolButton* createDrawMapToolButton();

      virtual QToolButton* createObjectToImageButton();

      virtual QToolButton* createExitButton();

      virtual QToolButton* createExportToPDFButton();
      
      virtual QToolButton* createToolButton(QString text, QString tooltip, QString icon);

      virtual QPushButton* createPushButton(QString text, QString tooltip, QString icon);
      
      virtual QAction* createAction(QString text, QString objName, QString icon, QString tooltip = "", QWidget* parent = 0);

      Scene* getScene();

    protected:

      QComboBox* m_comboZoom;

      /* Map Menu */
      QString m_actionMapDefault;
      QString m_actionLegendDefault;
      QString m_actionScale;

      /* Map Tools */
      QString m_actionMapZoomIn;
      QString m_actionMapZoomOut;
      QString m_actionMapPan;
      QString m_actionMapSystematicScale;
      QString m_actionMapCreateTextGrid;
      QString m_actionMapCreateMapText;
      QString m_actionMapCreateLegendChildAsObject;
      QString m_actionGridPlanar;
      QString m_actionGridGeodesic;
      QString m_actionNorth;
      QString m_actionThreeNorth;
      QString m_actionMapLocation;
      
      /* Geometry Menu */
      QString m_actionRectangle;
      QString m_actionPoint;
      QString m_actionEllipse;
      QString m_actionArrow;
      QString m_actionLine;
      QString m_actionPolygon;
      QString m_actionSVG;

      /* View Area */
      QString m_actionViewPan;
      QString m_actionViewZoomIn;
      QString m_actionViewZoomOut;
      
      /* Items Tools */

      QString m_actionGroup;
      QString m_actionUngroup;
      
      QString m_actionSceneZoom;

      QString m_actionRemoveObject;

      /* Text Tools */
      QString m_actionTextDefault;
      QString m_actionImage;
      QString m_actionStringGrid;
      QString m_actionTitle;
      QString m_actionBalloon;
      QString m_actionBarCode;

      /* Align */
      QString m_actionAlignLeft;
      QString m_actionAlignRight;
      QString m_actionAlignTop;
      QString m_actionAlignBottom;
      QString m_actionAlignCenterHorizontal;
      QString m_actionAlignCenterVertical;

      /* Undo/Redo */
      QString m_actionUndo;
      QString m_actionRedo;

      QString m_actionDrawMap;

      QString m_actionObjectToImage;
      QString m_actionExit;
      QString m_actionExportToPDF;

      /* ToolButton */
      QToolButton* m_mapToolButton;
      QToolButton* m_mapToolsToolButton;
      QToolButton* m_geometryToolButton;
      QToolButton* m_viewAreaToolButton;
      QToolButton* m_arrowCursorButton;
      QToolButton* m_itemTools;
      QToolButton* m_bringToFrontToolButton;
      QToolButton* m_sendToBackToolButton;
      QToolButton* m_bringForwardToolButton;
      QToolButton* m_sendBackwardToolButton;
      QToolButton* m_recomposeToolButton;
      QToolButton* m_textToolButton;
      QToolButton* m_alignLeftToolButton;
      QToolButton* m_alignRightToolButton;
      QToolButton* m_alignTopToolButton;
      QToolButton* m_alignBottomToolButton;
      QToolButton* m_alignCenterHorizontalToolButton;
      QToolButton* m_alignCenterVerticalToolButton;
      QToolButton* m_removeObjectToolButton;
      QToolButton* m_undoToolButton;
      QToolButton* m_redoToolButton;
      QToolButton* m_drawMapToolButton;
      QToolButton* m_objectToImageButton;
      QToolButton* m_exitButton;
      QToolButton* m_exportToPDFButton;

      /* Actions (Widget's) */

      QAction* m_actionComboZoom;
      QAction* m_actionMapToolButton;
      QAction* m_actionMapToolsToolButton;
      QAction* m_actionGeometryToolButton;
      QAction* m_actionViewAreaToolButton;
      QAction* m_actionArrowCursorButton;
      QAction* m_actionItemTools;
      QAction* m_actionBringToFrontToolButton;
      QAction* m_actionSendToBackToolButton;
      QAction* m_actionBringForwardToolButton;
      QAction* m_actionSendBackwardToolButton;
      QAction* m_actionRecomposeToolButton;
      QAction* m_actionTextToolButton;
      QAction* m_actionAlignLeftToolButton;
      QAction* m_actionAlignRightToolButton;
      QAction* m_actionAlignTopToolButton;
      QAction* m_actionAlignBottomToolButton;
      QAction* m_actionAlignCenterHorizontalToolButton;
      QAction* m_actionAlignCenterVerticalToolButton;
      QAction* m_actionRemoveObjectToolButton;
      QAction* m_actionUndoToolButton;
      QAction* m_actionRedoToolButton;
      QAction* m_actionDrawMapToolButton;
      QAction* m_actionObjectToImageButton;
      QAction* m_actionExitButton;
      QAction* m_actionExportToPDFButton;

      Scene* m_scene;
    };
  }
}

#endif
