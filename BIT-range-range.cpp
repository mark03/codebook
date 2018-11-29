int sum(int x, int BITTree1[], int BITTree2[]) {
  return getSum(BITTree1, x) * x - getSum(BITTree2, x);
}
void updateRange(int BITTree1[], int BITTree2[], int n, int val,
                 int l, int r) {
  updateBIT(BITTree1, n, l, val);
  updateBIT(BITTree1, n, r + 1, -val);
  updateBIT(BITTree2, n, l, val * (l - 1));
  updateBIT(BITTree2, n, r + 1, -val * r);
}
int rangeSum(int l, int r, int BITTree1[], int BITTree2[]) {
  return sum(r, BITTree1, BITTree2) -
         sum(l - 1, BITTree1, BITTree2);
}