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
  \file TextItem.h
   
  \brief Class that represents text. This object is of type QGraphicsTextItem. He is directly editable via user interaction. 
  His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
  but its position in the scene remains in millimeters.
  This is also son of AbstractItem, so it can become observer of a model (Observable). "View" part of MVC component.
  This item will only accept "plain text", no "rich texts".

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H
#define __TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H

// TerraLib
#include "../../core/Config.h"
#ifndef Q_MOC_RUN
#include "AbstractItem.h"
#endif

//Qt
class QTextDocument;
class QTextCursor;
class QTimer;

namespace te
{
  namespace layout
  {
    class AbstractItemController;
    class AbstractItemModel;
    /*!
    \brief Class that represents text. This object is of type QGraphicsTextItem. He is directly editable via user interaction. 
    His transformation matrix is inverted, that is, the inverse of the matrix of the scene, so its coordinate system is screen (pixel), 
    but its position in the scene remains in millimeters.
    Drawing starting point is llx, lly.
    This is also the son of AbstractItem, so it can become observer of a model (Observable). "View" part of MVC component.
    This item will only accept "plain text", no "rich texts".

    \ingroup layout

    \sa te::layout::AbstractItem
  */
    class TELAYOUTEXPORT TextItem : public QObject, public AbstractItem
    {
      Q_OBJECT

      public:

        /*!
          \brief Constructor

          \param controller "Controller" part of MVC component
          \param o "Model" part of MVC component
        */ 
        TextItem();

        /*!
          \brief Destructor
        */
        virtual ~TextItem();

        /*!
        \brief Sets the text document to be displayed
        */
        virtual void setDocument(QTextDocument* textDocument);

        /*!
        \brief Reimplemented from QGraphicsItem
        */
        virtual QRectF boundingRect() const;

        virtual void refresh();

        /*!
          \brief For any specific drawing, the item must reimplement this function
        */
        virtual void drawItem ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

      protected:

        virtual AbstractItemModel* createModel() const;

        virtual AbstractItemController* createController() const;

        /*!
        \brief Reimplemented from QGraphicsTextItem
        */
        virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
        
        /*!
        \brief Reimplemented from QGraphicsTextItem
        */
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

        /*!
          \brief Reimplemented from QGraphicsTextItem
         */
        virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

        /*!
        \brief Reimplemented from QGraphicsTextItem
        */
        virtual void keyPressEvent(QKeyEvent * event);

        /*!
          \brief Enters the edition mode
         */
        virtual void enterEditionMode();

        /*!
          \brief Leaves edition mode
         */
        virtual void leaveEditionMode();

        /*!
        \brief This function is called when the edtion of a document has fineshed, and it must set the new values in the controller
        */
        virtual void documentEditionFinished();

        virtual void updateBlockEditionRange();

      protected slots:

        void timerEvent();

      protected:

        QTextDocument*  m_document; //!< The text document of the text item
        QTextCursor*    m_textCursor; //!< The cursor object that is used to navigate and manipulate the text document
        QTimer*         m_cursorTimer;
        bool            m_showCursor;
        int             m_blockEditionRangeStart;
        int             m_blockEditionRangeEnd;

    };
  }
}
#endif //__TERRALIB_LAYOUT_INTERNAL_TEXT_ITEM_H
