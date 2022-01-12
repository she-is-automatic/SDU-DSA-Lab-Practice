//输入格式
//第一行为一个数字n (10<=n<=100000)，表示有这棵树有n个节点，编号为1~n。
//之后n行每行两个数字，第 i 行的两个数字a、b表示编号为 i 的节点的左孩子节点为 a，右孩子节点为 b，-1表示该位置没有节点。
//保证数据有效，根节点为1。
//
//输出格式
//第一行，n个数字，表示该树的层次遍历。
//第二行，n个数字，第i个数字表示以 i 节点为根的子树的节点数目。
//第三行，n个数字，第i个数字表示以 i 节点为根的子树的高度。

#include <iostream>
#include <queue>
using namespace std;

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

    void setTree(const int *left, const int *right, int n);
    void subNodeAmount(); //每个子树的节点个数
    void subHeight();  //每个子树的高度


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

    static int subHeight(binaryTreeNode<T> *t, int *a);
    static int subNodeAmount(binaryTreeNode<T> *t, int *a);

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
void linkedBinaryTree<T>::setTree(const int *left, const int *right, int n) {
    this->erase();
    if(n<1) return; //空树

    treeSize = n;
    root =  new binaryTreeNode<T>(1);
    queue<binaryTreeNode<T>*> q;
    q.push(root);
    binaryTreeNode<T>*t;
    while (!q.empty()) {
        t = q.front();
        q.pop();
        int i = t->element - 1;
        if (left[i] != -1) {
            t->leftChild = new binaryTreeNode<T>(left[i]);
            q.push(t->leftChild);
        }
        if (right[i] != -1) {
            t->rightChild = new binaryTreeNode<T>(right[i]);
            q.push(t->rightChild);
        }
    }
}

template<class T>
int linkedBinaryTree<T>::subHeight(binaryTreeNode<T> *t, int *a) {
    if(t==NULL) return 0;
    int hl = subHeight(t->leftChild,a);
    int hr = subHeight(t->rightChild,a);
    int h = max(hl, hr) + 1;
    a[t->element - 1] = h;
    return h;
}

template<class T>
int linkedBinaryTree<T>::subNodeAmount(binaryTreeNode<T> *t, int *a) {
    if (t==nullptr) return 0;
    int amount = subNodeAmount(t->leftChild,a) + subNodeAmount(t->rightChild,a) + 1;
    a[t->element - 1] = amount;
    return amount;
}

template<class T>
void linkedBinaryTree<T>::subNodeAmount() {
    int *nodeAmount = new int[treeSize];
    subNodeAmount(root,nodeAmount);
    for (int i = 0; i < treeSize; ++i)
        cout << nodeAmount[i] << " ";
    cout << endl;
    delete[]nodeAmount;
}

template<class T>
void linkedBinaryTree<T>::subHeight() {
    int *height = new int[treeSize];
    subHeight(root,height);
    for (int i = 0; i < treeSize; ++i)
        cout<<height[i]<<" ";
    cout << endl;
    delete[]height;
}


int main() {
    linkedBinaryTree<int> tree;
    int n;
    cin >> n;
    int *left = new int[n];
    int *right = new int[n];
    for (int i = 0; i < n; ++i) {
        cin >> left[i] >> right[i];
    }

    tree.setTree(left, right, n);
    tree.levelOrderOutput();

    tree.subNodeAmount();
    tree.subHeight();

    return 0;
}
