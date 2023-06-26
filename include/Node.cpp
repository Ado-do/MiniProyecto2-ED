#pragma once

#include "Node.hpp"


Node::Node(){
    data = -1;
}
Node::Node(Point point, int data){
    this->point = point;
    this->data = data;
}
Node::~Node(){}
void Node::setdata(int data){
    this->data = data;
}
void Node::setPoint(Point point){
    this->point = point;
}
int Node::getdata(){
    return(data);
}
Point Node::getPoint(){
    return(point);
}