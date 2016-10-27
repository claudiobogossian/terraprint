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
  \file View.h
   
  \brief Class representing the view. This view is child of QGraphicsView, part of Graphics View Framework. 
  The View object is a widget where the items in the scene are drawn. 
  It is responsible for presentation and interaction between the screen events and the scene. 
  Manages interactive functions and intercepts all mouse or keyboard events. 
  The coordinate system of this representation is pixel and your point 0.0 is in the top left(Screen coordinate system).
  The default settings of the layout module are in the object Context (singleton).

  \ingroup layout
*/

#ifndef  __TERRALIB_LAYOUT_INTERNAL_VIEW_H 
#define __TERRALIB_LAYOUT_INTERNAL_VIEW_H

// TerraLib
#include "../../core/AbstractView.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/geometry/Coord2D.h"
#include "Scene.h"
#ifndef Q_MOC_RUN
#include "../outside/PageSetupOutside.h"
#include "../outside/SystematicScaleOutside.h"
#include "../../core/property/Property.h"
#endif

// STL
#include <string>

// Qt
#include <QGraphicsView>
#include <QMenu>
#include <QImage>
#include <QCursor>
#include <QMap>
#include <QClipboard>
#include <QMimeData>
#include <QApplication>
#include <QString>

class QMouseEvent;
class QWheelEvent;
class QKeyEvent;
class QString;
class QHideEvent;
class QCloseEvent;
class QGraphicsItemGroup;
class QLineF;
class QContextMenuEvent;
class QEvent;

namespace te
{
  namespace layout
  {
    class VisualizationArea;
    class AbstractLayoutTool;
    class HorizontalRuler;
    class VerticalRuler;
    class EnumType;
    class WaitView;
    class ContextObject;
    class Scene;
    class EnumType;
    class ToolbarItemInside;
    class DialogItemToolbar;
    class TempDataStorageEditor;

  /*!
    \brief Class representing the view. This view is child of QGraphicsView, part of Graphics View Framework. 
    The View object is a widget where the items in the scene are drawn. 
    It is responsible for presentation and interaction between the screen events and the scene. 
    Manages interactive functions and intercepts all mouse or keyboard events. 
    The coordinate system of this representation is pixel and your point 0.0 is in the top left(Screen coordinate system).
    The default settings of the layout module are in the object Context (singleton).
    
    \ingroup layout

    \sa te::layout::AbstractView
  */
    class TELAYOUTEXPORT View : public QGraphicsView, public AbstractView
    {
      Q_OBJECT //for slots/signals


      public:

    /*!
          \brief Constructor
        */ 
        View(QWidget* widget = (QWidget*)0);

    /*!
          \brief Destructor
        */ 
        virtual ~View();

    /*!
          \brief Initialization method that configures the View and sets the transformation matrix of the scene in the View object.    
        */
        virtual void config();     

    /*!
          \brief Close all windows that have been instantiated by this View. Ex .: Page Settings     
        */
        virtual void closeOutsideWindows();

    /*!
          \brief Create snapshot of the View current state.

      \return image
        */
        virtual QImage createImage();

    /*!
          \brief Reset to initial configuration made in the config() method.    
        */
        virtual void resetView();

    /*!
          \brief Method that sets View object for the Pan function.  
        */
        virtual void pan();

    /*!
          \brief Method that sets View object for the Zoom Area function.  
        */
        virtual void zoomArea();

    /*!
          \brief Method that sets View object for the Zoom Out function.  
        */
        virtual void zoomOut();

    /*!
          \brief Method that sets View object for the Print function.  
        */
        virtual void print();

    /*!
          \brief Reset to initial configuration made in the config() method. Reset to default zoom factor. 
        */
        virtual void recompose();

        /*!
          \brief Method that sets View object for the Print function.  
        */
        virtual void exportToPDF();

        /*!
      \brief This method allows to refresh all properties of the selected item (send a signal).
    */
        virtual void refreshAllProperties();

        /*!
          \brief Reset the view interaction with the default settings, cursor, viewport update and no current tools.
          \param lateToolDeletion If true, the tool will no be deleted imediately, but added to a list to late removal. This is useful 
        */
        virtual void resetDefaultConfig(bool toolLateRemoval = false);

        virtual void disableUpdate();

        virtual void enableUpdate();

        virtual void arrowCursor();

        /*!
        \brief Groups selected objects
        */
        virtual void createItemGroup();

        /*!
        \brief Method that delete Grouping object selected, but the individual objects continue to exist.
        */
        virtual void destroyItemGroup();

        /*!
        \brief Saves each item in the scene as image. Ex .: .png
        */
        virtual void exportItemsToImage();

        virtual void newTemplate();

        /*!
        \brief Method that exports all the objects in the scene to a template. Ex.: JSON.

        \param type type of template. Ex .: JSON type
        \return true if exported, false otherwise
        */
        virtual bool exportProperties(EnumType* type);

        /*!
        \brief Method that import a template and build all objects. Ex.: JSON.

        \return true if exported, false otherwise
        */
        virtual bool importTemplate(EnumType* type);

        /*!
        \brief Method that instantiates and shows the Page Setup window.
        */
        virtual void showPageSetup();

        virtual void createLineItem();

        virtual void createPolygonItem();

        virtual void createItem(EnumType* itemType);

        virtual ContextObject getContext();

        virtual Scene* getScene();

        /*!
        \brief Method that adds ToolbarItemInside with a toolbar to be displayed when editing an item, which is identified by name.
        By adding the toolbar, the owner becomes the View.

        \param itemType type of the item
        \param toolbarInside ToolbarItemInside with a toolbar that will be displayed when editing the item
        \return true if added, false otherwise
        */
        bool addToolbarItemInside(EnumType* itemType, ToolbarItemInside* toolbarInside);

        /*!
        \brief Method that return a toolbar to be displayed when editing an item, which is identified by name. The View is the owner.

        \param itemType type of the item
        \return ToolbarItemInside with toolbar that will be displayed when editing the item
        */
        ToolbarItemInside* getToolbarInside(EnumType* itemType);

        /*!
        \brief Method that show a dock with a toolbar when editing an item. The View is the owner of the dock and ToolBar.

        \param itemType type of the item
        */
        virtual void showToolbar(EnumType* itemType, AbstractItemView* item);

        /*!
        \brief Method that close a dock with a toolbar when exit a editing item. The View is the owner of the dock. Change to NULL the owner of the ToolBar.

        \param itemType type of the item
        */
        virtual void closeToolbar();

        virtual void copyToClipboard();

        virtual void paste();

        virtual void cutSelectedItens();

        virtual void removeSelectedItem();

        virtual QString getFullTempPath();

        virtual QString getTempFileName();

        virtual void configLayoutWithTempFile();

      public slots:
    
        /*!
          \brief Notifies View object that some configuration was modified in the Page Settings Window.
        */
        virtual void onChangeConfig();

        virtual void onSystematicApply(double scale, SystematicScaleType type);
                
         /*!
          \brief Sets the zoom of the QGraphicsView to fit the given rect
        */
        virtual void fitZoom(const QRectF& rect);

        /*!
          \brief Sets the zoom of the View to the given value
        */
        virtual void setZoom(int zoom);

        /*!
          \brief Update properties when the scene leaves the edition mode
        */
        virtual void onEditionFinalized();

      protected slots:
      
        virtual void onScrollBarValueChanged(int value);

        virtual void onShowDialogWindow(EnumType* type, QList<QGraphicsItem*> itemList);

        virtual void onRequestIOEnterAccessTempDataStorage();

        virtual void onRequestIOEndAccessTempDataStorage();

      signals:

    /*!
          \brief This signal is emitted when the mouse move changed.
      
      \param pos current position
        */
        void changeSceneCoordMouse(QPointF pos);

    /*!
          \brief This signal is emitted when show View object.
        */
        void showView();
  
    /*!
          \brief This signal is emitted when close View object.
        */
        void closeView();

    /*!
          \brief This signal is emitted when hide View object.
        */
        void hideView();
    
        /*!
          \brief This signal is emitted when mouse release or property refresh requested.
        */
        void reloadProperties(); 

        /*!
        \brief This signal is emitted when selection change.
        */
        void loadProperties();

    /*!
          \brief This signal is emitted when View object changes the zoom factor internally.
          */
        void zoomChanged(int zoom);

        /*!
          \brief This signal is emitted when context change.
        */
        void changeContext();

        /*!
          \brief This signal is emitted when View object changes the mode type internally.
          */
        void modeChanged(int mode);

        /*!
        \brief This signal is emitted when the view is about to perform an IO operation.
        */
        void aboutToPerformIO();

        /*!
        \brief This signal is emitted when the view just ended performing the IO operation.
        */
        void endedPerformingIO();

        void showContextMenu(QPointF scenePoint, QPointF globalPoint);

        void showDialogWindow(EnumType* type, QList<QGraphicsItem*> itemList);
        
      protected:

    /*!
          \brief Reimplemented from QGraphicsView. Call method createItem() of the scene.
        */
        virtual void mousePressEvent(QMouseEvent * event);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void  mouseMoveEvent ( QMouseEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void  mouseReleaseEvent ( QMouseEvent * event );

        /*!
        \brief Reimplemented from QGraphicsView
        */
        virtual void mouseDoubleClickEvent(QMouseEvent * event);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void wheelEvent(QWheelEvent *event);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void keyPressEvent(QKeyEvent* keyEvent);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void resizeEvent(QResizeEvent * event);

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void hideEvent ( QHideEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void closeEvent ( QCloseEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void  showEvent ( QShowEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void  contextMenuEvent ( QContextMenuEvent * event );

    /*!
          \brief Reimplemented from QGraphicsView
        */
        virtual void drawForeground ( QPainter * painter, const QRectF & rect );

        virtual void showSystematicScale();
        
        virtual QCursor createCursor(std::string pathIcon);

        /*!
          \brief Reload properties because mouse release or property refresh requested.
        */
        virtual void reload();

        /*!
        \brief Load properties because selection change
        */
        virtual void load();

        /*!
        \brief Apply the zoom in the QGraphicsView to the given value
        */
        virtual void applyScale(double horizontalScale, double verticalScale);
        
        /*!
        \brief positioning the dock (toolbar) on top of the item 
        */
        virtual void positioningToolbarOnTheScreen(AbstractItemView* item);

        QMimeData* convert2MimeData(const  std::vector<Properties>& properties);

        virtual void configTempFileDataStorage(const QString& fullNewPath);

        virtual bool importTempFile(EnumType* type, const QString& fullTempPath);

      protected:

        VisualizationArea*                   m_visualizationArea;
        AbstractLayoutTool*                  m_currentTool;
        PageSetupOutside*                    m_pageSetupOutside;
        SystematicScaleOutside*              m_systematicOutside;
        te::gm::Coord2D                      m_coordSystematic;
        bool                                 m_selectionChange;
        HorizontalRuler*                     m_horizontalRuler;
        VerticalRuler*                       m_verticalRuler;
        double                               m_width;
        double                               m_height;
        bool                                 m_isMoving;
        bool                                 m_updateItemPos;
        QPixmap                              m_foreground; //!< This pixmap represents the foreground drawings and is used for double buffering
        std::vector<AbstractLayoutTool*>     m_lateRemovalVec;
        bool                                 m_mouseEvent; //!< if False yet happened mouseRelease, otherwise True
        QMap<EnumType*, ToolbarItemInside*>  m_itemToolbars; //!< toolbars to be displayed when editing an item
        DialogItemToolbar*                   m_dialogItemToolbar;
        EnumType*                            m_currentToolbarInsideType;
        bool                                 m_midButtonClicked;
        bool                                 m_showContextMenu;
        QClipboard*                          m_clipboard;
        bool                                 m_cutObject;
        TempDataStorageEditor*               m_tempDataStorageEditor;
        QString                              m_fullTempPath;
        QString                              m_tempFileName;
    };
  }
}
#endif
