#include "ColorEditWidget.h"

#include <QApplication>
#include <QColorDialog>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QKeyEvent>
#include <QPainter>
#include <QStyleOption>
#include <QToolButton>

te::layout::ColorEditWidget::ColorEditWidget(QWidget *parent) :
    QWidget(parent),
    m_pixmapLabel(new QLabel),
    m_label(new QLabel),
    m_button(new QToolButton)
{
  QHBoxLayout *lt = new QHBoxLayout(this);
  setupTreeViewEditorMargin(lt);
  lt->setSpacing(0);
  lt->addWidget(m_pixmapLabel);
  lt->addWidget(m_label);
  lt->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Ignored));

  m_button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Ignored);
  m_button->setFixedWidth(20);
  setFocusProxy(m_button);
  setFocusPolicy(m_button->focusPolicy());
  m_button->setText(tr("..."));
  m_button->installEventFilter(this);
  connect(m_button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
  lt->addWidget(m_button);
  m_pixmapLabel->setPixmap(brushValuePixmap(QBrush(m_color)));
  m_label->setText(colorValueText(m_color));
}

void te::layout::ColorEditWidget::setValue(const QColor &c)
{
  if (m_color != c) 
  {
    m_color = c;
    m_pixmapLabel->setPixmap(brushValuePixmap(QBrush(c)));
    m_label->setText(colorValueText(c));
  }
}

void te::layout::ColorEditWidget::buttonClicked()
{
  QColor newColor = QColorDialog::getColor(m_color, this, m_title, QColorDialog::ShowAlphaChannel);
  if (newColor.isValid())
  {
    setValue(newColor);
    emit valueChanged(m_color);
  }
}

void te::layout::ColorEditWidget::setTitle(const QString& title)
{
  m_title = title;
}

bool te::layout::ColorEditWidget::eventFilter(QObject *obj, QEvent *ev)
{
  if (obj == m_button) 
  {
    switch (ev->type()) 
    {
      case QEvent::KeyPress:
      case QEvent::KeyRelease: 
      { // Prevent the QToolButton from handling Enter/Escape meant control the delegate
        switch (static_cast<const QKeyEvent*>(ev)->key())
        {
        case Qt::Key_Escape:
        case Qt::Key_Enter:
        case Qt::Key_Return:
          ev->ignore();
          return true;
        default:
          break;

        }
        break;
      default:
        break;
      }
    }
  }
  return QWidget::eventFilter(obj, ev);
}

void te::layout::ColorEditWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QPixmap te::layout::ColorEditWidget::brushValuePixmap(const QBrush &b)
{
    QImage img(16, 16, QImage::Format_ARGB32_Premultiplied);
    img.fill(0);

    QPainter painter(&img);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(0, 0, img.width(), img.height(), b);
    QColor color = b.color();
    if (color.alpha() != 255) { // indicate alpha by an inset
        QBrush  opaqueBrush = b;
        color.setAlpha(255);
        opaqueBrush.setColor(color);
        painter.fillRect(img.width() / 4, img.height() / 4,
                         img.width() / 2, img.height() / 2, opaqueBrush);
    }
    painter.end();
    return QPixmap::fromImage(img);
}

QString te::layout::ColorEditWidget::colorValueText(const QColor &c)
{
  return QString("[%1, %2, %3] (%4)").arg(c.red()).arg(c.green()).arg(c.blue()).arg(c.alpha());
}

void te::layout::ColorEditWidget::setupTreeViewEditorMargin(QLayout *lt)
{
    enum { DecorationMargin = 4 };
    if (QApplication::layoutDirection() == Qt::LeftToRight)
        lt->setContentsMargins(DecorationMargin, 0, 0, 0);
    else
        lt->setContentsMargins(0, 0, DecorationMargin, 0);
}
