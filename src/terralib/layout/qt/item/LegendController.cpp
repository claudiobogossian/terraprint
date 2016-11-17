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
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/proxy/AbstractProxyProject.h"
#include "../core/Value.h"
#include "../core/Scene.h"

te::layout::LegendController::LegendController(te::layout::AbstractItemModel* model)
  : AbstractItemController(model)
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

  const Property& pLayersNewUri = subjectModel->getProperty("layers_uri");
  const Property& pLayersCurrentUri = this->getProperty("layers_uri");

  const std::vector<std::string>& layersNewUri = te::layout::Property::GetValueAs< std::vector<std::string> >(pLayersNewUri);
  const std::vector<std::string>& layersCurrentUri = te::layout::Property::GetValueAs< std::vector<std::string> >(pLayersCurrentUri);

  if (layersNewUri != layersCurrentUri)
  {
    setProperty(pLayersNewUri);
  }
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

te::layout::AbstractProxyProject* te::layout::LegendController::getAbstractProxyProject()
{
  AbstractProxyProject* project = 0;
  LegendItem* view = dynamic_cast<LegendItem*>(m_view);
  if (!view)
  {
    return project;
  }

  Scene* scene = dynamic_cast<Scene*>(view->scene());
  if (!scene)
  {
    return project;
  }

  std::list<te::map::AbstractLayerPtr> allLayerList;
  Value<AbstractProxyProject* >* value = scene->getContextValues<AbstractProxyProject *>("proxy_project");
  if (value)
  {
    project = value->get();
  }
  return project;
}

