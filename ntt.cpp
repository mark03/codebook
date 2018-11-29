typedef long long ll;

inline int lg2(int x) { return 31 - __builtin_clz(x); }

const int MAXLOG = 27;
const ll P = 2013265921;
const ll WN[] = {1,          2013265920, 1728404513, 1592366214,
                 196396260,  760005850,  1721589904, 397765732,
                 1732600167, 1753498361, 341742893,  1340477990,
                 1282623253, 298008106,  1657000625, 2009781145,
                 1421947380, 1286330022, 1559589183, 1049899240,
                 195061667,  414040701,  570250684,  1267047229,
                 1003846038, 1149491290, 975630072,  440564289};
const ll WN1[] = {
    1,          2013265920, 284861408,  1801542727, 567209306,
    1273220281, 662200255,  1856545343, 1611842161, 1861675199,
    774513262,  449056851,  1255670133, 1976924129, 106301669,
    1411306935, 1540942033, 1043440885, 173207512,  463443832,
    1021415956, 1574319791, 953617870,  987386499,  1469248932,
    165179394,  1498740239, 1713844692};

ll modpow(ll b, ll e, ll m) {
  ll res = 1LL;
  while (e) {
    if (e & 1)
      res = (res * b) % m;
    b = (b * b) % m;
    e >>= 1;
  }
  return res;
}

void powofroots(vector<ll> &wn, vector<ll> &wn1, int lg) {
  wn[0] = WN[lg];
  wn1[0] = WN1[lg];
  for (int i = 1; i <= MAXLOG; ++i) {
    wn[i] = (wn[i - 1] * wn[i - 1]) % P;
    wn1[i] = (wn1[i - 1] * wn1[i - 1]) % P;
  }
}

void recursion(const vector<ll> &s, vector<ll> &t, int n, int f,
               const vector<ll> &wn) {
  if (n == 1) {
    t = s;
    return;
  }

  ll w = 1;
  int half = n >> 1;
  vector<ll> sl(half), sr(half), tl(half), tr(half);
  for (int i = 0; i < half; ++i) {
    sl[i] = s[i << 1];
    sr[i] = s[i << 1 | 1];
  }
  recursion(sl, tl, n >> 1, f + 1, wn);
  recursion(sr, tr, n >> 1, f + 1, wn);

  for (int i = 0; i < half; ++i) {
    t[i] = (tl[i] + w * tr[i]) % P;
    t[i + half] = (tl[i] + (w * (P - 1) % P) * tr[i]) % P;
    w = (w * wn[f]) % P;
  }
}

void poly_mul(vector<ll> a, vector<ll> b, vector<ll> &c) {
  int lg = 2 + lg2(max(a.size(), b.size()) - 1);
  int n = 1 << lg;
  a.resize(n);
  b.resize(n);
  c.resize(n);

  vector<ll> wn(MAXLOG + 1), wn1(MAXLOG + 1);
  powofroots(wn, wn1, lg);

  vector<ll> px(n);
  vector<ll> qx(n);

  recursion(a, px, n, 0, wn);
  recursion(b, qx, n, 0, wn);

  for (int i = 0; i < n; ++i)
    px[i] = (px[i] * qx[i]) % P;

  recursion(px, c, n, 0, wn1);

  for (int i = 0; i < n; ++i)
    c[i] = (c[i] * modpow(n, P - 2, P)) % P;
}
