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

private slots:
    void on_buttonStart_released();
    void on_buttonReturn_released();

signals:
     /* 信号必须有signals关键字来声明
      * 信号没有返回值，但可以有参数
      * 信号就是函数的声明，只需声明，无需定义
      * 使用：emit mySignal();
      * 信号可以重载
     */
    void returnSignal(int index);

private:
    Ui::TestWidget *ui;
    int index;
};


#endif // TESTWIDGET_H
