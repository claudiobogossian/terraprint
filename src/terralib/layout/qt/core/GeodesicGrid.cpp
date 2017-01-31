#include "GeodesicGrid.h"

#include "ItemUtils.h"
#include "../../core/Utils.h"
#include "../../core/WorldTransformer.h"
#include "../../core/enum/Enums.h"
#include "../../core/enum/EnumTextFormatType.h"
#include "../../core/property/GeodesicGridSettingsConfigProperties.h"
#include "../../core/property/Properties.h"

//terralib
#include <terralib/common/StringUtils.h>
#include <terralib/common/UnitOfMeasure.h>
#include <terralib/geometry/Utils.h>
#include <terralib/maptools/Utils.h>
#include <terralib/srs/SpatialReferenceSystemManager.h>

#include <QFont>
#include <QString>

te::layout::GeodesicGrid::GeodesicGrid()
  : Grid()
{

}

te::layout::GeodesicGrid::~GeodesicGrid()
{

}

void te::layout::GeodesicGrid::initialize(const te::layout::Properties& gridSettings)
{
  clear();

  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pWorldBox = gridSettings.getProperty("world_box");
  const Property& pSrid = gridSettings.getProperty("srid");
  const Property& pWidth = gridSettings.getProperty("width");
  const Property& pHeight = gridSettings.getProperty("height");
  const Property& pStyle = gridSettings.getProperty(settingsConfig.getStyle());
  const Property& pFrameThickness = gridSettings.getProperty("frame_thickness");
  const Property& pIsVisible = gridSettings.getProperty(settingsConfig.getVisible());

  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  int srid = te::layout::Property::GetValueAs<int>(pSrid);
  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);
  const std::string& style = pStyle.getOptionByCurrentChoice().toString();
  double frameThickness = te::layout::Property::GetValueAs<double>(pFrameThickness);
  bool isVisible = te::layout::Property::GetValueAs<bool>(pIsVisible);

  if (isVisible == false)
  {
    return;
  }


  //if first prepare the world box, ensuring that it is a geographic box. If it is not, we reproject it
  te::gm::Envelope geographicBox = Utils::GetWorldBoxInGeographic(worldBox, srid);

  // Box necessario para desenhar a curvatura
  te::gm::Envelope planarBox = geographicBox;
  int zone = te::map::CalculatePlanarZone(geographicBox);
  if (zone < 0 || zone > 60)
  {
    return;
  }

  if (width == 0 || height == 0)
  {
    return;
  }

  Utils::remapToPlanar(&planarBox, zone);

  te::gm::Envelope referenceBoxMM(0, 0, width, height);
  
  std::vector<te::gm::LineString> verticallLines = calculateVerticalLines(planarBox, geographicBox, referenceBoxMM, gridSettings);
  std::vector<te::gm::LineString> horizontalLines = calculateHorizontalLines(planarBox, geographicBox, referenceBoxMM, gridSettings);

  EnumType* currentStyle = Enums::getInstance().getEnumGridStyleType()->getEnum(style);
  EnumGridStyleType gridStyleType;

  if (currentStyle)
  {
    if (currentStyle->getName() == gridStyleType.getStyleCross()->getName())
    {
      m_gridLines = this->convertCrossesToPath(verticallLines, horizontalLines, gridSettings, settingsConfig);
    }

    if (currentStyle->getName() == gridStyleType.getStyleContinuous()->getName())
    {
      m_gridLines = convertLinesToPath(verticallLines, horizontalLines);
    }
  }

  calculateTexts(gridSettings);

  QRectF rectGrid = m_gridLines.boundingRect();
  QRectF rectTexts = m_gridText.boundingRect();

  QRectF recfFull = rectGrid.unite(rectTexts);

  QPointF origin = recfFull.topLeft();
  m_gridLines.translate(-origin.x(), -origin.y());
  m_gridText.translate(-origin.x(), -origin.y());

  rectGrid = m_gridLines.boundingRect();
  rectTexts = m_gridText.boundingRect();

  recfFull = rectGrid.unite(rectTexts);

  m_size = recfFull.size();
  m_origin = rectGrid.topLeft();
}

std::vector<te::gm::LineString> te::layout::GeodesicGrid::calculateVerticalLines(const te::gm::Envelope& planarBox, const te::gm::Envelope& geographicBox, const te::gm::Envelope& boxMM, const Properties& gridSettings)
{
  std::vector<te::gm::LineString> verticalLines;

  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pHorizontalGap = gridSettings.getProperty(settingsConfig.getHorizontalLineGap());
  const Property& pVerticalGap = gridSettings.getProperty(settingsConfig.getVerticalLineGap());
  const Property& pShowDegreesText = gridSettings.getProperty(settingsConfig.getDegreesText());
  const Property& pShowMinutesText = gridSettings.getProperty(settingsConfig.getMinutesText());
  const Property& pShowSecondsText = gridSettings.getProperty(settingsConfig.getSecondsText());
  const Property& pTextFormat = gridSettings.getProperty(settingsConfig.getTextFormat());
  const Property& pVerticalDisplacement = gridSettings.getProperty(settingsConfig.getVerticalLineDisplacement());
  const Property& pHorizontalDisplacement = gridSettings.getProperty(settingsConfig.getHorizontalLineDisplacement());
  const Property& pTopRotate = gridSettings.getProperty(settingsConfig.getTopRotateText());
  const Property& pBottomRotate = gridSettings.getProperty(settingsConfig.getBottomRotateText());
  const Property& pTextFontFamily = gridSettings.getProperty(settingsConfig.getFont());
  const Property& pSecPrecisionText = gridSettings.getProperty(settingsConfig.getSecondsPrecisionText());

  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);
  //  std::string fontFamily = pFontFamily.getValue().toString();
  //  int textPointSize = pTextPointSize.getValue().toInt();
  double horizontalLineGap = te::layout::Property::GetValueAs<double>(pHorizontalGap);
  double verticalLineGap = te::layout::Property::GetValueAs<double>(pVerticalGap);
  bool showDegreesText = te::layout::Property::GetValueAs<bool>(pShowDegreesText);
  bool showMinutesText = te::layout::Property::GetValueAs<bool>(pShowMinutesText);
  bool showSecondsText = te::layout::Property::GetValueAs<bool>(pShowSecondsText);
  std::string textFormat = pTextFormat.getOptionByCurrentChoice().toString();
  double verticalDisplacement = te::layout::Property::GetValueAs<double>(pVerticalDisplacement);
  double horizontalDisplacement = te::layout::Property::GetValueAs<double>(pHorizontalDisplacement);
  bool bTopRotate = te::layout::Property::GetValueAs<bool>(pTopRotate);
  bool bBottomRotate = te::layout::Property::GetValueAs<bool>(pBottomRotate);

  int secPrecision = te::layout::Property::GetValueAs<int>(pSecPrecisionText);

  // Draw a vertical line and the x coordinate change(horizontal)
  WorldTransformer transf = Utils::getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  int zone = te::map::CalculatePlanarZone(geographicBox);
  if (zone < 0 || zone > 60)
  {
    return verticalLines;
  }

  //here we check if the gap count would exceed the maximum number of gaps
  if (geographicBox.getWidth() / verticalLineGap > m_maxGaps)
  {
    return verticalLines;
  }

  std::string fontName = txtFont.getFamily();
  QString qFontName = ItemUtils::convert2QString(fontName);

  QFont ft(qFontName, txtFont.getPointSize());

  //as the geographic grid comes from the result of a reprojection, the geographic box sometimes can be smaller than the planar box, maninly when the distance from equator gets higher.
  //when this happens, these calculated lines do not reach the boundaries of the item, and consequently the texts are not drawn in the correct position
  //to fix this, we calculate a grid bigger than we need, and then we clip the lines by making an intertection operation with the item
  double x1 = initVerticalLines(geographicBox, gridSettings, settingsConfig);
  double x2 = geographicBox.getUpperRightX();

  x1 -= 3 * verticalLineGap;
  x2 += 3 * verticalLineGap;

  for (; x1 <= x2; x1 += verticalLineGap)
  {
    te::gm::Envelope env(x1, geographicBox.getLowerLeftY() - 3 * horizontalLineGap, x1, geographicBox.getUpperRightY() + 3 * horizontalLineGap);
    te::gm::LinearRing* line = 0;
    line = Utils::addCoordsInY(env, x1, verticalLineGap);

    // Line curvature: of latlong to planar;
    // Draw line: planar to mm
    Utils::remapToPlanar(line, zone);
    Utils::convertToMillimeter(transf, line);

    //here we clip the line using the boundaries of the item in MM
    te::gm::Geometry* rectPolygon = te::gm::GetGeomFromEnvelope(&boxMM, line->getSRID());
    te::gm::Geometry* clippedGeometry = line->intersection(rectPolygon);
    if (clippedGeometry == 0 || clippedGeometry->isValid() == false)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    te::gm::LineString* lineString = dynamic_cast<te::gm::LineString*>(clippedGeometry);
    if (lineString == 0)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    size_t numCoords = lineString->size();
    te::gm::Point* firstPoint = lineString->getPointN(0);
    te::gm::Point* lastPoint = lineString->getPointN(numCoords - 1);

    te::gm::LineString topBorderLine(2, te::gm::LineStringType);
    topBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getUpperRightY());
    topBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getUpperRightY());

    te::gm::LineString bottomBorderLine(2, te::gm::LineStringType);
    bottomBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getLowerLeftY());
    bottomBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getLowerLeftY());

    verticalLines.push_back(te::gm::LineString(*lineString));

    EnumTextFormatType textFormatEnum;
    EnumType* currentTextFormat = textFormatEnum.getEnum(textFormat);

    std::string text = "";

    if (currentTextFormat)
    {
      if (currentTextFormat->getName() == textFormatEnum.getDefaultFormat()->getName())
      {
        text = Utils::convertDecimalToDegree(x1, showDegreesText, showMinutesText, showSecondsText, secPrecision);
      }
      else if (currentTextFormat->getName() == textFormatEnum.getANPFormat()->getName())
      {
        text = Utils::convertDecimalToDegreeANP(x1, showDegreesText, showMinutesText, showSecondsText, secPrecision);
      }
    }

    QString qText = ItemUtils::convert2QString(text);

    QPainterPath textObject = ItemUtils::textToVector(qText, ft, QPointF(), 0);
    QRectF rectF(textObject.boundingRect());

    //as the grid lines an be curved, texts must only de drawn in the cases that the grid line reaches the top or the botton of the item bounding rect
    if (lineString->intersects(&bottomBorderLine) == true)
    {
      calculateBottom(QPointF(firstPoint->getX(), firstPoint->getY()), rectF, qText, bBottomRotate, horizontalDisplacement);
    }
    if (lineString->intersects(&topBorderLine) == true)
    {
      calculateTop(QPointF(lastPoint->getX(), lastPoint->getY()), rectF, qText, bTopRotate, horizontalDisplacement);
    }

    if (line)
    {
      delete line;
      line = 0;
    }

    delete rectPolygon;
    delete clippedGeometry;
  }

  return verticalLines;
}

std::vector<te::gm::LineString> te::layout::GeodesicGrid::calculateHorizontalLines(const te::gm::Envelope& planarBox, const te::gm::Envelope& geographicBox, const te::gm::Envelope& boxMM, const Properties& gridSettings)
{
  std::vector<te::gm::LineString> horizontalLines;

  GeodesicGridSettingsConfigProperties settingsConfig;

  const Property& pTextFontFamily = gridSettings.getProperty(settingsConfig.getFont());

  const Property& pVerticalLineGap = gridSettings.getProperty(settingsConfig.getVerticalLineGap());
  const Property& pHorizontalLineGap = gridSettings.getProperty(settingsConfig.getHorizontalLineGap());
  const Property& pShowDegreesText = gridSettings.getProperty(settingsConfig.getDegreesText());
  const Property& pShowMinutesText = gridSettings.getProperty(settingsConfig.getMinutesText());
  const Property& pShowSecondsText = gridSettings.getProperty(settingsConfig.getSecondsText());
  const Property& pTextFormat = gridSettings.getProperty(settingsConfig.getTextFormat());
  const Property& pHorizontalDisplacement = gridSettings.getProperty(settingsConfig.getHorizontalLineDisplacement());
  const Property& pVerticalDisplacement = gridSettings.getProperty(settingsConfig.getVerticalLineDisplacement());
  const Property& pLeftRotate = gridSettings.getProperty(settingsConfig.getLeftRotateText());
  const Property& pRightRotate = gridSettings.getProperty(settingsConfig.getRightRotateText());

  const Property& pSecPrecisionText = gridSettings.getProperty(settingsConfig.getSecondsPrecisionText());

  double verticalLineGap = te::layout::Property::GetValueAs<double>(pVerticalLineGap);
  double horizontalLineGap = te::layout::Property::GetValueAs<double>(pHorizontalLineGap);
  bool showDegreesText = te::layout::Property::GetValueAs<bool>(pShowDegreesText);
  bool showMinutesText = te::layout::Property::GetValueAs<bool>(pShowMinutesText);
  bool showSecondsText = te::layout::Property::GetValueAs<bool>(pShowSecondsText);
  std::string textFormat = pTextFormat.getOptionByCurrentChoice().toString();
  double horizontalDisplacement = te::layout::Property::GetValueAs<double>(pHorizontalDisplacement);
  double verticalDisplacement = te::layout::Property::GetValueAs<double>(pVerticalDisplacement);
  bool bLeftRotate = te::layout::Property::GetValueAs<bool>(pLeftRotate);
  bool bRightRotate = te::layout::Property::GetValueAs<bool>(pRightRotate);
  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);

  int secPrecision = te::layout::Property::GetValueAs<int>(pSecPrecisionText);

  // Draw a horizontal line and the y coordinate change(vertical)
  WorldTransformer transf = Utils::getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  int zone = te::map::CalculatePlanarZone(geographicBox);
  if (zone < 0 || zone > 60)
  {
    return horizontalLines;
  }

  std::string fontName = txtFont.getFamily();
  QString qFontName = ItemUtils::convert2QString(fontName);

  QFont ft(qFontName, txtFont.getPointSize());

  double y1 = initVerticalLines(geographicBox, gridSettings, settingsConfig);
  double y2 = geographicBox.getUpperRightY();

  y1 -= 3 * horizontalLineGap;
  y2 += 3 * horizontalLineGap;

  for (; y1 <= y2; y1 += horizontalLineGap)
  {
    te::gm::Envelope env(geographicBox.getLowerLeftX() - 3 * verticalLineGap, y1, geographicBox.getUpperRightX() + 3 * verticalLineGap, y1);

    te::gm::LinearRing* line = 0;
    line = Utils::addCoordsInX(env, y1, horizontalLineGap);

    // Line curvature: of latlong to planar;
    // Draw line: planar to mm
    Utils::remapToPlanar(line, zone);
    Utils::convertToMillimeter(transf, line);

    //here we clip the line using the boundaries of the item in MM
    te::gm::Geometry* rectPolygon = te::gm::GetGeomFromEnvelope(&boxMM, line->getSRID());
    te::gm::Geometry* clippedGeometry = line->intersection(rectPolygon);
    if (clippedGeometry == 0 || clippedGeometry->isValid() == false)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    te::gm::LineString* lineString = dynamic_cast<te::gm::LineString*>(clippedGeometry);
    if (lineString == 0)
    {
      delete rectPolygon;
      delete clippedGeometry;
      continue;
    }

    size_t numCoords = lineString->size();
    te::gm::Point* firstPoint = lineString->getPointN(0);
    te::gm::Point* lastPoint = lineString->getPointN(numCoords - 1);

    te::gm::LineString leftBorderLine(2, te::gm::LineStringType);
    leftBorderLine.setPoint(0, boxMM.getLowerLeftX(), boxMM.getLowerLeftY());
    leftBorderLine.setPoint(1, boxMM.getLowerLeftX(), boxMM.getUpperRightY());

    te::gm::LineString rightBorderLine(2, te::gm::LineStringType);
    rightBorderLine.setPoint(0, boxMM.getUpperRightX(), boxMM.getLowerLeftY());
    rightBorderLine.setPoint(1, boxMM.getUpperRightX(), boxMM.getUpperRightY());

    horizontalLines.push_back(te::gm::LineString(*lineString));

    EnumTextFormatType textFormatEnum;
    EnumType* currentTextFormat = textFormatEnum.getEnum(textFormat);

    std::string text = "";

    if (currentTextFormat)
    {
      if (currentTextFormat->getName() == textFormatEnum.getDefaultFormat()->getName())
      {
        text = Utils::convertDecimalToDegree(y1, showDegreesText, showMinutesText, showSecondsText, secPrecision);
      }
      else if (currentTextFormat->getName() == textFormatEnum.getANPFormat()->getName())
      {
        text = Utils::convertDecimalToDegreeANP(y1, showDegreesText, showMinutesText, showSecondsText, secPrecision);
      }
    }

    QString qText = ItemUtils::convert2QString(text);

    QPainterPath textObject = ItemUtils::textToVector(qText, ft, QPointF(), 0);

    QRectF rectF(textObject.boundingRect());


    //as the grid lines an be curved, texts must only de drawn in the cases that the grid line reaches the top or the botton of the item bounding rect
    if (lineString->intersects(&leftBorderLine) == true)
    {
      calculateLeft(QPointF(firstPoint->getX(), firstPoint->getY()), rectF, qText, bLeftRotate, verticalDisplacement);
    }
    if (lineString->intersects(&rightBorderLine) == true)
    {
      calculateRight(QPointF(lastPoint->getX(), lastPoint->getY()), rectF, qText, bRightRotate, verticalDisplacement);
    }

    if (line)
    {
      delete line;
      line = 0;
    }
  }

  return horizontalLines;
}
