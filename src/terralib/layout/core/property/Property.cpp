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
  \file Property.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "../enum/Enums.h"
#include "Property.h"

#include <terralib/common/Exception.h>

// STL
#include <vector>
#include <algorithm>

te::layout::Property::Property( int parentItemHashCode ) :
  m_parentItemHashCode(parentItemHashCode),
  m_name("unknown"),
  m_type(0),
  m_editable(true),
  m_label(""),
  m_menu(false),
  m_icon(""),
  m_visible(true),
  m_required(false),
  m_composeWidget(false),
  m_public(false),
  m_serializable(true),
  m_usePrecision(false),
  m_precisionValue(2)
{
  m_type = Enums::getInstance().getEnumDataType()->getDataTypeNone();
}

te::layout::Property::Property(const std::string& propertyName, te::dt::AbstractData* data, EnumType* type)
  : m_parentItemHashCode(0)
  , m_name(propertyName)
  , m_type(type)
  , m_editable(true)
  , m_label("")
  , m_menu(false)
  , m_icon("")
  , m_visible(true)
  , m_required(false)
  , m_composeWidget(false)
  , m_public(false)
  , m_serializable(true)
  , m_usePrecision(false)
  , m_precisionValue(2)
  , m_data(data)
{
}

te::layout::Property::Property(const te::layout::Property& rhs)
{
  *this = rhs;
}

te::layout::Property::~Property()
{
  
}

const std::string& te::layout::Property::getName() const
{
  return m_name;
}

void te::layout::Property::setName( const std::string& name )
{
  m_name = name;
}

te::layout::EnumType* te::layout::Property::getType() const
{
  return m_type;
}

void te::layout::Property::setValue(te::dt::AbstractData* data, EnumType* type, bool usePrecision, int precisionValue)
{
  m_data.reset(data);
  m_type = type;

  setUsePrecision(usePrecision);
  setPrecision(precisionValue);
}

const te::dt::AbstractData* te::layout::Property::getValue() const
{
  return m_data.get();
}

void te::layout::Property::addOption(const Variant& variant)
{
  if(m_options.empty())
  {
    m_currentChoice = variant;
  }

  m_options.push_back(variant);
}

void te::layout::Property::removeOption(const Variant& variant)
{
  for(std::vector<Variant>::iterator it = m_options.begin(); it != m_options.end(); ++it)
  {
    if((*it) == variant)
    {
      m_options.erase(it);
      break;
    }
  }
}

void te::layout::Property::setOptionChoice(const Variant& variant)
{
  m_currentChoice = variant; 
}

const te::layout::Variant& te::layout::Property::getOptionByCurrentChoice() const
{
  return m_currentChoice;
}

const std::vector<te::layout::Variant>& te::layout::Property::getOptionChoices() const
{
  return m_options;
}

bool te::layout::Property::isEditable() const
{
  return m_editable;
}

void te::layout::Property::setEditable( bool editable )
{
  m_editable = editable;
}

void te::layout::Property::addSubProperty( const Property& property )
{
  m_subProperty.push_back(property);
}

bool te::layout::Property::addSubProperty(const Property& parent, const Property& subProperty)
{
  const std::vector<te::layout::Property>& subProperties = parent.getSubProperty();
  std::size_t total = subProperties.size();
  std::size_t totalResult = 0;

  if (std::find(m_subProperty.begin(), m_subProperty.end(), parent) != m_subProperty.end())
  {
    std::vector<Property>::iterator it = std::find(m_subProperty.begin(), m_subProperty.end(), parent);
    it->addSubProperty(subProperty);
    totalResult = it->getSubProperty().size();
  }
  else
  {
    for (std::vector<te::layout::Property>::iterator itSub = m_subProperty.begin(); itSub != m_subProperty.end(); ++itSub)
    {
      itSub->addSubProperty(parent, subProperty);
    }
  }

  if (totalResult > total)
    return true;
  return false;
}
void te::layout::Property::removeSubProperty( const Property& property )
{
  for(std::vector<Property>::iterator it = m_subProperty.begin(); it != m_subProperty.end(); ++it)
  {
    if((*it) == property)
    {
      m_subProperty.erase(it);
      break;
    }
    else
    {
      it->removeSubProperty(property);
    }
  }
}

bool te::layout::Property::removeSubProperty(const std::string& name)
{
  bool is_removeProp = false;

  Property property;
  property.setName(name);

  if (m_subProperty.empty())
    return is_removeProp;

  if (std::find(m_subProperty.begin(), m_subProperty.end(), property) != m_subProperty.end())
  {
    std::vector<Property>::iterator it = std::find(m_subProperty.begin(), m_subProperty.end(), property);
    property = (*it);
    is_removeProp = true;
  }
  else
  {
    for (std::vector<Property>::iterator itSub = m_subProperty.begin(); itSub != m_subProperty.end(); ++itSub)
    {
      is_removeProp = itSub->removeSubProperty(name);
      if (is_removeProp)
      {
        break;
      }
    }
  }
  return is_removeProp;
}

const std::vector<te::layout::Property>& te::layout::Property::getSubProperty() const
{
  return m_subProperty;
}

bool te::layout::Property::updateSubProperty(const Property& property)
{
  bool result = false;

  if (m_subProperty.empty())
    return result;

  for (std::vector<te::layout::Property>::iterator it = m_subProperty.begin(); it != m_subProperty.end(); ++it)
  {
    if ((*it) == property)
    {
      it->setValue(property.getValue()->clone(), property.getType());
      it->setOptionChoice(property.getOptionByCurrentChoice());
      result = true;
    }
    else
    {
      it->updateSubProperty(property);
    }
  }

  return result;
}

bool te::layout::Property::completelyUpdateSubProperty(const Property& property)
{
  bool result = false;

  for (std::vector<te::layout::Property>::iterator it = m_subProperty.begin(); it != m_subProperty.end(); ++it)
  {
    if ((*it) == property)
    {
      it->setValue(property.getValue()->clone(), property.getType());
      it->setOptionChoice(property.getOptionByCurrentChoice());
      it->setEditable(property.isEditable());
      it->setLabel(property.getLabel());
      it->setMenu(property.isMenu());
      it->setIcon(property.getIcon());
      it->setVisible(property.isVisible());
      it->setRequired(property.isRequired());
      it->setParentItemHashCode(property.getParentItemHashCode());
      it->setComposeWidget(property.isComposeWidget());
      it->setPublic(property.isPublic());
      it->setSerializable(property.isSerializable());
      result = true;
    }
    else
    {
      it->updateSubProperty(property);
    }
  }
  return result;
}

bool te::layout::Property::isNull() const
{
  if (m_data.get() == 0)
  {
    return true;
  }

  return false;
}

bool te::layout::Property::containsSubProperty(const std::string& name) const
{
  if (m_subProperty.empty())
  {
    return false;
  }

  if (std::find(m_subProperty.begin(), m_subProperty.end(), name) != m_subProperty.end())
  {
    return true;
  }
  else
  {
    for (std::vector<Property>::const_iterator itSub = m_subProperty.begin(); itSub != m_subProperty.end(); ++itSub)
    {
      if(itSub->containsSubProperty(name))
      {
        return true;
      }
    }
  }

  return false;
}

const te::layout::Property& te::layout::Property::getSubProperty( const std::string& name ) const
{
  if (m_nullProperty.get() == 0)
  {
    m_nullProperty.reset(new Property);
  }

  if (m_subProperty.empty())
  {
    Property* p = m_nullProperty.get();
    return *p;
  }

  if(std::find(m_subProperty.begin(), m_subProperty.end(), name) != m_subProperty.end())
  {
    std::vector<Property>::const_iterator it = std::find(m_subProperty.begin(), m_subProperty.end(), name);
    return (*it);
  }
  else
  {
    for (std::vector<Property>::const_iterator itSub = m_subProperty.begin(); itSub != m_subProperty.end(); ++itSub)
    {
      if (itSub->containsSubProperty(name) == 0)
      {
        const Property& prop = itSub->getSubProperty(name);
        return prop;
      }
    }
  }
  
  throw te::common::Exception("Property::The given sub property name was not found in the sub property list");
}

bool te::layout::Property::equals(const Property& property) const
{
  bool result = true;

  if (m_subProperty.size() != property.getSubProperty().size())
  {
    return false;
  }
  
  if (this->getValue()->toString() == property.getValue()->toString())
  {
    for (std::vector<Property>::const_iterator it = m_subProperty.begin(); it != m_subProperty.end(); ++it)
    {
      Property prop = (*it);
      std::string name = prop.getName();
      if (property.containsSubProperty(name) == true)
      {
        Property otherProp = property.getSubProperty(name);
        // will also check values of the sub properties
        if (prop.equals(otherProp) == false)
        {
          result = false;
          break;
        }
      }
      else
      {
        result = false;
        break;
      }
    }
  }
  else
  {
    result = false;
  }
  return result;
}

void te::layout::Property::clear()
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  m_name = "unknown";
  m_editable = true;
  m_type = dataType->getDataTypeNone();
  m_currentChoice.clear();
  m_options.clear();
  m_subProperty.clear();
  m_menu = false;
  m_icon = "";
  m_required = false;
  m_composeWidget = false;
  m_label = "";
  m_data.reset();
}

void te::layout::Property::setLabel( std::string label )
{
  m_label = label;
}

const std::string& te::layout::Property::getLabel() const
{
  return m_label;
}

void te::layout::Property::setMenu( bool menu )
{
  m_menu = menu;
}

bool te::layout::Property::isMenu() const
{
  return m_menu;
}

void te::layout::Property::setIcon( const std::string& icon )
{
  m_icon = icon;
}

const std::string& te::layout::Property::getIcon() const
{
  return m_icon;
}

void te::layout::Property::setVisible( bool visible )
{
  m_visible = visible;
}

bool te::layout::Property::isVisible() const
{
  return m_visible;
}

bool te::layout::Property::isRequired() const
{
  return m_required;
}

void te::layout::Property::setRequired( bool required )
{
  m_required = required;
}

void te::layout::Property::setParentItemHashCode( int hashCode )
{
  m_parentItemHashCode = hashCode;
}

int te::layout::Property::getParentItemHashCode() const
{
  return m_parentItemHashCode;
}

bool te::layout::Property::isComposeWidget() const
{
  return m_composeWidget;
}

void te::layout::Property::setComposeWidget( bool compose )
{
  m_composeWidget = compose;
}

bool te::layout::Property::isPublic() const
{
  return m_public;
}

void te::layout::Property::setPublic( bool publicProperty )
{
  /*
  If the component, father of this property, is a child of another component, 
  then this property can be used by the parent component to display the value or call windows. It can not be edited.
  */
  m_public = publicProperty;
  if(m_public)
  {
    m_editable = false;
  }
}

bool te::layout::Property::isSerializable() const
{
  return m_serializable;
}

void te::layout::Property::setSerializable(bool serializable)
{
  m_serializable = serializable;
}


const std::string& te::layout::Property::getParent() const
{
  return m_parentClass;
}

void te::layout::Property::setParent(const std::string& parentClass)
{
  m_parentClass = parentClass;

  for (std::vector<Property>::iterator it = this->m_subProperty.begin(); it != this->m_subProperty.end(); ++it)
  {
    it->setParent(parentClass);
  }
}

bool te::layout::Property::getUsePrecision() const
{
  return m_usePrecision;
}

void te::layout::Property::setUsePrecision(bool usePrecision)
{
  m_usePrecision = usePrecision;
}

int te::layout::Property::getPrecision() const
{
  return m_precisionValue;
}

void te::layout::Property::setPrecision(int precision)
{
  m_precisionValue = precision;
}
