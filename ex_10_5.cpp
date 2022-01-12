//  SortedArrayList

#include <iostream>

using namespace std;

template<class T>
void changeLength1D(T *&a, int oldLength, int newLength) {
    if (newLength < 0)
        throw invalid_argument("new length must be >= 0");

    T *temp = new T[newLength];
    int number = min(oldLength, newLength);
    copy(a, a + number, temp);
    delete[] a;
    a = temp;
}

template<class K, class E>
class sortedArrayList {
    pair<K, E> *element;   // 存放pair的数组
    int arrayLength;        // 数组容量
    int listSize;           // 有效数对的个数

public:
    explicit sortedArrayList(int initialCapacity = 10);

    ~sortedArrayList() { delete[]element; }

    // ADT
    bool empty() const { return listSize == 0; }
    int size() const { return listSize; }
    pair<K, E> *find(const K &) const;
    void insert(const pair<K, E> &);
    void erase(const K &theKey);

    void output() {
        for (int i = 0; i < listSize; ++i) {
            cout << element[i].first << ":\t" << element[i].second << endl;
        }
    }
};

template<class K, class E>
sortedArrayList<K, E>::sortedArrayList(int initialCapacity) {
    if (initialCapacity < 0)
        throw invalid_argument("Initial Capacity Must be > 0.");
    arrayLength = initialCapacity;
    listSize = 0;
    element = new pair<K, E>[arrayLength];
}

template<class K, class E>
pair<K, E> *sortedArrayList<K, E>::find(const K &theKey) const {
    // 折半查找
    int start = 0, end = listSize - 1;
    while (true) {
        if (start > end) return nullptr;
        else {
            int mid = (start + end) / 2;
            if (element[mid].first == theKey) return &element[mid];
            else if (element[mid].first < theKey) start = mid + 1;
            else end = mid - 1;
        }
    }
}

template<class K, class E>
void sortedArrayList<K, E>::insert(const pair<K, E> &thePair) {
    int i;
    for (i = 0; i < listSize && element[i].first < thePair.first; ++i);

    // 若数组中已存在，则覆盖
    if (i < listSize && element[i].first == thePair.first) {
        element[i].second = thePair.second;
        return;
    }

    // 若数组中不存在，则插入
    if (listSize == arrayLength) {  //数组满，扩容
        changeLength1D(element, arrayLength, arrayLength * 2);
        arrayLength *= 2;
    }
    copy_backward(element + i, element + listSize, element + listSize + 1); // 后移一位
    element[i] = thePair;
    ++listSize;
}

template<class K, class E>
void sortedArrayList<K, E>::erase(const K &theKey) {
    int i;
    for (i = 0; i < listSize && element[i].first < theKey; ++i);

    if (i < listSize && element[i].first == theKey) {  //找到要删除的元素
        copy(element + i + 1, element + listSize, element + i); //后边元素前移一位
        element[--listSize].~pair();
    }
}

int main() {
    sortedArrayList<int, int> a(1);

    pair<int, int> p;


    for (int i = 0; i < 15; ++i) {
        p.first = i + 1;
        p.second = 100 * p.first;
        a.insert(p);
    }

    a.erase(5);
    a.erase(4);

    p.first = 4;
    p.second = 50000;
    a.insert(p);

    a.output();

    pair<int, int> *fp = a.find(2);
    cout << fp->second << endl;
    cout << a.size() << endl;
    cout << a.empty();
}


