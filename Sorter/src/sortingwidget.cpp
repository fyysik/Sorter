#include "sortingwidget.h"
#include "mysort.h"
#include <cmath>
#include <QPainter>
#include <QPaintEvent>
#include <QLabel>
#include <random>
#include <algorithm>
#include <iostream>


/**
  Constructs a drawing widget.
  \param[in] parent parent widget of the drawing widget.
*/
//type * const variable
SortingWidget::SortingWidget(MainWindow *parent)
    : QWidget(parent), m_mainWindow(parent)
{
    m_actionString  = "Stop";
    m_action= m_actions[m_actionString];
    m_algoString = "Bubble";
    m_algo = m_algorithms[m_algoString];
    m_sorter = new MySort(this, ArrayToSort, ASIZE, BUBBLE);

    QObject::connect(this, SIGNAL(algoChanged(QString)), parent, SLOT(algoLabel(QString)));
    QObject::connect(this, SIGNAL(statusChanged(QString)), parent, SLOT(statusLabel(QString)));
    QObject::connect(this, SIGNAL(debugChanged(QString)), parent, SLOT(debugLabel(QString)));
    QObject::connect(this, SIGNAL(stateChanged(tActions)), m_sorter, SLOT(setState(tActions)));


}

/**
 @brief SortingWidget::selectAction
 Receives signal from main widget Run menu.
 Sets sorting algorithm, sends state signal to main.
 @param a action
 */
void SortingWidget::selectAlgo(QAction* a){
    QString(a->text()).toStdString() ;
    m_algoString = a->text();
    m_algo = m_algorithms[m_algoString];
    std::cerr <<"SortingWidget::selectAlgo:" << m_algoString.toStdString() << m_algo;
    emit algoChanged(QString(m_algoString));

}

/**
 @brief SortingWidget::selectAction
 Receives signal from main widget Run menu.
 Initializes sorting array, sends state signal to main and to sorter.
 @param a action
 */
void SortingWidget::selectAction(QAction* a){
    QString(a->text()).toStdString() ;
    QString action(a->text());
    m_action = m_actions[action];
    std::cerr <<"SortingWidget::selectAction:" << action.toStdString() << m_action;
    emit statusChanged(action);
    if(m_action != STOP) {
        m_sorter->initSort(ArrayToSort, ASIZE, tAlgorithms(m_algo));
        update();
        m_sorter->sleepLoop(200);
    }
    emit stateChanged(m_action);
}

/**
  Overridden method to handle all received paint events.
  \param[in] event the paint event that was received.
*/
void SortingWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), Qt::darkBlue);
    QPen myPen(Qt::darkGray, 2, Qt::SolidLine);
    painter.setBrush(Qt::darkGray);
    painter.setPen(myPen);
    float yscale = float(this->rect().height())/float(YMAX);
    float xstep = event->rect().width()/ASIZE;
    //std::cerr << "rect height:" << yscale << "xstep" << xstep;
    QColor myColor;
    for (int i=0; i<ASIZE;++i) {

        switch(ArrayToSort[i].state){
            case NEUTRAL: myColor = Qt::lightGray; break;
            case ACTIVE: myColor = Qt::white; break;
            case LEFT: myColor = Qt::red; break;
            case RIGHT: myColor = Qt::green; break;
            case SORTED: myColor = Qt::white;
            default: Qt::lightGray;
        }
        int invertedY = this->rect().height() - int(ArrayToSort[i].point*yscale);
        painter.fillRect(i*xstep, invertedY,
                         xstep-5, this->rect().height(), myColor);
    }

}
/**
 @brief SortingWidget::toRedrawEvent
 Slot, listening for redraw request
 @param st
 */
void SortingWidget::toRedrawEvent(tArrayStates st){
    //std::cerr<<"toRedrawEvent"<<std::endl;
    update();
}

SortingWidget::~SortingWidget(){
    if(m_sorter)
        delete m_sorter;
}

