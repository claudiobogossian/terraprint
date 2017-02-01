#include "PlanarGrid.h"

#include "ItemUtils.h"
#include "../../core/Utils.h"
#include "../../core/WorldTransformer.h"
#include "../../core/enum/Enums.h"
#include "../../core/property/PlanarGridSettingsConfigProperties.h"
#include "../../core/property/Properties.h"

//terralib
#include <terralib/maptools/Utils.h>

#include <QFont>
#include <QString>

te::layout::PlanarGrid::PlanarGrid()
  : Grid()
{

}

te::layout::PlanarGrid::~PlanarGrid()
{

}

void te::layout::PlanarGrid::initialize(const te::layout::Properties& gridSettings)
{
  clear();

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pWorldBox = gridSettings.getProperty("world_box");
  const Property& pSrid = gridSettings.getProperty("srid");
  const Property& pWidth = gridSettings.getProperty("width");
  const Property& pHeight = gridSettings.getProperty("height");
  //const Property& pFrameThickness = gridSettings.getProperty("frame_thickness");
  const Property& pStyle = gridSettings.getProperty(settingsConfig.getStyle());
  const Property& pIsVisible = gridSettings.getProperty(settingsConfig.getVisible());

  const te::gm::Envelope& worldBox = te::layout::Property::GetValueAs<te::gm::Envelope>(pWorldBox);
  int srid = te::layout::Property::GetValueAs<int>(pSrid);
  double width = te::layout::Property::GetValueAs<double>(pWidth);
  double height = te::layout::Property::GetValueAs<double>(pHeight);
  //double frameThickness = te::layout::Property::GetValueAs<double>(pFrameThickness);
  double horizontalLineInitial = te::layout::Property::GetValueAs<double>(gridSettings.getProperty(settingsConfig.getHorizontalLineInitial()));
  double verticalLineInitial = te::layout::Property::GetValueAs<double>(gridSettings.getProperty(settingsConfig.getVerticalLineInitial()));
  double horizontalLineGap = te::layout::Property::GetValueAs<double>(gridSettings.getProperty(settingsConfig.getHorizontalLineGap()));
  double verticalLineGap = te::layout::Property::GetValueAs<double>(gridSettings.getProperty(settingsConfig.getVerticalLineGap()));
  bool isVisible = te::layout::Property::GetValueAs<bool>(pIsVisible);
  const std::string& style = pStyle.getOptionByCurrentChoice().toString();

  if (isVisible == false)
  {
    return;
  }

  if (worldBox.isValid() == false)
  {
    return;
  }

  if (width == 0 || height == 0)
  {
    return;
  }

  if (horizontalLineGap == 0 || verticalLineGap == 0)
  {
    return;
  }

  //if first prepare the world box, ensuring that it is a planar box. If it is not, we reproject it
  te::gm::Envelope planarBox = te::map::GetWorldBoxInPlanar(worldBox, srid);

  te::gm::Envelope referenceBoxMM(0, 0, width, height);

  std::vector<te::gm::LineString> verticallLines = calculateVerticalLines(planarBox, referenceBoxMM, gridSettings);
  std::vector<te::gm::LineString> horizontalLines = calculateHorizontalLines(planarBox, referenceBoxMM, gridSettings);

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

double te::layout::PlanarGrid::getUnit(const std::string& strUnit) const
{
  double unit = 1000.0;
  if (strUnit == "m")
  {
    unit = 1.0;
  }
  return unit;
}

std::vector<te::gm::LineString> te::layout::PlanarGrid::calculateVerticalLines(const te::gm::Envelope& planarBox, const te::gm::Envelope& boxMM, const Properties& gridSettings)
{
  std::vector<te::gm::LineString> verticalLines;

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pVerticalLineGap = gridSettings.getProperty(settingsConfig.getVerticalLineGap());
  const Property& pVerticalLineDisplacement = gridSettings.getProperty(settingsConfig.getVerticalLineDisplacement());
  const Property& pUnit = gridSettings.getProperty(settingsConfig.getUnit());
  const Property& pLeftRotate = gridSettings.getProperty(settingsConfig.getLeftRotateText());
  const Property& pRightRotate = gridSettings.getProperty(settingsConfig.getRightRotateText());
  const Property& pTextFontFamily = gridSettings.getProperty(settingsConfig.getFont());
  const Property& pSuperscript = gridSettings.getProperty(settingsConfig.getSuperscriptText());
  const Property& pTopRotate = gridSettings.getProperty(settingsConfig.getTopRotateText());
  const Property& pBottomRotate = gridSettings.getProperty(settingsConfig.getBottomRotateText());

  double verticalLineGap = te::layout::Property::GetValueAs<double>(pVerticalLineGap);
  double verticalLineDisplacement = te::layout::Property::GetValueAs<double>(pVerticalLineDisplacement);
  const string& strUnit = pUnit.getOptionByCurrentChoice().toString();
  double unitV = getUnit(strUnit);
  bool bLeftRotate = te::layout::Property::GetValueAs<bool>(pLeftRotate);
  bool bRightRotate = te::layout::Property::GetValueAs<bool>(pRightRotate);
  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);
  bool bTopRotate = te::layout::Property::GetValueAs<bool>(pTopRotate);
  bool bBottomRotate = te::layout::Property::GetValueAs<bool>(pBottomRotate);

  //here we check if the gap count would exceed the maximum number of gaps
  if (planarBox.getWidth() / verticalLineGap > m_maxGaps)
  {
    return verticalLines;
  }

  //we define the transformation from planar CS to millimeters CS
  WorldTransformer transf = te::layout::Utils::getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  double x1 = initVerticalLines(planarBox, gridSettings, settingsConfig);

  QFont ft = ItemUtils::convertToQfont(txtFont);

  for (; x1 <= planarBox.getUpperRightX(); x1 += verticalLineGap)
  {
    if (x1 < planarBox.getLowerLeftX())
      continue;

    double lly = 0;
    double ury = 0;
    double x = 0;
    transf.system1Tosystem2(x1, planarBox.getLowerLeftY(), x, lly);
    transf.system1Tosystem2(x1, planarBox.getUpperRightY(), x, ury);

    QLineF line(x, lly, x, ury);

    te::gm::LineString lineString(2, te::gm::LineStringType);
    lineString.setPoint(0, x, lly);
    lineString.setPoint(1, x, ury);

    verticalLines.push_back(lineString);

    double number = x1 / unitV;
    QString convert = QString::number(number, 'f', 0);

    std::string text = convert.toStdString();
    QPainterPath textObject;

    if (useSuperScript) 
    {
      std::vector<QString> textVect;
      std::vector<QFont> fontVect;

      QFont fontSScript2 = ft;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      fontVect.push_back(fontSScript2);
      fontVect.push_back(ft);

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

      textObject = ItemUtils::superscriptTextToVector(textVect, fontVect, QPointF());
    }
    else 
    {
      textObject = ItemUtils::textToVector(convert, ft, QPointF(), 0);
    }

    QRectF rectF(textObject.boundingRect());

    calculateTop(line.p2(), rectF, convert, bTopRotate, verticalLineDisplacement);
    calculateBottom(line.p1(), rectF, convert, bBottomRotate, verticalLineDisplacement);
  }

  return verticalLines;
}

std::vector<te::gm::LineString> te::layout::PlanarGrid::calculateHorizontalLines(const te::gm::Envelope& planarBox, const te::gm::Envelope& boxMM, const Properties& gridSettings)
{
  std::vector<te::gm::LineString> horizontalLines;

  PlanarGridSettingsConfigProperties settingsConfig;

  const Property& pHorizontalLineGap = gridSettings.getProperty(settingsConfig.getHorizontalLineGap());
  const Property& pHorizontalLineDisplacement = gridSettings.getProperty(settingsConfig.getHorizontalLineDisplacement());
  const Property& pUnit = gridSettings.getProperty(settingsConfig.getUnit());
  const Property& pLeftRotate = gridSettings.getProperty(settingsConfig.getLeftRotateText());
  const Property& pRightRotate = gridSettings.getProperty(settingsConfig.getRightRotateText());
  const Property& pTextFontFamily = gridSettings.getProperty(settingsConfig.getFont());
  const Property& pSuperscript = gridSettings.getProperty(settingsConfig.getSuperscriptText());

  double horizontalLineGap = te::layout::Property::GetValueAs<double>(pHorizontalLineGap);
  double horizontalLineDisplacement = te::layout::Property::GetValueAs<double>(pHorizontalLineDisplacement);
  const string& strUnit = pUnit.getOptionByCurrentChoice().toString();
  double unitV = getUnit(strUnit);
  bool bLeftRotate = te::layout::Property::GetValueAs<bool>(pLeftRotate);
  bool bRightRotate = te::layout::Property::GetValueAs<bool>(pRightRotate);
  Font txtFont = te::layout::Property::GetValueAs<Font>(pTextFontFamily);
  bool useSuperScript = te::layout::Property::GetValueAs<bool>(pSuperscript);

  //here we check if the gap count would exceed the maximum number of gaps
  if (planarBox.getHeight() / horizontalLineGap > m_maxGaps)
  {
    return horizontalLines;
  }

  //we define the transformation from planar CS to millimeters CS
  WorldTransformer transf = te::layout::Utils::getTransformGeo(planarBox, boxMM);
  transf.setMirroring(false);

  double y1 = initHorizontalLines(planarBox, gridSettings, settingsConfig);

  QFont ft = ItemUtils::convertToQfont(txtFont);
  for (; y1 <= planarBox.getUpperRightY(); y1 += horizontalLineGap)
  {
    if (y1 < planarBox.getLowerLeftY())
      continue;

    double llx = 0;
    double urx = 0;
    double y = 0;
    transf.system1Tosystem2(planarBox.getLowerLeftX(), y1, llx, y);
    transf.system1Tosystem2(planarBox.getUpperRightX(), y1, urx, y);

    QLineF line(llx, y, urx, y);

    te::gm::LineString lineString(2, te::gm::LineStringType);
    lineString.setPoint(0, llx, y);
    lineString.setPoint(1, urx, y);

    horizontalLines.push_back(lineString);

    double number = y1 / unitV;
    QString convert = QString::number(number, 'f', 0);

    std::string text = convert.toStdString();

    QPainterPath textObject;
    if (useSuperScript) 
    {
      std::vector<QString> textVect;
      std::vector<QFont> fontVect;

      QFont fontSScript2 = ft;
      fontSScript2.setPointSize(fontSScript2.pointSize() / 2);

      fontVect.push_back(fontSScript2);
      fontVect.push_back(ft);

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

      textObject = ItemUtils::superscriptTextToVector(textVect, fontVect, QPointF());
    }
    else {
      textObject = ItemUtils::textToVector(convert, ft, QPointF(), 0);
    }

    QRectF rectF(textObject.boundingRect());

    calculateLeft(line.p1(), rectF, convert, bLeftRotate, horizontalLineDisplacement);
    calculateRight(line.p2(), rectF, convert, bRightRotate, horizontalLineDisplacement);
  }

  return horizontalLines;
}
