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

#include "WorldTransformer.h"
#include "terralib/color/RGBAColor.h"
#include "terralib/geometry/Polygon.h"
#include "terralib/geometry/Enums.h"
#include "terralib/geometry/LinearRing.h"
#include "terralib/geometry/Point.h"
#include "terralib/maptools/Utils.h"
#include "terralib/qt/widgets/canvas/Canvas.h"
#include "terralib/srs/SpatialReferenceSystemManager.h"
#include "terralib/common/Translator.h"
#include "../core/ContextObject.h"
#include "enum/AbstractType.h"
#include "pattern/singleton/Context.h"
#include "AbstractScene.h"
#include "PaperConfig.h"
#include "property/Properties.h"
#include "property/Property.h"
#include "../qt/core/Scene.h"
#include "terralib/common/StringUtils.h"

// STL
#include <math.h> 
#include <string>
#include <sstream> 
#include <exception>

double te::layout::Utils::m_lineWidthMinimumValue = 0.3;

te::layout::Utils::Utils(te::layout::Scene* scene, te::qt::widgets::Canvas* canvas) :
  m_applyZoom(true),
  m_scene(scene),
  m_canvas(canvas)
{

}

te::layout::Utils::~Utils()
{

}

void te::layout::Utils::drawRectW( te::gm::Envelope box )
{
  if(!m_canvas)
  {
    return;
  }
      
  te::gm::Polygon* rect = new te::gm::Polygon(1, te::gm::PolygonType);
  
  te::gm::LinearRing* outRingPtr0 = new te::gm::LinearRing(5, te::gm::LineStringType);
  outRingPtr0->setPointN( 0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));
  outRingPtr0->setPointN( 1, te::gm::Point(box.getUpperRightX(), box.getLowerLeftY())); 
  outRingPtr0->setPointN( 2, te::gm::Point(box.getUpperRightX(), box.getUpperRightY()));
  outRingPtr0->setPointN( 3, te::gm::Point(box.getLowerLeftX(), box.getUpperRightY())); 
  outRingPtr0->setPointN( 4, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY())); 

  rect->setRingN(0, outRingPtr0);

  m_canvas->draw(rect);

  if(rect)
  {
    delete rect;
    rect = 0;
  }
}

void te::layout::Utils::drawLineW( te::gm::LinearRing* line )
{
  if(!m_canvas)
  {
    return;
  }
    
  m_canvas->draw(line);
}

te::gm::LinearRing* te::layout::Utils::createSimpleLine( te::gm::Envelope box )
{
  te::gm::LinearRing* line = new te::gm::LinearRing(3, te::gm::LineStringType);
  line->setNumCoordinates(3);

  line->setPointN(0, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));
  line->setPointN(1, te::gm::Point(box.getUpperRightX(), box.getUpperRightY()));
  line->setPointN(2, te::gm::Point(box.getLowerLeftX(), box.getLowerLeftY()));

  return line;
}

te::color::RGBAColor** te::layout::Utils::getImageW( te::gm::Envelope boxmm )
{
  te::color::RGBAColor** pixmap = 0;

  if(!m_canvas)
  {
    return pixmap;
  }

  te::gm::Envelope boxViewport = viewportBox(boxmm);

  if(boxViewport.isValid())
  {
    pixmap = m_canvas->getImage(0, 0, boxViewport.getWidth(), boxViewport.getHeight());
  }
  return pixmap;
}

int te::layout::Utils::mm2pixel( double mm )
{
  if(!m_scene)
  {
    return -1;
  }
  const ContextObject& context = m_scene->getContext();

  int devDpi = context.getDpiX();
  int px = (mm * devDpi) / 25.4 ;
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
  double mm = (pixel / devDpi) * 25.4 ;
  return mm;
}

void te::layout::Utils::configCanvas( te::gm::Envelope box, bool resize, bool applyZoom )
{
  m_applyZoom = applyZoom;
  te::gm::Envelope boxViewport = viewportBox(box);
  changeCanvas(boxViewport, box, resize); 
}

void te::layout::Utils::changeCanvas( te::gm::Envelope viewport, te::gm::Envelope world, bool resize /*= true*/ )
{
  if(!m_canvas)
  {
    return;
  }

  if(resize)
  {
    //Transparent
    te::color::RGBAColor color(255,255,255, 0);
    m_canvas->setBackgroundColor(color);

    m_canvas->resize(viewport.getWidth(), viewport.getHeight());
  }

  m_canvas->setWindow(world.getLowerLeftX(), world.getLowerLeftY(), 
    world.getUpperRightX(), world.getUpperRightY()); 
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

void te::layout::Utils::textBoundingBox( double &w, double &h, std::string txt )
{
  if(!m_canvas)
  {
    return;
  }

  w = 0;
  h = 0;

  te::gm::Polygon* poly = m_canvas->getTextBoundary(0, 0, txt, 0);
  if(poly)
  {
    //Box = mbr: minimum bounding rectangle
    const te::gm::Envelope* env = poly->getMBR();
    te::gm::Envelope* box = 0;
    box = const_cast<te::gm::Envelope*>(env);
    if(box)
    {
      w = box->getWidth();
      h = box->getHeight();
    }
  }
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
  std::string proj4;
  proj4 += "+proj=longlat";
  proj4 += " +ellps=aust_SA";
  proj4 += " +towgs84=-57,1,-41,0,0,0,0";
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

void te::layout::Utils::remapToPlanar( te::gm::Envelope* latLongBox, int zone )
{
  if(!latLongBox->isValid())
    return;
  
  try
  {
    std::string proj4 = te::map::GetUTMProj4FromZone(zone);

    // Get the id of the projection of destination 
    std::pair<std::string, unsigned int> projMeters = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4); 

    std::string proj4geo = proj4DescToGeodesic();

    // Get the id of the projection source 
    std::pair<std::string, unsigned int> currentBoxProj = te::srs::SpatialReferenceSystemManager::getInstance().getIdFromP4Txt(proj4geo); 

    // Remapping 
    int srid = currentBoxProj.second;
    latLongBox->transform(srid, projMeters.second); 
  }
  catch(const te::common::Exception&)
  {
    zone = -1;
  }
}

void te::layout::Utils::remapToPlanar( te::gm::LinearRing* line, int zone )
{
  if(!line)
    return;

  std::size_t npoints = line->getNPoints();

  for(std::size_t i = 0 ; i < npoints ; ++i)
  {
    te::gm::Point* p = line->getPointN(i);
    const te::gm::Envelope* env = p->getMBR();
    te::gm::Envelope* en = const_cast<te::gm::Envelope*>(env);
    remapToPlanar(en, zone);
    line->setPoint(i, env->getLowerLeftX(), env->getLowerLeftY());
    p->computeMBR(true);
  }
  line->computeMBR(true);
}

void te::layout::Utils::remapToPlanar( te::gm::Point* point, int zone )
{
  if(!point)
    return;

  const te::gm::Envelope* env = point->getMBR();
  te::gm::Envelope* en = const_cast<te::gm::Envelope*>(env);
  remapToPlanar(en, zone);
  point->computeMBR(true);
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
}

char* te::layout::Utils::imageToChar( std::string fileName, std::ifstream::pos_type &size )
{
  char* memblock = 0;

  if(fileName.compare("") == 0)
    return memblock;

  try 
  { 
    std::ifstream file (fileName.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
      size = file.tellg();
      memblock = new char[size]; 
      file.seekg (0, std::ios::beg);
      file.read((char*)memblock, size); // cast to a char* to give to file.read

      file.close();
    }
  }
  catch (std::ifstream::failure &e) 
  {
    std::cerr << e.what() << std::endl;
    std::string errmsg = "Exception opening/reading/closing file: \n ";
    te::common::Exception ex(TE_TR(errmsg));
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
  return memblock;
}

std::string te::layout::Utils::getFileExtension( std::string fileName )
{
  std::string extension = fileName.substr(fileName.find_last_of("/\\.") + 1);
  return extension;
}

void te::layout::Utils::setApplyZoom( bool apply )
{
  m_applyZoom = apply;
}

bool te::layout::Utils::getApplyZoom()
{
  return m_applyZoom;
}

void te::layout::Utils::resetCanvas()
{
  if(!m_canvas)
  {
    return;
  }

  int size = 1;

  m_canvas->clear();
  m_canvas->setLineWidth(size);
  m_canvas->setPointWidth(size);
  m_canvas->setPolygonContourWidth(size);
  m_canvas->setPolygonPatternWidth(size);
  m_canvas->setTextContourWidth(size);
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
    property.setValue((int)paperType, dataType->getDataTypeInt());
    properties.addProperty(property);
  }
  {
    Property property(0);
    property.setName("paper_orientation");
    property.setValue((int)paperOrientation, dataType->getDataTypeInt());
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

  double paperWidth = pPaperWidth.getValue().toDouble();
  double paperHeight = pPaperHeight.getValue().toDouble();
  LayoutAbstractPaperType paperType = (LayoutAbstractPaperType)pPaperType.getValue().toInt();
  LayoutOrientationType paperOrientation = (LayoutOrientationType)pPaperOrientation.getValue().toInt();

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

