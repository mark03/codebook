using ii = pair<int,int>;
using ll = long long;

ii floyd(ll x0) {
  ll tort = f(x0), hare = f(f(x0));
  while(tort != hare) {
    tort = f(tort);
    hare = f(f(hare));
  }

  int mu = 0;
  hare = x0;
  while(tort != hare) {
    tort = f(tort);
    hare = f(hare);
    ++mu;
  }

  int lambda = 1;
  hare = f(tort);
  while(tort != hare) {
    hare = f(hare);
    ++lambda;
  }

  // mu is the index of the first element in the cycle
  // lambda is the length of the cycle
  return ii(mu, lambda);
}