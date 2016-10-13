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
}

te::layout::TextGridController::~TextGridController()
{
}

QTextDocument* te::layout::TextGridController::createTextDocument(const te::layout::Properties& properties)
{
  QTextDocument* textDocument = TextController::createTextDocument(properties);

  Property pTextMatrix = properties.getProperty("text_matrix");
  if (pTextMatrix.isNull())
  {
    pTextMatrix = this->getProperty("text_matrix");
  }

  const std::vector< std::vector<std::string> >& textMatrix = pTextMatrix.getValue().toStringMatrix();
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

  QVector<QTextLength> constraints;
  for (int c = 0; c < 2; ++c)
  {
    constraints << QTextLength(QTextLength::PercentageLength, 100);
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
  return TextController::needUpdateBox(properties);
}
