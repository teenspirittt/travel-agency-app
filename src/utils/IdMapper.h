#include <unordered_map>

class IdMapper {
 public:
  IdMapper() : nextAbstractId_(1) {}

  int generateNextAbstractId() {
    int abstractId = nextAbstractId_++;
    return abstractId;
  }

  int mapToRealId(int abstractId) {
    if (abstractToRealMap_.count(abstractId) > 0) {
      return abstractToRealMap_[abstractId];
    }
    return -1;
  }

  int mapToAbstractId(int realId) {
    if (realToAbstractMap_.count(realId) > 0) {
      return realToAbstractMap_[realId];
    }
    return -1;
  }

  void addMapping(int abstractId, int realId) {
    abstractToRealMap_[abstractId] = realId;
    realToAbstractMap_[realId] = abstractId;
  }

 private:
  std::unordered_map<int, int> abstractToRealMap_;
  std::unordered_map<int, int> realToAbstractMap_;
  int nextAbstractId_;
};
