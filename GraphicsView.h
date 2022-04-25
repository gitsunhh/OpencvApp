#pragma once
#include <QtWidgets/QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
//#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>
#include <QRectF>
#include <QImage>
#include <opencv2/opencv.hpp>
class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphicsView(QWidget* parent = Q_NULLPTR);
    ~GraphicsView();
    void wheelEvent(QWheelEvent* event);
    void fitInView();
    void update(const cv::Mat& src);
private:
    int _zoom;
    bool _empty;
    QGraphicsPixmapItem* _photo;
};
