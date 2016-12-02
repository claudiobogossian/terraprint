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
  \file EditorFactoryParamsCreate.h
   
  \brief Parameters to create a new editor. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_EDITOR_FACTORY_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_EDITOR_FACTORY_PARAMS_CREATE_H

// TerraLib
#include <terralib/common/AbstractParameters.h>
#include "../../../../../../core/Config.h"
#include "../../../../../../core/property/Property.h"

// STL
#include <vector>

// Qt
#include <QModelIndex>

class QWidget;

namespace te
{
  namespace layout
  {
    class EnumType;
    class ContextPropertyEditor;
    /*!
    \brief Parameters to create a new tool. 
    
    \ingroup layout

    \sa te::layout::ParamsCreate
    */
    class TELAYOUTEXPORT EditorFactoryParamsCreate : public te::common::AbstractParameters
    {
      public:
        

        /*! \brief Copy constructor. */
        EditorFactoryParamsCreate(const EditorFactoryParamsCreate& rhs);

        /*!
          \brief Constructor

          \param view
        */ 
        EditorFactoryParamsCreate(const QModelIndex& index, std::vector<Property> vprops, 
          ContextPropertyEditor* context, QWidget* parent = 0);

        /*!
          \brief Destructor
        */ 
        virtual ~EditorFactoryParamsCreate();

        te::common::AbstractParameters* clone() const;

        void reset() throw(te::common::Exception);

        std::vector<Property> getProperties() const;

        /*!
          \brief Returns the QModelIndex of the QTreeWidget

          \return QModelIndex
        */
        QModelIndex getModelIndex() const;

        QWidget* getParent();

        ContextPropertyEditor* getContextPropertyEditor();
        
      protected:

        std::vector<Property>           m_vprops;
        QModelIndex                     m_index; //!< Model Index from QTreeWidget
        ContextPropertyEditor*          m_context;
        QWidget*                        m_parent;
    };
  }
}
#endif

