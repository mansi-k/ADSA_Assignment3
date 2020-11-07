#include <iostream>
#include <vector>
#include <climits>
#include <cmath>
using namespace std;
#define ll long long int
#define MAXBITS 40

struct TrieNode {
    TrieNode* left = NULL;  // 0
    TrieNode* right = NULL;  // 1
};

void insertNode(TrieNode* root, ll x) {
    TrieNode* curr = root;
    for(ll i=MAXBITS;i>=0;i--) {
        ll b = (x>>i)&1;
        if(b==1) {
            if(!curr->right) {
                curr->right = new TrieNode();
            }
            curr = curr->right;
        }
        else {
            if(!curr->left) {
                curr->left = new TrieNode();
            }
            curr = curr->left;
        }
    }
}

long searchMaxXOR(TrieNode* root, ll x) {
    TrieNode* curr = root;
    ll max_XOR = 0;
    ll b, i;
    for(i=MAXBITS;i>=0;i--) {
        b = (x>>i)&1;
        if(!curr) {
            break;
        }
        if(b==0) {
            if(curr->right) {
                max_XOR += pow(2,i);
                curr = curr->right;
            }
            else {
                curr = curr->left;
            }
        }
        else {
            if(curr->left) {
                max_XOR += pow(2,i);
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }
        }
    }
    return max_XOR;
}

int main() {
    ll n, q, x;
    cin >> n >> q;
    TrieNode* root = new TrieNode;
    for(ll i=0;i<n;i++) {
        cin >> x;
        insertNode(root,x);
    }
    while(q--) {
        cin >> x;
        cout << searchMaxXOR(root,x) << endl;
    }
    return 0;
}
