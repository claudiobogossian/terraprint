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
  \file WarningManager.h
   
  \brief Class specifies a font.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_WARNINGMANAGER_H 
#define __TERRALIB_LAYOUT_INTERNAL_WARNINGMANAGER_H

// TerraLib
#include "enum/AbstractType.h"

// STL
#include <string>
#include <vector>

namespace te
{
  namespace layout
  {
    /*!
      \brief Class specifies a font.

      \ingroup layout
      */
    class WarningManager
    {
    public:

      /*!
       \brief Constructor
       */
      WarningManager();

      /*!
       \brief Destructor
       */
      virtual ~WarningManager();

      virtual void addWarning(const std::string &warningText);

      virtual void clearWarnings(); 

      virtual std::vector<std::string> getWarnings();

      virtual bool hasWarning();


    private:

      std::vector<std::string> m_warningList;
      bool m_hasWarning;

    };
  }
}
#endif
