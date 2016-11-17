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
  \file Serialization.h
   
  \brief Abstract class for serialization properties in any file extension.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SERIALIZATION_H 
#define __TERRALIB_LAYOUT_INTERNAL_SERIALIZATION_H

// TerraLib
#include "../property/Properties.h"
#include "../Config.h"

namespace te
{
  namespace layout
  {

    class PaperConfig;

    /*!
      \brief Abstract class for serialization properties in any file extension.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT Serialization
    {
      public:

        Serialization();

        virtual ~Serialization();

        virtual void loadFromPath(std::string loadPath) = 0;

        /*!
        \brief Encodes the given vector of Properties into a Boost Property Tree container
        */
        virtual boost::property_tree::ptree encode(const PaperConfig& paperConfig, const std::vector<te::layout::Properties>& vecProperties, const std::map< std::string, std::vector<std::string> >& mapGroups) = 0;

        /*!
        \brief Decodes the given Boost Property Tree container into a vector of Properties
        */
        virtual bool decode(const boost::property_tree::ptree& tree, PaperConfig& oPaperConfig, std::vector<te::layout::Properties>& oProperties, std::map< std::string, std::vector<std::string> >& oMapGroups) = 0;

        virtual std::string getLoadPath();

        virtual bool isEmpty() = 0;

        virtual void setRootKey(std::string rootKey);

        virtual std::string getRootKey();
       
       protected:

         std::string m_loadPath;
         std::string m_rootKey;
    };
  }
}

#endif
