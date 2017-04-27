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
\file Grid.h

\brief A class that handles the creation and drawing of grids.
\ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_PLANARGRID_H
#define __TERRALIB_LAYOUT_INTERNAL_PLANARGRID_H

// TerraLib
#include "Grid.h"

#include "../../core/Config.h"

#include <cstdlib>
#include <vector>

namespace te
{
  namespace gm
  {
    class Envelope;
  }

  namespace layout
  {
    class Properties;

    /*!
    \brief Class that represents a grid.
    */
    class TELAYOUTEXPORT PlanarGrid : public Grid
    {
      public:

        /*!
        \brief Constructor
        */
        PlanarGrid();

        /*!
        \brief Destructor
        */
        virtual ~PlanarGrid();

        void initialize(const te::layout::Properties& gridSettings);

    protected:

        virtual double getUnit(const std::string& strUnit) const;

        //! Calculates the horizontal lines
        virtual std::vector<te::gm::LineString> calculateHorizontalLines(const te::gm::Envelope& planarBox, const te::gm::Envelope& boxMM, const Properties& gridSettings);
        
        //! Calculates the vertical lines
        virtual std::vector<te::gm::LineString> calculateVerticalLines(const te::gm::Envelope& planarBox, const te::gm::Envelope& boxMM, const Properties& gridSettings);
    };
  }
}


#endif //__TERRALIB_LAYOUT_INTERNAL_PLANARGRID_H