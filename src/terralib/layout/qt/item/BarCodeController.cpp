#include "BarCodeController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../qt/item/BarCodeItem.h"


te::layout::BarCodeController::BarCodeController(AbstractItemModel* model, AbstractItemView* view)
: TextController(model, view)
{

}
