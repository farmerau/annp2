#include "layer.h"

using namespace std;

layer::layer(){
  nodes = NULL;
  numNodes = 0;
}

void layer::setNodes(int x){
  numNodes = x;
  nodes = new node[numNodes];
}

node& layer::getNode(int x){
  return nodes[x];
}

int layer::getNumNodes(){
  return numNodes;
}
