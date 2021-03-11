#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QObject>
#include <QMainWindow>
#include <QVector>
#include <QMap>
class SortingWidget;
class QMenu;
class QLabel;
class QAction;
class QActionGroup;
//class MySort;

enum  tAlgorithms {ALL = 0, BUBBLE, QUICK, SELECTION, SHELL, COCTAIL };
Q_DECLARE_METATYPE(tAlgorithms);

enum  tActions {STOP=0, RUN=1};
Q_DECLARE_METATYPE(tActions);

enum  tArrayStates {NEUTRAL=0, ACTIVE=1, LEFT=2, RIGHT = 3, SORTED =4};
Q_DECLARE_METATYPE(tArrayStates)

class MainWindow: public QMainWindow {
    Q_OBJECT
    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
        QLabel *m_debugLabel; //Not so good to set public, I understand, signal-slot is more nice solution
        QLabel *m_toolLabel;
         QLabel *m_statusLabel;
    public slots:
        void algoLabel(QString algo);
        void statusLabel(QString action);
        void debugLabel(QString dbs);
    protected:
        void setMenus();
        void prepareWidgets();
        void setStatuses();

        SortingWidget *m_SortingWidget;

        QMenu *m_fileMenu;
            QAction *m_quitAction;
         QMenu *m_runMenu;
            QAction *m_RunSelectedAction;
            QAction *m_RunAllAction;
            QAction *m_StopAction;
        QMenu *m_helpMenu;
            QAction *m_whatsThisAction;
            QAction *m_aboutAction;          
        QMenu *m_algorithmsMenu;
        QActionGroup *m_algoGroup;
                QAction *m_BubbleSortAction;
                QAction *m_QuickSortAction;
                QAction *m_SelectionSortAction;
                QAction *m_CoctailSortAction;
                QAction *m_AllSortAction;
};

#endif // MAINWINDOW_H
