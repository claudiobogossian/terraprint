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
  \file terralib/layout/qt/core/pattern/factory/outside/ScaleSettingsOutsideFactory.cpp

  \brief This is the concrete factory for arrow item.
*/

// TerraLib
#include "ScaleSettingsOutsideFactory.h"

#include "../../../../../core/enum/Enums.h"

#include "../../../../../outside/ScaleSettingsModel.h"
#include "../../../../outside/ScaleSettingsController.h"
#include "../../../../outside/ScaleSettingsOutside.h"

te::layout::AbstractOutsideView* te::layout::ScaleSettingsOutsideFactory::build(OutsideFactoryParamsCreate params)
{
  ScaleSettingsModel* model = new ScaleSettingsModel();

  Scene* scene = params.getScene();

  ScaleSettingsController* controller = new ScaleSettingsController(scene, model);
  ScaleSettingsOutside* view = new ScaleSettingsOutside(controller, params.getParent());
  controller->setView(view);

  return view;
}

te::layout::ScaleSettingsOutsideFactory::ScaleSettingsOutsideFactory() :
  OutsideFactory(Enums::getInstance().getEnumObjectType()->getScaleSettingsDialog()->getName())
{

}

te::layout::ScaleSettingsOutsideFactory::~ScaleSettingsOutsideFactory()
{

}


