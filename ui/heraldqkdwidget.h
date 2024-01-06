#ifndef HERALDQKDWIDGET_H
#define HERALDQKDWIDGET_H

#include <QWidget>

namespace Ui {
class HeraldQkdWidget;
}

class HeraldQkdWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeraldQkdWidget(QWidget *parent = nullptr);
    ~HeraldQkdWidget();

private:
    Ui::HeraldQkdWidget *ui;
};

#endif // HERALDQKDWIDGET_H
