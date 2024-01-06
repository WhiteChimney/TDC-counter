#include "widget.h"
#include "ui_widget.h"


void Widget::on_buttonQkd_released()
{
//    初始化统计 Widget
    if (qkdWidgetLaunched)
    {
        qkdW->show();
        qkdW->activateWindow();
    }
    else
    {
        qkdW = new HeraldQkdWidget(this);

        connect(qkdW,&HeraldQkdWidget::sendReturnSignal,this,&Widget::dealHeraldQkdReturn);
        connect(qkdW,&HeraldQkdWidget::heraldQkdRequestSync,this,&Widget::dealHeraldQkdRequestSync);
        connect(qkdW,&HeraldQkdWidget::heraldQkdRequestStopSync,this,&Widget::dealHeraldQkdRequestStopSync);

        qkdW->show();
        qkdWidgetLaunched = true;
    }
}

void Widget::dealHeraldQkdReturn()
{
    qkdW->close();
    qkdWidgetLaunched = false;
}

void Widget::dealHeraldQkdRequestSync()
{

}

void Widget::dealHeraldQkdRequestStopSync()
{

}
