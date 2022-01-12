//第一行一个数n（n<=5000)，代表堆的大小；第二行n个数，代表堆的各个元素；第三行一个数m (m<=1000)，代表接下来共m个操作。接下来m行，分别代表各个操作。
//插入操作：1 num；
//删除操作：2；
//排序操作：第一行两个数3和n，3代表是排序操作，n代表待排序的数的数目，接下来一行n个数是待排序数。
//保证排序操作只出现一次且一定是最后一个操作。
//输出：
//第一行建堆操作输出建好堆后的堆顶的元素。接下来m个操作，若是插入和删除操作。每行输出执行操作后堆顶的元素的值；若是排序操作，输出一行按升序排序好的结果，每个元素间用空格分隔。

#include <iostream>
#include <algorithm>
#include <iterator>
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

template<class T>
class minHeap
{
public:
    explicit minHeap(int initialCapacity = 10);

    ~minHeap() {delete [] heap;}
    bool empty() const {return heapSize == 0;}
    int size() const
    {return heapSize;}
    const T& top()
    {// return min element
        if (heapSize == 0)
            throw invalid_argument("Queue Empty");
        return heap[1];
    }
    void pop();
    void push(const T&);
    void initialize(T *, int);

    void deactivateArray()
    {heap = NULL; arrayLength = heapSize = 0;}


private:
    int heapSize;       // 堆的大小
    int arrayLength;    // 数组容量
    T *heap;            // 数组
};

template<class T>
minHeap<T>::minHeap(int initialCapacity)
{// Constructor.
    if (initialCapacity < 1)
        throw invalid_argument("Initial capacity Must be > 0.");

    arrayLength = initialCapacity + 1;
    heap = new T[arrayLength];
    heapSize = 0;
}

template<class T>
void minHeap<T>::push(const T& theElement)
{
    if (heapSize == arrayLength - 1){ // 数组满，扩容
        changeLength1D(heap, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }

    // 找到该元素应该放置的位置
    int currentNode = ++heapSize;
    while (currentNode != 1 && heap[currentNode / 2] > theElement){
        heap[currentNode] = heap[currentNode / 2]; // 元素下移
        currentNode /= 2;
    }
    // 将该元素放在currentNode处
    heap[currentNode] = theElement;
}

template<class T>
void minHeap<T>::pop(){
    if (heapSize == 0)   // 堆空，抛出异常
        throw invalid_argument("Queue Empty");

    // 删除最大元素
    heap[1].~T();

    // 获取最后一个元素
    T lastElement = heap[heapSize--];

    // 最后一个元素寻找应该放置的位置
    int currentNode = 1, child = 2;
    while (child <= heapSize)
    {
        // child是小孩子
        if (child < heapSize && heap[child] > heap[child + 1])
            child++;

        if (lastElement <= heap[child])
            break;   // yes

        heap[currentNode] = heap[child];
        currentNode = child;
        child *= 2;
    }
    heap[currentNode] = lastElement;
}

template<class T>
void minHeap<T>::initialize(T *theHeap, int theSize){
    // 释放原来的数组空间
    delete [] heap;

    //heap指向传入的数组，并获得堆的大小
    heap = theHeap;
    heapSize = theSize;
    arrayLength = theSize + 1;

    // 建堆
    for (int root = heapSize / 2; root >= 1; root--){
        T rootElement = heap[root];

        int child = 2 * root;
        // 寻找可以放置rootElement的位置
        while (child <= heapSize){
            if (child < heapSize && heap[child] > heap[child + 1])  //child是较小的孩子
                child++;

            if (rootElement <= heap[child])
                break;

            heap[child / 2] = heap[child]; // 孩子上移
            child *= 2;
        }
        heap[child / 2] = rootElement;
    }
}

template<class T>
void heapSort(T *a, int n){
    minHeap<T>heap;
    heap.initialize(a, n);
    T *sortedArray = new T[n+1];
    for (int i = 1; i <= n; ++i) {
        T temp = heap.top();
        heap.pop();
        sortedArray[i] = temp;
    }
    copy(sortedArray, sortedArray + n + 1, a);
    delete[]sortedArray;
    heap.deactivateArray();
}


int main(){
    int n,m,op,element;
    cin>>n;

    int *arr = new int[n+1];
    for (int i = 1; i <= n; ++i) {
        cin >> arr[i];
    }
    minHeap<int>A;
    A.initialize(arr, n);
    cout<<A.top()<<endl;

    cin>>m;
    for ( int i = 0; i < m; ++i ) {
        cin>>op;
        switch(op){
            case 1:{
                cin>>element;
                A.push(element);
                cout<<A.top()<<endl;
                break;
            }
            case 2:{
                A.pop();
                cout<<A.top()<<endl;
                break;
            }
            case 3:{
                int num;
                cin>>num;
                int *min=new int [num+1];
                for ( int j = 1; j <=num ; ++j ) {
                    cin>>min[j];
                }
                heapSort(min,num);
                for (int j = 1; j <= num; ++j) {
                    cout << min[j]<<" ";
                }
            }
            default:
                break;

        }
    }
    return 0;
}
