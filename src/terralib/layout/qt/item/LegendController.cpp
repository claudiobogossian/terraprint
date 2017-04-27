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
#include "LegendController.h"
#include "LegendItem.h"
#include "../core/Scene.h"
#include "../../core/ItemInputProxy.h"
#include "../../core/Value.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/proxy/AbstractProxyProject.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/SharedProperties.h"

te::layout::LegendController::LegendController(te::layout::AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
{
}

te::layout::LegendController::~LegendController()
{
}

void te::layout::LegendController::update(const Subject* subject)
{
  if (m_model == subject)
  {
    AbstractItemController::update(subject);

    LegendItem* legendItem = dynamic_cast<LegendItem*>(this->getView());
    if (legendItem == 0)
    {
      return;
    }
    legendItem->refreshLegendProperties();

    return;
  }

  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if (subjectModel == 0)
  {
    return;
  }

  Properties newProperties = checkToUpdate(subjectModel);
  if (!newProperties.getProperties().empty())
  {
    setProperties(newProperties);
  }
}

te::layout::Properties te::layout::LegendController::checkToUpdate(const AbstractItemModel* subjectModel)
{
  Properties newProperties;

  const Property& pLayersNewUri = subjectModel->getProperty("layers_uri");
  const Property& pLayersCurrentUri = this->getProperty("layers_uri");
  Property pLayersVisibleUri = this->getProperty("visible_layers_uri");

  const std::vector<std::string>& layersNewUri = te::layout::Property::GetValueAs< std::vector<std::string> >(pLayersNewUri);
  const std::vector<std::string>& layersCurrentUri = te::layout::Property::GetValueAs< std::vector<std::string> >(pLayersCurrentUri);
  const std::vector<std::string>& layersVisibleUri = te::layout::Property::GetValueAs< std::vector<std::string> >(pLayersVisibleUri);

  if (layersNewUri != layersCurrentUri)
  {
    newProperties.addProperty(pLayersNewUri);

    if (!someExistsVisibleList(layersNewUri, layersVisibleUri))
    {
      EnumDataType* dataType = Enums::getInstance().getEnumDataType();
      pLayersVisibleUri.setValue(layersNewUri, dataType->getDataTypeStringVector());
      newProperties.addProperty(pLayersVisibleUri);
    }
  }
  return newProperties;
}

bool te::layout::LegendController::someExistsVisibleList(const std::vector<std::string>& layersNewUri, const std::vector<std::string>& layersVisibleUri)
{
  bool result = false;

  for (std::vector<std::string>::const_iterator it = layersVisibleUri.begin(); it != layersVisibleUri.end(); ++it)
  {
    std::string uri = (*it);
    if (std::find(layersNewUri.begin(), layersNewUri.end(), uri) != layersNewUri.end())
    {
      result = true;
      break;
    }
  }

  return result;
}

std::list<te::map::AbstractLayerPtr>  te::layout::LegendController::searchLayersFromURI()
{
  Property property = m_model->getProperty("layers_uri");
  const std::vector<std::string>& vlayersCurrentUri = te::layout::Property::GetValueAs< std::vector<std::string> >(property);

  std::list<te::map::AbstractLayerPtr> layerList;
  
  AbstractProxyProject* project = getAbstractProxyProject();
  if (!project)
  {
    return layerList;
  }

  // search layers 
  for (std::vector<std::string>::const_iterator it = vlayersCurrentUri.begin(); it != vlayersCurrentUri.end(); ++it)
  {
    std::string uri = (*it);
    te::map::AbstractLayerPtr layer = project->getLayerFromURI(uri);
    if (layer)
    {
      layerList.push_back(layer);
    }
  }
  return layerList;
}

std::list<te::map::AbstractLayerPtr>  te::layout::LegendController::searchVisibleLayersFromURI()
{
  Property property = m_model->getProperty("visible_layers_uri");
  const std::vector<std::string>& vlayersCurrentUri = te::layout::Property::GetValueAs< std::vector<std::string> >(property);

  std::list<te::map::AbstractLayerPtr> layerList;

  AbstractProxyProject* project = getAbstractProxyProject();
  if (!project)
  {
    return layerList;
  }

  // search layers 
  for (std::vector<std::string>::const_iterator it = vlayersCurrentUri.begin(); it != vlayersCurrentUri.end(); ++it)
  {
    std::string uri = (*it);
    te::map::AbstractLayerPtr layer = project->getLayerFromURI(uri);
    if (layer)
    {
      layerList.push_back(layer);
    }
  }
  return layerList;
}

te::layout::AbstractProxyProject* te::layout::LegendController::getAbstractProxyProject()
{
  AbstractProxyProject* project = 0;
  LegendItem* view = dynamic_cast<LegendItem*>(m_view);
  if (!view)
  {
    return project;
  }

  ItemInputProxy* itemInputProxy = m_view->getItemInputProxy();
  if (itemInputProxy == 0)
  {
    return project;
  }

  std::list<te::map::AbstractLayerPtr> allLayerList;
  Value<AbstractProxyProject* >* value = itemInputProxy->getContextValues<AbstractProxyProject *>("proxy_project");
  if (value)
  {
    project = value->get();
  }
  return project;
}

void te::layout::LegendController::setProperties(const te::layout::Properties& properties)
{
  te::layout::Properties propertiesCopy = properties;
  
  SharedProperties sharedProps;  
  if (properties.contains(sharedProps.getItemObserver()))
  {
    Property prop = properties.getProperty(sharedProps.getItemObserver());
    const std::string& connectedTo = te::layout::Property::GetValueAs<std::string>(prop);
    if (connectedTo.empty())
    {
      resetSettings(propertiesCopy);

      //we finally set the properties into the model
      AbstractItemController::setProperties(propertiesCopy);
      
      // refresh view item
      LegendItem* legendItem = dynamic_cast<LegendItem*>(this->getView());
      if (legendItem == 0)
      {
        return;
      }
      legendItem->refreshLegendProperties();
      return;
    }
  }

  //we finally set the properties into the model
  AbstractItemController::setProperties(propertiesCopy);
}

void te::layout::LegendController::resetSettings(Properties& properties)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property pLayersCurrentUri = this->getProperty("layers_uri");
  Property pLayersVisibleUri = this->getProperty("visible_layers_uri");

  std::vector<std::string> resetLayersUri;

  pLayersCurrentUri.setValue(resetLayersUri, dataType->getDataTypeStringVector());
  properties.addProperty(pLayersCurrentUri);

  pLayersVisibleUri.setValue(resetLayersUri, dataType->getDataTypeStringVector());
  properties.addProperty(pLayersVisibleUri);
}

