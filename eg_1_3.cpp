#include <iostream>
#include<iterator>
using namespace std;

template<typename T>
void permutations(T *a, int len, int cur){
    if (cur == len) {
        copy(a, a + len, ostream_iterator<T>(cout, " "));
        cout << endl;
    } else{
        for (int i = cur; i < len; ++i) {
            swap(a[cur], a[i]);
            permutations(a, len, cur + 1);
            swap(a[cur], a[i]);
        }
    }
}

int main(){
    int a[5] = {1, 2, 3,4,5};
    permutations(a, 5, 0);
}
