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
  \file terralib/layout/qt/core/pattern/factory/editor/IntSpinBoxEditorFactory.cpp

  \brief This is the concrete factory for zoom area tools.
*/

// TerraLib
#include "IntSpinBoxEditorFactory.h"
#include "../../../../../../core/enum/Enums.h"
#include "../../../../View.h"
#include "../../../../propertyeditor/editor/IntSpinBoxEditor.h"

te::layout::AbstractEditor* te::layout::IntSpinBoxEditorFactory::build(EditorFactoryParamsCreate params)
{
  QModelIndex index = params.getModelIndex();
  QWidget* parent = params.getParent();

  IntSpinBoxEditor* editor = new IntSpinBoxEditor(index, parent);
  return editor;
}

te::layout::IntSpinBoxEditorFactory::IntSpinBoxEditorFactory() :
  EditorFactory(Enums::getInstance().getEnumDataType()->getDataTypeInt()->getName())
{

}

te::layout::IntSpinBoxEditorFactory::~IntSpinBoxEditorFactory()
{

}
