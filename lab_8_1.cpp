//散列表的数组描述
//给定散列函数的除数D和操作数m，输出每次操作后的状态。
//有以下三种操作：
//插入x，若散列表已存在x，输出“Existed”，否则插入x到散列表中，输出所在的下标。
//查询x，若散列表不含有x，输出“-1”，否则输出x对应下标。
//删除x，若散列表不含有x，输出“Not Found”，否则输出删除x过程中移动元素的个数。

#include <iostream>
#include <string>

using namespace std;

template<class K>
class hashTable {
public:
    explicit hashTable(int theDivisor = 11);
    ~hashTable() { delete[] table; }

    // ADT
    bool empty() const { return dSize == 0; }
    int size() const { return dSize; }
    int find(const K &) const;
    void insert(const K &);
    void erase(const K &);
    void output(ostream &out) const;

protected:
    int search(const K &) const;

    K **table;                 // 散列表数组，存放指向元素的指针
//    hash<K> hash;              // 将K转换为整数
    int dSize;                 // 字典中数对的个数
    int divisor;               // 表的长度
};

template<class K>
hashTable<K>::hashTable(int theDivisor) {
    divisor = theDivisor;
    dSize = 0;

    // 初始化散列表数组，将每一个指针赋为空
    table = new K *[divisor];
    for (int i = 0; i < divisor; i++)
        table[i] = NULL;
}

template<class K>
int hashTable<K>::search(const K &theKey) const {
    // 若数组中存在该元素，则返回该元素的位置；否则返回该元素可能可以插入的位置

    int i = theKey % divisor;  //计算起始桶
    int j = i;
    do {    //若遇到空桶或theKey，则返回索引
        if (table[j] == NULL || *table[j] == theKey)
            return j;
        j = (j + 1) % divisor;
    } while (j != i);

    return j;   // 不可插入
}

template<class K>
int hashTable<K>::find(const K &theKey) const {
    int b = search(theKey); //调用search函数

    if (table[b] == NULL || *table[b] != theKey)
        return -1;    //没有匹配的，返回-1

    return b;    //找到匹配的，返回索引
}

template<class K>
void hashTable<K>::insert(const K &theKey) {
    int b = search(theKey);  //调用search函数

    // 检查是否可以插入
    if (table[b] == NULL) { // 该位为空，可插入
        cout << b << endl;  //输出下标
        table[b] = new K(theKey);   //分配空间，插入
        dSize++;
    }
    else {// 该位不为空，判断键是否等于thisKey
        if (*table[b] == theKey)//相等，则哈希表中存在该数，输出存在
            cout << "Existed" << endl;

        else // 不等，表示哈希表满，不可插入
            throw invalid_argument("Hash Table Full.");
    }
}

template<class K>
void hashTable<K>::output(ostream &out) const { // 输出函数
    for (int i = 0; i < divisor; i++)
        if (table[i] == NULL)
            cout << "NULL" << endl;
        else
            cout << *table[i] << " ";

    cout << endl;
}

template<class K>
void hashTable<K>::erase(const K &theKey) {
    int thePlace = search(theKey); // 找到该元素的位置
    if (table[thePlace] == NULL || *table[thePlace]!= theKey){
        cout << "Not Found" << endl;
        return;  //表中没有该元素，返回
    }

    //删除该元素
    int blank = thePlace; // 找到该元素的位置
    delete table[blank];
    table[blank] = NULL;
    int moveNum = 0;    // 移动元素个数

    // 移动该移动的元素
    int now = (blank + 1) % divisor;    //遍历table
    while (table[now] != NULL && now != thePlace) { //循环，直到遇到空桶或返回原位
        int place = (*table[now]) % divisor;    //当前元素应放置的位置

        // 如有必要，将当前元素移至空位
        if ((blank < now && (place <= blank || place > now)) || (place > now && place <= blank)) {
            table[blank] = table[now];
            blank = now;
            table[blank] = NULL;
            ++moveNum;
        }

        //当前元素后移一位
        now = (now + 1) % divisor;
    }
    cout << moveNum << endl;
}


int main(){
    int D, m;   //除数与操作次数
    cin >> D >> m;
    int opt, x;
    hashTable<int> table(D);
    for (int i = 0; i < m; ++i) {
        cin >> opt >> x;
        switch (opt) {
            case 0:
                table.insert(x);
                break;
            case 1:
                cout << table.find(x) << endl;
                break;
            case 2:
                table.erase(x);
                break;
            default:break;
        }
    }
    return 0;
}