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
  \file AlignItems.h
   
  \brief Class applying the alignment in one or more components. Ex .: send to back, bring to front, align right, align bottom.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_ICON_MANAGER_H
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_ICON_MANAGER_H

// TerraLib
#include "../../core/Config.h"

// STL
#include <map>
#include <string>

class QString;

namespace te
{
  namespace layout
  {
    class Scene;
    /*!
    \brief Class applying the alignment in one or more components. Ex .: send to back, bring to front, align right, align bottom.
    
    \ingroup layout
    */
    class TELAYOUTEXPORT ItemIconManager
    {
      public:

        /*!
          \brief Constructor
        */
        ItemIconManager();

        /*!
          \brief Destructor
        */
        virtual ~ItemIconManager();

        void setIcon(const std::string& itemTypeName, const std::string& itemName);

        std::string getIconName(const std::string& itemTypeName) const;

        QString getIconNameAsQString(const std::string& itemTypeName) const;

      protected:

        std::map<std::string, std::string> m_mapItemIcons; //A map defining the icons for each item
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ITEM_ICON_MANAGER_H
