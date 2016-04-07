#include "node.h"

using namespace std;

node::node(){
  val = 0;
  weights = NULL;
}

void node::setVal(double x){
  val = x;
}

double node::getVal(){
  return val;
}

void node::initWeights(int x){
  numWeights = x;
  weights = new double[numWeights];
}

double node::getWeight(int x){
  if (weights != NULL){
      return weights[x];
  }
  else{
    return -99999999999;
  }
}

void node::setWeight(int x, double value){
  weights[x] = value;
}

int node::getNumWeights(){
  return numWeights;
}
