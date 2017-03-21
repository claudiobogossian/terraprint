#include "ItemInputProxy.h"

#include "AbstractScene.h"
#include "ContextObject.h"
#include "Utils.h"

te::layout::ItemInputProxy::ItemInputProxy(AbstractScene* scene)
  : m_scene(scene)
{

}

te::layout::ItemInputProxy::~ItemInputProxy()
{

}

te::layout::AbstractItemView* te::layout::ItemInputProxy::getItem(const std::string& itemName) const
{
  return m_scene->getItem(itemName);
}

te::layout::Utils te::layout::ItemInputProxy::getUtils() const
{
  return m_scene->getUtils();
}

const te::layout::ContextObject& te::layout::ItemInputProxy::getContext() const
{
  return m_scene->getContext();
}
