#include "drawingwidget.h"
#include <cmath>
#include <QPainter>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QLabel>

/**
  Constructs a drawing widget.
  \param[in] parent parent widget of the drawing widget.
*/
//type * const variable
DrawingWidget::DrawingWidget(MainWindow *parent)
    : QWidget(parent), m_mainWindow(parent)
{
    //setGeometry(0, 0, 800, 600);
    m_eventPoint.x=0;
    m_eventPoint.y=0;  
    m_itemsToDraw = new PrimitiveSet{};

    QObject::connect(this, SIGNAL(verticesChanged(int)), parent, SLOT(verticesLabel(int)));
    QObject::connect(this, SIGNAL(linesChanged(int)), parent, SLOT(linesLabel(int)));
    QObject::connect(this, SIGNAL(debugChanged(QString)), parent, SLOT(debugLabel(QString)));
    QObject::connect(this, SIGNAL(toolsChanged(QString*)), parent, SLOT(toolsLabel(QString*)));
}

DrawingWidget::~DrawingWidget() {

    qDeleteAll(m_itemsToDraw->points.begin(),m_itemsToDraw->points.end());
    m_itemsToDraw->points.clear();

    //qDeleteAll(m_itemsToDraw->lines.begin(),m_itemsToDraw->lines.end());
    m_itemsToDraw->lines.clear();
    delete m_itemsToDraw;
 }



void DrawingWidget::action_addVertex(QMouseEvent *e) {
        if(e->button() != Qt::LeftButton)
            return;
        Point2 *newp = new Point2(e->x(),e->y());

        for(Point2 *p: m_itemsToDraw->points) {

            if(matchPoint(*p,*newp))
                return;
        }
        m_itemsToDraw->points.push_back(newp);
}


void DrawingWidget::action_delVertex(QMouseEvent *e) {
        if(e->button() != Qt::LeftButton)
            return;
        Point2 np(e->x(),e->y());
        for(Point2 *p: m_itemsToDraw->points) {
            if(matchPoint(*p, np)) {
                delPoint2(p);
                return;
            }
        }
}
bool DrawingWidget::delPoint2(Point2* p){
    if(m_itemsToDraw == nullptr)
        return false;
    int i = m_itemsToDraw->points.indexOf(p);
    if(i==-1){
        return false;
     }
    auto ld = m_itemsToDraw->lines.begin();
    while(ld != m_itemsToDraw->lines.end()){
        if((*ld).contains(p)){
            ld=m_itemsToDraw->lines.erase(ld);
        } else {
            ++ld;
        }
    }

    m_itemsToDraw->points.removeOne(p);
    delete p;
    m_mainWindow->m_verticesLabel->setText(QString::number(m_itemsToDraw->points.length()));
    return true;
}

void DrawingWidget::action_addSide(QMouseEvent *ev){
    if(ev->button() != Qt::LeftButton)
        return;
    Point2 mp(ev->x(),ev->y());
    std::cerr << "\naddLine eventPos:"
              << ev->x() <<","<< ev->y() << mp << "\n";
    QVector<Point2*>& pp = m_itemsToDraw->points;
    for(int i=0; i< (pp).length(); ++i) {
        if(matchPoint(*pp[i],mp)) {
            //match = &pp[i];
            if(!m_1point_selected){
                std::cerr <<"Addl 1 match found:"<< (pp)[i] << "and" << mp
                         <<"i="<< i <<"\n";
                m_1point = i;
                m_1point_selected = true;
                std::cerr <<"1 vertex selected"<< (pp)[m_1point] <<"\n";
                return;
            }
            else if(m_1point_selected){
                std::cerr <<"Addl 2 match found:"<< (pp)[i] << "and" << mp
                         <<"&="<< &pp[i] <<"\n";

                if (i != m_1point) {
                    m_2point = i;
                     std::cerr << "\n2 vertex p1:"<<(pp)[m_1point] <<","
                              << "p2:"<< (pp)[m_2point] <<"i1:"<< m_1point
                              << "i2:" << m_2point <<"\n";

                     std::cerr <<"2 vertex selected"<< (pp)[m_2point] <<"\n";
                     DynamicLine<Point2> l(pp[m_1point], pp[m_2point]);
                     addDynamicLine(l);
                     std::cerr << "\naddLine coords:"
                               << m_itemsToDraw->lines.last() << "\n";
                     m_1point_selected = false;
                     m_1point = -1;
                     m_2point = -1;
                     return;
                }
            }

            break;
        }
    }

}

bool DrawingWidget::matchPoint(Point2 p1, Point2 p2, unsigned d){
    return p1.distanceFrom(p2)<= d ;
}

bool DrawingWidget::addDynamicLine(DynamicLine<Point2>& l){
    if(m_itemsToDraw == nullptr) return false;

    if(!m_itemsToDraw->lines.contains(l)) {
        m_itemsToDraw->lines.push_back(l);
        return true;
    } else {
        std::cerr <<"Line exists:"<< l << "\n";
    }

    return true;
}

void DrawingWidget::action_delSide(QMouseEvent *e){
    if(e->button() != Qt::LeftButton)
        return;
    Point2 mp(e->x(), e->y());
    std::cerr <<"Deleting line at point:"<< mp << "\n";
    for(unsigned i = 0; i < m_itemsToDraw->lines.length(); ++i) {
        DynamicLine<Point2> ld = m_itemsToDraw->lines[i];
        std::cerr <<"Scanning line:"<< ld << "for:"<<mp<<"\n";

        if(matchLine(ld, mp)) {
            m_itemsToDraw->lines.removeOne(ld);
            std::cerr <<"Deleting line:"<< ld << "\n";
        }
    }
    //for все линии
        //for точки на линии (int)
}

bool DrawingWidget::matchLine(DynamicLine<Point2> l,  Point2 p) {
    Point2 lp1=*l.p1;
    Point2 lp2=*l.p2;
    double  distance;
    double xa = lp1.x;
    double xb = lp2.x;
    double x = double(p.x);
    double ya = double(lp1.y);
    double yb = double(lp2.y);
    double y = double(p.y);
    //double area of triangle == area of parallelogramm
    double len = l.length();
    double dx = 0, dy = 0;
    double mx = -1.0, my = -1.0;
    unsigned delta = 3;

    // testing area conditions
    if((std::min(xa, xb) - x > delta))
        return false;
    if((x - std::max(xa, xb)  > delta))
        return false;
    if((std::min(ya, yb) - y > delta))
        return false;
    if((y - std::max(ya, yb)  > delta))
        return false;
    if(xa==xb){
        std::cerr << "xa==xb)\n";
        dx = x - xa;
    }
    if(ya==yb){
        std::cerr << "ya==yb)\n";
        dy = y - ya;
    }
    //(x - xa) * (yb - ya) == (y - ya) * (xb - xa)
    if(xa != xb && ya != yb){
        std::cerr << "xa:"<<xa << ", ya:" <<ya<< ", x:"  <<x << ", y:" <<y<<"\n";
        my = ((x - xa) * (yb - ya)) /(xb - xa) + ya;
        mx = ((y - ya) * (xb - xa))/(yb - ya) + xa;
        dy = y - my;
        dx = x - mx;
    }

    //std::cerr << "dx:"<<dx<<", dy:" << dy << ", mx:"<< mx << ", my:"<<my << ", point:"<<p<< "\n";


    if( abs(dx)<delta && abs(dy)<delta)
        return true;

    return false;
}

void DrawingWidget::action_movVertex(QMouseEvent *e) {
    Point2 ep(e->x(),e->y());
    if(m_mouse_pressed && e->button() == Qt::LeftButton) {

        for(int i = 0; i < m_itemsToDraw->points.size(); ++i) {

            if(matchPoint(ep, *m_itemsToDraw->points[i])) {
                m_vtc_to_move = i;
                std::cerr << "fixing vertex:"<< m_itemsToDraw->points[i] << "\n";
                m_vertex_catched = true;
            }
        }
        return;
    }
    if(m_mouse_moving){

            std::cerr << "moved:" << m_eventPoint << "\n";
            if(m_vtc_to_move >= 0 && m_vertex_catched == true) {
                (*m_itemsToDraw->points[m_vtc_to_move]).moveTo(ep);
                std::cerr << "updating vertex:"<< "start" << " to:"<<ep << "\n";
                update();
            }
    }
}

void DrawingWidget::mousePressEvent(QMouseEvent *e) {
    m_eventPoint.x = e->x();
    m_eventPoint.y = e->y();
    m_mouse_pressed = true;
    m_mouse_moving = false;
    m_vertex_catched = false;
    if(m_itemsToDraw==nullptr)
        return;

    std::cerr << "pressed:" << m_Action << "\n";
    switch (m_Action) {
        case ADD_VERTEX: action_addVertex(e); break;
        case DEL_VERTEX: action_delVertex(e); break;
        case ADD_LINE: action_addSide(e); break;
        case DEL_LINE: action_delSide(e); break;
        case MOV_VERTEX: action_movVertex(e); break;
        case 0: break;
    }  
    update();
}


void DrawingWidget::mouseMoveEvent(QMouseEvent *e){
    if(m_Action != MOV_VERTEX)
        return;
    m_mouse_moving = true;
    m_mouse_pressed= false;
    action_movVertex(e);
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent *e){
    m_mouse_pressed = false;
    m_mouse_moving = false;
    m_vertex_catched = false;
    m_vtc_to_move = -1;
}

/**
  Overridden method to handle all received paint events.
  \param[in] event the paint event that was received.
*/
void DrawingWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), QColor(125, 235, 255));

    if(m_itemsToDraw == nullptr)
        return;
    QPen myPen(Qt::black, 2, Qt::SolidLine);
    painter.setPen(myPen);

    QVector<Point2*> pp = m_itemsToDraw->points;
    QColor c;

    for(int i = 0; i < pp.length(); ++i) {
        c = i==m_1point ? Qt::red : Qt::black;
        painter.setBrush(QBrush(c, Qt::SolidPattern));
        painter.drawEllipse(pp[i]->x-10,pp[i]->y-10, 20, 20);
    }
    emit verticesChanged(m_itemsToDraw->points.size());
    //DynamicLine<Point2> l: m_itemsToDraw->lines
    for(int i = 0; i < m_itemsToDraw->lines.size(); i++) {
        //std::cerr <<"Drawing line:"<< l << "\n";
        //paintLine(l, &painter);
        DynamicLine<Point2>& l = m_itemsToDraw->lines[i];
        painter.drawLine(l.p1->x,l.p1->y,l.p2->x,l.p2->y);
        std::cerr << "drawing line:" << l << "\n";
    }
    emit linesChanged(m_itemsToDraw->lines.size());
}

/*void DrawingWidget::paintLine(DynamicLine<Point2> l, QPainter *painter) {
    float a;
    bool vert = abs(l.p2.x - l.p1.x)==0;
    // abs(l.p2.y - l.p1.y) > abs(l.p2.x - l.p1.x);// //line is rather vertical
    a = vert ? (l.p2.x - l.p1.x)/(l.p2.y - l.p1.y) : (l.p2.y - l.p1.y)/(l.p2.x - l.p1.x); //turn drawing to 90 deg
    int x0 = vert ? l.p1.y : l.p1.x;
    int x1 = vert ? l.p2.y : l.p2.x;
    int y0 = vert ? l.p1.x : l.p1.y;
    std::cerr <<"line:"<< "x0:"<<x0 <<", "<<"x1:" <<x1<<", vert:"<<vert<< "\n";
    int step = vert? 1 : 1;
    float y = 0;
    painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    for (int x = x0; x <= x1; x += step) {

        y = a*(x - x0) + y0;
        painter->drawEllipse(vert?y:x, vert? x: y, 3, 3);
    }
}*/

bool DrawingWidget::movePoint(Point2 oldp, Point2 newp){
    if(m_itemsToDraw == nullptr) return false;
    oldp=newp;
    return true;
}

void DrawingWidget::selectAction(QAction* a){
    QString(a->text()).toStdString() ;
    m_Action = m_actions[a->text()];
    QString tool(a->text());
    emit toolsChanged(&tool);
    QString s("starting");
    emit debugChanged(s);
    std::cerr << "Action:"<< QString(a->text()).toStdString()  << "\n";
}



//DrawingAction DrawingWidget::getAction(){
//    return m_Action;
//}
