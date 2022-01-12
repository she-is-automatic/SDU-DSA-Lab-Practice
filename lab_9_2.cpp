// 接收二叉树前序序列和中序序列(各元素各不相同)，输出该二叉树的后序序列。

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

    linkedBinaryTree<T> &preAndInOrderConstruct(T *pre, T *in, int size);
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
void linkedBinaryTree<T>::makeTree(const T &element, linkedBinaryTree<T> &leftTree, linkedBinaryTree<T> &rightTree) {
    root = new binaryTreeNode<T> (element, leftTree.root, rightTree.root);
    treeSize = leftTree.treeSize + rightTree.treeSize + 1;

    leftTree.root = rightTree.root = nullptr;
    leftTree.treeSize = rightTree.treeSize = 0;
}


template<class T>
linkedBinaryTree<T>& linkedBinaryTree<T>::preAndInOrderConstruct(T *pre, T *in, int size){
    if (size == 0) {    //返回空树
        auto emptyTree=new linkedBinaryTree<T>;
        return *emptyTree;
    }
    int i = 0;
    for (i = 0; i < size && in[i] != pre[0]; ++i); //计算根节点在中序中的位置
    linkedBinaryTree<T> left = preAndInOrderConstruct(pre + 1, in, i);
    linkedBinaryTree<T> right = preAndInOrderConstruct(pre + 1 + i, in + i + 1, size - i - 1);
    makeTree(pre[0], left, right);
    return *this;
}



int main() {
    linkedBinaryTree<int> tree;

    int n;
    cin >> n;
    int *pre = new int[n];
    int *in = new int[n];
    for (int i = 0; i < n; ++i)
        cin >> pre[i];
    for (int i = 0; i < n; ++i)
        cin >> in[i];

    tree.preAndInOrderConstruct(pre, in, n);
    tree.postOrderOutput();


    return 0;
}
