#include "mainwindow.h"


#include <QApplication>
#include <QEvent>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include <QWhatsThis>
#include "sortingwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    prepareWidgets();
    setMenus();
    setStatuses();
}

MainWindow::~MainWindow() {
    // Intentionally empty
}

/**
  Initializes all menus of this widget.
*/
void MainWindow::setMenus() {
    // Creates the file menu
    m_fileMenu = new QMenu(this);
    m_fileMenu->setTitle(QString("&Sorter"));
    menuBar()->addMenu(m_fileMenu);
    // Creates the quit action in the file menu
    m_quitAction = new QAction(this);  
    m_quitAction->setMenuRole(QAction::QuitRole);
    m_quitAction->setText(QString("&Quit"));
    m_fileMenu->addAction(m_quitAction);
    connect(m_quitAction, SIGNAL(triggered()),
            this, SLOT(close()));
    m_runMenu = new QMenu(this);
    m_runMenu->setTitle(QString("&Run"));
    menuBar()->addMenu(m_runMenu);

    m_StopAction = new QAction(this);
    m_StopAction->setText(QString("Stop"));
    m_runMenu->addAction(m_StopAction);
    m_RunSelectedAction = new QAction(this);
    m_RunSelectedAction->setText(QString("Run selected"));
    m_runMenu->addAction(m_RunSelectedAction);
    connect(m_runMenu, SIGNAL(triggered(QAction*)),
            m_SortingWidget, SLOT(selectAction(QAction*)));
    // Create tools menu

    m_algorithmsMenu = new QMenu(this);
    m_algorithmsMenu->setTitle(QString("&Algorithms"));
    QActionGroup *m_algoGroup = new QActionGroup(this);

    m_BubbleSortAction = new QAction(this);
    m_BubbleSortAction->setText("Bubble");
    m_BubbleSortAction->setCheckable(true);
    m_BubbleSortAction->setChecked(true);
    //m_add_vertexAction->setData(stat); There are mystical problems with Qvariant here :(((


    m_QuickSortAction = new QAction(this);
    m_QuickSortAction->setText(QString("Quick"));
    m_QuickSortAction->setCheckable(true);


    m_SelectionSortAction = new QAction(this);
    m_SelectionSortAction->setText(QString("Selection"));
    m_SelectionSortAction->setCheckable(true);


    /*m_ShellSortAction = new QAction(this);
    m_ShellSortAction->setText(QString("Shell"));
    m_algorithmsMenu->addAction(m_ShellSortAction);*/

    m_CoctailSortAction = new QAction(this);
    m_CoctailSortAction->setText(QString("Coctail"));
    m_CoctailSortAction->setCheckable(true);

    m_AllSortAction = new QAction(this);
    m_AllSortAction->setText(QString("All"));
    m_AllSortAction->setCheckable(true);

    m_algoGroup->addAction(m_BubbleSortAction);
    m_algoGroup->addAction(m_QuickSortAction);
    m_algoGroup->addAction(m_SelectionSortAction);
    m_algoGroup->addAction(m_CoctailSortAction);
    m_algoGroup->addAction(m_AllSortAction);

    m_algorithmsMenu->addAction(m_BubbleSortAction);
    m_algorithmsMenu->addAction(m_QuickSortAction);
    m_algorithmsMenu->addAction(m_SelectionSortAction);
    m_algorithmsMenu->addAction(m_CoctailSortAction);
    m_algorithmsMenu->addAction(m_AllSortAction);
    connect(m_algorithmsMenu, SIGNAL(triggered(QAction*)),
            m_SortingWidget, SLOT(selectAlgo(QAction*)));
    menuBar()->addMenu(m_algorithmsMenu);

    // Create the help menu and its contents
    m_helpMenu = new QMenu(this);
    m_helpMenu->setTitle(QString("&Help"));
    menuBar()->addMenu(m_helpMenu);

    m_whatsThisAction = QWhatsThis::createAction(this);
    m_whatsThisAction->setText(QString("&Whats this?"));
    m_helpMenu->addAction(m_whatsThisAction);

    m_aboutAction = new QAction(this);
    m_aboutAction->setMenuRole(QAction::AboutQtRole);
    m_aboutAction->setText(QString("&About"));
    m_helpMenu->addAction(m_aboutAction);
    connect(m_aboutAction, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
}
/*!

*/
void MainWindow::setStatuses(){
    m_toolLabel = new QLabel(QString("Algorithm:Bubble"),this);
    statusBar()->addWidget(m_toolLabel);

    m_statusLabel = new QLabel(QString("Status:Stop"),this);
    statusBar()->addWidget(m_statusLabel);

    m_debugLabel = new QLabel(QString("Debug"),this);
    statusBar()->addWidget(m_debugLabel);
}
/**
  Translates all texts of this widget.
*/
void MainWindow::prepareWidgets() {
    setWindowTitle(QString("Sorting"));

    m_SortingWidget = new SortingWidget(this);
    m_SortingWidget->setGeometry(0,0,1024,800);
    setCentralWidget(m_SortingWidget);
}

void MainWindow::algoLabel(QString algo){
    algo = "Algorithm:" + algo;
    m_toolLabel->setText(algo);
}
void MainWindow::statusLabel(QString action){
    action = "Status:" + action;
    m_statusLabel->setText(action);
}

void MainWindow::debugLabel(QString dbs){
    m_debugLabel->setText(dbs);
}


