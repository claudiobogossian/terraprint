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
  \file terralib/layout/qt/outside/LegendChoiceOutside.cpp

  \brief A dialog configure the input layer to address geocoding operation
*/

// TerraLib
#include "LegendChoiceOutside.h"
#include "ui_LegendChoice.h"
#include "../../outside/LegendChoiceModel.h"
#include "../../core/pattern/mvc/AbstractOutsideModel.h"
#include "../../core/pattern/mvc/AbstractOutsideController.h"
#include "../core/ItemUtils.h"

// STL
#include <string>

// Qt
#include <QGridLayout>

te::layout::LegendChoiceOutside::LegendChoiceOutside(AbstractOutsideController* controller, QWidget* parent)
  : QDialog(parent),
  AbstractOutsideView(controller),
  m_ui(new Ui::LegendChoice)
{
  // add controls
  m_ui->setupUi(this);

  m_widget.reset(new DoubleTreeWidgetOutside(m_ui->m_widget));

  QGridLayout* displayLayout = new QGridLayout(m_ui->m_widget);
  displayLayout->addWidget(m_widget.get());

  QString qLeftLabel = tr("Available Layer Legends");
  std::string leftLabel = ItemUtils::convert2StdString(qLeftLabel);

  QString qRightLabel = tr("Selected Layer Legends");
  std::string rightLabel = ItemUtils::convert2StdString(qRightLabel);

  m_widget->setLeftLabel(leftLabel);
  m_widget->setRightLabel(rightLabel);

  connect(m_ui->m_okPushButton, SIGNAL(clicked()), this, SLOT(onOkPushButtonClicked()));
  connect(m_ui->m_cancelPushButton, SIGNAL(clicked()), this, SLOT(onCancelPushButtonClicked()));
}

te::layout::LegendChoiceOutside::~LegendChoiceOutside()
{

}

void te::layout::LegendChoiceOutside::init()
{
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  LegendChoiceModel* model = dynamic_cast<LegendChoiceModel*>(abstractModel);
  if (!model)
  {
    return;
  }

  std::vector <std::string> namesToInput;
  std::vector <std::string> namesToOutput;

  // Layers From Map Items
  std::list<te::map::AbstractLayerPtr> selectedLayers = model->getSelectedLayers();

  // All Layers from Project
  std::list<te::map::AbstractLayerPtr> layers = model->getLayers();

  for (std::list<te::map::AbstractLayerPtr>::iterator it = selectedLayers.begin(); it != selectedLayers.end(); ++it)
  {
    te::map::AbstractLayerPtr layer = (*it);
    if (std::find(layers.begin(), layers.end(), layer) != layers.end())
    {
      std::list<te::map::AbstractLayerPtr>::iterator findIt = std::find(layers.begin(), layers.end(), layer);
      namesToOutput.push_back(layer->getTitle());
    }
  }

  namesToInput = intersectionLayersTitle(namesToOutput);

  m_widget->setInputValues(namesToInput);
  m_widget->setOutputValues(namesToOutput);
}

void te::layout::LegendChoiceOutside::onOkPushButtonClicked()
{
  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  LegendChoiceModel* model = dynamic_cast<LegendChoiceModel*>(abstractModel);
  if (!model)
  {
    return;
  }

  m_layersOnTheRight = m_widget->getOutputValues();

  std::list<te::map::AbstractLayerPtr> layerListMap = model->getLayers();

  std::vector<std::string>::iterator itString = m_layersOnTheRight.begin();
  for (; itString != m_layersOnTheRight.end(); ++itString)
  {
    std::list<te::map::AbstractLayerPtr>::iterator it = layerListMap.begin();
    for (; it != layerListMap.end(); ++it)
    {
      te::map::AbstractLayerPtr layer = it->get();
      std::string nameLayer = layer->getTitle();

      QString qNameLayer(nameLayer.c_str());
      nameLayer = ItemUtils::convert2StdString(qNameLayer);

      std::string name = (*itString);
      if (nameLayer.compare(name) == 0)
      {
        m_layersSelected.push_back(layer);
      }
    }
  }

  EnumDataType* dataType = Enums::getInstance().getEnumDataType();

  // Layers From Map Items
  std::list<te::map::AbstractLayerPtr> selectedLayers = model->getSelectedLayers();

  if (selectedLayers == m_layersSelected)
  {
    m_layersSelected.clear();
    accept();
    return;
  }

  Property prop;
  prop.setName("layers");
  prop.setValue(m_layersSelected, dataType->getDataTypeLayerList());

  m_layersSelected.clear();

  emit updateProperty(prop);

  accept();
}

void te::layout::LegendChoiceOutside::onCancelPushButtonClicked()
{
  reject();
}

void te::layout::LegendChoiceOutside::setPosition(const double& x, const double& y)
{
  move(x, y);
  refresh();
}

te::gm::Coord2D te::layout::LegendChoiceOutside::getPosition()
{
  QPointF posF = pos();
  qreal valuex = posF.x();
  qreal valuey = posF.y();

  te::gm::Coord2D coordinate;
  coordinate.x = valuex;
  coordinate.y = valuey;

  return coordinate;
}

std::vector<std::string> te::layout::LegendChoiceOutside::intersectionLayersTitle(std::vector<std::string> output)
{
  std::vector <std::string> namesToInput;

  AbstractOutsideModel* abstractModel = const_cast<AbstractOutsideModel*>(m_controller->getModel());
  LegendChoiceModel* model = dynamic_cast<LegendChoiceModel*>(abstractModel);
  if (!model)
  {
    return namesToInput;
  }

  // All Layers from Project

  std::list<te::map::AbstractLayerPtr> layers = model->getLayers();

  for (std::list<te::map::AbstractLayerPtr>::iterator it = layers.begin(); it != layers.end(); ++it)
  {
    te::map::AbstractLayerPtr layer = (*it);
    std::string nameLayer = layer->getTitle();
    if (std::find(output.begin(), output.end(), nameLayer) == output.end())
    {
      namesToInput.push_back(layer->getTitle());
    }
  }

  return namesToInput;
}

