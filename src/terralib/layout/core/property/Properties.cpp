#include "Properties.h"

te::layout::Properties::Properties() 
  : m_objName("unknown")
  , m_typeObj(0)
  , m_hasWindows(false)
  , m_hashcode(0)
{

}

te::layout::Properties::Properties(const std::string& objectName, te::layout::EnumType* type, int hashCode) 
  : m_objName(objectName)
  , m_typeObj(type)
  , m_hasWindows(false)
  , m_hashcode(hashCode)
{

}

te::layout::Properties::~Properties(void)
{

}

bool te::layout::Properties::addProperty(const te::layout::Property& property)
{
  std::size_t total = m_properties.size();
  m_properties.push_back(property);

  if (m_properties.size() > total)
    return true;
  return false;
}

bool te::layout::Properties::addSubProperty(const te::layout::Property& parent, const te::layout::Property& subProperty)
{
  const std::vector<te::layout::Property>& subProperties = parent.getSubProperty();
  std::size_t total = subProperties.size();
  std::size_t totalResult = 0;

  if (std::find(m_properties.begin(), m_properties.end(), parent) != m_properties.end())
  {
    std::vector<Property>::iterator it = std::find(m_properties.begin(), m_properties.end(), parent);
    it->addSubProperty(subProperty);
    totalResult = it->getSubProperty().size();
  }
  else
  {
    for (std::vector<te::layout::Property>::iterator itSub = m_properties.begin(); itSub != m_properties.end(); ++itSub)
    {
      if (itSub->addSubProperty(parent, subProperty))
      {
        totalResult += total;
      }
    }
  }

  if (totalResult > total)
    return true;
  return false;
}

bool te::layout::Properties::removeProperty(const std::string& name)
{
  bool result = false;

  for (std::vector<Property>::iterator it = m_properties.begin(); it != m_properties.end(); ++it)
  {
    if (it->getName().compare(name) == 0)
    {
      m_properties.erase(it);
      result = true;
      break;
    }
    else
    {
      it->removeSubProperty(name);
    }
  }
  return result;
}

bool te::layout::Properties::updateProperty(const te::layout::Property& property)
{
  bool result = false;

  for (std::vector<Property>::iterator it = m_properties.begin(); it != m_properties.end(); ++it)
  {
    if (it->getName().compare(property.getName()) == 0)
    {
      it->setValue(property.getValue()->clone(), property.getType());
      it->setOptionChoice(property.getOptionByCurrentChoice());

      //if this property has subproperties, we must update them too
      const std::vector<Property>& vecSubProperties = property.getSubProperty();
      for (size_t j = 0; j < vecSubProperties.size(); ++j)
      {
        it->updateSubProperty(vecSubProperties[j]);
      }

      result = true;
    }
    else
    {
      it->updateSubProperty(property);
    }
  }
  return result;
}

bool te::layout::Properties::completelyUpdateProperty(const te::layout::Property& property)
{
  bool result = false;

  for (std::vector<Property>::iterator it = m_properties.begin(); it != m_properties.end(); ++it)
  {
    if (it->getName().compare(property.getName()) == 0)
    {
      it->setValue(property.getValue()->clone(), property.getType());
      it->setOptionChoice(property.getOptionByCurrentChoice());
      it->setEditable(property.isEditable());
      it->setLabel(property.getLabel());
      it->setMenu(property.isMenu());
      it->setIcon(property.getIcon());
      it->setVisible(property.isVisible());
      it->setRequired(property.isRequired());
      it->setParentItemHashCode(property.getParentItemHashCode());
      it->setComposeWidget(property.isComposeWidget());
      it->setPublic(property.isPublic());
      it->setSerializable(property.isSerializable());
      result = true;
    }
    else
    {
      it->completelyUpdateSubProperty(property);
    }
  }
  return result;
}

const std::vector<te::layout::Property>& te::layout::Properties::getProperties() const
{
  return m_properties;
}

bool te::layout::Properties::clear()
{
  m_properties.clear();
  return m_properties.empty();
}

const std::string te::layout::Properties::getObjectName() const
{
  return m_objName;
}

void te::layout::Properties::setObjectName(const std::string& name)
{
  m_objName = name;
}

te::layout::EnumType* te::layout::Properties::getTypeObj() const
{
  return m_typeObj;
}

void te::layout::Properties::setTypeObj(te::layout::EnumType* type)
{
  m_typeObj = type;
}

bool te::layout::Properties::contains(const std::string& propertyName) const
{
  if (std::find(m_properties.begin(), m_properties.end(), propertyName) != m_properties.end())
  {
    return true;
  }
  else
  {
    for (std::vector<te::layout::Property>::const_iterator itSub = m_properties.begin(); itSub != m_properties.end(); ++itSub)
    {
      if (itSub->containsSubProperty(propertyName))
      {
        return true;
      }
    }
  }

  return false;
}

const te::layout::Property& te::layout::Properties::getProperty(const std::string& name) const
{
  Property property(0);
  property.setName(name);

  std::vector<Property>::const_iterator it = std::find(m_properties.begin(), m_properties.end(), property);
  if (it != m_properties.end())
  {
    return (*it);
  }
  else
  {
    for (std::vector<te::layout::Property>::const_iterator itSub = m_properties.begin(); itSub != m_properties.end(); ++itSub)
    {
      if (itSub->containsSubProperty(name))
      {
        const Property& prop = itSub->getSubProperty(name);
        return prop;
      }
    }
  }
  throw te::common::Exception("Properties::The given property name was not found in the property list");
}

void te::layout::Properties::setHasWindows(bool windows)
{
  m_hasWindows = windows;
}

bool te::layout::Properties::hasWindows() const
{
  return m_hasWindows;
}

int te::layout::Properties::getHashCode()
{
  return m_hashcode;
}

void te::layout::Properties::setHashCode(int hashCode)
{
  m_hashcode = hashCode;
}

void te::layout::Properties::reparentProperties(const std::string& parentClass)
{
  for (std::vector<Property>::iterator it = m_properties.begin(); it != m_properties.end(); ++it)
  {
    it->setParent(parentClass);
  }
}
