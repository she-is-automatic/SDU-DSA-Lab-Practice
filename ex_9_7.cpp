#include <iostream>

using namespace std;

template<class T>
class arrayQueue {
public:
    explicit arrayQueue(int initialCapacity = 10);
    ~arrayQueue() { delete[] queue; }
    bool empty() const{ return queueSize == 0; };
    int size() const{ return queueSize; };
    T &front() {
        if (queueSize == 0)
            throw invalid_argument("Queue Empty.");

        return queue[theFront % arrayLength];
    }
    T &back() {
        if (queueSize == 0)
            throw invalid_argument("Queue Empty.");
        return queue[(theFront + queueSize - 1 + arrayLength) % arrayLength];
    }

    void pop();
    void push(const T &theElement);

private:
    int theFront;       // 队列首元素
    int queueSize;      // 队列长度
    int arrayLength;    // 数组容量
    T *queue;           // 一维数组
};

template<class T>
arrayQueue<T>::arrayQueue(int initialCapacity){
    if (initialCapacity < 1)
        throw invalid_argument("Initial capacity Must be > 0.");
    arrayLength = initialCapacity;
    queue = new T[arrayLength];
    theFront = 0;
    queueSize = 0;
}

template<class T>
void arrayQueue<T>::pop() {
    if (queueSize == 0)
        throw invalid_argument("Queue Empty.");
    queue[theFront].~T();
    theFront = (theFront + 1) % arrayLength;
    --queueSize;
}

template<class T>
void arrayQueue<T>::push(const T &theElement) {
    if (queueSize == arrayLength) { //数组满，扩容
        T *newQueue = new T[2 * arrayLength];
        if (theFront == 0) {    // 没有形成环，直接复制
            copy(queue, queue + queueSize, newQueue);
        }
        else{   //形成环，分两段复制
            copy(queue + theFront, queue + arrayLength, newQueue);
            copy(queue, queue + theFront, newQueue + arrayLength - theFront);
        }

        delete []queue;
        queue = newQueue;
        arrayLength *= 2;
        theFront = 0;
    }
    queue[(theFront + queueSize) % arrayLength] = theElement;
    ++queueSize;
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
//    a.output();
    //    cout << endl;
        for (int i = 0; i < 100; ++i) {
            a.pop();
        }
        cout<<a.empty()<<endl;
        cout << a.size() << endl;


}