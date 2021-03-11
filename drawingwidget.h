#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QTextStream>
#include <QAction>
#include "mainwindow.h"
class MainWindow;
class QAction;

struct PrimitiveSet { //will be turned to class
    QVector<Point2*> points;
    QVector<DynamicLine<Point2>> lines;
};

class DrawingWidget: public QWidget {
    Q_OBJECT
    signals:
        void toolsChanged(QString*);
        void debugChanged(QString);
        void verticesChanged(int);
        void linesChanged(int);
    public:
        DrawingWidget(MainWindow *parent = 0);
        ~DrawingWidget();

        int m_Action;
        Point2 m_eventPoint;
        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
    public slots:
        //void selectAction(DrawingAction act, QAction *qa);
        void selectAction(QAction* newAction);
    protected: //will go into class
    QMap<QString, int> m_actions{{"Add vertex", 1},
                                 {"Move vertex", 2},
                                 {"Delete vertex",3},
                                 {"Add line", 4},
                                 {"Delete line", 5}};
        bool addPoint2(Point2& p); //D = 20px
        bool movePoint(Point2 oldp, Point2 newp);
        bool delPoint2(Point2* p);
        bool addDynamicLine(DynamicLine<Point2>& l);
        //bool delDynamicLine(DynamicLine<Point2> l);
        bool matchPoint(Point2 p1, Point2 p2, unsigned d = 10); //10px distance
        bool matchLine(DynamicLine<Point2> l,  Point2 p); //1px distance

    protected:
        void paintEvent(QPaintEvent *event);
        MainWindow *m_mainWindow;
        PrimitiveSet *m_itemsToDraw;
        void action_addVertex(QMouseEvent *e);
        void action_delVertex(QMouseEvent *e);
        void action_addSide( QMouseEvent *e);
        void action_delSide(QMouseEvent *e);
        void action_movVertex(QMouseEvent *e);
        /// for line matching testing
        void paintLine(DynamicLine<Point2> l, QPainter *painter);
        int m_1point = -1;
        int m_2point = -1;
        bool m_1point_selected = false;
        bool m_mouse_pressed = false;
        bool m_mouse_moving = false;
        bool m_vertex_catched = false;
        unsigned m_vtc_to_move = -1;
};

#endif // DRAWINGWIDGET_H
