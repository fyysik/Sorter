#ifndef SortingWidget_H
#define SortingWidget_H

#ifndef ASIZE
#define ASIZE 20
#endif

#include <QWidget>
#include <QTextStream>
#include <QAction>
#include <windows.h>
#include "mainwindow.h"
class MainWindow;
class QAction;
class MySort;

struct DataToSort { //will be turned to class?
    int point;
    tArrayStates state;
};
//Q_DECLARE_METATYPE(DataToSort)

#ifndef YMAX
#define YMAX 800
#endif
class SortingWidget: public QWidget {
    Q_OBJECT
    signals:
        void algoChanged(QString);
        void debugChanged(QString);
        void statusChanged(QString);
        void stateChanged(tActions);
    public:
        SortingWidget(MainWindow *parent = 0);
        ~SortingWidget();
        DataToSort ArrayToSort[ASIZE];
        tActions m_action;
        QString m_actionString;
        int m_algo;
        QString m_algoString;
    public slots:
        void selectAction(QAction* newAction);
        void selectAlgo(QAction* newAction);
        void toRedrawEvent(tArrayStates st);
    protected:
        int asize = ASIZE;
        MySort* m_sorter = nullptr;
        QMap<QString, tAlgorithms> m_algorithms{{"All", ALL}, {"Bubble", BUBBLE},
                                 {"Quick", QUICK},
                                 {"Selection",SELECTION},
                                 {"Shell", SHELL},
                                                {"Coctail", COCTAIL}};
        QMap<QString, tActions> m_actions{{"Run selected", RUN},
                                     {"Stop", STOP}};
        QMap<QString, tArrayStates> m_states{{"Active", ACTIVE},
                                 {"Left", LEFT} ,
                                 {"Right", RIGHT},
                                 {"Sorted", SORTED},
                                 {"Neutral", NEUTRAL}};


        void paintEvent(QPaintEvent *event);
        bool m_mouse_down = false;
        MainWindow *m_mainWindow;

};

#endif // SortingWidget_H
