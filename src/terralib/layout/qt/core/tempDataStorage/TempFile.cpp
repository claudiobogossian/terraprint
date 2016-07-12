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
  \file TempFile.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TempFile.h"
#include "TempFileInfo.h"
#include "../../../core/template/TemplateEditor.h"
#include "../../../core/enum/Enums.h"
#include "../Scene.h"
#include "../../../core/property/Properties.h"
#include "../../../core/template/AbstractTemplate.h"

// STL
#include <vector>

te::layout::TempFile::TempFile(AbstractTempDataStorageInfo* info) :
  AbstractTempDataStorage(info)
{
  
}

te::layout::TempFile::~TempFile()
{
  
}

bool te::layout::TempFile::save()
{
  bool result = false;

  TempFileInfo* tempFileInfo = dynamic_cast<TempFileInfo*>(m_info);
  if (!tempFileInfo)
  {
    return result;
  }

  // XML File Type
  EnumType* type = Enums::getInstance().getEnumTemplateType()->getXmlType();

  std::string path = tempFileInfo->getPath();
  Scene* scene = tempFileInfo->getScene();
  PaperConfig* paperConfig = scene->getPaperConfig();

  std::vector<te::layout::Properties> properties;
  std::map< std::string, std::vector<std::string> > mapGroups;
  scene->getItemsProperties(properties, mapGroups);

  if (properties.empty())
    return result;

  TemplateEditor editor(type, path);
  AbstractTemplate* jtemplate = editor.getTemplate();

  if (!jtemplate)
    return result;

  result = jtemplate->exportTemplate(*paperConfig, properties, mapGroups);
  
  return result;
}

