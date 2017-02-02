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
  \file EnumBalloonType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumBalloonType.h"

#include <terralib/core/translator/Translator.h>

te::layout::EnumBalloonType::EnumBalloonType() :
  m_noneType(0),
  m_balloon1(0),
  m_balloon2(0),
  m_balloon3(0)
{
  init();
}

te::layout::EnumBalloonType::~EnumBalloonType()
{
  
}

void te::layout::EnumBalloonType::init()
{
  m_noneType = createEnum("None", this, TE_TR("None"));

  m_balloon1 = createEnum("RectangleBalloon", this, TE_TR("Rectangle Balloon"));

  m_balloon2 = createEnum("RoundedRectangleBalloon ", this, TE_TR("Rounded Rectangle Balloon"));

  m_balloon3 = createEnum("EllipseBalloon", this, TE_TR("Ellipse Balloon"));

}

te::layout::EnumType* te::layout::EnumBalloonType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumBalloonType::getRectangleBalloon() const
{
  return m_balloon1;
}

te::layout::EnumType* te::layout::EnumBalloonType::getRoundedRectangleBalloon() const
{
  return m_balloon2;
}

te::layout::EnumType* te::layout::EnumBalloonType::getEllipseBalloon() const
{
  return m_balloon3;
}