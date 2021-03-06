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
  \file terralib/qt/plugins/layout/AbstractAction.h

  \brief This file defines the abstract class AbstractAction
*/

#ifndef __TE_QT_PLUGINS_LAYOUT_INTERNAL_ABSTRACTACTION_H
#define __TE_QT_PLUGINS_LAYOUT_INTERNAL_ABSTRACTACTION_H

// TerraLib
#include "Config.h"

// Qt
#include <QObject>
#include <QMenu>
#include <QAction>
#include <QString>

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace layout
      {
        /*!
          \class AbstractAction

          \brief This is an abstract class used to register actions into vp pluging.

        */
        class AbstractAction : public QObject
        {
          Q_OBJECT

          public:

            /*!
              \brief Constructor.

              \param menu The parent menu object.
            */
            AbstractAction(QMenu* menu);

            /*! 
              \brief Destructor. 
            */
            virtual ~AbstractAction();

            QAction* getAction()
            {
              return m_action;
            }

          protected slots:

            /*!
              \brief Slot function used when a action was selected.

              \param checked Flag used in case a toggle action.
            */
            virtual void onActionActivated(bool checked) = 0;

          protected:

            /*!
              \brief Create and set the actions parameters.

              \param name The action name.

              \param pixmap The action pixmap name.
            */
            void createAction(const QString& name, const QString& pixmap = "");

          protected:

            QMenu* m_menu;        //!< Parent Menu.
            QAction* m_action;    //!< Action used to call the process.
        };

      } // end namespace layout
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif //__TE_QT_PLUGINS_LAYOUT_INTERNAL_ABSTRACTACTION_H