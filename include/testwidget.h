#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = nullptr, int index = 0);
    ~TestWidget();
    int index;

private slots:
    void on_buttonStart_released();
    void on_buttonReturn_released();

signals:
    void returnSignal(int index);

private:
    Ui::TestWidget *ui;
};


#endif // TESTWIDGET_H
