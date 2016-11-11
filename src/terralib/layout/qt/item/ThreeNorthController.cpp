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

// TerraLib
#include "ThreeNorthController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"

#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "ThreeNorthItem.h"

te::layout::ThreeNorthController::ThreeNorthController(AbstractItemModel* model)
  : AbstractItemController(model)
{

}

te::layout::ThreeNorthController::~ThreeNorthController()
{

}

void te::layout::ThreeNorthController::update(const Subject* subject)
{
  if (m_model == subject)
  {
    AbstractItemController::update(subject);
    return;
  }

  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if (subjectModel == 0)
  {
    return;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pNewWorldBox = subjectModel->getProperty("world_box");
  const Property& pNewSrid = subjectModel->getProperty("srid");

  Properties properties;

  properties.addProperty(pNewWorldBox);
  properties.addProperty(pNewSrid);

  setProperties(properties);
}


