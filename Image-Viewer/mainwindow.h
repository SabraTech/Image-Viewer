#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QUndoStack>
#include <QUndoView>
#include <QRect>
#include <QRubberBand>

class QAction;
class QMenu;
class QActionGroup;
class QLabel;
class QScrollArea;
class QScrollBar;
class QUndoStack;
class QUndoView;
//class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
      MainWindow();
      bool loadFile(const QString &);

  private slots:
      void open();
      void saveAs();
      void undo();
      void redo();
      void reset();

      //crop
      void crop();

      //rotation
      void rotateLeft();
      void rotateRight();
      void rotateAngle();

      //zoom
      void zoomIn();
      void zoomOut();
      void fitScreen();

      //about
      void about();


private:
    void createMenus();
    void createActions();
    void updateActions();
    bool openFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QPoint origin;
    QImage image;
    QImage originalImage;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;
    bool click ;

    QRubberBand *rubberBand = NULL;
    QPoint myPoint;
    //QPushButton btn_rotateLeft;
    //QPushButton btn_rotateRight;

    QMenu *fileMenu;
    QMenu *editeMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QAction *openAction;
    QAction *saveAction;
    QAction *closeAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *resetAction;
    QAction *cropAction;
    QAction *rotateAngleAction;
    QAction *rotateLeftAction;
    QAction *rotateRightAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *fitScreenAction;

    QStack<QImage> *undoStack ;
    QStack<QImage> *redoStack ;

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

//#ifndef QT_NO_CONTEXTMENU
    //void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
//#endif // QT_NO_CONTEXTMENU

};

#endif // MAINWINDOW_H
