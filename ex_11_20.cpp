// 数组描述的二叉树的前序遍历

#include <iostream>

using namespace std;

template<class T>
void preOrder(pair<bool, T> *tree, int last, int index) {
    // 树中元素从数组index=1开始存放,遍历树即从index=1开始遍历
    if (tree[index].first && index <= last) {
        cout << tree[index].second << " ";        //访问本位
        preOrder(tree, last, 2 * index);    //左子树
        preOrder(tree, last, 2 * index + 1);//右子树
    }
}
int main(){
    pair<bool,int> a[30];
    for(int i = 1; i < 21;i++){
        a[i].first = true;
        a[i].second = i;
    }
    a[6].first = false;
    preOrder(a,20,1);
    return 0;
}


