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
#include "Utils.h"

#include "enum/EnumDataType.h"
#include "enum/Enums.h"
#include "../core/ContextObject.h"
#include "enum/AbstractType.h"
#include "pattern/singleton/Context.h"
#include "AbstractScene.h"
#include "PaperConfig.h"
#include "property/Properties.h"
#include "property/Property.h"
#include "../qt/core/Scene.h"
#include "WorldTransformer.h"

// TerraLib
#include "terralib/geometry/Polygon.h"
#include "terralib/geometry/Enums.h"
#include "terralib/geometry/LinearRing.h"
#include "terralib/geometry/Point.h"
#include "terralib/maptools/Utils.h"
#include "terralib/maptools/WorldDeviceTransformer.h"
#include "terralib/srs/SpatialReferenceSystemManager.h"
#include "terralib/srs/Converter.h"
#include "terralib/common/Translator.h"
#include "terralib/common/StringUtils.h"

//boost
#include <boost/tokenizer.hpp>

// STL
#include <math.h> /* isinf, isnan, isfinite */
#include <string>
#include <sstream> 
#include <exception>
#include <memory> /* unique_ptr */
#include <algorithm> /* min, max */

double te::layout::Utils::m_lineWidthMinimumValue = 0.3;

te::layout::Utils::Utils(te::layout::Scene* scene) 
  : m_applyZoom(true)
  , m_scene(scene)
{

}

te::layout::Utils::~Utils()
{

}

int te::layout::Utils::mm2pixel( double mm )
{
  if(!m_scene)
  {
    return -1;
  }
  const ContextObject& context = m_scene->getContext();

  double devDpi = (double)context.getDpiX();
  return mm2pixel(mm, devDpi);
}

int te::layout::Utils::mm2pixel(double mm, double dpi)
{
  int px = roundNumber((mm * dpi) / 25.4);
  return px;
}

double te::layout::Utils::pixel2mm( int pixel )
{
  if(!m_scene)
  {
    return -1;
  }
  const ContextObject& context = m_scene->getContext();

  double devDpi = (double)context.getDpiX();
  return pixel2mm(pixel, devDpi);
}

double te::layout::Utils::pixel2mm(int pixel, double dpi)
{
  double mm = (pixel / dpi) * 25.4;
  return mm;
}

te::gm::Envelope te::layout::Utils::viewportBox( te::gm::Envelope box)
{
  te::gm::Envelope boxViewport;
  
  if(!box.isValid())
    return boxViewport;

  boxViewport = viewportBoxFromMM(box);
  return boxViewport;
}

te::gm::Envelope te::layout::Utils::viewportBoxFromMM( te::gm::Envelope box )
{
  te::map::WorldDeviceTransformer transf; // World Device Transformer.

  int zoom = 100;
  if(m_applyZoom)
  {
    if(!m_scene)
    {
      te::gm::Envelope env;
      return env;
    }
    ContextObject context = m_scene->getContext();
    zoom = context.getZoom();
  }

  double zoomFactor = (double)zoom / 100.;

  int pxwidth = mm2pixel(box.getWidth() * zoomFactor);
  int pxheight = mm2pixel(box.getHeight() * zoomFactor);
    
  // Adjust internal renderer transformer
  transf.setTransformationParameters(box.getLowerLeftX(), box.getLowerLeftY(), 
    box.getUpperRightX(), box.getUpperRightY(), pxwidth, pxheight);
  
  te::gm::Envelope boxViewport = transformToViewport(transf, box);
  return boxViewport;
}

te::gm::Envelope te::layout::Utils::transformToViewport( te::map::WorldDeviceTransformer transf, te::gm::Envelope box )
{
  double px1 = 0;
  double py1 = 0;
  double px2 = 0;
  double py2 = 0;
  double pycopy = 0;
  transf.world2Device(box.getLowerLeftX(), box.getLowerLeftY(), px1, py1);
  transf.world2Device(box.getUpperRightX(), box.getUpperRightY(), px2, py2);

  if(py1 > py2 )
  {
    pycopy = py1;
    py1 = py2;
    py2 = pycopy;
  }

  te::gm::Envelope boxViewport(px1, py1, px2, py2);
  return boxViewport;
}

te::gm::LinearRing* te::layout::Utils::addCoordsInX( te::gm::Envelope box, double axisCoord, double gap )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(te::gm::LineStringType);
  std::map<int, te::gm::Point> coords;

  int count = 1;
  for(double sub_x = box.getLowerLeftX(); sub_x < box.getUpperRightX(); sub_x +=(gap / 4.))
  {
    coords[count] = te::gm::Point(sub_x, axisCoord);
    count+=1;
  }

  line->setNumCoordinates(count + 1);
  line->setPointN(0, te::gm::Point(box.getLowerLeftX(), axisCoord));

  for(int i = 1 ; i < count ; ++i)
  {
    line->setPointN(i, coords[i]);
  }

  line->setPointN(count, te::gm::Point(box.getUpperRightX(), axisCoord));

  return line;
}

te::gm::LinearRing* te::layout::Utils::addCoordsInY( te::gm::Envelope box, double axisCoord, double gap )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(te::gm::LineStringType);
  std::map<int, te::gm::Point> coords;

  int count = 1;
  for(double sub_y = box.getLowerLeftY(); sub_y < box.getUpperRightY(); sub_y +=(gap / 4.))
  {
    coords[count] = te::gm::Point(axisCoord, sub_y);
    count+=1;
  }

  line->setNumCoordinates(count + 1);
  line->setPointN(0, te::gm::Point(axisCoord, box.getLowerLeftY()));

  for(int i = 1 ; i < count ; ++i)
  {
    line->setPointN(i, coords[i]);
  }

  line->setPointN(count, te::gm::Point(axisCoord, box.getUpperRightY()));

  return line;
}

te::layout::WorldTransformer te::layout::Utils::getTransformGeo(te::gm::Envelope boxgeo, te::gm::Envelope boxmm)
{
  WorldTransformer transf; // World Transformer.

  if(!boxgeo.isValid())
    return transf;

  if(!boxmm.isValid())
    return transf;
  
  // Adjust internal renderer transformer
  transf.setTransformationParameters(boxgeo, boxmm);

  return transf;
}

std::string te::layout::Utils::convertDecimalToDegree(const double& value, bool bDegrees, bool bMinutes, bool bSeconds, int precision)
{
  std::string    degreeValue;
  double      dbValue;
  double      degree;
  double      sec;
  double      min;

  degree = (int)value;
  dbValue = value - degree;

  min = std::fabs((dbValue-(int)dbValue)*60.);
  sec = std::fabs(std::fabs((min-int(min))*60.));
  
  if(roundNumber(sec) >= 60)
  {
    min++;
    sec=0;
  }

  min = std::floor(min);
  if(min >= 60.0)
  {
    min = 0;
    dbValue++;
  }

  std::string space = "";

  if (bDegrees)
  {
    degreeValue = te::common::Convert2String(std::floor(degree), 0) + "�";
    space = " ";
  }
  if (bMinutes)
  {
    degreeValue += space + te::common::Convert2String(std::floor(min), 0) + "'";
    space = " ";
  }
  if (bSeconds)
  {
    degreeValue += space + te::common::Convert2String(std::fabs(sec), precision) + "''";
  }

  if(bDegrees == false && bMinutes == false && bSeconds == false)
    degreeValue = te::common::Convert2String(std::floor(degree), 0) + "� " + te::common::Convert2String(std::fabs(min), 0) + "' " + te::common::Convert2String(std::fabs(sec), precision) + "''";

  return degreeValue;
}

std::string te::layout::Utils::convertDecimalToDegreeANP(const double& value, bool bDegrees, bool bMinutes, bool bSeconds, int precision)
{
  double      dbValue;
  double      degree;
  double      sec;
  double      min;

  degree = (int)value;
  dbValue = value - degree;

  min = std::fabs((dbValue - (int)dbValue)*60.);
  sec = std::fabs(std::fabs((min - int(min))*60.));

  if (roundNumber(sec) >= 60)
  {
    min++;
    sec = 0;
  }

  min = std::floor(min);
  if (min >= 60.0)
  {
    min = 0;
    dbValue++;
  }

  std::string space = "";

  std::string signal = "+";
  if (degree < 0.)
  {
    signal = "-";
  }

  std::string degreeValue;
  std::string minuteValue;
  std::string secondValue;

  degreeValue = te::common::Convert2String(std::floor(std::fabs(degree)), 0);
  degreeValue = lpadString(degreeValue, 2, '0');
  degreeValue = signal + degreeValue;

  minuteValue = te::common::Convert2String(std::floor(min), 0);
  minuteValue = lpadString(minuteValue, 2, '0');

  secondValue = te::common::Convert2String(std::fabs(sec), precision);
  if (precision == 0)
  {
    secondValue = lpadString(secondValue, 2, '0');
  }
  else
  {
    //we must consider the decimal separator
    secondValue = lpadString(secondValue, precision + 3, '0');
  }

  std::string formattedNumber = "";
  if (bDegrees)
  {
    formattedNumber = degreeValue;
    space = ":";
  }
  if (bMinutes)
  {
    formattedNumber += space + minuteValue;
    space = ":";
  }
  if (bSeconds)
  {
    formattedNumber += space + secondValue;
  }

  if (bDegrees == false && bMinutes == false && bSeconds == false)
    formattedNumber = degreeValue + ":" + minuteValue + ":" + secondValue;

  return formattedNumber;
}

std::string te::layout::Utils::lpadString(std::string& text, size_t length, char lpadValue)
{
  std::string output = text;
  while (output.size() < length)
  {
    output = lpadValue + output;
  }

  return output;
}

int te::layout::Utils::roundNumber( const double& value )
{
  if (value >= 0)
    return (int)(value+.5);
  else
    return (int)(value-.5);
}

double te::layout::Utils::convertDegreeToDecimal()
{
  return 0;
}

std::string te::layout::Utils::proj4DescToGeodesic()
{
  // EPSG:4326
  std::string proj4;
  proj4 += "+proj=longlat";
  proj4 += " +datum=WGS84";
  proj4 += " +no_defs ";
  return proj4;
}

te::common::UnitOfMeasurePtr te::layout::Utils::unitMeasure( int srid )
{
  te::common::UnitOfMeasurePtr unitPtr;

  // Checks if is Planar Geographic
  std::string authName = "EPSG"; // Now: So far it is the only one supported by TerraLib 5. Future: Review this line!
  te::srs::SpatialReferenceSystemManager::getInstance().isGeographic(srid, authName);
  unitPtr = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(srid, authName);

  return unitPtr;
}

void te::layout::Utils::remapToPlanar(te::gm::Envelope* latLongBox, int sourceSRID, int planarSRID)
{
  if (!latLongBox->isValid())
    return;

  try
  {
    latLongBox->transform(sourceSRID, planarSRID);
  }
  catch (const te::common::Exception&)
  {
    std::cout << "Could not remap box to planar projection!" << std::endl;
  }
}

void te::layout::Utils::remapToPlanar(te::srs::Converter* converter, te::gm::Envelope* latLongBox, int sourceSRID, int planarSRID)
{
  if (!latLongBox->isValid())
    return;

  double x1 = 0;
  double y1 = 0;
  double x2 = 0;
  double y2 = 0;
  double x3 = 0;
  double y3 = 0;
  double x4 = 0;
  double y4 = 0;

  // convert the four corners
  converter->convert(latLongBox->m_llx, latLongBox->m_lly, x1, y1);
  converter->convert(latLongBox->m_urx, latLongBox->m_lly, x2, y2);
  converter->convert(latLongBox->m_urx, latLongBox->m_ury, x3, y3);
  converter->convert(latLongBox->m_llx, latLongBox->m_ury, x4, y4);

  // evaluate the minimum box that includes all four corner
  latLongBox->m_llx = std::min(std::min(x1, x4), std::min(x2, x3));
  latLongBox->m_urx = std::max(std::max(x1, x4), std::max(x2, x3));
  latLongBox->m_lly = std::min(std::min(y1, y4), std::min(y2, y3));
  latLongBox->m_ury = std::max(std::max(y1, y4), std::max(y2, y3));
}

void te::layout::Utils::remapToPlanar(te::gm::LinearRing* line, int sourceSRID, int planarSRID)
{
  if (!line)
    return;

  /* Optimization so that the convert object is created only once, decreasing the execution time of this method,
  since it will no longer use the Envelope::transform method (for each point). */

  std::unique_ptr<te::srs::Converter> converter(new te::srs::Converter());
  
  try
  {
    converter->setSourceSRID(sourceSRID);
    converter->setTargetSRID(planarSRID);
  }
  catch (te::common::Exception& /* ex */)
  {
    return;
  }

  double x = 0;
  double y = 0;

  // convert the four corners
  std::size_t npoints = line->getNPoints();

  for (std::size_t i = 0; i < npoints; ++i)
  {
    te::gm::Point* p = line->getPointN(i);
    const te::gm::Envelope* env = p->getMBR();
    te::gm::Envelope* en = const_cast<te::gm::Envelope*>(env);
    converter->convert(p->getX(), p->getY(), x, y);
    line->setPoint(i, x, y);
    p->computeMBR(true);
  }
  line->computeMBR(true);
  line->setSRID(planarSRID);
}

void te::layout::Utils::remapToPlanar(te::gm::Point* point, int sourceSRID, int planarSRID)
{
  if (!point)
    return;

  const te::gm::Envelope* env = point->getMBR();
  te::gm::Envelope* en = const_cast<te::gm::Envelope*>(env);
  remapToPlanar(en, sourceSRID, planarSRID);
  point->computeMBR(true);
}

te::gm::Envelope te::layout::Utils::GetWorldBoxInGeographic(const te::gm::Envelope& worldBox, int srid)
{
  te::gm::Envelope worldBoxGeographic = worldBox;

  //About units names (SI): terralib5\resources\json\uom.json 
  te::layout::Utils utils(0);
  te::common::UnitOfMeasurePtr unitPtr = utils.unitMeasure(srid);

  if (!unitPtr)
    return worldBoxGeographic;

  std::string unitPtrStr = unitPtr->getName();
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if (unitPtrStr.compare("DEGREE") != 0)
  {
    std::string proj4 = utils.proj4DescToGeodesic();

    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projGeographic = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4);

    // Remapping 
    worldBoxGeographic.transform(srid, projGeographic.second);
  }

  return worldBoxGeographic;
}

te::gm::Envelope te::layout::Utils::worldBoxTo(const te::gm::Envelope& worldBox, int sourceSRID, int targetSRID)
{
  te::gm::Envelope copyWorldBox = worldBox;

  // Checks if is Planar Geographic
  std::string authName = "EPSG"; // Now: So far it is the only one supported by TerraLib 5. Future: Review this line!
  te::srs::SpatialReferenceSystemManager::getInstance().isGeographic(sourceSRID, authName);
  te::common::UnitOfMeasurePtr unitPtr = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(sourceSRID, authName);

  // check if srid exist and worldbox is valid
  if (!unitPtr || !copyWorldBox.isValid())
  {
    return copyWorldBox;
  }

  // Remapping
  copyWorldBox.transform(sourceSRID, targetSRID);
  return copyWorldBox;
}

void te::layout::Utils::convertToMillimeter( WorldTransformer transf, te::gm::LinearRing* line )
{
  if(!line)
    return;

  std::size_t npoints = line->getNPoints();

  for(std::size_t i = 0 ; i < npoints ; ++i)
  {
    te::gm::Point* p = line->getPointN(i);   
    double x = 0;
    double y = 0;
    transf.system1Tosystem2(p->getX(), p->getY(), x, y);    
    line->setPoint(i, x, y);  
    p->computeMBR(true);
  }

  line->computeMBR(true);
  line->setSRID(TE_UNKNOWN_SRS);
}

void te::layout::Utils::convertToMillimeter( WorldTransformer transf, te::gm::Polygon* poly )
{
  if(!poly)
    return;

  std::size_t nrings = poly->getNumInteriorRings();

  for(std::size_t i = 0 ; i < nrings ; ++i)
  {
    te::gm::LinearRing* line = dynamic_cast<te::gm::LinearRing*>(poly->getInteriorRingN(i));
    if(line)
    {
      convertToMillimeter(transf, line);
    }
  }

  poly->computeMBR(true);
  poly->setSRID(TE_UNKNOWN_SRS);
}

te::layout::Properties te::layout::Utils::convertToProperties(const te::layout::PaperConfig& paperConfig)
{
  double paperWidth = 0.;
  double paperHeight = 0.;
  LayoutAbstractPaperType paperType = paperConfig.getPaperType();
  LayoutOrientationType paperOrientation = paperConfig.getPaperOrientantion();
  paperConfig.getPaperSize(paperWidth, paperHeight);


  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Properties properties;
  {
    Property property(0);
    property.setName("paper_width");
    property.setValue(paperWidth, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_height");
    property.setValue(paperHeight, dataType->getDataTypeDouble());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_type");
    property.setValue<int>((int)paperType, dataType->getDataTypeInt());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_orientation");
    property.setValue<int>((int)paperOrientation, dataType->getDataTypeInt());
    properties.addProperty(property);
  }

  return properties;
}

te::layout::PaperConfig te::layout::Utils::convertToPaperConfig(const te::layout::Properties& properties)
{
  const Property& pPaperHeight = properties.getProperty("paper_height");
  const Property& pPaperWidth = properties.getProperty("paper_width");
  const Property& pPaperType = properties.getProperty("paper_type");
  const Property& pPaperOrientation = properties.getProperty("paper_orientation");

  double paperWidth = te::layout::Property::GetValueAs<double>(pPaperWidth);
  double paperHeight = te::layout::Property::GetValueAs<double>(pPaperHeight);
  LayoutAbstractPaperType paperType = (LayoutAbstractPaperType)te::layout::Property::GetValueAs<int>(pPaperType);
  LayoutOrientationType paperOrientation = (LayoutOrientationType)te::layout::Property::GetValueAs<int>(pPaperOrientation);

  PaperConfig paperConfig;
  paperConfig.setPaperSizeCustom(paperWidth, paperHeight);
  paperConfig.setPaperType(paperType);
  paperConfig.setPaperOrientation(paperOrientation);

  return paperConfig;
}

te::gm::Envelope te::layout::Utils::getWorldBoxInGeographic(const te::gm::Envelope& worldBox, int srid)
{
  te::gm::Envelope worldBoxGeographic = worldBox;

  //About units names (SI): terralib5\resources\json\uom.json 


  te::common::UnitOfMeasurePtr unitPtr = unitMeasure(srid);

  if (!unitPtr)
    return worldBoxGeographic;

  std::string unitPtrStr = unitPtr->getName();
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if (unitPtrStr.compare("DEGREE") != 0)
  {
    std::string proj4 = proj4DescToGeodesic();

    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projGeographic = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4);

    // Remapping 
    worldBoxGeographic.transform(srid, projGeographic.second);
  }

  return worldBoxGeographic;
}

double te::layout::Utils::getLineWidthMinimumValue()
{
  return m_lineWidthMinimumValue;
}

void te::layout::Utils::setLineWidthMinimumValue(double minimumValue)
{
  m_lineWidthMinimumValue = minimumValue;
}

double te::layout::Utils::round(double n, double precision)
{
  return floor(n * pow(10., precision) + .5) / pow(10., precision);
}


double te::layout::Utils::calculateAngle(QPointF p1, QPointF p2)
{
  double angle = std::atan2(p1.y() - p2.y(), p1.x() - p2.x());

  return (angle * 180) / 3.14159265;
}

std::vector<std::string> te::layout::Utils::Tokenize(const std::string& value, const std::string& separator)
{
  std::vector<std::string> vecString;

  //here we need to ensure some additional precision
  typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
  boost::char_separator<char> boostSeparator{ separator.c_str(), "", boost::keep_empty_tokens };
  tokenizer tokenizerStr{ value, boostSeparator };
  for (tokenizer::iterator it = tokenizerStr.begin(); it != tokenizerStr.end(); ++it)
  {
    vecString.push_back(*it);
  }

  return vecString;
}

int te::layout::Utils::toPlanar(const te::gm::Envelope& worldBox, int sourceSRID)
{
  int targetSRID = TE_UNKNOWN_SRS;
  te::layout::Utils utils(0);
  te::common::UnitOfMeasurePtr unitPtr = utils.unitMeasure(sourceSRID);

  if (!unitPtr || !worldBox.isValid())
  {
    return targetSRID;
  }

  targetSRID = sourceSRID;
  std::string unitPtrStr = unitPtr->getName();
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if (unitPtrStr.compare("DEGREE") == 0)
  {
    // Get the id of the projection of destination
    targetSRID = planarSRID(worldBox);
  }
  return targetSRID;
}

int te::layout::Utils::toGeographic(const te::gm::Envelope& worldBox, int sourceSRID)
{
  int targetSRID = TE_UNKNOWN_SRS;
  te::gm::Envelope worldBoxPlanar = worldBox;
  te::layout::Utils utils(0);
  te::common::UnitOfMeasurePtr unitPtr = utils.unitMeasure(sourceSRID);

  if (!unitPtr || !worldBoxPlanar.isValid())
  {
    return targetSRID;
  }

  targetSRID = sourceSRID;
  std::string unitPtrStr = unitPtr->getName();
  unitPtrStr = te::common::Convert2UCase(unitPtrStr);

  if (unitPtrStr.compare("DEGREE") != 0)
  {
    std::string proj4 = utils.proj4DescToGeodesic();
    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projGeographic = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4);
    targetSRID = projGeographic.second;
  }
  return targetSRID;
}

bool te::layout::Utils::isValid(const te::gm::Envelope& box)
{
  double lowLeftX = box.getLowerLeftX();
  double lowLeftY = box.getLowerLeftY();
  double upperRightX = box.getUpperRightX();
  double upperRightY = box.getUpperRightY();
  double w = box.getWidth();
  double h = box.getHeight();

  if (std::isnan(lowLeftX) || std::isnan(lowLeftY) || std::isnan(upperRightX)
    || std::isnan(upperRightY) || std::isnan(w) || std::isnan(h))
  {
    return false;
  }

  if (std::isinf(lowLeftX) || std::isinf(lowLeftY) || std::isinf(upperRightX)
    || std::isinf(upperRightY) || std::isinf(w) || std::isinf(h))
  {
    return false;
  }

  return true;
}

int te::layout::Utils::planarSRID(const te::gm::Envelope& worldBox)
{
  int srid = TE_UNKNOWN_SRS;
  int zone = te::map::CalculatePlanarZone(worldBox); // get zone number
  double hemisphere = worldBox.getCenter().getY(); // lat (y) < 0 is south

  if (hemisphere > 0)
    srid = 32600 + zone; // 32600 - WGS 84 / UTM grid system (northern hemisphere)
  else
    srid = 32700 + zone; // 32700 - WGS 84 / UTM grid system (southern hemisphere)

  return srid;
}
