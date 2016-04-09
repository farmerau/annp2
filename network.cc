#include "network.h"

using namespace std;

network::network(){
  numLayers = 0;
  layers = NULL;
  errors = NULL;
  corrects = NULL;
  solutions = NULL;
  inputLayerVals = NULL;
  hiddenLayerVals = NULL;
  outputLayerVals = NULL;
  weights = "weights.in";
  patterns = "patterns.in";
  outputs = "outputs.out";
  correct = "correct.out";
}

void network::initLayers(int x){
  numLayers = x;
  layers = new layer[numLayers];
}

void network::initLayerNodes(int layerN, int nodeNum){
  layers[layerN].setNodes(nodeNum);
}

int network::getNumLayers(){
  return numLayers;
}

void network::go(){
  readWeights();
  cout << "Options:\n1.)Production Mode\n2.)Training Mode\n>>";
  cin >> mode;
  feedForward();
}

void network::readWeights(){
  cout << "Reading " << weights << "...\n";
  weightFile.open(weights.c_str());
  if (weightFile.fail()){
    cout << "Error opening " << weights << "...";
    exit(1);
  }
  int inputNodes;
  weightFile >> inputNodes;
  int hiddenNodes;
  weightFile >> hiddenNodes;
  int outputNodes;
  weightFile >> outputNodes;
  initLayers(3);
  cout << "Initializing " << inputNodes << " in input layer...";
  initLayerNodes(0, inputNodes);
  cout << " Done.\n";
  cout << "Initializing " << hiddenNodes << " in hidden layer...";
  initLayerNodes(1, hiddenNodes);
  for (int i = 0; i < hiddenNodes; i++){
    layers[1].getNode(i).initWeights(inputNodes);
  }
  cout << " Done.\n";
  cout << "Initializing " << outputNodes << " in output layer...";
  initLayerNodes(2, outputNodes);
  for (int i = 0; i < outputNodes; i++){
    layers[2].getNode(i).initWeights(hiddenNodes);
  }
  cout << " Done.\n";

  for (int i = 0; i < hiddenNodes; i++){
    for (int j = 0; j < layers[1].getNode(i).getNumWeights(); j++){
      double temp;
      weightFile >> temp;
      layers[1].getNode(i).setWeight(j,temp);
    }
  }

  for (int i = 0; i < outputNodes; i++){
    for (int j = 0; j < layers[2].getNode(i).getNumWeights(); j++){
      double temp;
      weightFile >> temp;
      layers[2].getNode(i).setWeight(j, temp);
    }
  }
  weightFile.close();
  cout << weights << " loaded successfully!\n";
}

void network::feedForward(){
  cout << "Reading " << patterns << "...\n";
  patternFile.open(patterns.c_str());
  if (patternFile.fail()){
    cout << "Error opening " << patterns << "...";
    exit(1);
  }
  patternFile >> numPat;
  int inputs;
  patternFile >> inputs;
  patternFile >> patternMax;
  if (inputs != layers[0].getNumNodes()){
    cout << patterns << " incompatible with this network.";
    exit(2);
  }
  if (mode == 2){
    inputLayerVals = new double[numPat*layers[0].getNumNodes()];
    hiddenLayerVals = new double[numPat*layers[1].getNumNodes()];
    outputLayerVals = new double[numPat*layers[2].getNumNodes()];
  }
  outFile.open(outputs.c_str());
  outFile << numPat << "\n";
  for (int cycle = 0; cycle < numPat; cycle++){
    for (int i = 0; i < inputs; i++){
      double temp;
      patternFile >> temp;
      layers[0].getNode(i).setVal(temp/patternMax);
      if (mode == 2){
        inputLayerVals[layers[0].getNumNodes()*i + cycle] = layers[0].getNode(i).getVal();
      }
    }

    for (int i = 0; i < layers[1].getNumNodes(); i++){
      double inputSum;
      for (int j = 0; j < layers[1].getNode(i).getNumWeights(); j++){
        cout << layers[0].getNode(j).getVal() << " * " << layers[1].getNode(i).getWeight(j) << " = " << layers[0].getNode(j).getVal() * layers[1].getNode(i).getWeight(j) << "\n";
        inputSum += (layers[0].getNode(j).getVal() * layers[1].getNode(i).getWeight(j));
      }
      layers[1].getNode(i).setVal(1/(exp(-inputSum)+1));
      if (mode == 2){
        hiddenLayerVals[layers[1].getNumNodes()*i + cycle] = layers[1].getNode(i).getVal();
      }
      inputSum = 0;
    }
    for (int i = 0; i < layers[2].getNumNodes(); i++){
      double hiddenSum;
      for (int j = 0; j < layers[2].getNode(i).getNumWeights(); j++){
        cout << layers[1].getNode(j).getVal() << " * " << layers[2].getNode(i).getWeight(j) << " = " << layers[1].getNode(j).getVal() * layers[2].getNode(i).getWeight(j) << "\n";
        hiddenSum+= (layers[1].getNode(j).getVal() * layers[2].getNode(i).getWeight(j));
      }
      layers[2].getNode(i).setVal(hiddenSum);
      if (mode == 2){
        outputLayerVals[layers[2].getNumNodes()*i + cycle] = layers[2].getNode(i).getVal();
      }
      hiddenSum = 0;
      cout << " " << layers[2].getNode(i).getVal() << " ";
      if (i != 0){
        outFile << " ";
      }
      outFile << layers[2].getNode(i).getVal();
    }


    cout << "\n";
    outFile << "\n";
  }
  outFile.close();
  cout << outputs << " written!\n";
  patternFile.close();
  if (mode == 2){
    train();
  }
}

void network::train(){
  cout << "Starting training procedures...\n";
  cout << "Loading reference file " << correct << "...\n";
  correctFile.open(correct.c_str());
  if (correctFile.fail()){
    cout << "Failure opening " << correct << "\n";
    exit (2);
  }
  cout << "Loading results from feed forward mechanism...\n";
  trainOut.open(outputs.c_str());
  if (trainOut.fail()){
    cout << "Failure opening " << outputs << "\n";
    exit(2);
  }
  int cPatterns;
  correctFile >> cPatterns;
  int cOutputs;
  correctFile >> cOutputs;
  int numTests;
  trainOut >> numTests;
  if ((cPatterns != numTests)||(cOutputs != layers[2].getNumNodes())){
    cout << "Reference file incompatible with this simulation...\n";
    exit (3);
  }
  calculateError(cPatterns, cOutputs);
  adjustOutWeights();
}

void network::calculateError(int cPatterns, int cOutputs){
  errors = new double[(cPatterns*cOutputs)];
  corrects = new double[(cPatterns*cOutputs)];
  solutions = new double[(cPatterns*cOutputs)];
  for (int i = 0; i < (cPatterns*cOutputs); i++){
    double temp;
    trainOut >> temp;
    solutions[i] = temp;
    correctFile >> temp;
    corrects[i] = temp;
    errors[i] = corrects[i]-solutions[i];
  }
}

void network::adjustOutWeights(){
  double sum = 0.0;
  double a = 0.1;
  for (int i = 0; i < layers[2].getNumNodes(); i++){
    for (int j = 0; j < layers[1].getNumNodes(); j++){
      for (int k = 0; k < numPat; k++){
        sum += -errors[i] * hiddenLayerVals[layers[1].getNumNodes()*j + k];
      }
      cout << layers[2].getNode(i).getWeight(j) << " :: ";
      layers[2].getNode(i).setWeight(j, (layers[2].getNode(i).getWeight(j)-(a*sum)));
      cout << layers[2].getNode(i).getWeight(j) << "\n";
      sum = 0;
    }
  }
}

void network::adjustHiddenWeights(){

}

void network::adjustInputWeights(){

}
