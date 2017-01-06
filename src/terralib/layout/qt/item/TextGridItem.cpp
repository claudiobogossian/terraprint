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
  \file TextGridItem.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TextGridItem.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractItemController.h"

// Qt
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTableCell>

te::layout::TextGridItem::TextGridItem(AbstractItemController* controller) 
  : TextItem(controller)
{
}

te::layout::TextGridItem::~TextGridItem()
{
  
}

void te::layout::TextGridItem::keyPressEvent(QKeyEvent * event)
{
  //we must ensure that the cursor stay inside the table
  TextItem::keyPressEvent(event);
}

void te::layout::TextGridItem::documentEditionFinished()
{
  QTextTable* textTable = 0;

  //we first access the table inside the document
  QTextFrame* rootFrame = m_document->rootFrame();
  QList<QTextFrame*> qFramesList = rootFrame->childFrames();
  QList<QTextFrame*>::iterator itFrames = qFramesList.begin();
  while (itFrames != qFramesList.end())
  {
    textTable = qobject_cast<QTextTable*>(*itFrames);
    if (textTable)
    {
      break;
    }
  }

  if (textTable == 0)
  {
    return;
  }

  // Update Model
  int numRows = textTable->rows();
  int numColumns = textTable->columns();

  //Table Values
  std::vector< std::vector<std::string> > textMatrix;
  for (int i = 0; i < numRows; ++i)
  {
    std::vector<std::string> textRow;
    for (int j = 0; j < numColumns; ++j)
    {
      QTextTableCell cell = textTable->cellAt(i, j);
      QTextCursor cellCursor = cell.firstCursorPosition();
      std::string text = ItemUtils::convert2StdString(cellCursor.block().text());

      textRow.push_back(text);
    }

    textMatrix.push_back(textRow);
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  Property propertyText(0);
  propertyText.setName("text_matrix");
  propertyText.setValue(textMatrix, dataType->getDataTypeStringMatrix());

  te::layout::AbstractItem::setProperty(propertyText);
}

void te::layout::TextGridItem::updateBlockEditionRange()
{
  QTextTable* textTable = 0;

  //we first access the table inside the document
  QTextFrame* rootFrame = m_document->rootFrame();
  QList<QTextFrame*> qFramesList = rootFrame->childFrames();
  QList<QTextFrame*>::iterator itFrames = qFramesList.begin();
  while (itFrames != qFramesList.end())
  {
    textTable = qobject_cast<QTextTable*>(*itFrames);
    if (textTable)
    {
      break;
    }
  }

  if (textTable == 0)
  {
    return;
  }

  int numRows = textTable->rows();
  int numColumns = textTable->columns();

  QTextTableCell firstCell = textTable->cellAt(0, 0);
  QTextTableCell lastCell = textTable->cellAt(numRows - 1, numColumns - 1);

  m_blockEditionRangeStart = firstCell.firstCursorPosition().block().blockNumber();
  m_blockEditionRangeEnd = lastCell.firstCursorPosition().block().blockNumber();
}


/*
void te::layout::TextGridItem::updateDocument()
{
  document()->clear();

  QTextDocument* doc = document();

  QTextCursor cursor(doc);
  cursor.movePosition(QTextCursor::Start);

  QBrush blackBrush(Qt::SolidPattern);

  QTextTableFormat tableFormat;
  tableFormat.setAlignment(Qt::AlignLeft);
  tableFormat.setBorderBrush(blackBrush);
  tableFormat.setBorder(0.5);

  int columns = model->getNumberColumns();
  int rows = model->getNumberRows();
 
  tableFormat.setCellSpacing(3);  
  tableFormat.setCellPadding(3); 
  tableFormat.setHeaderRowCount(1);
  
  m_table = cursor.insertTable(rows, columns, tableFormat);
    
  int countRows = m_table->rows();
  int countColumns = m_table->columns();
    
  //Table Headers (Hrz)
  for(int i = 1 ; i < countRows ; ++i)
  {
    QTextTableCell cellOne = m_table->cellAt(i, 0);
    QTextCharFormat fmtOne = cellOne.format(); 
    fmtOne.setBackground(Qt::gray);
    fmtOne.setFontWeight(QFont::Bold);
    cellOne.setFormat(fmtOne);
  }

  //Table Headers (Vrt)
  for(int j = 0 ; j < countColumns ; ++j)
  {
    QTextTableCell cellTwo = m_table->cellAt(0, j);
    QTextCharFormat fmtTwo = cellTwo.format(); 
    fmtTwo.setBackground(Qt::gray); 
    fmtTwo.setFontWeight(QFont::Bold);
    cellTwo.setFormat(fmtTwo);
    
  }
}
*/
