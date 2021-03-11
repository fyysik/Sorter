#ifndef MYSORT_H
#define MYSORT_H

#include <QObject>
#include <QWidget>
#include "sortingwidget.h"
class SortingWidget;
class MySort;

class MySort: public QObject
{
    Q_OBJECT
    signals:
        void redrawRequest(tArrayStates);
        void changeAlgoLabel(QString);
        void changeDebugLabel(QString);
    public:
        MySort();
        MySort(QObject *, DataToSort* , int, tAlgorithms);
        DataToSort *m_array = nullptr;
        int m_size =0;
        tAlgorithms m_type = BUBBLE;
        tActions m_state = STOP;
        void initSort(DataToSort* sortArray, int size, tAlgorithms sortType);
        void doSort();
        void sleepLoop(long delay);
    public slots:
        void setState(tActions state);
    protected:
        QObject *m_parent = nullptr;
        void bubbleSort(DataToSort* data, int const len);
        void quickSort(DataToSort* data, int const len);
        void selectionSort(DataToSort* data, int const len);
        void shellSort(DataToSort* data, int const len);
        void coctailSort(DataToSort* data, int const len);
        void swapper(DataToSort* data, int ind1, int ind2);
        void selector(DataToSort* data, int ind1, int ind2);
        void marker(DataToSort* data, int ind);
    private:
        int increment(int inc[], int size);
};

#endif // MYSORT_H
