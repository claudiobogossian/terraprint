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
  \file SVGDialogModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SVGDialogModel.h"
#include "../core/property/Properties.h"
#include "../core/enum/Enums.h"

// STL
#include <algorithm>

te::layout::SVGDialogModel::SVGDialogModel() :
  AbstractOutsideModel()
{
  m_type = Enums::getInstance().getEnumObjectType()->getSVGDialog();
  m_box = te::gm::Envelope(0., 0., 200., 200.);
}

te::layout::SVGDialogModel::~SVGDialogModel()
{

}

te::layout::Properties* te::layout::SVGDialogModel::getProperties() const
{
  m_properties->clear();

  Property pro_name(m_hashCode);
  pro_name.setName(m_name);

  m_properties->addProperty(pro_name);

  m_properties->setTypeObj(m_type);
  return m_properties;
}

void te::layout::SVGDialogModel::updateProperties( te::layout::Properties* properties, bool notify )
{

}

void te::layout::SVGDialogModel::setPathsProperty(Property property)
{
  m_svgProperty = property;
}

std::vector<std::string> te::layout::SVGDialogModel::getPaths()
{
  std::vector<std::string> pathsName;

  if (m_svgProperty.getSubProperty().empty())
    return pathsName;

  const std::vector<Property>& props = m_svgProperty.getSubProperty();

  if (props.empty())
    return pathsName;

  const Property& pro_paths = props[0];
  const std::vector<std::string>& vecString = te::layout::Property::GetValueAs< std::vector<std::string> >(pro_paths);

  return vecString;
}

te::layout::Property te::layout::SVGDialogModel::containsOutsideSubProperty(std::string name)
{
  return m_property.getSubProperty(name);
}