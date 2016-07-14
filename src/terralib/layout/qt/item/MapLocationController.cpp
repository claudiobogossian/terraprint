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
#include "MapLocationController.h"

#include "../../core/pattern/mvc/AbstractItemModel.h"

te::layout::MapLocationController::MapLocationController(AbstractItemModel* model)
  : MapController(model)
{

}

te::layout::MapLocationController::~MapLocationController()
{

}

void te::layout::MapLocationController::update(const Subject* subject)
{
  //checks if the update was sent by my model. If so, we just call our parent update
  if (subject == m_model)
  {
    AbstractItemController::update(subject);
    return;
  }

  //if we got here, the update function was called by MapItem.
  //So we need to update our model based of the current state of the map model

  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if (subjectModel == 0)
  {
    return;
  }

  Property pBoxNew = subjectModel->getProperty("world_box");
  const Property& pBoxCurrent = this->getProperty("reference_box");
  te::gm::Envelope boxNew = pBoxNew.getValue().toEnvelope();
  te::gm::Envelope boxCurrent = pBoxCurrent.getValue().toEnvelope();

  Property pSridNew = subjectModel->getProperty("srid");
  const Property& pSridCurrent = this->getProperty("reference_srid");
  int sridNew = pSridNew.getValue().toInt();
  int sridCurrent = pSridCurrent.getValue().toInt();

  if (sridNew != sridCurrent || boxNew.equals(boxCurrent) == false)
  {
    pSridNew.setName("reference_srid");
    pBoxNew.setName("reference_box");

    Properties properties;
    properties.addProperty(pSridNew);
    properties.addProperty(pBoxNew);

    setProperties(properties);
  }
}