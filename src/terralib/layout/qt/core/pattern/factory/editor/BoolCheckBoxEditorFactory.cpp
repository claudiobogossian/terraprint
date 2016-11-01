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
  \file terralib/layout/qt/core/pattern/factory/editor/BoolCheckBoxEditorFactory.cpp

  \brief This is the concrete factory for zoom area tools.
*/

// TerraLib
#include "BoolCheckBoxEditorFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../View.h"
#include "../../../propertyeditor/BoolCheckBoxEditor.h"

te::layout::AbstractEditor* te::layout::BoolCheckBoxEditorFactory::build(EditorFactoryParamsCreate params)
{
  std::vector<Property> props = params.getProperties();
  QModelIndex index = params.getModelIndex();
  QWidget* parent = params.getParent();

  BoolCheckBoxEditor* editor = new BoolCheckBoxEditor(index, parent);
  editor->setProperties(props);
  return editor;
}

te::layout::BoolCheckBoxEditorFactory::BoolCheckBoxEditorFactory() :
  EditorFactory(Enums::getInstance().getEnumDataType()->getDataTypeBool()->getName())
{

}

te::layout::BoolCheckBoxEditorFactory::~BoolCheckBoxEditorFactory()
{

}
