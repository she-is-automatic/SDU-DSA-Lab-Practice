// 首先创建队列类，采用数组描述；实现卡片游戏，假设桌上有一叠扑克牌，依次编号为1-n（从最上面开始）。
// 当至少还有两张的时候，可以进行操作：把第一张牌扔掉，然后把新的第一张放到整叠牌的最后。输入n，输出最后剩下的牌。
// 输入：一个整数n，代表一开始卡片的总数。
// 输出：最后一张卡片的值。

#include <iostream>

using namespace std;

template<class T>
class arrayQueue {
public:
    explicit arrayQueue(int initialCapacity = 10);
    ~arrayQueue() { delete[] queue; }

    // ADT
    bool empty() const { return theFront == theBack; }
    int size() const { return (theBack - theFront + arrayLength) % arrayLength; }
    T &front(); // 队列头
    T &back();  // 队列尾
    void pop(); // 队列首元素出队列
    void push(const T &theElement); //将新元素添加到队列尾

private:
    int theFront;   // 队列首元素的前一位
    int theBack;    // 队列尾元素
    int arrayLength;// 一维数组的容量
    T *queue;       // 队列的数组表示
};

template<class T>
arrayQueue<T>::arrayQueue(int initialCapacity) {
    if (initialCapacity < 1) {
        throw invalid_argument("Initial capacity Must be > 0.");
    }
    arrayLength = initialCapacity;
    queue = new T[arrayLength];
    theFront = 0;
    theBack = 0;
}

template<class T>
void arrayQueue<T>::push(const T &theElement) {
    if ((theBack + 1) % arrayLength == theFront) {  //若队列满，则扩容
        T *newQueue = new T[2 * arrayLength];
        int start = (theFront + 1) % arrayLength;   //数组第一个元素的位置

        if (start < 2)  //没有形成环，直接复制
            copy(queue + start, queue + start + arrayLength - 1, newQueue);
        else {          //形成环，分两段复制
            copy(queue + start, queue + arrayLength, newQueue);
            copy(queue, queue + theBack + 1, newQueue + arrayLength - start);
        }
        // 改变其他参数
        theFront = 2 * arrayLength - 1;
        theBack = arrayLength - 2;
        arrayLength *= 2;
        delete[]queue;
        queue = newQueue;
    }

    // 在队列尾插入元素
    theBack = (theBack + 1) % arrayLength;
    queue[theBack] = theElement;
}

template<class T>
T &arrayQueue<T>::back() {
    if (theFront == theBack)
        throw invalid_argument("Queue Empty.");
    return queue[theBack];
}

template<class T>
T &arrayQueue<T>::front() {
    if (theFront == theBack)
        throw invalid_argument("Queue Empty.");
    return queue[(theFront + 1) % arrayLength];
}

template<class T>
void arrayQueue<T>::pop() {
    if (theFront == theBack)
        throw invalid_argument("Queue Empty.");
    theFront = (theFront + 1) % arrayLength;
    queue[theFront].~T();   // 队列首元素析构
}

int play(int n) {
    arrayQueue<int> cards;
    for (int i = 0; i < n; ++i) // 将n张牌入队列
        cards.push(i + 1);

    while (cards.size() >= 2) {
        cards.pop();    //扔掉第一张牌
        cards.push(cards.front());  //第一张牌放到最后
        cards.pop();    //扔掉第一张牌
    }
    return cards.front();
}

int main(){
    int n;
    cin >> n;
    cout << play(n) << endl;
    return 0;
}