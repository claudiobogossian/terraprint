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

// STL
#include <iostream>
#include <utility>

// Boost
#include <boost/lexical_cast.hpp>

// Qt
#include <QUndoStack>

te::layout::TempDataStorageEditor::TempDataStorageEditor(QUndoStack* undoStack, EnumType* dataStorageType, AbstractTempDataStorageInfo* info, bool asynchronous) :
  m_undoStack(undoStack),
  m_info(info),
  m_asynchronous(asynchronous),
  m_dataStorageType(dataStorageType),
  m_requestIOEnterAccess(false),
  m_tempDataStorage(0),
  m_threadSentinel(0)
{
  connect(m_undoStack, SIGNAL(indexChanged(int)), this, SLOT(onIndexChanged(int)));
  if (!m_tempDataStorage)
  {
    TempDataStorageFactoryParamCreate params(m_info);
    m_tempDataStorage = te::layout::TempDataStorageFactory::make(m_dataStorageType->getName(), params);
  }
}

te::layout::TempDataStorageEditor::~TempDataStorageEditor()
{
  if (m_tempDataStorage)
  {
    delete m_tempDataStorage;
    m_tempDataStorage = 0;
  }

  if (m_info)
  {
    delete m_info;
    m_info = 0;
  }

  if (m_threadSentinel)
  {
    delete m_threadSentinel;
    m_threadSentinel = 0;
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

void te::layout::TempDataStorageEditor::save()
{
  m_tempDataStorage->save();
}

void te::layout::TempDataStorageEditor::update()
{
  
}

void te::layout::TempDataStorageEditor::saveAsynchronous()
{
  if (!m_threadSentinel)
  {
    // create sentinel thread
    m_threadSentinel = new boost::thread(&te::layout::TempDataStorageEditor::verifySentinelThread, this);
  }

  m_tempDataStorage->readProperties();
    
  // add new thread
  boost::thread* t = new boost::thread([this]{
    te::layout::TempDataStorageEditor::runSave();
    te::layout::TempDataStorageEditor::done();
  });

  std::string id = boost::lexical_cast<std::string>(t->get_id());
  m_threads.insert(std::pair<std::string, boost::thread*>(id, t));
}

void te::layout::TempDataStorageEditor::runSave()
{
  // A mutex to lock the access
  boost::mutex::scoped_lock lock(m_mutex);

  if (!m_requestIOEnterAccess)
  {
    m_requestIOEnterAccess = true;
    emit requestIOEnterAccess();
  }  

  save(); // save temp data storage

  lock.unlock();
}

void te::layout::TempDataStorageEditor::done()
{
  // A mutex to lock the access
  boost::mutex::scoped_lock lock(m_mutex);
  
  std::string id = boost::lexical_cast<std::string>(boost::this_thread::get_id());
  m_idsDone.push_back(id);

  lock.unlock();
}

void te::layout::TempDataStorageEditor::wait(int millisec)
{
  // Sleep and check for interrupt.
  boost::this_thread::sleep(boost::posix_time::milliseconds(millisec));
}

void te::layout::TempDataStorageEditor::verifySentinelThread()
{
  try
  {
    while (true)
    {
      wait(5000);
      deleteAllDoneThreads();
      if (m_requestIOEnterAccess && m_idsDone.empty())
      {
        m_requestIOEnterAccess = false;
        emit requestIOEndAccess();
      }
    }
  }
  catch (boost::thread_interrupted&)
  {
    std::cout << "Sentinel Thread for temp data storage is stopped" << std::endl;
    return;
  }
}

void te::layout::TempDataStorageEditor::deleteAllDoneThreads()
{
  if (m_idsDone.empty())
  {
    return;
  }

  std::vector<std::string>::iterator it;

  for (it = m_idsDone.begin(); it != m_idsDone.end(); ++it)
  {
    std::string threadId = (*it);
    std::map<std::string, boost::thread*>::iterator itThread = m_threads.find(threadId);
    if (itThread != m_threads.end())
    {
      boost::thread* thread = itThread->second;
      m_threads.erase(itThread);
      
      thread->interrupt();
      thread->join();

      delete thread;
    }
  }

  m_idsDone.clear();
}

void te::layout::TempDataStorageEditor::stop()
{
  if (m_threadSentinel)
  {
    m_threadSentinel->interrupt();
    m_threadSentinel->join();
  }

  deleteAllDoneThreads();

  if (m_requestIOEnterAccess && m_idsDone.empty())
  {
    m_requestIOEnterAccess = false;
    emit requestIOEndAccess();
  }
}

