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
\file DataConverters.h

\brief This file contains several data-type converter functions

\ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_DATACONVERTERS_H
#define __TERRALIB_LAYOUT_INTERNAL_DATACONVERTERS_H

#include <boost/lexical_cast.hpp>
#include <terralib/layout/core/property/DataTypesUtils.h>

namespace te
{
  namespace dt
  {
    class AbstractData;
  }

  namespace layout
  {
    template <typename T>
    te::dt::AbstractData* ConvertToStringUsingBoost(te::dt::AbstractData* abstractData)
    {
      T inputValue = GetValueAs<T>(abstractData);
      std::string outputValue = boost::lexical_cast<std::string>(inputValue);
      return CreateData(outputValue);
    }

    template <typename T>
    te::dt::AbstractData* ConvertFromStringUsingBoost(te::dt::AbstractData* abstractData)
    {
      std::string inputValue = GetValueAs<std::string>(abstractData);
      T outputValue = boost::lexical_cast<T>(inputValue);
      return CreateData(outputValue);
    }

    //!< Bool to String converter
    te::dt::AbstractData* ConvertBoolToString(te::dt::AbstractData* abstractData);

    //!< String to Bool converter
    te::dt::AbstractData* ConvertStringToBool(te::dt::AbstractData* abstractData);

    //!< Color to String converter
    te::dt::AbstractData* ConvertColorToString(te::dt::AbstractData* abstractData);

    //!< String to Color converter
    te::dt::AbstractData* ConvertStringToColor(te::dt::AbstractData* abstractData);

    //!< Double to String converter
    te::dt::AbstractData* ConvertDoubleToString(te::dt::AbstractData* abstractData);

    //!< String to Double converter
    te::dt::AbstractData* ConvertStringToDouble(te::dt::AbstractData* abstractData);

    //!< Envelope to String converter
    te::dt::AbstractData* ConvertEnvelopeToString(te::dt::AbstractData* abstractData);

    //!< String to Envelope converter
    te::dt::AbstractData* ConvertStringToEnvelope(te::dt::AbstractData* abstractData);

    //!< Font to String converter
    te::dt::AbstractData* ConvertFontToString(te::dt::AbstractData* abstractData);

    //!< String to Font converter
    te::dt::AbstractData* ConvertStringToFont(te::dt::AbstractData* abstractData);

    //!< Geometry to String converter
    te::dt::AbstractData* ConvertGeometryToString(te::dt::AbstractData* abstractData);

    //!< String to Geometry converter
    te::dt::AbstractData* ConvertStringToGeometry(te::dt::AbstractData* abstractData);

    //!< Int to String converter
    te::dt::AbstractData* ConvertIntToString(te::dt::AbstractData* abstractData);

    //!< String to Int converter
    te::dt::AbstractData* ConvertStringToInt(te::dt::AbstractData* abstractData);

    //!< StringList to String converter
    te::dt::AbstractData* ConvertStringListToString(te::dt::AbstractData* abstractData);

    //!< String to StringList converter
    te::dt::AbstractData* ConvertStringToStringList(te::dt::AbstractData* abstractData);

    //!< MatrixList to String converter
    te::dt::AbstractData* ConvertStringMatrixToString(te::dt::AbstractData* abstractData);

    //!< String to StringMatrix converter
    te::dt::AbstractData* ConvertStringToStringMatrix(te::dt::AbstractData* abstractData);

    //!< AbstractLayerList to String converter
    te::dt::AbstractData* ConvertAbstractLayerListToString(te::dt::AbstractData* abstractData);

    //!< String to AbstractLayerList converter
    te::dt::AbstractData* ConvertStringToAbstractLayerList(te::dt::AbstractData* abstractData);
  }
}


#endif //__TERRALIB_LAYOUT_INTERNAL_DATACONVERTERS_H 
