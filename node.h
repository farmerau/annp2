#include <iostream>
#include <cstdlib>

using namespace std;

class node{
public:
  node(); //Default constructor
  //Getters and Setters
  void setVal(double x);
  double getVal();
  void initWeights(int);
  double getWeight(int);
  void setWeight(int, double);
  int getNumWeights();
  //End Getters and Setters

private:
  double val;
  double* weights;
  int numWeights;
};
