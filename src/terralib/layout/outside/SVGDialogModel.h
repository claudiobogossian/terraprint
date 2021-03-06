/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file SVGDialogModel.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SVG_DIALOG_MODEL_H 
#define __TERRALIB_LAYOUT_INTERNAL_SVG_DIALOG_MODEL_H

// TerraLib
#include "../core/pattern/mvc/AbstractOutsideModel.h"
#include "../core/Config.h"
#include "../core/property/Property.h"

// STL
#include <vector>
#include <list>

namespace te
{
  namespace layout
  {
    class Properties;

    class TELAYOUTEXPORT SVGDialogModel : public AbstractOutsideModel
    {
    public:

      SVGDialogModel();

      virtual ~SVGDialogModel();

      virtual void updateProperties(te::layout::Properties* properties, bool notify = true);

      virtual Properties* getProperties() const;

      virtual void setPathsProperty(Property property);

      virtual std::vector<std::string> getPaths();

      virtual te::layout::Property containsOutsideSubProperty(std::string name);
    
    protected:

      Property m_svgProperty;

      Property m_property;

    };
  }
}

#endif

