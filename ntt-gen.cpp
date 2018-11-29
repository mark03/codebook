const int MAXPF = 1e6;
vector<int> primes; // primes till MAXPF
int find_primitive_root(int p) {
  vector<int> factors;
  factorize(p - 1, factors);

  for (auto a : primes) { // primitive root must be a prime
                          // number (? maybe)
    bool ok = true;
    for (auto f : factors)
      /* if a is not a primitive root then the sequence of the
       * powers from 1 to p - 1 contains a cycle and since a ^
       * (p - 1) = 1 mod p a ^ (cycle_length * k) = 1 mod p for
       * any integer k it is enough to check the largest
       * divisors of p - 1, obtained by removing only one prime
       * factor of p - 1 at a time
       */
      if (modpow(a, (p - 1) / f, p) == 1LL)
        ok = false;
    if (ok)
      return a;
  }
}

int main() {
  sieve();
  int j = 27;
  ll P = 2013265921;
  // 2 ^ j * k + 1, any prime of this form works for input size
  // up to 2 ^ j and for coefficients (of the product) up to 2 ^
  // j * k
  ll r = find_primitive_root(P);
  // the sequence of the powers of r from 1 to P - 1
  // contains all numbers from 1 to P - 1 exactly once
  vector<int> _wn, _wn1;
  for (int i = 1; i <= j; ++i) {
    int n = 1 << i;
    int k = (P - 1) / n;
    // P = n * k + 1
    ll wn = modpow(r, k, P);
    // wn^n = (r ^ k) ^ n = r ^ (k * n) = r ^ (P - 1) = 1 mod P
    ll wn1 = modpow(wn, n - 1, P);
    // wn1 = wn ^ (n - 1) mod P, used in the interpolation step
    _wn.push_back(wn);
    _wn1.push_back(wn1);
  }
  pprint(_wn);
  pprint(_wn1);
  return 0;
}
