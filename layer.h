#include <iostream>
#include <cstdlib>
#include "node.h"

using namespace std;

class layer{
public:
  layer();
  void setNodes(int);
  node& getNode(int);
  int getNumNodes();

private:
    node* nodes;
  int numNodes;
};
