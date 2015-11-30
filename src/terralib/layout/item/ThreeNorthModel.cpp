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
  \file ThreeNorthModel.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ThreeNorthModel.h"
#include "../core/enum/Enums.h"
#include "../core/property/Properties.h"
#include "../core/property/Property.h"
#include "geos/platform.h"
#include "../core/Utils.h"
#include "../core/pattern/singleton/Context.h"

te::layout::ThreeNorthModel::ThreeNorthModel()
: AbstractItemModel()
, Observer()
{
  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getThreeNorthItem());

  te::color::RGBAColor color(0, 0, 0, 255);
  double magnetic_convergence_angle = 0;
  double magnetic_angle = -0.17;
  
  

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  {
	  std::string emptyString;

	  Property property(0);
	  property.setName("map_name");
    property.setLabel(TR_LAYOUT("Map Name"));
	  property.setValue(emptyString, dataType->getDataTypeStringList());

	  Variant v;
	  v.setValue(emptyString, dataType->getDataTypeString());
	  property.addOption(v);
	  m_properties.addProperty(property);
  }

  {
    Property property(0);
    property.setName("color");
    property.setLabel(TR_LAYOUT("color"));
    property.setValue(color, dataType->getDataTypeColor());
    this->m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("magnetic_convergence");
    property.setLabel(TR_LAYOUT("magnetic_convergence"));
    property.setValue(false, dataType->getDataTypeBool());
    property.setMenu(true);
    this->m_properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("magnetic_north");
    property.setLabel(TR_LAYOUT("magnetic_north"));
    property.setValue(false, dataType->getDataTypeBool());
    property.setMenu(true);
    this->m_properties.addProperty(property);
  }

  {
	  Property property(0);
	  property.setName("angle_magnetic_convergence");
    property.setLabel(TR_LAYOUT("angle_magnetic_convergence"));
	  property.setValue(magnetic_convergence_angle, dataType->getDataTypeDouble());
	  property.setMenu(true);
	  this->m_properties.addProperty(property);
  }

  {
	  Property property(0);
	  property.setName("angle_magnetic_north");
    property.setLabel(TR_LAYOUT("angle_magnetic_north"));
	  property.setValue(magnetic_angle, dataType->getDataTypeDouble());
	  property.setMenu(true);
	  this->m_properties.addProperty(property);
  }
}

void te::layout::ThreeNorthModel::update(const Subject* subject)
{
	const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
	if (subjectModel == 0)
	{
		return;
	}

	EnumDataType* dataType = Enums::getInstance().getEnumDataType();
	
	const Property& pNewWorldBox = subjectModel->getProperty("world_box");
	const Property& pNewSrid = subjectModel->getProperty("srid");


	Utils utils(0, 0);
	int newSrid = pNewSrid.getValue().toInt();
	const te::gm::Envelope& newWorldBox = pNewWorldBox.getValue().toEnvelope();
	te::gm::Envelope newGeographicBox = utils.getWorldBoxInGeographic(newWorldBox, newSrid);

	te::gm::Coord2D referenceCoord = newGeographicBox.getCenter();

	double longitude = referenceCoord.x;
	int meridiano = (int)(longitude / 6);
	meridiano = meridiano * 6;

	meridiano = abs(meridiano) + 3;

	double centralMeridian = meridiano;
	if (referenceCoord.x < 0.)
	{
		centralMeridian *= -1.;
	}

	double meridianconvergence = calculateMeridianConvergence(referenceCoord.y, referenceCoord.x, centralMeridian);
	Property pMeridianConvergenceNew;
	pMeridianConvergenceNew.setName("angle_magnetic_convergence");
	pMeridianConvergenceNew.setValue(meridianconvergence, dataType->getDataTypeDouble());

	const Property& pMeridianConvergenceCurrent = this->getProperty("angle_magnetic_convergence");
	double meridianConvergenceNew = pMeridianConvergenceNew.getValue().toDouble();
	double meridianConvergenceCurrent = pMeridianConvergenceCurrent.getValue().toDouble();

	if (meridianConvergenceNew != meridianConvergenceCurrent)
	{
		setProperty(pMeridianConvergenceNew);
	}
}

double te::layout::ThreeNorthModel::calculateMeridianConvergence(const double &latitude, const double &longitude, const double &longitudeSource)
{
	double deltaLambida;
	double p1;
	double p3;
	double p5;
	double XII;
	double XIIp;
	double XIIIp3;
	double sen1 = sin(M_PI / (180. * 3600.));
	double e2 = 0.00673949677548; //SIRGAS2000
	double C5;
	double  fi;

	double TeCDR = 0.01745329251994329576;    //!< Conversion factor: degrees to radians

	fi = (latitude*(4 * atan(1.0))) / 180.;
	fi = latitude * TeCDR;

	deltaLambida = fabs((longitudeSource - longitude) * 3600);

	p1 = deltaLambida*0.0001;
	p3 = p1*p1*p1;
	p5 = p1*p1*p1*p1*p1;

	XII = sin(fi) * 10000;
	XIIp = p1*XII;

	double pow12 = std::pow(10., 12.);
	double pow20 = std::pow(10., 20.);

	double XIII = (sen1*sen1*sin(fi)*cos(fi)*cos(fi)) / 3;
	XIII = XIII * (1 + (3 * e2 * cos(fi) * cos(fi)) + (2 * e2 * e2 * cos(fi)*cos(fi)* cos(fi)*cos(fi)));
	XIII = XIII * pow12;
	XIIIp3 = XIII * p3;

	C5 = (p5*(sen1*sen1*sen1*sen1)*sin(fi)*cos(fi)*cos(fi)* cos(fi)*cos(fi)) / 15;
	C5 *= (2 - tan(fi)*tan(fi))*(pow20);

	C5 += (XIIp + XIIIp3);


	C5 /= 3600.0;
	return C5;
}

te::layout::ThreeNorthModel::~ThreeNorthModel()
{

}

