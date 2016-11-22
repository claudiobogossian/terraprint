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
  \file RenderFactoryParamsCreate.h
   
  \brief Parameters to create a new render. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_RENDER_FACTORY_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_RENDER_FACTORY_PARAMS_CREATE_H

// TerraLib
#include "terralib/common/AbstractParameters.h"
#include "../../../../../core/Config.h"

namespace te
{
  namespace layout
  {
    /*!
    \brief Parameters to create a new render. 
    
    \ingroup layout

    \sa te::layout::ParamsCreate
    */
    class TELAYOUTEXPORT RenderFactoryParamsCreate : public te::common::AbstractParameters
    {
      public:
        

        /*! \brief Copy constructor. */
        RenderFactoryParamsCreate(const RenderFactoryParamsCreate& rhs);

        /*!
          \brief Constructor

          \param view
        */ 
        RenderFactoryParamsCreate();

        /*!
          \brief Destructor
        */ 
        virtual ~RenderFactoryParamsCreate();

        te::common::AbstractParameters* clone() const;

        void reset() throw(te::common::Exception);
    };
  }
}
#endif

