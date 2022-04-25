#pragma once
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QDockWidget>

#include <QtWidgets/QLabel>
#include <QtWidgets/QMenu>

#include "FuncItem.h"
#include <iostream>
class FuncListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit FuncListWidget(QWidget* parent = Q_NULLPTR);
    ~FuncListWidget();

private:
};

class UsedListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit UsedListWidget(QWidget* parent = Q_NULLPTR);
    ~UsedListWidget();
private:
};
