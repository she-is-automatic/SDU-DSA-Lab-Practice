//输入：第一行两个整数 N 和 M；
//     第二行 N 个整数，代表第一组整数序列；
//     第三行 M 个整数，代表第二组整数序列。
//输出：三行整数。分别代表第一组数、第二组数对应的有序链表与合并后有序链表的索引与元素的异或和。
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

template<class T>
struct chainNode {  //链表节点
    T element;
    chainNode<T> *next;

    chainNode() = default;

    explicit chainNode(const T &element) { this->element = element; }

    chainNode(const T &element, chainNode<T> *next) {
        this->element = element;
        this->next = next;
    }
};


template<class T>
class chain {
protected:
    void checkIndex(int theIndex) const;    // 检查索引是否越界
    chainNode<T> *firstNode;  // 链表头指针
    int listSize;             // 元素个数
public:

    explicit chain(int initialCapacity = 10);

    chain(const chain<T> &);

    ~chain();

    // ADT
    bool empty() const { return listSize == 0; }

    int size() const { return listSize; }

    T &get(int theIndex) const;

    int indexOf(const T &theElement) const;

    void erase(T &theElement);

    void insert(int theIndex, const T &theElement);

    void output(ostream &out) const;

    void insertSort();

    int xorSum();

    // 链表迭代器的首尾
    class iterator;

    iterator begin() { return iterator(firstNode); }

    iterator end() { return iterator(NULL); }

    // 链表迭代器
    class iterator {
    public:
        // 声明前向迭代器
        typedef forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;

        explicit iterator(chainNode<T> *theNode = NULL) { node = theNode; }

        T &operator*() const { return node->element; }
        T *operator->() const { return &node->element; }
        iterator &operator++() {
            node = node->next;
            return *this;
        }
        iterator operator++(int) {
            iterator old = *this;
            node = node->next;
            return old;
        }
        bool operator!=(const iterator right) const { return node != right.node; }
        bool operator==(const iterator right) const { return node == right.node; }

    protected:
        chainNode<T> *node;
    };  // end of iterator class

};

template<class T>
ostream &operator<<(ostream &out, const chain<T> &x) {
    x.output(out);
    return out;
}

template<class T>
chain<T>::chain(int initialCapacity) {
    // 若容量<1，则抛出异常
    if (initialCapacity < 1) {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0";
        throw invalid_argument(s.str());
    }
    // 链表初始化为空
    firstNode = NULL;
    listSize = 0;
}

template<class T>
chain<T>::chain(const chain<T> &theList) {
    listSize = theList.listSize;
    // 若链表为空，则抛出异常
    if (listSize == 0) {
        firstNode = NULL;
        throw invalid_argument("The Chain shouldn't be empty.");
    }

    //复制链表
    chainNode<T> *sourceNode = theList.firstNode;
    firstNode = new chainNode<T>(sourceNode->element);
    sourceNode = sourceNode->next;
    chainNode<T> *targetNode = firstNode;

    while (sourceNode != NULL) {
        targetNode->next = new chainNode<T>(sourceNode->element);
        targetNode = targetNode->next;
        sourceNode = sourceNode->next;
    }
    targetNode->next = NULL;
}

template<class T>
chain<T>::~chain() {
    chainNode<T> *nextNode;
    // 依次释放节点空间
    while (firstNode != NULL) {
        nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

template<class T>
void chain<T>::checkIndex(int theIndex) const {
    // 若索引越界，则抛出异常
    if (theIndex < 0 || theIndex >= listSize) {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw invalid_argument(s.str());
    }
}

template<class T>
T &chain<T>::get(int theIndex) const {
    // 检查索引是否越界
    checkIndex(theIndex);

    // 遍历链表，找到要求的元素
    chainNode<T> *currentNode = firstNode;
    for (int i = 0; i < theIndex; i++)
        currentNode = currentNode->next;
    return currentNode->element;
}

template<class T>
int chain<T>::indexOf(const T &theElement) const {
    chainNode<T> *currentNode = firstNode;
    int index = 0;
    while (currentNode != NULL &&
           currentNode->element != theElement) {
        currentNode = currentNode->next;
        index++;
    }

    if (currentNode == NULL) return -1;
    else return index;
}

template<class T>
void chain<T>::insert(int theIndex, const T &theElement) {
    // 检查索引是否越界
    if (theIndex < 0 || theIndex > listSize) {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw invalid_argument(s.str());
    }

    if (theIndex == 0)  // 若插入节点为头节点，则头节点前移
        firstNode = new chainNode<T>(theElement, firstNode);
    else {
        chainNode<T> *p = firstNode;
        for (int i = 0; i < theIndex - 1; i++)
            p = p->next;
        p->next = new chainNode<T>(theElement, p->next);
    }
    listSize++;
}

template<class T>
void chain<T>::output(ostream &out) const {
    // 遍历链表，输出元素值
    for (chainNode<T> *currentNode = firstNode;
         currentNode != NULL;
         currentNode = currentNode->next)
        out << currentNode->element << "  ";
}

template<class T>
void chain<T>::erase(T &theElement) {

    chainNode<T> *deleteNode;
    chainNode<T> *p = firstNode;
    //若要删除的节点为头节点，则头节点后移
    if (firstNode->element == theElement) {
        deleteNode = firstNode;
        firstNode = firstNode->next;
    } else {    //若不是头节点，则删除节点前一节点的next指向删除节点的next
        while (p->next != NULL && p->next->element != theElement)
            p = p->next;

        if (p->next == NULL) { //不存在此元素，输出-1返回
            cout << -1 << endl;
            return;
        }

        //存在此元素
        deleteNode = p->next;
        p->next = p->next->next;
    }
    // 删除待删除节点，链表长度-1
    listSize--;
    delete deleteNode;
}

template<class T>
void chain<T>::insertSort() {
    //链表中有小于1个节点，不排序直接返回
    if (listSize <= 1) return;

    // 取下链表的头，作为有序链表的开始
    chainNode<T> *p = firstNode->next;
    firstNode->next = NULL;

    // 将无序区的第一个元素从前向后插入有序区中
    while (p != NULL) {
        chainNode<T> *nextNode = p->next;
        if (p->element < firstNode->element) {
            p->next = firstNode;
            firstNode = p;
        } else {
            chainNode<T> *q = firstNode;
            while (q->next != NULL && q->next->element <= p->element) {
                q = q->next;
            }
            p->next = q->next;
            q->next = p;
        }
        p = nextNode;
    }
}

template<class T>
int chain<T>::xorSum() {
    int n = 0, ret = 0;
    for (chain<T>::iterator it = this->begin(); it != end(); ++it) {
        ret += (n ^ *it);
        ++n;
    }
    return ret;
}

template<class T>
chain<T> merge(chain<T> &a, chain<T> &b){
    //传入链表有序
    chain<T> c;
    typename chain<T>::iterator ia = a.begin();
    typename chain<T>::iterator ib = b.begin();

    int i = 0;

    while (ia != a.end() && ib != b.end()) {
        if (*ia <= *ib) {
            c.insert(i,*ia);
            ++ia;
        }
        else{
            c.insert(i, *ib);
            ++ib;
        }
        i++;
    }

    // 复制剩下的元素
    while (ia != a.end()) {
        c.insert(i, *ia);
        ++ia;
        ++i;
    }
    while (ib != b.end()) {
        c.insert(i, *ib);
        ++ib;
        ++i;
    }
    return c;
}

int main() {
    int n, m;
    cin >> n >> m;
    chain<int> a, b;

    // 输入链表
    for (int i = 0; i < n; ++i) {
        int val;
        cin >> val;
        a.insert(i, val);
    }
    for (int i = 0; i < m; ++i) {
        int val;
        cin >> val;
        b.insert(i, val);
    }

    // 排序
    a.insertSort();
    b.insertSort();
    cout << a.xorSum() << endl;
    cout << b.xorSum() << endl;
    chain<int> c = merge(a, b);
    cout << c.xorSum() << endl;

    return 0;
}
