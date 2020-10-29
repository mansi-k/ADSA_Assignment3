#include <iostream>
#include <vector>
#include <algorithm>
#include <stdexcept>
using namespace std;

template <class T>
class defaultCompare {
public:
    bool operator()(const T a, const T b) {
        return a > b;
    }
};

template <class T, class C=defaultCompare<T>>
struct PriorityQ
{
private:
    vector<T> A;
    C cmp;
    int PARENT(int i) {
        return (i - 1) / 2;
    }
    int LEFT(int i) {
        return (2 * i + 1);
    }
    int RIGHT(int i) {
        return (2 * i + 2);
    }
    void heapify_down(int i) {
        int left = LEFT(i);
        int right = RIGHT(i);
        int smallest = i;
        if (left < size() && cmp(A[left],A[i])<0)
            smallest = left;
        if (right < size() && cmp(A[right],A[smallest])<0)
            smallest = right;
        if (smallest != i) {
            swap(A[i], A[smallest]);
            heapify_down(smallest);
        }
    }
    void heapify_up(int i) {
        if(i && cmp(A[PARENT(i)],A[i])>0) {
            swap(A[i], A[PARENT(i)]);
            heapify_up(PARENT(i));
        }
    }

public:
    unsigned int size() {
        return A.size();
    }
    bool empty() {
        return size() == 0;
    }
    void push(T key) {
        A.push_back(key);
        int index = size() - 1;
        heapify_up(index);
    }
    void pop() {
        A[0] = A.back();
        A.pop_back();
        heapify_down(0);
    }
    T top() {
        return A.at(0);
    }
};
