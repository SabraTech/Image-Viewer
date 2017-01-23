#include "mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow() : imageLabel(new QLabel), scrollArea(new QScrollArea), scaleFactor(1) /*btn_rotateLeft(new QPushButton), btn_rotateRight(new QPushButton)*/
{
  imageLabel->setBackgroundRole(QPalette::Base);
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  imageLabel->setScaledContents(true);

  //btn_rotateLeft->setText(tr("Left 90"));
  //btn_rotateRight->setText(tr("Right 90"));


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
  originImage = reader.read();
  if(newImage.isNull()){
    QMessageBox::information(this.QGuiApplication::applicationDisplayName(),tr("Cannot Load %1: %2").arg(QDir::tonativeSeparators(fileName), reader.errorString()));

  }
  setImage(newImage);
  setWindowFilePath(fileName);
  const QSrting message = tr("Opend \"%1\", %2x%3").arg(QDir::tonativeSeparators(fileName)).arg(image.width()).arg(image.height());
  statusBar()->showMessage(message);
  return true;
}

void MainWindow::open(){
  QFileDialog dialog(this, tr("Open File"));
  dialog.setMimeTypeFilters(mimeTypeFilters);
  dialog.selectMimeTypeFilter("image/jpeg");

  while(dialog.exec() == QDialog::Accepted && !openFile(dialog.selectedFiles().first()){}
}

void MainWindow::setImage(const QImage &newImage){
  image = newImage;
  imageLabel->setPixmap(QPixmap::fromImage(image));
  scaleFactor = 1.0;

  scrollArea->setVisible(true);
  updateActions();
}

bool MainWindow::saveFile(const QString &fileName){
  QImageWriter writer(filename);

  if(!writer.write(image)){
    QMessageBox::information(this, QGuiApplication::applicationDisplayName(), tr("Cannot write %1: %2").arg(QDir::tonativeSeparators(filename)), writer.errorString());
  }
  const QString message = tr("Wrote \"%1\"").arg(QDir::tonativeSeparators(filename));
  statusBar()->showMessage(message);
  return true;
}

void MainWindow::saveAs(){

}

void MainWindow::undo(){

}

void MainWindow::redo(){

}

void MainWindow::reset(){
  setImage(originImage);
}

void MainWindow::crop(){

}

void MainWindow::rotateLeft(){
  QTransform trans;
  trans.rotate(90);
  image = image.transformed(trans);
  setImage(image);
}

void MainWindow::rotateRight(){
  QTransform trans;
  trans.rotate(270);
  image = image.transformed(trans);
  setImage(image);
}

void MainWindow::zoomIn(){

}

void MainWindow::zoomOut(){

}

void MainWindow::fitScreen(){
  imageLabel->adjustSize();
  scaleFactor = 1.0;
}

void MainWindow::about(){
  QMessageBox::about(this, tr("About Image-Viewer"),
    tr("<p> The <b>Image-Viewer</b> Developed by
        <br>
        Mohamed Sabra
        Ziad Hendawy
        Ahmed Sallam
        <br>
        "));
}


void MainWindow::createActions(){
  fileMenu = menuBar()->addMenu(tr("&File"));

  openAction = fileMenu->addAction(tr("&Open"), this, &MainWindow::open);
  openAction->setShortcut(QKeySequence::Open);

  saveAction = fileMenu->addAction(tr("&Save As"), this, &MainWindow::saveAs);
  saveAction->setShortcut("Ctrl+S");
  saveAction->setEnabled(false);

  fileMenu->addSeparator();

  closeAction = fileMenu->addAction(tr("&Exit"), this, &QtWidget::close);
  closeAction->setShortcut("Ctrl+Q");

  editeMenu = menuBar()->addMenu(tr("&Edit"));

  undoAction = editeMenu->addAction(tr("&Undo"), this, &MainWindow::undo);
  undoAction->setShortcut("Ctrl+Z");

  redoAction = editeMenu->addAction(tr("&Redo"), this, &MainWindow::redo);
  redoAction->setShortcut("Ctrl+Y");

  resetAction = editeMenu->addAction(tr("&Reset"), this, &MainWindow::reset);
  resetAction->setShortcut("Ctrl+F");

  editeMenu->addSeparator();

  cropAction = editeMenu->addAction(tr("&Crop"), this, &MainWindow::crop);
  cropAction->setShortcut("Ctrl+C");

  editeMenu->addSeparator();

  rotateLeftAction = editeMenu->addAction(tr("&Rotate Left by 90"), this, &MainWindow::rotateLeft);
  rotateLeftAction->setShortcut("Ctrl+L");

  rotateRightAction = editeMenu->addAction(tr("&Rotate Right by 90"), this, &MainWindow::rotateRight);
  rotateRightAction->setShortcut("Ctrl+R");

  viewMenu = menuBar()->addMenu(tr("&View"));

  zoomInAction = editeMenu->addAction(tr("&Zoom In"), this, &MainWindow::ZoomIn);
  zoomInAction->setShortcut(QKeySequence::ZoomIn);
  zoomInAction->setEnabled(false);

  zoomOutAction = editeMenu->addAction(tr("&Zoom Out"), this, &MainWindow::ZoomOut);
  zoomOutAction->setShortcut(QKeySequence::ZoomOut);
  zoomOutAction->setEnabled(false);

  fitScreenAction = editeMenu->addAction(tr("&Reset Zoom"), this, &MainWindow::fitScreen);
  fitScreenAction->setShortcut("Ctrl+0");
  fitScreenAction->setEnabled(false);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(tr("&About"), this, &MainWindow::about);
}

void MainWindow::updateActions(){
    saveAction->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!image.isNull());
    zoomOutAction->setEnabled(!image.isNull());
    normalSizeAct->setEnabled(!image.isNull);
}

void MainWindow::scaleImage(double factor){
  Q_ASSERT(imageLabel->pixmap());
  scaleFactor *= factor;
  imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

  adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
  adjustScrollBar(scrollArea->verticalScrollBar(), factor);

  zoomInAction->setEnabled(scaleFactor < 3.0);
  zoomOutAction->setEnabled(scaleFactor > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor){
  scrollBar->setValue(int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep()/2)));
}
