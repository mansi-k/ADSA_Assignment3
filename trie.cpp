#include <iostream>
#include <vector>
#include <climits>
using namespace std;
#define ll long long int

struct TrieNode {
    TrieNode* left = nullptr;  // 0
    TrieNode* right = nullptr;  // 1
};

void insertNode(TrieNode* root, ll x, ll b) {
    TrieNode* curr = root;

}

long searchMaxXOR(TrieNode* root, ll x) {

}

ll getMaxBits(ll x) {
    ll b=0;
    while(x>0) {
        x /= 2;
        b++;
    }
    return b;
}

int main() {
    ll n, q;
    cin >> n >> q;
    vector<ll> vec;
    ll arr[n];
    ll maxx=LONG_LONG_MIN;
    for(ll i=0;i<n;i++) {
        cin >> arr[i];
        if(arr[i]>maxx)
            maxx = arr[i];
    }
    ll maxbits = getMaxBits(maxx);
    cout << maxbits;
    TrieNode* root = new TrieNode;
    for(ll i=0;i<n;i++) {

    }
    return 0;
}
