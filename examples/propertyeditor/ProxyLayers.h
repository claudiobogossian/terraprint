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
  \file ProxyLayers.h
   
   \brief Provide a surrogate or placeholder for te::qt::af::Project to control access to it. 
    A wrapper to access without complexity. This proxy is required because module not must have dependence te::qt::af.
    Useful to access the Layers belonging to the project.

  \ingroup layout
*/

#ifndef  __TERRALIB_LAYOUT_INTERNAL_EXAMPLES_PROXY_LAYERS_H 
#define __TERRALIB_LAYOUT_INTERNAL_EXAMPLES_PROXY_LAYERS_H

// TerraLib
#ifndef Q_MOC_RUN
#include <terralib/layout/core/pattern/proxy/AbstractProxyProject.h>
#include <terralib/maptools/DataSetLayer.h>
#endif

// STL
#include <string>

// Qt
class QString;
class QStringList;

namespace te
{
  namespace qt
  {
    namespace af
    {
      namespace evt
      {
        struct Event;
      }
    }
  }
  namespace layout
  {
    namespace example
    {
      namespace propertyeditor
      {
        /*!
        \brief Provide a surrogate or placeholder for access layers.
        A wrapper to access without complexity. This proxy is required because module not must have dependence te::qt::af.
        Useful to access the Layers belonging to the project.

        \ingroup layout

        \sa te::layout::AbstractProxyProject
        */
        class ProxyLayers : public te::layout::AbstractProxyProject
        {
          public:

            /*!
            \brief Constructor
            */
            ProxyLayers();

            /*!
            \brief Destructor
            */
            virtual ~ProxyLayers();

            /*!
            \brief Reimplemented from AbstractProxyProject
            */
            std::list<te::map::AbstractLayerPtr> getAllLayers(bool invalid = true);

            /*!
            \brief Reimplemented from AbstractProxyProject
            */
            const std::list<te::map::AbstractLayerPtr> getSelectedLayers(bool invalid = true);

            /*!
            \brief Reimplemented from AbstractProxyProject
            */
            te::map::AbstractLayerPtr contains(std::string name);

            /*!
            \brief Reimplemented from AbstractProxyProject
            */
            virtual te::map::AbstractLayerPtr getLayerFromURI(std::string uri);

            /*!
            \brief Reimplemented from AbstractProxyProject
            */
            virtual std::string getURIFromLayer(te::map::AbstractLayerPtr layer);

            bool loadShapesToLayers(QStringList files);

          protected:
            
            bool createLayer(QString fileName);

          protected:

            std::list<te::map::AbstractLayerPtr> m_listLayer;
        };
      }
    }
  }
}

#endif
