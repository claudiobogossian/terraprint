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
  \file TemplateEditor.h
   
  \brief Calls the factory to create a temp data storage of the specified type and update. 
        Each change in the Undo / Redo will be mirrored in the temporary data storage.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEMP_DATASTORAGE_EDITOR_H 
#define __TERRALIB_LAYOUT_INTERNAL_TEMP_DATASTORAGE_EDITOR_H

// TerraLib
#include "../../../core/Config.h"

// STL
#include <string>
#include <memory>
#include <vector>
#include <map>

#ifndef Q_MOC_RUN
// Boost
#include <boost/thread.hpp>
#endif

// Qt
#include <QObject>

class QUndoStack;

namespace te
{
  namespace layout
  {
    class AbstractTempDataStorageInfo;
    class EnumType;
    class Scene;
    class AbstractTempDataStorage;

    /*!
      \brief Calls the factory to create a temp data storage of the specified type and update. 
        Each change in the Undo / Redo will be mirrored in the temporary data storage.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT TempDataStorageEditor : public QObject
    {
      Q_OBJECT //for slots/signals

      public:

        /*
          \brief Constructor. Gain ownership of the pointer information about the temporary data.

          \param info information about the temporary data
        */
        TempDataStorageEditor(QUndoStack* undoStack, EnumType* dataStorageType, AbstractTempDataStorageInfo* info, bool asynchronous = true);

        virtual ~TempDataStorageEditor();

        void stop();

        AbstractTempDataStorageInfo* getTempDataStorageInfo();

        bool deleteDataStorage();

      signals:

        void requestIOEnterAccess();

        void requestIOEndAccess();

      protected slots:

        virtual void onIndexChanged(int idx);

      protected:

        virtual void save();

        virtual void update();

        virtual void saveAsynchronous();

        void runSave();

        void done();

        void wait(int millisec);

        void verifySentinelThread();

        void deleteAllDoneThreads();

      protected:

        AbstractTempDataStorageInfo*                    m_info;
        QUndoStack*                                     m_undoStack;
        bool                                            m_asynchronous;
        EnumType*                                       m_dataStorageType;
        bool                                            m_requestIOEnterAccess;
        boost::mutex                                    m_mutex; //!< A mutex to lock the access
        std::map<std::string, boost::thread*>           m_threads;
        std::vector<std::string>                        m_idsDone;
        AbstractTempDataStorage*                        m_tempDataStorage;
        boost::thread*                                  m_threadSentinel;
    };
  }
}

#endif
