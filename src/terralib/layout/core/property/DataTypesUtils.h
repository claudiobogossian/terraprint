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
\file DataTypesUtils.h

\brief This file defines the enum and typedefs for all basic types in the layout module

\ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DATATYPESUTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_DATATYPESUTILS_H 

#include "LayoutData.h"
#include "TypeManager.h"

//Terralib
#include <terralib/common/Exception.h>

namespace te
{
  namespace layout
  {
    void registerLayoutTypes();

    template<typename T> 
    te::dt::AbstractData* CreateData(const T& value)
    {
      te::dt::AbstractData* abstractData = new LayoutData<T>(value, TypeManager::getInstance());
      return abstractData;
    }

    template<typename T>
    const T& GetValueAs(const te::dt::AbstractData* absData)
    {
      const LayoutData<T>* castData = dynamic_cast<const LayoutData<T>*>(absData);
      if (castData == 0)
      {
        throw te::common::Exception("Property: invalid type cast");
      }

      return castData->getValue();
    }
  }
}


#endif //__TERRALIB_LAYOUT_INTERNAL_DATATYPESUTILS_H 
