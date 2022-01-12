#include <iostream>

using namespace std;

template<class T>
class twoStacks {
    T *stack;
    int arrayLength;
    int top1;
    int top2;
public:
    explicit twoStacks(int initialCapacity = 20);
    twoStacks(const twoStacks<T> &);
    ~twoStacks(){ delete[]stack; }

    // ADT
    bool empty(int which);
    int size(int which);
    T &top(int which);
    void push(int which, const T &value);
    void pop(int which);

    //检查传入的栈的编号是否为1或2
    void checkStackNumber(int which);

};

template<class T>
void push1(bool num, const T &theElement);

template<class T>
twoStacks<T>::twoStacks(int initialCapacity) {
    arrayLength = initialCapacity;
    stack = new T[initialCapacity];
    top1 = -1;
    top2 = arrayLength;
}

template<class T>
twoStacks<T>::twoStacks(const twoStacks<T> &a) {
    arrayLength = a.arrayLength;
    top1 = a.top1;
    top2 = a.top2;
    stack = new T[arrayLength];
    copy(a.stack, a.stack + arrayLength, stack);
}

template<class T>
bool twoStacks<T>::empty(int which) {
    checkStackNumber(which);
    if (which == 1) return top1 == -1;
    else return top2 == arrayLength;
}

template<class T>
int twoStacks<T>::size(int which) {
    checkStackNumber(which);
    if (which == 1) return top1 + 1;
    else return arrayLength - top2;
}

template<class T>
T &twoStacks<T>::top(int which) {
    checkStackNumber(which);
    if (empty(which)) throw invalid_argument("Stack empty.");
    if (which == 1)return stack[top1];
    else return stack[top2];
}

template<class T>
void twoStacks<T>::push(int which, const T &value) {
    checkStackNumber(which);
    if (top1 + 1 == top2) { //数组满，扩容
        T *temp = new T[arrayLength * 2];
        copy(stack, stack + top1 + 1, temp);
        copy_backward(stack + top2, stack + arrayLength, temp + 2 * arrayLength);
        top2 += arrayLength;
        arrayLength *= 2;
        delete[]stack;
        stack = temp;
    }
    if (which==1) stack[++top1] = value;
    else stack[--top2] = value;
}

template<class T>
void twoStacks<T>::checkStackNumber(int which) {
    if (which != 1 && which != 2)
        throw invalid_argument("Stack number must be 1 or 2.");
}

template<class T>
void twoStacks<T>::pop(int which) {
    checkStackNumber(which);
    if (empty(which)) throw invalid_argument("Stack empty.");
    if (which == 1) stack[top1--].~T();
    else stack[top2++].~T();
}

int main(){
    twoStacks<int> a;

    for (int i = 0; i < 100; ++i) {
        a.push(1, i);
        a.push(2, i);
    }
    cout << a.top(1) << endl;
    a.pop(1);
    cout << a.top(1) << endl;
    cout << a.top(2) << endl;
    cout << a.size(1) << " " << a.size(2) << endl;

    a.pop(2);
    cout << a.top(1) << endl;
    cout << a.top(2) << endl;
    cout << a.size(1) << " " << a.size(2) << endl;
    return 0;
}