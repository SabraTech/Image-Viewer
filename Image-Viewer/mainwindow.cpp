#include "mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow() : imageLabel(new QLabel), scrollArea(new QScrollArea), scaleFactor(1)
{
  imageLabel->setBackgroundRole(QPalette::Base);
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  imageLabel->setScaledContents(true);

  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setWidget(imageLabel);
  scrollArea->setVisible(false);
  setCentralWidget(scrollArea);

  createActions();

  resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

}

bool MainWindow::openFile(const QString &fileName){
  QImageReader reader(fileName);
  reader.setAutoTransform(true);
  const QImage newImage = reader.read();
  if(newImage.isNull()){
    QMessageBox::information(this.QGuiApplication::applicationDisplayName(),tr("Cannot Load %1: %2").arg(QDir::tonativeSeparators(fileName), reader.errorString()));

  }
  setImage(newImage);
  setWindowFilePath(fileName);
  const QSrting message = tr("Opend \"%1\", %2x%3").arg(QDir::tonativeSeparators(fileName)).arg(image.width()).arg(image.height());
  statusBar()->showMessage(message);
  return true;
}

void MainWindow::createActions(){
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
  

  QMenu *fileMenu = menuBar()->addMenu(tr("&Edit"));


  QMenu *fileMenu = menuBar()->addMenu(tr("&View"));


  QMenu *fileMenu = menuBar()->addMenu(tr("&About"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
