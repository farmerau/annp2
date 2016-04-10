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
  void readPatterns();
  void feedForward();
  void train();
  void calculateError(int, int);
  void adjustOutWeights();
  void adjustHiddenWeights();
private:
  int numLayers;
  int numPat;
  int inputs;
  layer* layers;
  double* errors;
  double* corrects;
  double* solutions;
  double* hiddenLayerVals;
  double* outputLayerVals;
  double* inputLayerVals;
  double* patternValues;
  double patternMax;
  double sysErr;
  bool memFlag;
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
