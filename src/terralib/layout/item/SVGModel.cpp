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
  \file SVCModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "SVGModel.h"
#include "../core/ContextItem.h"
#include "terralib/geometry/Envelope.h"
#include "terralib/color/RGBAColor.h"
#include "terralib/maptools/Canvas.h"
#include "../core/enum/Enums.h"
#include "../core/pattern/mvc/AbstractItemModel.h"

te::layout::SVGModel::SVGModel()
  : AbstractItemModel()
{

  std::string fileName("");

  std::string fileDir("");

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  {
    Property property(0);
    property.setName("file_name");
    property.setLabel(TR_LAYOUT("File Name"));
    property.setValue(fileName, dataType->getDataTypeString());
    property.setEditable(false);
    property.setMenu(true);
    m_properties.addProperty(property);
  }


  {
    Property property(0);
    property.setName("file_dir");
    property.setLabel(TR_LAYOUT("File Dir"));
    property.setValue(fileDir, dataType->getDataTypePath());
    property.setMenu(false);
    property.setVisible(false);
    m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("show_frame");
    property.setValue(false, dataType->getDataTypeBool());
    this->m_properties.updateProperty(property);
  }
  {
    Property property(0);
    property.setName("keep_aspect");
    property.setLabel(TR_LAYOUT("Keep Aspect"));
    property.setValue(true, dataType->getDataTypeBool());
    property.setVisible(true);
    m_properties.completelyUpdateProperty(property);
  }

  {
    Property property(0);
    property.setName("SVG_Library");
    property.setLabel(TR_LAYOUT("SVG Library"));
    std::string name = "";
    property.setValue(name, dataType->getDataTypeSVGView());
    property.setVisible(true);
    property.setMenu(true);
    m_properties.addProperty(property);
  }


  this->reparentProperties(Enums::getInstance().getEnumObjectType()->getSVGItem());
}

te::layout::SVGModel::~SVGModel()
{
  
}

//te::layout::Properties* te::layout::SVGModel::getProperties() const
//{
//  ItemModelObservable::getProperties();
//
//  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
//
//  if(!dataType)
//    return m_properties;
//
//  std::string name = "Choice";
//
//  Property pro_svgchoice(m_hashCode);
//  pro_svgchoice.setName("sgvChoice");
//  pro_svgchoice.setValue(name, dataType->getDataTypeSVGView());
//  pro_svgchoice.setMenu(true);
//  
//  Property pro_svgPaths = getSVGPathsProperty();
//  pro_svgchoice.addSubProperty(pro_svgPaths);
//  
//  m_properties->addProperty(pro_svgchoice);
//
//  return m_properties;
//}
//
//void te::layout::SVGModel::updateProperties( te::layout::Properties* properties, bool notify )
//{
//  ItemModelObservable::updateProperties(properties, false);
//
//  Properties* vectorProps = const_cast<Properties*>(properties);  
//
//  //updateVisitors();
//
//  if(notify)
//  {
//    ContextItem context;
//    notifyAll(context);
//  }  
//}
//
//
//te::layout::Property te::layout::SVGModel::getSVGPathsProperty() const
//{
//  Property prop;
//  prop.setName("paths");
//
//  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
//
//  GenericVariant gv;
//
//  gv.setVector(m_pathsName, dataType->getDataTypeStringVector());
//
//  prop.setValue(gv, dataType->getDataTypeGenericVariant());
//
//  prop.setEditable(false);
//  prop.setVisible(false);
//  return prop;
//}
//
//void te::layout::SVGModel::setPathsName( std::vector<std::string> pathVector )
//{
//  m_pathsName = pathVector;
//}
//
//
