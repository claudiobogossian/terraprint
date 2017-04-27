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

// TerraLib
#include "ThreeNorthController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"

#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "ThreeNorthItem.h"

#ifdef GEOGRAPHICLIB_ENABLED
#include <GeographicLib/MagneticModel.hpp>
#endif

# define M_PI		3.14159265358979323846

te::layout::ThreeNorthController::ThreeNorthController(AbstractItemModel* model, AbstractItemView* view)
  : AbstractItemController(model, view)
{

}

te::layout::ThreeNorthController::~ThreeNorthController()
{

}

void te::layout::ThreeNorthController::update(const Subject* subject)
{
  if (m_model == subject)
  {
    AbstractItemController::update(subject);
    return;
  }

  const AbstractItemModel* subjectModel = dynamic_cast<const AbstractItemModel*>(subject);
  if (subjectModel == 0)
  {
    return;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  const Property& pNewWorldBox = subjectModel->getProperty("world_box");
  const Property& pNewSrid = subjectModel->getProperty("srid");

  Properties properties;

  properties.addProperty(pNewWorldBox);
  properties.addProperty(pNewSrid);

  setProperties(properties);
}

void te::layout::ThreeNorthController::setProperties(const Properties& properties)
{
  Properties newProperties = properties;
  calculateThreeNorth(newProperties);
  AbstractItemController::setProperties(newProperties);
}

double te::layout::ThreeNorthController::calculateMeridianConvergence(const double &latitude, const double &longitude, const double &longitudeSource)
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

void te::layout::ThreeNorthController::calculateThreeNorth(Properties& properties)
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

  Utils utils(0);
  int newSrid = te::layout::Property::GetValueAs<int>(pNewSrid);
  const te::gm::Envelope& newWorldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pNewWorldBox);

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

  int date = te::layout::Property::GetValueAs<int>(getProperty("date"));
  Property pDateNew;
  pDateNew.setName("date");
  pDateNew.setValue(date, dataType->getDataTypeInt());

  if (properties.contains("date"))
  {
    pDateNew = properties.getProperty("date");
  }

  const Property& pdateCurrent = this->getProperty("date");
  int dateNew = te::layout::Property::GetValueAs<int>(pDateNew);
  int dateCurrent = te::layout::Property::GetValueAs<int>(pdateCurrent);
  
#ifdef GEOGRAPHICLIB_ENABLED
  try
  {
    std::string a = TE_LAYOUT_MAGNET_MODEL_DIR;
    GeographicLib::MagneticModel magneticModel("igrf12", TE_LAYOUT_MAGNET_MODEL_DIR);
    magneticModel(dateNew, point.getY(), point.getX(), 0, Bx, By, Bz);

    GeographicLib::MagneticModel::FieldComponents(Bx, By, Bz, H, F, D, I);
  }
  catch (const std::exception& e)
  {
    std::string what = e.what();
  }
#endif

  //end of magnet declination calculation

  double meridianconvergence = calculateMeridianConvergence(geographicReferenceCoord.y, geographicReferenceCoord.x, centralMeridian);
  Property pMeridianConvergenceNew;
  pMeridianConvergenceNew.setName("angle_meridian_convergence");
  pMeridianConvergenceNew.setValue(meridianconvergence, dataType->getDataTypeDouble());

  const Property& pMeridianConvergenceCurrent = this->getProperty("angle_meridian_convergence");
  double meridianConvergenceNew = te::layout::Property::GetValueAs<double>(pMeridianConvergenceNew);
  double meridianConvergenceCurrent = te::layout::Property::GetValueAs<double>(pMeridianConvergenceCurrent);


  if (meridianConvergenceNew != meridianConvergenceCurrent)
  {
    properties.addProperty(pMeridianConvergenceNew);
  }

  double magneticDeclination = D;
  Property pMagneticDeclinationNew;
  pMagneticDeclinationNew.setName("angle_magnetic_north");
  pMagneticDeclinationNew.setValue(magneticDeclination, dataType->getDataTypeDouble());

  const Property& pMagneticDeclinationCurrent = this->getProperty("angle_magnetic_north");
  double magneticDeclinationNew = te::layout::Property::GetValueAs<double>(pMagneticDeclinationNew);
  double magneticDeclinationCurrent = te::layout::Property::GetValueAs<double>(pMagneticDeclinationCurrent);

  if (magneticDeclinationNew != magneticDeclinationCurrent)
  {
    properties.addProperty(pMagneticDeclinationNew);
  }
}

