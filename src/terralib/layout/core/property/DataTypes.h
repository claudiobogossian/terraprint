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
\file DataTypes.h

\brief This file defines the enum and typedefs for all basic types in the layout module

\ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DATATYPES_H 
#define __TERRALIB_LAYOUT_INTERNAL_DATATYPES_H 

#include <terralib/datatype/Enums.h>

namespace te
{
  namespace layout
  {
    enum class LayoutTypes
    {
      BOOLEAN_TYPE = te::dt::LAST_COMMON_DATATYPE_CODE + 100
      , COLOR_TYPE
      , DOUBLE_TYPE
      , ENVELOPE_TYPE
      , FONT_TYPE
      , GEOMETRY_TYPE
      , INT_TYPE
      , STRING_TYPE
      , STRING_LIST_TYPE
      , STRING_MATRIX_TYPE
      , LAYER_LIST_TYPE
    };
  }
}


#endif //__TERRALIB_LAYOUT_INTERNAL_DATATYPES_H
