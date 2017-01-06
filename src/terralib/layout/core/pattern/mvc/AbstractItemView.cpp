#include "AbstractItemView.h"

#include "AbstractItemController.h"
#include "AbstractItemModel.h"

#include "../../property/Properties.h"
#include "../../property/Property.h"

te::layout::AbstractItemView::AbstractItemView()
  : m_controller(0)
  , m_isEditionMode(false)
  , m_subSelected(false)
  , m_useResizePixmap(true)
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

void te::layout::AbstractItemView::setController(AbstractItemController* controller)
{
  if (m_controller != 0)
  {
    delete m_controller;
    m_controller = 0;
  }
  m_controller = controller;
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

te::layout::AbstractItemController* te::layout::AbstractItemView::createController() const
{
  AbstractItemModel* model = createModel();
  return new AbstractItemController(model, (AbstractItemView*)this);
}
