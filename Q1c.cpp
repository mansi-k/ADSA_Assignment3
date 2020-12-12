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

void calculateLCPs(ll n){
    for (int i=0; i<n; ++i) crank[pos[i]] = i;
    height[0] = 0;
    for (int i=0, h=0; i<n; ++i){
        if (crank[i] > 0){
            int j = pos[crank[i]-1];
            while (i + h < n && j + h < n && str[i+h] == str[j+h]) h++;
            height[crank[i]] = h;
            if (h > 0) h--;
        }
    }
}

string findPalindrome(string orgstr) {
    ll orglen = orgstr.length();
    string revstr = orgstr;
    reverse(revstr.begin(), revstr.end());
    str = orgstr+"#"+revstr;
    ll newlen = str.length();
    init_arrs(newlen);
    suffixSort(newlen);
    calculateLCPs(newlen);
    ll reqht=0;
    ll startptr=0;
    for(int i=1;i<newlen;++i) {
        if((height[i]>reqht)) {
            if((pos[i-1]<orglen && pos[i]>orglen)||(pos[i]<orglen && pos[i-1]>orglen)) {
                reqht=height[i];
                startptr=pos[i];
            }
        }
    }
    string longpalin = str.substr(startptr,reqht);
    return longpalin;
}

int main() {
    string orgstr;
    cin >> orgstr;
    string palin = findPalindrome(orgstr);
    cout << palin << endl;
    return 0;
}
