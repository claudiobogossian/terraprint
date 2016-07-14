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
  \file EnumTempDataStorageType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumTempDataStorageType.h"

te::layout::EnumTempDataStorageType::EnumTempDataStorageType() :
  m_noneType(0),
  m_tempFileType(0)
{
  init();
}

te::layout::EnumTempDataStorageType::~EnumTempDataStorageType()
{
  if(m_noneType)
  {
    delete m_noneType;
    m_noneType = 0;
  }
  if (m_tempFileType)
  {
    delete m_tempFileType;
    m_tempFileType = 0;
  }
}

void te::layout::EnumTempDataStorageType::init()
{
  m_noneType = createEnum("None", this, TR_LAYOUT("None"));

  m_tempFileType = createEnum("TempFile", this, TR_LAYOUT("Temp File"));
}

te::layout::EnumType* te::layout::EnumTempDataStorageType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumTempDataStorageType::getTempFileType() const
{
  return m_tempFileType;
}
