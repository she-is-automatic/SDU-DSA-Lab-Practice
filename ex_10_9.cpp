// 删除跳表关键字最大和最小的节点

#include <iostream>
#include <cmath>

using namespace std;

template<class K, class E>
struct skipNode {
    typedef pair<const K, E> pairType;
    pairType element;
    skipNode<K, E> **next;   // 1D array of pointers

    skipNode(const pairType &thePair, int size)
            : element(thePair) { next = new skipNode<K, E> *[size]; }
};

template<class K, class E>
class skipList{
public:
    skipList(K, int maxPairs = 10000, float prob = 0.5);
    ~skipList();

    bool empty() const { return dSize == 0; }
    int size() const { return dSize; }
    pair<const K, E> *find(const K &) const;
    void erase(const K &);
    void insert(const pair<const K, E> &);
    void eraseSmallestKey();
    void eraseLargestKey();

    void output(ostream &out) const;

protected:
    float cutOff;
    int level() const;     // 计算插入的数的级数
    int levels;            // 现阶段允许的level
    int dSize;             // 数对数
    int maxLevel;          // 最大级
    K tailKey;             // 极大的key值
    skipNode<K, E> *search(const K &) const;

    skipNode<K, E> *headerNode;  // 头节点
    skipNode<K, E> *tailNode;    // 尾节点
    skipNode<K, E> **last;       // 每一级的最后一个元素
};

template<class K, class E>
skipList<K, E>::skipList(K largeKey, int maxPairs,
                         float prob) {// Constructor for skip lists with keys smaller than largeKey and
    // size at most maxPairs. 0 < prob < 1.
    cutOff = prob * RAND_MAX;
    maxLevel = (int) ceil(logf((float) maxPairs) / logf(1 / prob)) - 1;
    levels = 0;  // initial number of levels
    dSize = 0;
    tailKey = largeKey;

    pair<K, E> tailPair;
    tailPair.first = tailKey;
    headerNode = new skipNode<K, E>(tailPair, maxLevel + 1);
    tailNode = new skipNode<K, E>(tailPair, 0);
    last = new skipNode<K, E> *[maxLevel + 1];

    for (int i = 0; i <= maxLevel; i++)
        headerNode->next[i] = tailNode;
}

template<class K, class E>
skipList<K, E>::~skipList() {
    skipNode<K, E> *nextNode;

    while (headerNode != tailNode) {
        nextNode = headerNode->next[0];
        delete headerNode;
        headerNode = nextNode;
    }
    delete tailNode;

    delete[] last;
}

template<class K, class E>
pair<const K, E> *skipList<K, E>::find(const K &theKey) const {

    if (theKey >= tailKey)
        return NULL;


    skipNode<K, E> *beforeNode = headerNode;
    for (int i = levels; i >= 0; i--)
        // follow level i pointers
        while (beforeNode->next[i]->element.first < theKey)
            beforeNode = beforeNode->next[i];


    if (beforeNode->next[0]->element.first == theKey)
        return &beforeNode->next[0]->element;

    return NULL;
}

template<class K, class E>
int skipList<K, E>::level() const {
    int lev = 0;
    while (rand() <= cutOff)
        lev++;
    return (lev <= maxLevel) ? lev : maxLevel;
}

template<class K, class E>
skipNode<K, E> *skipList<K, E>::search(const K &theKey) const {// Search for theKey saving last nodes seen at each
    skipNode<K, E> *beforeNode = headerNode;
    for (int i = levels; i >= 0; i--) {
        while (beforeNode->next[i]->element.first < theKey)
            beforeNode = beforeNode->next[i];
        last[i] = beforeNode;
    }
    return beforeNode->next[0];
}

template<class K, class E>
void skipList<K, E>::insert(const pair<const K, E> &thePair) {// Insert thePair into the dictionary. Overwrite existing
    // pair, if any, with same key.
    if (thePair.first >= tailKey) // key too large
    {
        throw invalid_argument("Key must be < tailKey. ");
    }

    // see if pair with theKey already present
    skipNode<K, E> *theNode = search(thePair.first);
    if (theNode->element.first == thePair.first) {// update theNode->element.second
        theNode->element.second = thePair.second;
        return;
    }

    // not present, determine level for new node
    int theLevel = level(); // level of new node
    // fix theLevel to be <= levels + 1
    if (theLevel > levels) {
        theLevel = ++levels;
        last[theLevel] = headerNode;
    }

    // get and insert new node just after theNode
    skipNode<K, E> *newNode = new skipNode<K, E>(thePair, theLevel + 1);
    for (int i = 0; i <= theLevel; i++) {// insert into level i chain
        newNode->next[i] = last[i]->next[i];
        last[i]->next[i] = newNode;
    }

    dSize++;
}

template<class K, class E>
void skipList<K, E>::erase(const K &theKey) {// Delete the pair, if any, whose key equals theKey.
    if (theKey >= tailKey) // too large
        return;

    // see if matching pair present
    skipNode<K, E> *theNode = search(theKey);
    if (theNode->element.first != theKey) // not present
        return;

    // delete node from skip list
    for (int i = 0; i <= levels &&
                    last[i]->next[i] == theNode; i++)
        last[i]->next[i] = theNode->next[i];

    // update levels
    while (levels > 0 && headerNode->next[levels] == tailNode)
        levels--;

    delete theNode;
    dSize--;
}

template<class K, class E>
void skipList<K,E>::output(ostream& out) const
{// Insert the dictionary pairs into the stream out.
    // follow level 0 chain
    for (skipNode<K,E>* currentNode = headerNode->next[0];
    currentNode != tailNode;
    currentNode = currentNode->next[0])
        out << currentNode->element.first << " "
        << currentNode->element.second << "  ";
}

template<class K, class E>
void skipList<K, E>::eraseSmallestKey() {
    skipNode<K,E> *deleteNode = headerNode->next[0];   //key最小的节点

    for (int i = 0; i <= levels && headerNode->next[i] == deleteNode; ++i) {
        headerNode->next[i] = headerNode->next[i]->next[i];
    }

    while (levels > 0 && headerNode->next[levels] == tailNode)
        --levels;

    delete deleteNode;
    --dSize;
}

template<class K, class E>
void skipList<K, E>::eraseLargestKey() {
    skipNode<K,E> *deleteNode = headerNode;
    for (int i = levels; i >= 0 ; --i) {
        // 找到每一级的最后一个节点
        while(deleteNode->next[i] != tailNode ) {
            deleteNode = deleteNode->next[i];
        }
    }
    K theKey=deleteNode->element.first;
    erase(theKey);
}

// overload <<
template <class K, class E>
ostream& operator<<(ostream& out, const skipList<K,E>& x)
{x.output(out); return out;}


int main(){
    skipList<int, int> a(10000);

    pair<int,int>p;
    for (int i = 0; i < 15; ++i) {
        p.first = i + 1;
        p.second = 100 * p.first;
        a.insert(p);
    }
    a.eraseLargestKey();
    cout << a.size() << endl;
    cout << a << endl;

}