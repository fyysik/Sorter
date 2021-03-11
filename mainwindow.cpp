#include "mainwindow.h"


#include <QApplication>
#include <QEvent>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QLabel>
#include <QWhatsThis>
#include "drawingwidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	initWidgets();
    initMenus();
    initStatuses();
}

MainWindow::~MainWindow() {
    // Intentionally empty
}

/**
  Initializes all menus of this widget.
*/
void MainWindow::initMenus() {
    // Creates the file menu
    m_fileMenu = new QMenu(this);
    m_fileMenu->setTitle(QString("&File"));
    menuBar()->addMenu(m_fileMenu);
    // Creates the quit action in the file menu
    m_quitAction = new QAction(this);
    m_quitAction->setMenuRole(QAction::QuitRole);
    m_quitAction->setText(QString("&Quit"));
    m_quitAction->setStatusTip(QString("Quits this application"));
    m_quitAction->setToolTip(QString("Quits this application"));
    m_quitAction->setWhatsThis(QString("Activate this item to quit this application. You will"
                                  " be asked for confirmation."));
    m_fileMenu->addAction(m_quitAction);
    // Connects the triggering of the quit action
    // to closing the main window
    connect(m_quitAction, SIGNAL(triggered()),
            this, SLOT(close()));
    QVariant stat;
    // Create tools menu
    m_toolsMenu = new QMenu(this);
    m_toolsMenu->setTitle(QString("Tools"));

    m_add_vertexAction = new QAction(this);
    m_add_vertexAction->setText("Add vertex");
    //m_add_vertexAction->setData(stat); There are mystical problems with Qvariant here :(((
    m_toolsMenu->addAction(m_add_vertexAction);

    m_mov_vertexAction = new QAction(this);
    m_mov_vertexAction->setText(QString("Move vertex"));
    m_toolsMenu->addAction(m_mov_vertexAction);

    m_del_vertexAction = new QAction(this);
    m_del_vertexAction->setText(QString("Delete vertex"));
    m_toolsMenu->addAction(m_del_vertexAction);

    m_add_lineAction = new QAction(this);
    m_add_lineAction->setText(QString("Add line"));
    m_toolsMenu->addAction(m_add_lineAction);

    m_del_lineAction = new QAction(this);
    m_del_lineAction->setText(QString("Delete line"));
    m_toolsMenu->addAction(m_del_lineAction);

    connect(m_toolsMenu, SIGNAL(triggered(QAction*)),
            m_drawingWidget, SLOT(selectAction(QAction*)));
    menuBar()->addMenu(m_toolsMenu);

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
void MainWindow::initStatuses(){
    m_toolLabel = new QLabel(QString("empty"),this);
    statusBar()->addWidget(m_toolLabel);

    m_verticesLabel = new QLabel(QString("Vertices"),this);
    statusBar()->addWidget(m_verticesLabel);

    m_linesLabel = new QLabel(QString("Lines"),this);
    statusBar()->addWidget(m_linesLabel);
    m_debugLabel = new QLabel(QString("debug"),this);
    statusBar()->addWidget(m_debugLabel);
}
/**
  Translates all texts of this widget.
*/
void MainWindow::initWidgets() {
    setWindowTitle(QString("Drawing"));

    m_drawingWidget = new DrawingWidget(this);
    m_drawingWidget->setGeometry(0,0,800,600);
    setCentralWidget(m_drawingWidget);
}

void MainWindow::toolsLabel(QString* tools){
    m_toolLabel->setText(*tools);
}
void MainWindow::verticesLabel(int v){
    QString vcs("Vertices:");
    vcs += QString::number(v);
    m_verticesLabel->setText(vcs);
}
void MainWindow::linesLabel(int l){
    QString ls("Lines:");
    ls += QString::number(l);
    m_linesLabel->setText(ls);
}
void MainWindow::debugLabel(QString dbs){
    m_debugLabel->setText(dbs);
}


