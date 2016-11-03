#include "TypeManager.h"

#include "LayoutData.h"
#include "DataTypes.h"

//terralib
#include <terralib/common/Exception.h>
#include <terralib/datatype/DataConverterManager.h>

void te::layout::TypeManager::registerType(const std::string& typeName, int typeCode, const te::dt::DataTypeConverter& converterFromString, const te::dt::DataTypeConverter& converterToString)
{
  if (m_mapNames.find(typeName) != m_mapNames.end())
  {
    throw te::common::Exception("TypeManager::Type code already registered");
  }

  if (m_mapTypeCodes.find(typeCode) != m_mapTypeCodes.end())
  {
    throw te::common::Exception("TypeManager::Type name already registered");
  }

  m_mapNames[typeName] = typeCode;
  m_mapTypeCodes[typeCode] = typeName;

  //then we register the converters in DataConverterManager
  //as there is not way to check if the converter is already added, we handle the exception

  try
  {
    te::dt::DataConverterManager::getInstance().add((int)te::layout::LayoutTypes::STRING_TYPE, typeCode, converterFromString);
  }
  catch (const te::common::Exception&)
  {
  }

  try
  {
    te::dt::DataConverterManager::getInstance().add(typeCode, (int)te::layout::LayoutTypes::STRING_TYPE, converterToString);
  }
  catch (const te::common::Exception&)
  {
  }
}

const te::dt::DataTypeConverter& te::layout::TypeManager::getConverter(int fromTypeCode, int toTypeCode) const
{
  const te::dt::DataTypeConverter& converter = te::dt::DataConverterManager::getInstance().get(fromTypeCode, toTypeCode);
  return converter;
}

te::dt::AbstractData* te::layout::TypeManager::convertTo(te::dt::AbstractData* data, int typeCode) const
{
  if (data->getTypeCode() == typeCode)
  {
    return data->clone();
  }

  const te::dt::DataTypeConverter& converter = getConverter(data->getTypeCode(), typeCode);
  te::dt::AbstractData* convertedData = converter(data);
  return convertedData;
}

int te::layout::TypeManager::getTypeCode(const std::string& typeName) const
{
  std::map<std::string, int>::const_iterator it = m_mapNames.find(typeName);
  if (it == m_mapNames.end())
  {
    throw te::common::Exception("TypeManager::Invalid type code");
  }

  return it->second;
}

std::string te::layout::TypeManager::getName(int typeCode) const
{
  std::map<int, std::string>::const_iterator it = m_mapTypeCodes.find(typeCode);
  if (it == m_mapTypeCodes.end())
  {
    throw te::common::Exception("TypeManager::Invalid type name");
  }

  return it->second;
}

//protected constructor
te::layout::TypeManager::TypeManager()
{

}

//protected destructor
te::layout::TypeManager::~TypeManager()
{

}
