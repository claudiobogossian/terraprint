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
  \file LegendChoiceController.h
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendChoiceController.h"
#include "../core/property/Property.h"
#include "../qt/item/LegendItem.h"

// Qt
#include <QStringList>

te::layout::LegendChoiceController::LegendChoiceController(Scene * scene, AbstractProxyProject * proxy, AbstractOutsideModel* o) :
  AbstractOutsideController(o),
  m_scene(scene),
  m_proxy(proxy)
{
  
}

te::layout::LegendChoiceController::~LegendChoiceController()
{

}

te::layout::Property te::layout::LegendChoiceController::getProperty(std::string name)
{
  Property prop;

  QList<QGraphicsItem*> items = m_scene->selectedItems();
  if (items.isEmpty())
    return prop;

  QGraphicsItem* item = items.first();
  LegendItem* legendItem = dynamic_cast<LegendItem*>(item);
  if (legendItem)
  {
    prop = legendItem->getProperty(name);
  }
  return prop;
}

std::list<te::map::AbstractLayerPtr> te::layout::LegendChoiceController::getlistLayers()
{
  std::list<te::map::AbstractLayerPtr> list;

  if (m_proxy)
  {
    list = searchLayers();
  }

  return list;
}

std::list<te::map::AbstractLayerPtr> te::layout::LegendChoiceController::searchLayers(const std::string& name)
{
  std::string propertyName = name;
  if (name.empty())
  {
    propertyName = "layers_uri";
  }

  Property prop = getProperty(propertyName);
  const std::vector<std::string>& vlayersCurrentUri = te::layout::Property::GetValueAs< std::vector<std::string> >(prop);

  std::list<te::map::AbstractLayerPtr> layerList;

  if (!m_proxy)
  {
    return layerList;
  }

  // search layers 
  for (std::vector<std::string>::const_iterator it = vlayersCurrentUri.begin(); it != vlayersCurrentUri.end(); ++it)
  {
    std::string uri = (*it);
    te::map::AbstractLayerPtr layer = m_proxy->getLayerFromURI(uri);
    if (layer)
    {
      layerList.push_back(layer);
    }
  }
  return layerList;
}

std::list<te::map::AbstractLayerPtr> te::layout::LegendChoiceController::getSelectedlayers()
{
  std::string name = "visible_layers_uri";
  std::list<te::map::AbstractLayerPtr> selectedLayers = searchLayers(name);

  return selectedLayers;
}

QStringList te::layout::LegendChoiceController::getItemNames(const QStringList& list, const EnumType* type)
{
  QStringList newList = list;

  QList<QGraphicsItem*> items = m_scene->items();
  foreach(QGraphicsItem* item, items)
  {
    if (item)
    {
      AbstractItemView* view = dynamic_cast<AbstractItemView*>(item);
      if (view)
      {
        const Property& prop_name = view->getProperty("name");

        std::string value = te::layout::Property::GetValueAs<std::string>(prop_name);
        QString txt = ItemUtils::convert2QString(value);

        const Properties& prop_type = view->getProperties();

        if (txt.compare("") != 0 && prop_type.getTypeObj() == type)
        {
          newList.append(txt);
        }
      }
    }
  }

  return newList;
}

std::string te::layout::LegendChoiceController::searchLayerToURI(te::map::AbstractLayerPtr layer)
{
  return m_proxy->getURIFromLayer(layer);
}

