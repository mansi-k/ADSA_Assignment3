using namespace std;

int merge_lists(vector<int> &A, long p, long q, long r) {
    long n1,n2,i,j,k;
    n1=q-p+1;
    n2=r-q;
    long L[n1],R[n2];
    for(i=0;i<n1;i++) {
        L[i]=A[p+i];
    }
    for(j=0;j<n2;j++) {
        R[j]=A[q+j+1];
    }
    i=0,j=0;
    for(k=p;i<n1&&j<n2;k++) {
        if(L[i]<R[j]) {
            A[k]=L[i++];
        }
        else {
            A[k]=R[j++];
        }
    }
    while(i<n1) {
        A[k++]=L[i++];
    }
    while(j<n2) {
        A[k++]=R[j++];
    }
}

int mergeSort(vector<int> &A,long p,long r) {
    long q;
    if(p<r) {
        q=(p+r)/2;
        mergeSort(A,p,q);
        mergeSort(A,q+1,r);
        merge_lists(A,p,q,r);
    }
}