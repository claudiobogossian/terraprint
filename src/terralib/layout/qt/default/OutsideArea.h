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
  \file OutsideArea.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_OUTSIDE_AREA_H 
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_OUTSIDE_AREA_H

// TerraLib
#include "../../../layout/qt/core/View.h"
#include "../../../layout/qt/outside/ToolbarOutside.h"

// STL
#include <string>

// Qt
#include <QObject>
#include <QString>

class QWidget;
class QMenu;
class QAction;
class QStatusBar;
class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class PropertiesDock;
    class ObjectInspectorDock;
    class EditTemplateDock;
    class EnumType;
    class AbstractProxyProject;
    class MenuPrincipal;

    class OutsideArea : public QObject
    {
      Q_OBJECT //for slots/signals

    public:

      OutsideArea(AbstractProxyProject* proxyProject, te::layout::View* view, QStatusBar* status = 0);

      virtual ~OutsideArea();

      PropertiesDock* getPropertiesDock();

      ObjectInspectorDock* getObjectInspectorDock();

      te::layout::ToolbarOutside* getToolbar();

      te::layout::EditTemplateDock* getEditTemplate();

      MenuPrincipal* getMenuPrincipal(QMenu* parentMenu);

      virtual void openAllDocks();

      virtual void closeAllDocks();
      
    public slots:
    
      virtual void onAddItemFinalized(QGraphicsItem* item);

      virtual void onShowView();

      virtual void onCloseView();

      virtual void onRefreshStatusBar();

      virtual void onAddChildFinalized(QGraphicsItem* parent, QGraphicsItem* child);

      virtual void onDeleteFinalized(std::vector<std::string>);

      virtual void onSelectionChanged(QList<QGraphicsItem*> selectedItems);

      virtual void onSelectionChanged();

      virtual void onPropertiesChanged();

      virtual void onEditionFinalized();

      virtual void onEditionInitialized();

      virtual void onExit();

    signals:
      
      void exit();

    protected:

      virtual void init(AbstractProxyProject* proxyProject);

      virtual void createPropertiesDock(AbstractProxyProject* proxyProject);

      virtual void createInspectorDock();

      virtual void createToolbar();
      
      virtual void createEditTemplateDock();
      
      virtual void addAllItemToolbars();

    protected:

      PropertiesDock*              m_dockProperties;
      ObjectInspectorDock*         m_dockInspector;
      EditTemplateDock*            m_dockEditTemplate;
      te::layout::View*            m_view;
      te::layout::ToolbarOutside*  m_toolbar;
      QStatusBar*                  m_statusBar;
      MenuPrincipal*               m_menuPrincipal;

      /* Menu options */

      QString m_optionNew;
      QString m_optionUpdate;
      QString m_optionImportXml;
      QString m_optionExportXml;
      QString m_optionPageConfig;
      QString m_optionPrint;
      QString m_optionExit;
      QString m_optionDockInspector;
      QString m_optionDockProperties;
      QString m_optionDockToolbar;
      QString m_optionDockEditTemplate;
    };
  }
}


#endif
