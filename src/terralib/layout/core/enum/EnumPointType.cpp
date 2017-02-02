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
  \file EnumPointType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumPointType.h"

#include <terralib/core/translator/Translator.h>

te::layout::EnumPointType::EnumPointType() :
  m_noneType(0),
  m_star1(0),
  m_star2(0),
  m_star3(0),
  m_star4(0),
  m_circle(0),
  m_x(0),
  m_square(0),
  m_rhombus(0),
  m_cross(0)
{
  init();
}

te::layout::EnumPointType::~EnumPointType()
{
  
}

void te::layout::EnumPointType::init()
{
  m_noneType = createEnum("None", this, TE_TR("None"));

  m_star1 = createEnum("5-Points Star", this, TE_TR("5-Points Star"));

  m_star2 = createEnum("6-Points Star", this, TE_TR("6-Points Star"));

  m_star3 = createEnum("7-Points Star", this, TE_TR("7-Points Star"));

  m_star4 = createEnum("8-Points Star", this, TE_TR("8-Points Star"));

  m_circle = createEnum("Circle", this, TE_TR("Circle"));

  m_x = createEnum("X", this, TE_TR("X"));

  m_square = createEnum("Square", this, TE_TR("Square"));

  m_rhombus = createEnum("Rhombus", this, TE_TR("Rhombus"));

  m_cross = createEnum("Cross", this, TE_TR("Cross"));
}

te::layout::EnumType* te::layout::EnumPointType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumPointType::getStar1Type() const
{
  return m_star1;
}

te::layout::EnumType* te::layout::EnumPointType::getStar2Type() const
{
  return m_star2;
}

te::layout::EnumType* te::layout::EnumPointType::getStar3Type() const
{
  return m_star3;
}

te::layout::EnumType* te::layout::EnumPointType::getStar4Type() const
{
  return m_star4;
}

te::layout::EnumType* te::layout::EnumPointType::getCircleType() const
{
  return m_circle;
}

te::layout::EnumType* te::layout::EnumPointType::getXType() const
{
  return m_x;
}

te::layout::EnumType* te::layout::EnumPointType::getSquareType() const
{
  return m_square;
}

te::layout::EnumType* te::layout::EnumPointType::getRhombusType() const
{
  return m_rhombus;
}

te::layout::EnumType* te::layout::EnumPointType::getCrossType() const
{
  return m_cross;
}

