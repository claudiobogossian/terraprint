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


    Magnetic declination validated from NOAA www.ngdc.noaa.gov/geomag-web/
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
#include "../core/Utils.h"
#include "../core/pattern/singleton/Context.h"
#include "../core/property/SharedProperties.h"
#include "../core/Utils.h"

#ifdef GEOGRAPHICLIB_ENABLED
  #include <GeographicLib/MagneticModel.hpp>
#endif

# define M_PI		3.14159265358979323846

te::layout::ThreeNorthModel::ThreeNorthModel()
: AbstractItemModel()
{
  this->m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getThreeNorthItem());
  te::gm::Envelope wbox;
  int srid = 0;
  te::color::RGBAColor color(0, 0, 0, 255);
  double meridian_convergence = 0;
  double magnetic_north = 0;
  int date = 2015;
  std::string itemName = "";
  SharedProperties sharedProps;
  double lineWidth = Utils::getLineWidthMinimumValue();

  bool magneticVisible = false;
  bool magneticMenu = false;
  bool magneticValue = false;
  bool dateVisible = false;
  bool dateMenu = false;

#ifdef GEOGRAPHICLIB_ENABLED
  magneticVisible = true;
  magneticMenu = true;
  magneticValue = true;
  dateVisible = true;
  dateMenu = true;
#endif

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  {
    {
      Property property(0);
      property.setName("line_width");
      property.setLabel(TR_LAYOUT("Line Width"));
      property.setVisible(false);
      property.setValue(lineWidth, dataType->getDataTypeDouble());
      this->m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName(sharedProps.getItemObserver());
      property.setLabel(TR_LAYOUT("Connection with"));
      property.setComposeWidget(true);
      property.setValue(itemName, dataType->getDataTypeItemObserver());
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
      property.setName("meridian_convergence");
      property.setLabel(TR_LAYOUT("Meridian Convergence"));
      property.setValue(true, dataType->getDataTypeBool());
      property.setMenu(true);
      this->m_properties.addProperty(property);
    }
    {
      Property property(0);
      property.setName("magnetic_north");
      property.setLabel(TR_LAYOUT("Magnetic North"));
      property.setValue(magneticValue, dataType->getDataTypeBool());
      property.setVisible(magneticVisible);
      property.setMenu(magneticMenu);
      this->m_properties.addProperty(property);
    }

   {
      Property property(0);
      property.setName("angle_meridian_convergence");
      property.setLabel(TR_LAYOUT("Angle Meridian Convergence"));
      property.setValue(meridian_convergence, dataType->getDataTypeDouble());
      property.setMenu(true);
      this->m_properties.addProperty(property);
    }

   {
      Property property(0);
      property.setName("angle_magnetic_north");
      property.setLabel(TR_LAYOUT("Angle Magnetic North"));
      property.setValue(magnetic_north, dataType->getDataTypeDouble());
      property.setVisible(magneticVisible);
      property.setMenu(magneticMenu);
      this->m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName("width");
      property.setValue(110., dataType->getDataTypeDouble());
      m_properties.updateProperty(property);
    }

    {
      Property property(0);
      property.setName("keep_aspect");
      property.setValue(true, dataType->getDataTypeBool());
      property.setVisible(false);
      m_properties.completelyUpdateProperty(property);
   }

    {
      Property property(0);
      property.setName("date");
      property.setLabel(TR_LAYOUT("Date"));
      property.setValue(date, dataType->getDataTypeInt());
      property.setMenu(dateMenu);
      property.setVisible(dateVisible);
      m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName("world_box");
      property.setLabel(TR_LAYOUT("World Box"));
      property.setValue(wbox, dataType->getDataTypeEnvelope());
      m_properties.addProperty(property);
    }

    {
      Property property(0);
      property.setName("srid");
      property.setLabel(TR_LAYOUT("SRID"));
      property.setValue(srid, dataType->getDataTypeInt());
      m_properties.addProperty(property);
    }
  }
}

te::layout::ThreeNorthModel::~ThreeNorthModel()
{

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

void te::layout::ThreeNorthModel::setProperty(const Property& property)
{
    Properties properties;
    properties.addProperty(property);
    setProperties(properties);
}

void te::layout::ThreeNorthModel::setProperties(const Properties& properties)
{
    Properties newProperties;
    const std::vector<Property>& vecProperties = properties.getProperties();
    for (unsigned int i = 0; i < vecProperties.size(); ++i)
    {
      const Property& property = vecProperties[i];
      newProperties.addProperty(property);
    }

    calculateThreeNorth(newProperties);
    AbstractItemModel::setProperties(newProperties);
}

void te::layout::ThreeNorthModel::calculateThreeNorth(Properties& properties)
{
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    Property pNewWorldBox = getProperty("world_box");
    Property pNewSrid = getProperty("srid");

    if (properties.contains("world_box"))
    {
      pNewWorldBox = properties.getProperty("world_box");
    }

    if (properties.contains("srid"))
    {
      pNewSrid = properties.getProperty("srid");
    }

    Utils utils(0, 0);
    int newSrid = pNewSrid.getValue().toInt();
    const te::gm::Envelope& newWorldBox = pNewWorldBox.getValue().toEnvelope();

    if (newSrid <= 0 && !newWorldBox.isValid())
    {
      return;
    }
    te::gm::Envelope newGeographicBox = utils.getWorldBoxInGeographic(newWorldBox, newSrid);

    te::gm::Coord2D referenceCoord = newWorldBox.getCenter();
    te::gm::Coord2D geographicReferenceCoord = newGeographicBox.getCenter();
    te::gm::Point point(referenceCoord.getX(), referenceCoord.getY(), newSrid);


    double longitude = geographicReferenceCoord.x;
    int meridiano = (int)(longitude / 6);
    meridiano = meridiano * 6;

    meridiano = abs(meridiano) + 3;

    double centralMeridian = meridiano;
    if (geographicReferenceCoord.x < 0.)
    {
      centralMeridian *= -1.;
    }

    //magnetic declination

    //we must ensure that the point is in LatLong/WGS84 CS
    if (newSrid != 4326)
    {
      point.transform(4326);
    }

    double Bx = 0.;
    double By = 0.;
    double Bz = 0.;
    double H = 0.;
    double F = 0.;
    double D = 0.;
    double I = 0.;

    int date = getProperty("date").getValue().toInt();
    Property pDateNew;
    pDateNew.setName("date");
    pDateNew.setValue(date, dataType->getDataTypeInt());

    if (properties.contains("date"))
    {
      pDateNew = properties.getProperty("date");
    }

    const Property& pdateCurrent = this->getProperty("date");
    int dateNew = pDateNew.getValue().toInt();
    int dateCurrent = pdateCurrent.getValue().toInt();

    /*if (dateNew != dateCurrent)
    {
      properties.addProperty(dateNew);
    }*/

#ifdef GEOGRAPHICLIB_ENABLED
    try
    {
      GeographicLib::MagneticModel magneticModel("igrf12", TE_LAYOUT_MAGNET_MODEL_DIR);
      magneticModel(dateNew, point.getY(), point.getX(), 0, Bx, By, Bz);

      GeographicLib::MagneticModel::FieldComponents(Bx, By, Bz, H, F, D, I);
    }

    catch (const exception& /*e*/)
    {
      
    }
#endif

    //end of magnet declination calculation

    double meridianconvergence = calculateMeridianConvergence(geographicReferenceCoord.y, geographicReferenceCoord.x, centralMeridian);
    Property pMeridianConvergenceNew;
    pMeridianConvergenceNew.setName("angle_meridian_convergence");
    pMeridianConvergenceNew.setValue(meridianconvergence, dataType->getDataTypeDouble());

    const Property& pMeridianConvergenceCurrent = this->getProperty("angle_meridian_convergence");
    double meridianConvergenceNew = pMeridianConvergenceNew.getValue().toDouble();
    double meridianConvergenceCurrent = pMeridianConvergenceCurrent.getValue().toDouble();


    if (meridianConvergenceNew != meridianConvergenceCurrent)
    {
      properties.addProperty(pMeridianConvergenceNew);
    }

    double magneticDeclination = D;
    Property pMagneticDeclinationNew;
    pMagneticDeclinationNew.setName("angle_magnetic_north");
    pMagneticDeclinationNew.setValue(magneticDeclination, dataType->getDataTypeDouble());

    const Property& pMagneticDeclinationCurrent = this->getProperty("angle_magnetic_north");
    double magneticDeclinationNew = pMagneticDeclinationNew.getValue().toDouble();
    double magneticDeclinationCurrent = pMagneticDeclinationCurrent.getValue().toDouble();

    if (magneticDeclinationNew != magneticDeclinationCurrent)
    {
      properties.addProperty(pMagneticDeclinationNew);
    }
}
