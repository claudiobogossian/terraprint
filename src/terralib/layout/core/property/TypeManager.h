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


#ifndef __TERRALIB_LAYOUT_INTERNAL_TYPEMANAGER_H 
#define __TERRALIB_LAYOUT_INTERNAL_TYPEMANAGER_H 

#include "../Config.h"

// TerraLib
#include <terralib/common/Singleton.h>
#include <terralib/datatype/DataTypeConverter.h>
#include <terralib/common/Exception.h>

//STL
#include <map>
#include <string>

namespace te
{
  namespace dt
  {
    class AbstractData;
  }

  namespace layout
  {
    
    class TELAYOUTEXPORT TypeManager : public te::common::Singleton<TypeManager>
    {
      friend class te::common::Singleton<TypeManager>;

      public:

        template <typename T> 
        void registerType(int typeCode, const std::string& typeName, const te::dt::DataTypeConverter& converterFromString, const te::dt::DataTypeConverter& converterToString);

        //!< Returns the converted FROM fromTypeCode TO toTypeCode
        const te::dt::DataTypeConverter& getConverter(int fromTypeCode, int toTypeCode) const;

        //!< Converts the given data into the given type code. If there is not a registered converter, throws exception
        te::dt::AbstractData* convertTo(te::dt::AbstractData* data, int toTypeCode) const;

        //!< Returns the typeCode from a given typeName
        int getTypeCode(const std::string& typeName) const;

        //!< Returns the typeCode from a given typeName
        template <typename T> 
        int getTypeCode() const;

        //!< Returns the typeName from a given typeCode
        std::string getName(int typeCode) const;

    protected:

      /*! \brief Constructor for singletons is protected. */
      TypeManager();

      /*! \brief Destructor for singletons is protected. */
      virtual ~TypeManager();

      void registerTypeImpl(int typeCode, const std::string& typeName, const std::string& cTypeName, const te::dt::DataTypeConverter& converterFromString, const te::dt::DataTypeConverter& converterToString);

    protected:

      std::map<std::string, int> m_mapTypeNames;
      std::map<int, std::string> m_mapTypeCodes;

      std::map<std::string, std::string> m_mapCTypeNames; //!< Maps the C++ names to the alias given during Type registration <cTypeName, typeName>
    };

    template <typename T>
    int te::layout::TypeManager::getTypeCode() const
    {
      std::string cTypeName = typeid(T).name();

      std::map<std::string, std::string>::const_iterator itCTypeName = m_mapCTypeNames.find(cTypeName);
      if (itCTypeName == m_mapCTypeNames.end())
      {
        throw te::common::Exception("TypeManager::Invalid type code");
      }

      std::string typeName = itCTypeName->second;
      return getTypeCode(typeName);
    }

    //implementation of the template function
    template <typename T>
    void te::layout::TypeManager::registerType(int typeCode, const std::string& typeName, const te::dt::DataTypeConverter& converterFromString, const te::dt::DataTypeConverter& converterToString)
    {
      std::string cTypeName = typeid(T).name();
      registerTypeImpl(typeCode, typeName, cTypeName, converterFromString, converterToString);
    }
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_TYPEMANAGER_H 
