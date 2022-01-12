#include <iostream>
#include <sstream>

using namespace std;

template<class T>
class circularArrayList {
public:
    explicit circularArrayList(int initialCapacity); //构造函数

    circularArrayList(const circularArrayList<T> &);    //复制构造
    ~circularArrayList() { delete[]element; }    //析构函数

    //ADT方法
    //判断线性表是否为空
    bool empty() const { return first == -1; }

    //返回线性表的元素个数
    int size() const {
        if (first == -1) return 0;
        return (last - first + arrayLength) % arrayLength + 1;
    }

    //返回索引为index的元素
    T &get(int theIndex) const {
        checkIndex(theIndex);
        return element[(first + theIndex) % arrayLength];
    }

    //返回元素第一次出现的索引
    int indexOf(const T &theElement) const;

    //删除索引为index的元素
    void erase(int theIndex);

    //将元素插入index处
    void insert(int theIndex, const T &theElement);

    //返回一维数组的长度
    int capacity() { return arrayLength; }

    template<class U>
    friend ostream &operator<<(ostream &out, circularArrayList<U> &a);  //重载<<，输出数组的值

protected:
    void checkIndex(int theIndex) const;    //若索引无效，抛出异常

    T *element;         //数组
    int arrayLength;    //一维数组的容量
    int first;          //线性表首元素位置
    int last;           //线性表尾元素位置
};

template<class T>
void circularArrayList<T>::checkIndex(int theIndex) const {
    if (theIndex < 0 || theIndex >= size()) {
        ostringstream s;
        s << "index=" << theIndex << " size=" << size();
        throw invalid_argument(s.str());
    }
}

template<class T>
circularArrayList<T>::circularArrayList(int initialCapacity) {
    if (initialCapacity < 1) {
        ostringstream s;
        s << "Initial capacity=" << initialCapacity << " Must be > 0";
        throw invalid_argument(s.str());
    }
    arrayLength = initialCapacity;
    element = new T[initialCapacity]{0};
    first = -1;
    last = first;
}

template<class T>
circularArrayList<T>::circularArrayList(const circularArrayList<T> &theList) {
    arrayLength = theList.arrayLength;
    first = theList.first;
    last = theList.last;
    element = new T[arrayLength];
    copy(theList.element, theList.element + theList.arrayLength, element);
}

template<class T>
int circularArrayList<T>::indexOf(const T &theElement) const {
    int i = 0;
    for (i = 0; i < size() && element[(first + i) % arrayLength] != theElement; ++i);
    if (i == size()) return -1;
    else return i;
}

template<class T>
void circularArrayList<T>::erase(int theIndex) {
    checkIndex(theIndex);

 /*    for (int i = theIndex + 1; i < size(); ++i) {
        stack[(first + i - 1) % arrayLength] = stack[(first + i) % arrayLength];
    }
    //调用最后一个元素的析构函数，释放其空间
    stack[last].~T();
    last = (last + arrayLength - 1) % arrayLength;*/

    if (size() == 1) {
        element[first].~T();
        first = -1;
        last = -1;
        return;
    }
    int listSize = size();
    if (theIndex <= listSize/ 2) {// 要删除的元素索引在前半部分，将前半部分后移
        for (int i = theIndex - 1; i >= 0; i--)
            element[(first + i + 1) % arrayLength] = element[(first + i) % arrayLength];
        element[first].~T();    // 释放原来首位的空间
        first = (first + 1) % arrayLength;
    } else {// 要删除的在后半部分，将后半部分前移
        for (int i = theIndex + 1; i < listSize; i++)
            element[(first + i - 1) % arrayLength] = element[(first + i) % arrayLength];
        element[last].~T();   // 释放原来尾位的空间
        last = (arrayLength + last - 1) % arrayLength;
    }
}

template<class T>
void circularArrayList<T>::insert(int theIndex, const T &theElement) {
    //判断插入位置是否合法
    if (theIndex < 0 || theIndex > size()) {
        ostringstream s;
        s << "index=" << theIndex << " size=" << size();
        throw invalid_argument(s.str());
    }
    //若数组满，则扩大空间至2倍
    if (size() == arrayLength) {
        T *temp = new T[arrayLength * 2]{0};
        for (int i = 0; i < size(); ++i) {
            temp[(first + i) % (arrayLength * 2)] = element[(first + i) % arrayLength];
        }
        delete[]element;
        element = temp;
        last = first + size() - 1;
        arrayLength *= 2;
    }

    //优化之前，固定first
//     for (int i = size(); i > theIndex; --i) {
//        stack[(first + i) % arrayLength] = stack[(first + i - 1) % arrayLength];
//    }
//    stack[(first + theIndex) % arrayLength] = theElement;
//    last = (last + 1) % arrayLength;

    if (first == -1) { //数组空
        first = last = 0;
        element[0] = theElement;
        return;
    }

    int listSize = size();
    if (theIndex < listSize / 2) {  // 前半部分插入，first前移
        for (int i = 0; i < theIndex; ++i) {
            element[(first + i - 1 + arrayLength) % arrayLength] = element[(first + i) % arrayLength];
        }
        element[(first + theIndex - 1 + arrayLength) % arrayLength] = theElement;
        first = (first - 1 + arrayLength) % arrayLength;
    } else { //后半部分插入，last后移
        for (int i = size(); i > theIndex; --i) {
            element[(first + i) % arrayLength] = element[(first + i - 1) % arrayLength];
        }
        element[(first + theIndex) % arrayLength] = theElement;
        last = (last + 1) % arrayLength;
    }
}

template<class T>
ostream &operator<<(ostream &out, circularArrayList<T> &a) {
    for (int i = 0; i < a.size(); ++i) {
        out << a.element[(a.first + i) % a.arrayLength] << " ";
    }
    return out;
}


int main() {
    circularArrayList<int> a(5);
    a.insert(0, 1);
    cout << "a= " << a << endl;
    cout << "size = " << a.size() << endl;
    a.insert(0, 2);
    cout << "a= " << a << endl;
    cout << "size = " << a.size() << endl;
    a.insert(2, 0);
    cout << "a= " << a << endl;
    cout << "size = " << a.size() << endl;
//    a.erase(2);
//    cout << "a= " << a << endl;
//    cout << "size = " << a.size() << endl;
//
//    a.erase(1);
//    cout << "a= " << a << endl;
//    cout << "size = " << a.size() << endl;
//    a.erase(0);
//    cout << "a= " << a << endl;
//    cout << "size = " << a.size() << endl;

    return 0;
}
