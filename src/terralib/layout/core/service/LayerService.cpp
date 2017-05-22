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
  \file Utils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LayerService.h"

#include "../enum/EnumDataType.h"
#include "../enum/Enums.h"
#include "../enum/Enums.h"
//#include "../terralib/maptools/AbstractLayer.h"
#include "../pattern/proxy/AbstractProxyProject.h"

te::layout::LayerService::LayerService()
{
}

std::vector<std::string> te::layout::LayerService::encodeLayerList2URI(te::layout::AbstractProxyProject* proxyProject, const std::list<te::map::AbstractLayerPtr>& layerList)
{
  std::vector<std::string>  vString;

  for (std::list<te::map::AbstractLayerPtr>::const_iterator it = layerList.begin(); it != layerList.end(); ++it)
  {
	  te::map::AbstractLayerPtr layer = (*it);

	  std::string uri = proxyProject->getURIFromLayer(layer);

	  vString.push_back(uri);
  }

  return vString;

}
std::list<te::map::AbstractLayerPtr> te::layout::LayerService::decodeURI2LayerList(te::layout::AbstractProxyProject* proxyProject, const std::vector<std::string>& layerURIList )
{
  std::list<te::map::AbstractLayerPtr> layerList;

    // search layers
    for (unsigned int i=0;i< layerURIList.size(); i++)
    {
      std::string uri =  layerURIList[i];
      te::map::AbstractLayerPtr layer = proxyProject->getLayerFromURI(uri);
      if (layer)
      {
	layerList.push_back(layer);
      }
    }

  return layerList;
}
