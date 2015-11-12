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
#include "terralib/common/Exception.h"
#include "terralib/common/STLUtils.h"
#include "terralib/common/Translator.h"
#include "../Config.h"
#include "../enum/Enums.h"

// Boost
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "boost/system/system_error.hpp"

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

boost::property_tree::ptree te::layout::BoostPropertySerializer::retrievePTree()
{
  return m_array;
}

std::vector<te::layout::Properties> te::layout::BoostPropertySerializer::retrieve() 
{
  std::vector<te::layout::Properties> propsRetrieve;
    
  //v.first //is the name of the child.
  //v.second //is the child tree.

  boost::property_tree::ptree::assoc_iterator it1 = m_array.find("template");
  boost::property_tree::ptree::assoc_iterator it_nofound = m_array.not_found();

  if (it1 == it_nofound)
    return propsRetrieve;

  boost::property_tree::ptree subtree = (*it1).second;  

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int count = 0;
  while(true)
  {
    std::stringstream ss;//create a stringstream
    ss << count;//add number to the stream

    std::string s_prop = "properties_"+ ss.str();

    boost::property_tree::ptree::assoc_iterator it2 = subtree.find(s_prop); 
    boost::property_tree::ptree::assoc_iterator it_nofound2 = subtree.not_found();

    if (it2 == it_nofound2)
      return propsRetrieve;

    boost::property_tree::ptree subtree1 = (*it2).second;

    boost::property_tree::ptree::assoc_iterator itName = subtree1.find("name");
    boost::property_tree::ptree::assoc_iterator it_nofoundName = subtree1.not_found();

    if (itName == it_nofoundName)
      return propsRetrieve;

    te::layout::Properties props = (*itName).second.data();

    EnumObjectType* enumObj = Enums::getInstance().getEnumObjectType();

    std::string valName;
    boost::property_tree::ptree tree;
    Property prop;
    BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, subtree.get_child(s_prop)) 
    {
      if(v.first.compare("object_type") == 0)
      {
        EnumType* type = enumObj->getEnum(v.second.data());
        props.setTypeObj(type);
        continue;
      }

      if(v.first.compare("type") == 0)
      {
        prop.setName(valName);
        EnumType* tp = dataType->getEnum(v.second.data());
        Variant vt;
        vt.fromPtree(tree, tp);
        prop.setValue(vt);
        props.addProperty(prop); 
        prop.clear();
      }
      else
      {
        std::string val = v.first;
        valName = val;
        tree = v.second;
        
        retrieveSubPTree(tree, prop);
      }
    }

    propsRetrieve.push_back(props);
    count+= 1;
  }

  return propsRetrieve;
}

void te::layout::BoostPropertySerializer::retrieveSubPTree( boost::property_tree::ptree subTree, Property& prop )
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  std::string valName;
  boost::property_tree::ptree tree;
  BOOST_FOREACH( const boost::property_tree::ptree::value_type &v, subTree.get_child("") ) 
  {  
    Property proper;
    if(v.first.compare("type") == 0)
    {
      proper.setName(valName);
      EnumType* tp = dataType->getEnum(v.second.data());
      proper.setValue(tree.data(), tp);
      prop.addSubProperty(proper);
    }
    else
    {
      std::string val = v.first;
      valName = val;
      tree = v.second;
    }

    // recursive go down the hierarchy  
    retrieveSubPTree(tree, proper);
  }
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

boost::property_tree::ptree te::layout::BoostPropertySerializer::encode(const te::layout::PaperConfig& paperConfig, const std::vector<te::layout::Properties>& vecProperties)
{
  double paperWidth = 0.;
  double paperHeight = 0.;
  paperConfig.getPaperSize(paperWidth, paperHeight);

  std::vector<te::layout::Properties>::iterator it;
  std::vector<te::layout::Property>::iterator ity;
  
  boost::property_tree::ptree rootNode; //the root node contains the metadata of the template and the list of the items
  rootNode.add("name", m_rootKey);
  
  boost::property_tree::ptree paperNode;
  
  paperNode.add("orientation", paperConfig.getPaperOrientantion());
  paperNode.add("width", paperWidth);
  paperNode.add("height", paperHeight);
  paperNode.add("type", paperConfig.getPaperType());
  
  rootNode.push_back(std::make_pair("paper", paperNode));

  boost::property_tree::ptree itemsNode;
  for (size_t i = 0; i < vecProperties.size(); ++i)
  {    
    const Properties& properties = vecProperties[i];

    const std::vector<te::layout::Property>& vecProperty = properties.getProperties();
    if (vecProperty.empty())
      continue;

    const std::string& objectName = properties.getProperty("name").getValue().toString(); 

    boost::property_tree::ptree vecPropertyNode = encode(vecProperty);
    
    boost::property_tree::ptree propertiesNode;
    propertiesNode.add("object_type", properties.getTypeObj()->getName());
    propertiesNode.push_back(std::make_pair("properties", vecPropertyNode));

    itemsNode.push_back(std::make_pair("item", propertiesNode));
  }

  rootNode.push_back(std::make_pair("items", itemsNode));
  
  boost::property_tree::ptree templateNode;
  templateNode.push_back(std::make_pair("template", rootNode));

  return templateNode;
}

bool te::layout::BoostPropertySerializer::decode(const boost::property_tree::ptree& tree, te::layout::PaperConfig& oPaperConfig, std::vector<te::layout::Properties>& oProperties)
{
  std::vector<te::layout::Properties> vecProperties;

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

  return true;
}

void te::layout::BoostPropertySerializer::searchProperty( Property& property, boost::property_tree::ptree& array, boost::property_tree::ptree& child )
{
  if(!property.getSubProperty().empty())
  {
    Property propCopy = property;

    std::vector<Property> props = property.getSubProperty();

    std::vector<Property>::iterator it;

    for(it = props.begin(); it != props.end(); ++it)
    {    
      Property prop = (*it);

      boost::property_tree::ptree childArray;
      
      childArray.add(prop.getName(), prop.getValue().convertToString());
      childArray.add("type", prop.getType()->getName()); 

      std::string s_name = prop.getName() + "_child";

      child.push_back(std::make_pair(s_name,childArray));
      
      searchProperty(prop, array, childArray);
    }

    std::string s_nameChild = propCopy.getName() + "_child";
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
    const std::string& value = property.getValue().convertToString();
    const std::string& currentChoice = property.getOptionByCurrentChoice().convertToString();

    boost::property_tree::ptree propertyNode;
    propertyNode.add("name", name);
    propertyNode.add("type", type);
    propertyNode.add("value", value);
    propertyNode.add("currentChoice", currentChoice);

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
  std::string value = propertyNode.get<std::string>("value");
  std::string currentChoice = propertyNode.get<std::string>("currentChoice");

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  EnumType* valueType = dataType->getEnum(type);

  Property property;
  property.setName(name);

  const boost::property_tree::ptree& valueNode = propertyNode.get_child("value");
  Variant variantValue;
  variantValue.fromPtree(valueNode, valueType);

  property.setValue(variantValue);

  if (currentChoice.empty() == false)
  {
    const boost::property_tree::ptree& currentChoiceNode = propertyNode.get_child("currentChoice");

    Variant variantCurrentChoice;
    variantCurrentChoice.fromPtree(currentChoiceNode, dataType->getDataTypeString());

    property.setOptionChoice(variantCurrentChoice);
  }
  
  return property;
}
