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
  \file terralib/layout/qt/core/pattern/factory/TextGridItemFactory.cpp

  \brief This is the concrete factory for text grid item.
*/

// TerraLib
#include "TextGridItemFactory.h"
#include "../../../../../core/enum/Enums.h"
#include "../../../../../item/TextGridModel.h"
#include "../../../../item/TextGridController.h"
#include "../../../../item/TextGridItem.h"

te::layout::AbstractItemView* te::layout::TextGridItemFactory::build(ItemFactoryParamsCreate params)
{
  Properties      props = params.getProperties(); 

  TextGridModel* model = new TextGridModel();
  AbstractItemController* controller = new TextGridController(model);
  TextGridItem* view = new TextGridItem(controller);
  controller->setView(view);

  if (props.getProperties().empty())
  {
    props = convertToProperties(params);

    std::vector< std::vector<std::string> > textMatrix;

    std::vector<std::string> r1;
    r1.push_back("(1x1)");

    std::vector<std::string> r2;
    r2.push_back("(2x1)");

    std::vector<std::string> r3;
    r3.push_back("(3x1)");

    textMatrix.push_back(r1);
    textMatrix.push_back(r2);
    textMatrix.push_back(r3);

    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    {
      Property pMatrix;
      pMatrix.setName("text_matrix");
      pMatrix.setValue(textMatrix, dataType->getDataTypeStringMatrix());

      props.addProperty(pMatrix);
    }

  }
  controller->setProperties(props);

  return view;
}

te::layout::TextGridItemFactory::TextGridItemFactory() :
  ItemFactory(Enums::getInstance().getEnumObjectType()->getTextGridItem()->getName())
{

}

te::layout::TextGridItemFactory::~TextGridItemFactory()
{

}
