
#ifndef __TERRALIB_LAYOUT_INTERNAL_BARCODE_CONTROLLER_H
#define __TERRALIB_LAYOUT_INTERNAL_BARCODE_CONTROLLER_H


#include "../../core/Config.h"
#include "../../core/pattern/mvc/AbstractItemController.h"
#include "TextController.h"


class QSizeF;


namespace te
{
  namespace layout
  {

class TELAYOUTEXPORT BarCodeController : public TextController
{
public:

      BarCodeController(AbstractItemModel* model);
      virtual QSizeF updateView();

};
  }
}

#endif