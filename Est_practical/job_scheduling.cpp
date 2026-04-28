#include <bits/stdc++.h>
using namespace std;
struct Job { int id, d, p; };
int main() {
    int n;
    if (!(cin >> n)) return 0;
    vector<Job> a(n);
    for (int i = 0; i < n; i++) cin >> a[i].id >> a[i].d >> a[i].p;
    sort(a.begin(), a.end(), [](const Job &x, const Job &y) { return x.p > y.p; });
    int maxd = 0;
    for (auto &j : a) if (j.d > maxd) maxd = j.d;
    vector<int> slot(maxd + 1, -1);
    int total = 0;
    for (int i = 0; i < n; i++) {
        for (int t = a[i].d; t > 0; t--) {
            if (slot[t] == -1) {
                slot[t] = i;
                total += a[i].p;
                break;
            }
        }
    }
    for (int t = 1; t <= maxd; t++) if (slot[t] != -1) cout << a[slot[t]].id << " ";
    cout << "\n" << total;
    return 0;
}
