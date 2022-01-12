// maxHeap

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
class maxHeap
{
public:
    maxHeap(int initialCapacity = 10);
    ~maxHeap() {delete [] heap;}
    bool empty() const {return heapSize == 0;}
    int size() const
    {return heapSize;}
    const T& top()
    {// return max element
        if (heapSize == 0)
            throw invalid_argument("Queue Empty");
        return heap[1];
    }
    void pop();
    void push(const T&);
    void initialize(T *, int);
    void deactivateArray()
    {heap = NULL; arrayLength = heapSize = 0;}
    void output(ostream& out) const;

    void changeMax(const T &newElement);    //改变最大元素
    T remove(int i);    //移除并返回第i个元素

private:
    int heapSize;       // number of elements in queue
    int arrayLength;    // queue capacity + 1
    T *heap;            // element array
};

template<class T>
maxHeap<T>::maxHeap(int initialCapacity)
{// Constructor.
    if (initialCapacity < 1)
        throw invalid_argument("Initial capacity Must be > 0.");

    arrayLength = initialCapacity + 1;
    heap = new T[arrayLength];
    heapSize = 0;
}

template<class T>
void maxHeap<T>::push(const T& theElement)
{// Add theElement to heap.

    // increase array length if necessary
    if (heapSize == arrayLength - 1)
    {// double array length
        changeLength1D(heap, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }

    // find place for theElement
    // currentNode starts at new leaf and moves up tree
    int currentNode = ++heapSize;
    while (currentNode != 1 && heap[currentNode / 2] < theElement)
    {
        // cannot put theElement in heap[currentNode]
        heap[currentNode] = heap[currentNode / 2]; // move element down
        currentNode /= 2;                          // move to parent
    }

    heap[currentNode] = theElement;
}

template<class T>
void maxHeap<T>::pop()
{// Remove max element.
    // if heap is empty return null
    if (heapSize == 0)   // heap empty
        throw invalid_argument("Queue Empty");

    // Delete max element
    heap[1].~T();

    // Remove last element and reheapify
    T lastElement = heap[heapSize--];

    // find place for lastElement starting at root
    int currentNode = 1,
            child = 2;     // child of currentNode
    while (child <= heapSize)
    {
        // heap[child] should be larger child of currentNode
        if (child < heapSize && heap[child] < heap[child + 1])
            child++;

        // can we put lastElement in heap[currentNode]?
        if (lastElement >= heap[child])
            break;   // yes

        // no
        heap[currentNode] = heap[child]; // move child up
        currentNode = child;             // move down a level
        child *= 2;
    }
    heap[currentNode] = lastElement;
}

template<class T>
void maxHeap<T>::initialize(T *theHeap, int theSize)
{// Initialize max heap to element array theHeap[1:theSize].
    delete [] heap;
    heap = theHeap;
    heapSize = theSize;
    arrayLength = theSize + 1;

    // heapify
    for (int root = heapSize / 2; root >= 1; root--)
    {
        T rootElement = heap[root];

        // find place to put rootElement
        int child = 2 * root; // parent of child is target
        // location for rootElement
        while (child <= heapSize){
            // heap[child] should be larger sibling
            if (child < heapSize && heap[child] < heap[child + 1])
                child++;

            // can we put rootElement in heap[child/2]?
            if (rootElement >= heap[child])
                break;  // yes

            // no
            heap[child / 2] = heap[child]; // move child up
            child *= 2;                    // move down a level
        }
        heap[child / 2] = rootElement;
    }
}

template<class T>
void maxHeap<T>::output(ostream& out) const
{// Put the list into the stream out.
    copy(heap + 1, heap + heapSize + 1, ostream_iterator<T>(cout, "  "));
}

template<class T>
void maxHeap<T>::changeMax(const T &newElement) {
    int currentNode = 1, child = 2;
    while (child <= heapSize) {
        if (child < heapSize && heap[child] < heap[child + 1]) { //child是大孩子
            ++child;
        }

        if (heap[child] < newElement)
            break;
        else{
            heap[currentNode] = heap[child];
            currentNode = child;
            child *= 2;
        }
    }
    heap[currentNode] = newElement;
}

template<class T>
T maxHeap<T>::remove(int i) {
    if (i>heapSize) //超过索引，抛出异常
        throw invalid_argument("Out of range");

    T removeElement = heap[i];
    T lastElement = heap[heapSize];
    heap[heapSize--].~T();  //析构最后一个元素，给其在堆中寻找合适的位置
    int currentNode = i;

    // 若i是最后一个元素 或 最后一个元素的元素值小于等于第i个元素的父节点的元素值，则向下探查，为其寻找插入位置
    if (i == 1 || (i > 1 && lastElement <= heap[i / 2])) {
        int child = 2 * i;
        while (child <= heapSize) {
            if (child < heapSize && heap[child] < heap[child + 1]) //child是大孩子
                ++child;
            if (heap[child] < lastElement) break;

            heap[currentNode] = heap[child];
            currentNode = child;
            child *= 2;
        }
    }

        // 若最后一个元素的元素值大于第i个元素的父节点的元素值，则向上探查，为其寻找插入位置
    else {
        while (currentNode > 1 && heap[currentNode / 2] < lastElement) {
            heap[currentNode] = heap[currentNode / 2];
            currentNode /= 2;
        }
    }
    heap[currentNode] = lastElement;
    return removeElement;
}

// overload <<
template <class T>
ostream& operator<<(ostream& out, const maxHeap<T>& x)
{x.output(out); return out;}

int main(){
    maxHeap<int> a;
    int max[14] = {0, 20, 11, 19, 10, 9, 18, 17, 5, 4, 3, 2, 15, 16};
    a.initialize(max, 13);
    cout << a << endl;
//    a.changeMax(16);
    a.remove(4);
    cout << a << endl;
    return 0;
}
