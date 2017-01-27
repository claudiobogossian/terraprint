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
  \file ItemUtils.h
   
  \brief Utility class for manipulating items in the scene and vectorization of text and legend.

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_ITEM_UTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_ITEM_UTILS_H

// TerraLib
#include "../../core/Config.h"
#include "terralib/geometry/Point.h"
#include "../../core/property/Properties.h"

// STL
#include <vector>
#include <string>
#include <map>

// Qt
#include <QList>
#include <QPainter>
#include <QFont>
#include <QString>

class QGraphicsItem;
class QGraphicsScene;
class QFont;

namespace te
{
  namespace color
  {
    class RGBAColor;
  }
  namespace se
  {
    class PointSymbolizer;
  }
  namespace gm
  {
    class Line;
  }

  namespace layout
  {
    class MapItem;
    class EnumType;
    class MapModel;
    class GridMapItem;
    class AbstractItemView;

    /*!
      \brief Utility class for manipulating items in the scene and vectorization of text and legend.
    
      \ingroup layout
    */
    class TELAYOUTEXPORT ItemUtils
    {
      public:

        /*!
          \brief Constructor
        */ 
        explicit ItemUtils( QGraphicsScene* scene );

        /*!
          \brief Destructor
        */
        virtual ~ItemUtils();

        /*!
          \brief MVC components list of type te::layout::MapItem
      
          \param selected true if only selected te::layout::MapItem, false otherwise.
          \return MVC components list of type te::layout::MapItem
        */
        virtual std::vector<MapItem*> getMapItemList(bool selected = false);

        /*!
          \brief Search for te::layout::MapItem by name
      
          \param name of te::layout::MapItem
          \return MapItem found
        */
        virtual MapItem* getMapItem(std::string name);

        /*!
          \brief List of names te::layout::MapItem
      
          \param selected true if only selected te::layout::MapItem, false otherwise.
          \return list of names
        */
        virtual std::vector<std::string> mapNameList(bool selected = false);

        /*!
          \brief Returns the number of items added to the scene with the type
      
          \param type type to search
          \return number of items
        */
        virtual int countType(te::layout::EnumType* type);

        /*!
          \brief Search max id for a type.
      
          \param type type to search
          \return max id
        */
        virtual int maxTypeId(te::layout::EnumType* type);

        /*!
          \brief Checks whether the coordinated intersects an item and returns.

          \param x axis x coordinate, in scene CS
          \param y axis y coordinate, in scene CS

          \return 
        */
        virtual QGraphicsItem* intersectionSelectionItem(int x, int y);

        /*!
        \brief Checks whether the coordinated intersects an paper item and returns.

        \param x axis x coordinate, in scene CS
        \param y axis y coordinate, in scene CS

        \return
        */
        virtual QGraphicsItem* intersectionOnlyPaperItem(int x, int y);

        /*!
          \brief Get text boundary in mm.
      
          \param w
          \param h
        */
        virtual QRectF getTextBoundary( const std::string& fontName, int fontSize, const std::string& text ) const;
                
          /*!
          \brief Get minimum text boundary in mm.

          \param w
          \param h
          */
        virtual QRectF getMinimumTextBoundary(const std::string& fontName, int fontSize, const std::string& text) const;

        /*!
        \brief Vectorizes a text by creating a painter path that contains it
        \param text The text to be vectorizes
        \param font The font properties
        \param dpi The [optional] current DPI. If DPI is different from 72., than the resulting text point size will be ajusted to the given DPI
        \param referencePoint The [optional] reference point where the painter path must be traslated
        \param rotationAngle The [optional] rotation angle
        
        */
        static QPainterPath textToVector(const QString& text, const QFont& font, const QPointF& referencePoint = QPointF(), double rotationAngle = 0.);

        /*!
        \brief Vectorizes a text by creating a painter path that contains it
        \param text vector contains super script text
        \param font vector font used in text vector
        \param dpi The [optional] current DPI. If DPI is different from 72., than the resulting text point size will be ajusted to the given DPI
        \param referencePoint The [optional] reference point where the painter path must be traslated
        \param rotationAngle The [optional] rotation angle

        */
        static QPainterPath superscriptTextToVector(std::vector<QString> text, std::vector<QFont> font, const QPointF& referencePoint = QPointF(), double rotationAngle = 0.);

        virtual AbstractItemView* getSelectedItem();

        /*!
          \brief This function is used to conigure the painter for text drawing in a Coordinate System based on millimeters
          \param painter The painter to be configured
          \param fontFamily The font to be used
          \param fontSize The font size of the text
        */
        static void ConfigurePainterForTexts(QPainter* painter, Font txtFont);

        /*!
          \brief This function converts a object Font to QFont
          \param txtFont object Font
          \
        */
        static QFont convertToQfont(Font txtFont);

        static double getTextDPI();

        static void setTextDPI(double textDpi);

        virtual te::color::RGBAColor** changePointMarkSize(te::se::PointSymbolizer* pointSymbol, std::size_t width, std::size_t height);

        static QPainterPath lineToQPath(const te::gm::LineString& line);

        static std::string convert2StdString(const QString& qString);
        
        static QString convert2QString(const std::string& stdString);

        static void addOrUpdateProperty(const Property& property, Properties& properties);

        static void calculateAspectRatio(double widthMM, double heightMM, double& llx, double& lly, double& urx, double& ury);

        static void calculateAspectRatio(double widthMM, double heightMM, te::gm::Envelope& worldBox);

        static double calculateScaleFromBox(double widthMM, double heightMM, int srid, const te::gm::Envelope& worldBox);

        static te::gm::Envelope calculateBoxFromScale(double widthMM, double heightMM, int srid, const te::gm::Envelope& worldBox, double scale);

        static te::gm::Envelope calculateZoom(const te::gm::Envelope& envelope, double factor, bool zoomIn, const te::gm::Point& pointToCenter = te::gm::Point());

        static te::gm::Envelope calculatePan(const te::gm::Envelope& envelope, double dx, double dy);

        static QString DD2DMS(QString dd);

        static QString DMS2DD(const QString dms);

        static void DD2DMS(QString dd, QString& degreeString, QString& minuteString, QString& secondString);

        static void DMS2DD(QString degree, QString minute, QString second, QString &dd);

        static void normalizeItem(QGraphicsItem* item);
        static void normalizeChildrenPosition(QGraphicsItem* item);
        static void normalizeChildrenPosition(QRectF& parentRect, QList<QRectF>& childrenRectList);

        /*!
        \brief Draws the given image in the given location
        */
        static void drawImage(const QRectF& rectMM, QPainter* painter, const QImage& image);

        /*!
        \brief Draws the given pixmap in the given location
        */
        static void drawPixmap(const QRectF& rectMM, QPainter* painter, const QPixmap& pixmap);

        /*!
        \brief Draws the given text in the given location with rotation
        */
        static void drawText(const QPointF& pointMM, QPainter* painter, const QFont& font, const std::string& text, int rotate = 0);

        /*!
        \brief Draws the given text super script in the given location with rotation
        */
        static void drawSuperScriptText(const QPointF& pointMM, QPainter* painter, const QFont& font, const std::string& text, int rotate = 0);

        static QPainterPath getRotationSymbol(const QPointF& pos, double widthMM);

        static void prepareBoundingRectForUpdate(const QRectF& boundingRect, Properties& properties);

        static void preparePositionForUpdate(const QPointF& position, Properties& properties);

      protected:

        /*!
          \brief Returns the list of items added to a scene.

          \param selected true if only selected items, false otherwise.
          \return list of items
        */
        virtual QList<QGraphicsItem*> getItems(bool selected = false);

      protected:

        QGraphicsScene* m_scene; //!< A pointer to scene

        static double m_textDPI;
    };
  }
}

#endif 
