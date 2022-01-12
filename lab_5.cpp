//创建稀疏矩阵类（参照课本MatrixTerm三元组定义） ,采用行主顺序把稀疏矩阵非0元素映射到一维数组中，实现操作：两个稀疏矩阵相加、两个稀疏矩阵相乘、稀疏矩阵的转置、输出矩阵。
//重置矩阵：操作1，即重置矩阵 P 的尺寸为 n 行 m 列,且随后按行优先顺序输入矩阵 P 的各个元素。
//矩阵乘法：操作2，t 行非零元素已按行优先顺序给出，矩阵中非零元素的表示为 x y v。设输入的矩阵为Q，若PxQ运算合法,则将PxQ的结果矩阵赋给 P,若不合法,则将Q赋给P，同时输出-1。
//矩阵加法：操作3，t 行非零元素已按行优先顺序给出，矩阵中非零元素的表示为 x y v。设输入的矩阵为 Q,若 P+Q 运算合法,则将 P+Q 的结果矩阵赋给 P,若不合法,则将 Q 赋给 P,同时输出 -1。
//输出操作：操作4，设当前矩阵 P 的尺寸为 n 行 m 列,第一行输出矩阵 P 的行数和列数，随后 n 行按行优先顺序输出矩阵 P,每行 m 个数字,来表示当前的矩阵内容，每行数字之间用空格分隔。
//转置操作：操作5，设当前矩阵 P 的尺寸为 n 行 m 列，将其转置为 m 行 n 列的矩阵，无需输出。

#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>

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
class arrayList {
public:

    arrayList(int initialCapacity = 10);

    arrayList(const arrayList<T> &);

    ~arrayList() { delete[] element; }

    // ADT methods
    bool empty() const { return listSize == 0; }

    int size() const { return listSize; }

    T &get(int theIndex) const;

    int indexOf(const T &theElement) const;

    void erase(int theIndex);

    void insert(int theIndex, const T &theElement);

    void output(ostream &out) const;

    // additional methods
    int capacity() const { return arrayLength; }

    void reSet(int);

    void set(int, const T &);

    void clear() { listSize = 0; }

    arrayList<T> &operator=(const arrayList<T> &a);

    class iterator;

    iterator begin() { return iterator(element); }

    iterator end() { return iterator(element + listSize); }

    // iterator for arrayList
    class iterator {
    public:
        // typedefs required by C++ for a bidirectional iterator
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T *pointer;
        typedef T &reference;

        iterator(T *thePosition = 0) { position = thePosition; }

        T &operator*() const { return *position; }

        T *operator->() const { return &*position; }

        // increment
        iterator &operator++()
        {
            ++position;
            return *this;
        }

        iterator operator++(int)
        {
            iterator old = *this;
            ++position;
            return old;
        }

        // decrement
        iterator &operator--()
        {
            --position;
            return *this;
        }

        iterator operator--(int)
        {
            iterator old = *this;
            --position;
            return old;
        }

        // equality testing
        bool operator!=(const iterator right) const { return position != right.position; }

        bool operator==(const iterator right) const { return position == right.position; }

    protected:
        T *position;
    };  // end of iterator class

protected:
    void checkIndex(int theIndex) const;

    T *element;
    int arrayLength;
    int listSize;
};

template<class T>
class matrixTerm {
public:
    int row;
    int col;
    T value;

    matrixTerm() = default;

    matrixTerm(int theRow, int theCol, const T &theValue) : row(theRow), col(theCol), value(theValue) {}

    matrixTerm(const matrixTerm<T> &a) {
        row = a.row;
        col = a.col;
        value = a.value;
    }

    explicit operator T() const { return value; }
};

template<class T>
class sparseMatrix {
    int rows, cols;
    arrayList<matrixTerm<T>> terms;
public:
    template<class U>
    friend ostream &operator<<(ostream &out, sparseMatrix<U> &x);

    template<class U>
    friend istream &operator>>(istream &in, sparseMatrix<U> &x);

    void checkIndex(int i, int j) {
        if (i < 1 || i > rows || j < 1 || j > cols)
            throw invalid_argument("out of bounds.");
    }

    T get(int theRow, int theCol);  // 获取元素
    void set(int theRow, int theCol, const T &value);   //设置元素
    void transpose();   // 矩阵转置
    void add(sparseMatrix<T> &);  // 矩阵加法
    void multi(sparseMatrix<T> &);// 矩阵乘法
    void output();  //输出矩阵
    void input();   //重置矩阵
    void multi2(sparseMatrix<T> &b);    //不聪明的超时乘法

};

template<class T>
T sparseMatrix<T>::get(int theRow, int theCol) {
    checkIndex(theRow, theCol);
    typename arrayList<matrixTerm<T>>::iterator it = terms.begin();
    for (; it != terms.end(); ++it) {
        if ((*it).row == theRow && (*it).col == theCol) return (*it).value;
    }
    return -1;
}

template<class T>
void sparseMatrix<T>::set(int theRow, int theCol, const T &value) {
    checkIndex(theRow, theCol);

    // 原始矩阵为全0矩阵，即 terms.listSize = 0
    if (terms.size() == 0) {
        if (value != 0) {
            matrixTerm<T> mTerm(theRow, theCol, value);
            terms.insert(0, mTerm);
        }
        return;
    }

    // 原始矩阵不全为0，有则覆盖，无则插入
    int index = 0;
    typename arrayList<matrixTerm<T>>::iterator it = terms.begin();

    for (; it != terms.end(); ++it) {
        index++;

        if (((*it).row == theRow && (*it).col >= theCol) || ((*it).row > theRow)) {
            break;
        }
    }
    matrixTerm<T> mTerm(theRow, theCol, value);

    // 矩阵中已经有当前索引，覆盖原有的值
    if ((*it).row == theRow && (*it).col == theCol) {
        if (value == 0) terms.erase(index - 1); // 变为0，从非0数组中删除
        else terms.set(index - 1, mTerm);   // 不为0，覆盖
    }

        //矩阵中没有当前索引，在输入元素不为0的时候，插入非0数组中
    else {
        if (value != 0) terms.insert(index, mTerm);
    }
}

template<class T>
ostream &operator<<(ostream &out, sparseMatrix<T> &x) {

    // put matrix characteristics
    out << "rows = " << x.rows << " columns = "
        << x.cols << endl;
    out << "nonzero terms = " << x.terms.size() << endl;

    typename arrayList<matrixTerm<T> >::iterator i = x.terms.begin();
    for (; i != x.terms.end(); i++)
        out << "a(" << (*i).row << ',' << (*i).col << ") = " << (*i).value << endl;

    return out;
}


// overload >>
template<class T>
istream &operator>>(istream &in, sparseMatrix<T> &x) {

    // input matrix characteristics
    int numberOfTerms;
    //    cout << "Enter number of rows, columns, and #terms"
    //    << endl;
    in >> x.rows >> x.cols >> numberOfTerms;

    x.terms.reSet(numberOfTerms);

    // input terms
    matrixTerm<T> mTerm;
    for (int i = 0; i < numberOfTerms; i++) {
        //        cout << "Enter row, column, and value of term "
        //        << (i + 1) << endl;
        in >> mTerm.row >> mTerm.col >> mTerm.value;
        x.terms.set(i, mTerm);
    }

    return in;
}

template<class T>
void sparseMatrix<T>::transpose() {
    // 初始化转置矩阵b
    arrayList<matrixTerm<T>> b;
    b.reSet(terms.size());

    // 初始化每列的元素个数、每列元素的起始位置
    int *colSize = new int[cols + 1];
    int *rowNext = new int[cols + 1];
    for (int i = 1; i <= cols; ++i) colSize[i] = 0;
    for (typename arrayList<matrixTerm<T>>::iterator it = terms.begin(); it != terms.end(); ++it)
        colSize[(*it).col]++;

    rowNext[1] = 0;
    for (int i = 2; i <= cols; ++i)
        rowNext[i] = rowNext[i - 1] + colSize[i - 1];

    // 转置矩阵
    for (typename arrayList<matrixTerm<T>>::iterator it = terms.begin(); it != terms.end(); ++it) {
        int place = rowNext[(*it).col]++;
        matrixTerm<T> mTerm((*it).col, (*it).row, (*it).value);
        b.set(place, mTerm);
    }

    swap(rows, cols);
    terms = b;
}

template<class T>
void sparseMatrix<T>::add(sparseMatrix<T> &b) {
    if (rows != b.rows || cols != b.cols) { // 矩阵不相容
        cout << -1 << endl;
        rows = b.rows;
        cols = b.cols;
        terms = b.terms;
        return;
    }

    // 定义迭代器
    typename arrayList<matrixTerm<T>>::iterator it = terms.begin();
    typename arrayList<matrixTerm<T>>::iterator itEnd = terms.end();
    typename arrayList<matrixTerm<T>>::iterator ib = b.terms.begin();
    typename arrayList<matrixTerm<T>>::iterator ibEnd = b.terms.end();

    int index = 0;
    arrayList<matrixTerm<T>> c;
    while (it != itEnd && ib != ibEnd) {
        // 计算当前元素在矩阵中的位置索引
        int tIndex = (*it).row * cols + (*it).col;
        int bIndex = (*ib).row * cols + (*ib).col;

        if (tIndex < bIndex) {
            c.insert(index++, (*it));
            ++it;
        } else if (tIndex == bIndex) {
            if ((*it).value + (*ib).value != 0) {   // 索引相等且相加值不为0，插入
                matrixTerm<T> mTerm((*it).row, (*it).col, (*it).value + (*ib).value);
                c.insert(index++, mTerm);
            }
            ++ib;
            ++it;
        } else {
            c.insert(index++, (*ib));
            ++ib;
        }
    }

    // 将没加到尾的直接加入新矩阵
    for (; it != itEnd; ++it)
        c.insert(index++, *it);
    for (; ib != ibEnd; ++ib)
        c.insert(index++, *ib);

    terms = c;
}

template<class T>
void sparseMatrix<T>::multi2(sparseMatrix<T> &b) {
    if (cols != b.rows) {   // 不相容

        rows = b.rows;
        cols = b.cols;
        terms = b.terms;
        cout << "-1" << endl;

        return;
    }

    // 相容，计算矩阵相乘的结果
    arrayList<matrixTerm<T>> temp;  // 保存矩阵相乘的最终结果

    // 计算a矩阵每行的元素个数和每行第一个元素的索引
    int *numRows = new int[rows + 1];
    int *rowFirst = new int[rows + 1];
    for (int i = 1; i <= rows; ++i)
        numRows[i] = 0;
    for (typename arrayList<matrixTerm<T>>::iterator it = terms.begin(); it != terms.end(); ++it)
        numRows[(*it).row]++;
    rowFirst[1] = 0;
    for (int i = 2; i <= rows; ++i)
        rowFirst[i] = rowFirst[i - 1] + numRows[i - 1];

    T *result = new T[b.rows + 1];  // 结果数组，储存矩阵相乘第i列的结果
    matrixTerm<T> mTerm;    // 每一位的结果
    int index = 0;  //计算插入的索引

    for (int i = 1; i <= rows; ++i) {   //计算矩阵相乘第i行的结果并保存

        //初始化结果矩阵，从0开始累加
        for (int j = 1; j <= b.rows; ++j)
            result[j] = 0;

        // 计算第i行的结果
        for (int j = 0; j < numRows[i]; ++j) {
            matrixTerm<T> &aTerm = terms.get(rowFirst[i] + j);  // 第i行第j个元素
            for (int k = 0; k < b.terms.size(); ++k) {  // 遍历b矩阵，累加结果
                matrixTerm<T> &bTerm = b.terms.get(k);
                if (aTerm.row == i && aTerm.col == bTerm.row) {
                    result[bTerm.col] += aTerm.value * bTerm.value;
                }
            }
        }

        // 第i行结果保存
        for (int j = 1; j <= b.cols; ++j) {
            if (result[j] != 0) {
                mTerm.row = i;
                mTerm.col = j;
                mTerm.value = result[j];
                temp.insert(index++, mTerm);
            }
        }
    }
    delete[]numRows;
    delete[]rowFirst;
    cols = b.cols;
    terms = temp;
}


template<class T>
void sparseMatrix<T>::output() {
    cout << rows << " " << cols << endl;
    typename arrayList<matrixTerm<T> >::iterator p = terms.begin();
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            if (p != terms.end() && (*p).row == i && (*p).col == j) {
                cout << (*p).value << " ";
                p++;
            } else { cout << 0 << " "; }
        }
        cout << endl;
    }

}

template<class T>
void sparseMatrix<T>::input() {
    cin >> rows >> cols;
    terms.reSet(0);
    T value;
    matrixTerm<T> mTerm;
    int index = 0;
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            cin >> value;
            if (value != 0) {
                mTerm.row = i;
                mTerm.col = j;
                mTerm.value = value;
                terms.insert(index++, mTerm);
            }
        }
    }
}

template<class T>
void sparseMatrix<T>::multi(sparseMatrix<T> &b) {
    if (cols != b.rows) {   // 不相容
        rows = b.rows;
        cols = b.cols;
        terms = b.terms;
        cout << -1 << endl;
        return;
    }

    // 相容，计算矩阵相乘的结果
    arrayList<matrixTerm<T>> result;  // 保存矩阵相乘的最终结果
    b.transpose();
    // 计算a矩阵每行的元素个数和每行第一个元素的索引
    int *rowSize = new int[rows + 1];
    int *rowFirst = new int[rows + 2];
    for (int i = 1; i <= rows; ++i)
        rowSize[i] = 0;
    for (typename arrayList<matrixTerm<T>>::iterator it = terms.begin(); it != terms.end(); ++it)
        rowSize[(*it).row]++;
    rowFirst[1] = 0;
    for (int i = 2; i <= rows + 1; ++i)
        rowFirst[i] = rowFirst[i - 1] + rowSize[i - 1];

    matrixTerm<T> mTerm;    // 每一位的结果
    int index = 0;  //计算插入的索引
    typename arrayList<matrixTerm<T>>::iterator it = b.terms.begin();
    typename arrayList<matrixTerm<T>>::iterator itEnd = b.terms.end();

    for (int i = 1; i <= rows; ++i) {   //计算矩阵相乘第i行的结果并保存
        // 第i行为空，直接进入下一行循环
        if (rowSize[i] == 0) continue;

        //从第i行起始位置开始，遍历b矩阵
        int aIndex = rowFirst[i];
        it = b.terms.begin();

        // 遍历b矩阵，计算第i行的结果
        while (it != itEnd) {
            int j = (*it).row;  //计算第i行第j列
            int value = 0;
            while (aIndex < rowFirst[i + 1] && it != itEnd && (*it).row == j) {
                if (terms.get(aIndex).col > (*it).col) ++it;
                else if (terms.get(aIndex).col < (*it).col) ++aIndex;
                else {
                    value += terms.get(aIndex).value * (*it).value;
                    ++aIndex;
                    ++it;
                }
            }
            // 保存结果矩阵第i行第j列的元素
            if (value != 0) {
                mTerm.row = i;
                mTerm.col = j;
                mTerm.value = value;
                result.insert(index++, mTerm);
            }

            // 计算第i行的下一个数
            aIndex = rowFirst[i];
            while (it != itEnd && (*it).row == j) ++it;
        }
    }

    // 结果处理
    b.transpose();
    cols = b.cols;
    terms = result;
    delete[]rowFirst;
    delete[]rowSize;
}

template<class T>
arrayList<T>::arrayList(int initialCapacity) {
    if (initialCapacity < 1) {
        ostringstream s;
        s << "Initial capacity = " << initialCapacity << " Must be > 0";
        throw invalid_argument(s.str());
    }
    arrayLength = initialCapacity;
    element = new T[arrayLength];
    listSize = 0;
}

template<class T>
arrayList<T>::arrayList(const arrayList<T> &theList) {
    arrayLength = theList.arrayLength;
    listSize = theList.listSize;
    element = new T[arrayLength];
    copy(theList.element, theList.element + listSize, element);
}

template<class T>
void arrayList<T>::reSet(int theSize) {
    if (theSize < 0) {
        ostringstream s;
        s << "Requested size = " << theSize << " Must be >= 0";
        throw invalid_argument(s.str());
    }

    if (theSize > arrayLength) {
        delete[]element;
        element = new T[theSize];
        arrayLength = theSize;
    }
    listSize = theSize;
}

template<class T>
void arrayList<T>::checkIndex(int theIndex) const {
    if (theIndex < 0 || theIndex >= listSize) {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw invalid_argument(s.str());
    }
}

template<class T>
T &arrayList<T>::get(int theIndex) const {
    checkIndex(theIndex);
    return element[theIndex];
}

template<class T>
void arrayList<T>::set(int theIndex, const T &newValue) {
    checkIndex(theIndex);
    element[theIndex] = newValue;
}

template<class T>
int arrayList<T>::indexOf(const T &theElement) const {

    int theIndex = (int) (find(element, element + listSize, theElement)
                          - element);

    if (theIndex == listSize)
        // not found
        return -1;
    else return theIndex;
}

template<class T>
void arrayList<T>::erase(int theIndex) {
    checkIndex(theIndex);

    copy(element + theIndex + 1, element + listSize,
         element + theIndex);

    element[--listSize].~T();
}

template<class T>
void arrayList<T>::insert(int theIndex, const T &theElement) {
    if (theIndex < 0 || theIndex > listSize) {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw invalid_argument(s.str());
    }

    // valid index, make sure we have space
    if (listSize == arrayLength) {
        changeLength1D(element, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }

    copy_backward(element + theIndex, element + listSize,
                  element + listSize + 1);

    element[theIndex] = theElement;

    listSize++;
}

template<class T>
void arrayList<T>::output(ostream &out) const {
    copy(element, element + listSize, ostream_iterator<T>(cout, "  "));
}

template<class T>
arrayList<T> &arrayList<T>::operator=(const arrayList<T> &a) {
    if (this == &a) return *this;

    listSize = a.listSize;
    arrayLength = a.arrayLength;
    delete[]element;
    element = new T[arrayLength];
    copy(a.element, a.element + listSize, element);
    return *this;
}

// overload <<
template<class T>
ostream &operator<<(ostream &out, const arrayList<T> &x) {
    x.output(out);
    return out;
}

int main() {
    sparseMatrix<int> a;
    int w, ch;
    cin >> w;
    for (int i = 0; i < w; ++i) {
        cin >> ch;
        switch (ch) {
            case 1: {
                a.input();
                break;
            }
            case 2: {
                sparseMatrix<int> b;
                cin >> b;
                a.multi(b);
                break;
            }
            case 3: {
                sparseMatrix<int> b;
                cin >> b;
                a.add(b);
                break;
            }
            case 4: {
                a.output();
                break;
            }
            case 5: {
                a.transpose();
                break;
            }
            default:
                break;
        }
    }
    return 0;
}