#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QScrollArea>
#include <QSlider>
#include "lut3d.h"
#include "image.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_openAct_triggered();

    void on_exportLutAct_triggered();

    void on_exportImageAct_triggered();

    void on_fitToWindowAct_triggered();

    void on_fullSizeAct_triggered();

    void on_zoomInAct_triggered();

    void on_zoomOutAct_triggered();

    void on_sliderL_sliderReleased();
    void on_sliderA_sliderReleased();
    void on_sliderB_sliderReleased();

private:
    Lut3D *indentityLut3d;
    Lut3D *lut3d;
    double scaleFactor;
    Image* originalImage;
    Image* processedImage;

    Ui::MainWindow *ui;

    QLabel *originalImageLabel;
    QLabel *processedImageLabel;
    QScrollArea *originalScrollArea;
    QScrollArea *processedScrollArea;
    QSlider *sliderL;
    QSlider *sliderA;
    QSlider *sliderB;

    QAction *openAct;
    QAction *exportLutAct;
    QAction *exportImageAct;
    QAction *fitToWindowAct;
    QAction *fullSizeAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;

    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);
    void refresh();
};

#endif // MAINWINDOW_H
