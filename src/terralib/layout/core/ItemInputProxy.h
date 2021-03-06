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


#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_INPUT_PROXY_H
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_INPUT_PROXY_H

#include "Config.h"
#include "Value.h"
#include "AbstractScene.h"

#include <string>

namespace te
{
  namespace layout
  {
    class AbstractItemView;
    class AbstractScene;
    class ContextObject;
    class Utils;

    class TELAYOUTEXPORT ItemInputProxy
    {

    public:

      ItemInputProxy(AbstractScene* scene);
      virtual ~ItemInputProxy();

      AbstractItemView* getItem(const std::string& itemName) const;

      Utils getUtils() const;

      virtual const ContextObject& getContext() const;

      template <typename T>
      Value<T>* getContextValues(const std::string& name) const;

    private:

      AbstractScene* m_scene;

    };

    template <typename T>
    inline te::layout::Value<T>* te::layout::ItemInputProxy::getContextValues(const std::string& name) const
    {
      return m_scene->getContextValues<T>(name);
    }
  }
}

#endif //__TERRALIB_LAYOUT_INTERNAL_ITEM_INPUT_PROXY_H
