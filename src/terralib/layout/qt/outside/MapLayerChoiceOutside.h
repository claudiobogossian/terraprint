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
  \file terralib/layout/qt/outside/MapLayerChoiceOutside.h

  \brief
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_MAP_LAYER_CHOICE_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_MAP_LAYER_CHOICE_OUTSIDE_H

// TerraLib
#include "terralib/qt/widgets/utils/DoubleListWidget.h"
#ifndef Q_MOC_RUN
#include "terralib/maptools/AbstractLayer.h"
#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#endif

// STL
#include <memory>
#include <string>
#include <vector>
#include <list>

// Qt
#include <QDialog>

namespace Ui { class MapLayerChoice; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;

    class TELAYOUTEXPORT MapLayerChoiceOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        MapLayerChoiceOutside(AbstractOutsideController* controller, QWidget* parent);

        ~MapLayerChoiceOutside();

        /*!
          \brief Load layers in double widget.
        */
        virtual void init();
        
        virtual void setPosition( const double& x, const double& y );

        virtual te::gm::Coord2D getPosition();
        
        virtual Property getSavedLayers();

        virtual void load();

      signals:

        void updateWidgetProperty(Property prop);

        void updateProperties(std::vector<Property> props);
        
        void closeWidget();


        
      protected slots:

       void onCancelPushButtonClicked();

       void onOkPushButtonPressed();

       void onCancelPushButtonPressed();

       void on_lneWidth_editingFinished();

       void on_lneHeight_editingFinished();

       void on_ckbFixedScale_clicked();

       virtual void on_cmbUnit_currentIndexChanged(const QString & text);

       virtual void on_cmbScale_currentIndexChanged(const QString & text);

      protected:

        std::vector<std::string> intersectionLayersTitle(std::vector<std::string> output);
        virtual Property getProperty(std::string name);

      private:

        std::auto_ptr<Ui::MapLayerChoice> m_ui;
        std::auto_ptr<te::qt::widgets::DoubleListWidget> m_widget;
        te::map::AbstractLayerPtr m_selectedLayer;  //!< Layer 
        std::vector<std::string> m_selectedProps; //!< Selected properties related to the selected Layer
        std::vector<std::string> m_layersOnTheRight; //!< Layers on the right in the DoubleList Widget
        std::list<te::map::AbstractLayerPtr> m_layersSelected; //!< Vector that will store selected layers (on the rigth DoubleList Widget)

        virtual std::list<te::map::AbstractLayerPtr> getLayers();
        virtual std::list<te::map::AbstractLayerPtr> getSelectedLayers();

        void initDouble(QWidget* widget, std::string nameComponent);
        void initCombo(QWidget* widget, std::string nameComponent);
        void initBool(QWidget* widget, std::string nameComponent);
        double mm2cm(double mmSize);
        double cm2mm(double cmSize);
        std::string formatScaleValue(std::string inputValue);
        virtual void loadScaleCombobox();
    };
  }    
}     

#endif

