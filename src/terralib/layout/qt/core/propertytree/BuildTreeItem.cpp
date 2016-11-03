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
  \file BuildTreeItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "BuildTreeItem.h"
#include "../pattern/factory/treeItem/PropertyTreeItemFactory.h"
#include "../pattern/factory/treeItem/PropertyTreeItemFactoryParamsCreate.h"
#include "PropertyTreeItem.h"

#include <terralib/common/ParameterizedAbstractFactory.h>

// STL
#include <string>

te::layout::BuildTreeItem::BuildTreeItem()
{
  
}

te::layout::BuildTreeItem::~BuildTreeItem()
{
 
}

te::layout::PropertyTreeItem* te::layout::BuildTreeItem::buildTreeItem(Property & prop, QTreeWidgetItem* parent)
{
  PropertyTreeItemFactoryParamsCreate params(prop, parent);

  EnumType* type = prop.getType();
  std::string editorName = type->getName();    

  te::common::ParameterizedAbstractFactory<PropertyTreeItem, std::string, PropertyTreeItemFactoryParamsCreate>::dictionary_type& d = te::common::ParameterizedAbstractFactory<PropertyTreeItem, std::string, PropertyTreeItemFactoryParamsCreate>::getDictionary();

  PropertyTreeItemFactory* fact = dynamic_cast<PropertyTreeItemFactory*>(d.find(editorName));
  PropertyTreeItem* treeItem = 0;
  if (fact)
  {
    treeItem = te::layout::PropertyTreeItemFactory::make(editorName, params);
  }
  else
  {
    treeItem = new PropertyTreeItem(prop, parent);
  }
  return treeItem;
}

