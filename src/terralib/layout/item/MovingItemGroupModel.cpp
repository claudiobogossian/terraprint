#include "MovingItemGroupModel.h"

#include "../core/enum/Enums.h"

te::layout::MovingItemGroupModel::MovingItemGroupModel()
{
  m_properties.setTypeObj(Enums::getInstance().getEnumObjectType()->getMovingItemGroup());
}

te::layout::MovingItemGroupModel::~MovingItemGroupModel()
{

}

