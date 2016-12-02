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
  \file terralib/layout/qt/core/pattern/factory/editor/ColorEditorFactory.cpp

  \brief This is the concrete factory for color editors.
*/

// TerraLib
#include "ColorEditorFactory.h"
#include "../../../../../../core/enum/Enums.h"
#include "../../../../View.h"
#include "../../../../propertyeditor/editor/ColorEditor.h"

te::layout::AbstractEditor* te::layout::ColorEditorFactory::build(EditorFactoryParamsCreate params)
{
  QModelIndex index = params.getModelIndex();
  QWidget* parent = params.getParent();
  ContextPropertyEditor* context = params.getContextPropertyEditor();

  ColorEditor* editor = new ColorEditor(index, context, parent);
  return editor;
}

te::layout::ColorEditorFactory::ColorEditorFactory() :
  EditorFactory(Enums::getInstance().getEnumDataType()->getDataTypeColor()->getName())
{

}

te::layout::ColorEditorFactory::~ColorEditorFactory()
{

}

