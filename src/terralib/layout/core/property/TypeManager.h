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

// TerraLib
#include <terralib/common/Singleton.h>
#include <terralib/datatype/DataTypeConverter.h>

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
    
    class TypeManager : public te::common::Singleton<TypeManager>
    {
      friend class te::common::Singleton<TypeManager>;

      public:

        template <typename T> void registerType(int typeCode, const te::dt::DataTypeConverter& converterFromString, const te::dt::DataTypeConverter& converterToString);

        void registerType(const std::string& typeName, int typeCode, const te::dt::DataTypeConverter& converterFromString, const te::dt::DataTypeConverter& converterToString);

        //!< Returns the converted FROM fromTypeCode TO toTypeCode
        const te::dt::DataTypeConverter& getConverter(int fromTypeCode, int toTypeCode) const;

        //!< Converts the given data into the given type code. If there is not a registered converter, throws exception
        te::dt::AbstractData* convertTo(te::dt::AbstractData* data, int toTypeCode) const;

        //!< Returns the typeCode from a given typeName
        int getTypeCode(const std::string& typeName) const;

        //!< Returns the typeName from a given typeCode
        std::string getName(int typeCode) const;

    protected:

      /*! \brief Constructor for singletons is protected. */
      TypeManager();

      /*! \brief Destructor for singletons is protected. */
      virtual ~TypeManager();

    protected:

      std::map<std::string, int> m_mapNames;
      std::map<int, std::string> m_mapTypeCodes;
    };

    //implementation of the template function
    template <typename T>
    void te::layout::TypeManager::registerType(int typeCode, const te::dt::DataTypeConverter& converterFromString, const te::dt::DataTypeConverter& converterToString)
    {
      std::string name = typeid(T).name();
      registerType(name, typeCode, converterFromString, converterToString);
    }
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_TYPEMANAGER_H 
