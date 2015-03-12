#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void openSourceImage();
    void openTemplateImage();
    void performTemplateMatching();
    void newOperation();
    void normalSize();
    void fitToWindow();
    void about();

private:
    void createGridGroupBox();
    void createActions();
    void createMenus();
    void updateActions();

    QMenuBar *menuBar;
    QLabel *label1, *label2, *label3, *label4, *label5, *label6, *label7, *label8, *label9;
    QScrollArea *scrollArea;
    QGroupBox *gridGroupBox;
    QLineEdit *lineEdit1, *lineEdit2, *lineEdit3;

    QAction *openSourceAct;
    QAction *openTemplateAct;
    QAction *performTemplateMatchingAct;
    QAction *newOperationAct;
    QAction *exitAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *aboutAct;

    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
};

#endif // MAINWINDOW_H
