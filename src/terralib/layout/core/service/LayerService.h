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
  \file LayerService.h
   
  \brief Service class to manipulate layer data and logic. (Stateless class)

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LAYERSERVICE_H
#define __TERRALIB_LAYOUT_INTERNAL_LAYERSERVICE_H

// TerraLib
#include "../Config.h"

// STL
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "terralib/maptools/AbstractLayer.h"

class QPointF;

namespace te
{
  namespace layout
  {
    class AbstractProxyProject;

    class TELAYOUTEXPORT LayerService
    {
      private:

        /*!
          \brief Constructor
        */ 
        explicit LayerService();


      public:
        
        static std::vector<std::string> encodeLayerList2URI(te::layout::AbstractProxyProject* proxyProject, const std::list<te::map::AbstractLayerPtr>& layerList);

        static std::list<te::map::AbstractLayerPtr> decodeURI2LayerList(te::layout::AbstractProxyProject* proxyProject, const std::vector<std::string>& layerURIList );
    };
  }
}

#endif 
