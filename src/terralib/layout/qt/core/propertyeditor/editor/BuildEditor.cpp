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
  \file BuildEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildEditor.h"
#include "../../../../core/enum/Enums.h"
#include "../../../core/pattern/factory/propertyeditor/editor/EditorFactory.h"
#include "../../pattern/factory/propertyeditor/editor/EditorFactoryParamsCreate.h"

// Qt
#include <QMetaType>

te::layout::BuildEditor::BuildEditor()
{
  
}

te::layout::BuildEditor::~BuildEditor()
{
 
}

te::layout::AbstractEditor* te::layout::BuildEditor::buildEditor(const EditorFactoryParamsCreate& params)
{
  QModelIndex index = params.getModelIndex();

  int propertyType = qMetaTypeId<te::layout::Property>();
  QVariant variant = index.data(propertyType);
  
  AbstractEditor* editor = 0;

  te::common::ParameterizedAbstractFactory<AbstractEditor, std::string, EditorFactoryParamsCreate>::dictionary_type& d = te::common::ParameterizedAbstractFactory<AbstractEditor, std::string, EditorFactoryParamsCreate>::getDictionary();

  te::layout::Property prop = qvariant_cast<te::layout::Property>(variant);
  std::string editorName = prop.getType()->getName();
  EditorFactory* fact = dynamic_cast<EditorFactory*>(d.find(editorName));
  if (fact)
  {
    editor = te::layout::EditorFactory::make(editorName, params);
  }
  return editor;
}

