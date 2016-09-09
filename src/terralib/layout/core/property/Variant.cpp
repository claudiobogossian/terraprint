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
  \file Variant.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "Variant.h"
#include "../enum/Enums.h"
#include <terralib/common/StringUtils.h>
#include "terralib/geometry/WKTReader.h"
#include "terralib/geometry/WKTWriter.h"
#include "../../qt/core/ItemUtils.h"

//Boost
#include <boost/tokenizer.hpp>

// STL
#include <sstream>
#include <string>  
#include <exception>
#include <stdexcept>
#include <cctype>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

// Qt
#include <QColor>
#include <QFont>
#include <QVector>
#include <QVariant>

te::layout::Variant::Variant() :
  m_sValue("unknown"),
  m_dValue(-1000.),
  m_iValue(-1000),
  m_lValue(-1000),
  m_fValue(-1000.),
  m_bValue(false),
  m_type(0),
  m_null(true),
  m_complex(false),
  m_usePrecision(false),
  m_precision(0)
{
  m_type = Enums::getInstance().getEnumDataType()->getDataTypeNone();
}

te::layout::Variant::Variant(te::layout::EnumType* type, const void* valueCopy) :
  m_sValue("unknown"),
  m_dValue(-1000.),
  m_iValue(-1000),
  m_lValue(-1000),
  m_fValue(-1000.),
  m_bValue(false),
  m_type(type),
  m_null(true),
  m_complex(false),
  m_usePrecision(false),
  m_precision(2)
{
  if(valueCopy)
  {
    convertValue(valueCopy);
  }
}

te::layout::Variant::~Variant()
{
  
}

te::layout::EnumType* te::layout::Variant::getType() const
{
  return m_type;
}

void te::layout::Variant::convertValue( const void* valueCopy )
{
  void* value = const_cast<void*>(valueCopy);
  bool null = true;

  //init variables
  std::string* sp = 0;
  double* dValue = 0;
  float* fValue = 0;
  long* lValue = 0;
  int* iValue = 0;
  bool* bValue = 0;
  te::color::RGBAColor* colorValue = 0;
  Font* fontValue = 0;
  te::gm::Envelope* envelopeValue = 0;
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!m_type || !dataType)
  {
    return;
  }

  try
  {
    if(m_type == dataType->getDataTypeString())
    {
      // Cast it back to a string pointer.
      sp = static_cast<std::string*>(value);
      if(sp)
      {
        null = false;
        m_sValue = *sp;
      }
    }
    else if(m_type == dataType->getDataTypeStringList())
    {
      // Cast it back to a string pointer.
      sp = static_cast<std::string*>(value);
      if(sp)
      {
        null = false;
        m_sValue = *sp;
      }
    }
   else if(m_type == dataType->getDataTypeDouble())
   {
      dValue = static_cast<double*>(value);
      if(dValue)
      {
        null = false;
        m_dValue = *dValue;
      }      
   }
   else if(m_type == dataType->getDataTypeFloat())
   {
      fValue = static_cast<float*>(value);
      if(fValue)
      {
        null = false;
        m_fValue = *fValue;
      }
   }
   else if(m_type == dataType->getDataTypeLong())
   {
      lValue = static_cast<long*>(value);
      if(lValue)
      {
        null = false;
        m_lValue = *lValue;
      }
      
   }
   else if(m_type == dataType->getDataTypeInt())
   {
      iValue = static_cast<int*>(value);
      if(iValue)
      {
        null = false;
        m_iValue = *iValue;
      }
   }
   else if(m_type == dataType->getDataTypeBool())
   {
      bValue = static_cast<bool*>(value);
      if(bValue)
      {
        null = false;
        m_bValue = *bValue;
      }
   }
   else if(m_type == dataType->getDataTypeColor())
   {
      // Cast it back to a string pointer.
      colorValue = static_cast<te::color::RGBAColor*>(value);
      if(colorValue)
      {
        null = false;
        m_colorValue = *colorValue;
        m_complex = true;
      }
   }
   else if(m_type == dataType->getDataTypeFont())
   {
      // Cast it back to a string pointer.
      fontValue = static_cast<Font*>(value);
      if(fontValue)
      {
        null = false;
        m_fontValue = *fontValue;
        m_complex = true;
      }
   }
   else if(m_type == dataType->getDataTypeEnvelope())
   {
      // Cast it back to a string pointer.
      envelopeValue = static_cast<te::gm::Envelope*>(value);
      if(envelopeValue)
      {
        null = false;
        m_envelopeValue = *envelopeValue;
        m_complex = true;
      }
   }
   else if(m_type == dataType->getDataTypeGeometry())
   {
     // Cast it back to a shared pointer of a te::gm::Geometry.
     te::gm::GeometryShrPtr* geoPtrRef = static_cast<te::gm::GeometryShrPtr*>(value);
     if(geoPtrRef)
     {
       null = false;
       m_geometryPtr = *geoPtrRef;
       m_complex = true;
     }
   }
   else if (m_type == dataType->getDataTypeStringVector())
   {
     // Cast it back to a vector of string..
     std::vector<std::string>*  vstringValue = static_cast<std::vector<std::string>* >(value); //Attempting to convert *void to vector
     if (vstringValue)
     {
       null = false;
       m_vString = *vstringValue;
       m_complex = true;
     }
   }
   else if (m_type == dataType->getDataTypeStringMatrix())
   {
     // Cast it back to a vector of string..
     std::vector< std::vector<std::string> >*  vstringValue = static_cast<std::vector< std::vector<std::string> >* >(value); //Attempting to convert *void to vector
     if (vstringValue)
     {
       null = false;
       m_stringMatrix = *vstringValue;
       m_complex = true;
     }
   }
   else if (m_type == dataType->getDataTypeLayerList())
   {
     // Cast it back to a list of te::map::AbstractLayerPtr.
     std::list<te::map::AbstractLayerPtr>* listLayer = static_cast<std::list<te::map::AbstractLayerPtr>* >(value); //Attempting to convert *void to list
     if (listLayer)
     {
       null = false;
       m_listLayer = *listLayer;
       m_complex = true;
     }
   }
   else // Any remaining data will be by default "std::string"  
   {
     // Cast it back to a string pointer.
     sp = static_cast<std::string*>(value);
     if(sp)
     {
       null = false;
       m_sValue = *sp;
     }
   }
  }
  catch (std::exception const& e)
  {
    std::string s_type = m_type->getName();
    std::cerr << e.what() << "Failed - te::layout::Variant: convert to " << s_type << std::endl;
  }

  m_null = null;
}

void te::layout::Variant::fromString(const std::string& value, EnumType* type)
{
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  bool null = true;

  if(!dataType)
  {
    return;
  }

  if(!type)
  {
    return;
  }

  m_type = type;

  /* the ptree boost returns data with string type */

  try
  {
    if (type == dataType->getDataTypeString() || type == dataType->getDataTypeStringList())
    {
      m_sValue = value;
      null = false;
    }
    else if(type == dataType->getDataTypeDouble())
    {
      m_dValue = std::atof(value.c_str());
      null = false;
    }
    else if(type == dataType->getDataTypeInt())
    {
      m_iValue = std::atoi(value.c_str());
      null = false;
    }
    else if(type == dataType->getDataTypeLong())
    {
      m_lValue = std::atol(value.c_str());
      null = false;
    }
    else if(type == dataType->getDataTypeFloat())
    {
      m_fValue = (float)std::atof(value.c_str());
      null = false;
    }
    else if(type == dataType->getDataTypeBool())
    {
      m_bValue = toBool(value);
      null = false;
    }
    else if(type == dataType->getDataTypeColor())
    {
      std::string color = value;

      std::vector<std::string> strings;
      std::istringstream f(color);
      std::string s;    
      while (std::getline(f, s, ',')) 
      {
        strings.push_back(s);
      }

      if(strings.empty() || strings.size() > 4)
        return;

      int r = std::atoi(strings[0].c_str());
      int g = std::atoi(strings[1].c_str());
      int b = std::atoi(strings[2].c_str());
      int a = std::atoi(strings[3].c_str());

      m_colorValue.setColor(r,g,b,a);

      m_complex = true;
      null = false;
    }
    else if(type == dataType->getDataTypeFont())
    {
      std::string font = value;
      m_fontValue.fromString(font);
      m_complex = true;
      null = false;
    }
    else if(type == dataType->getDataTypeEnvelope())
    {
      std::string color = value;

      std::vector<std::string> strings;
      std::istringstream f(color);
      std::string s;    
      while (std::getline(f, s, ',')) 
      {
        strings.push_back(s);
      }

      if(strings.empty() || strings.size() > 4)
        return;

      double x1 = std::atof(strings[0].c_str());
      double y1 = std::atof(strings[1].c_str());
      double x2 = std::atof(strings[2].c_str());
      double y2 = std::atof(strings[3].c_str());

      m_envelopeValue.init(x1, y1, x2, y2);

      m_complex = true;
      null = false;
    }
    else if (type == dataType->getDataTypeGeometry())
    {
      te::gm::Geometry* geometry = te::gm::WKTReader::read(value.c_str());
      m_geometryPtr.reset(geometry);

      m_complex = true;
      null = false;
    }
    else if (type == dataType->getDataTypeStringVector())
    {
      std::string vString = value;
      std::istringstream f(vString);
      std::string s;
      while (std::getline(f, s, ','))
      {
        m_vString.push_back(s);
        null = false;
      }
    }
    else if (m_type == dataType->getDataTypeStringMatrix())
    {
      typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
      boost::char_separator<char> separatorRow{";"};
      tokenizer tokenizerRows{ value, separatorRow};
      for (tokenizer::iterator itRow = tokenizerRows.begin(); itRow != tokenizerRows.end(); ++itRow)
      {
        std::string rowValue = *itRow;

        std::vector<std::string> vecRow;

        boost::char_separator<char> separatorColumn{ ",", 0, boost::keep_empty_tokens };
        tokenizer tokenizerColumns{ rowValue, separatorColumn };
        for (tokenizer::iterator itColumn = tokenizerColumns.begin(); itColumn != tokenizerColumns.end(); ++itColumn)
        {
          std::string columnValue = *itColumn;
          vecRow.push_back(columnValue);
        }

        m_stringMatrix.push_back(vecRow);
      }
      null = false;
    }
    else if (type == dataType->getDataTypeLayerList())
    {
      // do nothing
    }
    else // Any remaining data will be by default "std::string"  
    {
      m_sValue = value;
      null = false;
    }
  }
  catch (std::exception const& e)
  {
    std::string s_type = type->getName();
    std::cerr << e.what() << "Failed - te::layout::Variant: convert to " << s_type << std::endl;
  }

  m_null = null;
}

const std::string& te::layout::Variant::toString() const
{
  return m_sValue;
}

double te::layout::Variant::toDouble() const
{
  return m_dValue;
}

int te::layout::Variant::toInt() const
{
  return m_iValue;
}

long te::layout::Variant::toLong() const
{
  return m_lValue;
}

float te::layout::Variant::toFloat() const
{
  return m_fValue;
}

bool te::layout::Variant::toBool() const
{
  return m_bValue;
}

const te::color::RGBAColor& te::layout::Variant::toColor() const
{
  return m_colorValue;
}

const te::layout::Font& te::layout::Variant::toFont() const
{
  return m_fontValue;
}

const te::gm::Envelope& te::layout::Variant::toEnvelope() const
{
  return m_envelopeValue;
}

const std::list<te::map::AbstractLayerPtr> te::layout::Variant::toLayerList() const
{
  return m_listLayer;
}

const std::vector<std::string>& te::layout::Variant::toStringVector() const
{
  return m_vString;
}

const std::vector< std::vector<std::string> >& te::layout::Variant::toStringMatrix() const
{
  return m_stringMatrix;
}

bool te::layout::Variant::isNull() const
{
  return m_null;
}

void te::layout::Variant::clear()
{
  m_sValue = "unknown";
  m_dValue = -1000.;
  m_iValue = -1000;
  m_lValue = -1000;
  m_fValue = -1000.;
  m_bValue = false;
  m_type = Enums::getInstance().getEnumDataType()->getDataTypeNone();
  m_null = true;
  //m_generic.clear();
  m_geometryPtr.reset();
}

std::string te::layout::Variant::convertToString() const
{
  std::ostringstream convert;
  convert.precision(15);

  std::stringstream ss;//create a stringstream
  std::string s_convert;
  
  if(m_null)
    return s_convert;

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(m_type == dataType->getDataTypeNone())
    return s_convert;

  if (m_type == dataType->getDataTypeString() || m_type == dataType->getDataTypeStringList())
  {
    s_convert = m_sValue;
  }
  else if(m_type == dataType->getDataTypeDouble())
  {
    convert << m_dValue; //add number to the stream
    s_convert = convert.str();
  }
  else if(m_type == dataType->getDataTypeInt())
  {
    ss << m_iValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_type == dataType->getDataTypeLong())
  {
    ss << m_lValue;//add number to the stream
    s_convert = ss.str();
  }
  else if(m_type == dataType->getDataTypeFloat())
  {
    convert << m_fValue; //add number to the stream
    s_convert = convert.str();
  }
  else if(m_type == dataType->getDataTypeColor())
  {
    s_convert = toString(m_colorValue.getRed());
    s_convert += "," + toString(m_colorValue.getGreen());
    s_convert += "," + toString(m_colorValue.getBlue());
    s_convert += "," + toString(m_colorValue.getAlpha());
  }
  else if(m_type == dataType->getDataTypeFont())
  {
    s_convert = m_fontValue.toString();
  }
  else if(m_type == dataType->getDataTypeEnvelope())
  {
    s_convert = toString(m_envelopeValue.getLowerLeftX());
    s_convert += "," + toString(m_envelopeValue.getLowerLeftY());
    s_convert += "," + toString(m_envelopeValue.getUpperRightX());
    s_convert += "," + toString(m_envelopeValue.getUpperRightY());
  }
  else if(m_type == dataType->getDataTypeBool()) 
  {
    s_convert = m_bValue ? "true" : "false"; 
  }
  else if (m_type == dataType->getDataTypeGeometry())
  {
    s_convert = m_geometryPtr->asText();
  }
  else if (m_type == dataType->getDataTypeStringVector())
  {
    if (!m_vString.empty())
    {
      for (std::vector<std::string>::const_iterator it = m_vString.begin(); it != m_vString.end(); ++it)
      {
        if (!s_convert.empty())
          s_convert += ",";
        s_convert += (*it);
      }
    }
  }
  else if (m_type == dataType->getDataTypeStringMatrix())
  {
    for (std::size_t i = 0; i < m_stringMatrix.size(); ++i)
    {
      if (i != 0)
      {
        s_convert += ";";
      }
      for (std::size_t j = 0; j < m_stringMatrix[i].size(); ++j)
      {
        if (j != 0)
        {
          s_convert += ",";
        }
        s_convert += m_stringMatrix[i][j];
      }
    }
  }
  else if (m_type == dataType->getDataTypeLayerList())
  {
    // do nothing
  }
  else
  {
    s_convert = m_sValue;
  }
  
  return s_convert;
}

double te::layout::Variant::string2Double( std::string str )
{
  // Convert a string representation of a number into a double value.
  
  double result;

  // Get rid of any trailing whitespace
  str.erase( str.find_last_not_of( " \f\n\r\t\v" ) + 1 );

  // Read it into the target type
  std::istringstream ss( str );
  ss >> result;

  // Check to see that there is nothing left over
  if (!ss.eof())
  {
    throw std::runtime_error("Failed: convert string to double.");
  }

  return result;
}

int te::layout::Variant::string2Int( std::string str )
{
  // Convert a string representation of a number into a int value.

  int result;

  // Get rid of any trailing whitespace
  str.erase( str.find_last_not_of( " \f\n\r\t\v" ) + 1 );

  // Read it into the target type
  std::istringstream ss( str );
  ss >> result;

  // Check to see that there is nothing left over
  if (!ss.eof())
  {
    throw std::runtime_error("Failed: convert string to int.");
  }

  return result;
}

float te::layout::Variant::string2Float( std::string str )
{
  // Convert a string representation of a number into a floating point value.

  float result;

  // Get rid of any trailing whitespace
  str.erase( str.find_last_not_of( " \f\n\r\t\v" ) + 1 );

  // Read it into the target type
  std::istringstream ss( str );
  ss >> result;

  // Check to see that there is nothing left over
  if (!ss.eof())
  {
    throw std::runtime_error("Failed: convert string to float.");
  }

  return result;
}

long te::layout::Variant::string2Long( std::string str )
{
  // Convert a string representation of a number into a long value.

  long result;

  // Get rid of any trailing whitespace
  str.erase( str.find_last_not_of( " \f\n\r\t\v" ) + 1 );

  // Read it into the target type
  std::istringstream ss( str );
  ss >> result;

  // Check to see that there is nothing left over
  if (!ss.eof())
  {
    throw std::runtime_error("Failed: convert string to long.");
  }

  return result;
}

bool te::layout::Variant::isComplex() const
{
  return m_complex;
}

std::string te::layout::Variant::toString( int value ) const
{
  std::stringstream ss;//create a stringstream
  ss << value;//add number to the stream
  
  return ss.str();
}

std::string te::layout::Variant::toString(double value) const
{
  std::ostringstream convert;
  convert.precision(15);

  convert << value;//add number to the stream

  return convert.str();
}

bool te::layout::Variant::toBool( std::string str )
{
  if(str.compare("true") == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

const te::gm::GeometryShrPtr te::layout::Variant::toGeometry() const
{
  return m_geometryPtr;
}



void  te::layout::Variant::setPrecision(int p) {
  
  m_precision = p;

}

void  te::layout::Variant::usePrecision(bool usePrecision) {

  m_usePrecision = usePrecision;

}

int te::layout::Variant::getprecision() const{
  return m_precision;
}

bool  te::layout::Variant::isUsingPrecision() const{
  return m_usePrecision;
}

te::layout::Variant& te::layout::Variant::operator=(const Variant& variant)
{
  this->m_sValue = variant.m_sValue;
  this->m_dValue = variant.m_dValue; //!< value of double type
  this->m_iValue = variant.m_iValue; //!< value of int type
  this->m_lValue = variant.m_lValue; //!< value of long type
  this->m_fValue = variant.m_fValue; //!< value of float type
  this->m_bValue = variant.m_bValue; //!< value of boolean type 
  this->m_colorValue = variant.m_colorValue; //!< value of te::color::RGBAColor type
  this->m_fontValue = variant.m_fontValue; //!< value of te::layout::Font type
  this->m_envelopeValue = variant.m_envelopeValue; //!< value of the envelope type
  this->m_type = variant.m_type; //!< data type of this object
  this->m_null = variant.m_null; //!< true if no value has been set, false otherwise
  this->m_complex = variant.m_complex; //!< true if value is not of common C++ data type, false otherwise
  this->m_vString = variant.m_vString; //!< value of string vector type
  this->m_stringMatrix = variant.m_stringMatrix; //!< value of vector of string vector type
  if (variant.m_listLayer.empty() == false)
  {
    this->m_listLayer = variant.m_listLayer; //!< value of te::map::AbstractLayerPtr list type
  }
  this->m_usePrecision = variant.m_usePrecision; //!< true if uses precision in double value
  this->m_precision = variant.m_precision;

  if (variant.m_geometryPtr.get() != 0)
    this->m_geometryPtr.reset((te::gm::Geometry*)variant.m_geometryPtr->clone());

  return *this;
}

bool te::layout::Variant::operator ==(const Variant& other) const
{
  Variant& otherProp = const_cast<Variant&>(other);

  if (getType() == otherProp.getType())
  {
    if (m_sValue == otherProp.toString() &&
      m_dValue == otherProp.toDouble() &&
      m_iValue == otherProp.toInt() &&
      m_lValue == otherProp.toLong() &&
      m_fValue == otherProp.toFloat() &&
      m_bValue == otherProp.toBool() &&
      m_colorValue == otherProp.toColor() &&
      m_fontValue == otherProp.toFont())
    {
      return true;
    }
  }
  return false;
}

bool te::layout::Variant::operator !=(const Variant& other) const
{
  Variant& otherProp = const_cast<Variant&>(other);

  if (getType() != otherProp.getType())
  {
    return true;
  }

  if (getType() == otherProp.getType())
  {
    if (m_sValue != otherProp.toString() ||
      m_dValue != otherProp.toDouble() ||
      m_iValue != otherProp.toInt() ||
      m_lValue != otherProp.toLong() ||
      m_fValue != otherProp.toFloat() ||
      m_colorValue != otherProp.toColor() ||
      m_fontValue != otherProp.toFont())
    {
      return true;
    }
    else
    {
      if (m_bValue != otherProp.toBool())
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  return true;
}

QVariant te::layout::Variant::toQVariant() const
{
  if (!m_type)
  {
    return QVariant();
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  QVariant value;

  if (m_type == dataType->getDataTypeString())
  {
    QString svalue = ItemUtils::convert2QString(m_sValue);
    value = QVariant(svalue);
  }
  else if (m_type == dataType->getDataTypeStringList())
  {
    QString svalue = ItemUtils::convert2QString(m_sValue);
    value = QVariant(svalue);
  }
  else if (m_type == dataType->getDataTypeDouble())
  {
    value = QVariant(m_dValue);
  }
  else if (m_type == dataType->getDataTypeFloat())
  {
    value = QVariant(m_fValue);
  }
  else if (m_type == dataType->getDataTypeLong())
  {
    value = QVariant(m_lValue);
  }
  else if (m_type == dataType->getDataTypeInt())
  {
    value = QVariant(m_iValue);
  }
  else if (m_type == dataType->getDataTypeBool())
  {
    value = QVariant(m_bValue);
  }
  else if (m_type == dataType->getDataTypeColor())
  {
    int a = m_colorValue.getAlpha();
    int r = m_colorValue.getRed();
    int g = m_colorValue.getGreen();
    int b = m_colorValue.getBlue();
    QColor color(r, g, b, a);
    value = QVariant(color);
  }
  else if (m_type == dataType->getDataTypeFont())
  {
    QFont qfont;
    std::string fontName = m_fontValue.getFamily();
    QString qFontName = ItemUtils::convert2QString(fontName);

    qfont.setFamily(qFontName);
    qfont.setPointSize(m_fontValue.getPointSize());
    qfont.setBold(m_fontValue.isBold());
    qfont.setItalic(m_fontValue.isItalic());
    qfont.setUnderline(m_fontValue.isUnderline());
    qfont.setStrikeOut(m_fontValue.isStrikeout());
    qfont.setKerning(m_fontValue.isKerning());
    value = QVariant(qfont);
  }
  else if (m_type == dataType->getDataTypeEnvelope())
  {
    QRectF rect(m_envelopeValue.getLowerLeftX(), m_envelopeValue.getUpperRightY(), m_envelopeValue.getWidth(), m_envelopeValue.getHeight());
    value = QVariant(rect);
  }
  else if (m_type == dataType->getDataTypeStringVector())
  {
    QList<QString> qvString;

    std::vector<std::string>::const_iterator it;
    for (it = m_vString.begin(); it != m_vString.end(); ++it)
    {
      QString svalue = ItemUtils::convert2QString(m_sValue);
      qvString.append(svalue);
    }
    value = QVariant(qvString);
  }
  else // Any remaining data will be by default "std::string"  
  {
    QString svalue = ItemUtils::convert2QString(m_sValue);
    value = QVariant(svalue);
  }
  return value;
}

