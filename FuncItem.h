#pragma once
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QHeaderView>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <QObject>

class funcitem :public QObject {
    Q_OBJECT
public:
    funcitem();
    ~funcitem();
    QListWidgetItem* getitem() {
        return item;
    };
    QTableWidget* gettable() {
        return table;
    };
    virtual cv::Mat call(const cv::Mat& src)=0;
signals:
    void valueChangedSignal();
public Q_SLOTS:
    virtual void value_changed() {};
protected:
    QListWidgetItem* item;
    QTableWidget* table;
};
//*********** GrayingItem ***********//
class GrayingItem : public funcitem {
    Q_OBJECT
public:
    GrayingItem();
    ~GrayingItem();
    cv::Mat call(const cv::Mat& src);

private:
};
//*********** filteritem ***********//
class filteritem : public funcitem {
    Q_OBJECT
public:
    filteritem();
    ~filteritem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QComboBox* kind_comBox;
    QSpinBox* ksize_spinBox;
};
//*********** EqualizeItem ***********//
class EqualizeItem : public funcitem {
    Q_OBJECT
public:
    EqualizeItem();
    ~EqualizeItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QCheckBox* red_checkBox;
    QCheckBox* green_checkBox;
    QCheckBox* blue_checkBox; 
};
//*********** MorphItem ***********//
class MorphItem : public funcitem {
    Q_OBJECT
public:
    MorphItem();
    ~MorphItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QComboBox* op_comBox;
    QSpinBox* ksize_spinBox;
    QComboBox* kshape_comBox;
};
//*********** GradItem ***********//
class GradItem : public funcitem {
    Q_OBJECT
public:
    GradItem();
    ~GradItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QComboBox* kind_comBox;
    QSpinBox* ksize_spinBox;
    QSpinBox* dx_spinBox;
    QSpinBox* dy_spinBox;
};
//*********** ThresholdItem ***********//
class ThresholdItem : public funcitem {
    Q_OBJECT
public:
    ThresholdItem();
    ~ThresholdItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QComboBox* method_comBox;
    QSpinBox* thresh_spinBox;
    QSpinBox* maxval_spinBox;
};
//*********** EdgeItem ***********//
class EdgeItem : public funcitem {
    Q_OBJECT
public:
    EdgeItem();
    ~EdgeItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QSpinBox* thresh1_spinBox;
    QSpinBox* thresh2_spinBox;
};
//*********** ContourItem ***********//
class ContourItem : public funcitem {
    Q_OBJECT
public:
    ContourItem();
    ~ContourItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QComboBox* bbox_comBox;
    QComboBox* mode_comBox;
    QComboBox* method_comBox;
};
//*********** HoughLineItem ***********//
class HoughLineItem : public funcitem {
    Q_OBJECT
public:
    HoughLineItem();
    ~HoughLineItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QSpinBox* thresh_spinBox;
    QSpinBox* min_length_spinBox;
    QSpinBox* max_gap_spinbox;
};
//*********** LightItem ***********//
class LightItem : public funcitem {
    Q_OBJECT
public:
    LightItem();
    ~LightItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QDoubleSpinBox* alpha_spinBox;
    QSpinBox* beta_spinbox;
};
//*********** GammaItem ***********//
class GammaItem : public funcitem {
    Q_OBJECT
public:
    GammaItem();
    ~GammaItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
    QDoubleSpinBox* gamma_spinbox;
};
//*********** CustomItem ***********//
class CustomItem : public funcitem {
    Q_OBJECT
public:
    CustomItem();
    ~CustomItem();
    cv::Mat call(const cv::Mat& src);

public Q_SLOTS:
    void value_changed();
private:
};