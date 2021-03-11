#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QObject>
#include <QMainWindow>
#include "Point2.h"
#include "DynamicLine.h"
#include <QVector>
#include <QMap>
class DrawingWidget;
class QMenu;
class QLabel;
class QAction;

enum  tActions {DO_NOTHING=0, ADD_VERTEX=1, MOV_VERTEX=2, DEL_VERTEX=3, ADD_LINE=4, DEL_LINE=5};
Q_DECLARE_METATYPE(tActions);

class MainWindow: public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
        QLabel *m_debugLabel; //Not so good to set public, I understand, signal-slot is more nice solution
        QLabel *m_toolLabel;
        QLabel *m_verticesLabel;
        QLabel *m_linesLabel;
    public slots:
        void toolsLabel(QString* tools);
        void verticesLabel(int);
        void linesLabel(int);
        void debugLabel(QString dbs);
    protected:
        void initMenus();
        void initWidgets();
        void initStatuses();

        DrawingWidget *m_drawingWidget;

        QMenu *m_fileMenu;
            QAction *m_quitAction;
        QMenu *m_helpMenu;
            QAction *m_whatsThisAction;
            QAction *m_aboutAction;          
        QMenu *m_toolsMenu;
                QAction *m_add_vertexAction;
                QAction *m_mov_vertexAction;
                QAction *m_del_vertexAction;
                QAction *m_add_lineAction;
                QAction *m_del_lineAction;
};

#endif // MAINWINDOW_H
