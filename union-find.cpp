#include <numeric>
#include <vector>
using namespace std;

struct UnionFind {
  UnionFind(int N) : rank(N, 0), uf(N) {
    iota(uf.begin(), uf.end(), 0);
  }
  int Find(int i) {
    return uf[i] == i ? i : (uf[i] = Find(uf[i]));
  }
  bool IsSameSet(int i, int j) { return Find(i) == Find(j); }
  void Union(int i, int j) {
    if (!IsSameSet(i, j)) {
      int x = Find(i), y = Find(j);
      if (rank[x] > rank[y]) {
        uf[y] = x;
      } else {
        uf[x] = y;
        if (rank[x] == rank[y])
          rank[y]++;
      }
    }
  }
  vector<int> rank, uf;
};