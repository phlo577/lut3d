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


#include "QDebug"

#define INTERVAL 273
#define MAX_INDEX 16

#define IMG_WIDTH  720
#define IMG_HEIGHT 540


static lab_t lab;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    indentityLut3d = new Lut3D(INTERVAL, MAX_INDEX);
    lut3d = new Lut3D(INTERVAL, MAX_INDEX);

    exportImageAct = ui->exportImageAct;
    exportLutAct   = ui->exportLutAct;
    openAct        = ui->openAct;
    exportLutAct   = ui->exportLutAct;
    exportImageAct = ui->exportImageAct;
    fitToWindowAct = ui->fitToWindowAct;
    fullSizeAct    = ui->fullSizeAct;
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

        originalImageLabel->setPixmap(QPixmap::fromImage(originalImage->getImage8bit()));


        processedImage->process(indentityLut3d);
        processedImageLabel->setPixmap(QPixmap::fromImage(processedImage->getImage8bit()));



        // Enable view actions
        fitToWindowAct->setEnabled(true);
        exportImageAct->setEnabled(true);
        fullSizeAct->setEnabled(true);
        zoomInAct->setEnabled(true);
        zoomOutAct->setEnabled(true);
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

void MainWindow::on_exportImageAct_triggered()
{

}

void MainWindow::on_fitToWindowAct_triggered()
{

}

void MainWindow::on_fullSizeAct_triggered()
{
    originalImageLabel->adjustSize();
    processedImageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::on_zoomInAct_triggered()
{
    scaleImage(1.25);
}

void MainWindow::on_zoomOutAct_triggered()
{
    scaleImage(0.8);
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

void  MainWindow::refresh(void)
{
    processedImage = new Image(IMG_WIDTH,IMG_HEIGHT, originalImage->getImage8bit());
    processedImage->process(lut3d);
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage->getImage8bit()));
    processedImageLabel->repaint();
}

void MainWindow::on_sliderL_sliderReleased(void)
{
    for (uint8_t b = 0; b < MAX_INDEX; b++)
    {
        for (uint8_t g = 0; g < MAX_INDEX; g++)
        {
            for (uint8_t r = 0; r < MAX_INDEX; r++)
            {
                QRgba64 rgb = indentityLut3d->getEntry(r, g, b);
                lab = rgb2lab(rgb);
                lab.L += sliderL->value();
                rgb = lab2rgb(lab);
                lut3d->setEntry(r,g,b,rgb);
            }
        }
    }
    qDebug() << sliderL->value();
    refresh();
}

void MainWindow::on_sliderA_sliderReleased(void)
{
    for (uint8_t b = 0; b < MAX_INDEX; b++)
    {
        for (uint8_t g = 0; g < MAX_INDEX; g++)
        {
            for (uint8_t r = 0; r < MAX_INDEX; r++)
            {
                QRgba64 rgb = indentityLut3d->getEntry(r, g, b);
                lab = rgb2lab(rgb);
                lab.a += sliderA->value();
                rgb = lab2rgb(lab);
                lut3d->setEntry(r,g,b,rgb);
            }
        }
    }
    qDebug() << sliderA->value();
    refresh();
}

void MainWindow::on_sliderB_sliderReleased(void)
{
    for (uint8_t b = 0; b < MAX_INDEX; b++)
    {
        for (uint8_t g = 0; g < MAX_INDEX; g++)
        {
            for (uint8_t r = 0; r < MAX_INDEX; r++)
            {
                QRgba64 rgb = indentityLut3d->getEntry(r, g, b);
                lab = rgb2lab(rgb);
                lab.b += sliderB->value();
                rgb = lab2rgb(lab);
                lut3d->setEntry(r,g,b,rgb);
            }
        }
    }
    qDebug() << sliderB->value();
    refresh();
}
