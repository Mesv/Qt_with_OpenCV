#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include "opencv2/core/core.hpp"
#include "opencv/cv.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

QString fileName, fileName1;

MainWindow::MainWindow() {
    /*imageLabel = new QLabel;
    imageLabel -> setBackgroundRole(QPalette::Base);
    imageLabel -> setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel -> setScaledContents(true);*/

    //setCentralWidget(scrollArea);
    createActions();

    createMenus();

    createGridGroupBox();

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout -> setMenuBar(menuBar);
    mainLayout -> addWidget(gridGroupBox);

    setLayout(mainLayout);

    setWindowTitle(tr("Şablon Eşleştirme Uygulaması (Template Matching Application)"));

    /// Bu kod satırları ile uygulama pencreresini tam ekran yapıyoruz.
    QDesktopWidget *desk = qApp -> desktop();
    int width = desk -> width();
    int height = desk -> height();
    resize(width, height);
}

void MainWindow::createGridGroupBox() {
    QGridLayout *layout = new QGridLayout;

    gridGroupBox = new QGroupBox;

    label1 = new QLabel(tr("Kaynak Resmin Yolu: "));
    layout -> addWidget(label1, 0, 0);

    lineEdit1 = new QLineEdit;
    lineEdit1 -> setReadOnly(true);
    layout -> addWidget(lineEdit1, 0, 1);

    label2 = new QLabel(tr("Kaynak Resim: "));
    layout -> addWidget(label2, 1, 0);

    label3 = new QLabel;
    label3 -> setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label3 -> setScaledContents(true);

    layout -> addWidget(label3, 2, 0);

    label4 = new QLabel(tr("Şablon Resmin Yolu: "));
    layout -> addWidget(label4, 3, 0);

    lineEdit2 = new QLineEdit;
    lineEdit2 -> setReadOnly(true);
    layout -> addWidget(lineEdit2, 3, 1);

    label5 = new QLabel(tr("Şablon Resim: "));
    layout -> addWidget(label5, 4, 0);

    label6 = new QLabel;
    label6 -> setBackgroundRole(QPalette::Base);
    label6 -> setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label6 -> setScaledContents(true);
    layout -> addWidget(label6, 5, 0);

    label7 = new QLabel(tr("Şablon Eşleştirme İşleminin Sonucunun Görseli: "));
    layout -> addWidget(label7, 1, 2);

    label8 = new QLabel;
    layout -> addWidget(label8, 2, 2);

    label9 = new QLabel(tr("Sonuç: "));
    layout -> addWidget(label9, 3, 2);

    lineEdit3 = new QLineEdit;
    lineEdit3 -> setReadOnly(true);
    layout -> addWidget(lineEdit3, 3, 3);

    gridGroupBox -> setLayout(layout);
}

void MainWindow::openSourceImage() {
    //fileName = QFileDialog::getOpenFileName(this, tr("Dosyayı Aç"), QDir::currentPath());
    fileName = QFileDialog::getOpenFileName(this, tr("Dosyayı Aç"), "",
                tr("Resim Dosyaları(*.jpeg *.jpg *.png *.tif)"));
    lineEdit1 -> setText(fileName);

    if (!fileName.isEmpty()) {
        QImage image(fileName);

        if (image.isNull()) {
            QMessageBox::information(this, tr("Resim Görüntüleme"), tr("Yüklenemedi %1."));

            return;
        }

        label3 -> setPixmap((QPixmap::fromImage(image)));
    }
}

void MainWindow::openTemplateImage() {
    fileName1 = QFileDialog::getOpenFileName(this, tr("Dosyayı Aç"), "",
                                             tr("Resim Dosyaları(*.jpeg *.jpg *.png *.tif *.JPEG *.JPG *.PNG *.TIF)"));
    lineEdit2 -> setText(fileName);

    if (!fileName1.isEmpty()) {
        QImage image(fileName1);

        if (image.isNull()) {
            QMessageBox::information(this, tr("Resim Görüntüleme"), tr("Yüklenemedi %1."));

            return;
        }

        label6 -> setPixmap((QPixmap::fromImage(image)));
    }
}

void MainWindow::performTemplateMatching() {
    if(fileName.toStdString().empty() || fileName1.toStdString().empty()) {
        QMessageBox::information(this, tr("İşlem Sonucu"), tr("'Kaynak Resim' veya 'Şablon Resim' seçilmedi."));
        return;
    }

    Mat ref = imread(fileName.toStdString());
    Mat tpl = imread(fileName1.toStdString());

    if (ref.empty() || tpl.empty())
        return;

    Mat gref, gtpl;

    /// Kaynak resmi ve şablon resmi griton hale getiriyoruz.
    cvtColor(ref, gref, CV_BGR2GRAY);
    cvtColor(tpl, gtpl, CV_BGR2GRAY);

    Mat dst_ref, dst_templ;

    dst_ref = ref.clone();
    dst_templ = tpl.clone();

    /// Griton hale gelmiş kaynak resme ve şablon resmi Gaussian Filtresi uyguluyoruz.

    /* for ( int i = 1; i < 31; i = i + 2 ) {
           GaussianBlur( gref, dst_ref, Size( i, i ), 0, 0 );
    }

    for ( int i = 1; i < 31; i = i + 2 ) {
           GaussianBlur( gref, dst_templ, Size( i, i ), 0, 0 );
    }*/

    //GaussianBlur(gref, dst_ref, Size(3, 3), 0, 0 );
    /*cout << gref.cols << "  " << gref.rows << endl;
    cout << gtpl.cols << "  " << gtpl.rows << endl;*/

    int result_cols = (ref.cols + tpl.cols) - 1;
    int result_rows = (ref.rows + tpl.rows) - 1;

    Mat res(result_rows, result_cols, CV_32FC1);

    //cout << res.cols << "  " << res.rows << endl;

    threshold(gref, gref, 200, 255, THRESH_TRUNC);
    threshold(gtpl, gtpl, 200, 255, THRESH_TRUNC);

    GaussianBlur(gref, gref, Size(5, 5), 0, 0 );
    GaussianBlur(gtpl, gtpl, Size(5, 5), 0, 0 );

    imshow("gref", gref);
    imshow("gtpl", gtpl);

    //matchTemplate(dst_ref, dst_templ, res, CV_TM_CCOEFF_NORMED);
    //matchTemplate(gref, gtpl, res, CV_TM_CCOEFF_NORMED);
    matchTemplate(gref, gtpl, res, CV_TM_CCOEFF_NORMED);
    // cout << res.cols << "  " << res.rows << endl;
    //imshow("resource", res);
    //threshold(res, res, 0.8, 1., CV_THRESH_TOZERO);

    // cout << res.cols << "  " << res.rows << endl;
    //imshow("reference1", res);

    while (true)
    {
        double minval, maxval, threshold = 0.8;
        Point minLoc, maxLoc;
        minMaxLoc(res, &minval, &maxval, &minLoc, &maxLoc);

        //cout << maxLoc.x << "  " << maxLoc.y << endl;

        if (maxval >= threshold) {
            rectangle(
                    ref,
                    maxLoc,
                    Point(maxLoc.x + tpl.cols, maxLoc.y + tpl.rows),
                    CV_RGB(0,255,0), 2  // CV_RGB(0, 255, 0) ile çizilen dikdörtgenin rengi yeşil oluyor.
            );
            floodFill(res, maxLoc, Scalar(0), 0, Scalar(.1), Scalar(1.));
            //floodFill(res, maxLoc, CV_RGB(0,255,0), Scalar(3.5), Scalar(3.5), NULL, 4, NULL );
        }

        else
            break;
    }

    //imshow("reference2", ref);
    label8 -> setPixmap((QPixmap::fromImage(QImage((const unsigned char*)(ref.data), ref.cols, ref.rows, ref.step, QImage::Format_RGB888))));

    /*fitToWindowAct -> setEnabled(true);
    updateActions();

    if(!fitToWindowAct -> isChecked())
        label8 -> adjustSize();*/
}

void MainWindow::newOperation() {
    label3 -> clear();
    label6 -> clear();
    label8 -> clear();
    lineEdit1 -> setText("");
    lineEdit2 -> setText("");
    lineEdit3 -> setText("");
    fileName.clear();
    fileName1.clear();
}

void MainWindow::normalSize() {
    label8 -> adjustSize();
}

void MainWindow::fitToWindow() {
    scrollArea = new QScrollArea;
    scrollArea -> setWidget(label8);
    bool fitToWindow = fitToWindowAct -> isChecked();
    scrollArea -> setWidgetResizable(fitToWindow);

    if (!fitToWindow) {
        normalSize();
    }

    updateActions();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("Uygulama Hakkında"), tr("Bu uygulama MESV93 tarafından yapılmıştır. Bu uygulama ile <b>Şablon Eşleştirme (Template Matching)</b> işlemini gerçekleştirebilirsiniz. <p><b>İletişim: </b>MehmetSavascii@gmail.com</p>"));
}

void MainWindow::createActions() {
    openSourceAct = new QAction(tr("&Kaynak Resmi Aç..."), this);
    openSourceAct -> setShortcut(tr("Ctrl+S"));
    connect(openSourceAct, SIGNAL(triggered()), this, SLOT(openSourceImage()));

    openTemplateAct = new QAction(tr("Şablon Resmi Aç..."), this);
    openTemplateAct -> setShortcut(tr("Ctrl+T"));
    connect(openTemplateAct, SIGNAL(triggered()), this, SLOT(openTemplateImage()));

    performTemplateMatchingAct = new QAction(tr("Şablon Eşleştirme İşlemini Yap"), this);
    performTemplateMatchingAct -> setShortcut(tr("Ctrl+P"));
    connect(performTemplateMatchingAct, SIGNAL(triggered()), this, SLOT(performTemplateMatching()));

    newOperationAct = new QAction(tr("Yeni İşlem"), this);
    newOperationAct -> setShortcut(tr("Ctrl+N"));
    connect(newOperationAct, SIGNAL(triggered()), this, SLOT(newOperation()));

    exitAct = new QAction(tr("&Çıkış"), this);
    exitAct -> setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    normalSizeAct = new QAction(tr("&Normal Boyut"), this);
    normalSizeAct -> setShortcut(tr("Ctrl+N"));
    normalSizeAct -> setEnabled(false);
    connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

    fitToWindowAct = new QAction(tr("&Pencereye Sığdır"), this);
    fitToWindowAct -> setShortcut(tr("Ctrl+F"));
    fitToWindowAct -> setEnabled(false);
    fitToWindowAct -> setCheckable(true);
    connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

    aboutAct = new QAction(tr("Uygulama Hakkında"), this);
    aboutAct -> setShortcut(tr("Ctrl+H"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus() {
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&Dosya"), this);
    fileMenu -> addAction(openSourceAct);
    fileMenu -> addAction(openTemplateAct);
    fileMenu -> addAction(performTemplateMatchingAct);
    fileMenu -> addSeparator();
    fileMenu -> addAction(newOperationAct);
    fileMenu -> addSeparator();
    fileMenu -> addAction(exitAct);

    viewMenu = new QMenu(tr("Görünüm"), this);
    viewMenu -> addAction(normalSizeAct);
    viewMenu -> addSeparator();
    viewMenu -> addAction(fitToWindowAct);

    helpMenu = new QMenu(tr("Yardım"), this);
    helpMenu -> addAction(aboutAct);

    menuBar -> addMenu(fileMenu);
    menuBar -> addMenu(viewMenu);
    menuBar -> addMenu(helpMenu);
}

void MainWindow::updateActions() {
    normalSizeAct -> setEnabled(!fitToWindowAct -> isChecked());
}
