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
  \file EnvelopePropertyTreeItem.h
   
   \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ENVELOPE_PROPERTY_TREE_ITEM_H 
#define __TERRALIB_LAYOUT_INTERNAL_ENVELOPE_PROPERTY_TREE_ITEM_H

// TerraLib
#include "../../../core/property/Property.h"
#include "../../../core/Config.h"
#include "PropertyTreeItem.h"

// STL
#include <vector>

// Qt
#include <QString>
class QVariant;

namespace te
{
  namespace layout
  {
    class EnumType;

    /*!
    \brief 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT EnvelopePropertyTreeItem : public PropertyTreeItem
    {
      public:

        EnvelopePropertyTreeItem(Property & prop, QTreeWidgetItem* parent = 0);

        virtual ~EnvelopePropertyTreeItem();
        
        virtual void refresh(int column, int role, QString name, QVariant value, QTreeWidgetItem* childItem);

      protected:

        QString   m_x1Name;
        QString   m_y1Name;
        QString   m_x2Name;
        QString   m_y2Name;
    };
  }
}

#endif

