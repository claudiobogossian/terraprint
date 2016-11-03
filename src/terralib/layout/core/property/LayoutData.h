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


#ifndef __TERRALIB_LAYOUT_INTERNAL_LAYOUTDATA_H 
#define __TERRALIB_LAYOUT_INTERNAL_LAYOUTDATA_H 

#include "TypeManager.h"

#include "DataTypes.h"

//terralib
#include <terralib/common/Exception.h>
#include <terralib/datatype/AbstractData.h>
#include <terralib/datatype/DataTypeConverter.h>

//STL
#include <memory>
#include <typeinfo>

namespace te
{
  namespace dt
  {
    class AbstractData;
  }

  namespace layout
  {
    template<typename T> class LayoutData : public te::dt::AbstractData
    {
      public:
        //!< Constructor. The given value will be copied
        LayoutData(const T& value, const TypeManager& typeManager);

        virtual ~LayoutData();

        /*!
        \brief Copy constructor.

        \param rhs The right-hand-side data.
        */
        LayoutData(const LayoutData& rhs);

        /*!
        \brief It returns a clone of this object.

        \return A clone of this object.
        */
        virtual te::dt::AbstractData* clone() const;

        /*!
        \brief It returns the data type code associated to the data value.

        \return The data type code associated to the data value.
        */
        virtual int getTypeCode() const;

        /*!
        \brief It returns the data value in a string notation.

        \return The data value in a string notation.
        */
        virtual std::string toString() const;

        virtual const T& getValue() const;

      protected:

        const LayoutData<T>& operator=(const LayoutData<T>& rhs);

      protected:

        int m_typeCode; //!< The type code associated to this data
        std::unique_ptr<T> m_value; //!< The value of the data
        te::dt::DataTypeConverter m_converterToString;
    };


    template<typename T> LayoutData<T>::LayoutData(const T& value, const TypeManager& typeManager)
      : te::dt::AbstractData()
      , m_typeCode(0)
      , m_value(new T(value))
    {
      const std::type_info& typeInfo = typeid(T);
      std::string typeName = typeInfo.name();
      m_typeCode = typeManager.getTypeCode(typeName);
      m_converterToString = typeManager.getConverter(m_typeCode, (int)te::layout::LayoutTypes::STRING_TYPE);
    }

    template<typename T> LayoutData<T>::~LayoutData()
    {

    }

    template<class T> LayoutData<T>::LayoutData(const LayoutData& rhs)
      : te::dt::AbstractData(rhs)
      , m_typeCode(rhs.m_typeCode)
      , m_value(new T(*rhs.m_value))
      , m_converterToString(rhs.m_converterToString)
    {
    }

    template<typename T> te::dt::AbstractData* LayoutData<T>::clone() const
    {
      return new LayoutData(*this);
    }
    
    template<typename T> int LayoutData<T>::getTypeCode() const
    {
      return m_typeCode;
    }

    template<typename T> std::string LayoutData<T>::toString() const
    {
      if (m_typeCode == (int)te::layout::LayoutTypes::STRING_TYPE)
      {
        const LayoutData<std::string>* stringData = dynamic_cast<const LayoutData<std::string>*>(this);
        if (stringData == 0)
        {
          throw te::common::Exception("LayoutData::Invalid type cast");
        }

        return stringData->getValue();
      }

      std::unique_ptr<te::dt::AbstractData> temporaryData(m_converterToString((te::dt::AbstractData*)this));
      LayoutData<std::string>* stringData = dynamic_cast<LayoutData<std::string>*>(temporaryData.get());
      if (stringData == 0)
      {
        throw te::common::Exception("LayoutData::Invalid type cast");
      }

      return stringData->getValue();
    }

    template<typename T> const T& LayoutData<T>::getValue() const
    {
      return *m_value;
    }
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_LAYOUTDATA_H 
