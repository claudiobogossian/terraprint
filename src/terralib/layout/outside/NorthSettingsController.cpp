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
  \file NorthSettingsController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "NorthSettingsController.h"
#include "../core/pattern/mvc/AbstractOutsideModel.h"
#include "NorthSettingsModel.h"

te::layout::NorthSettingsController::NorthSettingsController(AbstractOutsideModel* o) :
  AbstractOutsideController(o)
{
  
}

te::layout::NorthSettingsController::~NorthSettingsController()
{

}

te::layout::Property te::layout::NorthSettingsController::updateProperty(std::string name, Variant variant, EnumType* enumType)
{
  NorthSettingsModel* outsideModel = 0;

  if(m_model)
  {
    AbstractOutsideModel* model = dynamic_cast<AbstractOutsideModel*>(m_model);
    outsideModel = dynamic_cast<NorthSettingsModel*>(model);    
  }
 
  if(!outsideModel)
  {
    Property prop;
    return prop;
  }
  
  Property property = outsideModel->containsProperty(name, enumType);

  if(!property.isNull())
  {
    if(property.getOptionChoices().empty())
    {
      if(property.getValue() != variant)
      {
        property.setValue(variant);
        outsideModel->updateProperty(property, enumType);
      }
      else
      {
        return property;
      }
    }
    else
    {
      if(property.getOptionByCurrentChoice() != variant)
      {
        property.setOptionChoice(variant);
        outsideModel->updateProperty(property, enumType);
      }
      else
      {
        return property;
      }
    }
  } 

  return property;
}

te::layout::Property te::layout::NorthSettingsController::getProperty(std::string name, EnumType* enumType)
{
  NorthSettingsModel* outsideModel = 0;
  Property prop;

  if(m_model)
  {
    AbstractOutsideModel* model = dynamic_cast<AbstractOutsideModel*>(m_model);
    outsideModel = dynamic_cast<NorthSettingsModel*>(model);
  }

  if(!outsideModel)
    return prop;

  prop = outsideModel->containsProperty(name, enumType);

  return prop;
}


