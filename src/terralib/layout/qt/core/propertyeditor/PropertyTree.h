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
  \file PropertyTree.h
   
   \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTY_TREE_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTY_TREE_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../core/property/Property.h"
#include "../../../core/Config.h"
#endif

// STL
#include <vector>

// Qt
#include <QTreeWidget>

class QTreeWidgetItem;

namespace te
{
  namespace layout
  {
    class View;

    /*!
    \brief 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT PropertyTree : public QTreeWidget
    {
      Q_OBJECT //for slots/signals

      public:

        PropertyTree(View* view, QWidget* parent = 0);

        virtual ~PropertyTree();

        virtual void setProperties(std::vector<Property> vprops);

        virtual void load(std::vector<Property> props);

        virtual QTreeWidgetItem* createNewRow(Property prop, QTreeWidgetItem* parent = 0);

        void clearAll();
        
      signals:

        void propertiesChanged(std::vector<Property> vprops);

      protected:

        virtual void configTree(int numberColumns);

      protected:
        
        View*                   m_view;
        std::vector<Property>   m_vprops;
        int                     m_columns;
    };
  }
}

#endif


