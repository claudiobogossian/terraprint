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
  \file terralib/layout/qt/outside/LegendChoiceOutside.h

  \brief 
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_LEGEND_CHOICE_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_LEGEND_CHOICE_OUTSIDE_H

// TerraLib
#include "terralib/qt/widgets/utils/DoubleListWidget.h"
#ifndef Q_MOC_RUN
#include "../../core/pattern/mvc/AbstractOutsideView.h"
#include "../../core/Config.h"
#include "../../core/property/Property.h"
#include <terralib/color/RGBAColor.h>
#endif

// STL
#include <map>
#include <memory>
#include <list>

// Qt
#include <QDialog>

class QKeyEvent;
class QString;
class QComboBox;

namespace Ui { class LegendChoice; }

namespace te
{
  namespace layout
  {
    class AbstractOutsideController;
    class Variant;

    class TELAYOUTEXPORT LegendChoiceOutside : public QDialog, public AbstractOutsideView
    {
      Q_OBJECT

      public:

        LegendChoiceOutside(AbstractOutsideController* controller, QWidget* parent);

        ~LegendChoiceOutside();
        
        /*!
        \brief Load layers in double widget.
        */
        virtual void init();

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

        virtual Property getSavedLayers();

      signals:

        void updateProperty(Property prop);

        void updateProperties(std::vector<Property> props);

      protected slots:

        void onOkPushButtonClicked();

        void onCancelPushButtonClicked();

        void on_m_btnLayerTitleFont_clicked();

        void on_m_btnHierarchyFont_clicked();

        void on_m_btnLayerTitleColor_clicked();

        void on_m_btnHierarchyFontColor_clicked();

        void on_m_txtSymbolSize_editingFinished();

        void on_m_txtRows_editingFinished();

        void on_m_txtDisplacementBetweenSymbols_editingFinished();

        void on_m_txtDisplacementBorder_editingFinished();

        void on_m_txtDisplacementBetweenTitles_editingFinished();

        void on_m_txtDisplacementBetweenColumns_editingFinished();

        void on_m_txtDisplacementBetweenSymbolsAndTexts_editingFinished();

        void on_m_txtDisplacementHierarchyPair_editingFinished();

        void on_m_cmbConnectedTo_currentIndexChanged(const QString & text);

      protected:

        std::vector<std::string> intersectionLayersTitle(const std::vector<std::string>& output);

        virtual std::list<te::map::AbstractLayerPtr> getLayers();

        virtual std::list<te::map::AbstractLayerPtr> getSelectedLayers();

        virtual Property getProperty(const std::string& name);

        virtual void load();

        virtual void keyPressEvent(QKeyEvent * e);

        void initDouble(QWidget* widget, const std::string& nameComponent);

        void initCombo(QWidget* widget, const std::string& nameComponent);

        void initInt(QWidget* widget, const std::string& nameComponent);

        void initColor(QWidget* widget, const std::string& nameComponent);

        void initTextEdit(QWidget* widget, const std::string& nameComponent, const std::string& nameFontComponent);

        QString fontHTML(const Property& prop, const std::string& nameFontColorComponent);

        te::color::RGBAColor configColor(QWidget* widget);

        virtual Font configFont(QWidget* widget, const std::string& nameFontComponent, const std::string& nameFontColorComponent);

        Font qFont2Font(QFont qFont);

        QFont font2QFont(Font font);

        virtual void addComboOptions(QComboBox* combo, const std::vector<Variant>& options);

        virtual void loadDoubleListWidget();

      private:

        std::auto_ptr<Ui::LegendChoice> m_ui;
        std::auto_ptr<te::qt::widgets::DoubleListWidget> m_widget;

        te::map::AbstractLayerPtr m_selectedLayer;  //!< Layer 
        std::vector<std::string> m_selectedProps; //!< Selected properties related to the selected Layer
        std::vector<std::string> m_layersOnTheRight; //!< Layers on the right in the DoubleList Widget

        std::list<te::map::AbstractLayerPtr> m_layersSelected; //!< Vector that will store selected layers (on the rigth DoubleList Widget)
    };
  }   
}    

#endif



