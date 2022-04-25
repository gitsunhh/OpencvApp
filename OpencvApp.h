#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OpencvApp.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QDockWidget>
#include <QLabel>

#include "ListWidget.h"
#include "GraphicsView.h"

namespace Ui {
    class OpencvAppClass;
}
class MyDockWidget : public QDockWidget{
    Q_OBJECT

public:
    explicit MyDockWidget(QString title, QWidget * parent = Q_NULLPTR)
        :QDockWidget(parent)
    {
        QLabel* label = new QLabel(title);
        this->setTitleBarWidget(label);
        this->setFeatures(QDockWidget::DockWidgetFeature::NoDockWidgetFeatures);
    };
    ~MyDockWidget() {};
};

class OpencvApp : public QMainWindow
{
    Q_OBJECT

public:
    OpencvApp(QWidget *parent = Q_NULLPTR);
    ~OpencvApp();
public Q_SLOTS:
    void fileopen();
    void showContextMenu(const QPoint& pos);
    void eraseItem();
    void additem(QListWidgetItem* o);
    void showattr(QListWidgetItem* o);  
    void update_image();
private:
    Ui::OpencvAppClass* ui;
    FuncListWidget* funclist;
    UsedListWidget* usedlist;
    MyDockWidget* dockattr;
    QVector<funcitem*> funcs;
    GraphicsView* graphicsView;
    cv::Mat src_img;
    cv::Mat cur_img;

    //void test();
};
