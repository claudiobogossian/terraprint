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
  \file EnumNorthArrowType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumNorthArrowType.h"

#include <terralib/core/translator/Translator.h>

te::layout::EnumNorthArrowType::EnumNorthArrowType() :
  m_noneType(0),
  m_northArrow1(0),
  m_northArrow2(0),
  m_northArrow3(0),
  m_northArrow4(0)
{
  init();
}

te::layout::EnumNorthArrowType::~EnumNorthArrowType()
{
  
}

void te::layout::EnumNorthArrowType::init()
{
  m_noneType = createEnum("None", this, TE_TR("None"));

  m_northArrow1 = createEnum("NorthArrow1", this, TE_TR("North Arrow 1"));

  m_northArrow2 = createEnum("NorthArrow2", this, TE_TR("North Arrow 2"));

  m_northArrow3 = createEnum("NorthArrow3", this, TE_TR("North Arrow 3"));

  m_northArrow4 = createEnum("NorthArrow4", this, TE_TR("North Arrow 4"));

}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNorthArrowType1() const
{
  return m_northArrow1;
}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNorthArrowType2() const
{
  return m_northArrow2;
}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNorthArrowType3() const
{
  return m_northArrow3;
}

te::layout::EnumType* te::layout::EnumNorthArrowType::getNorthArrowType4() const
{
  return m_northArrow4;
}