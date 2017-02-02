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
  \file EnumTemplateType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumTemplateType.h"

#include <terralib/core/translator/Translator.h>

te::layout::EnumTemplateType::EnumTemplateType() :
  m_noneType(0),
  m_xmlType(0)
{
  init();
}

te::layout::EnumTemplateType::~EnumTemplateType()
{
  if(m_noneType)
  {
    delete m_noneType;
    m_noneType = 0;
  }
  if (m_xmlType)
  {
    delete m_xmlType;
    m_xmlType = 0;
  }
}

void te::layout::EnumTemplateType::init()
{
  m_noneType = createEnum("None", this, TE_TR("None"));

  m_xmlType = createEnum("XmlTemplate", this, TE_TR("XML Template"));
}

te::layout::EnumType* te::layout::EnumTemplateType::getNoneType() const
{
  return m_noneType;
}

te::layout::EnumType* te::layout::EnumTemplateType::getXmlType() const
{
  return m_xmlType;
}
