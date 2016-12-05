/*  Copyright (C) 2013-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file ProxyLayers.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ProxyLayers.h"
#include "terralib/qt/af/ApplicationController.h"
#include "terralib/qt/af/events/LayerEvents.h"
#include <terralib/dataaccess.h>
#include <terralib/datatype/Property.h>
#include "terralib/dataaccess/datasource/DataSourceInfoManager.h"
#include <terralib/dataaccess/datasource/DataSourceFactory.h>
#include <terralib/common/StringUtils.h>
#include <terralib/geometry/GeometryProperty.h>
#include <terralib/ogr/Config.h>
#include <terralib/ogr/DataSource.h>

#include "terralib/layout/qt/core/ItemUtils.h"

// STL
#include <iostream>
#include <map>

// Qt
#include <QString>
#include <QStringList>

te::layout::example::propertyeditor::ProxyLayers::ProxyLayers()
{
  
}

te::layout::example::propertyeditor::ProxyLayers::~ProxyLayers()
{
  
}

bool te::layout::example::propertyeditor::ProxyLayers::loadShapesToLayers(QStringList files)
{
  for (QStringList::iterator it = files.begin(); it != files.end(); ++it)
  {
    QString fileName = *it;
    if (!fileName.isEmpty())
    {
      if (fileName.endsWith(".shp") == false)
      {
        fileName.append(".shp");
      }
      return createLayer(fileName);
    }
  }
  return false;
}

bool te::layout::example::propertyeditor::ProxyLayers::createLayer(QString fileName)
{
  std::map<std::string, std::string> connInfo;
  connInfo["URI"] = ItemUtils::convert2StdString(fileName);
  
  // Creates and connects data source
  std::auto_ptr<te::da::DataSource> datasource = te::da::DataSourceFactory::make("OGR");
  datasource->setConnectionInfo(connInfo);
  datasource->open();

  // Get the number of data set types that belongs to the data source
  std::vector<std::string> datasets = datasource->getDataSetNames();
  assert(!datasets.empty());

  // Gets the first dataset
  std::string datasetName(datasets[0]);
  std::auto_ptr<te::da::DataSetType> dt(datasource->getDataSetType(datasetName));
  assert(dt->hasGeom());

  te::gm::GeometryProperty* geomProperty = te::da::GetFirstGeomProperty(dt.get());
  assert(geomProperty);

  // Box
  std::auto_ptr<te::gm::Envelope> extent(datasource->getExtent(datasetName, geomProperty->getName()));
  
  // Creates a DataSetLayer
  te::map::DataSetLayer* layer = new te::map::DataSetLayer(te::common::Convert2String(0), datasetName);
  layer->setDataSourceId(datasource->getId());
  layer->setDataSetName(datasetName);
  layer->setExtent(*extent);
  layer->setRendererType("DATASET_LAYER_RENDERER");

  m_listLayer.push_back(layer);

  return true;
}

std::list<te::map::AbstractLayerPtr> te::layout::example::propertyeditor::ProxyLayers::getAllLayers(bool invalid /*= true*/)
{
  return m_listLayer;
}

const std::list<te::map::AbstractLayerPtr> te::layout::example::propertyeditor::ProxyLayers::getSelectedLayers(bool invalid /*= true*/)
{
  return m_listLayer;
}

te::map::AbstractLayerPtr te::layout::example::propertyeditor::ProxyLayers::contains(std::string name)
{
  te::map::AbstractLayerPtr layer;

  std::list<te::map::AbstractLayerPtr> layers = getAllLayers();

  std::list<te::map::AbstractLayerPtr>::iterator it;

  for(it = layers.begin() ; it != layers.end() ; ++it)
  {
    te::map::AbstractLayerPtr layerPtr = (*it);
    if(!layerPtr)
      continue;

    if(layerPtr->getTitle().compare(name) == 0)
    {
      layer = layerPtr;
      break;
    }
  }
  return layer;
}

te::map::AbstractLayerPtr te::layout::example::propertyeditor::ProxyLayers::getLayerFromURI(std::string uri)
{
  te::map::AbstractLayerPtr layer;
  std::string uriInfo = "URI";

  std::list<te::map::AbstractLayerPtr> layers = getAllLayers();

  std::list<te::map::AbstractLayerPtr>::iterator it;

  for (it = layers.begin(); it != layers.end(); ++it)
  {
    te::map::AbstractLayerPtr layerPtr = (*it);
    if (!layerPtr)
      continue;
    const std::string& id = it->get()->getDataSourceId();

    te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
    const std::map<std::string, std::string>& connInfo = info->getConnInfo();

    std::string uriFromLayer = "";
    for (std::map<std::string, std::string>::const_iterator it = connInfo.begin(); it != connInfo.end(); ++it)
    {
      uriFromLayer = it->second;
      if (uriFromLayer.compare(uri) == 0)
      {
        layer = layerPtr;
        break;
      }
    }
  }
  return layer;
}

std::string te::layout::example::propertyeditor::ProxyLayers::getURIFromLayer(te::map::AbstractLayerPtr layer)
{
  std::string uri;
  std::string uriInfo = "URI";

  const std::string& id = layer->getDataSourceId();

  te::da::DataSourceInfoPtr info = te::da::DataSourceInfoManager::getInstance().get(id);
  if (info)
  {
    const std::map<std::string, std::string>& connInfo = info->getConnInfo();
    for (std::map<std::string, std::string>::const_iterator it = connInfo.begin(); it != connInfo.end(); ++it)
    {
      std::string nameURI = it->first;
      if (nameURI.compare(uriInfo) == 0)
      {
        uri = it->second;
        break;
      }
    }
  }
  return uri;
}

