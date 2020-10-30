#include <iostream>
#include <bits/stdc++.h>
#include <climits>
using namespace std;
#define ll long long int

string str;
ll *crank, *pos;
ll *cnt, *nextr;
bool *bh, *b2h;
ll *height;


bool smaller_first_char(int a, int b){
    return str[a] < str[b];
}

void init_arrs(ll n) {
    crank = new ll[n];
    cnt = new ll[n];
    pos = new ll[n];
    nextr = new ll[n];
    bh = new bool[n];
    b2h = new bool[n];
    height = new ll[n];
}

void suffixSort(ll n){
    for (ll i=0; i<n; ++i){
        pos[i] = i;
    }
    sort(pos, pos + n, smaller_first_char);

    for (ll i=0; i<n; ++i){
        bh[i] = i == 0 || str[pos[i]] != str[pos[i-1]];
        b2h[i] = false;
    }

    for (ll h = 1; h < n; h <<= 1){
        ll buckets = 0;
        for (ll i=0, j; i < n; i = j){
            j = i + 1;
            while (j < n && !bh[j]) j++;
            nextr[i] = j;
            buckets++;
        }
        if (buckets == n) break;

        for (ll i = 0; i < n; i = nextr[i]){
            cnt[i] = 0;
            for (ll j = i; j < nextr[i]; ++j){
                crank[pos[j]] = i;
            }
        }

        cnt[crank[n - h]]++;
        b2h[crank[n - h]] = true;
        for (ll i = 0; i < n; i = nextr[i]){
            for (ll j = i; j < nextr[i]; ++j){
                ll s = pos[j] - h;
                if (s >= 0){
                    ll head = crank[s];
                    crank[s] = head + cnt[head]++;
                    b2h[crank[s]] = true;
                }
            }
            for (ll j = i; j < nextr[i]; ++j){
                ll s = pos[j] - h;
                if (s >= 0 && b2h[crank[s]]){
                    for (ll k = crank[s]+1; !bh[k] && b2h[k]; k++)
                        b2h[k] = false;
                }
            }
        }
        for (ll i=0; i<n; ++i){
            pos[crank[i]] = i;
            bh[i] |= b2h[i];
        }
    }
    for (ll i=0; i<n; ++i){
        crank[pos[i]] = i;
    }
}

void getLCP(ll n){
    for (ll i=0; i<n; ++i)
        crank[pos[i]] = i;
    for (ll i=0, h=0; i<n; ++i){
        if(crank[i] == n-1) {
            h = 0;
            continue;
        }
        ll j = pos[crank[i]+1];
        while (i + h < n && j + h < n && str[i+h] == str[j+h])
            h++;
        height[crank[i]] = h;
        if (h > 0)
            h--;
    }
    for(ll i=0;i<n;i++) {
        cout << height[i] << " ";
    }
}

void findKLongSb(ll k) {
    ll n = str.length();
    init_arrs(n);
    suffixSort(n);
    for(ll i=0;i<str.length();i++) {
        cout << pos[i] << " ";
    }
    cout << endl;
    ll maxh=0, maxi=0;
    if(k==1) {
        cout << str.substr(pos[0],1) << endl;
        return;
    }
    getLCP(str.length());
    cout << endl;
    for(ll i=0;i<=str.length()-k;i++) {
        cout << str.substr(pos[i],str.length()) << endl;
    }
    for(ll i=0;i<=str.length()-k;i++) {
        ll minh = LONG_LONG_MAX;
        ll mj = 0;
        for(ll j=i;j<i+k-1;j++) {
//            cout << "ht" << height[j] << " ";
            if(height[j]<minh) {
                minh = height[j];
                mj = pos[j];
            }
        }
        if(minh>maxh) {
            maxh = minh;
            maxi = mj;
        }
    }
    if(maxh>0)
        cout << endl << maxh << ":" << maxi << " : " << str.substr(maxi,maxh) << endl;
}

int main() {
    ll k;
    cin >> str;
    cin >> k;
    findKLongSb(k);
    return 0;
}
