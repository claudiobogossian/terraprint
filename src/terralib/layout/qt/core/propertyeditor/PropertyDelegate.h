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
  \file PropertyDelegate.h
   
   \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PROPERTY_DELEGATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_PROPERTY_DELEGATE_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../core/property/Property.h"
#include "../../../core/Config.h"
#endif

// STL
#include <vector>

// Qt
#include <QItemDelegate>

namespace te
{
  namespace layout
  {
    class View;

    /*!
    \brief 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT PropertyDelegate : public QItemDelegate
    {
      Q_OBJECT //for slots/signals

      public:

        PropertyDelegate(QObject* parent = 0);

        virtual ~PropertyDelegate();

        virtual void setProperties(std::vector<Property> vprops);

        virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;

        virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;

        virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

        virtual void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        
      signals:

        void propertiesChanged(std::vector<Property> vprops);

      protected slots:

        void onPropertiesChanged(std::vector<Property> vprops);

      protected:

        virtual QWidget* createFromFactory(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
        
      protected:
        
        std::vector<Property>   m_vprops;
    };
  }
}

#endif


