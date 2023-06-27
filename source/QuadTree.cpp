#include "include/QuadTree.hpp"

#include <cmath>
#include <iostream>

#include "QuadTree.hpp"

QuadTree::QuadTree() {}

QuadTree::QuadTree(Point TopLeft, Point BotRight) {
    node = nullptr;
    this->TopLeftPoint = TopLeft;
    OriginalBottomRightPoint = BotRight;
    type = WHITE;
    count = 0;
    datasum = 0;
    // Comentario para compañeros borrar: esto de abajo es para encontrarla
    // potencia de 2 mas cercana para hacer una matriz de potencia de 2 esto
    // facilita las particiones del arbol por lo menos segun yo mas explicacion
    // pueden preguntarme
    int n = log2(abs(this->TopLeftPoint.getX() - OriginalBottomRightPoint.getX()));
    int m = log2(abs(this->TopLeftPoint.getY() - OriginalBottomRightPoint.getY()));
    if (ceil(log2(abs(this->TopLeftPoint.getX() - OriginalBottomRightPoint.getX()))) !=
        floor(log2(abs(this->TopLeftPoint.getX() - OriginalBottomRightPoint.getX())))) {
        while (pow(2, n) < abs(this->TopLeftPoint.getX() - OriginalBottomRightPoint.getX())) {
            n++;
        }
    }
    if (ceil(log2(abs(this->TopLeftPoint.getY() - OriginalBottomRightPoint.getY()))) !=
        floor(log2(abs(this->TopLeftPoint.getY() - OriginalBottomRightPoint.getY())))) {
        while (pow(2, m) < abs(this->TopLeftPoint.getY() - OriginalBottomRightPoint.getY())) {
            m++;
        }
    }
    if (n >= m) {
        this->BottomRightPoint.setPoint(pow(2, n) + this->TopLeftPoint.getX(),
                                pow(2, n) + this->TopLeftPoint.getY());
    } else {
        this->BottomRightPoint.setPoint(pow(2, m) + this->TopLeftPoint.getX(),
                                pow(2, m) + this->TopLeftPoint.getY());
    }
}

QuadTree::QuadTree(Point TopLeft, Point BotRight, bool t) {
    node = nullptr;
    this->TopLeftPoint = TopLeft;
    this->BottomRightPoint = BotRight;
    type = WHITE;
    count = 0;
    datasum = 0;
}

QuadTree::~QuadTree() {}

int QuadTree::totalPoints() { return count; }

int QuadTree::totalNodesAux(QuadTree* QT) {
    int count = 1;
    if (TopLeft != nullptr) {
        count = count + totalNodesAux(this->TopLeft);
    }
    if (TopRight != nullptr) {
        count = count + totalNodesAux(this->TopRight);
    }
    if (BottomLeft != nullptr) {
        count = count + totalNodesAux(this->BottomLeft);
    }
    if (BottomRight != nullptr) {
        count = count + totalNodesAux(this->BottomRight);
    }
    return count;
}
int QuadTree::totalNodes() { return totalNodesAux(this); }

void QuadTree::insert(Point p, int data) {
    if (inBounds(p)) {
        if (this->type == WHITE) {
            this->type = BLACK;
            this->count++;
            this->datasum = this->datasum + data;
        }
        if (abs(TopLeftPoint.getX() - BottomRightPoint.getX()) == 0 &&
            abs(TopLeftPoint.getY() - BottomRightPoint.getY()) == 0) {
            node = new Node(p, data);
        }
        if (TopLeftPoint.getX() + BottomRightPoint.getX() / 2 <= p.getX()) {
            if (TopLeftPoint.getY() + BottomRightPoint.getY() / 2 <= p.getY()) {
                if (TopLeft == nullptr) {
                    TopLeft = new QuadTree(
                        TopLeftPoint,
                        Point(TopLeftPoint.getX() + BottomRightPoint.getX() / 2,
                              TopLeftPoint.getY() + BottomRightPoint.getY() / 2),
                        1);
                }
                TopLeft->insert(p, data);
            } else {
                if (TopRight == nullptr) {
                    TopRight = new QuadTree(
                        Point((TopLeftPoint.getX() + BottomRightPoint.getX() / 2) + 1,
                              TopLeftPoint.getY()),
                        Point(BottomRightPoint.getX(),
                              TopLeftPoint.getY() + BottomRightPoint.getY() / 2),
                        1);
                }
                TopRight->insert(p, data);
            }
        } else {
            if (TopLeftPoint.getY() + BottomRightPoint.getY() / 2 <= p.getY()) {
                if (BottomLeft == nullptr) {
                    BottomLeft = new QuadTree(
                        Point(TopLeftPoint.getX(),
                              (TopLeftPoint.getY() + BottomRightPoint.getY() / 2) + 1),
                        Point(TopLeftPoint.getX() + BottomRightPoint.getX() / 2,
                              BottomRightPoint.getY()),
                        1);
                }
                BottomLeft->insert(p, data);
            } else {
                if (BottomRight == nullptr) {
                    BottomLeft = new QuadTree(
                        Point((TopLeftPoint.getX() + BottomRightPoint.getX() / 2) + 1,
                              (TopLeftPoint.getY() + BottomRightPoint.getY() / 2) + 1),
                        BottomRightPoint, 1);
                }
                BottomRight->insert(p, data);
            }
        }
    }
}
int QuadTree::countRegion(Point p, int d) {
    if (inBounds(p)) {
        Point pTL = Point(p.getX() - d, p.getY() - d);
        if (!inBounds(pTL)) {
            if (pTL.getX() < this->TopLeftPoint.getX()) {
                pTL.setPoint(TopLeftPoint.getX(), pTL.getY());
            }
            if (pTL.getY() < this->TopLeftPoint.getY()) {
                pTL.setPoint(pTL.getX(), TopLeftPoint.getY());
            }
        }
        Point pBR = Point(p.getX() + d, p.getY() + d);
        if (!inBounds(pBR)) {
            if (pBR.getX() > this->BottomRightPoint.getX()) {
                pBR.setPoint(BottomRightPoint.getX(), pBR.getY());
            }
            if (pBR.getY() > this->BottomRightPoint.getY()) {
                pBR.setPoint(pBR.getX(), BottomRightPoint.getY());
            }
        }
        return countRegionAux(pTL, pBR, this);
    }
    return 0;
}

int QuadTree::countRegionAux(Point pTL, Point pBR, QuadTree* QT) {
    int sum = 0;
    if (QT->TopLeftPoint.getX() >= pTL.getX() && QT->TopLeftPoint.getY() >= pTL.getY() &&
        QT->BottomRightPoint.getX() <= pBR.getX() &&
        QT->BottomRightPoint.getY() <= pBR.getY()) {
        return (QT->count);
    }
    if (QT->TopLeft != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->TopLeft);
    }
    if (QT->TopRight != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->TopRight);
    }
    if (QT->BottomLeft != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->BottomLeft);
    }
    if (QT->BottomRight != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->BottomRight);
    }
    return (sum);
}

int QuadTree::AggregateRegion(Point p, int d) {
    if (inBounds(p)) {
        Point pTL = Point(p.getX() - d, p.getY() - d);
        if (!inBounds(pTL)) {
            if (pTL.getX() < this->TopLeftPoint.getX()) {
                pTL.setPoint(TopLeftPoint.getX(), pTL.getY());
            }
            if (pTL.getY() < this->TopLeftPoint.getY()) {
                pTL.setPoint(pTL.getX(), TopLeftPoint.getY());
            }
        }
        Point pBR = Point(p.getX() + d, p.getY() + d);
        if (!inBounds(pBR)) {
            if (pBR.getX() > this->BottomRightPoint.getX()) {
                pBR.setPoint(BottomRightPoint.getX(), pBR.getY());
            }
            if (pBR.getY() > this->BottomRightPoint.getY()) {
                pBR.setPoint(pBR.getX(), BottomRightPoint.getY());
            }
        }
        return AggregateRegionAux(pTL, pBR, this);
    }
    return 0;
}

int QuadTree::AggregateRegionAux(Point pTL, Point pBR, QuadTree* QT) {
    int sum = 0;
    if (QT->TopLeftPoint.getX() >= pTL.getX() && QT->TopLeftPoint.getY() >= pTL.getY() &&
        QT->BottomRightPoint.getX() <= pBR.getX() &&
        QT->BottomRightPoint.getY() <= pBR.getY()) {
        return (QT->datasum);
    }
    if (QT->TopLeft != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->TopLeft);
    }
    if (QT->TopRight != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->TopRight);
    }
    if (QT->BottomLeft != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->BottomLeft);
    }
    if (QT->BottomRight != nullptr) {
        sum = sum + countRegionAux(pTL, pBR, QT->BottomRight);
    }
    return (sum);
}

void QuadTree::clear() {}

RegionType QuadTree::getType() { return (type); }

bool QuadTree::inBounds(Point p) {
    return (p.getX() >= TopLeftPoint.getX() && p.getX() <= OriginalBottomRightPoint.getX() &&
            p.getY() <= TopLeftPoint.getY() && p.getY() >= OriginalBottomRightPoint.getY());
}

Point QuadTree::getPointTL() { return (TopLeftPoint); }

Point QuadTree::getPointBR() { return (BottomRightPoint); }