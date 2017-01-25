#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

class QAction;
class QMenu;
class QActionGroup;
class QLabel;
class QScrollArea;
class QScrollBar;
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
      void crop();
      void rotateLeft();
      void rotateRight();
      void zoomIn();
      void zoomOut();
      void fitScreen();
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

    QImage image;
    QImage originImage;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor;
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
    QAction *rotateLeftAction;
    QAction *rotateRightAction;
    QAction *zoomInAction;
    QAction *zoomOutAction;
    QAction *fitScreenAction;

protected:

//#ifndef QT_NO_CONTEXTMENU
    //void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
//#endif // QT_NO_CONTEXTMENU

};

#endif // MAINWINDOW_H
