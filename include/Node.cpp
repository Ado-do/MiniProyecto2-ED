#pragma once

#include "Node.hpp"


Node::Node(){
    NW = nullptr;
    NE = nullptr;
    SW = nullptr;
    SE = nullptr;
    data = -1;
    type = WHITE;
    point = nullptr;
}
Node::~Node(){
    delete NW;
    delete NE;
    delete SW;
    delete SE;
}
void Node::setNW(Node* NewNode){
    NW = NewNode;
}
void Node::setNE(Node* NewNode){
    NE = NewNode;
}
void Node::setSW(Node* NewNode){
    SW = NewNode;
}
void Node::setSE(Node* NewNode){
    SE = NewNode;
}
void Node::setdata(int data){
    this->data = data;
}
void Node::stype(){ //cambia el color de type
    if (type == WHITE){
        type = BLACK;
    }else{
        type = WHITE;
    }
}
void Node::setPoint(Point* point){
    this->point = point;
}
Node* Node::getNW(){
    return(NW);
}
Node* Node::getNE(){
    return(NE);
}
Node* Node::getSW(){
    return(SW);
}
Node* Node::getSE(){
    return(SE);
}
int Node::getdata(){
    return(data);
}
NodeType Node::gettype(){
    return(type);
}
Point* Node::getPoint(){
    return(point);
}