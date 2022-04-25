#include "FuncItem.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

funcitem::funcitem() : QObject(nullptr)
{
    item = new QListWidgetItem(QIcon("icons\\color.png"), "Item");
    item->setSizeHint(QSize(90, 90));

    table = new QTableWidget;
    table->setShowGrid(true);
    table->setAlternatingRowColors(true);
    table->setFocusPolicy(Qt::NoFocus);
    table->horizontalHeader()->setVisible(false);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setStretchLastSection(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}
funcitem::~funcitem() {
    delete item;
    delete table;
}

GrayingItem::GrayingItem()
{
    item->setText("gray");
}
GrayingItem::~GrayingItem() {
}
cv::Mat GrayingItem::call(const cv::Mat& src) {
    cv::Mat dst;
    if (src.channels() == 3)
        cvtColor(src, dst, CV_BGR2GRAY);
    else
        src.copyTo(dst);
    return dst;
}

//*********** filteritem ***********//
filteritem::filteritem()
{
    item->setText("filter");
    kind_comBox = new QComboBox;
    QStringList kinds{ "均值滤波","高斯滤波","中值滤波" };
    kind_comBox->addItems(kinds);
    kind_comBox->setObjectName("kind");

    ksize_spinBox = new QSpinBox;
    ksize_spinBox->setObjectName("ksize");
    ksize_spinBox->setMinimum(1);
    ksize_spinBox->setSingleStep(2);

    table->setColumnCount(2);
    table->setRowCount(2);
    QTableWidgetItem* kinditem = new QTableWidgetItem("类型");
    table->setItem(0, 0, kinditem);
    table->setCellWidget(0, 1, kind_comBox);
    QTableWidgetItem* kernelitem = new QTableWidgetItem("核大小");
    table->setItem(1, 0, kernelitem);
    table->setCellWidget(1, 1, ksize_spinBox);
    connect(kind_comBox, SIGNAL(currentTextChanged(QString)),this, SLOT(value_changed()));
    connect(ksize_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
}
void filteritem::value_changed() {
    QSpinBox* i = table->findChild<QSpinBox*>();
    std::cout << "filteritem param value changed " << i->value() << std::endl;
    emit valueChangedSignal();
};
filteritem::~filteritem() {
}
cv::Mat filteritem::call(const cv::Mat& src) {
    cv::Mat dst;
    int _kind = kind_comBox->currentIndex();
    int _ksize = ksize_spinBox->value();
    int _sigmax = 0;
    cv::Size s(_ksize, _ksize);
    switch (_kind) {
    case 0://MEAN_FILTER
        blur(src, dst, s);
        break;
    case 1://GAUSSIAN_FILTER
        GaussianBlur(src, dst, s, _sigmax);
        break;
    default://MEDIAN_FILTER
        medianBlur(src, dst, _ksize);
        break;
    }
    return dst;
}

//*********** EqualizeItem ***********//
EqualizeItem::EqualizeItem() {
    item->setText("Equalize");
    red_checkBox = new QCheckBox;
    red_checkBox->setObjectName("red");
    red_checkBox->setTristate(false);
    red_checkBox->setChecked(true);
    green_checkBox = new QCheckBox;
    green_checkBox->setObjectName("green");
    green_checkBox->setTristate(false);
    green_checkBox->setChecked(true);
    blue_checkBox = new QCheckBox;
    blue_checkBox->setObjectName("blue");
    blue_checkBox->setTristate(false);
    blue_checkBox->setChecked(true);

    table->setColumnCount(2);
    table->setRowCount(3);
    QTableWidgetItem* reditem = new QTableWidgetItem("R通道");
    table->setItem(0, 0, reditem);
    table->setCellWidget(0, 1, red_checkBox);
    QTableWidgetItem* greenitem = new QTableWidgetItem("G通道");
    table->setItem(1, 0, greenitem);
    table->setCellWidget(1, 1, green_checkBox);
    QTableWidgetItem* blueitem = new QTableWidgetItem("B通道");
    table->setItem(2, 0, blueitem);
    table->setCellWidget(2, 1, blue_checkBox);
    connect(red_checkBox, SIGNAL(stateChanged(int)), this, SLOT(value_changed()));
    connect(green_checkBox, SIGNAL(stateChanged(int)), this, SLOT(value_changed()));
    connect(blue_checkBox, SIGNAL(stateChanged(int)), this, SLOT(value_changed()));
}
void EqualizeItem::value_changed() {
    QCheckBox* i = table->findChild<QCheckBox*>();
    std::cout << "EqualizeItem param value changed " << i->isChecked() << std::endl;
    emit valueChangedSignal();
};
EqualizeItem::~EqualizeItem() {
}
cv::Mat EqualizeItem::call(const cv::Mat& src) {
    cv::Mat dst;
    if (src.channels() == 3) {
        std::vector<cv::Mat> vec_channels;
        split(src, vec_channels);
        if(this->red_checkBox->isChecked())
            equalizeHist(vec_channels[0], vec_channels[0]);//Equalize the histogram of the r channel 
        if(this->green_checkBox->isChecked())
            equalizeHist(vec_channels[1], vec_channels[1]);
        if(this->blue_checkBox->isChecked())
            equalizeHist(vec_channels[2], vec_channels[2]);
        //Merge 3 channels in the std::vector to form the color image in YCrCB color space. 
        merge(vec_channels, dst);
    }else
        equalizeHist(src, dst);
    return dst;
}
//*********** MorphItem ***********//
MorphItem::MorphItem() {
    item->setText("Morph");

    op_comBox = new QComboBox;
    QStringList opkinds{ "腐蚀操作","膨胀操作","开操作","闭操作","梯度操作","顶帽操作","黑帽操作"};
    op_comBox->addItems(opkinds);
    op_comBox->setObjectName("op");

    ksize_spinBox = new QSpinBox;
    ksize_spinBox->setMinimum(1);
    ksize_spinBox->setSingleStep(2);
    ksize_spinBox->setObjectName("ksize");

    kshape_comBox = new QComboBox;
    QStringList shapekinds{ "方形","十字形","椭圆形" };
    kshape_comBox->addItems(shapekinds);
    kshape_comBox->setObjectName("kshape");

    table->setColumnCount(2);
    table->setRowCount(3);
    QTableWidgetItem* opitem = new QTableWidgetItem("类型");
    table->setItem(0, 0, opitem);
    table->setCellWidget(0, 1, op_comBox);
    QTableWidgetItem* sizeitem = new QTableWidgetItem("核大小");
    table->setItem(1, 0, sizeitem);
    table->setCellWidget(1, 1, ksize_spinBox);
    QTableWidgetItem* shapeitem = new QTableWidgetItem("核形状");
    table->setItem(2, 0, shapeitem);
    table->setCellWidget(2, 1, kshape_comBox);
    connect(op_comBox, SIGNAL(currentTextChanged(QString)), this, SLOT(value_changed()));
    connect(ksize_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
    connect(kshape_comBox, SIGNAL(currentTextChanged(QString)), this, SLOT(value_changed()));

}
void MorphItem::value_changed() {
    QSpinBox* i = table->findChild<QSpinBox*>();
    std::cout << "MorphItem param value changed " << i->value() << std::endl;
    emit valueChangedSignal();
};
MorphItem::~MorphItem() {
}
cv::Mat MorphItem::call(const cv::Mat& src) {
    cv::Mat dst;
    int _kind = op_comBox->currentIndex();
    int _ksize = ksize_spinBox->value();
    int _kshape = kshape_comBox->currentIndex();

    // Create structuring element
    cv::Mat element;
    cv::Size s(_ksize, _ksize);
    switch (_kshape) {
    case 0://方形
        element = cv::getStructuringElement(cv::MORPH_RECT, s);
        break;
    case 1://十字形
        element = cv::getStructuringElement(cv::MORPH_CROSS, s);
        break;
    default://椭圆形
        element = cv::getStructuringElement(cv::MORPH_ELLIPSE, s);
        break;
    }
    switch (_kind) {
    case 0://腐蚀操作
        morphologyEx(src, dst, cv::MORPH_ERODE, element);
        break;
    case 1://膨胀操作
        morphologyEx(src, dst, cv::MORPH_DILATE, element);
        break;
    case 2://开操作
        morphologyEx(src, dst, cv::MORPH_OPEN, element);
        break;
    case 3://闭操作
        morphologyEx(src, dst, cv::MORPH_CLOSE, element);
        break;
    case 4://梯度操作
        morphologyEx(src, dst, cv::MORPH_GRADIENT, element);
        break;
    case 5://顶帽操作
        morphologyEx(src, dst, cv::MORPH_TOPHAT, element);
        break;
    default://黑帽操作
        morphologyEx(src, dst, cv::MORPH_BLACKHAT, element);
        break;
    }
    return dst;
}
//*********** GradItem ***********//
GradItem::GradItem() {
    item->setText("Grad");

    kind_comBox = new QComboBox;
    QStringList kinds{ "Sobel算子","Scharr算子","Laplacian算子" };
    kind_comBox->addItems(kinds);
    kind_comBox->setObjectName("kind");

    ksize_spinBox = new QSpinBox;
    ksize_spinBox->setMinimum(1);
    ksize_spinBox->setSingleStep(2);
    ksize_spinBox->setObjectName("ksize");

    dx_spinBox = new QSpinBox;
    dx_spinBox->setMaximum(1);
    dx_spinBox->setMinimum(0);
    dx_spinBox->setSingleStep(1);
    dx_spinBox->setObjectName("dx");

    dy_spinBox = new QSpinBox;
    dy_spinBox->setMaximum(1);
    dy_spinBox->setMinimum(0);
    dy_spinBox->setSingleStep(1);
    dy_spinBox->setObjectName("dy");

    table->setColumnCount(2);
    table->setRowCount(4);
    QTableWidgetItem* kinditem = new QTableWidgetItem("类型");
    table->setItem(0, 0, kinditem);
    table->setCellWidget(0, 1, kind_comBox);
    QTableWidgetItem* sizeitem = new QTableWidgetItem("核大小");
    table->setItem(1, 0, sizeitem);
    table->setCellWidget(1, 1, ksize_spinBox);
    QTableWidgetItem* xitem = new QTableWidgetItem("x方向");
    table->setItem(2, 0, xitem);
    table->setCellWidget(2, 1, dx_spinBox);
    QTableWidgetItem* yitem = new QTableWidgetItem("y方向");
    table->setItem(3, 0, yitem);
    table->setCellWidget(3, 1, dy_spinBox);

    connect(kind_comBox, SIGNAL(currentTextChanged(QString)), this, SLOT(value_changed()));
    connect(ksize_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
    connect(dx_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
    connect(dy_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
}
void GradItem::value_changed() {
    QSpinBox* i = table->findChild<QSpinBox*>();
    std::cout << "GradItem param value changed " << i->value() << std::endl;
    emit valueChangedSignal();
};
GradItem::~GradItem() {
}
cv::Mat GradItem::call(const cv::Mat& src) {
    cv::Mat dst;
    int _kind = kind_comBox->currentIndex();
    if (this->dx_spinBox->value() == 0 && this->dy_spinBox->value() == 0 && _kind != 2) {
        this->item->setBackground(QColor(255, 0, 0));
        this->item->setText("图像梯度 （无效: dx与dy不同时为0）");
        src.copyTo(dst);
    }
    else {
        this->item->setBackground(QColor(200, 200, 200));
        this->item->setText("图像梯度");
        switch (_kind)
        {
        case 0:
            cv::Sobel(src, dst, -1, this->dx_spinBox->value(), this->dy_spinBox->value(), ksize_spinBox->value());//cv::CV_16S
            break;
        case 1:
            if (this->dx_spinBox->value() == 1 && this->dy_spinBox->value() == 1) {
                this->item->setBackground(QColor(255, 0, 0));
                this->item->setText("Scharr （无效: dx与dy不同时为1）");
                src.copyTo(dst);
                break;
            }
            cv::Scharr(src, dst, -1, this->dx_spinBox->value(), this->dy_spinBox->value());
            break;
        default:
            cv::Laplacian(src, dst, -1);
            break;
        }
    }
    return dst;
}
//*********** ThresholdItem ***********//
ThresholdItem::ThresholdItem() {
    item->setText("Threshold");
    thresh_spinBox = new QSpinBox;
    thresh_spinBox->setMaximum(255);
    thresh_spinBox->setMinimum(0);
    thresh_spinBox->setSingleStep(1);
    thresh_spinBox->setObjectName("thresh");
    thresh_spinBox->setValue(127);

    maxval_spinBox = new QSpinBox;
    maxval_spinBox->setMaximum(255);
    maxval_spinBox->setMinimum(0);
    maxval_spinBox->setSingleStep(1);
    maxval_spinBox->setObjectName("maxval");
    maxval_spinBox->setValue(255);

    method_comBox = new QComboBox;
    QStringList methods{ "二进制阈值化","反二进制阈值化","截断阈值化","阈值化为0","反阈值化为0","大津算法"};
    method_comBox->addItems(methods);
    method_comBox->setObjectName("method");

    table->setColumnCount(2);
    table->setRowCount(3);
    QTableWidgetItem* kinditem = new QTableWidgetItem("类型");
    table->setItem(0, 0, kinditem);
    table->setCellWidget(0, 1, method_comBox);
    QTableWidgetItem* threshitem = new QTableWidgetItem("阈值");
    table->setItem(1, 0, threshitem);
    table->setCellWidget(1, 1, thresh_spinBox);
    QTableWidgetItem* maxvalitem = new QTableWidgetItem("最大值");
    table->setItem(2, 0, maxvalitem);
    table->setCellWidget(2, 1, maxval_spinBox);

    connect(method_comBox, SIGNAL(currentTextChanged(QString)), this, SLOT(value_changed()));
    connect(thresh_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
    connect(maxval_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
}
void ThresholdItem::value_changed() {
    QSpinBox* i = table->findChild<QSpinBox*>();
    std::cout << "ThresholdItem param value changed " << i->value() << std::endl;
    emit valueChangedSignal();
};
ThresholdItem::~ThresholdItem() {
}
cv::Mat ThresholdItem::call(const cv::Mat& src) {
    cv::Mat dst;
    if (src.channels() == 3)
        cvtColor(src, dst, CV_BGR2GRAY);
    else
        src.copyTo(dst);
    switch (method_comBox->currentIndex())
    {
    case 0:
        cv::threshold(dst, dst, thresh_spinBox->value(), maxval_spinBox->value(), cv::THRESH_BINARY);
        break;
    case 1:
        cv::threshold(dst, dst, thresh_spinBox->value(), maxval_spinBox->value(), cv::THRESH_BINARY_INV);
        break;
    case 2:
        cv::threshold(dst, dst, thresh_spinBox->value(), maxval_spinBox->value(), cv::THRESH_TRUNC);
        break;
    case 3:
        cv::threshold(dst, dst, thresh_spinBox->value(), maxval_spinBox->value(), cv::THRESH_TOZERO);
        break;
    case 4:
        cv::threshold(dst, dst, thresh_spinBox->value(), maxval_spinBox->value(), cv::THRESH_TOZERO_INV);
        break;
    case 5:
        cv::threshold(dst, dst, thresh_spinBox->value(), maxval_spinBox->value(), cv::THRESH_OTSU);
        break;
    default:
        break;
    }
    return dst;
}
//*********** EdgeItem ***********//
EdgeItem::EdgeItem() {
    item->setText("Edge");
    thresh1_spinBox = new QSpinBox;
    thresh1_spinBox->setMaximum(255);
    thresh1_spinBox->setMinimum(0);
    thresh1_spinBox->setSingleStep(1);
    thresh1_spinBox->setObjectName("thresh1");
    thresh1_spinBox->setValue(20);

    thresh2_spinBox = new QSpinBox;
    thresh2_spinBox->setMaximum(255);
    thresh2_spinBox->setMinimum(0);
    thresh2_spinBox->setSingleStep(1);
    thresh2_spinBox->setObjectName("thresh2");
    thresh2_spinBox->setValue(100);

    table->setColumnCount(2);
    table->setRowCount(2);
    QTableWidgetItem* thresh1item = new QTableWidgetItem("阈值1");
    table->setItem(0, 0, thresh1item);
    table->setCellWidget(0, 1, thresh1_spinBox);
    QTableWidgetItem* thresh2item = new QTableWidgetItem("阈值2");
    table->setItem(1, 0, thresh2item);
    table->setCellWidget(1, 1, thresh2_spinBox);

    connect(thresh1_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
    connect(thresh2_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
}
void EdgeItem::value_changed() {
    QSpinBox* i = table->findChild<QSpinBox*>();
    std::cout << "EdgeItem param value changed " << i->value() << std::endl;
    emit valueChangedSignal();
};
EdgeItem::~EdgeItem() {
}
cv::Mat EdgeItem::call(const cv::Mat& src) {
    cv::Mat dst;
    cv::Canny(src, dst, thresh1_spinBox->value(), thresh2_spinBox->value());
    return dst;
}
//*********** ContourItem ***********//
ContourItem::ContourItem() {
    item->setText("Contour");

    bbox_comBox = new QComboBox;
    QStringList bboxs{ "正常轮廓","外接矩形","最小外接矩形","最小外接圆"};
    bbox_comBox->addItems(bboxs);
    bbox_comBox->setObjectName("bbox");

    mode_comBox = new QComboBox;
    QStringList modes{ "外轮廓","轮廓列表","外轮廓与内孔","轮廓等级树"};
    mode_comBox->addItems(modes);
    mode_comBox->setObjectName("mode");

    method_comBox = new QComboBox;
    QStringList methods{ "无近似","简易近似" };
    method_comBox->addItems(methods);
    method_comBox->setObjectName("method");

    table->setColumnCount(2);
    table->setRowCount(3);
    QTableWidgetItem* bboxitem = new QTableWidgetItem("轮廓模式");
    table->setItem(0, 0, bboxitem);
    table->setCellWidget(0, 1, bbox_comBox);
    QTableWidgetItem* modeitem = new QTableWidgetItem("轮廓近似");
    table->setItem(1, 0, modeitem);
    table->setCellWidget(1, 1, mode_comBox);
    QTableWidgetItem* methoditem = new QTableWidgetItem("边界模式");
    table->setItem(2, 0, methoditem);
    table->setCellWidget(2, 1, method_comBox);

    connect(bbox_comBox, SIGNAL(currentTextChanged(QString)), this, SLOT(value_changed()));
    connect(mode_comBox, SIGNAL(currentTextChanged(QString)), this, SLOT(value_changed()));
    connect(method_comBox, SIGNAL(currentTextChanged(QString)), this, SLOT(value_changed()));
}
void ContourItem::value_changed() {
    QComboBox* i = table->findChild<QComboBox*>();
    std::cout << "ContourItem param value changed " << i->currentIndex() << std::endl;
    emit valueChangedSignal();
};
ContourItem::~ContourItem() {
}
cv::Mat ContourItem::call(const cv::Mat& src) {

    int mode = 0, method = 0;
    switch (mode_comBox->currentIndex())
    { 
    case 0:
        mode = cv::RETR_EXTERNAL;
        break;
    case 1:
        mode = cv::RETR_LIST;
        break;
    case 2:
        mode = cv::RETR_CCOMP;
        break;
    default:
        mode = cv::RETR_TREE;
        break;
    }
    switch (method_comBox->currentIndex()) {
    case 0:
        method = cv::CHAIN_APPROX_NONE;
        break;
    default:
        method = cv::CHAIN_APPROX_SIMPLE;
        break;
    }
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy; 
    cv::Mat dst,tmp;
    if (src.channels() == 1) {
        cvtColor(src, dst, CV_GRAY2BGR);
        src.copyTo(tmp);
    }
    if (src.channels() == 3) {
        cvtColor(src, tmp, CV_BGR2GRAY);
        src.copyTo(dst);
    }
    findContours(tmp, contours, hierarchy, mode, method);
    std::cout << "Contours count: " << contours.size() << std::endl;

    cv::Scalar color{ 0,255,0 };
    switch (bbox_comBox->currentIndex())
    {
    case 0://正常轮廓
        cv::drawContours(dst, contours, -1, color, 2);
        break;
    case 1://外接矩形
        for (auto cnt : contours) {
            cv::Rect rect = cv::boundingRect(cnt);
            cv::rectangle(dst, rect, color, 2);
        }
        break;
    case 2://最小外接矩形
        std::cout << "ok" << std::endl;
        for (int i = 0; i < contours.size(); ++i) {
            cv::RotatedRect bbox = cv::minAreaRect(contours[i]);
            cv::Point2f rect_points[4];
            bbox.points(rect_points);
            for (int j = 0; j < 4; j++) {
                cv::line(dst, rect_points[j], rect_points[(j + 1) % 4], color);
            }
        }
        break;
    default://最小外接圆
        cv::Point2f c;
        float r;
        for (auto cnt : contours) {
            cv::minEnclosingCircle(cnt,c,r);
            cv::circle(dst, c, r, color, 2);
        }
        break;
    }
    return dst;
}
//*********** HoughLineItem ***********//
HoughLineItem::HoughLineItem() {
    item->setText("HoughLine");

    thresh_spinBox = new QSpinBox;
    thresh_spinBox->setMinimum(0);
    thresh_spinBox->setSingleStep(1);
    thresh_spinBox->setObjectName("thresh");
    thresh_spinBox->setValue(10);

    min_length_spinBox = new QSpinBox;
    min_length_spinBox->setMinimum(0);
    min_length_spinBox->setSingleStep(1);
    min_length_spinBox->setObjectName("min_length");
    min_length_spinBox->setValue(20);

    max_gap_spinbox = new QSpinBox;
    max_gap_spinbox->setMinimum(0);
    max_gap_spinbox->setSingleStep(1);
    max_gap_spinbox->setObjectName("max_gap");
    max_gap_spinbox->setValue(5);

    table->setColumnCount(2);
    table->setRowCount(3);
    QTableWidgetItem* thitem = new QTableWidgetItem("交点阈值");
    table->setItem(0, 0, thitem);
    table->setCellWidget(0, 1, thresh_spinBox);
    QTableWidgetItem* minitem = new QTableWidgetItem("最小长度");
    table->setItem(1, 0, minitem);
    table->setCellWidget(1, 1, min_length_spinBox);
    QTableWidgetItem* maxitem = new QTableWidgetItem("最大间距");
    table->setItem(2, 0, maxitem);
    table->setCellWidget(2, 1, max_gap_spinbox);

    connect(thresh_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
    connect(min_length_spinBox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
    connect(max_gap_spinbox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
}
void HoughLineItem::value_changed() {
    QSpinBox* i = table->findChild<QSpinBox*>();
    std::cout << "HoughLineItem param value changed " << i->value() << std::endl;
    emit valueChangedSignal();
};
HoughLineItem::~HoughLineItem() {
}
cv::Mat HoughLineItem::call(const cv::Mat& src) {
    cv::Mat dst, tmp;
    if (src.channels() == 1) {
        cvtColor(src, dst, CV_GRAY2BGR);
        src.copyTo(tmp);
    }
    if (src.channels() == 3) {
        cvtColor(src, tmp, CV_BGR2GRAY);
        src.copyTo(dst);
    }
    // Probabilistic Line Transform
    std::vector<cv::Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(tmp, linesP, 1, CV_PI / 180, thresh_spinBox->value(), min_length_spinBox->value(), max_gap_spinbox->value()); // runs the actual detection
    // Draw the lines
    cv::Scalar color{ 0,255,0 };
    for (size_t i = 0; i < linesP.size(); i++)
    {
        cv::Vec4i l = linesP[i];
        line(dst, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), color, 2);
    }
    return dst;
}
//*********** LightItem ***********//
LightItem::LightItem() {
    item->setText("Light");

    alpha_spinBox = new QDoubleSpinBox;
    alpha_spinBox->setMinimum(0);
    alpha_spinBox->setMaximum(3);
    alpha_spinBox->setSingleStep(0.1);
    alpha_spinBox->setObjectName("alpha");
    alpha_spinBox->setValue(1);

    beta_spinbox = new QSpinBox;
    beta_spinbox->setMinimum(0);
    beta_spinbox->setSingleStep(1);
    beta_spinbox->setObjectName("beta");
    beta_spinbox->setValue(0);

    table->setColumnCount(2);
    table->setRowCount(2);
    QTableWidgetItem* alphapitem = new QTableWidgetItem("alpha");
    table->setItem(0, 0, alphapitem);
    table->setCellWidget(0, 1, alpha_spinBox);
    QTableWidgetItem* sizeitem = new QTableWidgetItem("beta");
    table->setItem(1, 0, sizeitem);
    table->setCellWidget(1, 1, beta_spinbox);
    connect(alpha_spinBox, SIGNAL(valueChanged(double)), this, SLOT(value_changed()));
    connect(beta_spinbox, SIGNAL(valueChanged(int)), this, SLOT(value_changed()));
}
void LightItem::value_changed() {
    QSpinBox* i = table->findChild<QSpinBox*>();
    std::cout << "LightItem param value changed " << i->value() << std::endl;
    emit valueChangedSignal();
};
LightItem::~LightItem() {
}
cv::Mat LightItem::call(const cv::Mat& src) {
    cv::Mat dst;
    cv::Mat blank = cv::Mat::zeros(src.size(),src.type());
    cv::addWeighted(src, alpha_spinBox->value(), blank, 1 - alpha_spinBox->value(), beta_spinbox->value(),dst);
    return dst;
}

//*********** GammaItem ***********//
GammaItem::GammaItem() {
    item->setText("Gamma");

    gamma_spinbox = new QDoubleSpinBox;
    gamma_spinbox->setMinimum(0);
    gamma_spinbox->setSingleStep(0.1);
    gamma_spinbox->setObjectName("gamma");
    gamma_spinbox->setValue(1);

    table->setColumnCount(2);
    table->setRowCount(1);
    QTableWidgetItem* gammaitem = new QTableWidgetItem("gamma");
    table->setItem(0, 0, gammaitem);
    table->setCellWidget(0, 1, gamma_spinbox);

    connect(gamma_spinbox, SIGNAL(valueChanged(double)), this, SLOT(value_changed()));
}
void GammaItem::value_changed() {
    QDoubleSpinBox* i = table->findChild<QDoubleSpinBox*>();
    std::cout << "GammaItem param value changed " << i->value() << std::endl;
    emit valueChangedSignal();
};
GammaItem::~GammaItem() {
}
cv::Mat GammaItem::call(const cv::Mat& src) {
    cv::Mat dst;
    cv::Mat lookupTable(1, 256, CV_8U);
    uchar* lut = lookupTable.ptr();
    for (int i = 0; i < 256; i++) {
        lut[i] = int(255 * (pow((double)i / 255.0, gamma_spinbox->value())));
    }
    //std::cout << "LUT: " << lookupTable << std::endl;

    //gamma adjustment. convert image using LUT table. It maps the pixel intensities in the input to the output using values from lut
    LUT(src, lookupTable, dst);

    return dst;
}
//*********** CustomItem ***********//
CustomItem::CustomItem() {
    item->setText("filter");
}
void CustomItem::value_changed() {
    QSpinBox* i = table->findChild<QSpinBox*>();
    std::cout << "CustomItem param value changed" << i->value() << std::endl;
    emit valueChangedSignal();
};
CustomItem::~CustomItem() {
}
cv::Mat CustomItem::call(const cv::Mat& src) {
    cv::Mat dst;
    return dst;
}