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
  \file EnumTextFormatType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumTextFormatType.h"

te::layout::EnumTextFormatType::EnumTextFormatType()
  : m_defaultFormat(0)
  , m_ANPFormat(0)
{
  init();
}

te::layout::EnumTextFormatType::~EnumTextFormatType()
{
  if(m_defaultFormat)
  {
    delete m_defaultFormat;
    m_defaultFormat = 0;
  }
  if(m_ANPFormat)
  {
    delete m_ANPFormat;
    m_ANPFormat = 0;
  }
}

void te::layout::EnumTextFormatType::init()
{
  m_defaultFormat = createEnum("DefaultFormat", this, TR_LAYOUT("Default Format - GGºMM'SS''"));

  m_ANPFormat = createEnum("ANPFormat", this, TR_LAYOUT("ANP Format - GG:MM:SS"));
}

te::layout::EnumType* te::layout::EnumTextFormatType::getDefaultFormat() const
{
  return m_defaultFormat;
}

te::layout::EnumType* te::layout::EnumTextFormatType::getANPFormat() const
{
  return m_ANPFormat;
}
