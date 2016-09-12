#include "AbstractItem.h"

te::layout::AbstractItem::AbstractItem(AbstractItemController* controller)
  : QGraphicsItem()
  , AbstractItemView(controller)
  , m_enumSides(TPNoneSide)
  , m_currentAction(te::layout::NO_ACTION)
  , m_marginResizePrecision(5.)
  , m_selectionPointSize(10.)
  , m_isPrinting(false)
{
  this->setFlags(QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable
    | QGraphicsItem::ItemSendsGeometryChanges
    | QGraphicsItem::ItemIsFocusable);

  //If enabled is true, this item will accept hover events
  this->setAcceptHoverEvents(true);
}

te::layout::AbstractItem::~AbstractItem()
{
}

QRectF te::layout::AbstractItem::boundingRect() const
{
  if (m_currentAction == te::layout::RESIZE_ACTION)
  {
    return m_rect;
  }

  //models stores size information in item CS. 
  double width = m_controller->getProperty("width").getValue().toDouble();
  double height = m_controller->getProperty("height").getValue().toDouble();

  QRectF boundingRect(0, 0, width, height);
  return boundingRect;
}

void te::layout::AbstractItem::refresh()
{
  this->update();
}

void te::layout::AbstractItem::contextUpdated(const ContextObject& context)
{
  EnumModeType* enumMode = Enums::getInstance().getEnumModeType();

  m_isPrinting = false;

  if (context.getCurrentMode() == enumMode->getModePrinterPreview())
  {
    m_isPrinting = true;
    return;
  }
  else if (context.getCurrentMode() == enumMode->getModePrinter())
  {
    m_isPrinting = true;

    return;
  }

  this->update();
}


double te::layout::AbstractItem::getItemRotation() const
{
  return this->rotation();
}

void te::layout::AbstractItem::setItemRotation(double angle)
{
  QPointF center = boundingRect().center();
  QTransform currentTransform = this->transform();

  //checks if the is a negative scalling
  //this is usually true form texts, images, etc., due to the inverted CS
  if (currentTransform.m22() < 0)
  {
    angle = angle * -1;
  }

  if (angle == 0.)
    this->setTransformOriginPoint(0, 0);
  else
    this->setTransformOriginPoint(boundingRect().width() / 2., boundingRect().height() / 2.);

  this->setRotation(angle);
}

void te::layout::AbstractItem::setItemPosition(double x, double y)
{
  this->setPos(x, y);
}

void te::layout::AbstractItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0 */)
{
  Q_UNUSED(option);
  Q_UNUSED(widget);
  if (painter == 0)
  {
    return;
  }

  if ((m_currentAction == te::layout::RESIZE_ACTION || m_currentAction == te::layout::MOVE_ACTION) && m_useResizePixmap)
  {
    drawItemResized(painter, option, widget);
    drawFrame(painter);
    drawSelection(painter);
    return;
  }

  painter->save();

  //Draws the background
  drawBackground(painter);

  //Draws the item
  drawItem(painter, option, widget);
  if (m_controller->getWarningManager()->hasWarning() && (m_isPrinting == false))
  {
    drawWarningAlert(painter);
  }
      
  //Draws the frame
  drawFrame(painter);

  //Draws the selection
  if (option->state & QStyle::State_Selected || m_subSelected == true)
  {
    drawSelection(painter);
  }

  painter->restore();
}

void te::layout::AbstractItem::enterEditionMode()
{
  if (this->parentItem() != 0)
  {
    this->parentItem()->setHandlesChildEvents(false);
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, false);
  }
}

void te::layout::AbstractItem::leaveEditionMode()
{
  if (this->parentItem() != 0)
  {
    this->parentItem()->setHandlesChildEvents(true);
    this->setFlag(QGraphicsItem::ItemStacksBehindParent, true);
  }
}

QRectF te::layout::AbstractItem::getAdjustedBoundingRect(QPainter* painter) const
{
  qreal penWidth = painter->pen().widthF();
  if (painter->pen().style() == Qt::NoPen)
  {
    penWidth = 0.;
  }

  QRectF bRect = boundingRect();

  qreal adj = penWidth / 2.;
  QRectF rectAdjusted = bRect.adjusted(adj, adj, -adj, -adj);

  return rectAdjusted;
}

void te::layout::AbstractItem::drawBackground(QPainter * painter)
{
  if (!painter)
  {
    return;
  }

  const Property& pBackgroundColor = m_controller->getProperty("background_color");
  const te::color::RGBAColor& backgroundColor = pBackgroundColor.getValue().toColor();
  QColor qBackgroundColor(backgroundColor.getRed(), backgroundColor.getGreen(), backgroundColor.getBlue(), backgroundColor.getAlpha());

  painter->save();
  painter->setPen(Qt::NoPen);
  painter->setBrush(QBrush(qBackgroundColor));
  painter->setBackground(QBrush(qBackgroundColor));
  painter->setRenderHint(QPainter::Antialiasing, true);

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  //draws the background
  painter->drawRect(rectAdjusted);

  painter->restore();
}

void te::layout::AbstractItem::drawFrame(QPainter * painter)
{
  if (!painter)
  {
    return;
  }

  if (m_controller->getProperty("show_frame").getValue().toBool() == false)
  {
    return;
  }

  const Property& pFrameColor = m_controller->getProperty("frame_color");
  const Property& pFrameThickness = m_controller->getProperty("frame_thickness");
  const te::color::RGBAColor& frameColor = pFrameColor.getValue().toColor();
  double frameThickness = pFrameThickness.getValue().toDouble();

  QColor qFrameColor(frameColor.getRed(), frameColor.getGreen(), frameColor.getBlue(), frameColor.getAlpha());

  painter->save();
  QPen pen(qFrameColor, frameThickness, Qt::SolidLine);
  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);
  painter->setRenderHint(QPainter::Antialiasing, true);

  //gets the adjusted boundigng rectangle based of the painter settings
  QRectF rectAdjusted = getAdjustedBoundingRect(painter);

  //draws the frame
  painter->drawRect(rectAdjusted);

  painter->restore();
}

void te::layout::AbstractItem::drawSelection(QPainter* painter)
{
  if (!painter)
  {
    return;
  }

  const Property& pFrameThickness = m_controller->getProperty("frame_thickness");
  double frameThickness = pFrameThickness.getValue().toDouble();

  painter->save();

  const QColor fgcolor(0, 255, 0);
  const QColor backgroundColor(0, 0, 0);

  QPen penBackground(backgroundColor, frameThickness, Qt::SolidLine);
  painter->setPen(penBackground);
  painter->setBrush(Qt::NoBrush);

  //gets the adjusted boundigng rectangle based of the painter settings
  //QRectF rectAdjusted = getAdjustedBoundingRect(painter);
  QRectF rectAdjusted = boundingRect();
      
  QPen penForeground(fgcolor, frameThickness, Qt::DashLine);
  painter->setPen(penForeground);
  painter->setBrush(Qt::NoBrush);

  double half = m_selectionPointSize / 2;

  int tempRotation = this->getItemRotation();
  this->setItemRotation(0);
  QRectF convertedRect =  qRectToQPolygonMap(rectAdjusted);
  this->setItemRotation(tempRotation);

  qreal penWidth = 3;
  if (painter->pen().style() == Qt::NoPen)
  {
    penWidth = 0.;
  }

  QRectF bRect = convertedRect;

  qreal adj = std::floor(penWidth / 2.);
  convertedRect = bRect.adjusted(adj, adj, -adj, -adj);

  QPointF topL = convertedRect.topLeft();

  QPointF topR = convertedRect.topRight();

  QPointF bottomR = convertedRect.bottomRight();

  QPointF bottomL = convertedRect.bottomLeft();

  QPointF center = convertedRect.center();

      
  QLineF lineDown(bottomL, bottomR);
  QLineF lineUp(topL, topR);
  QLineF lineRight(bottomL, topL);
  QLineF lineLeft(bottomR, topR);

  double rotation = getItemRotation();

  QTransform transform;
  painter->setTransform(transform);

  double transformWidth = convertedRect.width();
  double transformHeight = convertedRect.height();

  QPointF originPoint(transformWidth / 2, transformHeight / 2);

  //this->setTransformOriginPoint(originPoint);

  double translateX = convertedRect.topLeft().x() + transformWidth / 2;
  double translateY = convertedRect.topLeft().y() + transformHeight / 2;
  transform.translate(translateX, translateY);
  transform.rotate(-rotation);
  transform.translate(-translateX, -translateY);
  painter->setTransform(transform);

  penBackground.setWidthF(penWidth);
  painter->setPen(penBackground);
  painter->setBrush(Qt::NoBrush);

  QRectF selectionRect(topL, bottomR);
      
  painter->drawLine(lineDown);
  painter->drawLine(lineUp);
  painter->drawLine(lineRight);
  painter->drawLine(lineLeft);

  penForeground.setWidthF(penWidth);
  painter->setPen(penForeground);
  painter->setBrush(Qt::NoBrush);

  painter->drawRect(selectionRect);

  QPen pen;
  pen.setColor(QColor(255, 0, 0));
  pen.setStyle(Qt::DotLine);

  QBrush brushEllipse(fgcolor);
  painter->setBrush(fgcolor);

  pen.setWidthF(m_selectionPointSize);
  painter->setPen(pen);
  painter->drawPoint(topL.x()+ half, topL.y() + half);
  painter->drawPoint(topR.x() - half, topR.y() + half);
  painter->drawPoint(bottomR.x() - half, bottomR.y() - half);
  painter->drawPoint(bottomL.x() + half, bottomL.y() - half);
  painter->drawPoint(center.x(), center.y());

  double rectSize = 16;

  QRectF rectRotation((convertedRect.x() + (convertedRect.width() / 2)) - (rectSize / 2), convertedRect.y() + 8, rectSize, rectSize);

  QPainterPath Ppath;
      
  pen.setColor(QColor(0, 0, 0));
  pen.setStyle(Qt::SolidLine);
  pen.setWidthF(1.5);
  painter->setPen(pen);
  painter->setBrush(Qt::NoBrush);

  QPointF centerRight = rectRotation.center();
  centerRight.setX(centerRight.x() + (rectRotation.width() / 2));
  Ppath.moveTo(centerRight);

  Ppath.lineTo(centerRight.x() + 3, centerRight.y() - 3);
  Ppath.moveTo(centerRight);
  Ppath.lineTo(centerRight.x() - 3, centerRight.y() - 3);
  Ppath.moveTo(centerRight);
  Ppath.arcTo(rectRotation, 0.0, 150.0);

  QPointF centerLeft = rectRotation.center();
  centerLeft.setX(centerLeft.x() - (rectRotation.width() / 2));

  Ppath.moveTo(centerLeft);

  Ppath.lineTo(centerLeft.x() - 3, centerLeft.y() + 3);
  Ppath.moveTo(centerLeft);
  Ppath.lineTo(centerLeft.x() + 3, centerLeft.y() + 3);
  Ppath.moveTo(centerLeft);
  Ppath.arcTo(rectRotation, 180.0, 150.0);


  painter->drawPath(Ppath);

  painter->restore();
}

QRectF te::layout::AbstractItem::qRectToQPolygonMap(QRectF rect)
{
       
  QGraphicsScene* scene = this->scene();
  QGraphicsView* view = scene->views().first();

  QRectF convRect = this->mapRectToScene(rect);
  QRectF rectConverted = view->mapFromScene(convRect).boundingRect();

  return rectConverted;
}

QVariant te::layout::AbstractItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant & value)
{
  if (change == QGraphicsItem::ItemPositionHasChanged)
  {
    if (m_currentAction == te::layout::NO_ACTION)
    {
      m_controller->itemPositionChanged(this->pos().x(), this->pos().y());
    }
  }
  else if (change == QGraphicsItem::ItemZValueHasChanged)
  {
    m_controller->itemZValueChanged(this->zValue());
  }

  return QGraphicsItem::itemChange(change, value);
}

void te::layout::AbstractItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
  if (isEditionMode() == false && m_controller->getProperty("resizable").getValue().toBool())
  {
    checkTouchesCorner(event->pos().x(), event->pos().y());
  }
  checkTouchesWarningAlert(event->pos().x(), event->pos().y());
  QGraphicsItem::hoverMoveEvent(event);
}

void te::layout::AbstractItem::drawWarningAlert(QPainter * painter)
{
  if (!painter)
  {
    return;
  }

  painter->save();

  QColor qFillColor(255, 0, 0);
  QColor qContourColor(255, 0, 0);

  QBrush brush(qFillColor);
  QPen pen(qContourColor, 0.5, Qt::SolidLine);

  painter->setPen(pen);
  painter->setBrush(brush);

  QRectF rectAdjusted = boundingRect();

  QTransform t;
  painter->setTransform(t);

  QRectF convertedRect = qRectToQPolygonMap(rectAdjusted);

  QRectF bRect = convertedRect;

  qreal adj = std::ceil(0.5 / 2.);
  convertedRect = bRect.adjusted(adj, adj, -adj, -adj);

  QPointF p1 = QPointF(convertedRect.topRight().x(), convertedRect.topRight().y());
  QPointF p2 = QPointF(convertedRect.topRight().x() - 40, convertedRect.topRight().y());
  QPointF p3 = QPointF(convertedRect.topRight().x(), convertedRect.topRight().y() + 40);

  QPolygonF warningPolygon;

  warningPolygon << p1 << p2 << p3 << p1;

  m_polygonWarning = warningPolygon;

  //draws the item
  painter->drawPolygon(warningPolygon);

  QColor qContourColorRed(0, 0, 0);
  pen.setStyle(Qt::DashLine);
  pen.setColor(qContourColorRed);
  painter->setPen(pen);

  painter->drawPolygon(warningPolygon);


  QString warningString = "!";

  qFillColor.setRed(0);
  qFillColor.setGreen(0);
  qFillColor.setBlue(0);

  qContourColor.setRed(0);
  qContourColor.setGreen(0);
  qContourColor.setBlue(0);

  brush.setColor(qFillColor);
  pen.setColor(qContourColor);

  painter->setPen(pen);
  painter->setBrush(brush);

  QFont font;
  font.setPointSize(60);
  font.setFamily("Arial");


  double adjX = warningPolygon.boundingRect().center().x() + 6;
  double adjY = warningPolygon.boundingRect().center().y() - 12;

  QPointF txtPoint(adjX, adjY);

  ItemUtils::drawText(txtPoint, painter, font, "!", 180);

  painter->restore();

}

void te::layout::AbstractItem::checkTouchesWarningAlert(const double& x, const double& y/*, QPainter * painter*/)
{
  QRectF rectAdjusted = boundingRect();
  QRectF remapRect = qRectToQPolygonMap(rectAdjusted);

  QGraphicsScene* scene = this->scene();
  QGraphicsView* view = scene->views().first();


  QPointF mousePoint(x, y);

  QPointF convPoint = this->mapToScene(mousePoint);
  QPointF remapedPoint = view->mapFromScene(convPoint);

  QPointF checkPoint(remapedPoint.x(), remapedPoint.y());


  QPoint tipPoint = QCursor::pos();

  std::vector<std::string> warningVect = m_controller->getWarningManager()->getWarnings();

  std::string warningsMsg = "";

  for (int i = 0; i < warningVect.size(); i++)
  {
    if (i == 0)
    {
      warningsMsg = warningsMsg + warningVect.at(i);
    }
    else
    {
      warningsMsg = warningsMsg + "\n" + warningVect.at(i);
    }
  }

  QString qWarningsMsg = ItemUtils::convert2QString(warningsMsg);

  if (m_polygonWarning.containsPoint(checkPoint, Qt::OddEvenFill))
  {
    QToolTip::showText(tipPoint, qWarningsMsg, view);
  }
}

bool te::layout::AbstractItem::checkTouchesCorner(const double& x, const double& y)
{
  bool result = true;

  QRectF bRect = boundingRect();
      
  int tempRotation = this->getItemRotation();
  this->setItemRotation(0);
  QRectF remapRect = qRectToQPolygonMap(bRect);
  this->setItemRotation(tempRotation);

  QPointF ll = remapRect.bottomLeft();
  QPointF lr = remapRect.bottomRight();
  QPointF tl = remapRect.topLeft();
  QPointF tr = remapRect.topRight();

  QGraphicsScene* scene = this->scene();
  QGraphicsView* view = scene->views().first();

  QPointF mousePoint(x, y);

  QPointF convPoint = this->mapToScene(mousePoint);
  QPointF remapedPoint = view->mapFromScene(convPoint);



  Utils utils = this->getScene()->getUtils();
  double marginPixel =  utils.mm2pixel(m_marginResizePrecision);
      
  double w = marginPixel;
  double h = marginPixel;
  double half = marginPixel / 2.;

  QRectF smallLeftTopRect(remapRect.topLeft().x(), remapRect.topLeft().y() - half, w, h); // left-top
  QRectF smallRightTopRect(remapRect.topRight().x() - half, remapRect.topRight().y() - half, w, h); // right-top
  QRectF smallLeftBottomRect(remapRect.bottomLeft().x(), remapRect.bottomLeft().y() - half, w, h); // left-bottom
  QRectF smallRightBottomRect(remapRect.bottomRight().x() - half, remapRect.bottomRight().y() - half, w, h); // right-bottom

  QRectF leftRect(smallLeftTopRect.topLeft(), smallLeftBottomRect.bottomRight());
  QRectF rightRect(smallRightTopRect.topLeft(), smallRightBottomRect.bottomRight());
  QRectF topRect(smallLeftTopRect.topLeft(), smallRightTopRect.bottomRight());
  QRectF bottomRect(smallLeftBottomRect.topLeft(), smallRightBottomRect.bottomRight());

           
  QPointF checkPoint(remapedPoint.x(), remapedPoint.y());

  Property pKeepAspect = m_controller->getProperty("keep_aspect");

  bool keepAspect = pKeepAspect.getValue().toBool();

  if (smallLeftTopRect.contains(checkPoint))
  {
    this->setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPTopLeft;
  }
  else if (smallRightTopRect.contains(checkPoint))
  {
    this->setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPTopRight;
  }
  else if (smallLeftBottomRect.contains(checkPoint))
  {
    this->setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPLowerLeft;
  }
  else if (smallRightBottomRect.contains(checkPoint))
  {
    this->setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPLowerRight;
  }

  else if (keepAspect == false && leftRect.contains(checkPoint))
  {
    this->setCursor(Qt::SizeHorCursor);
    m_enumSides = TPLeft;
  }
  else if (keepAspect == false && rightRect.contains(checkPoint))
  {
    this->setCursor(Qt::SizeHorCursor);
    m_enumSides = TPRight;
  }
  else if (keepAspect == false && topRect.contains(checkPoint))
  {
    this->setCursor(Qt::SizeVerCursor);
    m_enumSides = TPTop;
  }
  else if (keepAspect == false && bottomRect.contains(checkPoint))
  {
    this->setCursor(Qt::SizeVerCursor);
    m_enumSides = TPLower;
  }
  else
  {
    this->setCursor(Qt::ArrowCursor);
    m_enumSides = TPNoneSide;
    result = false;
  }

  return result;
}


bool te::layout::AbstractItem::checkRotationArea(const double& x, const double& y)
{
    Utils utils = this->getScene()->getUtils();
        
    QGraphicsScene* scene = this->scene();
    QGraphicsView* view = scene->views().first();

    int zoom = this->getScene()->getContext().getZoom();
    double zoomFactor = zoom / 100.;
    double rectSize = 16.;
    double rectSizeMM = utils.pixel2mm(rectSize) / zoomFactor;
    QPointF referencePointInItemCS(this->boundingRect().width() / 2., this->boundingRect().height() - rectSizeMM);

    QPointF referencePointInSceneCS = this->mapToScene(referencePointInItemCS);
    QPointF referencePointInViewCS = view->mapFromScene(referencePointInSceneCS);

    QPointF mousePoint(x, y);
    QRectF rectRotation(referencePointInItemCS.x() - (rectSizeMM / 2.), referencePointInItemCS.y() - (rectSizeMM / 2.), rectSizeMM, rectSizeMM);
        
    if (rectRotation.contains(mousePoint))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void te::layout::AbstractItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  //checks if the item is resizable.
  const Property& property = m_controller->getProperty("resizable");
  if (property.getValue().toBool() == true)
  {
    //If so, checks if the resize operation must be started
    bool startResizing = checkTouchesCorner(event->pos().x(), event->pos().y());


        
    if (startResizing == true)
    {
      m_rect = boundingRect();
      setPixmap();
      m_currentAction = te::layout::RESIZE_ACTION;
      m_initialCoord = event->pos();
    }
       
  }

  bool startRotation = checkRotationArea(event->pos().x(), event->pos().y());
  if (startRotation == true)
  {
      m_currentAction = te::layout::ROTATION_ACTION;

      QRectF bRect = boundingRect();

      QRectF remapRect = qRectToQPolygonMap(bRect);

      QGraphicsScene* scene = this->scene();
      QGraphicsView* view = scene->views().first();

      QPointF mousePoint(event->pos().x(), event->pos().y());

      QPointF convPoint = this->mapToScene(mousePoint);
      QPointF remapedPoint = view->mapFromScene(convPoint);

      QPointF checkPoint(remapedPoint.x(), remapedPoint.y());

  }

  QGraphicsItem::mousePressEvent(event);
}

void te::layout::AbstractItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  if (isEditionMode() == true)
  {
    return;
  }

  if (m_currentAction == te::layout::RESIZE_ACTION)
  {
    this->setOpacity(0.5);
    m_finalCoord = event->pos();
    this->prepareGeometryChange();
    m_rect = m_controller->calculateResize(m_enumSides, m_initialCoord, m_finalCoord);

    QGraphicsScene* scene = this->scene();
    QGraphicsView* view = scene->views().first();

    QPointF mousePoint(event->pos().x(), event->pos().y());
    QPointF convPoint = this->mapToScene(mousePoint);
    QPointF remapedPoint = view->mapFromScene(convPoint);

    QPointF checkPoint(remapedPoint.x(), remapedPoint.y());
    QString txt = "X: " + QString::number(mousePoint.x());
    txt = txt + ", ";
    txt = txt + "Y: " + QString::number(mousePoint.y());

    QPoint tipPoint = QCursor::pos();
    QToolTip::showText(tipPoint, txt, view);

  }
  else if (m_currentAction == te::layout::ROTATION_ACTION)
  {
    QRectF bRect = boundingRect();

    QRectF remapRect = qRectToQPolygonMap(bRect);

    QGraphicsScene* scene = this->scene();
    QGraphicsView* view = scene->views().first();

    QPointF mousePoint(event->pos().x(), event->pos().y());

    QPointF convPoint = this->mapToScene(mousePoint);
    QPointF remapedPoint = view->mapFromScene(convPoint);

    QPointF checkPoint(remapedPoint.x(), remapedPoint.y());

    double angle = Utils::calculateAngle(remapRect.center(), checkPoint);
    angle = angle - 90;

    //here we try to round the degree before setting it into the model
    double roundedAngle = std::round(angle / 90.) * 90.;
    if (fabs(roundedAngle - angle) <= 5)
    {
      angle = roundedAngle;
    }

    this->setItemRotation(-angle);
  }
  else
  {
    if (event->buttons() == Qt::LeftButton)
    {
      if(m_currentAction != te::layout::MOVE_ACTION)
        setPixmap();
      this->setOpacity(0.5);
      m_currentAction = te::layout::MOVE_ACTION;
    }

    QGraphicsItem::mouseMoveEvent(event);
  }
}

void te::layout::AbstractItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
  if (m_currentAction == te::layout::RESIZE_ACTION)
  {
    m_currentAction = te::layout::NO_ACTION;
    m_finalCoord = event->pos();
    m_controller->resize(m_enumSides, m_initialCoord, m_finalCoord);
    this->setOpacity(1.);
  }
  else if (m_currentAction == te::layout::MOVE_ACTION)
  {
    m_currentAction = te::layout::NO_ACTION;
    this->setOpacity(1.);
    m_controller->itemPositionChanged(this->pos().x(), this->pos().y());
  }
  else if (m_currentAction == te::layout::ROTATION_ACTION)
  {
    m_currentAction = te::layout::NO_ACTION;

    double rotation = getItemRotation();
    m_controller->rotated(rotation);
  }

  QGraphicsItem::mouseReleaseEvent(event);
}

void te::layout::AbstractItem::drawItemResized(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
  painter->save();
  painter->setClipRect(boundingRect());
  painter->drawPixmap(boundingRect(), m_clonePixmap, m_clonePixmap.rect());
  painter->restore();
}

void te::layout::AbstractItem::setPixmap()
{
  Utils utils = this->getScene()->getUtils();

  QRectF itemBounding = boundingRect();
  te::gm::Envelope box(0, 0, itemBounding.width(), itemBounding.height());
  box = utils.viewportBox(box);

  m_clonePixmap = QPixmap(box.getWidth(), box.getHeight());
  m_clonePixmap.fill(Qt::transparent);

  QPainter p(&m_clonePixmap);

  double resX = box.getWidth() / itemBounding.width();
  double resY = box.getHeight() / itemBounding.height();

  QTransform transform;
  transform.scale(resX, -resY);
  transform.translate(-itemBounding.bottomLeft().x(), -itemBounding.bottomLeft().y());
  p.setTransform(transform);

  QStyleOptionGraphicsItem opt;
  this->drawItem(&p, &opt, 0);

  p.end();

  QImage image = m_clonePixmap.toImage();
  image = image.mirrored();
  m_clonePixmap = QPixmap::fromImage(image);
}
    
te::layout::AbstractScene* te::layout::AbstractItem::getScene()
{
  QGraphicsScene* scene = this->scene();
  if (scene == 0)
  {
    return 0;
  }

  AbstractScene* myScene = dynamic_cast<AbstractScene*>(scene);
  if (myScene == 0)
  {
    return 0;
  }
  return myScene;
}

te::layout::ItemAction te::layout::AbstractItem::getCurrentAction()
{
  return m_currentAction;
}

void te::layout::AbstractItem::prepareGeometryChange()
{
  QGraphicsItem::prepareGeometryChange();
}
