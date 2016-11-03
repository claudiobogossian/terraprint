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
  \file JSON.h
   
   \brief Implementation of .json for Serialization. It is a JSON file. Save or change a file .json and serializes the properties of the objects, MVC component.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_BOOSTPROPERTYSERIALIZER_H 
#define __TERRALIB_LAYOUT_INTERNAL_BOOSTPROPERTYSERIALIZER_H 

//TerraLib
#include "Serialization.h"
#include "../property/Property.h"
#include "../Config.h"

// Boost
#include <boost/property_tree/ptree.hpp>

// STL
#include <utility>

namespace te
{
  namespace layout
  {

    class Properties;
    class Property;
    class PaperConfig;

    /*!
      \brief Implementation of .json for Serialization. It is a JSON file. Save or change a file .json and serializes the properties of the objects, MVC component.
    
      \ingroup layout

      \sa te::layout::Serialization
    */
    class TELAYOUTEXPORT BoostPropertySerializer : public Serialization
    {
      public:

        BoostPropertySerializer();

        virtual ~BoostPropertySerializer();

        virtual bool isEmpty();

        virtual void loadFromPath( std::string loadPath );

        /*!
        \brief Encodes the given vector of Properties into a Boost Property Tree container
        */
        virtual boost::property_tree::ptree encode(const PaperConfig& paperConfig, const std::vector<te::layout::Properties>& vecProperties, const std::map< std::string, std::vector<std::string> >& mapGroups);

        /*!
        \brief Decodes the given Boost Property Tree container into a vector of Properties
        */
        virtual bool decode(const boost::property_tree::ptree& tree, PaperConfig& oPaperConfig, std::vector<te::layout::Properties>& oProperties, std::map< std::string, std::vector<std::string> >& oMapGroups);

      protected:

        virtual void searchProperty(const Property& property, boost::property_tree::ptree& array, boost::property_tree::ptree& child);

        /*!
        \brief Encodes the given vector of Property to a Boost Property Tree container
        */
        virtual  boost::property_tree::ptree encode(const std::vector<te::layout::Property>& vecProperty) const;

        /*!
        \brief Decodes the given Boost Property Tree that container into a vector of Properties. The given node must represent an Item Node
        */
        virtual te::layout::Properties decodeItem(const boost::property_tree::ptree& itemNode);

        /*!
        \brief Decodes the given Boost Property Tree that container into a vector of Properties. The given node must represent the list of all Property of an item
        */
        virtual std::vector<te::layout::Property> decodeProperties(const boost::property_tree::ptree& propertiesNode);

        /*
        \brief Decodes the given Boost Property Tree that container into a vector of Property.The given node must represent a Property
        */
        virtual te::layout::Property decodeProperty(const boost::property_tree::ptree& propertyNode);

        /*
        \brief Converts the given Latin1 string to UTF-8
        */
        virtual std::string toUTF8(const std::string& latin1String) const;

        /*
        \brief Converts the given UTF-8 string to Latin1
        */
        virtual std::string fromUTF8(const std::string& utf8String) const;

    protected:

        boost::property_tree::ptree m_array;
    };
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_BOOSTPROPERTYSERIALIZER_H
