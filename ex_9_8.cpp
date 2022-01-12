#include <iostream>

using namespace std;

template<class T>
class arrayQueue {
public:
    explicit arrayQueue(int initialCapacity = 10);
    ~arrayQueue() { delete[] queue; }
    bool empty() const;
    int size() const;
    T &front() {
        if (empty())
            throw invalid_argument("Queue Empty.");
        return queue[(theFront + 1) % arrayLength];
    }
    T &back() {
        if (empty())
            throw invalid_argument("Queue Empty.");
        return queue[theBack];
    }
    void pop();
    void push(const T &theElement);

private:
    int theFront;       // 队列首元素
    int theBack;        // 队列尾元素
    int arrayLength;    // 数组容量
    bool lastOp;        // 记录最后一次操作，false为删除，true为插入
    T *queue;           // 一维数组
};

template<class T>
arrayQueue<T>::arrayQueue(int initialCapacity) {
    if (initialCapacity < 1)
        throw invalid_argument("Initial capacity Must be > 0.");
    arrayLength = initialCapacity;
    queue = new T[arrayLength];
    theFront = 0;
    theBack = 0;
    lastOp = false;
}

template<class T>
void arrayQueue<T>::push(const T &theElement) {

    if (!empty() && theBack == theFront) {  //队列满，扩容

        T *newQueue = new T[2 * arrayLength];

        int start = (theFront + 1) % arrayLength;
        if (start == 0) //没有形成环，直接复制
            copy(queue + start, queue + arrayLength, newQueue);
        else {  // 形成环，分两部分复制
            copy(queue + start, queue + arrayLength, newQueue);
            copy(queue, queue + theBack + 1, newQueue + arrayLength - start);
        }

        // 改参数
        theFront = 2 * arrayLength - 1;
        theBack = arrayLength - 1;
        arrayLength *= 2;
        queue = newQueue;
    }

    theBack = (theBack + 1) % arrayLength;
    queue[theBack] = theElement;
    lastOp = true;
}

template<class T>
bool arrayQueue<T>::empty() const {
    return (theFront == theBack && lastOp == false);
}

template<class T>
void arrayQueue<T>::pop() {
    if (empty())
        throw invalid_argument("Queue Empty.");
    theFront = (theFront + 1) % arrayLength;
    queue[theFront].~T();
    lastOp = false;
}

template<class T>
int arrayQueue<T>::size() const {
    if (theFront == theBack && lastOp == true) return arrayLength;  //数组满
    return (theBack - theFront + arrayLength) % arrayLength;
}

int main(){
    arrayQueue<int>a;
    cout << a.empty() << endl;
    for (int i = 0; i < 100; ++i) {
        a.push(i + 1);
    }
    cout<<a.empty()<<endl;
    cout << a.size() << endl;
    cout << a.front() << endl;
    cout << a.back() << endl;
//    cout << endl;
//    for (int i = 0; i < 19; ++i) {
//        a.pop();
//    }
//    cout << a.front()<<endl;
//    cout<<a.empty()<<endl;
//    cout << a.size() << endl;
//    cout << a.front() << endl;
//    cout << a.back() << endl;

}