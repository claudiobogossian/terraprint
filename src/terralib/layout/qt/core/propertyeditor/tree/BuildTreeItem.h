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
  \file BuildTreeItem.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BUILD_TREE_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_BUILD_TREE_ITEM_H

//TerraLib
#include "../../../../core/Config.h"

// Qt
class QTreeWidgetItem;

namespace te
{
  namespace layout
  {
    class Property;
    class PropertyTreeItem;

    /*!
    \brief 
    
      \ingroup layout
    */
    class TELAYOUTEXPORT BuildTreeItem
    {
      public:
        /*!
          \brief Constructor
        */
        BuildTreeItem();

        /*!
          \brief Destructor
        */
        virtual ~BuildTreeItem();
        
        /*!
          \brief Method to build a editor to a property inside a cell of QTreeWidget.
      
          \param vprops properties
      
          \return new editor
        */
        virtual PropertyTreeItem* buildTreeItem(Property & prop, QTreeWidgetItem* parent);
    };
  }
}

#endif

