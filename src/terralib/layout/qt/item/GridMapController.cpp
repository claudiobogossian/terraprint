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
#include "GridMapController.h"

#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/property/SharedProperties.h"
#include "GridMapItem.h"

te::layout::GridMapController::GridMapController(AbstractItemModel* model)
  : AbstractItemController(model)
{

}

te::layout::GridMapController::~GridMapController()
{

}

void te::layout::GridMapController::update(const Subject* subject)
{
  AbstractItemController::update(subject);
  
  if (m_model == subject)
  {
    GridMapItem* item = dynamic_cast<GridMapItem*>(m_view);
    if (item)
    {
      item->calculateGrid();
    }
  }
}

bool te::layout::GridMapController::syncItemAssociation(Properties& properties)
{
  SharedProperties sharedProps;

  Properties prop = m_model->getProperties();

  if (properties.contains(sharedProps.getItemObserver()) == false)
  {
    return false;
  }

  const Property& propObserv = properties.getProperty(sharedProps.getItemObserver());
  Property propertyHeigh = prop.getProperty("height");
  Property propertyWidth = prop.getProperty("width");

  std::string itemObservable = te::layout::Property::GetValueAs<std::string>(propObserv);

  if (!itemObservable.empty())
  {
    propertyHeigh.setEditable(false);
    propertyWidth.setEditable(false);

    m_model->completelyUpdateProperty(propertyHeigh);
    m_model->completelyUpdateProperty(propertyWidth);

  }
  else
  {
    propertyHeigh.setEditable(true);
    propertyWidth.setEditable(true);

    m_model->completelyUpdateProperty(propertyHeigh);
    m_model->completelyUpdateProperty(propertyWidth);
  }
  
  return AbstractItemController::syncItemAssociation(properties);
}

double te::layout::GridMapController::adjustInitialX(const te::gm::Envelope& planarBox, const double initialX, const double& gapX)
{
  double lowerLeftX = planarBox.getLowerLeftX();
  double distance = initialX - planarBox.getLowerLeftX();
  double gapCount = floor(distance / gapX);
  double newInitialX = initialX - (gapCount * gapX);

  return newInitialX;
}

double te::layout::GridMapController::adjustInitialY(const te::gm::Envelope& planarBox, const double initialY, const double& gapY)
{
  double loweLeftY = planarBox.getLowerLeftY();
  double distance = initialY - planarBox.getLowerLeftY();
  double gapCount = floor(distance / gapY);
  double newInitialY = initialY - (gapCount * gapY);

  return newInitialY;
}
