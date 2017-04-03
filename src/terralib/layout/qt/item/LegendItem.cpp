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

#include "LegendItem.h"
#include "../../item/LegendModel.h"
#include "LegendController.h"
#include "../../core/ItemInputProxy.h"
#include "../../core/pattern/singleton/Context.h"
#include "../core/Scene.h"
#include "../core/ItemUtils.h"

// TerraLib
#include "terralib/maptools/GroupingItem.h"
#include "terralib/maptools/Grouping.h"
#include "terralib/maptools/Canvas.h"
#include "terralib/maptools/CanvasConfigurer.h"
#include "terralib/qt/widgets/canvas/Canvas.h"
#include "terralib/se/Symbolizer.h"
#include "terralib/se/PointSymbolizer.h"
#include "terralib/se/Style.h"
#include "terralib/se/Rule.h"

// Qt
#include <QPixmap>

te::layout::LegendItem::LegendItem(te::layout::ItemInputProxy* itemInputProxy)
  : AbstractItem(itemInputProxy)
  , m_currentMaxHeight(0)
  , m_maxWidth(0)
  , m_displacementBetweenSymbols(0)
  , m_displacementBetweenSymbolsAndText(0)
  , m_symbolSize(0)
  , m_borderDisplacement(0)
  , m_dispBetweenTitles(0)
  , m_rows(0)
  , m_countColumns(0)
  , m_countRows(0)
  , m_offsetBetweenColumns(0)
  , m_penWidth(1.)
  , m_groupingOffsetPair(0)
  , m_hierarchyFactor(0.8)
{
  //The text size or length that exceeds the sides will be cut
  setFlag(QGraphicsItem::ItemClipsToShape);
}

te::layout::LegendItem::~LegendItem()
{

}

te::layout::AbstractItemModel* te::layout::LegendItem::createModel() const
{
  return new LegendModel();
}

te::layout::AbstractItemController* te::layout::LegendItem::createController() const
{
  AbstractItemModel* model = createModel();
  return new LegendController(model, (AbstractItemView*)this);
}

void te::layout::LegendItem::drawItem( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
  if(m_layerList.empty() == true)
  {
    return;
  }

  double x1 = 0;
  double y1 = 0;

  initXY(x1, y1);

  m_countRows = 0;
  m_countColumns = 0;

  m_penWidth = painter->pen().widthF();

  for (std::list<te::map::AbstractLayerPtr>::const_iterator it = m_layerList.begin(); it != m_layerList.end(); ++it)
  {
    const te::map::AbstractLayerPtr layer = (*it);

    verifyLimitRows(x1, y1);
    drawLegend(painter, layer, x1, y1);
  }
}

void te::layout::LegendItem::drawLegend(QPainter* painter, te::map::AbstractLayerPtr layer, double& x1, double& y1)
{
  std::string title = layer->getTitle();
  te::map::Grouping* grouping = layer->getGrouping();
  if (grouping)
  {
    drawSimple(painter, layer, title, x1, y1, m_symbolSize); // symbol with title
    drawGrouping(grouping, painter, layer, x1, y1); // legend (grouping)
  }
  else
  {
    drawSimple(painter, layer, title, x1, y1, m_symbolSize); // symbol with title
  }
}

void te::layout::LegendItem::drawGrouping(te::map::Grouping* grouping, QPainter* painter, te::map::AbstractLayerPtr layer, double& x1, double& y1)
{
  if (!grouping)
    return;

  if (grouping->isVisible() == false)
    return;
  
  std::string propertyName = grouping->getPropertyName();
  const std::vector<te::map::GroupingItem*>& items = grouping->getGroupingItems();
  te::map::GroupingType type = grouping->getType();
  double symbolSize = m_symbolSize * m_hierarchyFactor;

  double hierarchyOffset = m_symbolSize + m_groupingOffsetPair;

  // hierarchy offset
  x1 += hierarchyOffset;

  for (unsigned int i = 0; i < items.size(); ++i)
  {
    te::map::GroupingItem* item = items[i];
    verifyLimitRows(x1, y1);

    std::string label = getLabel(propertyName, type, item);
    verticalAdjustmentBetweenPairs(y1, label, m_symbolSize, m_qFontLegend);

    QPointF pt = verticalTextAdjustment(x1, y1, label, m_qFontLegend);
    drawLabel(painter, pt, m_qFontLegend, m_qFontLegendColor, label);

    const std::vector<te::se::Symbolizer*>& symbolizers = item->getSymbolizers();

    drawSymbolizers(painter, x1, y1, symbolizers, symbolSize);

    if (i == (items.size() - 1))
    {
      y1 -= m_dispBetweenTitles;
    }
    else
    {
      y1 -= m_displacementBetweenSymbols;
    }
  }

  // remove hierarchy offset
  x1 -= hierarchyOffset;
}

void te::layout::LegendItem::drawSimple(QPainter* painter, te::map::AbstractLayerPtr layer, const std::string& title, double& x1, double& y1, double symbolSize)
{
  // Gets the associated layer style
  te::se::Style* style = layer->getStyle();
  if (style == 0)
    return;
  
  if (style->getRules().empty())
    return;

  const te::se::Rule* rule = style->getRule(0);
  const std::vector<te::se::Symbolizer*>& symbolizers = rule->getSymbolizers();

  verticalAdjustmentBetweenPairs(y1, title, symbolSize, m_qFontTitle);

  QPointF pt = verticalTextAdjustment(x1, y1, title, m_qFontTitle);
  drawLabel(painter, pt, m_qFontTitle, m_qFontTitleColor, title);

  drawSymbolizers(painter, x1, y1, symbolizers, symbolSize);

  te::map::Grouping* grouping = layer->getGrouping();
  if (grouping)
  {
    y1 -= m_displacementBetweenSymbols;
  }
  else
  {
    y1 -= m_dispBetweenTitles;
  }
}

void te::layout::LegendItem::drawSymbolizers(QPainter* painter, double& x1, double& y1, std::vector<te::se::Symbolizer*> symbolizers, double symbolSize)
{
  foreach(te::se::Symbolizer* symbol, symbolizers)
  {
    QRectF geomRect(x1, y1, symbolSize, symbolSize);
    geomRect = verticalLegendGeomAdjustment(geomRect);

    te::gm::Geometry* geom = createGeometry(geomRect, symbol);
    if (!geom)
      continue;

    drawGeometry(painter, geomRect, symbol, geom);

    delete geom;
    geom = 0;
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

te::gm::Geometry* te::layout::LegendItem::createPolygonSymbolizer(QRectF geomRect)
{
  te::gm::Geometry* geom = 0;
    
  double x1 = geomRect.x();
  double y1 = geomRect.y();

  te::gm::Polygon* polygon = new te::gm::Polygon(1, te::gm::PolygonType);
  te::gm::LinearRing* ring = new te::gm::LinearRing(5, te::gm::LineStringType);
  ring->setPoint(0, x1 + m_penWidth, y1 + m_penWidth);
  ring->setPoint(1, x1 + geomRect.width() - m_penWidth, y1 + m_penWidth);
  ring->setPoint(2, x1 + geomRect.width() - m_penWidth, y1 + geomRect.height() - m_penWidth);
  ring->setPoint(3, x1 + m_penWidth, y1 + geomRect.height() - m_penWidth);
  ring->setPoint(4, x1 + m_penWidth, y1 + m_penWidth);
  polygon->setRingN(0, ring);
  geom = polygon;

  return geom;
}

te::gm::Geometry* te::layout::LegendItem::createLineSymbolizer(QRectF geomRect)
{
  te::gm::Geometry* geom = 0;

  double x1 = geomRect.x();
  double y1 = geomRect.y();

  te::gm::LineString* line = new te::gm::LineString(2, te::gm::LineStringType);
  line->setPoint(0, x1, y1 + geomRect.height() * 0.5);
  line->setPoint(1, x1 + geomRect.width(), y1 + geomRect.height() * 0.5);
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
  if (!symbol || !geom)
    return;

  if (!geomRect.isValid())
    return;

  te::qt::widgets::Canvas* geomCanvas = createCanvas(geomRect, symbol);
  if (geomCanvas)
  {
    // Let's draw!
    geomCanvas->draw(geom);

    painter->save();
    QPixmap* pixmap = geomCanvas->getPixmap();
    if (pixmap)
    {
      te::layout::ItemUtils::drawPixmap(geomRect, painter, *pixmap);
    }
    painter->restore();
    delete geomCanvas;
    geomCanvas = 0;
  }  
}

void te::layout::LegendItem::drawLabel(QPainter* painter, const QPointF& point, const QFont& font, const QColor& fontColor, std::string text)
{
  painter->save();

  const Property& lineWidth = this->getProperty("line_width");
  double lnew = te::layout::Property::GetValueAs<double>(lineWidth);

  QPen pen(fontColor, lnew, Qt::SolidLine);
  painter->setPen(pen);

  painter->setFont(font);
  painter->setBrush(fontColor);

  ItemUtils::drawText(point, painter, font, text);

  painter->restore();
}

te::qt::widgets::Canvas* te::layout::LegendItem::createCanvas(QRectF rectMM, te::se::Symbolizer*symbol)
{
  te::qt::widgets::Canvas* geomCanvas = 0;

  ItemInputProxy* itemInputProxy = this->getItemInputProxy();
  if (itemInputProxy == 0)
  {
    return geomCanvas;
  }
  Utils utils = itemInputProxy->getUtils();

  te::gm::Envelope box(0, 0, rectMM.width(), rectMM.height());
  box = utils.viewportBox(box);

  int pixmapWidth = box.getWidth();
  int pixmapHeight = box.getHeight();

  double llx = rectMM.x();
  double lly = rectMM.y();
  double urx = rectMM.x() + rectMM.width();
  double ury = rectMM.y() + rectMM.height();
  
  geomCanvas = new te::qt::widgets::Canvas(pixmapWidth, pixmapHeight);
  geomCanvas->setWindow(llx, lly, urx, ury);

  QPixmap* pixmap = geomCanvas->getPixmap();
  pixmap->fill(Qt::transparent);

  // Configuring...
  te::map::CanvasConfigurer cc(geomCanvas);
  cc.config(symbol);

  resizeMark(geomCanvas, symbol, pixmapWidth, pixmapHeight);

  geomCanvas->setPointWidth(pixmapWidth);
  geomCanvas->setLineWidth(pixmapWidth);
  geomCanvas->setPolygonPatternWidth(pixmapWidth);

  return geomCanvas;
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

void te::layout::LegendItem::resizeMark(te::qt::widgets::Canvas* geomCanvas, te::se::Symbolizer*symbol, int pxWidth, int pxHeight)
{
  // Resize mark (image) depending on the zoom 
  te::se::PointSymbolizer* pointSymbol = dynamic_cast<te::se::PointSymbolizer*>(symbol);
  if (pointSymbol)
  {
    // Gets the new graphic size 
    std::size_t width = static_cast<std::size_t>(pxWidth);
    std::size_t height = static_cast<std::size_t>(pxHeight);
    te::color::RGBAColor** rgba = ItemUtils::changePointMarkSize(pointSymbol, width, height);
    if (rgba)
    {
      geomCanvas->setPointPattern(rgba, pxWidth, pxHeight);
    }
  }
}

void te::layout::LegendItem::refreshLegendProperties()
{
  const Property& pFontColor = this->getProperty("font_legend_color");
  const Property& pLegendFont = this->getProperty("font_legend");
  const Property& pDisplacementBetweenSymbols = this->getProperty("displacement_between_symbols");
  const Property& pDisplacementBetweenSymbolsAndText = this->getProperty("displacement_between_symbols_and_texts");
  const Property& pSymbolSize = this->getProperty("symbol_size");
  const Property& pBorderDisplacement = this->getProperty("border_displacement");
  const Property& pDispBetweenTitles = this->getProperty("displacement_between_titles");
  const Property& pFontTitleColor = this->getProperty("font_title_color");
  const Property& pTitleFont = this->getProperty("font_title");
  const Property& pRows = this->getProperty("rows");
  const Property& pOffsetBetweenColumns = this->getProperty("offset_between_columns");
  const Property& pGroupingOffsetPair = this->getProperty("grouping_offset_pair");

  const te::color::RGBAColor& fontLegendColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFontColor);
  const Font& fontLegend = te::layout::Property::GetValueAs<Font>(pLegendFont);
  m_displacementBetweenSymbols = te::layout::Property::GetValueAs<double>(pDisplacementBetweenSymbols);
  m_displacementBetweenSymbolsAndText = te::layout::Property::GetValueAs<double>(pDisplacementBetweenSymbolsAndText);
  m_symbolSize = te::layout::Property::GetValueAs<double>(pSymbolSize);
  m_borderDisplacement = te::layout::Property::GetValueAs<double>(pBorderDisplacement);
  m_dispBetweenTitles = te::layout::Property::GetValueAs<double>(pDispBetweenTitles);
  const te::color::RGBAColor& fontTitleColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFontTitleColor);
  const Font& fontTitle = te::layout::Property::GetValueAs<Font>(pTitleFont);
  m_rows = te::layout::Property::GetValueAs<int>(pRows);
  m_offsetBetweenColumns = te::layout::Property::GetValueAs<double>(pOffsetBetweenColumns);
  m_groupingOffsetPair = te::layout::Property::GetValueAs<double>(pGroupingOffsetPair);

  m_qFontLegendColor.setRgb(fontLegendColor.getRed(), fontLegendColor.getGreen(), fontLegendColor.getBlue(), fontLegendColor.getAlpha());
  m_qFontLegend = ItemUtils::convertToQfont(fontLegend);
  m_qFontTitleColor.setRgb(fontTitleColor.getRed(), fontTitleColor.getGreen(), fontTitleColor.getBlue(), fontTitleColor.getAlpha());
  m_qFontTitle = ItemUtils::convertToQfont(fontTitle);

  LegendController* controller = dynamic_cast<LegendController*>(getController());
  if (controller)
  {
    m_layerList = controller->searchLayersFromURI();
  }
}

void te::layout::LegendItem::verticalAdjustmentBetweenPairs(double& y1, std::string label, double symbolSize, const QFont& font)
{
  if (!scene())
    return;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();

  QString qFontName = font.family();
  std::string fontName = ItemUtils::convert2StdString(qFontName);
  QRectF textBoundary = utils.getMinimumTextBoundary(fontName, font.pointSize(), label);

  double maxWidth = textBoundary.width() + m_symbolSize + m_displacementBetweenSymbolsAndText + m_borderDisplacement;
  m_maxWidth = qMax(maxWidth, m_maxWidth);

  m_currentMaxHeight = qMax(textBoundary.height(), m_symbolSize);
  y1 -= m_currentMaxHeight;
}

QPointF te::layout::LegendItem::verticalTextAdjustment(double x1, double y1, std::string text, const QFont& font)
{
  QPointF pt(x1, y1);

  if (!scene())
    return pt;

  Scene* sc = dynamic_cast<Scene*>(scene());
  ItemUtils utils = sc->getItemUtils();

  x1 += m_symbolSize + m_displacementBetweenSymbolsAndText;
  pt.setX(x1);

  QString qFontName = font.family();
  std::string fontName = ItemUtils::convert2StdString(qFontName);
  QRectF textBoundary = utils.getMinimumTextBoundary(fontName, font.pointSize(), text);
  if (textBoundary.height() != m_currentMaxHeight)
  {
    // if the symbol is bigger than the font, then the text is to be centered
    pt.setY(y1 + ((m_currentMaxHeight / 2.) - (textBoundary.height() / 2.)));
  }
  return pt;
}

QRectF te::layout::LegendItem::verticalLegendGeomAdjustment(QRectF geom)
{
  if (geom.height() != m_currentMaxHeight)
  {
    // a font is bigger than the symbol, then the symbol must be centered
    double y = geom.y() + ((m_currentMaxHeight / 2.) - (geom.height() / 2.));
    geom.setRect(geom.x(), y, geom.width(), geom.height());
  }
  return geom;
}

void te::layout::LegendItem::initXY(double& x1, double& y1)
{
  QRectF boundRect = this->boundingRect();

  x1 = boundRect.x() + m_borderDisplacement;
  y1 = boundRect.bottomLeft().y() - m_borderDisplacement;
}

void te::layout::LegendItem::verifyLimitRows(double& x1, double& y1)
{
  if (m_countRows == m_rows)
  {
    m_countRows = 0;
    double x = 0;
    initXY(x, y1);
    // m_symbolSize + m_groupingOffsetPair: used for grouping hierarchy offset 
    double hierarchyOffset = m_symbolSize + m_groupingOffsetPair;
    x1 += m_maxWidth + m_offsetBetweenColumns + hierarchyOffset;
    m_countColumns++;
  }
  m_countRows++;
}

