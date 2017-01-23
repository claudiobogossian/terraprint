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
  \file ItemUtils.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "ItemUtils.h"
#include "terralib/maptools/ExternalGraphicRendererManager.h"
#include "terralib/maptools/MarkRendererManager.h"
#include "terralib/maptools/Utils.h"
#include "terralib/se/PointSymbolizer.h"
#include "terralib/se/Graphic.h"
#include "terralib/se/Utils.h" 
#include "terralib/srs/SpatialReferenceSystemManager.h"
#include "../../core/pattern/mvc/AbstractItemView.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../core/pattern/singleton/Context.h"
#include "../../core/enum/Enums.h"
#include "../item/MapItem.h"
#include "../item/LegendItem.h"
#include "../../item/LegendModel.h"
#include "../../item/MapModel.h"
#include "../item/TextItem.h"
#include "../../item/TextModel.h"
#include "../item/LegendChildItem.h"
#include "../../item/LegendChildModel.h"
#include "Scene.h"
#include "View.h"
#include "../item/PaperItem.h"
#include "terralib/common/StringUtils.h"

// STL
#include <stddef.h>  // defines NULL
#include <memory>

// Qt
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QTextDocument>
#include <QFont>

double te::layout::ItemUtils::m_textDPI = 72.;

te::layout::ItemUtils::ItemUtils( QGraphicsScene* scene ) :
  m_scene(scene)
{

}

te::layout::ItemUtils::~ItemUtils()
{

}

std::vector<te::layout::MapItem*> te::layout::ItemUtils::getMapItemList(bool selected)
{
  std::vector<te::layout::MapItem*> list;

  QList<QGraphicsItem*> graphicsItems = getItems(selected);
  foreach(QGraphicsItem *item, graphicsItems)
  {
    if (!item)
      continue;

    te::layout::MapItem* mit = dynamic_cast<te::layout::MapItem*>(item);
    if (!mit)
      continue;

    list.push_back(mit);
  }

  return list;
}

te::layout::MapItem* te::layout::ItemUtils::getMapItem( std::string name )
{
  te::layout::MapItem* map = 0;

  te::layout::EnumObjectType* objectType = te::layout::Enums::getInstance().getEnumObjectType();

  std::vector<std::string> strList;

  QList<QGraphicsItem*> graphicsItems = getItems(false);
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::AbstractItemView* itemView = dynamic_cast<te::layout::AbstractItemView*>(item);
    if(itemView == 0)
      continue;

    te::layout::AbstractItemController* controller = itemView->getController();

    if(controller == 0)
      continue;

    if(controller->getProperties().getTypeObj() != objectType->getMapItem())
    {
      continue;
    }

    const Property& pName = controller->getProperty("name");
    if(te::layout::Property::GetValueAs<std::string>(pName).compare(name) != 0)
      continue;

    map = dynamic_cast<te::layout::MapItem*>(itemView);
    break;
  }

  return map;
}

std::vector<std::string> te::layout::ItemUtils::mapNameList(bool selected)
{
  te::layout::EnumObjectType* objectType = te::layout::Enums::getInstance().getEnumObjectType();

  std::vector<std::string> strList;

  QList<QGraphicsItem*> graphicsItems = getItems(selected);
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::AbstractItemView* itemView = dynamic_cast<te::layout::AbstractItemView*>(item);
    if(itemView == 0)
      continue;

    te::layout::AbstractItemController* controller = itemView->getController();

    if(controller == 0)
      continue;

    if(controller->getProperties().getTypeObj() != objectType->getMapItem())
    {
      continue;
    }

    const Property& pName = controller->getProperty("name");
    strList.push_back(te::layout::Property::GetValueAs<std::string>(pName));
  }

  return strList;
}

int te::layout::ItemUtils::countType( te::layout::EnumType* type )
{
  int count = 0;

  QList<QGraphicsItem*> graphicsItems = getItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::AbstractItemView* absItem = dynamic_cast<te::layout::AbstractItemView*>(item);
    if(absItem == 0)
      continue;

    te::layout::AbstractItemController* controller = absItem->getController();
    if(controller == 0)
      continue;

    if(controller->getProperties().getTypeObj() == type)
    {
      count+=1;
    }
  }

  return count;
}

int te::layout::ItemUtils::maxTypeId( te::layout::EnumType* type )
{
  int id = -1;

  QList<QGraphicsItem*> graphicsItems = getItems();
  foreach( QGraphicsItem *item, graphicsItems) 
  {
    if(!item)
      continue;

    te::layout::AbstractItemView* absItem = dynamic_cast<te::layout::AbstractItemView*>(item);
    if(absItem == 0)
      continue;

    te::layout::AbstractItemController* controller = absItem->getController();
    if(controller == 0)
      continue;

    int currentId = te::layout::Property::GetValueAs<int>(controller->getProperty("id"));

    if(controller->getProperties().getTypeObj() == type)
    {
      if(id == -1)
      {
        id = currentId;
      }
      else
      {
        if(currentId > id)
        {
          id = currentId;
        }
      }
    }
  }

  return id;
}

QList<QGraphicsItem*> te::layout::ItemUtils::getItems( bool selected )
{
  QList<QGraphicsItem*> graphicsItems;

  if(selected)
  {
    graphicsItems = m_scene->selectedItems();
  }
  else
  {
    graphicsItems = m_scene->items();
  }

  return graphicsItems;
}

QGraphicsItem* te::layout::ItemUtils::intersectionSelectionItem( int x, int y )
{
  QGraphicsItem* intersectionItem = 0;

  if(!m_scene)
  {
    return intersectionItem;
  }

  QList<QGraphicsItem*> items = m_scene->selectedItems();

  QPointF ptScene(x, y);

  foreach (QGraphicsItem *item, items) 
  {
    if(item)
    {
      QPointF ptLocal = item->mapFromScene(ptScene);
      if (item->contains(ptLocal) == true)
      {
        intersectionItem = item;
        break;
      }
    }
  }

  return intersectionItem;
}

QGraphicsItem* te::layout::ItemUtils::intersectionOnlyPaperItem(int x, int y)
{
  QGraphicsItem* intersectionItem = 0;

  if (!m_scene)
  {
    return intersectionItem;
  }

  QList<QGraphicsItem*> items = m_scene->items();

  QPointF ptScene(x, y);

  bool intersectOnlyPaper = true;

  foreach(QGraphicsItem *item, items)
  {
    if (item)
    {
      QPointF ptLocal = item->mapFromScene(ptScene);
      if (item->contains(ptLocal) == true)
      {
        PaperItem* paper = dynamic_cast<PaperItem*>(item);
        if (paper)
        {
          intersectionItem = item;
        }
        else
        {
          intersectionItem = 0;
          break;
        }
      }
    }
  }

  return intersectionItem;
}

QRectF te::layout::ItemUtils::getTextBoundary( const std::string& fontName, int fontSize, const std::string& text ) const
{
  if(m_scene == 0)
  {
    return QRectF();
  }

  Utils utils = ((Scene*) m_scene)->getUtils();

  const ContextObject& context = ((Scene *) m_scene)->getContext();

  double correctionFactorY = context.getDpiY() / 72.;

  QFont font(fontName.c_str());
  font.setPixelSize(fontSize * correctionFactorY);

  QFontMetrics fontMetrics(font);
  QRect rect = fontMetrics.boundingRect(text.c_str());
  int width = rect.width();
  int height = rect.height();
  int descend = fontMetrics.descent();

  double widthMM = utils.pixel2mm(width);
  double heightMM = utils.pixel2mm(height);
  double descendMM = utils.pixel2mm(descend);

   QRectF textBoundingRect(0, -descendMM, widthMM, heightMM);
   return textBoundingRect;
}

QRectF te::layout::ItemUtils::getMinimumTextBoundary(const std::string& fontName, int fontSize, const std::string& text) const
{
  if (m_scene == 0)
  {
    return QRectF();
  }

  Utils utils = ((Scene*) m_scene)->getUtils();

  const ContextObject& context = ((Scene*) m_scene)->getContext();

  double correctionFactorY = context.getDpiY() / 72.;

  QFont font(fontName.c_str());
  font.setPixelSize(fontSize * correctionFactorY);

  QFontMetrics fontMetrics(font);
  QRect rect = fontMetrics.tightBoundingRect(text.c_str());
  int width = rect.width();
  int height = rect.height();
  int descend = fontMetrics.descent();

  double widthMM = utils.pixel2mm(width);
  double heightMM = utils.pixel2mm(height);
  double descendMM = utils.pixel2mm(descend);

  QRectF textBoundingRect(0, -descendMM, widthMM, heightMM);
  return textBoundingRect;
}

QPainterPath te::layout::ItemUtils::superscriptTextToVector(std::vector<QString> text, std::vector<QFont> font, const QPointF& referencePoint, double rotationAngle){

  double textDPI = ItemUtils::getTextDPI();

  //we first calculate the correction factor to be use in the case that the given DPI is not 72.
  double correctionFactor = textDPI / 72.;

  //as we are in a CS based in millimeters, we calculate the relation between the millimeters CS and the device CS
  double ptSizeInches = 1. / 72.; //The size of 1 point, in inches
  double ptSizeMM = ptSizeInches * 25.4; //The size of 1 point, in millimeters
  double scale = ptSizeMM * (1. / correctionFactor); //for the calculation of the scale, we consider the pixel size in millimeters and the inverse of the correction factor
  
  QPainterPath painterPath;

  double x = 0.0;
  double y = 0.0;


  for (int i = 0; i < text.size(); i++ ){

    QFont fontCopy(font.at(i));
    if (font.at(i).pixelSize() > 0)
    {
      fontCopy.setPixelSize(font.at(i).pixelSize() * correctionFactor);
    }
    if (font.at(i).pointSize() > 0)
    {
      fontCopy.setPointSize(font.at(i).pointSize());
    }
    if (font.at(i).pointSizeF() > 0)
    {
      fontCopy.setPointSizeF(font.at(i).pointSizeF());
    }

    //Now we vectorize the text by addind it to a painter path

    painterPath.addText(x, y, fontCopy, text.at(i));

    x = x + painterPath.boundingRect().width();
    y = y + painterPath.boundingRect().height();

  }


  //Then we apply the [optional] rotation
  if (rotationAngle != 0)
  {
    double textWidth = painterPath.boundingRect().width();
    double textHeight = painterPath.boundingRect().height();

    QTransform transform;
    transform.rotate(-rotationAngle);

    painterPath = transform.map(painterPath);
    painterPath.translate(-painterPath.boundingRect().x(), -painterPath.boundingRect().y());
  }

  //and scale, so the vectorized text size can be compatible to the device
  QTransform transform;
  transform.scale(scale, -scale); //here we scale the item so the text size to be considered will be points and not millimeters

  //after the transformation is set, we apply it to the painter path
  painterPath = transform.map(painterPath);
  painterPath.translate(-painterPath.boundingRect().x(), -painterPath.boundingRect().y());


  QRectF currentRect = painterPath.boundingRect();

  
  //we [optionally] translate the painter path to the given reference coordinate
  painterPath.translate(referencePoint);


  //and return it
  return painterPath;

}

QPainterPath te::layout::ItemUtils::textToVector(const QString& text, const QFont& font, const QPointF& referencePoint, double rotationAngle)
{
  double textDPI = ItemUtils::getTextDPI();

  //we first calculate the correction factor to be use in the case that the given DPI is not 72.
  double correctionFactor = textDPI / 72.;
  
  //as we are in a CS based in millimeters, we calculate the relation between the millimeters CS and the device CS
  double ptSizeInches = 1. / 72.; //The size of 1 point, in inches
  double ptSizeMM = ptSizeInches * 25.4; //The size of 1 point, in millimeters
  double scale = ptSizeMM * (1. / correctionFactor); //for the calculation of the scale, we consider the pixel size in millimeters and the inverse of the correction factor

  QFont fontCopy(font);
  if (font.pixelSize() > 0)
  {
    fontCopy.setPixelSize(font.pixelSize() * correctionFactor);
  }
  if (font.pointSize() > 0)
  {
    fontCopy.setPointSize(font.pointSize());
  }
  if (font.pointSizeF() > 0)
  {
    fontCopy.setPointSizeF(font.pointSizeF());
  }

  //Now we vectorize the text by addind it to a painter path
  QPainterPath painterPath;
  painterPath.addText(0, 0, fontCopy, text);

  //Then we apply the [optional] rotation
  if(rotationAngle != 0)
  {
    double textWidth = painterPath.boundingRect().width();
    double textHeight = painterPath.boundingRect().height();

    QTransform transform;
    transform.rotate(-rotationAngle);

    painterPath = transform.map(painterPath);
    painterPath.translate(-painterPath.boundingRect().x(), -painterPath.boundingRect().y());
  }

  //and scale, so the vectorized text size can be compatible to the device
  QTransform transform;
  transform.scale(scale, -scale); //here we scale the item so the text size to be considered will be points and not millimeters

  //after the transformation is set, we apply it to the painter path
  painterPath = transform.map(painterPath);
  painterPath.translate(-painterPath.boundingRect().x(), -painterPath.boundingRect().y());

  QRectF currentRect = painterPath.boundingRect();
 
  //we [optionally] translate the painter path to the given reference coordinate
  painterPath.translate(referencePoint);

  //and return it
  return painterPath;
}

te::layout::AbstractItemView* te::layout::ItemUtils::getSelectedItem()
{
  AbstractItemView* abstractItem = 0;

  if (m_scene == 0)
  {
    return abstractItem;
  }

  QList<QGraphicsItem*> items = m_scene->items();
  //QGraphicsItem* item = items.first();

  foreach(QGraphicsItem *item, items)
  {

    if (!item)
    {
      return abstractItem;
    }

    if (item->isSelected())
    {
      abstractItem = dynamic_cast<AbstractItemView*>(item);
      return abstractItem;
    }
  }
  
  
  return abstractItem;
}

void te::layout::ItemUtils::ConfigurePainterForTexts(QPainter* painter, Font txtFont)
{
  QFont fontCopy = convertToQfont(txtFont);
  painter->setFont(fontCopy);
}

QFont  te::layout::ItemUtils::convertToQfont(Font txtFont){

  //converts information about the font
  QFont qft;
  qft.setFamily(txtFont.getFamily().c_str());
  qft.setBold(txtFont.isBold());
  qft.setItalic(txtFont.isItalic());
  qft.setKerning(txtFont.isKerning());
  qft.setStrikeOut(txtFont.isStrikeout());
  qft.setUnderline(txtFont.isUnderline());

  //we need to set the font size from PT to MM

  qft.setPointSizeF(txtFont.getPointSize());

  return qft;
}

double te::layout::ItemUtils::getTextDPI()
{
  return m_textDPI;
}

void te::layout::ItemUtils::setTextDPI(double textDpi)
{
  m_textDPI = textDpi;
}

te::color::RGBAColor** te::layout::ItemUtils::changePointMarkSize(te::se::PointSymbolizer* pointSymbol, std::size_t width, std::size_t height)
{
  te::color::RGBAColor** rgba = 0;

  if (!pointSymbol)
  {
    return rgba;
  }

  const te::se::Graphic* graphic = pointSymbol->getGraphic();    
  const std::vector<te::se::ExternalGraphic*> exgs = graphic->getExternalGraphics();

  // Change the graphic size
  if (!exgs.empty())
    rgba = te::map::ExternalGraphicRendererManager::getInstance().render(exgs[0], height, width);
  else
  {
    const std::vector<te::se::Mark*> marks = graphic->getMarks();
    if (!marks.empty())
      rgba = te::map::MarkRendererManager::getInstance().render(marks[0], height);
  }
  return rgba;
}

QPainterPath te::layout::ItemUtils::lineToQPath(const te::gm::LineString& line)
{
  QPainterPath path;
  
  std::auto_ptr<te::gm::Point> point(line.getPointN(0));
  path.moveTo(point->getX(), point->getY());

  size_t numCoords = line.size();
    for (size_t j = 1; j < numCoords; ++j)
    {
      std::auto_ptr<te::gm::Point> point(line.getPointN(j));
      path.lineTo(point->getX(), point->getY());
    }

  return path;
}

std::string te::layout::ItemUtils::convert2StdString(const QString& qString)
{
  std::string output(qString.toLatin1().constData());
  return output;
}

QString te::layout::ItemUtils::convert2QString(const std::string& stdString)
{
  return QString::fromLatin1(stdString.c_str());
}

void te::layout::ItemUtils::addOrUpdateProperty(const Property& property, Properties& properties)
{
  if (properties.contains(property.getName()) == true)
  {
    properties.updateProperty(property);
  }
  else
  {
    properties.addProperty(property);
  }
}

void te::layout::ItemUtils::calculateAspectRatio(double widthMM, double heightMM, double& llx, double& lly, double& urx, double& ury)
{
  double ww = urx - llx;
  double wh = ury - lly;
  double centerX = llx + (ww / 2.0);
  double centerY = lly + (wh / 2.0);

  double widthByHeight = widthMM / heightMM;

  if (widthByHeight > ww / wh)
  {
    double v = ww;

    ww = wh * widthByHeight;

    llx = centerX - (ww / 2.0);
    urx = centerX + (ww / 2.0);
  }
  else
  {
    double v = wh;

    wh = ww / widthByHeight;

    lly = centerY - (wh / 2.0);
    ury = centerY + (wh / 2.0);
  }
}

void te::layout::ItemUtils::calculateAspectRatio(double widthMM, double heightMM, te::gm::Envelope& worldBox)
{
  calculateAspectRatio(widthMM, heightMM, worldBox.m_llx, worldBox.m_lly, worldBox.m_urx, worldBox.m_ury);
}

double te::layout::ItemUtils::calculateScaleFromBox(double widthMM, double heightMM, int srid, const te::gm::Envelope& worldBox)
{
  double wMM = widthMM;

  te::common::UnitOfMeasurePtr unitPtr = te::srs::SpatialReferenceSystemManager::getInstance().getUnit(srid);

  te::gm::Envelope envelope = worldBox;

  double wdx = envelope.getWidth();
  double wdy = envelope.getHeight();

  double scale = 1.;
  if (unitPtr != NULL)
  {
    std::string unit = unitPtr->getName();

    if (unit == "DEGREE")
    {
      //here we do a simple but not precise unit convertion
      wdx = envelope.getWidth() * 111133.;
      wdy = envelope.getHeight() * 111133.;

      unit = "METRE";
    }

    double dx = widthMM / wdx;
    double dy = heightMM / wdy;
    double f = (dx > dy) ? dx : dy;

    double wT = wMM;

    if (unit == "METRE")
      wT = wMM / 1000.;
    else if (unit == "KILOMETRE")
      wT = wMM / 1000000.;
    else if (unit == "FOOT")
      wT = wMM / (12. * 25.4);

    double wp = wT / widthMM;
    scale = (1. / f) / wp;
  }

  return scale;
}

te::gm::Envelope te::layout::ItemUtils::calculateBoxFromScale(double widthMM, double heightMM, int srid, const te::gm::Envelope& worldBox, double scale)
{
  double worldWidth = worldBox.getWidth();
  double worldHeight = worldBox.getHeight();
  double centerX = worldBox.getCenter().getX();
  double centerY = worldBox.getCenter().getY();

  double currentScale = calculateScaleFromBox(widthMM, heightMM, srid, worldBox);
  double factor = scale / currentScale;

  double llx = centerX - (worldWidth / 2.0 * factor);
  double lly = centerY - (worldHeight / 2.0 * factor);
  double urx = centerX + (worldWidth / 2.0 * factor);
  double ury = centerY + (worldHeight / 2.0 * factor);

  te::gm::Envelope newWorld(llx, lly, urx, ury);
  return newWorld;
}

te::gm::Envelope te::layout::ItemUtils::calculateZoom(const te::gm::Envelope& envelope, double factor, bool zoomIn, const te::gm::Point& pointToCenter)
{
  if (envelope.isValid() == false)
    return envelope;

  // Adjusting zoom factor based on zoomType
  if (zoomIn == true)
    factor = 1 / factor;

  // If point is not null, the zoom extent will be centered on this point. Otherwise, keep the current center.
  te::gm::Envelope extent;
  if (pointToCenter.isValid() == true)
  {
    te::gm::Coord2D reference = te::gm::Coord2D(pointToCenter.getX(), pointToCenter.getY());

    double dxPercentage = (reference.getX() - envelope.getLowerLeftX()) / envelope.getWidth();
    double dyPercentage = (reference.getY() - envelope.getLowerLeftY()) / envelope.getHeight();

    // Bulding the zoom extent based on zoom factor value and the given point
    double w = envelope.getWidth() * factor;
    double h = envelope.getHeight() * factor;

    double leftWidth = w * dxPercentage;
    double rightWidth = w * (1. - dxPercentage);

    double lowerHeight = h * dyPercentage;
    double upperHeight = h * (1. - dyPercentage);

    extent.init(reference.x - leftWidth, reference.y - lowerHeight, reference.x + rightWidth, reference.y + upperHeight);
  }
  else
  {
    te::gm::Coord2D reference = envelope.getCenter();

    // Bulding the zoom extent based on zoom factor value and the given point
    double w = envelope.getWidth() * factor * 0.5;
    double h = envelope.getHeight() * factor * 0.5;

    extent.init(reference.x - w, reference.y - h, reference.x + w, reference.y + h);
  }

  return extent;
}

te::gm::Envelope te::layout::ItemUtils::calculatePan(const te::gm::Envelope& envelope, double dx, double dy)
{
  if (envelope.isValid() == false)
    return envelope;

  te::gm::Coord2D oldCenter = envelope.getCenter();
  double x = oldCenter.getX() - dx;
  double y = oldCenter.getY() - dy;

  double halfWidth = envelope.getWidth() / 2.;
  double halfHeight = envelope.getHeight() / 2.;

  te::gm::Envelope extent(x - halfWidth, y - halfHeight, x + halfWidth, y + halfHeight);
  return extent;
}


void te::layout::ItemUtils::DD2DMS(QString dd, QString& degreeString, QString& minuteString, QString& secondString)
{
  int degree;
  int minute;
  double second;
  double ll;

  double value = dd.replace(QString(" "), QString("")).toDouble();

  degree = (int)value;
  ll = value - degree;
  minute = (int)(ll * 60.);
  ll = (ll * 60.) - minute;
  second = ll * 60.;

  if (minute < 0)
  {
	  minute = minute * (-1);
  }


  if (second < 0)
  {
	  second = second * (-1);
  }


  degreeString = ItemUtils::convert2QString(boost::lexical_cast<std::string>(degree));
  minuteString = ItemUtils::convert2QString(boost::lexical_cast<std::string>(minute));
  secondString = ItemUtils::convert2QString(boost::lexical_cast<std::string>(second));


}

void te::layout::ItemUtils::DMS2DD(QString degree, QString minute, QString second, QString &dd)
{
  bool negativeValue = false;

  double deg = degree.toDouble();
  double min = minute.toDouble();
  double sec = second.toDouble();

  if (deg < 0)
  {
    negativeValue = true;
    deg = deg * (-1);
  }

  double coord = deg + (min / 60.0) + (sec / 3600.0);

  if (negativeValue == true)
  {
    coord = coord * (-1);
  }

  dd = ItemUtils::convert2QString(boost::lexical_cast<std::string>(coord));

}



QString te::layout::ItemUtils::DD2DMS(QString dd)
{
  int degree;
  int minute;
  double second;
  double ll;

  double value = dd.replace(QString(" "), QString("")).toDouble();

  degree = (int)value;
  ll = value - degree;
  minute = (int)(ll * 60.);
  ll = (ll * 60.) - minute;
  second = ll * 60.;

  QString output;
  if (degree < 0)
  {
    output = convert2QString(("-" + te::common::Convert2String(abs(degree)) + "°" + te::common::Convert2String(abs(minute)) + "'" + te::common::Convert2String(fabs(second), 3) + "''").c_str());
  }
  else
  {
    output = convert2QString((te::common::Convert2String(abs(degree)) + "°" + te::common::Convert2String(abs(minute)) + "'" + te::common::Convert2String(fabs(second), 3) + "''").c_str());
  }

  return output;
}

QString te::layout::ItemUtils::DMS2DD(const QString dms)
{
  int pos = 0;
  if (dms.startsWith('+') || dms.startsWith('-'))
  {
    pos = 1;
  }

  std::string str = convert2StdString(dms);

  QChar degreeSymbol('°');
  QChar minuteSymbol('\'');
  QChar secondsSymbol('\'');

  int a = dms.indexOf(degreeSymbol);
  int b = dms.indexOf(minuteSymbol);
  int c = dms.indexOf(secondsSymbol, b + 1);
  double deg = dms.mid(pos, a - pos).replace(QString(" "), QString("")).toDouble();
  double min = dms.mid(a + 1, b - a - 1).replace(QString(" "), QString("")).toDouble();
  double sec = dms.mid(b + 1, c - b - 1).replace(QString(" "), QString("")).toDouble();
  double coord = deg + (min / 60.0) + (sec / 3600.0);
  if (dms.mid(0, a).toDouble() < 0)
  {
    coord *= -1;
  }

  std::string output = te::common::Convert2String(coord, 12);
  QString qValue = convert2QString(output);

  return qValue;
}

void te::layout::ItemUtils::normalizeItem(QGraphicsItem* qItem)
{
  AbstractItemView* item = dynamic_cast<AbstractItemView*>(qItem);
  if (item == 0)
  {
    return;
  }

  Properties parentProperties;
  
  //we must first recalculate the size of the parent item
  //QPointF parentPos = qItem->pos();
  double x = Property::GetValueAs<double>(item->getProperty("x"));
  double y = Property::GetValueAs<double>(item->getProperty("y"));
  QPointF parentPos(x, y);
  QPointF parentPosAAAA = qItem->pos();

  QRectF parentRect = qItem->childrenBoundingRect();
  parentRect = qItem->mapToParent(parentRect).boundingRect();

  //then we check if the size need to be updated
  double width = Property::GetValueAs<double>(item->getProperty("width"));
  if (parentRect.width() != width)
  {
    Property property;
    property.setName("width");
    property.setValue(parentRect.width(), Enums::getInstance().getEnumDataType()->getDataTypeDouble());
    property.setParent(Enums::getInstance().getEnumObjectType()->getMapCompositionItem()->getName());
    parentProperties.addProperty(property);
  }

  double height = Property::GetValueAs<double>(item->getProperty("height"));
  if (parentRect.height() != height)
  {
    Property property;
    property.setName("height");
    property.setValue(parentRect.height(), Enums::getInstance().getEnumDataType()->getDataTypeDouble());
    property.setParent(Enums::getInstance().getEnumObjectType()->getMapCompositionItem()->getName());
    parentProperties.addProperty(property);
  }

  double dx = parentRect.x() - parentPos.x();
  if (dx != 0.)
  {
    Property property;
    property.setName("x");
    property.setValue(parentRect.x(), Enums::getInstance().getEnumDataType()->getDataTypeDouble());
    property.setParent(Enums::getInstance().getEnumObjectType()->getMapCompositionItem()->getName());
    parentProperties.addProperty(property);
  }

  double dy = parentRect.y() - parentPos.y();
  if (dy != 0.)
  {
    Property property;
    property.setName("y");
    property.setValue(parentRect.y(), Enums::getInstance().getEnumDataType()->getDataTypeDouble());
    property.setParent(Enums::getInstance().getEnumObjectType()->getMapCompositionItem()->getName());
    parentProperties.addProperty(property);
  }

  if (dx != 0. || dy != 0.)
  {
    QList<QGraphicsItem*> qChildrenList = qItem->childItems();
    QList<QGraphicsItem*>::iterator itChild = qChildrenList.begin();
    while (itChild != qChildrenList.end())
    {
      QGraphicsItem* qChild = *itChild;
      QPointF oldChildPos = qChild->pos();

      AbstractItemView* child = dynamic_cast<AbstractItemView*>(qChild);
      child->prepareGeometryChange();

      qChild->moveBy(-dx, -dy);

      ++itChild;
    }
  }

  if(parentProperties.getProperties().empty() == false)
  {
    item->prepareGeometryChange();
    item->setProperties(parentProperties);
  }
}

void te::layout::ItemUtils::normalizeChildrenPosition(QGraphicsItem* item)
{
  QRectF oldRectItem = item->childrenBoundingRect();

  //first, we check if the bounding rect of the item has a displacement
  double dx = oldRectItem.x();
  double dy = oldRectItem.y();

  if (dx == 0. && dy == 0.)
  {
    return;
  }

  //if so, we must move the item and all its children in order to make the bounding rect of the item be placed in 0,0

  QPointF oldPos = item->pos();
  item->moveBy(dx, dy);

  QList<QGraphicsItem*> qChildrenList = item->childItems();
  QList<QGraphicsItem*>::iterator itChild = qChildrenList.begin();
  while (itChild != qChildrenList.end())
  {
    QGraphicsItem* qChild = *itChild;
    QPointF oldChildPos = qChild->pos();

    qChild->moveBy(-dx, -dy);

    ++itChild;
  }
}

void te::layout::ItemUtils::normalizeChildrenPosition(QRectF& parentRect, QList<QRectF>& childrenRectList)
{
  //first, we check if the bounding rect of the item has a displacement
  double dx = parentRect.x();
  double dy = parentRect.y();

  if (dx == 0. && dy == 0.)
  {
    return;
  }

  //if so, we must move the item and all its children in order to make the bounding rect of the item be placed in 0,0
  //parentRect.translate(dx, dy);

  QList<QRectF>::iterator itChild = childrenRectList.begin();
  while (itChild != childrenRectList.end())
  {
    QRectF currentBoundinfRect = *itChild;
    itChild->translate(-dx, -dy);
    ++itChild;
  }
}

void te::layout::ItemUtils::drawImage(const QRectF& rectMM, QPainter* painter, const QImage& image)
{
  painter->save();

  QTransform transform;
  transform.translate(0., rectMM.y() + rectMM.height());
  transform.scale(1., -1.);
  transform.translate(0., -rectMM.y());

  painter->setTransform(transform, true);
  painter->drawImage(rectMM, image, image.rect());

  painter->restore();
}

void te::layout::ItemUtils::drawPixmap(const QRectF& rectMM, QPainter* painter, const QPixmap& pixmap)
{
  painter->save();

  QTransform transform;
  transform.translate(0., rectMM.y() + rectMM.height());
  transform.scale(1., -1.);
  transform.translate(0., -rectMM.y());

  painter->setTransform(transform, true);
  painter->drawPixmap(rectMM, pixmap, pixmap.rect());

  painter->restore();
}

void te::layout::ItemUtils::drawText(const QPointF& pointMM, QPainter* painter, const QFont& font, const std::string& text, int rotate)
{
  QString qText = ItemUtils::convert2QString(text);

  QPainterPath textObject = ItemUtils::textToVector(qText, font, pointMM, rotate);

  QPen pen = painter->pen();
  pen.setWidthF(0);

  QBrush brush = painter->brush();
  brush.setStyle(Qt::SolidPattern);

  painter->save();

  painter->setPen(pen);
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->fillPath(textObject, brush);

  painter->restore();
}

void te::layout::ItemUtils::drawSuperScriptText(const QPointF& pointMM, QPainter* painter, const QFont& font, const std::string& text, int rotate)
{
  std::vector<QString> textVect;
  std::vector<QFont> fontVect;

  QFont fontSScript2 = font;
  fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

  fontVect.push_back(fontSScript2);
  fontVect.push_back(font);

  int index = (int)text.size() / 2;
  int indexNegative = (int)(text.size() - 1) / 2;

  std::string txtSubstr1 = text.substr(0, indexNegative + 1).c_str();
  QString qTxtSubstr1 = ItemUtils::convert2QString(txtSubstr1);

  std::string txtSubstr2 = text.substr(0, index).c_str();
  QString qTxtSubstr2 = ItemUtils::convert2QString(txtSubstr2);

  std::string txtSubstr3 = text.substr(indexNegative + 1, text.size()).c_str();
  QString qTxtSubstr3 = ItemUtils::convert2QString(txtSubstr3);

  std::string txtSubstr4 = text.substr(index, text.size()).c_str();
  QString qTxtSubstr4 = ItemUtils::convert2QString(txtSubstr4);

  QString txtSScript1(text.at(0) == '-' ? qTxtSubstr1 : qTxtSubstr2);
  QString txtSScript2(text.at(0) == '-' ? qTxtSubstr3 : qTxtSubstr4);

  textVect.push_back(txtSScript1);

  textVect.push_back(txtSScript2);

  QPainterPath textObject = ItemUtils::superscriptTextToVector(textVect, fontVect, pointMM, rotate);

  QPen pen = painter->pen();
  pen.setWidthF(0);

  QBrush brush = painter->brush();
  brush.setStyle(Qt::SolidPattern);

  painter->save();

  painter->setPen(pen);
  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->fillPath(textObject, brush);

  painter->restore();
}

QPainterPath te::layout::ItemUtils::getRotationSymbol(const QPointF& pos, double widthMM)
{
  double halfWidthMM = widthMM / 2.;
  double arrowWidthMM = widthMM / 4.;

  QRectF rectRotation(pos.x() - halfWidthMM, pos.y() - widthMM, widthMM, widthMM);
  QPointF centerRight = rectRotation.center();
  centerRight.setX(centerRight.x() + halfWidthMM);

  QPainterPath Ppath;
  Ppath.moveTo(centerRight);

  Ppath.lineTo(centerRight.x() + arrowWidthMM, centerRight.y() - arrowWidthMM);
  Ppath.moveTo(centerRight);
  Ppath.lineTo(centerRight.x() - arrowWidthMM, centerRight.y() - arrowWidthMM);
  Ppath.moveTo(centerRight);
  Ppath.arcTo(rectRotation, 0.0, 150.0);

  QPointF centerLeft = rectRotation.center();
  centerLeft.setX(centerLeft.x() - halfWidthMM);

  Ppath.moveTo(centerLeft);

  Ppath.lineTo(centerLeft.x() - arrowWidthMM, centerLeft.y() + arrowWidthMM);
  Ppath.moveTo(centerLeft);
  Ppath.lineTo(centerLeft.x() + arrowWidthMM, centerLeft.y() + arrowWidthMM);
  Ppath.moveTo(centerLeft);
  Ppath.arcTo(rectRotation, 180.0, 150.0);

  return Ppath;
}

