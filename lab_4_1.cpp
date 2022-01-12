//第一行两个整数 N 和 Q。
//第二行 N 个整数，作为节点的元素值，创建链表。
//接下来 Q 行，执行各个操作，具体格式如下：
//插入操作 : 1 idx val，在链表的idx位置插入元素val;
//删除操作 : 2 val，删除链表中的 val 元素。若链表中存在多个该元素，仅删除第一个。若该元素不存在，输出 -1；
//逆置操作 : 3，原地逆置链表；
//查询操作 : 4 val，查询链表中的val元素，并输出其索引。若链表中存在多个该元素，仅输出第一个的索引。若不存在该元素，输出 -1；
//输出操作 : 5，使用链表迭代器，输出当前链表索引与元素的异或和。f(chain)=∑_(i=0)^(n-1)▒〖i⊕chain[i],n=len(chain)〗；
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

    void reverse();

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

        // increment
        iterator &operator++()
        {
            node = node->next;
            return *this;
        }

        iterator operator++(int)
        {
            iterator old = *this;
            node = node->next;
            return old;
        }

        // equality testing
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

    if (currentNode == NULL)    return -1;
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
void chain<T>::reverse() {
    // 将链表头取下，作为新的链表尾
    chainNode<T> *head = firstNode;
    firstNode = firstNode->next;
    head->next = NULL;

    // 每次循环将p指向新链表的头，新链表的头指向原链表的firstNode，
    // firstNode后移，head指向p作为新链表的头
    while (firstNode != NULL) {
        chainNode<T> *p = head;
        head = firstNode;
        firstNode = firstNode->next;
        head->next = p;
    }
    //将firstNode指向新链表
    firstNode = head;
}

template<class T>
void chain<T>::erase(T &theElement) {

    chainNode<T> *deleteNode;
    chainNode<T> *p = firstNode;
    //若要删除的节点为头节点，则头节点后移
    if (firstNode->element == theElement) {
        deleteNode = firstNode;
        firstNode = firstNode->next;
    }
    else {    //若不是头节点，则删除节点前一节点的next指向删除节点的next
        while(p->next!=NULL&&p->next->element!=theElement)
            p = p->next;

        if (p->next==NULL){ //不存在此元素，输出-1返回
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
int chain<T>::xorSum() {
    int n = 0, ret = 0;
    for (chain<T>::iterator it = this->begin(); it != end(); ++it) {
        ret += (n ^ *it);
        ++n;
    }
    return ret;
}

int main() {
    // 输入n、q
    int n, q;
    cin >> n >> q;

    // 创建n个节点的链表
    chain<int> a;
    for (int i = 0; i < n; ++i) {
        int element;
        cin >> element;
        a.insert(i, element);
    }

    //循环q次
    for (int i = 0; i < q; ++i) {
        int ch;
        cin >> ch;
        switch (ch) {
            case 1:{    //插入
                int index, val;
                cin >> index >> val;
                a.insert(index, val);
                break;
            }
            case 2:{    //删除
                int val;
                cin >> val;
                a.erase(val);
                break;
            }
            case 3:{    //逆置
                a.reverse();
                break;
            }
            case 4:{    //查询
                int val;
                cin >> val;
                cout << a.indexOf(val) << endl;
                break;
            }
            case 5:{    //输出异或和
                cout << a.xorSum() << endl;
                break;
            }
            default:break;
        }
    }

}
