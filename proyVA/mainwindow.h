#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <rcdraw.h>



using namespace cv;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer timer;

    VideoCapture *cap;
    RCDraw *visorS, *visorD;
    QImage *imgS, *imgD;
    Mat colorImage, grayImage, destColorImage, destGrayImage;
    Mat gray2ColorImage, destGray2ColorImage;
    bool capture, showColorImage, winSelected, nuevo, warpZoom;
    Rect imageWindow;
    QFileDialog dialog;


public slots:
    void compute();
    void start_stop_capture(bool start);
    void warp_zoom(bool warp);
    void change_color_gray(bool color);
    void load_image();
    void save_image();
    void copy_image();
    void resize_image();
    void enlarge_image();
    void selectWindow(QPointF p, int w, int h);
    void deselectWindow();

};


#endif // MAINWINDOW_H
