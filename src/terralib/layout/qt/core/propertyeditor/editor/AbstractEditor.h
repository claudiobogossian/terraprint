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
  \file AbstractEditor.h
   
   \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_EDITOR_H 
#define __TERRALIB_LAYOUT_INTERNAL_ABSTRACT_EDITOR_H

// TerraLib
#ifndef Q_MOC_RUN
#include "../../../../core/property/Property.h"
#include "../../../../core/Config.h"
#endif

// Qt
#include <QModelIndex>
#include <QVariant>

namespace te
{
  namespace layout
  {
    class View;
    class EnumType;

    /*!
    \brief 
    
    \ingroup layout
    */
    class TELAYOUTEXPORT AbstractEditor 
    {
      public:

        AbstractEditor(const QModelIndex& index, EnumType* type);

        virtual ~AbstractEditor();
        
        virtual void setEditorData(const QModelIndex& index);

        te::layout::Property getProperty();

        virtual QVariant getValue() = 0;

        virtual EnumType* getType();

        /*!
          \brief Value acquired after registration via meta type that QVariant recognizes objects of type te::layout::Property

          \return role value of te::layout::Property
        */
        int getRole();

      protected:

        /*
          \brief Checks if the data type is expected and set the new value in editor.
        */
        virtual void changeEditorData(const QModelIndex& index) = 0;

      protected:
        
        te::layout::Property    m_property;
        EnumType*               m_dataType;
    };
  }
}

#endif

