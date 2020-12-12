#include <iostream>
#include <cstring>
#include <string>
using namespace std;


void suffix_sort(long n, long k, long RA[], long SA[], long tempSA[] ) {
    long C[max((long)256,n)];
    memset(C, 0, sizeof C);

    for (long i = 0; i < n; i++)
        C[RA[(i + k)%n]]++;

    long sum = 0;
    for (long i = 0; i < max((long)256, n); i++) {
        long t = C[i];
        C[i] = sum;
        sum += t;
    }

    for (long i = 0; i < n; i++)
        tempSA[C[RA[(SA[i] + k)%n]]++] = SA[i];

    memcpy(SA, tempSA, n*sizeof(long));
}

long suffix_array(string str) {
    long n = str.length();
    char s[n+1];
    strcpy(s,str.c_str());
    long RA[n], tempRA[n];
    long SA[n], tempSA[n];
    memset(tempRA, 0, sizeof tempRA);
    memset(tempSA, 0, sizeof tempSA);

    for (long i = 0; i < n; i++)
        RA[i] = s[i];

    for (long i = 0; i < n; i++)
        SA[i] = i;

    for (long k = 1; k < n; k *= 2) {
        suffix_sort(n, k, RA, SA, tempSA);
        suffix_sort(n, 0, RA, SA, tempSA);

        long r = tempRA[SA[0]] = 0;
        for (long i = 1; i < n; i++) {
            long s1 = SA[i], s2 = SA[i-1];
            bool equal = true;
            equal &= RA[s1] == RA[s2];
            equal &= RA[(s1+k)%n] == RA[(s2+k)%n];

            tempRA[SA[i]] = equal ? r : ++r;
        }

        memcpy(RA, tempRA, n*sizeof(long));
    }
    return SA[0];
}

int main() {
    string s; cin >> s;
    long pos = suffix_array(s);
//    cout << pos+1 << endl;
    cout << s.substr(pos,s.length())+s.substr(0,pos) << endl;
    return 0;
}