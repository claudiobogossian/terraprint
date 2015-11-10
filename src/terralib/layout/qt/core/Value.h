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
  \file Values.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_VALUE_H 
#define __TERRALIB_LAYOUT_INTERNAL_VALUE_H

// TerraLib
#include "../../core/Config.h"

namespace te
{
  namespace layout
  {
    /*!
      \brief 
    
      \ingroup layout
    */
    class TELAYOUTEXPORT ValueBase
    {
    public:
      /*!
      \brief Destructor
      */
      virtual ~ValueBase() {}

      template<class T> const T get() const; 

      template<class T, class U> void setValue(const U rhs); 
    };

    template <typename T>
    class TELAYOUTEXPORT Value : public ValueBase
    {
    public:
      /*!
      \brief Constructor
      */
      Value(const T rhs) :m_value(rhs) {}

      const T get() const { return m_value; }
      /*!
      \brief Stores a copy of value.

      \param m_value copies the value to be stored
      */
      void setValue(const T rhs) { m_value = rhs; }
    private:
      T  m_value;
    };

    template<class T> const T te::layout::ValueBase::get() const
    {
      return dynamic_cast<const Value<T>>(*this).get();
    }
    template<class T, class U> void te::layout::ValueBase::setValue(const U rhs)
    {
      return dynamic_cast<Value<T>>(*this).setValue(rhs);
    }
  }
}

#endif

