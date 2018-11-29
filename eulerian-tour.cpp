list<int> cyc;
void EulerianTour(list<int>::iterator i, int u) {
  for (int j = 0; j < AdjList[u].size(), j++) {
    ii v = AdjList[u][j];
    if (v.second) {
      v.second = 0;
      for (int k = 0; k < AdjList[v.first]; k++) {
        ii uu = AdjList[v.first][k];
        if (uu.first == u && uu.second) {
          uu.second = 0;
          break;
        }
      }
      EulerianTour(cyc.insert(i, u), v.first);
    }
  }
}