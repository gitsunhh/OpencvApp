#include "OpencvApp.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

OpencvApp::OpencvApp(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::OpencvAppClass)
{
    ui->setupUi(this);
    
    MyDockWidget* dockfunc = new MyDockWidget("cv fnucs");
    funclist = new FuncListWidget;
    dockfunc->setWidget(funclist);
    MyDockWidget* dockused = new MyDockWidget("chosen funcs");
    usedlist = new UsedListWidget;
    dockused->setWidget(usedlist);
    dockused->setMinimumWidth(350);
    dockused->setMinimumHeight(400);
    dockattr = new MyDockWidget("attribute");
    dockattr->setMinimumHeight(300);


    this->addDockWidget(Qt::TopDockWidgetArea, dockfunc);
    this->addDockWidget(Qt::RightDockWidgetArea, dockused);
    this->addDockWidget(Qt::RightDockWidgetArea, dockattr);
    
    dockattr->close();   

    graphicsView = new GraphicsView;
    this->setCentralWidget(graphicsView);

    //slot
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(fileopen()));
    connect(funclist, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(additem(QListWidgetItem*)));
    connect(usedlist, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
    connect(usedlist, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(showattr(QListWidgetItem*)));
    //connect(usedlist, SIGNAL(valueChangedSignal()),this, SLOT(fileopen()));

    src_img = cv::Mat::zeros(5, 5, CV_8UC3);
    //src_img = cv::imread("C:\\Users\\SUNHH\\Desktop\\naruto.jpeg");
    //src_img = cv::imread(u8"C:\\Users\\SUNHH\\Desktop\\1.png");
    std::cout << src_img.size() << std::endl;
    //update_image();
}

void OpencvApp::fileopen() {
    //调用打开文件对话框打开一个文件
    QString curPath = QCoreApplication::applicationDirPath(); //获取应用程序的路径
    QString filename = QFileDialog::getOpenFileName(this, tr("Open image"), curPath, tr("Image file (*.jpg *.jpeg *.png)"));

    QString msg = "File chosen: ";
    msg += filename.toStdString().c_str();
    ui->statusBar->showMessage(msg);
    if (filename.isEmpty())
        return;
    src_img = cv::imread(filename.toStdString());
    update_image();
}
void OpencvApp::showContextMenu(const QPoint& pos) {
    QPoint globalPos = usedlist->mapToGlobal(pos);// Handle global position
    QMenu myMenu;// Create menu and insert some actions
    myMenu.addAction("Erase", this, SLOT(eraseItem()));
    myMenu.exec(globalPos);// Show context menu at handling position
}

void OpencvApp::eraseItem() {
    // If multiple selection is on, we need to erase all selected items
    std::cout << "selectedItems().size():" << usedlist->selectedItems().size() << std::endl;
    
    int currentrow = usedlist->currentRow();
    std::cout << "erase item in usedlist: " << currentrow << std::endl;
    
    QListWidgetItem* item = usedlist->takeItem(currentrow);// Get curent item on selected row
    //delete item;  // And remove it  

    std::cout << "used:" << usedlist->count() << " funcs:" << funcs.count() << std::endl;

    funcitem* i = funcs.takeAt(currentrow); 
    disconnect(i, SIGNAL(valueChangedSignal()), this, SLOT(update_image()));
    delete i;
 
    std::cout << "used:" << usedlist->count() << " funcs:" << funcs.count() << std::endl;
    update_image();
    dockattr->close();
}

void OpencvApp::additem(QListWidgetItem* o) {
    std::cout << "func list add item: " << o->text().toStdString() << std::endl;
    int id = funclist->currentRow();
    std::cout << "item in func list: " << id << std::endl;
    funcitem* n;
    switch (id)
    {
    case 0:
        n = new GrayingItem;
        break;
    case 1:
        n = new filteritem;
        break;
    case 2:
        n = new EqualizeItem;
        break;
    case 3:
        n = new MorphItem;
        break;
    case 4:
        n = new GradItem;
        break;
    case 5:
        n = new ThresholdItem;
        break;
    case 6:
        n = new EdgeItem;
        break;
    case 7:
        n = new ContourItem;
        break;
    case 8:
        n = new HoughLineItem;
        break;
    case 9:
        n = new LightItem;
        break;
    case 10:
        n = new GammaItem;
    default:
        break;
    }
    funcs.append(n);
    usedlist->addItem(n->getitem());
    dockattr->setWidget(n->gettable());
    usedlist->setCurrentItem(n->getitem());
    connect(n, SIGNAL(valueChangedSignal()), this, SLOT(update_image()));
    dockattr->show();
    update_image();
}

void OpencvApp::showattr(QListWidgetItem* o) {
    std::cout << "item in used list clicked: " << o->text().toStdString() << std::endl;
    int id = usedlist->currentRow();
    std::cout << "item in used list: " << id << std::endl;
    dockattr->setWidget(funcs.at(id)->gettable());
    dockattr->show();
}
void OpencvApp::update_image() {
    //if (funcs.size() == 0) return;
    cv::Mat tmp;
    src_img.copyTo(tmp);
    for (int i = 0; i < funcs.size(); ++i) {
        funcitem* n = funcs.at(i);
        tmp = n->call(tmp);
    }
    cur_img = tmp;
    //cv::imshow("original picture", cur_img);
    //cv::waitKey(0);
    this->graphicsView->update(cur_img);
}
OpencvApp::~OpencvApp() {
    for (int i = 0; i < funcs.size(); i++)
    {
        delete funcs.takeAt(i);
    }
}
