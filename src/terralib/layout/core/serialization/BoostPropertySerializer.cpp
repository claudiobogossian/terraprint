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
  \file BoostPropertySerializer.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BoostPropertySerializer.h"
#include "../PaperConfig.h"
#include "../property/Property.h"
#include "../property/Properties.h"
#include "../property/TypeManager.h"
#include "terralib/common/CharEncodingConv.h"
#include "terralib/common/Exception.h"
#include "terralib/common/STLUtils.h"
#include "terralib/common/Translator.h"
#include "../Config.h"
#include "../enum/Enums.h"

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/system/system_error.hpp>

// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>  

te::layout::BoostPropertySerializer::BoostPropertySerializer() 
{

}

te::layout::BoostPropertySerializer::~BoostPropertySerializer()
{

}

bool te::layout::BoostPropertySerializer::isEmpty()
{
  return m_array.empty();
}

void te::layout::BoostPropertySerializer::loadFromPath( std::string loadPath )
{
  m_loadPath = loadPath;

  std::ifstream inputFile;

  try 
  {     
    inputFile.open(m_loadPath.c_str());

    if (!inputFile.is_open())
      return;

    boost::property_tree::json_parser::read_json(inputFile, m_array);
    inputFile.close();
  }
  catch(boost::property_tree::json_parser::json_parser_error &je)
  {
    std::string errmsg = "Error parsing: " + je.filename() + ": " + je.message();
    te::common::Exception ex(TE_TR(errmsg));
    //throw(ex);
    return;
  }
  catch (std::ifstream::failure &e) 
  {
    std::cerr << e.what() << std::endl;
    std::string errmsg = "Exception opening/reading/closing file: \n ";
    te::common::Exception ex(TE_TR(errmsg));
    //throw(ex);
    return;
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

boost::property_tree::ptree te::layout::BoostPropertySerializer::encode(const te::layout::PaperConfig& paperConfig, const std::vector<te::layout::Properties>& vecProperties, const std::map< std::string, std::vector<std::string> >& mapGroups)
{
  double paperWidth = 0.;
  double paperHeight = 0.;
  paperConfig.getPaperSize(paperWidth, paperHeight);

  boost::property_tree::ptree rootNode; //the root node contains the metadata of the template and the list of the items
  rootNode.add("name", m_rootKey);
  
  boost::property_tree::ptree paperNode;
  
  paperNode.add("orientation", paperConfig.getPaperOrientantion());
  paperNode.add("width", paperWidth);
  paperNode.add("height", paperHeight);
  paperNode.add("type", paperConfig.getPaperType());
  
  rootNode.push_back(std::make_pair("paper", paperNode));

  //for all the items
  boost::property_tree::ptree itemsNode;
  for (size_t i = 0; i < vecProperties.size(); ++i)
  {    
    const Properties& properties = vecProperties[i];

    const std::vector<te::layout::Property>& vecProperty = properties.getProperties();
    if (vecProperty.empty())
      continue;

    const std::string& objectName = te::layout::Property::GetValueAs<std::string>(properties.getProperty("name"));

    boost::property_tree::ptree vecPropertyNode = encode(vecProperty);
    
    boost::property_tree::ptree propertiesNode;
    propertiesNode.add("object_type", properties.getTypeObj()->getName());
    propertiesNode.push_back(std::make_pair("properties", vecPropertyNode));

    itemsNode.push_back(std::make_pair("item", propertiesNode));
  }
  rootNode.push_back(std::make_pair("items", itemsNode));

  //for all the groups
  boost::property_tree::ptree groupsNode;
  std::map< std::string, std::vector<std::string> >::const_iterator itGroups = mapGroups.begin();
  while (itGroups != mapGroups.end())
  {
    const std::string& groupName = itGroups->first;
    const std::vector<std::string>& vecItems = itGroups->second;

    
    boost::property_tree::ptree itemsListNode;
    for (size_t i = 0; i < vecItems.size(); ++i)
    {
      boost::property_tree::ptree itemNode;
      itemNode.add("name", vecItems[i]);

      itemsListNode.push_back(std::make_pair("item", itemNode));
    }

    boost::property_tree::ptree groupNode;
    groupNode.add("name", groupName);
    groupNode.push_back(std::make_pair("items", itemsListNode));

    groupsNode.push_back(std::make_pair("group", groupNode));
    ++itGroups;
  }

  rootNode.push_back(std::make_pair("groups", groupsNode));

  boost::property_tree::ptree templateNode;
  templateNode.push_back(std::make_pair("template", rootNode));

  return templateNode;
}

bool te::layout::BoostPropertySerializer::decode(const boost::property_tree::ptree& tree, te::layout::PaperConfig& oPaperConfig, std::vector<te::layout::Properties>& oProperties, std::map< std::string, std::vector<std::string> >& oMapGroups)
{
  //we first read the information about the paper
  const boost::property_tree::ptree& paperNode = tree.get_child("template.paper");
  int orientation = paperNode.get<int>("orientation");
  double width = paperNode.get<double>("width");
  double height = paperNode.get<double>("height");
  int type = paperNode.get<int>("type");

  oPaperConfig.setPaperType((te::layout::LayoutAbstractPaperType)type);
  oPaperConfig.setPaperOrientation((te::layout::LayoutOrientationType)orientation);
  oPaperConfig.setPaperSizeCustom(width, height);
  

  //then we read the information about the items
  oProperties.clear();

  const boost::property_tree::ptree& itemsNode = tree.get_child("template.items");
  boost::property_tree::ptree::const_iterator itItems = itemsNode.begin();
  while (itItems != itemsNode.end())
  {
    Properties properties = decodeItem(itItems->second);
    oProperties.push_back(properties);

    ++itItems;
  }

  const boost::property_tree::ptree& groupsNode = tree.get_child("template.groups");
  boost::property_tree::ptree::const_iterator itGroups = groupsNode.begin();
  while (itGroups != groupsNode.end())
  {
    const boost::property_tree::ptree& groupNode = itGroups->second;

    std::string groupName = groupNode.get<std::string>("name");
    const boost::property_tree::ptree& itemsListNode = groupNode.get_child("items");

    boost::property_tree::ptree::const_iterator itItems = itemsListNode.begin();
    while (itItems != itemsListNode.end())
    {
      std::string itemName = itItems->second.get<std::string>("name");

      oMapGroups[groupName].push_back(itemName);

      ++itItems;
    }

    ++itGroups;
  }

  return true;
}

void te::layout::BoostPropertySerializer::searchProperty( const Property& property, boost::property_tree::ptree& array, boost::property_tree::ptree& child )
{
  if(!property.getSubProperty().empty())
  {
    const std::vector<Property>& props = property.getSubProperty();
    std::vector<Property>::const_iterator it;

    for(it = props.begin(); it != props.end(); ++it)
    {
      const Property& prop = (*it);

      boost::property_tree::ptree childArray;
      
      childArray.add(prop.getName(), prop.getValue()->toString());
      childArray.add("type", prop.getType()->getName()); 

      std::string s_name = prop.getName() + "_child";

      child.push_back(std::make_pair(s_name,childArray));
      
      searchProperty(prop, array, childArray);
    }

    std::string s_nameChild = property.getName() + "_child";
    if(array != child)
      array.push_back(std::make_pair(s_nameChild,child));
  }
}

boost::property_tree::ptree te::layout::BoostPropertySerializer::encode(const std::vector<te::layout::Property>& vecProperty) const
{
  boost::property_tree::ptree propertiesNode;
  for (size_t i = 0; i < vecProperty.size(); ++i)
  {
    const Property& property = vecProperty[i];

    if (!property.isSerializable())
      continue;

    const std::string& name = property.getName();
    const std::string& type = property.getType()->getName();
    std::string valueType = te::layout::TypeManager::getInstance().getName(property.getValue()->getTypeCode());
    const std::string& value = property.getValue()->toString();
    const std::string& currentChoice = property.getOptionByCurrentChoice().convertToString();
    const std::string& parentClass = property.getParent();

    boost::property_tree::ptree propertyNode;
    propertyNode.add("name", name);
    propertyNode.add("type", type);
    propertyNode.add("valueType", valueType);
    propertyNode.add("value", toUTF8(value));
    propertyNode.add("currentChoice", toUTF8(currentChoice));
    propertyNode.add("parentClass", parentClass);

    const std::vector<te::layout::Property>& vecSubProperty = property.getSubProperty();
    if (vecSubProperty.empty() == false)
    {
      boost::property_tree::ptree subPropertyNode = encode(vecSubProperty);
      propertyNode.push_back(std::make_pair("sub_property", subPropertyNode));
    }

    propertiesNode.push_back(std::make_pair("property", propertyNode));
  }

  return propertiesNode;
}

te::layout::Properties te::layout::BoostPropertySerializer::decodeItem(const boost::property_tree::ptree& itemNode)
{
  //const boost::property_tree::ptree& propertiesNode = itemNode.get_child("item");

  std::string type = itemNode.get<std::string>("object_type");
  EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();
  EnumType* enumType = enumObj->getEnum(type);

  te::layout::Properties properties;
  properties.setTypeObj(enumType);

  const boost::property_tree::ptree& vecPropertiesNode = itemNode.get_child("properties");

  std::vector<Property> vecProperty = decodeProperties(vecPropertiesNode);
  for (size_t i = 0; i < vecProperty.size(); ++i)
  {
    properties.addProperty(vecProperty[i]);
  }
  
  return properties;
}

std::vector<te::layout::Property> te::layout::BoostPropertySerializer::decodeProperties(const boost::property_tree::ptree& propertiesNode)
{
  std::vector<te::layout::Property> vecProperties;

  boost::property_tree::ptree::const_iterator itProperty = propertiesNode.begin();
  while (itProperty != propertiesNode.end())
  {
    Property property = decodeProperty(itProperty->second);
    vecProperties.push_back(property);

    ++itProperty;
  }

  return vecProperties;
}

te::layout::Property te::layout::BoostPropertySerializer::decodeProperty(const boost::property_tree::ptree& propertyNode)
{
  std::string name = propertyNode.get<std::string>("name");
  std::string type = propertyNode.get<std::string>("type");
  std::string valueType = propertyNode.get<std::string>("valueType");
  std::string value = fromUTF8(propertyNode.get<std::string>("value"));

  std::string currentChoice = fromUTF8(propertyNode.get<std::string>("currentChoice"));
  std::string parentClass = propertyNode.get<std::string>("parentClass");

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  EnumType* valueDataType = dataType->getEnum(type);

  std::unique_ptr<te::dt::AbstractData> inputData(CreateData(value));
  int typeCode = te::layout::TypeManager::getInstance().getTypeCode(valueType);
  te::dt::AbstractData* outputData = te::layout::TypeManager::getInstance().convertTo(inputData.get(), typeCode);


  Property property;
  property.setName(name);
  property.setParent(parentClass);
  property.setValue(outputData, valueDataType);

  const boost::property_tree::ptree& bValueNode = propertyNode.get_child("value");
  std::string cValue = fromUTF8(bValueNode.get_value<std::string>());

  if (currentChoice.empty() == false)
  {
    const boost::property_tree::ptree& currentChoiceNode = propertyNode.get_child("currentChoice");
    std::string cCurrentChoiceValue = fromUTF8(currentChoiceNode.get_value<std::string>());

    Variant variantCurrentChoice;
    variantCurrentChoice.fromString(cCurrentChoiceValue, dataType->getDataTypeString());

    property.setOptionChoice(variantCurrentChoice);
  }

  boost::optional<const boost::property_tree::ptree&> child = propertyNode.get_child_optional("sub_property");
  if (child)
  {
    const boost::property_tree::ptree& vecSubPropertyNode = propertyNode.get_child("sub_property");

    std::vector<Property> vecSubProperty = decodeProperties(vecSubPropertyNode);
    for (size_t i = 0; i < vecSubProperty.size(); ++i)
    {
      property.addSubProperty(vecSubProperty[i]);
    }
  }
  
  return property;
}

std::string te::layout::BoostPropertySerializer::toUTF8(const std::string& latin1String) const
{
  if (latin1String.empty())
  {
    return latin1String;
  }

  te::common::CharEncodingConv convert(te::common::LATIN1, te::common::UTF8);
  std::string utf8_string = convert.conv(latin1String);
  return utf8_string;
}

std::string te::layout::BoostPropertySerializer::fromUTF8(const std::string& utf8String) const
{
  if (utf8String.empty())
  {
    return utf8String;
  }

  te::common::CharEncodingConv convert(te::common::UTF8, te::common::LATIN1);
  std::string latin1_string = convert.conv(utf8String);

  return latin1_string;
}
