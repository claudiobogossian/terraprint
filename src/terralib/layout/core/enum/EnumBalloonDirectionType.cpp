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
#include "EnumBalloonDirectionType.h"

#include <terralib/core/translator/Translator.h>

te::layout::EnumBalloonDirectionType::EnumBalloonDirectionType() :
  m_none(0),
  m_left(0),
  m_right(0)
{
  init();
}

te::layout::EnumBalloonDirectionType::~EnumBalloonDirectionType()
{
  
}

void te::layout::EnumBalloonDirectionType::init()
{
  m_none = createEnum("None", this, TE_TR("None"));

  m_left = createEnum("Left", this, TE_TR("Left"));

  m_right = createEnum("Right", this, TE_TR("Right"));
}

te::layout::EnumType* te::layout::EnumBalloonDirectionType::getLeft() const
{
  return m_left;
}

te::layout::EnumType* te::layout::EnumBalloonDirectionType::getRight() const
{
  return m_right;
}

te::layout::EnumType* te::layout::EnumBalloonDirectionType::getNoneType() const
{
  return m_none;
}
