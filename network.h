#include <iostream>
#include <cstdlib>
#include "layer.h"
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

class network{
public:
  network();
  void initLayers(int);
  void initLayerNodes(int, int);
  int getNumLayers();
  void go();
  void readWeights();
  void feedForward();
  void train();
  void topology();
private:
  int numLayers;
  layer* layers;
  double* errors;
  double* corrects;
  double* solutions;
  double patternMax;
  string weights;
  string patterns;
  string outputs;
  string correct;
  ifstream weightFile;
  ifstream patternFile;
  ifstream correctFile;
  ifstream trainOut;
  ofstream outFile;
  int mode;
};
