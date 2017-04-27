#include "Grid.h"

#include "ItemUtils.h"
#include "../../core/Utils.h"
#include "../../core/WorldTransformer.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"
#include "../../core/property/GridSettingsConfigProperties.h"
#include "../../core/property/Properties.h"

//terralib
#include <terralib/maptools/WorldDeviceTransformer.h>
#include <terralib/srs/SpatialReferenceSystemManager.h>
#include <terralib/common/StringUtils.h>
#include <terralib/maptools/Utils.h>

// STL
#include <iostream>
#include <utility>

// Qt
#include <QFont>
#include <QString>

te::layout::Grid::Grid()
  : m_maxGaps(30)
  , m_defaultRotation(90.)
{

}

te::layout::Grid::~Grid()
{

}

void te::layout::Grid::clear()
{
  m_gridLines = QPainterPath();

  m_gridText = QPainterPath();

  m_topTexts.clear();
  m_bottomTexts.clear();
  m_rightTexts.clear();
  m_leftTexts.clear();

  m_size = QSizeF();
  m_origin = QPointF();
  m_final = QPointF();
}

QSizeF te::layout::Grid::getSize() const
{
  return m_size;
}

QPointF te::layout::Grid::getOrigin() const
{
  return m_origin;
}

QPointF te::layout::Grid::getFinal() const
{
  return m_final;
}

void te::layout::Grid::drawGrid(QPainter* painter, const te::layout::Properties& properties, const GridSettingsConfigProperties& settingsConfig) const
{
  if (m_gridLines.isEmpty() || m_gridText.isEmpty())
  {
    return;
  }

  painter->save();

  painter->setRenderHint(QPainter::Antialiasing, true);

  configPainterForGrid(painter, properties, settingsConfig);

  painter->drawPath(m_gridLines);

  configTextPainter(painter, properties, settingsConfig);

  QPen pen = painter->pen();
  pen.setWidthF(0);

  QBrush brush = painter->brush();
  brush.setStyle(Qt::SolidPattern);

  painter->setPen(pen);
  painter->fillPath(m_gridText, brush);

  painter->restore();
}

double te::layout::Grid::initHorizontalLines(const te::gm::Envelope& geoBox, const Properties& gridSettings, const GridSettingsConfigProperties& settingsConfig) const
{
  const Property& pHorizontalLineInitial = gridSettings.getProperty(settingsConfig.getHorizontalLineInitial());
  const Property& pHorizontalLineGap = gridSettings.getProperty(settingsConfig.getHorizontalLineGap());

  double horizontalLineInitial = te::layout::Property::GetValueAs<double>(pHorizontalLineInitial);
  double horizontalLineGap = te::layout::Property::GetValueAs<double>(pHorizontalLineGap);

  double yInit = horizontalLineInitial;
  if (yInit < geoBox.getLowerLeftY())
  {
    double dify = geoBox.getLowerLeftY() - yInit;
    int nParts = (int)(dify / horizontalLineGap);
    if (nParts == 0)
    {
      yInit = horizontalLineInitial;
    }
    else
    {
      yInit = yInit + (nParts * horizontalLineGap);
    }
  }

  return yInit;
}

double te::layout::Grid::initVerticalLines(const te::gm::Envelope& geoBox, const Properties& gridSettings, const GridSettingsConfigProperties& settingsConfig) const
{
  const Property& pVerticalLineInitial = gridSettings.getProperty(settingsConfig.getVerticalLineInitial());
  const Property& pVerticalLineGap = gridSettings.getProperty(settingsConfig.getVerticalLineGap());

  double verticalLineInitial = te::layout::Property::GetValueAs<double>(pVerticalLineInitial);
  double verticalLineGap = te::layout::Property::GetValueAs<double>(pVerticalLineGap);

  double xInit = verticalLineInitial;
  if (xInit < geoBox.getLowerLeftX())
  {
    double difx = geoBox.getLowerLeftX() - xInit;
    int nParts = (int)(difx / verticalLineGap);
    if (nParts == 0)
    {
      xInit = verticalLineInitial;
    }
    else
    {
      xInit = xInit + (nParts * verticalLineGap);
    }
  }

  return xInit;
}

QPainterPath te::layout::Grid::convertLinesToPath(const std::vector<te::gm::LineString>& verticalLines, const std::vector<te::gm::LineString>& horizontalLines) const
{
  QPainterPath gridLines;

  for (std::size_t i = 0; i < horizontalLines.size(); ++i)
  {
    gridLines.addPath(ItemUtils::lineToQPath(horizontalLines.at(i)));
  }

  for (std::size_t i = 0; i < verticalLines.size(); ++i)
  {
    gridLines.addPath(ItemUtils::lineToQPath(verticalLines.at(i)));
  }

  return gridLines;
}

QPainterPath te::layout::Grid::convertCrossesToPath(const std::vector<te::gm::LineString>& verticalLines, const std::vector<te::gm::LineString>& horizontalLines, const Properties& properties, const GridSettingsConfigProperties& settingsConfig) const
{
  QPainterPath crossLinesGrid;

  const Property& pCrossOffset = properties.getProperty(settingsConfig.getCrossOffset());
  double crossOffSet = te::layout::Property::GetValueAs<double>(pCrossOffset);

  const Property& pUseBorderIntersection = properties.getProperty(settingsConfig.getBorderIntersections());
  bool useBorderItersection = te::layout::Property::GetValueAs<bool>(pUseBorderIntersection);

  std::vector<te::gm::LineString>::const_iterator itv = verticalLines.begin();
  for (; itv != verticalLines.end(); ++itv)
  {
    const te::gm::LineString& verticalLine = (*itv);

    std::vector<te::gm::LineString>::const_iterator ith = horizontalLines.begin();
    for (; ith != horizontalLines.end(); ++ith)
    {
      const te::gm::LineString& horizontalLine = (*ith);

      std::auto_ptr<te::gm::Geometry> interGeometry;
      try
      {
        interGeometry.reset(verticalLine.intersection(&horizontalLine));
      }
      catch (...)
      {
        continue;
      }

      if (interGeometry.get() == 0 || interGeometry->isValid() == false)
      {
        continue;
      }

      if (interGeometry->getGeomTypeId() != te::gm::PointType)
      {
        continue;
      }

      te::gm::Point* interPoint = dynamic_cast<te::gm::Point*>(interGeometry.get());
      if (interPoint == 0)
      {
        continue;
      }

      QLineF lneHrz(interPoint->getX() - crossOffSet, interPoint->getY(), interPoint->getX() + crossOffSet, interPoint->getY());
      QLineF lneVrt(interPoint->getX(), interPoint->getY() - crossOffSet, interPoint->getX(), interPoint->getY() + crossOffSet);

      if (calculateCrossIntersectMapBorder(lneVrt, lneHrz, properties, settingsConfig, crossLinesGrid) == true)
      {
        continue;
      }

      crossLinesGrid.moveTo(lneHrz.p1().x(), lneHrz.p1().y());
      crossLinesGrid.lineTo(lneHrz.p2().x(), lneHrz.p2().y());

      crossLinesGrid.moveTo(lneVrt.p1().x(), lneVrt.p1().y());
      crossLinesGrid.lineTo(lneVrt.p2().x(), lneVrt.p2().y());

    }
  }

  if (useBorderItersection)
  {
    calculateBordersSegments(crossOffSet, verticalLines, horizontalLines, properties, settingsConfig, crossLinesGrid);
  }

  return crossLinesGrid;
}

bool te::layout::Grid::calculateCrossIntersectMapBorder(const QLineF& verticalLine, const QLineF& horizontalLine, const Properties& properties, const GridSettingsConfigProperties& settingsConfig, QPainterPath& crossLinesGrid) const
{
  bool result = false;

  const Property& pWidth = properties.getProperty("width");
  const Property& pHeight = properties.getProperty("height");
  const Property& pCrossOffset = properties.getProperty(settingsConfig.getCrossOffset());

  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);
  double crossOffSet = te::layout::Property::GetValueAs<double>(pCrossOffset);

  //if the cross intersects the border of the reference rect, a line must be drawn instead of the cross
  te::gm::Envelope boxMM(0, 0, width, height);

  QPointF intersectionPoint;

  QLineF bottomLine(boxMM.m_llx, boxMM.m_lly, boxMM.m_urx, boxMM.m_lly);
  QLineF topLine(boxMM.m_llx, boxMM.m_ury, boxMM.m_urx, boxMM.m_ury);
  QLineF leftLine(boxMM.m_llx, boxMM.m_lly, boxMM.m_llx, boxMM.m_ury);
  QLineF rightLine(boxMM.m_urx, boxMM.m_lly, boxMM.m_urx, boxMM.m_ury);

  bool intersects = false;
  if (bottomLine.intersect(verticalLine, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x(), intersectionPoint.y() + crossOffSet);
    intersects = true;

    crossLinesGrid.moveTo(borderLine.p1().x(), borderLine.p1().y());
    crossLinesGrid.lineTo(borderLine.p2().x(), borderLine.p2().y());
  }
  if (topLine.intersect(verticalLine, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x(), intersectionPoint.y() - crossOffSet);
    intersects = true;

    crossLinesGrid.moveTo(borderLine.p1().x(), borderLine.p1().y());
    crossLinesGrid.lineTo(borderLine.p2().x(), borderLine.p2().y());
  }
  if (leftLine.intersect(horizontalLine, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x() + crossOffSet, intersectionPoint.y());
    intersects = true;

    crossLinesGrid.moveTo(borderLine.p1().x(), borderLine.p1().y());
    crossLinesGrid.lineTo(borderLine.p2().x(), borderLine.p2().y());
  }
  if (rightLine.intersect(horizontalLine, &intersectionPoint) == QLineF::BoundedIntersection)
  {
    QLineF borderLine(intersectionPoint.x(), intersectionPoint.y(), intersectionPoint.x() - crossOffSet, intersectionPoint.y());
    intersects = true;

    crossLinesGrid.moveTo(borderLine.p1().x(), borderLine.p1().y());
    crossLinesGrid.lineTo(borderLine.p2().x(), borderLine.p2().y());
  }

  return intersects;
}

void te::layout::Grid::calculateBordersSegments(double crossOffSet, const std::vector<te::gm::LineString>& verticalLines, const std::vector<te::gm::LineString>& horizontalLines, const Properties& properties, const GridSettingsConfigProperties& settingsConfig, QPainterPath& crossLinesGrid) const
{
  const Property& pWidth = properties.getProperty("width");
  const Property& pHeight = properties.getProperty("height");

  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);

  te::gm::Envelope boxMM(0, 0, width, height);

  te::gm::LineString topBorderLine(2, te::gm::LineStringType);
  topBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getUpperRightY());
  topBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getUpperRightY());

  te::gm::LineString bottomBorderLine(2, te::gm::LineStringType);
  bottomBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getLowerLeftY());
  bottomBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getLowerLeftY());

  te::gm::LineString leftBorderLine(2, te::gm::LineStringType);
  leftBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getLowerLeftY());
  leftBorderLine.setPoint(1, boxMM.getLowerLeftX(), boxMM.getUpperRightY());

  te::gm::LineString rightBorderLine(2, te::gm::LineStringType);
  rightBorderLine.setPoint(0, boxMM.getUpperRightX(), boxMM.getLowerLeftY());
  rightBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getUpperRightY());


  std::vector<te::gm::LineString>::const_iterator ith = horizontalLines.begin();
  for (; ith != horizontalLines.end(); ++ith)
  {
    const te::gm::LineString& horizontalLine = (*ith);

    te::gm::Point p;
    if (checkBorderIntersection(leftBorderLine, horizontalLine, p) == false)
    {
      continue;
    }

    bool nearToEdge = isNearEdge(p, leftBorderLine, crossOffSet);

    if (!nearToEdge)
    {
      QLineF lneHrzIntersection(p.getX(), p.getY(), p.getX() + crossOffSet, p.getY());

      crossLinesGrid.moveTo(lneHrzIntersection.p1().x(), lneHrzIntersection.p1().y());
      crossLinesGrid.lineTo(lneHrzIntersection.p2().x(), lneHrzIntersection.p2().y());
    }
  }

  ith = horizontalLines.begin();
  for (; ith != horizontalLines.end(); ++ith)
  {
    const te::gm::LineString& horizontalLine = (*ith);

    te::gm::Point p;

    if (checkBorderIntersection(rightBorderLine, horizontalLine, p) == false)
    {
      continue;
    }

    bool nearToEdge = isNearEdge(p, rightBorderLine, crossOffSet);
    if (!nearToEdge)
    {
      QLineF lneHrzIntersection(p.getX(), p.getY(), p.getX() - crossOffSet, p.getY());

      crossLinesGrid.moveTo(lneHrzIntersection.p1().x(), lneHrzIntersection.p1().y());
      crossLinesGrid.lineTo(lneHrzIntersection.p2().x(), lneHrzIntersection.p2().y());
    }
  }

  std::vector<te::gm::LineString>::const_iterator itv = verticalLines.begin();

  for (; itv != verticalLines.end(); ++itv)
  {
    const te::gm::LineString& verticalLine = (*itv);

    te::gm::Point p;
    if (checkBorderIntersection(topBorderLine, verticalLine, p) == false)
    {
      continue;
    }

    bool nearToEdge = isNearEdge(p, topBorderLine, crossOffSet);
    if (!nearToEdge)
    {
      QLineF lneVrtIntersection(p.getX(), p.getY(), p.getX(), p.getY() - crossOffSet);
      crossLinesGrid.moveTo(lneVrtIntersection.p1().x(), lneVrtIntersection.p1().y());
      crossLinesGrid.lineTo(lneVrtIntersection.p2().x(), lneVrtIntersection.p2().y());
    }
  }

  itv = verticalLines.begin();

  for (; itv != verticalLines.end(); ++itv)
  {
    const te::gm::LineString& verticalLine = (*itv);

    te::gm::Point p;
    if (checkBorderIntersection(bottomBorderLine, verticalLine, p) == false)
    {
      continue;
    }

    QLineF lneVrtIntersection(p.getX(), p.getY(), p.getX(), p.getY() + crossOffSet);
    bool nearToEdge = isNearEdge(p, bottomBorderLine, crossOffSet);

    if (!nearToEdge)
    {
      crossLinesGrid.moveTo(lneVrtIntersection.p1().x(), lneVrtIntersection.p1().y());
      crossLinesGrid.lineTo(lneVrtIntersection.p2().x(), lneVrtIntersection.p2().y());
    }
  }
}

bool te::layout::Grid::isNearEdge(const te::gm::Point& p, const te::gm::LineString& line, const double& offSet) const
{
  te::gm::Point bottonPoint(line.getPointN(0)->getX(), line.getPointN(0)->getY());
  te::gm::Point upperPoint(line.getPointN(1)->getX(), line.getPointN(1)->getY());
  double distance1 = p.distance(&bottonPoint);
  double distance2 = p.distance(&upperPoint);

  if (distance1 <= offSet)
  {
    return true;
  }
  if (distance2 <= offSet)
  {
    return true;
  }

  return false;

}

bool te::layout::Grid::checkBorderIntersection(const te::gm::LineString& bolderLine, const te::gm::LineString& gridLine, te::gm::Point& intersectionPoint) const
{
  std::auto_ptr<te::gm::Geometry> interGeometry;
  te::gm::Point* interPoint = 0;

  te::gm::LineString bLine = roundLineString(bolderLine);
  te::gm::LineString gLine = roundLineString(gridLine);

  try
  {
    interGeometry.reset(bLine.intersection(&gLine));
  }
  catch (...)
  {
    return false;
  }

  if (interGeometry.get() == 0 || interGeometry->isValid() == false)
  {
    return false;
  }

  if (interGeometry->getGeomTypeId() != te::gm::PointType)
  {
    return false;
  }

  interPoint = dynamic_cast<te::gm::Point*>(interGeometry.get());

  intersectionPoint.setX(interPoint->getX());
  intersectionPoint.setY(interPoint->getY());

  return true;
}

te::gm::LineString te::layout::Grid::roundLineString(const te::gm::LineString& lines) const
{
  double x1 = Utils::round(lines.getPointN(0)->getX(), 6);
  double y1 = Utils::round(lines.getPointN(0)->getY(), 6);

  double x2 = Utils::round(lines.getPointN(1)->getX(), 6);
  double y2 = Utils::round(lines.getPointN(1)->getY(), 6);

  te::gm::LineString line(2, te::gm::LineStringType);
  line.setPoint(0, x1, y1);
  line.setPoint(1, x2, y2);

  return line;
}

te::gm::Envelope te::layout::Grid::calculateTop(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double verticalLineDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double x = referencePoint.x();
  double ury = referencePoint.y();

  // text top
  QPointF ptTop(x - (textWidth / 2.), ury + verticalLineDisplacement);

  std::string stdText = ItemUtils::convert2StdString(text);
  m_topTexts.push_back(TextPosition(stdText, ptTop));

  te::gm::Envelope topTextBox(ptTop.x(), ptTop.y(), ptTop.x() + textWidth, ptTop.y() + textHeight);
  return topTextBox;
}

te::gm::Envelope te::layout::Grid::calculateBottom(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double verticalLineDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double x = referencePoint.x();
  double lly = referencePoint.y();

  // text bottom
  QPointF ptBottom(x - (textWidth / 2.), lly - textHeight - verticalLineDisplacement);

  std::string stdText = ItemUtils::convert2StdString(text);
  m_bottomTexts.push_back(TextPosition(stdText, ptBottom));

  te::gm::Envelope bottomTextBox(ptBottom.x(), ptBottom.y(), ptBottom.x() + textWidth, ptBottom.y() + textHeight);
  return bottomTextBox;
}

te::gm::Envelope te::layout::Grid::calculateRight(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double horizontalLineDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  double urx = referencePoint.x();
  double y = referencePoint.y();

  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double yReference = y + textY - (textHeight / 2.);

  // text right
  QPointF ptRight(urx + horizontalLineDisplacement, yReference);

  std::string stdText = ItemUtils::convert2StdString(text);
  m_rightTexts.push_back(TextPosition(stdText, ptRight));

  te::gm::Envelope rightTextBox(ptRight.x(), ptRight.y(), ptRight.x() + textWidth, ptRight.y() + textHeight);

  return rightTextBox;
}

te::gm::Envelope te::layout::Grid::calculateLeft(QPointF referencePoint, QRectF textBoundingRect, QString text, bool rotate, double horizontalLineDisplacement)
{
  double textX = textBoundingRect.x();
  double textY = textBoundingRect.y();
  double textWidth = textBoundingRect.width();
  double textHeight = textBoundingRect.height();

  //if the 90 degrees rotation was applied, we transpose the bounding rect
  if (rotate)
  {
    textX = textBoundingRect.y();
    textY = textBoundingRect.x();
    textWidth = textBoundingRect.height();
    textHeight = textBoundingRect.width();
  }

  double llx = referencePoint.x();
  double y = referencePoint.y();

  double yReference = y + textY - (textHeight / 2.);

  // text left
  QPointF ptLeft(llx - textX - textWidth - horizontalLineDisplacement, yReference);

  std::string stdText = ItemUtils::convert2StdString(text);
  m_leftTexts.push_back(TextPosition(stdText, ptLeft));

  te::gm::Envelope leftTextBox(ptLeft.x(), ptLeft.y(), ptLeft.x() + textWidth, ptLeft.y() + textHeight);

  return leftTextBox;
}

void te::layout::Grid::addTextToPath(const QPointF& point, const QFont& font, const std::string& text, int rotate)
{
  QString qText = ItemUtils::convert2QString(text);

  m_gridText.addPath(ItemUtils::textToVector(qText, font, point, rotate));
}

void te::layout::Grid::calculateSuperScriptText(const QPointF& point, const QFont& font, const std::string& text, int rotate)
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

  m_gridText.addPath(ItemUtils::superscriptTextToVector(textVect, fontVect, point, rotate));
}


void te::layout::Grid::calculateTopTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig)
{
  const Property& pTopRotateText = properties.getProperty(settingsConfig.getTopRotateText());
  const Property& pFont = properties.getProperty(settingsConfig.getFont());

  bool bRotate = te::layout::Property::GetValueAs<bool>(pTopRotateText);
  const Font& font = te::layout::Property::GetValueAs<Font>(pFont);

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotation;
  }

  std::vector<TextPosition>::iterator it = m_topTexts.begin();

  const Property& pSuperscript = properties.getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

  for( ; it != m_topTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;


    if (useSuperScript == true && txt.size() > 2)
    {
      calculateSuperScriptText(pt, qFont, txt, iRotate);
    }
    else
    {
      addTextToPath(pt, qFont, txt, iRotate);
    }


#ifdef _DEBUG
      //debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

void te::layout::Grid::calculateBottomTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig)
{
  const Property& pBottomRotateText = properties.getProperty(settingsConfig.getBottomRotateText());
  const Property& pFont = properties.getProperty(settingsConfig.getFont());

  bool bRotate = te::layout::Property::GetValueAs<bool>(pBottomRotateText);
  const Font& font = te::layout::Property::GetValueAs<Font>(pFont);

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotation;
  }

  std::vector<TextPosition>::iterator it = m_bottomTexts.begin();


  const Property& pSuperscript = properties.getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

  for( ; it != m_bottomTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;


    if (useSuperScript == true && txt.size() > 2)
    {
      this->calculateSuperScriptText(pt, qFont, txt, iRotate);
    }
    else
    {
      this->addTextToPath(pt, qFont, txt, iRotate);
    }

#ifdef _DEBUG
    //debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

void te::layout::Grid::calculateLeftTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig)
{
  const Property& pLeftRotateText = properties.getProperty(settingsConfig.getLeftRotateText());
  const Property& pFont = properties.getProperty(settingsConfig.getFont());

  bool bRotate = te::layout::Property::GetValueAs<bool>(pLeftRotateText);
  const Font& font = te::layout::Property::GetValueAs<Font>(pFont);

  QFont qFont = ItemUtils::convertToQfont(font);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotation;
  }

  double width = 0;
  double height = 0;

  std::vector<TextPosition>::iterator it = m_leftTexts.begin();

  const Property& pSuperscript = properties.getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);


  for( ; it != m_leftTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;

    if (useSuperScript == true && txt.size() > 2)
    {
      calculateSuperScriptText(pt, qFont, txt, iRotate);
    }
    else
    {
      this->addTextToPath(pt, qFont, txt, iRotate);
    }

#ifdef _DEBUG
    //debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

void te::layout::Grid::calculateRightTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig)
{
  const Property& pRightRotateText = properties.getProperty(settingsConfig.getRightRotateText());
  const Property& pFont = properties.getProperty(settingsConfig.getFont());

  bool bRotate = te::layout::Property::GetValueAs<bool>(pRightRotateText);
  const Font& font = te::layout::Property::GetValueAs<Font>(pFont);

  QFont qFont = ItemUtils::convertToQfont(font);

  const Property& pSuperscript = properties.getProperty(settingsConfig.getSuperscriptText());
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

  int iRotate = 0;
  if (bRotate)
  {
    iRotate = m_defaultRotation;
  }

  std::vector<TextPosition>::iterator it = m_rightTexts.begin();
  for( ; it != m_rightTexts.end() ; ++it )
  {
    std::string txt = it->first;
    QPointF pt = it->second;

    if (useSuperScript == true && txt.size() > 2)
    {
    calculateSuperScriptText(pt, qFont, txt, iRotate);
    }
    else
    {
    this->addTextToPath(pt, qFont, txt, iRotate);
    }
#ifdef _DEBUG
    //debugDrawTextRect(painter, pt, txt, qFont, iRotate);
#endif
  }
}

void te::layout::Grid::calculateTexts(const Properties& properties, const GridSettingsConfigProperties& settingsConfig)
{
  const Property& pVisibleAllTexts = properties.getProperty(settingsConfig.getVisibleAllTexts());
  const Property& pLeftText = properties.getProperty(settingsConfig.getLeftText());
  const Property& pRightText = properties.getProperty(settingsConfig.getRightText());
  const Property& pTopText = properties.getProperty(settingsConfig.getTopText());
  const Property& pBottomText = properties.getProperty(settingsConfig.getBottomText());

  bool visibleAllTexts = te::layout::Property::GetValueAs<bool>(pVisibleAllTexts);
  bool leftText = te::layout::Property::GetValueAs<bool>(pLeftText);
  bool rightText = te::layout::Property::GetValueAs<bool>(pRightText);
  bool topText = te::layout::Property::GetValueAs<bool>(pTopText);
  bool bottomText = te::layout::Property::GetValueAs<bool>(pBottomText);

  if(visibleAllTexts == false)
  {
    return;
  }

  if(leftText == true)
  {
    calculateLeftTexts(properties, settingsConfig);
  }

  if(rightText == true)
  {
    calculateRightTexts(properties, settingsConfig);
  }

  if(bottomText == true)
  {
    calculateBottomTexts(properties, settingsConfig);
  }

  if(topText == true)
  {
    calculateTopTexts(properties, settingsConfig);
  }
}

void te::layout::Grid::configPainterForGrid(QPainter* painter, const te::layout::Properties& properties, const GridSettingsConfigProperties& settingsConfig) const
{
  const Property& pLineStyle = properties.getProperty(settingsConfig.getLineStyle());
  const Property& pLineColor = properties.getProperty(settingsConfig.getLineColor());
  const Property& pLineWidth = properties.getProperty(settingsConfig.getLineWidth());

  const std::string& lineStyleName = pLineStyle.getOptionByCurrentChoice().toString();
  const te::color::RGBAColor& lineColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pLineColor);
  double lineWidth = te::layout::Property::GetValueAs<double>(pLineWidth);

  EnumLineStyleType* lineStyle = Enums::getInstance().getEnumLineStyleType();
  EnumType* currentLineStyle = Enums::getInstance().getEnumLineStyleType()->getEnum(lineStyleName);

  QPen pen = painter->pen();

  if (currentLineStyle)
  {
    if (currentLineStyle == lineStyle->getStyleSolid())
    {
      pen.setStyle(Qt::SolidLine);
    }
    else if (currentLineStyle == lineStyle->getStyleDot())
    {
      pen.setStyle(Qt::DotLine);
    }
    else if (currentLineStyle == lineStyle->getStyleDash())
    {
      pen.setStyle(Qt::DashLine);
    }
    else if (currentLineStyle == lineStyle->getStyleDashDot())
    {
      pen.setStyle(Qt::DashDotLine);
    }
    else if (currentLineStyle == lineStyle->getStyleDashDotDot())
    {
      pen.setStyle(Qt::DashDotDotLine);
    }
  }

  QColor clrLine(lineColor.getRed(), lineColor.getGreen(), lineColor.getBlue(), lineColor.getAlpha());
  pen.setColor(clrLine);
  pen.setWidthF(lineWidth);

  painter->setPen(pen);
}

void te::layout::Grid::configTextPainter(QPainter* painter, const te::layout::Properties& properties, const GridSettingsConfigProperties& settingsConfig) const
{
  const Property& pTextFontFamily = properties.getProperty(settingsConfig.getFont());
  const Property& pTextColor = properties.getProperty(settingsConfig.getTextColor());

  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);
  const te::color::RGBAColor& textColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pTextColor);

  QColor clrText(textColor.getRed(), textColor.getGreen(), textColor.getBlue(), textColor.getAlpha());

  ItemUtils::ConfigurePainterForTexts(painter, txtFont);

  QBrush brush (clrText);
  painter->setBrush(brush);
}

