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
  \file JSONTemplate.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "XmlTemplate.h"

#include "../property/Properties.h"
#include "../serialization/BoostPropertySerializer.h"
#include "../enum/Enums.h"
#include "terralib/common/Translator.h"

// Boost
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/version.hpp>


// STL
#include <iostream>
#include <fstream>

te::layout::XmlTemplate::XmlTemplate(std::string path) :
  AbstractTemplate(path)
{
  m_type = Enums::getInstance().getEnumTemplateType()->getXmlType();
}

te::layout::XmlTemplate::~XmlTemplate()
{

}

bool te::layout::XmlTemplate::exportTemplate(const PaperConfig& paperConfig, const std::vector<te::layout::Properties>& properties, const std::map< std::string, std::vector<std::string> >& mapGroups)
{
  //We initialize the encoder object
  BoostPropertySerializer serializer;

  //we get the tree to be serialized
  const boost::property_tree::ptree& pTree = serializer.encode(paperConfig, properties, mapGroups);

  try
  {
    // Write property tree to JSON file
    //std::locale localeUTF8("en-US.UTF8");
    //boost::property_tree::json_parser::
    //boost::property_tree::write_json(m_path, pTree);

#if BOOST_VERSION > 105600
    boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
#else
    boost::property_tree::xml_writer_settings<char> settings('\t', 1);
#endif

    boost::property_tree::write_xml(m_path, pTree, std::locale(), settings);
  }
  catch (const boost::property_tree::xml_parser_error& pe)
  {
    std::string errmsg = TE_TR("Error parsing: ") + pe.filename() + ": " + pe.message();
    te::common::Exception teException(errmsg);
    throw(teException);
  }

  return true;
}

bool te::layout::XmlTemplate::importTemplate(PaperConfig& paperConfig, std::vector<te::layout::Properties>& properties, std::map< std::string, std::vector<std::string> >& mapGroups)
{
  boost::property_tree::ptree pTree;
  try
  {
    boost::property_tree::read_xml(m_path, pTree);
  }
  catch (const boost::property_tree::xml_parser_error& pe)
  {
    std::string errmsg = TE_TR("Error parsing: ") + pe.filename() + ": " + pe.message();
    te::common::Exception teException(errmsg);
    throw(teException);
  }

  BoostPropertySerializer serializer;
  bool result = serializer.decode(pTree, paperConfig, properties, mapGroups);

  return result;
}

bool te::layout::XmlTemplate::deleteTemplate()
{
  return true;
}


