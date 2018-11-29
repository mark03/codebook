for (int k = 0; k < V; k++)
  for (int i = 0; i < V; i++)
    for (int j = 0; j < V; j++)
      AdjMat[i][j] =
          min(AdjMat[i][j], AdjMat[i][k] + AdjMat[k][j]);