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
  \file terralib/layout/qt/outside/MapSettingsOutside.h

  \brief
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_SETTINGS_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_SETTINGS_OUTSIDE_H

// TerraLib
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractOutsideView.h"

// Qt
#include <QDialog>
#include <QString>

namespace Ui { class MapSettings; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;

    class TELAYOUTEXPORT MapSettingsOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        MapSettingsOutside(AbstractOutsideController* controller);

        virtual ~MapSettingsOutside();

        /*!
          \brief Load layers in double widget.
        */
        virtual void init();

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

        virtual std::string getFilePath();

        virtual int getDPI();

        virtual void setCurrentDPI(int dpi);

      signals:

        void updateProperty(Property prop);

        void updateProperties(std::vector<Property> props);
                        
      protected slots:

        void onOkPushButtonPressed();

        void onCancelPushButtonPressed();

        void onSaveAsClicked();

        void onCurrentIndexChanged(const QString & text);
        
      private:

        std::auto_ptr<Ui::MapSettings> m_ui;
        
    };
  }    
}     

#endif

