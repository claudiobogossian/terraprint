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


    Magnetic declination validated from NOAA www.ngdc.noaa.gov/geomag-web/
 */

/*!
  \file ThreeNorthModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ThreeNorthModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"
#include "../core/property/SharedProperties.h"
#include "../core/Utils.h"

#ifdef GEOGRAPHICLIB_ENABLED
  #include <GeographicLib/MagneticModel.hpp>
#endif

te::layout::ThreeNorthModel::ThreeNorthModel()
: AbstractItemModel()
{
  te::gm::Envelope wbox;
  int srid = 0;
  te::color::RGBAColor color(0, 0, 0, 255);
  double meridian_convergence = 0;
  double magnetic_north = 0;
  int date = 2015;
  std::string itemName = "";
  SharedProperties sharedProps;
  double lineWidth = Utils::getLineWidthMinimumValue();

  bool magneticVisible = false;
  bool magneticMenu = false;
  bool magneticValue = false;
  bool dateVisible = false;
  bool dateMenu = false;

#ifdef GEOGRAPHICLIB_ENABLED
  magneticVisible = true;
  magneticMenu = true;
  magneticValue = true;
  dateVisible = true;
  dateMenu = true;
#endif

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  {
    {
      Property property(0);
      property.setName("line_width");
      property.setLabel(TR_LAYOUT("Line Width"));
      property.setVisible(false);
      property.setValue(lineWidth, dataType->getDataTypeDouble());
      this->m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName(sharedProps.getItemObserver());
      property.setLabel(TR_LAYOUT("Connection with"));
      property.setComposeWidget(true);
      property.setValue(itemName, dataType->getDataTypeItemObserver());
      m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName("color");
      property.setLabel(TR_LAYOUT("color"));
      property.setValue(color, dataType->getDataTypeColor());
      this->m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName("meridian_convergence");
      property.setLabel(TR_LAYOUT("Meridian Convergence"));
      property.setValue<bool>(true, dataType->getDataTypeBool());
      property.setMenu(true);
      this->m_properties.addProperty(property);
    }
    {
      Property property(0);
      property.setName("magnetic_north");
      property.setLabel(TR_LAYOUT("Magnetic North"));
      property.setValue(magneticValue, dataType->getDataTypeBool());
      property.setVisible(magneticVisible);
      property.setMenu(magneticMenu);
      this->m_properties.addProperty(property);
    }

   {
      Property property(0);
      property.setName("angle_meridian_convergence");
      property.setLabel(TR_LAYOUT("Angle Meridian Convergence"));
      property.setValue(meridian_convergence, dataType->getDataTypeDouble());
      property.setMenu(false);
      this->m_properties.addProperty(property);
    }

   {
      Property property(0);
      property.setName("angle_magnetic_north");
      property.setLabel(TR_LAYOUT("Angle Magnetic North"));
      property.setValue(magnetic_north, dataType->getDataTypeDouble());
      property.setVisible(magneticVisible);
      property.setMenu(false);
      this->m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName("width");
      property.setValue<double>(110., dataType->getDataTypeDouble());
      m_properties.updateProperty(property);
    }

    {
      Property property(0);
      property.setName("keep_aspect");
      property.setValue<bool>(true, dataType->getDataTypeBool());
      property.setVisible(false);
      m_properties.completelyUpdateProperty(property);
   }

    {
      Property property(0);
      property.setName("date");
      property.setLabel(TR_LAYOUT("Date"));
      property.setValue(date, dataType->getDataTypeInt());
      property.setMenu(false);
      property.setVisible(dateVisible);
      m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName("world_box");
      property.setLabel(TR_LAYOUT("World Box"));
      property.setValue(wbox, dataType->getDataTypeEnvelope());
      m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName("srid");
      property.setLabel(TR_LAYOUT("SRID"));
      property.setValue(srid, dataType->getDataTypeInt());
      m_properties.addProperty(property);
    }
  }
  
  reparentProperties(Enums::getInstance().getEnumObjectType()->getThreeNorthItem());
}

te::layout::ThreeNorthModel::~ThreeNorthModel()
{

}

