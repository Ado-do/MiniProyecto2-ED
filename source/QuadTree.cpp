#include "include/QuadTree.hpp"
#include "QuadTree.hpp"
#include <cmath>
#include <iostream>

QuadTree::QuadTree() { } 

QuadTree::QuadTree(Point TopLeft, Point BotRight){
    node = nullptr;
    this->TopLeft = TopLeft;
    ORBotRight = BotRight;
    type = WHITE;
    count = 0;
    datasum = 0;
    //Comentario para compañeros borrar: esto de abajo es para encontrarla potencia de 2 mas cercana para hacer una matriz de potencia de 2 esto facilita las particiones del arbol por lo menos segun yo mas explicacion pueden preguntarme
    int n = log2(abs(this->TopLeft.getX()-ORBotRight.getX()));
    int m = log2(abs(this->TopLeft.getY()-ORBotRight.getY()));
    if (ceil(log2(abs(this->TopLeft.getX()-ORBotRight.getX()))) != floor(log2(abs(this->TopLeft.getX()-ORBotRight.getX())))){
        while(pow(2,n)<abs(this->TopLeft.getX()-ORBotRight.getX())){
            n++;
        }
    }
    if (ceil(log2(abs(this->TopLeft.getY()-ORBotRight.getY()))) != floor(log2(abs(this->TopLeft.getY()-ORBotRight.getY())))){
        while(pow(2,m)<abs(this->TopLeft.getY()-ORBotRight.getY())){
            m++;
        }
    }
    if (n >= m){
        this->BotRight.setPoint(pow(2,n) + this->TopLeft.getX(), pow(2,n) + this->TopLeft.getY());
    }else{
        this->BotRight.setPoint(pow(2,m) + this->TopLeft.getX(), pow(2,m) + this->TopLeft.getY());
    }
}

QuadTree::QuadTree(Point TopLeft, Point BotRight, bool t){
    node = nullptr;
    this->TopLeft = TopLeft;
    this->BotRight = BotRight;
    type = WHITE;
    count = 0;
    datasum = 0;
}

QuadTree::~QuadTree() { }

int QuadTree::totalPoints() { return count; }

int QuadTree::totalNodesAux(QuadTree* QT) {
    int count = 1;
    if (TL != nullptr){count = count + totalNodesAux(this->TL);}
    if (TR != nullptr){count = count + totalNodesAux(this->TR);}
    if (BL != nullptr){count = count + totalNodesAux(this->BL);}
    if (BR != nullptr){count = count + totalNodesAux(this->BR);}
    return count;
}
int QuadTree::totalNodes() { return totalNodesAux(this); }

void QuadTree::insert(Point p, int data) {
    if (inBounds(p)){
        if (this->type == WHITE){
            this->type = BLACK;
            this->count++;
            this->datasum = this->datasum + data;
        }
        if (abs(TopLeft.getX()-BotRight.getX())==0 && abs(TopLeft.getY()-BotRight.getY())==0){
            node = new Node(p, data);
        }
        if (TopLeft.getX()+BotRight.getX()/2<=p.getX()){
            if (TopLeft.getY()+BotRight.getY()/2<=p.getY()){
                if (TL == nullptr){
                    TL = new QuadTree(TopLeft,Point(TopLeft.getX()+BotRight.getX()/2,TopLeft.getY()+BotRight.getY()/2), 1);
                }
                TL->insert(p,data);
            }else{                
                if (TR == nullptr){
                    TR = new QuadTree(Point((TopLeft.getX()+BotRight.getX()/2)+1,TopLeft.getY()),Point(BotRight.getX(),TopLeft.getY()+BotRight.getY()/2), 1);
                }
                TR->insert(p,data);
            }
        }else{
            if (TopLeft.getY()+BotRight.getY()/2<=p.getY()){
                if (BL == nullptr){
                    BL = new QuadTree(Point(TopLeft.getX(),(TopLeft.getY()+BotRight.getY()/2)+1),Point(TopLeft.getX()+BotRight.getX()/2,BotRight.getY()), 1);
                }
                BL->insert(p,data);
            }else{                
                if (BR == nullptr){
                    BL = new QuadTree(Point((TopLeft.getX()+BotRight.getX()/2)+1,(TopLeft.getY()+BotRight.getY()/2)+1),BotRight, 1);
                }
                BR->insert(p,data);
            }
        }
        
    }
}
int QuadTree::countRegion(Point p, int d) {
    if(inBounds(p)){
        Point pTL = Point(p.getX()-d,p.getY()-d);
        if (!inBounds(pTL)){
            if (pTL.getX()<this->TopLeft.getX()){
                pTL.setPoint(TopLeft.getX(),pTL.getY());
            }
            if (pTL.getY()<this->TopLeft.getY()){
                pTL.setPoint(pTL.getX(),TopLeft.getY());
            }
        }
        Point pBR = Point(p.getX()+d,p.getY()+d);  
        if (!inBounds(pBR)){
            if (pBR.getX()>this->BotRight.getX()){
                pBR.setPoint(BotRight.getX(),pBR.getY());
            }
            if (pBR.getY()>this->BotRight.getY()){
                pBR.setPoint(pBR.getX(),BotRight.getY());
            }
        }
        return cRAux(pTL,pBR, this); 
    }  
    return 0; 
}

int QuadTree::cRAux(Point pTL, Point pBR, QuadTree* QT){
    int sum = 0;
    if (QT->TopLeft.getX()>=pTL.getX() && QT->TopLeft.getY()>=pTL.getY() && QT->BotRight.getX()<=pBR.getX() && QT->BotRight.getY()<=pBR.getY()){
        return(QT->count);
    }
    if (QT->TL != nullptr){sum = sum + cRAux(pTL,pBR, QT->TL);}
    if (QT->TR != nullptr){sum = sum + cRAux(pTL,pBR, QT->TR);}
    if (QT->BL != nullptr){sum = sum + cRAux(pTL,pBR, QT->BL);}
    if (QT->BR != nullptr){sum = sum + cRAux(pTL,pBR, QT->BR);}
    return(sum);
}

int QuadTree::AggregateRegion(Point p, int d) {
    if(inBounds(p)){
        Point pTL = Point(p.getX()-d,p.getY()-d);
        if (!inBounds(pTL)){
            if (pTL.getX()<this->TopLeft.getX()){
                pTL.setPoint(TopLeft.getX(),pTL.getY());
            }
            if (pTL.getY()<this->TopLeft.getY()){
                pTL.setPoint(pTL.getX(),TopLeft.getY());
            }
        }
        Point pBR = Point(p.getX()+d,p.getY()+d);  
        if (!inBounds(pBR)){
            if (pBR.getX()>this->BotRight.getX()){
                pBR.setPoint(BotRight.getX(),pBR.getY());
            }
            if (pBR.getY()>this->BotRight.getY()){
                pBR.setPoint(pBR.getX(),BotRight.getY());
            }
        }
        return ARAux(pTL,pBR, this); 
    }  
    return 0;
}

int QuadTree::ARAux(Point pTL, Point pBR, QuadTree* QT){
    int sum = 0;
    if (QT->TopLeft.getX()>=pTL.getX() && QT->TopLeft.getY()>=pTL.getY() && QT->BotRight.getX()<=pBR.getX() && QT->BotRight.getY()<=pBR.getY()){
        return(QT->datasum);
    }
    if (QT->TL != nullptr){sum = sum + cRAux(pTL,pBR, QT->TL);}
    if (QT->TR != nullptr){sum = sum + cRAux(pTL,pBR, QT->TR);}
    if (QT->BL != nullptr){sum = sum + cRAux(pTL,pBR, QT->BL);}
    if (QT->BR != nullptr){sum = sum + cRAux(pTL,pBR, QT->BR);}
    return(sum);
}

void QuadTree::clear() { }

// void QuadTree::insert(Entity* entity) { }

// void QuadTree::retrieve(std::vector<Entity*>& entities, Entity* entity) { }

// void QuadTree::list() { }

RegionType QuadTree::gettype(){
    return(type);
}

bool QuadTree::inBounds(Point p){
    return(p.getX()>=TopLeft.getX()&&p.getX()<=ORBotRight.getX()&&p.getY()<=TopLeft.getY()&&p.getY()>=ORBotRight.getY());
}

Point QuadTree::PointTL(){
    return(TopLeft);
}

Point QuadTree::PointBR(){
    return(BotRight);
}