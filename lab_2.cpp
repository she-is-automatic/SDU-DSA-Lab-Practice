// 输入：输入的第一行是一个整数n(1<=n<=1000)，表示需排序的数的个数。
//  接下来一行是n个整数，数的范围是0到1000，每两个相邻数据间用一个空格分隔。
// 输出：一行排好序的序列。
// 排序类需提供以下操作：名次排序、及时终止的选择排序、及时终止的冒泡排序、插入排序。

#include <iostream>
#include <iterator>

using namespace std;

template<class T>
class ArraySort {
    T *arr;
    int size;
public:
    ArraySort(T *arr, int n) : arr(arr), size(n) {};

    void rankSort();    //名次排序
    void selectSort();  //及时终止的选择排序
    void bubbleSort();  //及时终止的冒泡排序
    void insertSort();  //插入排序

    template<class U>
    friend ostream &operator<<(ostream &out, ArraySort<U> &a);  //重载<<，输出数组的值

};

int main() {
    int n;
    cin >> n;
    int *a = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }
    ArraySort<int> A(a, n);
//    A.bubbleSort();
    A.selectSort();
//    A.rankSort();
//    A.insertSort();
    cout << A << endl;
    return 0;
}


template<class T>
void ArraySort<T>::rankSort() {
    int *rank = new int[size]{0};   //rank数组初始化为0

    //给数组元素排名次
    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size; ++j) {
            if (arr[i] <= arr[j]) rank[j]++;
            else rank[i]++;
        }
    }

    //原地重排的名词排序
    for (int i = 0; i < size; ++i) {
        //当一个元素的名次rank与他当前的位置不符时，交换该位置的元素与rank位置的元素，并交换对应rank数组的值
        while (rank[i] != i) {
            swap(arr[i], arr[rank[i]]);
            swap(rank[i], rank[rank[i]]);
        }
    }

    delete[]rank;   //释放rank的空间
}

template<class T>
void ArraySort<T>::bubbleSort() {
    bool sorted = false;
    for (int i = 0; !sorted && i < size - 1; ++i) {
        sorted = true;  //默认数组有序
        for (int j = 0; j < size - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                sorted = false; //若进行了交换，则该数组无序
            }
        }
    }
}

template<class T>
void ArraySort<T>::selectSort() {
    bool sorted = false;
//    int indexOfMax = 0;
    for (int i = size; !sorted && i > 1; --i) {
        int indexOfMax = 0;
        sorted = true;  //默认数组有序
        for (int j = 1; j < i; ++j) {
            //若数组有序，则j每后移一次，indexOfMax都会更新，若有一次不更新，都说明该数组无序
            if (arr[indexOfMax] < arr[j]) {
                indexOfMax = j;
            } else sorted = false;
        }
        swap(arr[indexOfMax], arr[i - 1]);  //交换无序区的最后一个元素与最大元素
    }

}

template<class T>
void ArraySort<T>::insertSort() {
    for (int i = 1; i < size; ++i) { //i表示有序数组已有的元素个数
        T temp = arr[i];
        int j = i;

        //当temp<arr[j]时，就将arr[j]后移一位，并继续向前试探，直到arr[j]>=temp或j=0为止
        while (j > 0 && temp < arr[j - 1]) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = temp;
    }
}

template<class T>
ostream &operator<<(ostream &out, ArraySort<T> &a) {
    copy(a.arr, a.arr + a.size, ostream_iterator<T>(out, " "));
    return out;
}
