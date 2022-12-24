#ifndef CHOINKA_UTIL
#define CHOINKA_UTIL

#include <map>
#include <vector>
#include "src/segmentTree.hh"
using namespace std;

class ChristmasTree {
 public:
  ChristmasTree(vector<size_t> treeDescription, vector<size_t> colours);

  bool isSubtreeAlmostSingleColour(size_t node);

  void changeNodeColour(size_t node, size_t colour);

 private:
  SegmentTree tree;
  vector<pair<size_t, size_t>> subtreeRanges;
};




#endif  // CHOINKA_UTIL
