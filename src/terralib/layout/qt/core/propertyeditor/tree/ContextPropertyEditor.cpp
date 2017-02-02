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
  \file ContextPropertyEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ContextPropertyEditor.h"
#include "../../../../core/pattern/proxy/AbstractProxyProject.h"
#include "../../../../core/pattern/mvc/AbstractItemView.h"
#include "../../../../core/pattern/mvc/AbstractItemController.h"
#include "../../../../core/property/Properties.h"
#include "../../../../core/property/Property.h"
#include "../../../../core/enum/Enums.h"
#include "../../../../qt/core/Scene.h"
#include "../../../../qt/core/ItemUtils.h"

// Qt
#include <QStringList>
#include <QGraphicsItem>

te::layout::ContextPropertyEditor::ContextPropertyEditor(AbstractProxyProject* proxy, Scene* scene):
  m_proxy(proxy),
  m_scene(scene)
{

}

te::layout::ContextPropertyEditor::~ContextPropertyEditor()
{

}

te::layout::AbstractProxyProject* te::layout::ContextPropertyEditor::getProxy()
{
  return m_proxy;
}

QStringList te::layout::ContextPropertyEditor::getMapNames()
{
  EnumObjectType* dataTypeObj = Enums::getInstance().getEnumObjectType();
  return getItemNames(dataTypeObj->getMapItem());
}

QStringList te::layout::ContextPropertyEditor::getItemNames(const EnumType* type)
{
  QStringList list;

  if (!m_scene)
    return list;

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
          list.append(txt);
        }
      }
    }
  }

  return list;
}
