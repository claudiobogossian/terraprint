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

#ifndef __TERRALIB_LAYOUT_INTERNAL_EXAMPLES_PROPERTY_EDITOR_H
#define __TERRALIB_LAYOUT_INTERNAL_EXAMPLES_PROPERTY_EDITOR_H

// TerraLib
#ifndef Q_MOC_RUN
#include <terralib/layout/qt/core/View.h>
#endif

// STL
#include <memory>

// Qt
#include <QWidget>

class QComboBox;
class QString;
class QGraphicsItem;

namespace Ui { class PropertyEditorExampleDialog; }

namespace te
{
  namespace gm
  {
    struct Coord2D;
  }
  namespace layout
  {
    class RectangleItem;
    class MapItem;
    class MapCompositionItem;
    class PropertyTree;
    class EnumType;

    namespace example
    {
      namespace propertyeditor
      {
        class ProxyLayers;
        /*!
          \brief

          \ingroup layout
          */
        class PropertyEditorExample : public QWidget
        {
          Q_OBJECT

        public:

          /*!
          \brief Constructor.
          */
          PropertyEditorExample(QWidget *parent = 0);

          /*!
          \brief Destructor.
          */
          virtual ~PropertyEditorExample();

          void run();

        protected slots:

          void onCurrentIndexChanged(const QString & text);

          void on_tbtnLoadLayers_clicked();

        protected:
          
          void createGraphicsViewInfrastructure();

          void createRectangleItem();

          void createMapItem();

          void createMapCompositionItem();

          QGraphicsItem* createItem(EnumType* itemType, te::gm::Coord2D& coord, double width = 20, double height = 20);

          void createPropertyTree();

          void createLayout();

          void loadComboboxNames();

          void loadProperties(QGraphicsItem* item);

          void loadLayerTree();
          
        private:

          std::unique_ptr<Ui::PropertyEditorExampleDialog>  m_ui;
          std::unique_ptr<te::layout::View>                 m_view;
          te::layout::RectangleItem*                        m_rectItem;
          te::layout::MapItem*                              m_mapItem;
          te::layout::MapCompositionItem*                   m_mapCompositionItem;
          te::layout::PropertyTree*                         m_tree;
          QComboBox*                                        m_combobox;
          ProxyLayers*                                      m_proxy;
        };
      }
    }
  }
}

#endif

