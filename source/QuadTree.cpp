#include "..\include\QuadTree.hpp"

using std::vector, std::string;

Node::Node(Point p, int d, string c) : point(p), data(d), city(c) {}

QuadTree::QuadTree(Point _topLeft, Point _botRight, bool first) {
    this->topLeftPoint = _topLeft;
    this->originalBotRightPoint = _botRight;
    this->botRightPoint = _botRight;
    type = WHITE;
    count = 0;
    dataSum = 0;
    
    // En caso de ser el primer nodo, se debe modificar el botRightPoint para que sea potencia de 2
    if (first) {
        int n = log2(abs(this->topLeftPoint.getX() - originalBotRightPoint.getX())+1);
        int m = log2(abs(this->topLeftPoint.getY() - originalBotRightPoint.getY())+1);

        // Se verifica si el punto es potencia de 2
        if (ceil(log2(abs(this->topLeftPoint.getX() - originalBotRightPoint.getX())+1)) !=
            floor(log2(abs(this->topLeftPoint.getX() - originalBotRightPoint.getX())+1))) {

            // Se busca la potencia de 2 mas cercana
            while (pow(2, n) < (abs(this->topLeftPoint.getX() - originalBotRightPoint.getX()))+1) {
                n++;
            }
        }

        // Se verifica si el punto es potencia de 2
        if (ceil(log2(abs(this->topLeftPoint.getY() - originalBotRightPoint.getY()))+1) !=
            floor(log2(abs(this->topLeftPoint.getY() - originalBotRightPoint.getY()))+1)) {
            
            // Se busca la potencia de 2 mas cercana
            while (pow(2, m) < (abs(this->topLeftPoint.getY() - originalBotRightPoint.getY()))+1) {
                m++;
            }
        }

        // Se modifica el botRightPoint
        int max = (n >= m) ? n : m;
        botRightPoint.setPoint(pow(2, max) + topLeftPoint.getX()-1, pow(2, max) + topLeftPoint.getY()-1);
    } else {
        this->botRightPoint = _botRight;
    }
}

QuadTree::~QuadTree() {
    if (topLeftQT != nullptr) delete topLeftQT;
    if (topRightQT != nullptr) delete topRightQT;
    if (botLeftQT != nullptr) delete botLeftQT;
    if (botRightQT != nullptr) delete botRightQT;
    
    if (!nodes.empty()){
        for (size_t i = 0; i < nodes.size(); i++){
            delete nodes[i];
        }
    }
}

int QuadTree::totalPoints() { return count; }

int QuadTree::totalNodes() {
    int count = 1;

    if (topLeftQT != nullptr) count += topLeftQT->totalNodes();
    if (topRightQT != nullptr) count += topRightQT->totalNodes();
    if (botLeftQT != nullptr) count += botLeftQT->totalNodes();
    if (botRightQT != nullptr) count += botRightQT->totalNodes();

    return count; 
}

void QuadTree::insert(Point p, int data, string city) {
    if (inBounds(p)) {
        // Se verifica si el punto esta dentro del cuadrante
        if (type == WHITE) type = BLACK;
        dataSum += data;
        count++;

        // Se crea el nodo en caso de que no exista
        if ((topLeftPoint.getX() == botRightPoint.getX())) {
            bool sameCity = false;
            for (size_t i = 0; i < nodes.size(); i++) {
                if (iequals(nodes[i]->city, city)) {
                    dataSumChange(p, nodes[i]->data, data);
                    nodes[i]->data = data;
                    sameCity = true;
                    break;
                }
            }
            if (!sameCity) {
                Node* node = new Node(p, data, city);
                nodes.push_back(node);
            }
        } else {
            // Se crean los cuadrantes en el caso de que no existan
            if (topLeftQT == nullptr) {
                topLeftQT  = new QuadTree(topLeftPoint, 
                                        Point((topLeftPoint.getX() + botRightPoint.getX()) / 2, (topLeftPoint.getY() + botRightPoint.getY()) / 2), false);
                topRightQT = new QuadTree(Point(((topLeftPoint.getX() + botRightPoint.getX()) / 2) + 1, topLeftPoint.getY()), 
                                        Point(botRightPoint.getX(), (topLeftPoint.getY() + botRightPoint.getY()) / 2), false);
                botLeftQT  = new QuadTree(Point(topLeftPoint.getX(), ((topLeftPoint.getY() + botRightPoint.getY()) / 2) + 1), 
                                        Point((topLeftPoint.getX() + botRightPoint.getX()) / 2, botRightPoint.getY()), false);
                botRightQT = new QuadTree(Point(((topLeftPoint.getX() + botRightPoint.getX()) / 2) + 1, ((topLeftPoint.getY() + botRightPoint.getY()) / 2) + 1), 
                                        botRightPoint, false);
            }

            // Se inserta en el cuadrante correspondiente
            if ((topLeftPoint.getX() + botRightPoint.getX()) / 2 >= p.getX()) {
                if ((topLeftPoint.getY() + botRightPoint.getY()) / 2 >= p.getY()) {
                    topLeftQT->insert(p, data, city);
                } else {
                    botLeftQT->insert(p, data, city);
                }
            } else {
                if ((topLeftPoint.getY() + botRightPoint.getY()) / 2 >= p.getY()) {
                    topRightQT->insert(p, data, city);
                } else {
                    botRightQT->insert(p, data, city);
                }
            }
        }
    }
}

bool QuadTree::iequals(const string& a, const string& b) {
    return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) { return tolower(a) == tolower(b); });
}

void QuadTree::dataSumChange(Point p, int res, int sum) {
    dataSum += (sum-res);
    count--;
    if (nodes.empty()) {
        if ((topLeftPoint.getX() + botRightPoint.getX()) /2 >= p.getX()) {
            if ((topLeftPoint.getY() + botRightPoint.getY()) / 2 >= p.getY()) {
                topLeftQT->dataSumChange(p, res, sum);
            } else {
                botLeftQT->dataSumChange(p, res, sum);
            }
        } else {
            if ((topLeftPoint.getY() + botRightPoint.getY()) / 2 >= p.getY()) {
                topRightQT->dataSumChange(p, res, sum);
            } else {
                botRightQT->dataSumChange(p, res, sum);
            }
        }
    }
}

vector<Node*> QuadTree::search(Point p) {
    if (topLeftPoint.getX() == botRightPoint.getX()) {
        return (nodes);
    }
    if ((topLeftPoint.getX() + botRightPoint.getX()) / 2 >= p.getX()) {
        if ((topLeftPoint.getY() + botRightPoint.getY()) / 2 >= p.getY()) {
            if (topLeftQT != nullptr) {
                return (topLeftQT->search(p));
            } else {
                vector<Node*> nulo;
                return (nulo);
            }
        } else {
            if (botLeftQT != nullptr) {
                return (botLeftQT->search(p));
            } else {
                vector<Node*> nulo;
                return (nulo);
            }
        }
    } else {
        if ((topLeftPoint.getY() + botRightPoint.getY()) / 2 >= p.getY()) {
            if (topRightQT != nullptr) {
                return (topRightQT->search(p));
            } else {
                vector<Node*> nulo;
                return (nulo);
            }
        } else {
            if (botRightQT != nullptr) {
                return (botRightQT->search(p));
            } else {
                vector<Node*> nulo;
                return (nulo);
            }
        }
    }
}

int QuadTree::countRegion(Point p, int d) {
    if (inBounds(p)) {
        Point regionTopL = Point(p.getX() - d, p.getY() - d);
        if (!inBounds(regionTopL)) {
            if (regionTopL.getX() < this->topLeftPoint.getX()) {
                regionTopL.setPoint(topLeftPoint.getX(), regionTopL.getY());
            }
            if (regionTopL.getY() < this->topLeftPoint.getY()) {
                regionTopL.setPoint(regionTopL.getX(), topLeftPoint.getY());
            }
        }
        Point regionBotR = Point(p.getX() + d, p.getY() + d);
        if (!inBounds(regionBotR)) {
            if (regionBotR.getX() > this->botRightPoint.getX()) {
                regionBotR.setPoint(botRightPoint.getX(), regionBotR.getY());
            }
            if (regionBotR.getY() > this->botRightPoint.getY()) {
                regionBotR.setPoint(regionBotR.getX(), botRightPoint.getY());
            }
        }
        return countRegionAux(regionTopL, regionBotR, this);
    }
    return 0;
}

int QuadTree::countRegionAux(Point pTL, Point pBR, QuadTree* QT) {
    int sum = 0;
    if (QT->topLeftPoint.getX() >= pTL.getX() && QT->topLeftPoint.getY() >= pTL.getY() &&
        QT->botRightPoint.getX() <= pBR.getX() && QT->botRightPoint.getY() <= pBR.getY()) {
        return (QT->count);
    }
    if (QT->botRightPoint.getX() < pTL.getX() || QT->botRightPoint.getY() < pTL.getY() ||
       QT->topLeftPoint.getX() > pBR.getX() || QT->topLeftPoint.getY() > pBR.getY()){
        return sum;
    }
    if (QT->topLeftQT != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->topLeftQT);
    }
    if (QT->topRightQT != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->topRightQT);
    }
    if (QT->botLeftQT != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->botLeftQT);
    }
    if (QT->botRightQT != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->botRightQT);
    }
    return (sum);
}

int QuadTree::aggregateRegion(Point p, int d) {
    if (inBounds(p)) {
        Point pTL = Point(p.getX() - d, p.getY() - d);
        if (!inBounds(pTL)) {
            if (pTL.getX() < this->topLeftPoint.getX()) {
                pTL.setPoint(topLeftPoint.getX(), pTL.getY());
            }
            if (pTL.getY() < this->topLeftPoint.getY()) {
                pTL.setPoint(pTL.getX(), topLeftPoint.getY());
            }
        }
        Point pBR = Point(p.getX() + d, p.getY() + d);
        if (!inBounds(pBR)) {
            if (pBR.getX() > this->botRightPoint.getX()) {
                pBR.setPoint(botRightPoint.getX(), pBR.getY());
            }
            if (pBR.getY() > this->botRightPoint.getY()) {
                pBR.setPoint(pBR.getX(), botRightPoint.getY());
            }
        }
        return aggregateRegionAux(pTL, pBR, this);
    }
    return 0;
}

int QuadTree::aggregateRegionAux(Point pTL, Point pBR, QuadTree* QT) {
    int sum = 0;
    if (QT->topLeftPoint.getX() >= pTL.getX() && QT->topLeftPoint.getY() >= pTL.getY() &&
        QT->botRightPoint.getX() <= pBR.getX() && QT->botRightPoint.getY() <= pBR.getY()) {
        return (QT->dataSum);
    }
    if (QT->botRightPoint.getX() < pTL.getX() || QT->botRightPoint.getY() < pTL.getY() ||
        QT->topLeftPoint.getX() > pBR.getX() || QT->topLeftPoint.getY() > pBR.getY()) {
        return sum;
    }
    if (QT->topLeftQT != nullptr) {
        sum += aggregateRegionAux(pTL, pBR, QT->topLeftQT);
    }
    if (QT->topRightQT != nullptr) {
        sum += aggregateRegionAux(pTL, pBR, QT->topRightQT);
    }
    if (QT->botLeftQT != nullptr) {
        sum += aggregateRegionAux(pTL, pBR, QT->botLeftQT);
    }
    if (QT->botRightQT != nullptr) {
        sum += aggregateRegionAux(pTL, pBR, QT->botRightQT);
    }
        
    return (sum);
}


RegionType QuadTree::getType() { return (type); }

bool QuadTree::inBounds(Point p) {
    return (p.getX() >= topLeftPoint.getX() && p.getX() <= originalBotRightPoint.getX() &&
            p.getY() >= topLeftPoint.getY() && p.getY() <= originalBotRightPoint.getY());
}

vector<Node*> QuadTree::list() {
    vector<Node*> v;

    if (!nodes.empty()){
        for (size_t i = 0; i < nodes.size(); i++){
            v.push_back(nodes[i]);
        }
    }
    if (topLeftQT != nullptr) {
        vector<Node*> topLeftQTList = topLeftQT->list();
        v.insert(v.end(), topLeftQTList.begin(), topLeftQTList.end());
    }
    if (topRightQT != nullptr) {
        vector<Node*> topRightQTList = topRightQT->list();
        v.insert(v.end(), topRightQTList.begin(), topRightQTList.end());
    }
    if (botLeftQT != nullptr) {
        vector<Node*> botLeftQTList = botLeftQT->list();
        v.insert(v.end(), botLeftQTList.begin(), botLeftQTList.end());
    }
    if (botRightQT != nullptr) {
        vector<Node*> botRightQTList = botRightQT->list();
        v.insert(v.end(), botRightQTList.begin(), botRightQTList.end());
    }
    return v;
}
