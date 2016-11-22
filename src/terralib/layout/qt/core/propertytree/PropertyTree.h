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
    class PropertyDelegate;

    /*!
    \brief 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT PropertyTree : public QTreeWidget
    {
      Q_OBJECT //for slots/signals

      public:

        PropertyTree(View* view, PropertyDelegate* delegate = 0, QWidget* parent = 0);

        virtual ~PropertyTree();

        virtual void setProperties(std::vector<te::layout::Property> vprops);

        virtual void load(std::vector<te::layout::Property> props);

        virtual QTreeWidgetItem* createNewRow(te::layout::Property prop, QTreeWidgetItem* parent = 0);

        void clearAll();
        
      signals:

        void propertiesChanged(te::layout::Property prop);

      protected slots:
              
        /*!
          \brief Goes into editing mode when a item in a column is pressed with the mouse.
        */
        void onCheckEdit(QTreeWidgetItem * item, int column);

        virtual void onDataEditorChanged(te::layout::Property & prop, int row, int column);

      protected:

        virtual void configTree(int numberColumns);

        virtual QTreeWidgetItem* findTopParent(QTreeWidgetItem* topParent, QTreeWidgetItem* root);

      protected:
        
        View*                               m_view;
        std::vector<te::layout::Property>   m_vprops;
        int                                 m_columns;
        int                                 m_nameColumn;
        int                                 m_valueColumn;
    };
  }
}

#endif

