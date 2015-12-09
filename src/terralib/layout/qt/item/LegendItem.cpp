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
  \file LegendItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "LegendItem.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/pattern/singleton/Context.h"
#include "terralib/maptools/GroupingItem.h"
#include "terralib/maptools/Grouping.h"
#include "terralib/maptools/Canvas.h"
#include "terralib/maptools/CanvasConfigurer.h"
#include "terralib/qt/widgets/canvas/Canvas.h"
#include "terralib/se/Symbolizer.h"
#include "terralib/se/PointSymbolizer.h"
#include "../core/Scene.h"
#include "../core/ItemUtils.h"

// Qt
#include <QPixmap>
#include <QStyle>
#include <QStyleOption>
#include <QFont>
#include <QPaintDevice>
#include <QColor>
#include <QMatrix>

te::layout::LegendItem::LegendItem(AbstractItemController* controller)
  : AbstractItem<QGraphicsItem>(controller),
  m_vtrCenter(0)
{
  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
}

te::layout::LegendItem::~LegendItem()
{

}

void te::layout::LegendItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();
  
  const Property& pLayers = m_controller->getProperty("layers");  
  const Property& pBorderDisplacement = m_controller->getProperty("border_displacement");
  const Property& pDispBetweenTitleAndSymbols = m_controller->getProperty("displacement_between_title_and_symbols");

  const std::list<te::map::AbstractLayerPtr>& layerList = pLayers.getValue().toLayerList();
  int borderDisplacement = pBorderDisplacement.getValue().toDouble();
  int dispBetweenTitleAndSymbols = pDispBetweenTitleAndSymbols.getValue().toDouble();
  
  if(layerList.empty() == true)
  {
    return;
  }

  QRectF boundRect = this->boundingRect();

  double x1 = boundRect.x() + borderDisplacement;
  double y1 = boundRect.bottomLeft().y() - borderDisplacement;

  double wtxtInPixels = 0.;
  double htxtInPixels = 0.;

  for (std::list<te::map::AbstractLayerPtr>::const_iterator it = layerList.begin(); it != layerList.end(); ++it)
  {
    const te::map::AbstractLayerPtr layer = (*it);

    std::string title = layer->getTitle();

    QRectF textBoundary = utils.getMinimumTextBoundary("Arial", 12, title);
    
    y1 -= textBoundary.height();

    QPoint ptTitle(x1, y1);
    QFont font("Arial", 12);
    drawText(ptTitle, painter, font, title);

    y1 -= textBoundary.height() - dispBetweenTitleAndSymbols;

    drawLegend(painter, layer, x1, y1);
  }
}

void te::layout::LegendItem::drawLegend(QPainter* painter, te::map::AbstractLayerPtr layer, double x1, double& y1)
{
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();

  const Property& pDisplacementBetweenSymbols = m_controller->getProperty("displacement_between_symbols");
  const Property& pDisplacementBetweenSymbolsAndText = m_controller->getProperty("displacement_between_symbols_and_texts");
  const Property& pSymbolSize = m_controller->getProperty("symbol_size");
  const Property& pFont = m_controller->getProperty("font");

  double displacementBetweenSymbols = pDisplacementBetweenSymbols.getValue().toDouble();
  double displacementBetweenSymbolsAndText = pDisplacementBetweenSymbolsAndText.getValue().toDouble();
  double symbolSize = pSymbolSize.getValue().toDouble();
  const Font& font = pFont.getValue().toFont();

  QFont qFont(QString(font.getFamily().c_str()), font.getPointSize());
  
  te::map::Grouping* grouping = layer->getGrouping();

  if (grouping != 0 && grouping->isVisible() == true)
  {
    std::string propertyName = grouping->getPropertyName();
    const std::vector<te::map::GroupingItem*>& items = grouping->getGroupingItems();
    te::map::GroupingType type = grouping->getType();

    double labelX1 = x1 + symbolSize + displacementBetweenSymbolsAndText;

    for (unsigned int i = 0; i < items.size(); ++i)
    {
      te::map::GroupingItem* item = items[i];

      std::string label = getLabel(propertyName, type, item);
      QRectF textBoundary = utils.getMinimumTextBoundary(font.getFamily(), font.getPointSize(), label);
      
      if (textBoundary.height() > symbolSize)
      {
        y1 -= textBoundary.height();
        m_vtrCenter = textBoundary.height() / 2.;
      }
      else
      {
        y1 -= symbolSize;
        m_vtrCenter = symbolSize / 2.;
      }

      drawLabel(painter, labelX1, y1, propertyName, type, item, qFont);

      const std::vector<te::se::Symbolizer*>& symbolizers = item->getSymbolizers();

      foreach(te::se::Symbolizer* symbol, symbolizers)
      {
        double offset = 2.0;
        QRectF geomRect(x1, y1, symbolSize, symbolSize);

        te::gm::Geometry* geom = createGeometry(geomRect, symbol);
        if (!geom)
          continue;

        drawGeometry(painter, geomRect, symbol, geom);

        delete geom;
        geom = 0;
      }

      y1 -= displacementBetweenSymbols;
    }
  }
}

te::gm::Geometry* te::layout::LegendItem::createGeometry(QRectF geomRect, te::se::Symbolizer* symbol)
{
  te::gm::Geometry* geom = 0;
  if (symbol->getType() == "PolygonSymbolizer")
  {
    geom = createPolygonSymbolizer(geomRect);
  }
  else if (symbol->getType() == "LineSymbolizer")
  {
    geom = createLineSymbolizer(geomRect);
  }
  else if (symbol->getType() == "PointSymbolizer")
  {
    geom = createPointSymbolizer(geomRect);
  }
  return geom;
}

te::gm::Geometry* te::layout::LegendItem::createPolygonSymbolizer(QRectF geomRect, double offset)
{
  te::gm::Geometry* geom = 0;
  
  double x1 = geomRect.x();
  double y1 = geomRect.y();

  te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
  te::gm::LinearRing* ring = new te::gm::LinearRing(5, te::gm::LineStringType);
  ring->setPoint(0, x1 + offset, y1 + offset);
  ring->setPoint(1, x1 + geomRect.width() - offset, y1 + offset);
  ring->setPoint(2, x1 + geomRect.width() - offset, y1 + geomRect.height() - offset);
  ring->setPoint(3, x1 + offset, y1 + geomRect.height() - offset);
  ring->setPoint(4, x1 + offset, y1 + offset);
  polygon->setRingN(0, ring);
  geom = polygon;

  return geom;
}

te::gm::Geometry* te::layout::LegendItem::createLineSymbolizer(QRectF geomRect, double offset)
{
  te::gm::Geometry* geom = 0;

  double x1 = geomRect.x();
  double y1 = geomRect.y();

  te::gm::LineString* line = new te::gm::LineString(2, te::gm::LineStringType);
  line->setPoint(0, x1 + offset, y1 + geomRect.height() * 0.5);
  line->setPoint(1, x1 + geomRect.width() - offset, y1 + geomRect.height() * 0.5);
  geom = line;

  return geom;
}

te::gm::Geometry* te::layout::LegendItem::createPointSymbolizer(QRectF geomRect)
{
  te::gm::Geometry* geom = 0;

  double x1 = geomRect.x();
  double y1 = geomRect.y();

  x1 += geomRect.width() * 0.5;
  y1 += geomRect.height() * 0.5;

  geom = new te::gm::Point(x1, y1);

  return geom;
}

void te::layout::LegendItem::drawGeometry(QPainter* painter, QRectF geomRect, te::se::Symbolizer*symbol, te::gm::Geometry* geom)
{
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  Utils utils = sc->getUtils();
  ItemUtils itemUtils = sc->getItemUtils();

  if (!symbol || !geom)
    return;

  if (!geomRect.isValid())
    return;
  
  int pixmapWidthMM = geomRect.width();
  int pixmapHeightMM = geomRect.height();

  double x1 = geomRect.x();
  double y1 = geomRect.y();

  QRectF rect(x1, y1, pixmapWidthMM, pixmapHeightMM);

  if (rect.height() != (m_vtrCenter * 2))
  {
    double y = y1 + (m_vtrCenter - (pixmapHeightMM / 2.));
    rect.setRect(x1, y, pixmapWidthMM, pixmapHeightMM);
  }

  te::gm::Envelope box(0, 0, pixmapWidthMM, pixmapHeightMM);
  box = utils.viewportBox(box);
      
  int pixmapWidth = box.getWidth();
  int pixmapHeight = box.getHeight();

  double llx = x1;
  double lly = y1;
  double urx = x1 + pixmapWidthMM;
  double ury = y1 + pixmapHeightMM;

  QPixmap* pixmap = new QPixmap(pixmapWidth, pixmapHeight);
  pixmap->fill(Qt::transparent);

  te::qt::widgets::Canvas geomCanvas(pixmap);
  geomCanvas.setWindow(llx, lly, urx, ury);

  // Configuring...
  te::map::CanvasConfigurer cc(&geomCanvas);
  cc.config(symbol);

  // Resize mark (image) depending on the zoom 
  te::se::PointSymbolizer* pointSymbol = dynamic_cast<te::se::PointSymbolizer*>(symbol);
  if (pointSymbol)
  {
    // Gets the new graphic size 
    std::size_t width = static_cast<std::size_t>(pixmapWidth);
    std::size_t height = static_cast<std::size_t>(pixmapHeight);
    te::color::RGBAColor** rgba = itemUtils.changePointMarkSize(pointSymbol, width, height);
    if (rgba)
    {
      geomCanvas.setPointPattern(rgba, pixmapWidth, pixmapHeight);
    }
  }

  geomCanvas.setPointWidth(pixmapWidth);
  geomCanvas.setLineWidth(pixmapWidth);
  geomCanvas.setPolygonPatternWidth(pixmapWidth);
  
  // Let's draw!
  geomCanvas.draw(geom);
  
  painter->save();
  drawPixmap(rect, painter, *pixmap);
  painter->restore();
}

void te::layout::LegendItem::drawLabel(QPainter* painter, double x1, double& y1, std::string propertyName,
  te::map::GroupingType type, te::map::GroupingItem* item, QFont font)
{
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();

  const Property& pFontColor = m_controller->getProperty("font_color");
  const te::color::RGBAColor& fontColor = pFontColor.getValue().toColor();

  QColor qFontColor(fontColor.getRed(), fontColor.getGreen(), fontColor.getBlue(), fontColor.getAlpha());

  painter->save();
  
  std::string label = getLabel(propertyName, type, item);
    
  QPen pen(qFontColor);
  painter->setPen(pen);

  painter->setFont(font);
  painter->setBrush(qFontColor);

  QPointF pt(x1, y1);

  QRectF textBoundary = utils.getMinimumTextBoundary(font.family().toStdString(), font.pointSize(), label);

  if (textBoundary.height() != (m_vtrCenter * 2))
  {
    pt.setY(y1 + (m_vtrCenter - (textBoundary.height()/2.)));
  }

  drawText(pt, painter, font, label);

  painter->restore();
}

std::string te::layout::LegendItem::getLabel(std::string propertyName, te::map::GroupingType type, te::map::GroupingItem* item)
{
  std::string label = propertyName;
  label += ": ";

  if (type == te::map::UNIQUE_VALUE)
  {
    label += item->getValue();
  }
  else
  {
    const std::string& upperLimit = item->getUpperLimit();
    const std::string& lowerLimit = item->getLowerLimit();

    label += lowerLimit;
    label += " ~ ";
    label += upperLimit;
  }
  return label;
}

