#include "BarCodeController.h"
#include "../../core/pattern/mvc/AbstractItemModel.h"
#include "../../qt/item/BarCodeItem.h"


te::layout::BarCodeController::BarCodeController(AbstractItemModel* model)
: TextController(model)
{

}

QSizeF te::layout::BarCodeController::updateView(){
  BarCodeItem* view = dynamic_cast<BarCodeItem*>(m_view);

  if (view != 0)
  {
    EnumDataType* dataType = Enums::getInstance().getEnumDataType();
    Property pSize = getProperty("barcodesize");

    int size = 0;
    size = pSize.getValue().toInt();

    Property pFont = getProperty("font");
    Font font = pFont.getValue().toFont();
    if (size != font.getPointSize()){

      font.setPointSize(size);
      pFont.setValue(font, dataType->getDataTypeFont());

      m_model->setProperty(pFont);

    }
    

  }


  return te::layout::TextController::updateView();

}