// 输入：
// 输入第一行一个数字m (m<=1000000)，表示有m个操作
// 接下来m行，每一行有两个数字a，b；
// a = 0 ，插入 b
// a = 1 ，查找 b
// a = 2 ，删除 b
// a = 3 ，查找搜索树中名次为 b 的元素
// a = 4 ，删除搜索树中名次为 b 的元素

//输出：
//对于输入中的每一种操作，输出执行操作的过程中依次比较的元素值的异或值。


#include <iostream>
#include <queue>

using namespace std;

template<class T>
struct binaryTreeNode { //二叉树节点
    T element;  //节点元素值
    binaryTreeNode<T> *leftChild, *rightChild;  //左子树和右子树

    binaryTreeNode() { leftChild = rightChild = NULL; }

    explicit binaryTreeNode(const T &theElement) : element(theElement) {
        leftChild = rightChild = NULL;
    }

    binaryTreeNode(const T &theElement, binaryTreeNode<T> *theLeftChild, binaryTreeNode<T> *theRightChild)
            : element(theElement), leftChild(theLeftChild), rightChild(theRightChild) {}
};

template<class K, class E>
struct indexBSNode {
    int leftSize;
    K key;
    E value;

    indexBSNode() { leftSize = 0; }

    indexBSNode(const pair<K, E> &thePair) : key(thePair.first), value(thePair.second), leftSize(0) {}
    indexBSNode(const K &theKey, const E &theValue) : key(theKey), value(theValue), leftSize(0) {}
};

template<class K, class E>
class binarySearchTree {
public:
    binarySearchTree() {
        root = NULL;
        treeSize = 0;
    }

    ~binarySearchTree() { erase(); }

    void erase() {
        postOrder(dispose);
        root = nullptr;
        treeSize = 0;
    }

    bool empty() { return treeSize == 0; }
    int size() { return treeSize; }
    indexBSNode<K, E> *find(const K &) const;
    void insert(const pair<K, E> &);
    void erase(const K &);
    indexBSNode<K,E>*findByIndex(int index);
    void eraseByIndex(int index);


    void preOrderOutput() {preOrder(output);cout << endl;}
    void inOrderOutput() {inOrder(output);cout << endl;}
    void postOrderOutput() {postOrder(output);cout << endl;}
    void levelOrderOutput() {levelOrder(output);cout << endl;}


    void preOrder(void (*theVisit)(binaryTreeNode<indexBSNode<K, E>> *)) {visit = theVisit;preOrder(root);}
    void inOrder(void (*theVisit)(binaryTreeNode<indexBSNode<K, E>> *)) {visit = theVisit;inOrder(root);}
    void postOrder(void (*theVisit)(binaryTreeNode<indexBSNode<K, E>> *)) {visit = theVisit;postOrder(root);}
    void levelOrder(void(*theVisit)(binaryTreeNode<indexBSNode<K, E>> *)) {visit = theVisit;levelOrder(root);}


private:
    binaryTreeNode<indexBSNode<K, E>> *root;
    int treeSize;

    static void (*visit)(binaryTreeNode<indexBSNode<K, E>> *);
    static void preOrder(binaryTreeNode<indexBSNode<K, E>> *t);
    static void inOrder(binaryTreeNode<indexBSNode<K, E>> *t);
    static void postOrder(binaryTreeNode<indexBSNode<K, E>> *t);
    static void dispose(binaryTreeNode<indexBSNode<K, E>> *t) { delete t; }
    static void output(binaryTreeNode<indexBSNode<K, E>> *t) { cout << t->element.key << " " << t->element.value <<" "<<t->element.leftSize<< endl; }
    static void levelOrder(binaryTreeNode<indexBSNode<K, E>> *t);

};


template<class K, class E>
void (*binarySearchTree<K, E>::visit)(binaryTreeNode<indexBSNode<K, E>> *);


template<class K, class E>
void binarySearchTree<K, E>::preOrder(binaryTreeNode<indexBSNode<K, E>> *t) {
    if (t != nullptr) {
        binarySearchTree<K, E>::visit(t);
        preOrder(t->leftChild);
        preOrder(t->rightChild);
    }
}

template<class K, class E>
void binarySearchTree<K, E>::inOrder(binaryTreeNode<indexBSNode<K, E>> *t) {
    if (t != nullptr) {
        inOrder(t->leftChild);
        binarySearchTree<K, E>::visit(t);
        inOrder(t->rightChild);
    }
}

template<class K, class E>
void binarySearchTree<K, E>::postOrder(binaryTreeNode<indexBSNode<K, E>> *t) {
    if (t != nullptr) {
        postOrder(t->leftChild);
        postOrder(t->rightChild);
        binarySearchTree<K, E>::visit(t);
    }
}


template<class K, class E>
indexBSNode<K, E> *binarySearchTree<K, E>::find(const K &theKey) const {
    int xorResult = 0;
    binaryTreeNode<indexBSNode<K, E>> *p = root;
    while (p != NULL) {
        xorResult ^= p->element.key;
        if (p->element.key == theKey){
            cout << xorResult << endl;
            return &p->element;
        }
        else if (theKey < p->element.key) p = p->leftChild;
        else p = p->rightChild;
    }
    cout << 0 << endl;
    return NULL;    //未找到
}

template<class K, class E>
void binarySearchTree<K, E>::insert(const pair<K, E> &thePair) {
    int xorResult = 0;

    if (root == NULL) {
        root = new binaryTreeNode<indexBSNode<K, E>>(thePair);
        treeSize = 1;
        cout << xorResult << endl;
        return;
    }

    vector<binaryTreeNode<indexBSNode<K,E>>*>v;

    binaryTreeNode<indexBSNode<K, E>> *p = root, *pp = NULL;
    while (p != NULL) {
        xorResult ^= p->element.key;

        pp = p;
        if (p->element.key == thePair.first) {
            p->element.value = thePair.second; // 覆盖
            cout << 0 << endl;
            return;
        } else if (thePair.first < p->element.key) {    // 插入左树
            v.push_back(p);
            p = p->leftChild;
        }
        else p = p->rightChild;
    }

    p = new binaryTreeNode<indexBSNode<K, E>>(thePair);

    if (thePair.first > pp->element.key) pp->rightChild = p;
    else pp->leftChild = p;
    ++treeSize;

    for (int i = 0; i < v.size(); ++i)  // 左边leftSize+1
        ++v.at(i)->element.leftSize;

    cout << xorResult << endl;
}

template<class K, class E>
void binarySearchTree<K, E>::erase(const K &theKey) {
    binaryTreeNode<indexBSNode<K, E>> *p = root, *pp = NULL;
    int xorResult = 0;
    vector<binaryTreeNode<indexBSNode<K, E>> *> v;

    // 寻找要删除的节点
    while (p != NULL && p->element.key != theKey) {
        xorResult ^= p->element.key;
        pp = p;
        if (theKey < p->element.key) {
            v.push_back(p);
            p = p->leftChild;
        }
        else p = p->rightChild;
    }
    if (p == NULL) {
        cout << 0 << endl;
        return;    //要删除的节点不存在
    }

    xorResult ^= p->element.key;

    // 处理2个孩子的情况
    if (p->leftChild != NULL && p->rightChild != NULL) {
        binaryTreeNode<indexBSNode<K, E>> *s = p->rightChild, *ps = p;
        while (s->leftChild != NULL) { //寻找左子树中最大的元素
            s->element.leftSize--;
            ps = s;
            s = s->leftChild;
        }

        // 将右子树最小节点复制，替代要删除的节点的位置
        auto *q = new binaryTreeNode<indexBSNode<K, E>>(s->element, p->leftChild, p->rightChild);
        q->element.leftSize = p->element.leftSize;

        if (pp == NULL) root = q; //删除节点为根节点
        else {   //删除节点不为根节点
            if (p == pp->leftChild) pp->leftChild = q;
            else pp->rightChild = q;
        }

        // 将删除指针p及前驱指针pp指向原来右子树最小节点及其父节点，等待下一步处理
        if (ps == p) pp = q;
        else pp = ps;
        delete p;
        p = s;
    }

    // p最多有一个孩子
    binaryTreeNode<indexBSNode<K, E>> *child;
    if (p->leftChild != NULL) child = p->leftChild;
    else child = p->rightChild;

    if (p == root) root = child;
    else {
        if (p == pp->leftChild) pp->leftChild = child;
        else pp->rightChild = child;
    }
    --treeSize;
    delete p;

    // 处理经过的leftSize-1
    for (int i = 0; i < v.size(); ++i) {
        --v.at(i)->element.leftSize;
    }

    cout << xorResult << endl;
}

template<class K, class E>
void binarySearchTree<K, E>::levelOrder(binaryTreeNode<indexBSNode<K, E>> *t) {
    queue<binaryTreeNode<indexBSNode<K, E>> *> q;
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

template<class K, class E>
indexBSNode<K, E> *binarySearchTree<K, E>::findByIndex(int index) {
    if (index < 0 || index >= treeSize || root==NULL) {
        cout << 0 << endl;
        return NULL;
    }

    int xorResult = 0;

    binaryTreeNode<indexBSNode<K, E>> *p = root;
    while (p != NULL) {
        xorResult ^= p->element.key;
        if (index==p->element.leftSize){
            cout << xorResult << endl;
            return &p->element;
        }
        else if (index < p->element.leftSize) {
            p = p->leftChild;
        }
        else{
            index = index - p->element.leftSize - 1;
            p = p->rightChild;
        }
    }

    cout << 0 << endl;
    return NULL;
}

template<class K, class E>
void binarySearchTree<K, E>::eraseByIndex(int index) {
    if (index < 0 || index >= treeSize) {cout<<0<<endl;return;}

    int xorResult = 0;
    binaryTreeNode<indexBSNode<K, E>> *p = root, *pp = NULL;

    while (p != NULL && p->element.leftSize != index) {
        xorResult ^= p->element.key;
        pp = p;
        if (index < p->element.leftSize) {
            --p->element.leftSize;
            p = p->leftChild;
        } else {
            index = index - p->element.leftSize - 1;
            p = p->rightChild;
        }
    }
    if (p == NULL) {
        cout << 0 << endl;
        return;    //要删除的节点不存在
    }
    xorResult ^= p->element.key;

    // 处理2个孩子的情况
    if (p->leftChild != NULL && p->rightChild != NULL) {
        binaryTreeNode<indexBSNode<K, E>> *s = p->rightChild, *ps = p;
        while (s->leftChild != NULL) { //寻找左子树中最大的元素
            s->element.leftSize--;
            ps = s;
            s = s->leftChild;
        }

        // 将右子树最小节点复制，替代要删除的节点的位置
        auto *q = new binaryTreeNode<indexBSNode<K, E>>(s->element, p->leftChild, p->rightChild);
        q->element.leftSize = p->element.leftSize;

        if (pp == NULL) root = q; //删除节点为根节点
        else {   //删除节点不为根节点
            if (p == pp->leftChild) pp->leftChild = q;
            else pp->rightChild = q;
        }

        // 将删除指针p及前驱指针pp指向原来右子树最小节点及其父节点，等待下一步处理
        if (ps == p) pp = q;
        else pp = ps;
        delete p;
        p = s;
    }

    // p最多有一个孩子
    binaryTreeNode<indexBSNode<K, E>> *child;
    if (p->leftChild != NULL) child = p->leftChild;
    else child = p->rightChild;

    if (p == root) root = child;
    else {
        if (p == pp->leftChild) pp->leftChild = child;
        else pp->rightChild = child;
    }
    --treeSize;
    delete p;
    cout << xorResult << endl;
}



int main() {
    int m, op,b;
    cin >> m;
    binarySearchTree < int, int>  A;
    for ( int i = 0; i < m; ++i ) {
        cin >> op>>b;
        switch ( op ) {
            case 0: {
                A.insert({b, b});
//                cout << "  insert " << b << endl;
                break;
            }
            case 1: {
                A.find(b);
                break;
            }
            case 2: {
                A.erase(b);
                break;
            }
            case 3: {
                A.findByIndex(b-1);
                break;
            }
            case 4: {
                A.eraseByIndex(b-1);
                break;
            }
            default:break;
        }
    }

    return 0;
}
