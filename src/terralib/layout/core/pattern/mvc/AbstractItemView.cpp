#include "AbstractItemView.h"

#include "AbstractItemController.h"
#include "AbstractItemModel.h"

#include "../../property/Properties.h"
#include "../../property/Property.h"

te::layout::AbstractItemView::AbstractItemView(te::layout::ItemInputProxy* itemInputProxy)
  : m_controller(0)
  , m_isEditionMode(false)
  , m_subSelected(false)
  , m_undoEnabled(true)
  , m_itemInputProxy(itemInputProxy)
  , m_useResizePixmap(true)
  , m_isResizeTPTopRight(true)
  , m_isResizeTPTopLeft(true)
  , m_isResizeTPLowerRight(true)
  , m_isResizeTPLowerLeft(true)
  , m_isResizeTPRight(true)
  , m_isResizeTPLeft(true)
  , m_isResizeTPTop(true)
  , m_isResizeTPLower(true)
{

}

te::layout::AbstractItemView::~AbstractItemView()
{
  if(m_controller != 0)
  {
    delete m_controller;
    m_controller = 0;
  }
}

te::layout::AbstractItemController* te::layout::AbstractItemView::getController() const
{
  if (m_controller == nullptr)
  {
    m_controller = createController();
  }

  return m_controller;
}

void te::layout::AbstractItemView::setEditionMode(bool editionMode)
{
  if(m_isEditionMode == editionMode)
  {
    return;
  }

  m_isEditionMode = editionMode;
  if (m_isEditionMode)
  {
    enterEditionMode();
  }
  else
  {
    leaveEditionMode();
  }
}

bool te::layout::AbstractItemView::isEditionMode()
{
  return m_isEditionMode;
}

void te::layout::AbstractItemView::setSubSelection(bool selected)
{
  m_subSelected = selected;
  refresh();
}

bool te::layout::AbstractItemView::isSubSelected() const
{
  return m_subSelected;
}

const te::layout::Properties& te::layout::AbstractItemView::getProperties() const
{
  AbstractItemController* controller = getController();
  return controller->getProperties();
}

void te::layout::AbstractItemView::setProperties(const te::layout::Properties& properties)
{
  AbstractItemController* controller = getController();
  controller->setProperties(properties);
}

const te::layout::Property& te::layout::AbstractItemView::getProperty(const std::string& propertyName) const
{
  AbstractItemController* controller = getController();
  return controller->getProperty(propertyName);
}

void te::layout::AbstractItemView::setProperty(const te::layout::Property& property)
{
  AbstractItemController* controller = getController();
  controller->setProperty(property);
}

void te::layout::AbstractItemView::attach(AbstractItemView* view)
{
  m_controller->attach(view->getController());
}

void te::layout::AbstractItemView::detach(AbstractItemView* view)
{
  m_controller->detach(view->getController());
}

te::layout::AbstractItemController* te::layout::AbstractItemView::createController() const
{
  AbstractItemModel* model = createModel();
  return new AbstractItemController(model, (AbstractItemView*)this);
}

void te::layout::AbstractItemView::setUndoEnabled(bool enabled)
{
  m_undoEnabled = enabled;
}

bool te::layout::AbstractItemView::isUndoEnabled()
{
  return m_undoEnabled;
}

te::layout::ItemInputProxy* te::layout::AbstractItemView::getItemInputProxy() const
{
  return m_itemInputProxy;
}

bool te::layout::AbstractItemView::isUseResizePixmap()
{
  return m_useResizePixmap;
}

bool te::layout::AbstractItemView::isResizeTopRight()
{
  return m_isResizeTPTopRight;
}

bool te::layout::AbstractItemView::isResizeTopLeft()
{
  return m_isResizeTPTopLeft;
}

bool te::layout::AbstractItemView::isResizeLowerRight()
{
  return m_isResizeTPLowerRight;
}

bool te::layout::AbstractItemView::isResizeLowerLeft()
{
  return m_isResizeTPLowerLeft;
}

bool te::layout::AbstractItemView::isResizeRight()
{
  return m_isResizeTPRight;
}

bool te::layout::AbstractItemView::isResizeLeft()
{
  return m_isResizeTPLeft;
}

bool te::layout::AbstractItemView::isResizeTop()
{
  return m_isResizeTPTop;
}

bool te::layout::AbstractItemView::isResizeLower()
{
  return m_isResizeTPLower;
}

bool te::layout::AbstractItemView::canResize(const LayoutAlign& sides)
{
  bool result = true;
  switch (sides)
  {
  case TPTopRight:
    if (!m_isResizeTPTopRight)
    {
      result = false;
    }
    break;

  case TPTopLeft:
    if (!m_isResizeTPTopLeft)
    {
      result = false;
    }
    break;

  case TPLowerRight:
    if (!m_isResizeTPLowerRight)
    {
      result = false;
    }
    break;

  case TPLowerLeft:
    if (!m_isResizeTPLowerLeft)
    {
      result = false;
    }
    break;

  case TPRight:
    if (!m_isResizeTPRight)
    {
      result = false;
    };
    break;

  case TPLeft:
    if (!m_isResizeTPLeft)
    {
      result = false;
    }
    break;

  case TPTop:
    if (!m_isResizeTPTop)
    {
      result = false;
    }
    break;

  case TPLower:
    if (!m_isResizeTPLower)
    {
      result = false;
    }
    break;

  default:
    break;
  }
  return result;
}

