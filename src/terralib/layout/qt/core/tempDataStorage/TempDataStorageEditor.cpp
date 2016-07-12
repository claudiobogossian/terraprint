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
  \file TempDataStorageEditor.cpp
   
  \brief 

  \ingroup layout
*/

// TerraLib
#include "TempDataStorageEditor.h"
#include "../../../core/enum/Enums.h"
#include "AbstractTempDataStorage.h"
#include "AbstractTempDataStorageInfo.h"
#include "TempDataStorageFactory.h"
#include "TempDataStorageFactoryParamCreate.h"

// Qt
#include <QUndoStack>

te::layout::TempDataStorageEditor::TempDataStorageEditor(QUndoStack* undoStack, EnumType* dataStorageType, AbstractTempDataStorageInfo* info, bool asynchronous) :
  m_undoStack(undoStack),
  m_info(info),
  m_asynchronous(asynchronous),
  m_dataStorageType(dataStorageType)
{
  connect(m_undoStack, SIGNAL(indexChanged(int)), this, SLOT(onIndexChanged(int)));
}

te::layout::TempDataStorageEditor::~TempDataStorageEditor()
{
  if (m_info)
  {
    delete m_info;
    m_info = 0;
  }
}

void te::layout::TempDataStorageEditor::onIndexChanged(int idx)
{
  if (!m_asynchronous)
  {
    emit requestIOEnterAccess();
    save();
    emit requestIOEndAccess();
  }
  else
  {
    saveAsynchronous();
  }
}

bool te::layout::TempDataStorageEditor::save()
{
  bool result = true;

  TempDataStorageFactoryParamCreate params(m_info);
  AbstractTempDataStorage* tempDataStorage = te::layout::TempDataStorageFactory::make(m_dataStorageType->getName(), params);
  
  if (!tempDataStorage)
  {
    result = false;
  }
  else
  {
    result = tempDataStorage->save();

    delete tempDataStorage;
    tempDataStorage = 0;
  }

  return result;
}

bool te::layout::TempDataStorageEditor::update()
{
  return save();
}

bool te::layout::TempDataStorageEditor::saveAsynchronous()
{
  return false;
}

