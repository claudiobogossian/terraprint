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
  \file EnumGridStyleType.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "EnumModeType.h"

te::layout::EnumModeType::EnumModeType() :
  m_modeNone(0),
  m_modeMapPan(0),
  m_modeMapZoomIn(0),
  m_modeMapZoomOut(0),
  m_modeMapRecompose(0),
  m_modePrinterPreview(0),
  m_modePrinter(0),
  m_modeSystematicScale(0),
  m_modeUndo(0),
  m_modeRedo(0),
  m_modeTextEditorInteraction(0),
  m_modeArrowCursor(0)
{
  init();
}

te::layout::EnumModeType::~EnumModeType()
{
  if(m_modeMapPan)
  {
    delete m_modeMapPan;
    m_modeMapPan = 0;
  }
  if(m_modeMapZoomIn)
  {
    delete m_modeMapZoomIn;
    m_modeMapZoomIn = 0;
  }
  if(m_modeMapZoomOut)
  {
    delete m_modeMapZoomOut;
    m_modeMapZoomOut = 0;
  }
  if (m_modeMapRecompose)
  {
    delete m_modeMapRecompose;
    m_modeMapRecompose = 0;
  }

  if (m_modePrinterPreview)
  {
    delete m_modePrinterPreview;
    m_modePrinterPreview = 0;
  }

  if (m_modePrinter)
  {
    delete m_modePrinter;
    m_modePrinter = 0;
  }

  if(m_modeSystematicScale)
  {
    delete m_modeSystematicScale;
    m_modeSystematicScale = 0;
  }
  if(m_modeUndo)
  {
    delete m_modeUndo;
    m_modeUndo = 0;
  }
  if(m_modeRedo)
  {
    delete m_modeRedo;
    m_modeRedo = 0;
  }
  if(m_modeTextEditorInteraction)
  {
    delete m_modeTextEditorInteraction;
    m_modeTextEditorInteraction = 0;
  }
  if (m_modeArrowCursor)
  {
    delete m_modeArrowCursor;
    m_modeArrowCursor = 0;
  }
}

void te::layout::EnumModeType::init()
{
  m_modeNone = createEnum("None", this, TR_LAYOUT("None"));

  m_modeMapPan = createEnum("MapPan", this, TR_LAYOUT("Map Pan"));
  m_modeMapPan->setType(te::layout::EnumTool);

  m_modeMapZoomIn = createEnum("MapZoomIn", this, TR_LAYOUT("Map Zoom In"));
  m_modeMapZoomIn->setType(te::layout::EnumTool);

  m_modeMapZoomOut = createEnum("MapZoomOut", this, TR_LAYOUT("Map Zoom Out"));
  m_modeMapZoomOut->setType(te::layout::EnumTool);
    
  m_modeMapRecompose = createEnum("MapRecompose", this, TR_LAYOUT("Map Recompose"));
  m_modeMapRecompose->setType(te::layout::EnumAction);

  m_modePrinterPreview = createEnum("Printer Preview", this, TR_LAYOUT("Printer Preview"));
  m_modePrinterPreview->setType(te::layout::EnumDialog);

  m_modePrinter = createEnum("Printer", this, TR_LAYOUT("Printer"));
  m_modePrinter->setType(te::layout::EnumDialog);

  m_modeSystematicScale = createEnum("SystematicScale", this, TR_LAYOUT("Systematic Scale"));
  m_modeSystematicScale->setType(te::layout::EnumDialog);

  m_modeUndo = createEnum("Undo", this, TR_LAYOUT("Undo"));
  m_modeUndo->setType(te::layout::EnumAction);

  m_modeRedo = createEnum("Redo", this, TR_LAYOUT("Redo"));
  m_modeRedo->setType(te::layout::EnumAction);

  m_modeTextEditorInteraction = createEnum("TextEditorInteraction", this, TR_LAYOUT("Text Editor Interaction"));
  m_modeTextEditorInteraction->setType(te::layout::EnumAction);

  m_modeArrowCursor = createEnum("Arrow Cursor", this, TR_LAYOUT("Arrow Cursor"));
  m_modeArrowCursor->setType(te::layout::EnumAction);
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapPan() const
{
  return m_modeMapPan;
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapZoomIn() const
{
  return m_modeMapZoomIn;
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapZoomOut() const
{
  return m_modeMapZoomOut;
}

te::layout::EnumType* te::layout::EnumModeType::getModeMapRecompose() const
{
  return m_modeMapRecompose;
}

te::layout::EnumType* te::layout::EnumModeType::getModePrinterPreview() const
{
  return m_modePrinterPreview;
}

te::layout::EnumType* te::layout::EnumModeType::getModePrinter() const
{
  return m_modePrinter;
}

te::layout::EnumType* te::layout::EnumModeType::getModeSystematicScale() const
{
  return m_modeSystematicScale;
}

te::layout::EnumType* te::layout::EnumModeType::getModeNone() const
{
  return m_modeNone;
}

te::layout::EnumType* te::layout::EnumModeType::getModeUndo() const
{
  return m_modeUndo;
}

te::layout::EnumType* te::layout::EnumModeType::getModeRedo() const
{
  return m_modeRedo;
}

te::layout::EnumType* te::layout::EnumModeType::getModeTextEditorInteraction() const
{
  return m_modeTextEditorInteraction;
}

te::layout::EnumType* te::layout::EnumModeType::getModeArrowCursor() const
{
  return m_modeArrowCursor;
}

