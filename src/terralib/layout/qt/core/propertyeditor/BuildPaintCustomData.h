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
  \file BuildPaintCustomData.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BUILD_PAINT_CUSTOM_DATA_H
#define __TERRALIB_LAYOUT_INTERNAL_BUILD_PAINT_CUSTOM_DATA_H

//TerraLib
#include "../../../core/Config.h"

// Qt
class QTreeWidgetItem;
class QPainter;
class QStyleOptionViewItem;
class QModelIndex;
class QString;
class QVariant;

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
    class TELAYOUTEXPORT BuildPaintCustomData
    {
      public:
        /*!
          \brief Constructor
        */
        BuildPaintCustomData();

        /*!
          \brief Destructor
        */
        virtual ~BuildPaintCustomData();
        
        /*!
          \brief Method to build a editor to a property inside a cell of QTreeWidget.
      
          \param vprops properties
      
          \return new editor
        */
        virtual void build();

        virtual bool paintCustomData(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index);

        QString intToString(QVariant& value);

        QString doubleToString(QVariant& value);

        QString stringToString(QVariant& value);

        QString boolToString(QVariant& value);

        QString fontToString(QVariant& value);

        QString rectToString(QVariant& value);

        QString colorToString(QVariant& value);
    };
  }
}

#endif

