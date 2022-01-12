// 给定散列函数的除数D和操作数m，输出每次操作后的状态。
// 插入x，若散列表已存在x，输出"Existed"
// 查询x，若散列表不含有x，输出"Not Found"，否则输出x所在的链表长度
// 删除x，若散列表不含有x，输出"Delete Failed"，否则输出x所在链表删除x后的长度

#include <iostream>

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

template<class K>
class sortedChain
{
public:
    sortedChain() {firstNode = NULL; dSize = 0;}
    ~sortedChain();

    bool empty() const {return dSize == 0;}
    int size() const {return dSize;}
    K * find(const K&) const;
    void erase(const K&);
    void insert(const K&);
    void output(ostream& out) const;

protected:
     chainNode<K>* firstNode;
    int dSize;
};

template<class K>
sortedChain<K>::~sortedChain(){
    while (firstNode != NULL){
        chainNode<K>* nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

template<class K>
K * sortedChain<K>::find(const K& theKey) const{
    chainNode<K>* currentNode = firstNode;

    // 寻找theKey
    while (currentNode != NULL &&
           currentNode->element != theKey)
        currentNode = currentNode->next;

    // 找到，输出当前链表元素的指针
    if (currentNode != NULL && currentNode->element == theKey){
        return &currentNode->element;
    }

    // 没有匹配的
    return NULL;
}

template<class K>
void sortedChain<K>::insert(const K& theKey){
    chainNode<K> *p = firstNode,
            *tp = NULL; // tp trails p

    // tp是前驱指针
    while (p != NULL && p->element < theKey){
        tp = p;
        p = p->next;
    }

    if (p != NULL && p->element == theKey){// 已存在
        cout << "Existed" << endl;
        return;
    }

    // 不存在，新建节点插入
    auto *newNode = new chainNode<K>(theKey, p);

    if (tp == NULL) firstNode = newNode;    //头节点插入
    else tp->next = newNode;                //非头节点插入

    dSize++;
}

template<class K>
void sortedChain<K>::erase(const K& theKey){
    chainNode<K> *p = firstNode,
            *tp = NULL;

    while (p != NULL && p->element < theKey)
    {
        tp = p;
        p = p->next;
    }

    // verify match
    if (p != NULL && p->element == theKey){// 存在，删除，输出删除后的长度
        if (tp == NULL) firstNode = p->next;
        else tp->next = p->next;

        delete p;
        dSize--;
        cout << dSize << endl;
    }
    else    //不存在，删除失败
        cout << "Delete Failed" << endl;
}

template<class K>
void sortedChain<K>::output(ostream& out) const
{// Insert the chain elements into the stream out.
    for (chainNode<K>* currentNode = firstNode;
         currentNode != NULL;
         currentNode = currentNode->next)
        out << currentNode->element<< "  ";
}

// overload <<
template <class K>
ostream& operator<<(ostream& out, const sortedChain<K>& x)
{x.output(out); return out;}

template<class T>
class hashChains {
    sortedChain<T> *table;
    int divisor;

public:
    explicit hashChains(int theDivisor){divisor=theDivisor;
        table = new sortedChain<T>[divisor];}

    ~hashChains(){ delete[]table; }

    void insert(const T &theKey) {
        table[theKey % divisor].insert(theKey);
    }

    void find(const T &theKey) {
        if (table[theKey % divisor].find(theKey)==NULL) cout << "Not Found" << endl;
        else cout << table[theKey % divisor].size()<<endl;
    }

    void erase(const T&theKey){
        table[theKey % divisor].erase(theKey);
    }
};

int main(){
    int D, m;
    cin >> D >> m;
    int opt, x;
    hashChains<int> table(D);
    for (int i = 0; i < m; ++i) {
        cin >> opt >> x;
        switch (opt) {
            case 0:
                table.insert(x);break;
            case 1:
                table.find(x);
                break;

            case 2:
                table.erase(x);
                break;
            default:
                break;
        }
    }
}