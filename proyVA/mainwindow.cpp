#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cap = new VideoCapture(0);
    if(!cap->isOpened())
        cap = new VideoCapture(1);
    capture = true;
    showColorImage = false;
    winSelected = false;
    nuevo = false;
    cap->set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap->set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    imgS = new QImage(320,240, QImage::Format_RGB888);
    visorS = new RCDraw(320,240, imgS, ui->imageFrameS);
    imgD = new QImage(320,240, QImage::Format_RGB888);
    visorD = new RCDraw(320,240, imgD, ui->imageFrameD);

    colorImage.create(240,320,CV_8UC3);
    grayImage.create(240,320,CV_8UC1);
    destColorImage.create(240,320,CV_8UC3);
    destGrayImage.create(240,320,CV_8UC1);
    gray2ColorImage.create(240,320,CV_8UC3);
    destGray2ColorImage.create(240,320,CV_8UC3);

    connect(&timer,SIGNAL(timeout()),this,SLOT(compute()));
    connect(ui->captureButton,SIGNAL(clicked(bool)),this,SLOT(start_stop_capture(bool)));
    connect(ui->colorButton,SIGNAL(clicked(bool)),this,SLOT(change_color_gray(bool)));
    connect(ui->loadButton,SIGNAL(clicked(bool)),this,SLOT(load_image()));
    connect(ui->saveButton,SIGNAL(clicked(bool)),this,SLOT(save_image()));
    connect(ui->copyButton,SIGNAL(clicked(bool)),this,SLOT(copy_image()));
    connect(ui->resizeButton,SIGNAL(clicked(bool)),this,SLOT(resize_image()));
    connect(ui->enlargeButton,SIGNAL(clicked(bool)),this,SLOT(enlarge_image()));
    connect(visorS,SIGNAL(windowSelected(QPointF, int, int)),this,SLOT(selectWindow(QPointF, int, int)));
    connect(visorS,SIGNAL(pressEvent()),this,SLOT(deselectWindow()));
    timer.start(60);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete cap;
    delete visorS;
    delete visorD;
    delete imgS;
    delete imgD;

}

void MainWindow::compute()
{

    if(capture && cap->isOpened())
    {
        *cap >> colorImage;

        cvtColor(colorImage, grayImage, CV_BGR2GRAY);
        cvtColor(colorImage, colorImage, CV_BGR2RGB);

    }


    if(showColorImage)
    {
        memcpy(imgS->bits(), colorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destColorImage.data , 320*240*3*sizeof(uchar));
    }
    else
    {
        cvtColor(grayImage,gray2ColorImage, CV_GRAY2RGB);
        cvtColor(destGrayImage,destGray2ColorImage, CV_GRAY2RGB);
        memcpy(imgS->bits(), gray2ColorImage.data , 320*240*3*sizeof(uchar));
        memcpy(imgD->bits(), destGray2ColorImage.data , 320*240*3*sizeof(uchar));

    }

    if(winSelected)
    {
        visorS->drawSquare(QPointF(imageWindow.x+imageWindow.width/2, imageWindow.y+imageWindow.height/2), imageWindow.width,imageWindow.height, Qt::green );
    }


    visorS->update();
    visorD->update();

}

void MainWindow::start_stop_capture(bool start)
{
    if(start)
    {
        ui->captureButton->setText("Stop capture");
        capture = true;
    }
    else
    {
        ui->captureButton->setText("Start capture");
        capture = false;
    }
}

void MainWindow::change_color_gray(bool color)
{
    if(color)
    {
        ui->colorButton->setText("Gray image");
        showColorImage = true;
    }
    else
    {
        ui->colorButton->setText("Color image");
        showColorImage = false;
    }
}

void MainWindow::selectWindow(QPointF p, int w, int h)
{
    QPointF pEnd;
    if(w>0 && h>0)
    {
        imageWindow.x = p.x()-w/2;
        if(imageWindow.x<0)
            imageWindow.x = 0;
        imageWindow.y = p.y()-h/2;
        if(imageWindow.y<0)
            imageWindow.y = 0;
        pEnd.setX(p.x()+w/2);
        if(pEnd.x()>=320)
            pEnd.setX(319);
        pEnd.setY(p.y()+h/2);
        if(pEnd.y()>=240)
            pEnd.setY(239);
        imageWindow.width = pEnd.x()-imageWindow.x;
        imageWindow.height = pEnd.y()-imageWindow.y;

        winSelected = true;
    }
}

void MainWindow::load_image()
{
     QString img = dialog.getOpenFileName(this,
                           tr("Open Image File"), ".",
                           tr("Image Files (*.jpg *.png *.bmp *.jpeg *.tiff *.tif *.dib *.jp2 *.jpe *.ppm *.pgm *.pbm *.ras *.sr)"));

     String im = img.toStdString();

    colorImage = imread(im, CV_BGR2RGB);
    cv::resize(colorImage, colorImage, cvSize(320, 240));
    cvtColor(colorImage, colorImage, CV_BGR2RGB);
    cvtColor(colorImage, grayImage, CV_BGR2GRAY);
    capture = false;
}

void MainWindow::save_image()
{
    QString img = dialog.getSaveFileName(this, tr("Save File"), ".", "Image Files (*.jpg)");
    String im = img.toStdString();
    copy_image();
    cvtColor(destColorImage, destColorImage, CV_RGB2BGR);
    imwrite(im,destColorImage);
}

void MainWindow::copy_image()
{
    colorImage.copyTo(destColorImage);
    grayImage.copyTo(destGrayImage);
    if(winSelected)
    {
        destGrayImage.setTo(0);
        destColorImage.setTo(0);
        Rect rango;
        rango.width=imageWindow.width;
        rango.height=imageWindow.height;
        rango.x = (320-imageWindow.width)/2;
        rango.y = (240-imageWindow.height)/2;
        Mat win = grayImage(imageWindow);
        win.copyTo(destGrayImage(rango));
        Mat won = colorImage(imageWindow);
        won.copyTo(destColorImage(rango));

    }
}

void MainWindow::resize_image()
{
    ui->loadButton->setText("HOLA");
}

void MainWindow::enlarge_image()
{
    ui->loadButton->setText("HOLA");
}

void MainWindow::deselectWindow()
{
    winSelected = false;
}


