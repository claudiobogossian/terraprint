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
  \file PropertiesCentralController.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_CENTRAL_CONTROLLER_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTIES_CENTRAL_CONTROLLER_H

// Qt
#include <QObject>


// TerraLib
#include "../../core/Config.h"
#include "../../core/property/Properties.h"

// STL
#include <vector>
#include <string>

class QGraphicsItem;

namespace te
{
  namespace layout
  {
    class View;
    class PropertiesOutside;
    class ObjectInspectorOutside;
    class MenuBuilder;

    /*!
    \brief 
    
      \ingroup layout

      \sa 
    */
    class TELAYOUTEXPORT PropertiesCentralController : public QObject
    {
      Q_OBJECT //for slots/signals

      public:

        PropertiesCentralController(View* view, QObject *parent = 0);

        virtual ~PropertiesCentralController();

        void setPropertiesOutside(PropertiesOutside* outside);

        void setObjectInspector(ObjectInspectorOutside* outside);

        void setMenuBuilder(MenuBuilder* menu);

        void closeAllWindows();

      public slots:

        virtual void onAddItemFinalized(QGraphicsItem* item);

        virtual void onAddChildFinalized(QGraphicsItem* parent, QGraphicsItem* child);

        virtual void onDeleteFinalized(std::vector<std::string>);

        virtual void onSelectionChanged(QList<QGraphicsItem*> selectedItems);

        virtual void onSelectionChanged();

        virtual void onPropertiesChanged();

        virtual void onShowContextMenu(QPointF scenePoint, QPointF globalPoint);

        virtual void onShowDialogWindow(EnumType* type, QList<QGraphicsItem*> itemList);

      protected:

        virtual QList<QGraphicsItem*> getSelectedGraphicsItems();

      protected:

        View*                    m_view;
        PropertiesOutside*       m_propertiesOutside;
        ObjectInspectorOutside*  m_objectInspectorOutside;
        MenuBuilder*             m_menuBuilder;
    };
  }
}

#endif
