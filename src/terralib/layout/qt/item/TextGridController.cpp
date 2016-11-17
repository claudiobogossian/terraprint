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

// TerraLib
#include "TextGridController.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/enum/EnumAlignmentType.h"
#include "../../qt/core/Scene.h"
#include "../../qt/item/AbstractItem.h"
#include "../../qt/item/TextItem.h"


#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextTable>

te::layout::TextGridController::TextGridController(AbstractItemModel* model)
  : TextController(model)
{
  //here we initialize the empty text matrix
  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  int numRows = 2;
  int numColumns = 2;

  Properties properties;
  {
    Property property;
    property.setName("num_rows");
    property.setValue(numRows, dataType->getDataTypeInt());
    properties.addProperty(property);
  }
  {
    Property property;
    property.setName("num_columns");
    property.setValue(numColumns, dataType->getDataTypeInt());
    properties.addProperty(property);
  }

  setProperties(properties);
}

te::layout::TextGridController::~TextGridController()
{
}

void te::layout::TextGridController::setProperties(const te::layout::Properties& properties)
{
  if (properties.contains("text_matrix") == true && (properties.contains("num_rows") == false || properties.contains("num_columns") == false))
  {
    const Property& pTextMatrix = properties.getProperty("text_matrix");
    std::vector< std::vector< std::string > > textMatrix = te::layout::Property::GetValueAs< std::vector< std::vector< std::string > > >(pTextMatrix); //we copy the matrix

    //if the matrix is not empty, we must update the properties num_rows and num_columns
    //if the matrix is empty, we must initialize it with the default values
    if (textMatrix.empty() || textMatrix[0].empty())
    {
      textMatrix = m_defaultMatrix;
    }

    //here we analyse the matrix the get the number of rows and the number of columns
    int numRows = (int)textMatrix.size();
    int numColumns = (int)textMatrix[0].size();

    Properties propertiesCopy(properties);
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    {
      Property property;
      property.setName("num_rows");
      property.setValue(numRows, dataType->getDataTypeInt());

      propertiesCopy.addProperty(property);
    }

    {
      Property property;
      property.setName("num_columns");
      property.setLabel(TR_LAYOUT("Number of Columns"));
      property.setValue(numColumns, dataType->getDataTypeInt());

      propertiesCopy.addProperty(property);
    }

    TextController::setProperties(propertiesCopy);
    return;
  }
  else if (properties.contains("text_matrix") == false && (properties.contains("num_rows") == true || properties.contains("num_columns") == true))
  {
    Properties propertiesCopy(properties);
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();

    const Property& pNumRows = getProperty("num_rows", properties);
    const Property& pNumColumns = getProperty("num_columns", properties);
    Property pTextMatrix = this->getProperty("text_matrix");
    
    int numRows = te::layout::Property::GetValueAs<int>(pNumRows);
    int numColumns = te::layout::Property::GetValueAs<int>(pNumColumns);
    std::vector< std::vector< std::string > > textMatrix = te::layout::Property::GetValueAs< std::vector< std::vector< std::string > > >(pTextMatrix); //we copy the matrix

    //here we resize the text matrix according to the new given size
    textMatrix.resize(numRows);
    for (std::size_t i = 0; i < numRows; ++i)
    {
      textMatrix[i].resize(numColumns);
    }

    pTextMatrix.setValue(textMatrix, dataType->getDataTypeStringMatrix());
    propertiesCopy.addProperty(pTextMatrix);

    TextController::setProperties(propertiesCopy);
    return;
  }

  TextController::setProperties(properties);
}

QTextDocument* te::layout::TextGridController::createTextDocument(const te::layout::Properties& properties)
{
  QTextDocument* textDocument = TextController::createTextDocument(properties);

  const Property& pTextMatrix = getProperty("text_matrix", properties);
  const std::vector< std::vector<std::string> >& textMatrix = te::layout::Property::GetValueAs< std::vector< std::vector<std::string> > >(pTextMatrix);
  if (textMatrix.empty())
  {
    return textDocument;
  }

  textDocument->clear();

  int numRows = (int)textMatrix.size();
  int numColumns = (int)textMatrix[0].size();

  QTextCursor cursor(textDocument);
  cursor.movePosition(QTextCursor::Start);

  QTextTableFormat tableFormat;
  tableFormat.setAlignment(textDocument->defaultTextOption().alignment());

  int widthPercentage = 100 / numColumns;

  QVector<QTextLength> constraints;
  for (int c = 0; c < numColumns; ++c)
  {
    constraints << QTextLength(QTextLength::FixedLength, widthPercentage);
  }

  tableFormat.setColumnWidthConstraints(constraints);
  tableFormat.setHeaderRowCount(1);

  QTextTable* textTable = cursor.insertTable(numRows, numColumns, tableFormat);
  for (int row = 0; row < numRows; ++row)
  {
    for (int column = 0; column < numColumns; ++column)
    {
      QTextCursor textCursor = textTable->cellAt(row, column).firstCursorPosition();
      textCursor.insertText(ItemUtils::convert2QString(textMatrix[row][column]));
    }
  }

  //this is used to adjust the text
  textDocument->setTextWidth(-1);
  textDocument->setTextWidth(textDocument->size().width());

  return textDocument;
}

bool te::layout::TextGridController::needUpdateBox(const te::layout::Properties& properties)
{
  if (properties.contains("text_matrix") == true)
  {
    return true;
  }
  else if (properties.contains("num_rows") == true)
  {
    return true;
  }
  else if (properties.contains("num_columns") == true)
  {
    return true;
  }

  return TextController::needUpdateBox(properties);
}
