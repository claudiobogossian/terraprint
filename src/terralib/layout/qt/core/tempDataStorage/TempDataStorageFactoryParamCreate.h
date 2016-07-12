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
  \file TempDataStorageFactoryParamCreate.h
   
  \brief Parameters to create a new template. 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEMP_DATASTORAGE_FACTORY_PARAMS_CREATE_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEMP_DATASTORAGE_FACTORY_PARAMS_CREATE_H

// TerraLib
#include "terralib/common/AbstractParameters.h"
#include "../../../core/Config.h"

// STL
#include <string>

namespace te
{
  namespace layout
  {
    class AbstractTempDataStorageInfo;
    /*!
    \brief Parameters to create a new temp data storage. 
    
    \ingroup layout

    \sa te::layout::ParamsCreate
    */
    class TELAYOUTEXPORT TempDataStorageFactoryParamCreate : public te::common::AbstractParameters
    {
      public:
        
        /*!
        \brief Constructor

        \param file path
        */
        TempDataStorageFactoryParamCreate(AbstractTempDataStorageInfo* info);

        /*! \brief Copy constructor. */
        TempDataStorageFactoryParamCreate(const TempDataStorageFactoryParamCreate& rhs);

        /*!
          \brief Destructor
        */ 
        virtual ~TempDataStorageFactoryParamCreate();

        te::common::AbstractParameters* clone() const;

        void reset() throw(te::common::Exception);
                
        AbstractTempDataStorageInfo* getTempDataStorageInfo();

      protected:

        AbstractTempDataStorageInfo* m_info; //!< file path
    };
  }
}
#endif

