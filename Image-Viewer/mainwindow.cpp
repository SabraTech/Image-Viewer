#include "mainwindow.h"
#include <stack>
#include <QtWidgets>
#include <stack>


MainWindow::MainWindow() : imageLabel(new QLabel), scrollArea(new QScrollArea), scaleFactor(1) /*btn_rotateLeft(new QPushButton), btn_rotateRight(new QPushButton)*/
{
  //createUndoView();
  undoStack = new QStack<QImage>();
  redoStack = new QStack<QImage>();
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

  resize(QGuiApplication::primaryScreen()->availableSize());
  loadFile(tr("Pictures/Webcam/2016-09-16-204031.jpg"));
}

void MainWindow::mousePressEvent(QMouseEvent *event){
  event->accept();
  origin = event->pos();
  if(rubberBand){
      rubberBand ->close();
      rubberBand = NULL;
  }
  else{
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
  }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
  event->accept();
  if(rubberBand){
     rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
     rubberBand->show();
   }

}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
  event->accept();
  if(rubberBand){
      rubberBand->show();
   }
}

bool MainWindow::loadFile(const QString &fileName)
{

    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    const QImage originalImage = newImage;
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(newImage);

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}

void MainWindow::open(){
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString fileName = QFileDialog::getOpenFileName(this,
       tr("Open Images"), "/Pictures/Webcam/", tr("Image Files (*.png *.jpg *.bmp)"));
    while (dialog.exec() == QDialog::Accepted && !loadFile(fileName)) {}
}

void MainWindow::setImage(const QImage &newImage){

  undoStack->push(newImage);
  image = newImage;
  imageLabel->setPixmap(QPixmap::fromImage(image));
  scaleFactor = 1.0;

  scrollArea->setVisible(true);
  fitScreenAction->setEnabled(true);
  updateActions();
  if (!fitScreenAction->isChecked())
         imageLabel->adjustSize();

  //  new SetImageCommand(newImage,imageLabel);
}

bool MainWindow::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}


void MainWindow::saveAs(){
  QFileDialog dialog(this);
  dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
  dialog.setViewMode(QFileDialog::Detail);
  QString fileName = QFileDialog::getOpenFileName(this,
     tr("Save As Image"), "/Pictures/Webcam/", tr("Image Files (*.png *.jpg *.bmp)"));

    while (dialog.exec() == QDialog::Accepted && !saveFile(fileName)) {}

}

void MainWindow::undo(){
    if(undoStack->size()<=1)
           return;

    redoStack->push(image);
    undoStack->pop();
    QImage popped= undoStack->pop();
    setImage(popped);
}

void MainWindow::redo(){
    if(redoStack->size()==0)
           return;
    QImage popped= redoStack->pop();
    setImage(popped);

}

void MainWindow::reset(){
    setImage(originalImage);
}

void MainWindow::crop(){
    if(rubberBand){
       QImage copy ;
       double width = rubberBand->width();
       double height = rubberBand->height();
       int x = origin.rx();
       int y = origin.ry();
       copy = image.copy( x, y, width, height);
       rubberBand->close();
        setImage(copy);

    }
}
void MainWindow::rotateAngle(){

    bool ok;
    double value=0,min=-359,max=359;
    int decimals = 1;
    double rotationAngle =   QInputDialog::getDouble(this,
                                                            tr("Rotation"),
                                                            tr("Angle:"),
                                                             value,
                                                             min,
                                                             max ,
                                                             decimals,
                                                               &ok,

                                                          Qt::WindowFlags());

     if(ok)
    {
      QTransform trans;
      trans.rotate(rotationAngle);
      image = image.transformed(trans);
      setImage(image);
    }
}

void MainWindow::rotateLeft(){
  QTransform trans;
  trans.rotate(90);
  image = image.transformed(trans);
  delete redoStack;
  redoStack= new QStack<QImage>();
  setImage(image);
}

void MainWindow::rotateRight(){
  QTransform trans;
  trans.rotate(270);
  image = image.transformed(trans);
  delete redoStack;
  redoStack= new QStack<QImage>();
  setImage(image);
}

void MainWindow::zoomIn(){
    delete redoStack;
    redoStack= new QStack<QImage>();
    scaleImage(1.25);

}

void MainWindow::zoomOut(){
    delete redoStack;
    redoStack= new QStack<QImage>();
    scaleImage(0.75);
}

void MainWindow::fitScreen(){
  imageLabel->adjustSize();
  scaleFactor = 1.0;
}

void MainWindow::about(){
  QMessageBox::about(this, tr("About Image-Viewer"),
    tr("<p> The <b>Image-Viewer</b> Developed by"
        "<br>"
        "Mohamed Sabra"
        "<br>"
        "Ziad Hendawy"
        "<br>"
        "Ahmed Sallam"
        "<br>"
        ));
}


void MainWindow::createActions(){
  fileMenu = menuBar()->addMenu(tr("&File"));

  openAction = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
  openAction->setShortcut(QKeySequence::Open);

  saveAction = fileMenu->addAction(tr("&Save As..."), this, &MainWindow::saveAs);
  saveAction->setShortcut(tr("Ctrl+S"));
  saveAction->setEnabled(false);

  fileMenu->addSeparator();

  closeAction = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
  closeAction->setShortcut(tr("Ctrl+Q"));

  editeMenu = menuBar()->addMenu(tr("&Edit"));

  undoAction = editeMenu->addAction(tr("&Undo"), this, &MainWindow::undo);
  undoAction->setShortcut(tr("Ctrl+Z"));

  redoAction = editeMenu->addAction(tr("&Redo"), this, &MainWindow::redo);
  redoAction->setShortcut(tr("Ctrl+Y"));

  resetAction = editeMenu->addAction(tr("&Reset"), this, &MainWindow::reset);
  resetAction->setShortcut(tr("Ctrl+F"));

  editeMenu->addSeparator();

  cropAction = editeMenu->addAction(tr("&Crop"), this, &MainWindow::crop);
  cropAction->setShortcut(tr("Ctrl+C"));

  editeMenu->addSeparator();

  rotateAngleAction = editeMenu->addAction(tr("&Rotate By Angle"),this,&MainWindow::rotateAngle);

  rotateLeftAction = editeMenu->addAction(tr("&Rotate Left by 90"), this, &MainWindow::rotateLeft);
  rotateLeftAction->setShortcut(tr("Ctrl+L"));

  rotateRightAction = editeMenu->addAction(tr("&Rotate Right by 90"), this, &MainWindow::rotateRight);
  rotateRightAction->setShortcut(tr("Ctrl+R"));

  viewMenu = menuBar()->addMenu(tr("&View"));

  zoomInAction = viewMenu->addAction(tr("Zoom &In (25%)"), this, &MainWindow::zoomIn);
  zoomInAction->setShortcut(QKeySequence::ZoomIn);
  zoomInAction->setEnabled(false);

  zoomOutAction = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &MainWindow::zoomOut);
  zoomOutAction->setShortcut(QKeySequence::ZoomOut);
  zoomOutAction->setEnabled(false);

  fitScreenAction = editeMenu->addAction(tr("&Reset Zoom"), this, &MainWindow::fitScreen);
  fitScreenAction->setShortcut(tr("Ctrl+0"));
  fitScreenAction->setEnabled(false);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(tr("&About"), this, &MainWindow::about);
}

void MainWindow::updateActions(){
    saveAction->setEnabled(!image.isNull());
    zoomInAction->setEnabled(!image.isNull());
    zoomOutAction->setEnabled(!image.isNull());
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
