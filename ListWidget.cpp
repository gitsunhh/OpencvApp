#include "ListWidget.h"

FuncListWidget::FuncListWidget(QWidget* parent)
	:QListWidget(parent)
{
	this->setDragEnabled(true);
	this->setFocusPolicy(Qt::NoFocus);
	this->setMinimumWidth(1000);
	this->setFixedHeight(100);
	this->setFlow(QListView::LeftToRight);
	this->setAcceptDrops(false);
	this->setViewMode(QListView::IconMode);
	//this->setWrapping(false);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	funcitem* item0 = new GrayingItem;
	funcitem* item1 = new filteritem;
	funcitem* item2 = new EqualizeItem;
	funcitem* item3 = new MorphItem;
	funcitem* item4 = new GradItem;
	funcitem* item5 = new ThresholdItem;
	funcitem* item6 = new EdgeItem;
	funcitem* item7 = new ContourItem;
	funcitem* item8 = new HoughLineItem;
	funcitem* item9 = new LightItem;
	funcitem* item10 = new GammaItem;
	this->addItem(item0->getitem());
	this->addItem(item1->getitem());
	this->addItem(item2->getitem());
	this->addItem(item3->getitem());
	this->addItem(item4->getitem());
	this->addItem(item5->getitem());
	this->addItem(item6->getitem());
	this->addItem(item7->getitem());
	this->addItem(item8->getitem());
	this->addItem(item9->getitem());
	this->addItem(item10->getitem());
}
FuncListWidget::~FuncListWidget(){

}

UsedListWidget::UsedListWidget(QWidget* parent)
	:QListWidget(parent)
{
	this->setDragEnabled(true);
	this->setFocusPolicy(Qt::NoFocus);
	this->setMinimumWidth(250);
	this->setFlow(QListView::TopToBottom);
	this->setAcceptDrops(true);
	this->setDefaultDropAction(Qt::MoveAction);
	this->setDragDropMode(QAbstractItemView::InternalMove);

	this->setContextMenuPolicy(Qt::CustomContextMenu);
}

UsedListWidget::~UsedListWidget() {

}