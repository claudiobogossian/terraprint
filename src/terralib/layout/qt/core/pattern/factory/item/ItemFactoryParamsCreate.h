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
  \file ItemFactoryParamsCreate.h
   
  \brief Parameters to create a new item object (MVC graphic Object). 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_FACTORY_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_FACTORY_PARAMS_CREATE_H

// TerraLib
#include "terralib/common/AbstractParameters.h"
#include "../../../../../core/property/Properties.h"
#include "../../../../../core/Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class ItemInputProxy;

    /*!
      \brief Parameters to create a new item object (MVC graphic Object).  
    
      \ingroup layout

      \sa te::common::AbstractParameters
    */
    class TELAYOUTEXPORT ItemFactoryParamsCreate : public te::common::AbstractParameters
    {
      public:
        
        /*! \brief Copy constructor. */
        ItemFactoryParamsCreate(const ItemFactoryParamsCreate& rhs);
        
        /*
          \brief Constructor

          \param Properties of a item that will be constructed.
        */
        explicit ItemFactoryParamsCreate(const Properties& props, te::layout::ItemInputProxy* itemInputProxy);

        /*!
          \brief Destructor
        */ 
        virtual ~ItemFactoryParamsCreate();

        te::common::AbstractParameters* clone() const;

        void reset() throw(te::common::Exception);
        
        const Properties& getProperties() const;

        te::layout::ItemInputProxy* getItemInputProxy() const;

      protected:

        Properties                  m_props; //!< Properties of a graphic object.
        te::layout::ItemInputProxy* m_itemInputProxy;
    };
  }
}
#endif

