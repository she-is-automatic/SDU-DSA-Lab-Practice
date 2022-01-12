// 输入: 一串小写字母组成的字符串（不超过1000000)。
// 输出: 输出这个字符串通过Huffman编码后的长度。

#include <iostream>
#include <algorithm>
#include <iterator>
#include <queue>
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
struct binaryTreeNode { //二叉树节点
    T element;  //节点元素值
    binaryTreeNode<T> *leftChild, *rightChild;  //左子树和右子树

    binaryTreeNode(){ leftChild = rightChild - NULL; }
    explicit binaryTreeNode(const T &theElement) : element(theElement) {
        leftChild = rightChild = NULL;
    }
    binaryTreeNode(const T &theElement, binaryTreeNode<T> *theLeftChild, binaryTreeNode<T> *theRightChild)
            :element(theElement),leftChild(theLeftChild),rightChild(theRightChild){}
};

template<class T>
class linkedBinaryTree {  // 链表描述的二叉树
public:
    linkedBinaryTree(){root = nullptr; treeSize = 0;}
    ~linkedBinaryTree(){erase();}
    bool empty(){ return treeSize == 0; }
    int size(){ return treeSize; }


    void erase(){
        postOrder(dispose);
        root = nullptr;
        treeSize = 0;
    }
    int height(){return height(root);}//计算树的高度
    int nodeAmount(){return nodeAmount(root);}  //节点个数
    int maxLevel(){ return maxLevel(root); }

    void calculateLength(T *len){ calculateLength(len, root); }

    void makeTree(const T &element, linkedBinaryTree<T> &leftTree, linkedBinaryTree<T> &rightTree);
    void preOrderOutput(){ preOrder(output);cout << endl; }
    void inOrderOutput(){ inOrder(output);cout << endl; }
    void postOrderOutput(){ postOrder(output);cout << endl; }
    void levelOrderOutput(){ levelOrder(output);cout << endl; }


    void preOrder(void (*theVisit)(binaryTreeNode<T>*))
    {visit = theVisit;preOrder(root);}
    void inOrder(void (*theVisit)(binaryTreeNode<T>*))
    {visit = theVisit;inOrder(root);}
    void postOrder(void (*theVisit)(binaryTreeNode<T>*))
    {visit = theVisit;postOrder(root);}
    void levelOrder(void(*theVisit)(binaryTreeNode<T>*))
    {visit=theVisit;levelOrder(root);}

private:
    binaryTreeNode<T> *root;    //根节点
    int treeSize;               //树的节点个数
    static void (*visit)(binaryTreeNode<T> *); //访问函数
    static void preOrder(binaryTreeNode<T> *t);
    static void inOrder(binaryTreeNode<T> *t);
    static void postOrder(binaryTreeNode<T> *t);
    static void dispose(binaryTreeNode<T>*t){ delete t; }
    static void output(binaryTreeNode<T>*t){cout<<t->element<<" ";}
    static void levelOrder(binaryTreeNode<T>*t);
    static int height(binaryTreeNode<T> *t);
    static int nodeAmount(binaryTreeNode<T> *t);
    static int maxLevel(binaryTreeNode<T> *t);
    static void calculateLength(T *len, binaryTreeNode<T> *t);
};

template<class T>
linkedBinaryTree<T> *huffmanTree(T weight[], int n);

template<class T>
class huffmanNode {
    friend linkedBinaryTree<int> *huffmanTree(T *a, int n);

public:
    operator T()const{ return weight; }

    linkedBinaryTree<int> *tree;
    T weight;
};


template<class T>
void (*linkedBinaryTree<T>::visit)(binaryTreeNode<T>*);


template<class T>
void linkedBinaryTree<T>::preOrder(binaryTreeNode<T> *t) {
    if (t != nullptr) {
        linkedBinaryTree<T>::visit(t);
        preOrder(t->leftChild);
        preOrder(t->rightChild);
    }
}

template<class T>
void linkedBinaryTree<T>::inOrder(binaryTreeNode<T> *t) {
    if (t != nullptr) {
        inOrder(t->leftChild);
        linkedBinaryTree<T>::visit(t);
        inOrder(t->rightChild);
    }
}

template<class T>
void linkedBinaryTree<T>::postOrder(binaryTreeNode<T> *t) {
    if (t != nullptr) {
        postOrder(t->leftChild);
        postOrder(t->rightChild);
        linkedBinaryTree<T>::visit(t);
    }
}

template<class T>
int linkedBinaryTree<T>::height(binaryTreeNode<T> *t) {
    if (t==NULL) return 0;
    int hl = height(t->leftChild);  //计算左子树高度
    int hr = height(t->rightChild); //计算右子树高度
    if (hl > hr) return ++hl;
    else return ++hr;
}

template<class T>
int linkedBinaryTree<T>::nodeAmount(binaryTreeNode<T> *t) {
    if (t==NULL) return 0;
    return height(t->leftChild) + height(t->rightChild) + 1;    //左子树节点个数+右子树节点个数+1
}

template<class T>
void linkedBinaryTree<T>::levelOrder(binaryTreeNode<T> *t) {
    queue<binaryTreeNode<T>*> q;
    while (t != nullptr) {
        visit(t);

        //t的孩子入队列
        if (t->leftChild != nullptr)
            q.push(t->leftChild);
        if (t->rightChild != nullptr)
            q.push(t->rightChild);

        if (q.empty()) return;
        t = q.front();
        q.pop();
    }
}

template<class T>
int linkedBinaryTree<T>::maxLevel(binaryTreeNode<T> *t) {
    if (t==nullptr) return -1; //空树

    int curNum = 1, maxNum = 1, maxLevel = 1, curLevel = 1;
    queue<binaryTreeNode<T>*>q;
    q.push(t);  //根节点入队列
    while (!q.empty()) {
        while (curNum > 0) {    //将下一层的所有节点入队列，求出下一层的节点数，并将本层节点依次出队列
            t = q.front();
            if (t->leftChild != nullptr)
                q.push(t->leftChild);

            if (t->rightChild != nullptr)
                q.push(t->rightChild);

            q.pop();
            --curNum;
        }

        ++curLevel; //进入下一级
        curNum = q.size();  //下一级的节点数
        if (curNum > maxNum) {
            maxLevel = curLevel;
            maxNum = curNum;
        }
    }
    return maxLevel;
}

template<class T>
void linkedBinaryTree<T>::makeTree(const T &element, linkedBinaryTree<T> &leftTree, linkedBinaryTree<T> &rightTree) {
    root = new binaryTreeNode<T> (element, leftTree.root, rightTree.root);
    treeSize = leftTree.treeSize + rightTree.treeSize + 1;

    leftTree.root = rightTree.root = nullptr;
    leftTree.treeSize = rightTree.treeSize = 0;
}

template<class T>
void linkedBinaryTree<T>::calculateLength(T *len, binaryTreeNode<T> *t) {
    if (t==nullptr) return; //空树

    int curNum = 1, curLevel = 1;
    queue<binaryTreeNode<T>*>q;
    q.push(t);  //根节点入队列
    while (!q.empty()) {
        while (curNum > 0) {    //将下一层的所有节点入队列，求出下一层的节点数，并将本层节点依次出队列
            t = q.front();
            len[t->element] = curLevel - 1; // 层数-1即为该元素应当编码的位数

            if (t->leftChild != nullptr)
                q.push(t->leftChild);

            if (t->rightChild != nullptr)
                q.push(t->rightChild);

            q.pop();
            --curNum;
        }

        ++curLevel; //进入下一级
        curNum = q.size();  //下一级的节点数
    }
}


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

template<class T>
linkedBinaryTree<T> *huffmanTree(T weight[], int n){
    // 根据权值构造霍夫曼树
    huffmanNode<T> *hNode = new huffmanNode<T>[n + 1];
    linkedBinaryTree<int> emptyTree;
    for (int i = 1; i <= n; ++i) {
        hNode[i].weight = weight[i];
        hNode[i].tree = new linkedBinaryTree<int>;
        hNode[i].tree->makeTree(i, emptyTree, emptyTree);
    }

    // 单节点树放入小根堆
    minHeap<huffmanNode<T>> heap(1);
    heap.initialize(hNode, n);

    // 不断从小根堆中取树并合并，直到剩下一棵树
    huffmanNode<T> w, x, y;
    linkedBinaryTree<int> *z;
    for (int i = 1; i < n; ++i) {
        x = heap.top();heap.pop();
        y = heap.top();heap.pop();

        z=new linkedBinaryTree<int>;
        z->makeTree(0, *x.tree, *y.tree);
        w.weight = x.weight + y.weight;
        w.tree = z;
        heap.push(w);
        delete x.tree;
        delete y.tree;
    }
    return heap.top().tree;
}

int main(){
    string str;
    cin >> str;
    int weight[27] = {0};
    for (int i = 0; i < str.size(); ++i) {
        ++weight[str.at(i) - 'a' + 1];
    }
    int index = 1;
    for (int i = 1; i <= 26; ++i) {
        if (weight[i] != 0) {
            weight[index++] = weight[i];
        }
    }
    linkedBinaryTree<int> tree = *huffmanTree(weight, index-1);


    int *len = new int[index];
    tree.calculateLength(len);

    int totalLength = 0;
    for (int i = 1; i < index; ++i) {
        totalLength += weight[i] * len[i];
    }
    cout << totalLength << endl;

    return 0;
}