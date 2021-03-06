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
  \file MenuItem.h
   
  \brief Class responsible for creating the menu, right mouse button, and dynamically add menu options.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MENU_BUILDER_H 
#define __TERRALIB_LAYOUT_INTERNAL_MENU_BUILDER_H

// Qt
#include <QObject>

// TerraLib
#include "../../../core/Config.h"
#ifndef Q_MOC_RUN
#include "../../../core/property/Properties.h"
#include "DialogPropertiesBrowser.h"
#endif

// STL
#include <string>
#include <vector>

class QAction;
class QMenu;
class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class PropertiesUtils;

    /*!
    \brief Class responsible for creating the menu, right mouse button, and dynamically add menu options.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT MenuBuilder : public DialogPropertiesBrowser
    {
      Q_OBJECT //for slots/signals

    public:

      MenuBuilder(Scene* scene, AbstractProxyProject* proxyProject, QObject* parent = 0);

      virtual ~MenuBuilder();

      virtual void createMenuItem(QMenu* menu, const std::vector<te::layout::Property>& vecProperties);

      virtual void createMenu(const QList<QGraphicsItem*>& items);

      virtual void menuExec(int x = 0, int y = 0);

      virtual void setSelectedGraphicsItems(QList<QGraphicsItem*>& items);

      virtual void setCurrentProperty(std::string name);

      signals:

        /*!
          \brief Click on the context menu for an item, the user choose to open a window.
          \param prop corresponding property choice
        */
        void menuPropertyClicked(const Property& prop);

      protected slots:

        virtual void onMenuTriggered(QAction* action);

        virtual void onChangeDlgProperty(const Property& property);

        virtual void onShowPageSetupDlg();
                
      protected:
            
        virtual QAction* createAction(const QString& text, const QString& objName, const QString& icon, const QString& tooltip = "");
      
        virtual void changePropertyValue(const Property& property);

        virtual Property findMnuProperty(const std::string& name);

        virtual void checkedBool(bool checked);

      protected:

        QMenu*                m_menu;
        QList<QGraphicsItem*> m_graphicsItems;
        Properties            m_properties;
        PropertiesUtils*      m_propUtils;
    };
  }
}

#endif



