#include "DataConverters.h"

#include "DataTypes.h"
#include "DataTypesUtils.h"
#include "LayoutData.h"
#include "../Utils.h"

//DataTypes
#include "../Font.h"

//Terralib
#include <terralib/color/RGBAColor.h>
#include <terralib/common/Exception.h>
#include <terralib/geometry/Envelope.h>
#include <terralib/geometry/Geometry.h>
#include "terralib/geometry/WKTReader.h"
#include "terralib/maptools/AbstractLayer.h"

// Boost
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>


te::dt::AbstractData* te::layout::ConvertBoolToString(te::dt::AbstractData* abstractData)
{
  return ConvertToStringUsingBoost<bool>(abstractData);
}

te::dt::AbstractData* te::layout::ConvertStringToBool(te::dt::AbstractData* abstractData)
{
  return ConvertFromStringUsingBoost<bool>(abstractData);
}

te::dt::AbstractData* te::layout::ConvertColorToString(te::dt::AbstractData* abstractData)
{
  const te::color::RGBAColor& color = GetValueAs<te::color::RGBAColor>(abstractData);
  std::string strColor = boost::lexical_cast<std::string>(color.getRed());
  strColor += "," + boost::lexical_cast<std::string>(color.getGreen());
  strColor += "," + boost::lexical_cast<std::string>(color.getBlue());
  strColor += "," + boost::lexical_cast<std::string>(color.getAlpha());

  return CreateData<std::string>(strColor);
}

te::dt::AbstractData* te::layout::ConvertStringToColor(te::dt::AbstractData* abstractData)
{
  const std::string& strValue = GetValueAs<std::string>(abstractData);
  std::vector<std::string> vecTokens = te::layout::Utils::Tokenize(strValue, ",");

  if (vecTokens.size() != 4)
  {
    throw te::common::Exception("ConvertStringToColor: Cannot convert from String to Color");
  }

  int r = boost::lexical_cast<int>(vecTokens[0]);
  int g = boost::lexical_cast<int>(vecTokens[1]);
  int b = boost::lexical_cast<int>(vecTokens[2]);
  int a = boost::lexical_cast<int>(vecTokens[3]);

  te::color::RGBAColor rgbaColor(r, g, b, a);
  return CreateData<te::color::RGBAColor>(rgbaColor);
}

te::dt::AbstractData* te::layout::ConvertDoubleToString(te::dt::AbstractData* abstractData)
{
  return ConvertToStringUsingBoost<double>(abstractData);
}

te::dt::AbstractData* te::layout::ConvertStringToDouble(te::dt::AbstractData* abstractData)
{
  return ConvertFromStringUsingBoost<double>(abstractData);
}

te::dt::AbstractData* te::layout::ConvertEnvelopeToString(te::dt::AbstractData* abstractData)
{
  const te::gm::Envelope& envelope = GetValueAs<te::gm::Envelope>(abstractData);

  //here we need to ensure some additional precision
  std::ostringstream convert;
  convert.precision(15);

  convert << envelope.getLowerLeftX();
  convert << "," << envelope.getLowerLeftY();
  convert << "," << envelope.getUpperRightX();
  convert << "," << envelope.getUpperRightY();
  
  return CreateData<std::string>(convert.str());
}

te::dt::AbstractData* te::layout::ConvertStringToEnvelope(te::dt::AbstractData* abstractData)
{
  const std::string& strValue = GetValueAs<std::string>(abstractData);
  std::vector<std::string> vecTokens = te::layout::Utils::Tokenize(strValue, ",");
  if (vecTokens.size() != 4)
  {
    throw te::common::Exception("ConvertStringToEnvelope: Cannot convert from String to Envelope");
  }

  double x1 = boost::lexical_cast<double>(vecTokens[0]);
  double y1 = boost::lexical_cast<double>(vecTokens[1]);
  double x2 = boost::lexical_cast<double>(vecTokens[2]);
  double y2 = boost::lexical_cast<double>(vecTokens[3]);

  te::gm::Envelope envelope(x1, y1, x2, y2);
  return CreateData<te::gm::Envelope>(envelope);
}

te::dt::AbstractData* te::layout::ConvertFontToString(te::dt::AbstractData* abstractData)
{
  const te::layout::Font& font = GetValueAs<te::layout::Font>(abstractData);

  std::string strValue = font.toString();
  return CreateData<std::string>(strValue);
}

te::dt::AbstractData* te::layout::ConvertStringToFont(te::dt::AbstractData* abstractData)
{
  const std::string& strValue = GetValueAs<std::string>(abstractData);

  te::layout::Font font;
  font.fromString(strValue);

  return CreateData<te::layout::Font>(font);
}

te::dt::AbstractData* te::layout::ConvertGeometryToString(te::dt::AbstractData* abstractData)
{
  te::gm::Geometry* geometry = GetValueAs<te::gm::Geometry*>(abstractData);
  
  std::string strValue = geometry->asText();
  return CreateData<std::string>(strValue);
}

te::dt::AbstractData* te::layout::ConvertStringToGeometry(te::dt::AbstractData* abstractData)
{
  const std::string& strValue = GetValueAs<std::string>(abstractData);

  te::gm::Geometry* geometry = te::gm::WKTReader::read(strValue.c_str());
  return CreateData<te::gm::Geometry*>(geometry);
}

te::dt::AbstractData* te::layout::ConvertIntToString(te::dt::AbstractData* abstractData)
{
  return ConvertToStringUsingBoost<int>(abstractData);
}

te::dt::AbstractData* te::layout::ConvertStringToInt(te::dt::AbstractData* abstractData)
{
  return ConvertFromStringUsingBoost<int>(abstractData);
}

te::dt::AbstractData* te::layout::ConvertStringListToString(te::dt::AbstractData* abstractData)
{
  const std::vector<std::string>& vecString = GetValueAs< std::vector<std::string> >(abstractData);

  std::string strValue;
  for (std::size_t i = 0; i < vecString.size(); ++i)
  {
    if (i > 0)
    {
      strValue += ",";
    }
    strValue += vecString[i];
  }

  return CreateData<std::string>(strValue);
}

te::dt::AbstractData* te::layout::ConvertStringToStringList(te::dt::AbstractData* abstractData)
{
  const std::string& strValue = GetValueAs<std::string>(abstractData);

  std::vector<std::string> vecTokens = te::layout::Utils::Tokenize(strValue, ",");
  return CreateData< std::vector<std::string> >(vecTokens);
}

te::dt::AbstractData* te::layout::ConvertStringMatrixToString(te::dt::AbstractData* abstractData)
{
  const std::vector< std::vector<std::string> >& stringMatrix = GetValueAs< std::vector< std::vector<std::string> > >(abstractData);

  std::string strValue;
  for (std::size_t i = 0; i < stringMatrix.size(); ++i)
  {
    if (i > 0)
    {
      strValue += ";";
    }

    for (std::size_t j = 0; j < stringMatrix[i].size(); ++j)
    {
      if (j > 0)
      {
        strValue += ",";
      }

      strValue += stringMatrix[i][j];
    }
  }

  return CreateData<std::string>(strValue);
}

te::dt::AbstractData* te::layout::ConvertStringToStringMatrix(te::dt::AbstractData* abstractData)
{
  std::vector< std::vector<std::string> > stringMatrix;

  const std::string& strValue = GetValueAs<std::string>(abstractData);

  std::vector<std::string> vecRows = te::layout::Utils::Tokenize(strValue, ";");
  for (std::size_t i = 0; i < vecRows.size(); ++i)
  {
    std::vector<std::string> vecColumns = te::layout::Utils::Tokenize(vecRows[i], ",");
    stringMatrix.push_back(vecColumns);
  }

  return CreateData< std::vector< std::vector<std::string> > >(stringMatrix);
}

te::dt::AbstractData* te::layout::ConvertAbstractLayerListToString(te::dt::AbstractData* abstractData)
{
  return CreateData<std::string>("");
}

te::dt::AbstractData* te::layout::ConvertStringToAbstractLayerList(te::dt::AbstractData* abstractData)
{
  std::list<te::map::AbstractLayerPtr> emptyList;

  return CreateData< std::list<te::map::AbstractLayerPtr> >(emptyList);
}