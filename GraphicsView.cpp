#include "GraphicsView.h"

GraphicsView::GraphicsView(QWidget* parent)
    :QGraphicsView(parent)
{
    _zoom = 0;
    _empty = true;
    _photo = new QGraphicsPixmapItem;
    QGraphicsScene* _scene = new QGraphicsScene;
    _scene->addItem(_photo);
    this->setScene(_scene);
    //this->setAlignment(Qt::AlignCenter);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setMinimumSize(640, 480);
};

void GraphicsView::fitInView() {
    QRectF rect = QRectF(this->_photo->pixmap().rect());
    if (!rect.isNull()) {
        this->setSceneRect(rect);
        if (!this->_empty) {
            QRectF unity = this->transform().mapRect(QRectF(0, 0, 1, 1));
            this->scale(1 / unity.width(), 1 / unity.height());
            QRect viewrect = this->viewport()->rect();
            QRectF scenerect = this->transform().mapRect(rect);
            auto function = [](qreal first, qreal second) {return first<second? first:second; };
            qreal factor = function(viewrect.width() / scenerect.width(), viewrect.height() / scenerect.height());
            this->scale(factor, factor);
        }
        this->_zoom = 0;
    }
}
void GraphicsView::update(const cv::Mat& src) {
    this->_empty = false;
    cv::Mat img;
    if(src.channels()==1)
        cvtColor(src, img, CV_GRAY2RGB);
    else
        cvtColor(src, img, CV_BGR2RGB);
    int h = src.rows, w = src.cols, c = src.channels();
    QImage image = QImage(img.data, w, h, QImage::Format_RGB888);
    this->_photo->setPixmap(QPixmap::fromImage(image));
    this->fitInView();
};


void GraphicsView::wheelEvent(QWheelEvent* event) {
    qreal factor;
    if (!this->_empty) {
        if (event->angleDelta().y() > 0) {
            factor = 1.25;
            this->_zoom += 1;
        }
        else {
            factor = 0.8;
            this->_zoom -= 1;
        }
        if (this->_zoom > 0) {
            this->scale(factor, factor);
        }
        else if (this->_zoom == 0)
            this->fitInView();
        else
            this->_zoom = 0;

    }
};

GraphicsView::~GraphicsView() {

}