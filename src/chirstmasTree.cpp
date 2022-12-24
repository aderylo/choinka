// #include "src/chirstmasTree.hh"


// void preOrderTraversal(vector<size_t>& order, vector<bool>& visited,
//                        map<size_t, vector<size_t>> children, size_t currentNode) {
//   if (visited.at(currentNode))
//     return;

//   order.push_back(currentNode);
//   visited.at(currentNode) = true;

//   for (const auto& childNode : children[currentNode]) {
//     preOrderTraversal(order, visited, children, childNode);
//   }
// }

// map<size_t, vector<size_t>> getChildMap(vector<size_t> treeDescription) {
//   map<size_t, vector<size_t>> children;
//   for (size_t i = 0; i < treeDescription.size(); i++) {
//     int childIdx = i + 2;
//     int parentIdx = treeDescription.at(i);
//     children[parentIdx].push_back(childIdx);
//   }

//   return children;
// }

// vector<pair<size_t, size_t>> getSubtreeRanges(vector<size_t> preOrder,
//                                               map<size_t, vector<size_t>> childrenMap) {
//   vector<pair<size_t, size_t>> ranges;
//   //   size_t i = 1;
//   //   for (const auto& node : preOrder) {
//   //     int subtreeSize = childrenMap.at(node).size();
//   //     ranges.push_back({i, i + subtreeSize - 1)
//   //   }
//   // }


//   vector<size_t> getPreOrder(vector<size_t> treeDescription) {
//     size_t numOfNodes = treeDescription.size() + 1;
//     map<size_t, vector<size_t>> children = getChildMap(treeDescription);
//     vector<bool> visited(numOfNodes + 1, false);
//     vector<size_t> result;

//     preOrderTraversal(result, visited, children, 1);

//     return result;
//   }


//   ChristmasTree::ChristmasTree(vector<size_t> treeDescription, vector<size_t> colours) {
//     vector<size_t> leafs(colours.size(), 0);
//     vector<size_t> preOrder = getPreOrder(treeDescription);

//     for (size_t i = 0; i <= preOrder.size(); i++) {
//       leafs.at(i) = colours.at(preOrder.at(i) - 1);
//     }


//     this->tree = SegmentTree(leafs);
//   }

//   bool ChristmasTree::isSubtreeAlmostSingleColour(size_t node) {
//     pair<size_t, size_t> range = this->subtreeRanges.at(node);
//     this->tree =
//   }