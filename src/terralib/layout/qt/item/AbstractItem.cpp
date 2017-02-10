#include "AbstractItem.h"

#include "../../core/enum/EnumDataType.h"
#include "../../core/enum/Enums.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "../../core/Utils.h"
#include "../core/Scene.h"

te::layout::AbstractItem::AbstractItem()
  : QGraphicsItem()
  , AbstractItemView()
  , m_enumSides(TPNoneSide)
  , m_currentAction(te::layout::NO_ACTION)
  , m_marginResizePrecision(5.)
  , m_isPrinting(false)
  , m_hotPointSizePixels(10)
  , m_selectionLineWidthPixels(3)
  , m_rotationHotPointSizePixels(16)
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
  double width = te::layout::Property::GetValueAs<double>(this->getProperty("width"));
  double height = te::layout::Property::GetValueAs<double>(this->getProperty("height"));

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
  this->prepareGeometryChange();
  this->setPos(x, y);
}

void te::layout::AbstractItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
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
    drawEdition(painter);
    return;
  }

  painter->save();

  //Draws the background
  drawBackground(painter);

  //Draws the item
  drawItem(painter, option, widget);
  if (getController()->getWarningManager()->hasWarning() && (m_isPrinting == false))
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

  drawEdition(painter);

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

  const Property& pBackgroundColor = this->getProperty("background_color");
  const te::color::RGBAColor& backgroundColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pBackgroundColor);
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

  if (te::layout::Property::GetValueAs<bool>(this->getProperty("show_frame")) == false)
  {
    return;
  }

  const Property& pFrameColor = this->getProperty("frame_color");
  const Property& pFrameThickness = this->getProperty("frame_thickness");
  const te::color::RGBAColor& frameColor = te::layout::Property::GetValueAs<te::color::RGBAColor>(pFrameColor);
  double frameThickness = te::layout::Property::GetValueAs<double>(pFrameThickness);

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

  AbstractScene* myScene = this->getScene();
  if (!myScene)
  {
    return;
  }

  te::layout::Utils utils = myScene->getUtils();

  //we need to initialize the size of the pen. To do this, we convert the expeted size in Pixels to MM. We also consider the zoom factor
  int zoom = myScene->getContext().getZoom();
  double zoomFactor = zoom / 100.;

  double penWidthSelectionMM = utils.pixel2mm(m_selectionLineWidthPixels) / zoomFactor;
  double hotPointSizeMM = utils.pixel2mm(m_hotPointSizePixels) / zoomFactor;
  double rotationHotPointSizeMM = utils.pixel2mm(m_rotationHotPointSizePixels) / zoomFactor;;
  double rotationArrowWidthMM = utils.pixel2mm(3) / 2. / zoomFactor;;

  double halfHotPointSizeMM = hotPointSizeMM / 2.;
  double halfPenWidthSelectionMM = penWidthSelectionMM / 2.;
  double halfRotationPointSizeMM = rotationHotPointSizeMM / 2.;

  QRectF boxMM = this->boundingRect();

  //if the item is not resizable, or
  //if the rect is too small to fit two hot points in the same axis plus a gap between them, we do not draw the hot points
  bool drawHotPoints = true;
  const Property& pResizable = this->getProperty("resizable");
  if (te::layout::Property::GetValueAs<bool>(pResizable) == false)
  {
    drawHotPoints = false;
  }

  painter->save();

  //we first draw the selection. A continuous green rect followed by a dotted black rect
  QPen selectionBackgroundPen(Qt::black, penWidthSelectionMM, Qt::SolidLine);
  QPen selectionForegroundPen(Qt::green, penWidthSelectionMM, Qt::DashLine);
  QPen hotPointPen(Qt::red, hotPointSizeMM, Qt::SolidLine);
  QPen rotationPen(Qt::black, rotationArrowWidthMM, Qt::SolidLine);
  QBrush hotPointBrush(Qt::red);

  //drawSelection
  QRectF adjustedBoxMM = boxMM.adjusted(halfPenWidthSelectionMM, halfPenWidthSelectionMM, -halfPenWidthSelectionMM, -halfPenWidthSelectionMM);

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setBrush(Qt::NoBrush);
  painter->setPen(selectionBackgroundPen);
  painter->drawRect(adjustedBoxMM);
  painter->setPen(selectionForegroundPen);
  painter->drawRect(adjustedBoxMM);

  //draw the hot-points
  if (!m_isEditionMode && drawHotPoints && isZoomAdequateForResize())
  {
    QRectF adjustedHotPointsRectMM = boxMM.adjusted(halfHotPointSizeMM, halfHotPointSizeMM, -halfHotPointSizeMM, -halfHotPointSizeMM);

    painter->setPen(hotPointPen);
    painter->drawPoint(adjustedHotPointsRectMM.topLeft());
    painter->drawPoint(adjustedHotPointsRectMM.topRight());
    painter->drawPoint(adjustedHotPointsRectMM.bottomRight());
    painter->drawPoint(adjustedHotPointsRectMM.bottomLeft());
    painter->drawPoint(adjustedHotPointsRectMM.center());
  }

  //the we draw the rotation hot point
  if (!m_isEditionMode && isZoomAdequateForRotation())
  {
    painter->setPen(rotationPen);
    painter->setBrush(Qt::NoBrush);

    QPointF rotationSymbolPos(boxMM.x() + boxMM.width() / 2., boxMM.y() + boxMM.height() - halfRotationPointSizeMM);
    QPainterPath rotationSymbol = ItemUtils::getRotationSymbol(rotationSymbolPos, rotationHotPointSizeMM);
    painter->drawPath(rotationSymbol);
  }

  painter->restore();
}


void te::layout::AbstractItem::drawEdition(QPainter* painter)
{
  if (m_isEditionMode == false)
  {
    return;
  }

  AbstractScene* myScene = this->getScene();
  te::layout::Utils utils = myScene->getUtils();

  int zoom = myScene->getContext().getZoom();
  double zoomFactor = zoom / 100.;

  double penWidthSelectionMM = utils.pixel2mm(m_selectionLineWidthPixels) / zoomFactor;
  double adjustValueMM = penWidthSelectionMM / 2.;

  QRectF boxtMM = this->boundingRect();
  QRectF adjustedBoxMM = boxtMM.adjusted(adjustValueMM, adjustValueMM, -adjustValueMM, -adjustValueMM);;

  QColor color(178, 34, 34);
  QPen pen(color, penWidthSelectionMM);

  painter->save();

  painter->setRenderHint(QPainter::Antialiasing, true);
  painter->setPen(pen);
  painter->drawRect(adjustedBoxMM);
  
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
  if (change == QGraphicsItem::ItemSceneHasChanged)
  {
    Scene* myScene = dynamic_cast<Scene*>(this->scene());
    getController()->sceneHasChanged(myScene);
  }

  return QGraphicsItem::itemChange(change, value);
}

void te::layout::AbstractItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
  if (isEditionMode() == false && te::layout::Property::GetValueAs<bool>(this->getProperty("resizable")))
  {
    if (isZoomAdequateForResize())
    {
      checkTouchesCorner(event->pos().x(), event->pos().y());
    }
  }
  checkTouchesWarningAlert(event->pos().x(), event->pos().y());
  QGraphicsItem::hoverMoveEvent(event);
}

void te::layout::AbstractItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
  QGraphicsItem::hoverMoveEvent(event);
  if (isEditionMode() == false)
  {
    this->setCursor(Qt::ArrowCursor);
  }
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

bool te::layout::AbstractItem::isZoomAdequateForResize() const
{
  AbstractScene* myScene = this->getScene();
  if (myScene == 0)
  {
    throw te::common::Exception("The scene is invalid in AbstractItem::isZoomAdequateForResize");
  }

  te::layout::Utils utils = myScene->getUtils();

  //We convert the expeted size in Pixels to MM. We also consider the zoom factor
  QRectF boxMM = this->boundingRect();
  int zoom = myScene->getContext().getZoom();
  double zoomFactor = zoom / 100.;
  double hotPointSizeMM = utils.pixel2mm(m_hotPointSizePixels) / zoomFactor;    
  double minItemSizeMM = hotPointSizeMM * 4;

  //if the rect is too small to fit two hot points in the same axis plus a gap between them, we do not draw the hot points
  if (minItemSizeMM <= boxMM.width() && minItemSizeMM <= boxMM.height())
  {
    return true;
  }

  return false;
}

bool te::layout::AbstractItem::isZoomAdequateForRotation() const
{
  AbstractScene* myScene = this->getScene();
  te::layout::Utils utils = myScene->getUtils();

  //We convert the expeted size in Pixels to MM. We also consider the zoom factor
  QRectF boxMM = this->boundingRect();
  int zoom = myScene->getContext().getZoom();
  double zoomFactor = zoom / 100.;
  double rotationSizeMM = utils.pixel2mm(m_rotationHotPointSizePixels) / zoomFactor;
  double minItemSizeMM = rotationSizeMM * 3;

  //if the rect is too small to fit two hot points in the same axis plus a gap between them, we do not draw the hot points
  if (minItemSizeMM <= boxMM.width() && minItemSizeMM <= boxMM.height())
  {
    return true;
  }

  return false;
}


void te::layout::AbstractItem::checkTouchesWarningAlert(const double& x, const double& y)
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

  std::vector<std::string> warningVect = getController()->getWarningManager()->getWarnings();

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
  AbstractScene* myScene = this->getScene();
  te::layout::Utils utils = myScene->getUtils();

  //we need to initialize the size of the pen. To do this, we convert the expeted size in Pixels to MM. We also consider the zoom factor
  int zoom = myScene->getContext().getZoom();
  double zoomFactor = zoom / 100.;

  double hotPointSizeMM = utils.pixel2mm(m_hotPointSizePixels) / zoomFactor;
  double halfHotPointSizeMM = hotPointSizeMM / 2.;

  QRectF boxMM = boundingRect();
  QPointF checkPoint(x, y);

  //we compute the borders before checking the intersections
  QRectF adjustedHotPointsRectMM = boxMM.adjusted(halfHotPointSizeMM, halfHotPointSizeMM, -halfHotPointSizeMM, -halfHotPointSizeMM);

  QRectF topRect(adjustedHotPointsRectMM.topLeft(), adjustedHotPointsRectMM.topRight());
  QRectF bottomRect(adjustedHotPointsRectMM.bottomLeft(), adjustedHotPointsRectMM.bottomRight());
  QRectF leftRect(adjustedHotPointsRectMM.topLeft(), adjustedHotPointsRectMM.bottomLeft());
  QRectF rightRect(adjustedHotPointsRectMM.topRight(), adjustedHotPointsRectMM.bottomRight());
  
  topRect.adjust(-halfHotPointSizeMM, -halfHotPointSizeMM, halfHotPointSizeMM, halfHotPointSizeMM);
  bottomRect.adjust(-halfHotPointSizeMM, -halfHotPointSizeMM, halfHotPointSizeMM, halfHotPointSizeMM);
  leftRect.adjust(-halfHotPointSizeMM, -halfHotPointSizeMM, halfHotPointSizeMM, halfHotPointSizeMM);
  rightRect.adjust(-halfHotPointSizeMM, -halfHotPointSizeMM, halfHotPointSizeMM, halfHotPointSizeMM);

  //we must consider that QRect is inverted from page orientation. For this reason, we must mirror top with bottom
  bool hitsTop = bottomRect.contains(checkPoint);
  bool hitsBottom = topRect.contains(checkPoint); 
  bool hitsLeft = leftRect.contains(checkPoint);
  bool hitsRight = rightRect.contains(checkPoint);

  m_enumSides = TPNoneSide;
  this->setCursor(Qt::ArrowCursor);

  //we first check if the point is hitting the four corner hot-points
  //then we check if the point is hitting any border
  if (hitsTop && hitsLeft)
  {
    this->setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPTopLeft;
  }
  else if (hitsTop && hitsRight)
  {
    this->setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPTopRight;
  }
  else if (hitsBottom && hitsLeft)
  {
    this->setCursor(Qt::SizeBDiagCursor);
    m_enumSides = TPLowerLeft;
  }
  else if (hitsBottom && hitsRight)
  {
    this->setCursor(Qt::SizeFDiagCursor);
    m_enumSides = TPLowerRight;
  }
  else
  {
    const Property& pKeepAspect = this->getProperty("keep_aspect");
    bool keepAspect = te::layout::Property::GetValueAs<bool>(pKeepAspect);
    if (keepAspect == false)
    {
      if (hitsTop)
      {
        this->setCursor(Qt::SizeVerCursor);
        m_enumSides = TPTop;
      }
      else if (hitsBottom)
      {
        this->setCursor(Qt::SizeVerCursor);
        m_enumSides = TPLower;
      }
      else if (hitsLeft)
      {
        this->setCursor(Qt::SizeHorCursor);
        m_enumSides = TPLeft;
      }
      else if (hitsRight)
      {
        this->setCursor(Qt::SizeHorCursor);
        m_enumSides = TPRight;
      }
    }
  }

  if (m_enumSides == TPNoneSide)
  {
    return false;
  }

  return true;
}


bool te::layout::AbstractItem::checkRotationArea(const double& x, const double& y)
{
  QRectF boxMM = this->boundingRect();

  AbstractScene* myScene = this->getScene();
  Utils utils = this->getScene()->getUtils();

  int zoom = myScene->getContext().getZoom();
  double zoomFactor = zoom / 100.;

  double rotationHotPointSizeMM = utils.pixel2mm(m_rotationHotPointSizePixels) / zoomFactor;
  double halfRotationPointSizeMM = rotationHotPointSizeMM / 2.;

  QPointF rotationSymbolPos(boxMM.width() / 2., boxMM.height() - halfRotationPointSizeMM);
  QPainterPath rotationSymbol = ItemUtils::getRotationSymbol(rotationSymbolPos, rotationHotPointSizeMM);
  QRectF rotationSymbolRect = rotationSymbol.boundingRect();

  QPointF mousePoint(x, y);
  if (rotationSymbolRect.contains(mousePoint))
  {
      return true;
  }

  return false;
}

void te::layout::AbstractItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
  if (isEditionMode() == true)
  {
    QGraphicsItem::mousePressEvent(event);
    return;
  }

  //checks if the item is resizable.
  const Property& property = this->getProperty("resizable");
  if (te::layout::Property::GetValueAs<bool>(property) == true && isZoomAdequateForResize())
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

  if (isZoomAdequateForRotation())
  {
    bool startRotation = checkRotationArea(event->pos().x(), event->pos().y());
    if (startRotation == true)
    {
      m_currentAction = te::layout::ROTATION_ACTION;
    }
  }

  QGraphicsItem::mousePressEvent(event);
}

void te::layout::AbstractItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
  if (isEditionMode() == true)
  {
    QGraphicsItem::mouseMoveEvent(event);
    return;
  }

  if (m_currentAction == te::layout::RESIZE_ACTION)
  {
    this->setOpacity(0.5);
    m_finalCoord = event->pos();
    this->prepareGeometryChange();
    m_rect = getController()->calculateResize(m_enumSides, m_initialCoord, m_finalCoord);

    QGraphicsScene* scene = this->scene();
    QGraphicsView* view = scene->views().first();

    QPointF mousePoint(event->pos().x(), event->pos().y());
    QPointF convPoint = this->mapToScene(mousePoint);

    QString txt = "X: " + QString::number(mousePoint.x());
    txt = txt + ", ";
    txt = txt + "Y: " + QString::number(mousePoint.y());

    QPoint tipPoint = QCursor::pos();
    QToolTip::showText(tipPoint, txt, view);

  }
  else if (m_currentAction == te::layout::ROTATION_ACTION)
  {
    //we must use scene coords to avoid interference of item transformations
    QRectF boxMM = this->boundingRect();

    QPointF centerSceneMM = this->mapToScene(boxMM.center());    
    QPointF mousePointSceneMM = this->mapToScene(event->pos());

    double angle = Utils::calculateAngle(centerSceneMM, mousePointSceneMM);
    angle = angle + 90;

    //here we try to round the degree before setting it into the model
    double roundedAngle = std::round(angle / 90.) * 90.;
    if (fabs(roundedAngle - angle) <= 5)
    {
      angle = roundedAngle;
    }

    this->setItemRotation(angle);
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
  if (isEditionMode() == true)
  {
    QGraphicsItem::mouseReleaseEvent(event);
    return;
  }

  if (m_currentAction == te::layout::RESIZE_ACTION)
  {
    m_currentAction = te::layout::NO_ACTION;
    m_finalCoord = event->pos();
    getController()->resize(m_enumSides, m_initialCoord, m_finalCoord);
    this->setOpacity(1.);
  }
  else if (m_currentAction == te::layout::MOVE_ACTION)
  {
    m_currentAction = te::layout::NO_ACTION;
    this->setOpacity(1.);
    getController()->itemPositionChanged(this->pos().x(), this->pos().y());
  }
  else if (m_currentAction == te::layout::ROTATION_ACTION)
  {
    m_currentAction = te::layout::NO_ACTION;

    double rotation = getItemRotation();
    getController()->rotated(rotation);
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
    
te::layout::AbstractScene* te::layout::AbstractItem::getScene() const
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

void te::layout::AbstractItem::addUndoCommandToStack(QUndoCommand* command)
{
  te::layout::AbstractScene* absScene = getScene();
  if (absScene)
  {
    Scene* scene = dynamic_cast<Scene*>(absScene);
    if (scene)
    {
      scene->addUndoStack(command);
    }
  }
}
