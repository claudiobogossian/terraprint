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
  \file MenuPrincipal.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_MENU_PRINCIPAL_H 
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_MENU_PRINCIPAL_H

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

    class MenuPrincipal : public QObject
    {
      Q_OBJECT //for slots/signals

    public:

      MenuPrincipal(te::layout::View* view, QMenu* parentMenu);

      virtual ~MenuPrincipal();

      QMenu* getMenu();
      
      virtual void openMainMenu();

      virtual void closeMainMenu();
      
      void setDockProperties(PropertiesDock* dock);

      void setDockInspector(ObjectInspectorDock* dock);

      void setDockEditTemplate(EditTemplateDock* dock);

    public slots:

      virtual void onMainMenuTriggered(QAction* action);

    signals:

      void changeMenuMode(te::layout::EnumType* newMode);

      void exit();

    protected:

      virtual void init();

      virtual void createMainMenu();
      
      virtual QAction* createAction(const QString& text, const QString& objName, const QString& icon, const QString& tooltip = "");
      
    protected:

      PropertiesDock*       m_dockProperties;
      ObjectInspectorDock*  m_dockInspector;
      EditTemplateDock*     m_dockEditTemplate;
      te::layout::View*     m_view;
      QMenu*                m_layoutMenu;

      /* Menu options */

      QString               m_optionNew;
      QString               m_optionUpdate;
      QString               m_optionImportXml;
      QString               m_optionExportXml;
      QString               m_optionPageConfig;
      QString               m_optionPrint;
      QString               m_optionExit;
      QString               m_optionDockInspector;
      QString               m_optionDockProperties;
      QString               m_optionDockToolbar;
      QString               m_optionDockEditTemplate;
      QString               m_optionExportAs;
    };
  }
}

#endif
