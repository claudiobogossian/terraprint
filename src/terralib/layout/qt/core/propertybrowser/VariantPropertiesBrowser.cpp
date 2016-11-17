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
  \file PropertyBrowser.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "VariantPropertiesBrowser.h"
#include "../../../core/property/Properties.h"
#include "../../../core/enum/Enums.h"
#include "../../core/ItemUtils.h"

// QtPropertyBrowser
#include <QtPropertyBrowser/QtProperty>
#include <QtPropertyBrowser/QtVariantPropertyManager>
#include <QtPropertyBrowser/QtVariantEditorFactory>
#include <QtPropertyBrowser/QtVariantProperty>

// Qt
#include <QVariant>
#include <QFont>
#include <QColor>

te::layout::VariantPropertiesBrowser::VariantPropertiesBrowser(Scene* scene, QObject* parent) :
  AbstractPropertiesBrowser(scene, parent),
  m_variantPropertyEditorManager(0),
  m_variantPropertyEditorFactory(0)
{
  createManager();
}

te::layout::VariantPropertiesBrowser::~VariantPropertiesBrowser()
{
  if(m_variantPropertyEditorManager)
  {
    delete m_variantPropertyEditorManager;
    m_variantPropertyEditorManager = 0;
  }

  if(m_variantPropertyEditorFactory)
  {
    delete m_variantPropertyEditorFactory;
    m_variantPropertyEditorFactory = 0;
  }
}

void te::layout::VariantPropertiesBrowser::createManager()
{  
  m_variantPropertyEditorManager = new QtVariantPropertyManager; 
  m_variantPropertyEditorFactory = new QtVariantEditorFactory;
}

QtProperty* te::layout::VariantPropertiesBrowser::addProperty( const Property& property )
{
  QtVariantProperty* vproperty = 0;

  if(!property.isVisible())
  {
    return vproperty;
  }

  if(!m_variantPropertyEditorManager)
  {
    return vproperty;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  if(!dataType)
  {
    return vproperty;
  }

  std::string stdLabel = property.getLabel();
  if (stdLabel.compare("") == 0)
    stdLabel = property.getName();

  QString label = ItemUtils::convert2QString(stdLabel);

  int type = getVariantType(property.getType());
  vproperty = m_variantPropertyEditorManager->addProperty(type, label);
  changeQtVariantPropertyValue(vproperty, property);

  stdLabel = property.getLabel();
  QString qLabel = ItemUtils::convert2QString(stdLabel);

  std::string name = property.getName();
  QString qName = ItemUtils::convert2QString(name);

  addPropertyItem(vproperty, qName, qLabel, property);

  return vproperty;
}

void te::layout::VariantPropertiesBrowser::addAttribute( QtVariantProperty* vproperty, const Property& property )
{
  const std::vector<Variant>& vrt = property.getOptionChoices();
  QStringList  strList;
  foreach( Variant v, vrt) 
  {
    std::string value = v.toString();
    QString qValue = ItemUtils::convert2QString(value);

    strList.push_back(qValue);
  }

  /* "enumNames" is a name used by default in 
  QtVariantProperty class for properties with lists */
  vproperty->setAttribute("enumNames", strList);
}

te::layout::Property te::layout::VariantPropertiesBrowser::getProperty(QtProperty* qtProperty)
{
  Property prop;
 
  QVariant variant = getPropertyValue(qtProperty);

  if(variant.isNull() || !variant.isValid())
  {
    return prop;
  }

  QMap<QtProperty*, Property>::const_iterator itProperty = m_mapProperty.find(qtProperty);
  if (itProperty == m_mapProperty.end())
  {
    return prop;
  }

  prop = itProperty.value();

  EnumType* type = getLayoutType(variant.type(), qtProperty);
  
  QtVariantProperty* vproperty = 0;
  if(qtProperty)
  {
    vproperty = dynamic_cast<QtVariantProperty*>(qtProperty);
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  QString qValue = variant.toString();
  std::string valueString = ItemUtils::convert2StdString(qValue);

  if(type == dataType->getDataTypeString())
  {
    prop.setValue(valueString, dataType->getDataTypeString());
  }
  else if(type == dataType->getDataTypeStringList())
  {
    prop.setValue(valueString, type);
    if(vproperty)
    {
      QStringList list = variant.toStringList();
      QString qFoundValue = list.value(vproperty->value().toInt());

      Variant v;
      foreach(QString s, list)
      {
        v.clear();
        std::string currentString = ItemUtils::convert2StdString(s);
        v.setValue(currentString, dataType->getDataTypeString());
        prop.addOption(v);
        if (qFoundValue.compare(s) == 0)
        {
          prop.setOptionChoice(v);
        }
      }
    }
  }
  else if(type == dataType->getDataTypeDouble())
  {
    prop.setValue(variant.toDouble(), dataType->getDataTypeDouble());
  }
  else if(type == dataType->getDataTypeInt())
  {
    prop.setValue(variant.toInt(), dataType->getDataTypeInt());
  }
  else if(type == dataType->getDataTypeBool())
  {
    prop.setValue(variant.toBool(), dataType->getDataTypeBool());
  }
  else if(type == dataType->getDataTypeColor())
  {
    QColor qcolor = variant.value<QColor>();
    if(qcolor.isValid()) 
    {
      te::color::RGBAColor color(qcolor.red(), qcolor.green(), qcolor.blue(), qcolor.alpha());
      prop.setValue(color, dataType->getDataTypeColor());
    }
  }
  else if(type == dataType->getDataTypeFont())
  {
    QFont qfont = variant.value<QFont>();
    Font font;

    QString qFontName = qfont.family();
    std::string fontName = ItemUtils::convert2StdString(qFontName);

    font.setFamily(fontName);
    font.setPointSize(qfont.pointSize());
    font.setBold(qfont.bold());
    font.setItalic(qfont.italic());
    font.setUnderline(qfont.underline());
    font.setStrikeout(qfont.strikeOut());
    font.setKerning(qfont.kerning());
    prop.setValue(font, dataType->getDataTypeFont());
  }
  else if(type == dataType->getDataTypeEnvelope())
  {
    QRectF rect = variant.value<QRectF>();

    te::gm::Envelope envelope(rect.left(), rect.bottom(), rect.right(), rect.top());
    prop.setValue(envelope, dataType->getDataTypeEnvelope());
  }

  return prop;
}

te::layout::EnumType* te::layout::VariantPropertiesBrowser::getLayoutType(QVariant::Type type, QtProperty* qtProperty)
{
  EnumDataType* dtType = Enums::getInstance().getEnumDataType();

  EnumType* dataType = dtType->getDataTypeNone();
  QVariant variant;
  QtVariantProperty* vproperty = 0;

  switch(type)
  {
    case QVariant::String:
      {
        dataType = dtType->getDataTypeString();
      }
      break;
    case QVariant::StringList:
      vproperty = dynamic_cast<QtVariantProperty*>(qtProperty);
      if(vproperty)
      {
        if(QtVariantPropertyManager::enumTypeId() == vproperty->propertyType())
        {
          dataType = dtType->getDataTypeStringList();
        }
        else if(QtVariantPropertyManager::groupTypeId() == vproperty->propertyType())
        {
          dataType = dtType->getDataTypeGroup();
        }
      }
      break;
    case QVariant::Double:
      dataType = dtType->getDataTypeDouble();
      break;
    case QVariant::Int:
      {
        dataType = dtType->getDataTypeInt();
      }
      break;
    case QVariant::Bool:
      dataType = dtType->getDataTypeBool();
      break;
    case QVariant::Color:
      dataType = dtType->getDataTypeColor();
      break;
    case QVariant::Font:
      dataType = dtType->getDataTypeFont();
      break;
    case QVariant::RectF:
      dataType = dtType->getDataTypeEnvelope();
      break;
    default:
      dataType = dtType->getDataTypeNone();
  }

  return dataType;
}

int te::layout::VariantPropertiesBrowser::getVariantType( te::layout::EnumType* dataType )
{
  int type = QVariant::Invalid;

  EnumDataType* dtType = Enums::getInstance().getEnumDataType();
  
  if(!dtType)
  {
    return type;
  }

  if(dataType == dtType->getDataTypeString())
  {
    type = QVariant::String;
  }
  else if(dataType == dtType->getDataTypeStringList())
  {
    type = QtVariantPropertyManager::enumTypeId();
  }
  else if(dataType == dtType->getDataTypeGroup())
  {
    type = QtVariantPropertyManager::groupTypeId();
  }
  else if(dataType == dtType->getDataTypeDouble())
  {
    type = QVariant::Double;
  }
  else if(dataType == dtType->getDataTypeInt())
  {
    type = QVariant::Int;
  }
  else if(dataType == dtType->getDataTypeBool())
  {
    type = QVariant::Bool;
  }
  else if(dataType == dtType->getDataTypeColor())
  {
    type = QVariant::Color;
  }
  else if(dataType == dtType->getDataTypeFont())
  {
    type = QVariant::Font;
  }
  else if(dataType == dtType->getDataTypeEnvelope())
  {
    type = QVariant::RectF;
  }

  return type;
}

bool te::layout::VariantPropertiesBrowser::changeQtVariantPropertyValue( QtVariantProperty* vproperty, const Property& property )
{
  bool result = true;

  m_changeProperty = true;
  
  if(!vproperty)
  {
    return false;
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();
  
  if(property.getType() == dataType->getDataTypeString())
  {
    std::string value = te::layout::Property::GetValueAs<std::string>(property);
    QString qValue = ItemUtils::convert2QString(value);

    vproperty->setValue(qValue);    
  }
  else if(property.getType() == dataType->getDataTypeStringList())
  {    
    addAttribute(vproperty, property);

    const std::string& currentValue = property.getOptionByCurrentChoice().toString();

    const std::vector<Variant>& vecChoices = property.getOptionChoices();
    for(size_t i = 0; i < vecChoices.size(); ++i)
    {
      if(currentValue == vecChoices[i].toString())
      {
        QVariant vValue((int)i);
        vproperty->setValue(vValue);
        break;
      }
    }
  }
  else if(property.getType() == dataType->getDataTypeGroup())
  {
    std::string value = te::layout::Property::GetValueAs<std::string>(property);
    QString qValue = ItemUtils::convert2QString(value);

    vproperty->setValue(qValue);  
  }
  else if(property.getType() == dataType->getDataTypeDouble())
  {
    if (property.getUsePrecision()){
      vproperty->setAttribute(QLatin1String("decimals"), property.getPrecision());
    }
    vproperty->setValue(te::layout::Property::GetValueAs<double>(property));
  }
  else if(property.getType() == dataType->getDataTypeInt())
  {
    vproperty->setValue(te::layout::Property::GetValueAs<int>(property));
  }
  else if(property.getType() == dataType->getDataTypeBool())
  {
    vproperty->setValue(te::layout::Property::GetValueAs<bool>(property));
  }
  else if(property.getType() == dataType->getDataTypeColor())
  {
    const te::color::RGBAColor& color = te::layout::Property::GetValueAs<te::color::RGBAColor>(property);

    QColor qcolor;
    qcolor.setRed(color.getRed());
    qcolor.setGreen(color.getGreen());
    qcolor.setBlue(color.getBlue());
    qcolor.setAlpha(color.getAlpha());
    vproperty->setValue(qcolor);
  }
  else if(property.getType() == dataType->getDataTypeFont())
  {
    const Font& font = te::layout::Property::GetValueAs<Font>(property);
    QFont qfont;
    std::string fontName = font.getFamily();
    QString qFontName = ItemUtils::convert2QString(fontName);

    qfont.setFamily(qFontName);
    qfont.setPointSize(font.getPointSize());
    qfont.setBold(font.isBold());
    qfont.setItalic(font.isItalic());
    qfont.setUnderline(font.isUnderline());
    qfont.setStrikeOut(font.isStrikeout());
    qfont.setKerning(font.isKerning());
    vproperty->setValue(qfont);    
  }
  else if(property.getType() == dataType->getDataTypeEnvelope())
  {
    const te::gm::Envelope& envelope  = te::layout::Property::GetValueAs<te::gm::Envelope>(property);
    QRectF rect(envelope.getLowerLeftX(), envelope.getUpperRightY(), envelope.getWidth(), envelope.getHeight());

    vproperty->setValue(rect);
  }
  else
  {
    result = false;
  }

  m_changeProperty = false;

  return result;
}

QtVariantEditorFactory* te::layout::VariantPropertiesBrowser::getVariantEditorFactory()
{
  return m_variantPropertyEditorFactory;
}

QtVariantPropertyManager* te::layout::VariantPropertiesBrowser::getVariantPropertyManager()
{
  return m_variantPropertyEditorManager;
}

bool te::layout::VariantPropertiesBrowser::updateProperty( const Property& property )
{
  const std::string& stdLabel = property.getLabel();
  QString label = ItemUtils::convert2QString(stdLabel);

  QtProperty* qprop = findProperty(property.getName(), property.getParent());
  if (!qprop)
    return false;

  QtVariantProperty* vproperty = m_variantPropertyEditorManager->variantProperty(qprop);
  if(!vproperty)
  {
    return false;
  }  

  changeQtVariantPropertyValue(vproperty, property);

  return true;
}
