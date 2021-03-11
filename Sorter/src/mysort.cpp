#include "mysort.h"
#include <iostream>
#include <QTimer>
#include <QEventLoop>
#include <QApplication>
#include <ctime>
#include <Random>

#define mydelay 100
MySort::MySort() : QObject()
{
    m_parent = nullptr;
    m_array = nullptr;
    m_size = 0;
}

MySort::MySort(QObject *parent, DataToSort* sortArray, int size, tAlgorithms sortType) : QObject(parent) {
    std::cerr << "init my sort "<< size<< " type:" << sortType<< std::endl;
    m_parent = parent;
    m_array = sortArray;
    m_size = size;
    QObject::connect(this, SIGNAL(redrawRequest(tArrayStates)), m_parent, SLOT(toRedrawEvent(tArrayStates)));
    QObject::connect(this, SIGNAL(changeAlgoLabel(QString)), m_parent->parent(), SLOT(algoLabel(QString)));
    QObject::connect(this, SIGNAL(changeDebugLabel(QString)), m_parent->parent(), SLOT(debugLabel(QString)));
    srand(int(time(NULL)));

}

void MySort::sleepLoop(long delay){

       QEventLoop loop;
       QTimer timer;
       timer.setInterval(delay);
       timer.setSingleShot(true);
       QObject::connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
       timer.start();
       loop.exec();
}

void MySort::initSort(DataToSort* sortArray, int arraySize, tAlgorithms sortType){
    m_array = sortArray;
    m_size = arraySize;
    m_type = sortType;
    for(int i=0; i<m_size ;++i){
       m_array[i].point = rand() % YMAX;
       m_array[i].state = NEUTRAL;
       emit redrawRequest(NEUTRAL);
       sleepLoop(50);

    }
    sleepLoop(500);
}

void MySort::setState(tActions state){
    m_state = state;
    std::cerr << "state received: " << state << std::endl;
    if (state != STOP)
        doSort();
}
void MySort::doSort() {
    emit changeDebugLabel(QString("Sorting"));
    switch(m_type) {
        case ALL:
        case BUBBLE:
            emit changeAlgoLabel(QString("Bubble"));
            bubbleSort(m_array,  m_size);
            MessageBeep(MB_ICONWARNING);
            emit changeDebugLabel(QString("Sorting finished"));
            if(m_type != ALL)
                break;
            else {
                m_type = QUICK;
                emit changeAlgoLabel(QString("Selection"));
                initSort(m_array, m_size, m_type);
            }
        case QUICK: quickSort(m_array, m_size);
            MessageBeep(MB_ICONWARNING);
            //QApplication::beep();
            emit changeDebugLabel(QString("Sorting finished"));
            if(m_type != ALL)
                break;
            else {
                m_type = SELECTION;
                emit changeAlgoLabel(QString("Selection"));
                initSort(m_array, m_size, m_type);
            }
        case SELECTION: selectionSort(m_array, m_size);
            MessageBeep(MB_ICONWARNING);
            emit changeDebugLabel(QString("Sorting finished"));
            if(m_type != ALL)
                break;
            else {
                m_type = COCTAIL;
                emit changeAlgoLabel(QString("Coctail"));
                initSort(m_array, m_size, m_type);
            }
        case COCTAIL: coctailSort(m_array, m_size); if(m_type != ALL) break;
            MessageBeep(MB_ICONWARNING);
            emit changeDebugLabel(QString("Sorting finished"));
        //case SHELL: shellSort(m_array, m_size); if(m_type != ALL) break;
        default: ;
    }
    emit redrawRequest(SORTED);
}

void  MySort::bubbleSort(DataToSort* data, int const len){
    bool swapped;
    do
    {
        if(m_state == STOP){
            MessageBeep(MB_ICONSTOP);
            emit changeDebugLabel(QString("Sorting interrupted"));
            break;
        }
        swapped = false;
        for (int i = 1; i < len; i++) {
            marker(data, i);
            if (data[i - 1].point > data[i].point)
            {
                swapper(data, i - 1, i);
                swapped = true;
            }
        }
    } while (swapped != false);
}

void  MySort::quickSort(DataToSort* data, int const size){

#define MAXSTACK 2048
  int i, j;   			//  partition inices
  int lb, ub;  		// lower and upper borders
  int lbstack[MAXSTACK], ubstack[MAXSTACK]; // using stack instead recursion

  int stackpos = 1;
  int ppos;            // index of array pivot
  DataToSort pivot;

  lbstack[1] = 0;
  ubstack[1] = size-1;

  do {
      if(m_state == STOP){
          MessageBeep(MB_ICONSTOP);
          emit changeDebugLabel(QString("Sorting interrupted"));
          break;
      }
    lb = lbstack[ stackpos ];
    ub = ubstack[ stackpos ];
    stackpos--;

    do {
      // Step 1. partition by pivot

      ppos = ( lb + ub ) >> 1;
      i = lb; j = ub; pivot = data[ppos];

      do {
        while ( data[i].point < pivot.point ) {
            marker(data, i);
            i++;
        }
        while ( pivot.point < data[j].point ) {
            marker(data, j);
            j--;
        }

        if ( i <= j ) {

          swapper(data, i, j);
          i++; j--;
        }
      } while ( i <= j );

      // Step 2, 3. Pushing bigger part into stack, moving lb and ub

      if ( i < ppos ) {
        if ( i < ub ) {
          stackpos++;
          lbstack[ stackpos ] = i;
          ubstack[ stackpos ] = ub;
        }
        ub = j;
      } else {
        if ( j > lb ) {
          stackpos++;
          lbstack[ stackpos ] = lb;
          ubstack[ stackpos ] = j;
        }
        lb = i;
      }

    } while ( lb < ub );
  } while ( stackpos != 0 );    // while there are sorting requests in stack
  redrawRequest(ACTIVE);
}

void  MySort::selectionSort(DataToSort* data, int const size){
    int i, j, k;
    DataToSort temp;

    for( i=0; i < size; i++) {
        if(m_state == STOP){
            MessageBeep(MB_ICONSTOP);
            emit changeDebugLabel(QString("Sorting interrupted"));
            break;
        }
      marker(data, i);
      k=i; temp=data[i];

      for( j=i+1; j < size; j++)
        if (  data[j].point < temp.point ) {
          k=j; temp=data[j];	        // k - least element index
        }
      swapper(data, k, i);
    }

}

void  MySort::coctailSort(DataToSort* data, int const size){
    int last = size-1, left = 1, right = size-1, j;

    do
    {
        if(m_state == STOP){
            MessageBeep(MB_ICONSTOP);
            emit changeDebugLabel(QString("Sorting interrupted"));
            break;
        }
        for(j = right; j >= left; j--)
        {
            marker(data,j);
            if(data[j-1].point > data[j].point)
            {
                swapper(data, j-1, j);
                last = j;
            }
        }

        left = last + 1;

        for(j = left; j <= right; j++)
        {
            marker(data,j);
            if(data[j-1].point > data[j].point)
            {
                swapper(data, j-1, j);
                last = j;
            }
        }

        right = last-1;

    } while(left < right);
}


// 3 methods for visualisation decoration and redraw triggering

void  MySort::selector(DataToSort* data, int ind1, int ind2) {
    m_array[ind1].state  = LEFT;
    m_array[ind2].state  = RIGHT;
    emit redrawRequest(NEUTRAL);
    sleepLoop(mydelay);
}

void  MySort::swapper(DataToSort* data, int left, int right){
    selector(data, left, right);
    if (left != right)
    {
        DataToSort temp = m_array[left];
        m_array[left] = m_array[right];
        m_array[right] = temp;
    }
    emit redrawRequest(SORTED);
    sleepLoop(2*mydelay);
    data[right].state  = NEUTRAL;
    data[left].state  = NEUTRAL;
    emit redrawRequest(SORTED);
}

void MySort::marker(DataToSort* data, int ind){
    data[ind].state  = ACTIVE;
    sleepLoop(mydelay/2);
    emit redrawRequest(ACTIVE);
    data[ind].state  = NEUTRAL;
    sleepLoop(mydelay/2);
    emit redrawRequest(NEUTRAL);
}
