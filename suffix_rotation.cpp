#include <iostream>
#include <cstring>
#include <string>
using namespace std;


void suffix_sort(int n, int k, int RA[], int SA[], int tempSA[] ) {
    int C[max(256,n)];
    memset(C, 0, sizeof C);

    for (int i = 0; i < n; i++)
        C[RA[(i + k)%n]]++;

    int sum = 0;
    for (int i = 0; i < max(256, n); i++) {
        int t = C[i];
        C[i] = sum;
        sum += t;
    }

    for (int i = 0; i < n; i++)
        tempSA[C[RA[(SA[i] + k)%n]]++] = SA[i];

    memcpy(SA, tempSA, n*sizeof(int));
}

int suffix_array(string &s) {
    int n = s.size();
    int RA[n], tempRA[n];
    int SA[n], tempSA[n];
    memset(tempRA, 0, sizeof tempRA);
    memset(tempSA, 0, sizeof tempSA);

    for (int i = 0; i < n; i++)
        RA[i] = s[i];

    for (int i = 0; i < n; i++)
        SA[i] = i;

    for (int k = 1; k < n; k *= 2) {
        suffix_sort(n, k, RA, SA, tempSA);
        suffix_sort(n, 0, RA, SA, tempSA);

        int r = tempRA[SA[0]] = 0;
        for (int i = 1; i < n; i++) {
            int s1 = SA[i], s2 = SA[i-1];
            bool equal = true;
            equal &= RA[s1] == RA[s2];
            equal &= RA[(s1+k)%n] == RA[(s2+k)%n];

            tempRA[SA[i]] = equal ? r : ++r;
        }

        memcpy(RA, tempRA, n*sizeof(int));
    }
    return SA[0];
}

int main() {
    string s; cin >> s;
    int pos = suffix_array(s);
    cout << pos+1 << endl;
    cout << s.substr(pos,s.length())+s.substr(0,pos) << endl;
    return 0;
}
