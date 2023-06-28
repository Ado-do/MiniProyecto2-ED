#include "QuadTree.hpp"
#include "CityNode.hpp"

using std::vector, std::string;

QuadTree::QuadTree(Point _topLeft, Point _botRight, bool first) {
    this->topL_point = _topLeft;
    this->botR_point = _botRight;
    this->_botR_point = _botRight;
    
    type = WHITE;
    count = 0;
    dataSum = 0;
    
    // En caso de ser el primer nodo, se debe modificar el botR_point para que sea potencia de 2
    if (first) {
        int n = log2(abs(this->topL_point.getX() - _botR_point.getX())+1);
        int m = log2(abs(this->topL_point.getY() - _botR_point.getY())+1);

        // Se verifica si el punto es potencia de 2
        if (ceil(log2(abs(this->topL_point.getX() - _botR_point.getX())+1)) !=
            floor(log2(abs(this->topL_point.getX() - _botR_point.getX())+1))) {

            // Se busca la potencia de 2 mas cercana
            while (pow(2, n) < (abs(this->topL_point.getX() - _botR_point.getX()))+1) {
                n++;
            }
        }

        // Se verifica si el punto es potencia de 2
        if (ceil(log2(abs(this->topL_point.getY() - _botR_point.getY()))+1) !=
            floor(log2(abs(this->topL_point.getY() - _botR_point.getY()))+1)) {
            
            // Se busca la potencia de 2 mas cercana
            while (pow(2, m) < (abs(this->topL_point.getY() - _botR_point.getY()))+1) {
                m++;
            }
        }

        // Se modifica el botR_point
        int max = (n >= m) ? n : m;
        botR_point.setPoint(int(pow(2, max)) + topL_point.getX()-1, int(pow(2, max)) + topL_point.getY()-1);
    } else {
        this->botR_point = _botRight;
    }
}

QuadTree::~QuadTree() {
    if (topL_QT != nullptr) delete topL_QT;
    if (topR_QT != nullptr) delete topR_QT;
    if (botL_QT != nullptr) delete botL_QT;
    if (botR_QT != nullptr) delete botR_QT;
    
    if (!nodes.empty()){
        for (size_t i = 0; i < nodes.size(); i++){
            delete nodes[i];
        }
    }
}

int QuadTree::totalPoints() { return count; }

int QuadTree::totalNodes() {
    int count = 1;

    if (topL_QT != nullptr) count += topL_QT->totalNodes();
    if (topR_QT != nullptr) count += topR_QT->totalNodes();
    if (botL_QT != nullptr) count += botL_QT->totalNodes();
    if (botR_QT != nullptr) count += botR_QT->totalNodes();

    return count; 
}

void QuadTree::insert(Point p, int data, string city) {
    if (inBounds(p)) {
        // Se verifica si el punto esta dentro del cuadrante
        if (type == WHITE) type = BLACK;
        dataSum += data;
        count++;

        // Se crea el nodo en caso de que no exista
        if ((topL_point.getX() == botR_point.getX())) {
            bool sameCity = false;
            for (size_t i = 0; i < nodes.size(); i++) {
                if (nodes[i]->city == city) { // Comparacion de nombres
                    dataSumChange(p, nodes[i]->data, data);
                    nodes[i]->data = data;
                    sameCity = true;
                    break;
                }
            }
            if (!sameCity) {
                CityNode* node = new CityNode(p, data, city);
                nodes.push_back(node);
            }
        } else {
            // Se crean los cuadrantes en el caso de que no existan
            if (topL_QT == nullptr) {
                int x = int((floor((topL_point.getX() + botR_point.getX()) / 2.0f)));
                int y = int((floor((topL_point.getY() + botR_point.getY()) / 2.0f)));
                topL_QT  = new QuadTree(topL_point, Point(x, y), false);
                topR_QT = new QuadTree(Point(x + 1, topL_point.getY()), Point(botR_point.getX(), y), false);
                botL_QT  = new QuadTree(Point(topL_point.getX(), y + 1), Point(x, botR_point.getY()), false);
                botR_QT = new QuadTree(Point(x + 1, y + 1), botR_point, false);
            }

            // Se inserta en el cuadrante correspondiente
            int x = int((floor((topL_point.getX() + botR_point.getX()) / 2.0f)));
            int y = int((floor((topL_point.getY() + botR_point.getY()) / 2.0f)));
            if (x >= p.getX()) {
                if (y >= p.getY()) {
                    topL_QT->insert(p, data, city);
                } else {
                    botL_QT->insert(p, data, city);
                }
            } else {
                if (y >= p.getY()) {
                    topR_QT->insert(p, data, city);
                } else {
                    botR_QT->insert(p, data, city);
                }
            }
        }
    }
}

void QuadTree::dataSumChange(Point p, int res, int sum) {
    int x = int((floor((topL_point.getX() + botR_point.getX()) / 2.0f)));
    int y = int((floor((topL_point.getY() + botR_point.getY()) / 2.0f)));
    dataSum += (sum-res);
    count--;
    if (nodes.empty()) {
        if (x >= p.getX()) {
            if (y >= p.getY()) {
                topL_QT->dataSumChange(p, res, sum);
            } else {
                botL_QT->dataSumChange(p, res, sum);
            }
        } else {
            if (y >= p.getY()) {
                topR_QT->dataSumChange(p, res, sum);
            } else {
                botR_QT->dataSumChange(p, res, sum);
            }
        }
    }
}

vector<CityNode*> QuadTree::search(Point p) {
    int x = int((floor((topL_point.getX() + botR_point.getX()) / 2.0f)));
    int y = int((floor((topL_point.getY() + botR_point.getY()) / 2.0f)));
    if (topL_point.getX() == botR_point.getX()) {
        return (nodes);
    }
    if (x >= p.getX()) {
        if (y >= p.getY()) {
            if (topL_QT != nullptr) {
                return (topL_QT->search(p));
            } else {
                vector<CityNode*> nulo;
                return (nulo);
            }
        } else {
            if (botL_QT != nullptr) {
                return (botL_QT->search(p));
            } else {
                vector<CityNode*> nulo;
                return (nulo);
            }
        }
    } else {
        if (y >= p.getY()) {
            if (topR_QT != nullptr) {
                return (topR_QT->search(p));
            } else {
                vector<CityNode*> nulo;
                return (nulo);
            }
        } else {
            if (botR_QT != nullptr) {
                return (botR_QT->search(p));
            } else {
                vector<CityNode*> nulo;
                return (nulo);
            }
        }
    }
}

int QuadTree::countRegion(Point p, int d) {
    if (inBounds(p)) {
        Point regionTopL = Point(p.getX() - d, p.getY() - d);
        if (!inBounds(regionTopL)) {
            if (regionTopL.getX() < this->topL_point.getX()) {
                regionTopL.setPoint(topL_point.getX(), regionTopL.getY());
            }
            if (regionTopL.getY() < this->topL_point.getY()) {
                regionTopL.setPoint(regionTopL.getX(), topL_point.getY());
            }
        }
        Point regionBotR = Point(p.getX() + d, p.getY() + d);
        if (!inBounds(regionBotR)) {
            if (regionBotR.getX() > this->botR_point.getX()) {
                regionBotR.setPoint(botR_point.getX(), regionBotR.getY());
            }
            if (regionBotR.getY() > this->botR_point.getY()) {
                regionBotR.setPoint(regionBotR.getX(), botR_point.getY());
            }
        }
        return countRegionAux(regionTopL, regionBotR, this);
    }
    return 0;
}

int QuadTree::countRegionAux(Point pTL, Point pBR, QuadTree* QT) {
    int sum = 0;
    if (QT->topL_point.getX() >= pTL.getX() && QT->topL_point.getY() >= pTL.getY() &&
        QT->botR_point.getX() <= pBR.getX() && QT->botR_point.getY() <= pBR.getY()) {
        return (QT->count);
    }
    if (QT->botR_point.getX() < pTL.getX() || QT->botR_point.getY() < pTL.getY() ||
       QT->topL_point.getX() > pBR.getX() || QT->topL_point.getY() > pBR.getY()){
        return sum;
    }
    if (QT->topL_QT != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->topL_QT);
    }
    if (QT->topR_QT != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->topR_QT);
    }
    if (QT->botL_QT != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->botL_QT);
    }
    if (QT->botR_QT != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->botR_QT);
    }
    return (sum);
}

int QuadTree::aggregateRegion(Point p, int d) {
    if (inBounds(p)) {
        Point pTL = Point(p.getX() - d, p.getY() - d);
        if (!inBounds(pTL)) {
            if (pTL.getX() < this->topL_point.getX()) {
                pTL.setPoint(topL_point.getX(), pTL.getY());
            }
            if (pTL.getY() < this->topL_point.getY()) {
                pTL.setPoint(pTL.getX(), topL_point.getY());
            }
        }
        Point pBR = Point(p.getX() + d, p.getY() + d);
        if (!inBounds(pBR)) {
            if (pBR.getX() > this->botR_point.getX()) {
                pBR.setPoint(botR_point.getX(), pBR.getY());
            }
            if (pBR.getY() > this->botR_point.getY()) {
                pBR.setPoint(pBR.getX(), botR_point.getY());
            }
        }
        return aggregateRegionAux(pTL, pBR, this);
    }
    return 0;
}

int QuadTree::aggregateRegionAux(Point pTL, Point pBR, QuadTree* QT) {
    int sum = 0;
    if (QT->topL_point.getX() >= pTL.getX() && QT->topL_point.getY() >= pTL.getY() &&
        QT->botR_point.getX() <= pBR.getX() && QT->botR_point.getY() <= pBR.getY()) {
        return (QT->dataSum);
    }
    if (QT->botR_point.getX() < pTL.getX() || QT->botR_point.getY() < pTL.getY() ||
        QT->topL_point.getX() > pBR.getX() || QT->topL_point.getY() > pBR.getY()) {
        return sum;
    }
    if (QT->topL_QT != nullptr) {
        sum += aggregateRegionAux(pTL, pBR, QT->topL_QT);
    }
    if (QT->topR_QT != nullptr) {
        sum += aggregateRegionAux(pTL, pBR, QT->topR_QT);
    }
    if (QT->botL_QT != nullptr) {
        sum += aggregateRegionAux(pTL, pBR, QT->botL_QT);
    }
    if (QT->botR_QT != nullptr) {
        sum += aggregateRegionAux(pTL, pBR, QT->botR_QT);
    }
        
    return (sum);
}


RegionType QuadTree::getType() { return (type); }

bool QuadTree::inBounds(Point p) {
    return (p.getX() >= topL_point.getX() && p.getX() <= _botR_point.getX() &&
            p.getY() >= topL_point.getY() && p.getY() <= _botR_point.getY());
}

vector<CityNode*> QuadTree::list() {
    vector<CityNode*> v;

    if (!nodes.empty()){
        for (size_t i = 0; i < nodes.size(); i++){
            v.push_back(nodes[i]);
        }
    }
    if (topL_QT != nullptr) {
        vector<CityNode*> topL_list = topL_QT->list();
        v.insert(v.end(), topL_list.begin(), topL_list.end());
    }
    if (topR_QT != nullptr) {
        vector<CityNode*> topR_list = topR_QT->list();
        v.insert(v.end(), topR_list.begin(), topR_list.end());
    }
    if (botL_QT != nullptr) {
        vector<CityNode*> botL_list = botL_QT->list();
        v.insert(v.end(), botL_list.begin(), botL_list.end());
    }
    if (botR_QT != nullptr) {
        vector<CityNode*> botR_list = botR_QT->list();
        v.insert(v.end(), botR_list.begin(), botR_list.end());
    }
    return v;
}
