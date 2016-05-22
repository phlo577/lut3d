#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QString>
#include <QFile>
#include <QtWidgets>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image.h"
#include "color.h"
#include "lab.h"


#include "QDebug"

#define INTERVAL 273
#define MAX_INDEX 16

#define IMG_WIDTH  720
#define IMG_HEIGHT 540


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    identityLut3d = new Lut3D(INTERVAL, MAX_INDEX);
    lut3d = new Lut3D(INTERVAL, MAX_INDEX);

    openAct        = ui->openAct;
    exportLutAct   = ui->exportLutAct;
    exitApp        = ui->exitAct;
    fitToWindowAct = ui->fitToWindowAct;
    actualSizeAct  = ui->actualSizeAct;
    zoomInAct      = ui->zoomInAct;
    zoomOutAct     = ui->zoomOutAct;

    setWindowTitle(tr("3D LUT Generator"));

    originalImageLabel = new QLabel;
    processedImageLabel = new QLabel;

    originalScrollArea = ui->originalScrollArea;
    processedScrollArea = ui->processedScrollArea;

    originalImageLabel->setBackgroundRole(QPalette::Base);
    originalImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    originalImageLabel->setScaledContents(true);
    originalImageLabel->setMinimumSize(1,1);

    processedImageLabel->setBackgroundRole(QPalette::Base);
    processedImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    processedImageLabel->setScaledContents(true);
    processedImageLabel->setMinimumSize(1,1);

    originalScrollArea->setBackgroundRole(QPalette::Dark);
    originalScrollArea->setWidget(originalImageLabel);
    originalScrollArea->setWidgetResizable(false);

    processedScrollArea->setBackgroundRole(QPalette::Dark);
    processedScrollArea->setWidget(processedImageLabel);
    processedScrollArea->setWidgetResizable(false);

    sliderL = ui->sliderL;
    sliderA = ui->sliderA;
    sliderB = ui->sliderB;

    sliderL->setEnabled(false);
    sliderA->setEnabled(false);
    sliderB->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openAct_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

    if (!filename.isEmpty())
    {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly))
            return;
        QByteArray imgBuffer = file.readAll();
        file.close();

        originalImage = new Image(IMG_WIDTH, IMG_HEIGHT, (uint8_t*)(imgBuffer.data()), true);
        processedImage = new Image(IMG_WIDTH,IMG_HEIGHT, originalImage->getImage8bit());

        processLab(50,0,0,identityLut3d, lut3d);
        originalImage->process(lut3d);
        originalImageLabel->setPixmap(QPixmap::fromImage(originalImage->getImage8bit()));

        processedImage->process(lut3d);
        processedImageLabel->setPixmap(QPixmap::fromImage(processedImage->getImage8bit()));


        // Enable view actions
        fitToWindowAct->setEnabled(true);
        actualSizeAct->setEnabled(true);
        zoomInAct->setEnabled(true);
        zoomOutAct->setEnabled(true);
        sliderL->setEnabled(true);
        sliderA->setEnabled(true);
        sliderB->setEnabled(true);
    }
}


void MainWindow::on_exportLutAct_triggered()
{
    QString str = lut3d->getString();
    //Open text file
    QString filename = "../lut3d.3DL";
    QFile file(filename);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&file);
    //Export 3D Lut data to text stream
    out<<str<<endl;
    //Close file
    file.close();
}

void MainWindow::on_exitAct_triggered()
{
    qApp->exit();
}

void MainWindow::on_fitToWindowAct_triggered()
{
    bool fitToWindow = fitToWindowAct->isChecked();
    originalScrollArea->setWidgetResizable(fitToWindow);
    processedScrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        on_actualSizeAct_triggered();
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    actualSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void MainWindow::on_actualSizeAct_triggered()
{
    originalImageLabel->adjustSize();
    processedImageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::on_zoomInAct_triggered()
{
    scaleImage(1.25);
    originalImageLabel->repaint();
    processedImageLabel->repaint();
}

void MainWindow::on_zoomOutAct_triggered()
{
    scaleImage(0.8);
    originalImageLabel->repaint();
    processedImageLabel->repaint();
}

void MainWindow::scaleImage(double factor)
{
    Q_ASSERT(originalImageLabel->pixmap());
    Q_ASSERT(processedImageLabel->pixmap());

    scaleFactor *= factor;

    originalImageLabel->resize(scaleFactor * originalImageLabel->pixmap()->size());
    processedImageLabel->resize(scaleFactor * processedImageLabel->pixmap()->size());

    adjustScrollBar(processedScrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(processedScrollArea->verticalScrollBar(), factor);
    adjustScrollBar(originalScrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(originalScrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.1);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::on_sliderL_sliderReleased(void)
{
    processLab(sliderL->value(), sliderA->value(), sliderB->value(), identityLut3d, lut3d);
    refresh();
}

void MainWindow::on_sliderA_sliderReleased(void)
{
    processLab(sliderL->value(), sliderA->value(), sliderB->value(), identityLut3d, lut3d);
    refresh();
}

void MainWindow::on_sliderB_sliderReleased(void)
{
    processLab(sliderL->value(), sliderA->value(), sliderB->value(), identityLut3d, lut3d);
    refresh();
}


void  MainWindow::refresh(void)
{
    processedImage = new Image(IMG_WIDTH,IMG_HEIGHT, originalImage->getImage8bit());
    processedImage->process(lut3d);
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage->getImage8bit()));
    processedImageLabel->repaint();
}
