//设通讯录中每一个联系人的内容有：姓名、电话号码、班级、宿舍。由标准输入读入联系人信息，使用线性表中操作实现通讯录管理功能，
// 包括：插入、删除、编辑、查找（按姓名查找）；键盘输入一班级，输出通讯录中该班级中所有人的信息。
//§	0 姓名 电话 班级 宿舍 插入一条记录
//§	1 姓名 根据姓名删除一条记录
//§	2 姓名 编辑项目 项目新值 根据姓名编辑一条记录(编辑项目为1到3的整数，1代表编辑电话，2代表编辑班级，3代表编辑宿舍)
//§	3 姓名 根据姓名查找，找到输出1，未找到输出0
//§	4 班级 输出该班级的所有成员的宿舍号的异或值
//其中查找操作当找到相应的人时输出1，未找到输出0。输出一个班级的人员信息时输出所有成员的宿舍号的异或值。输入数据保证合法。
//输入：第一行一个n(1<=n<=20000), 代表接下来操作的数目。接下来n行代表各项操作。
//输出：当遇到查找和输出一个班所有人员信息操作时输出


#include <iostream>
#include <sstream>

using namespace std;


class arrayList;

class contact {
    string name;
    string tel;
    string cla;
    int dormitory;
public:
    friend class arrayList;
    contact() = default;
    contact(string theName, string theTel, string theClass, int theDormitory)
            : name(std::move(theName)), tel(std::move(theTel)), cla(std::move(theClass)),
              dormitory(theDormitory) {}
    contact(const contact &theContact) {
        name = theContact.name;
        tel = theContact.tel;
        cla = theContact.cla;
        dormitory = theContact.dormitory;
    }
};

class arrayList {
public:
    explicit arrayList(int initialCapacity = 10); //构造函数
    arrayList(const arrayList &);    //复制构造
    ~arrayList() { delete[]element; }    //析构函数

    //删除name的元素
    void erase(const string &theName);
    //将name对应的元素
    void insert(const contact &theElement);
    //编辑项目
    void update(const string &name, int ch);
    //检查姓名是否在数组中
    void have(const string &name);
    //返回姓名的索引
    int getIndex(const string &theName);
    //班级宿舍号的异或值
    void xorClass(const string &theClass);
    //返回一维数组的长度
    int capacity() const { return arrayLength; }

protected:

    contact *element;         //数组
    int arrayLength;    //一维数组的容量
    int listSize;       //线性表元素个数
};


arrayList::arrayList(int initialCapacity) {
    arrayLength = initialCapacity;
    element = new contact[initialCapacity];
    listSize = 0;
}


arrayList::arrayList(const arrayList &theList) {
    arrayLength = theList.arrayLength;
    listSize = theList.listSize;
    element = new contact[arrayLength];
    copy(theList.element, theList.element + theList.listSize, element);
}


void arrayList::erase(const string &theName) {
    int index = getIndex(theName);
    if (index != -1) {
        //调用copy函数，将theIndex后续元素均复制到前一个位置
        copy(element + index + 1, element + listSize, element + index);
        //调用最后一个元素的析构函数，释放其空间
        element[--listSize].~contact();
    }
}


void arrayList::insert(const contact &theElement) {
    //判断数组是否满
    if (listSize == arrayLength) {

        auto *temp = new contact[2 * arrayLength];
        copy(element, element + listSize, temp);
        delete[]element;
        element = temp;
        arrayLength *= 2;
    }
    element[listSize++] = theElement;
}

void arrayList::have(const string &name) {
    for (int i = 0; i < listSize; ++i) {
        if (element[i].name == name) {
            cout << 1 << endl;
            return;
        }
    }
    cout << 0 << endl;
}

void arrayList::xorClass(const string &theClass) {
    int ret = 0;
    for (int i = 0; i < listSize; ++i) {
        if (element[i].cla == theClass) {
            ret ^= element[i].dormitory;
        }
    }
    cout << ret << endl;
}

void arrayList::update(const string &name, int ch) {
    int index = getIndex(name);
    if (index == -1)return;
    switch (ch) {
        case 1: {
            string newTel;
            cin >> newTel;
            element[index].tel = newTel;
            break;
        }
        case 2: {
            string newClass;
            cin >> newClass;
            element[index].cla = newClass;
            break;
        }
        case 3: {
            int newDormitory;
            cin >> newDormitory;
            element[index].dormitory = newDormitory;
            break;
        }
        default:
            return;
    }
}

int arrayList::getIndex(const string &theName) {
    for (int i = 0; i < listSize; ++i) {
        if (element[i].name == theName) return i;
    }
    return -1;
}


int main() {
    int n;
    cin >> n;
    arrayList a;
    for (int i = 0; i < n; ++i) {
        int ch;
        cin >> ch;
        switch (ch) {
            case 0: {
                string name,tel,cla;
                int dormitory;
                cin >> name >> tel >> cla;
                cin >> dormitory;
                contact newContact(name, tel, cla, dormitory);
                a.insert(newContact);
                break;
            }
            case 1: {
                string name;
                cin >> name;
                a.erase(name);
                break;
            }
            case 2: {
                string name;
                cin >> name;
                int choice;
                cin >> choice;
                a.update(name, choice);
                break;
            }
            case 3: {
                string name;
                cin >> name;
                a.have(name);
                break;
            }
            case 4: {
                string theClass;
                cin >> theClass;
                a.xorClass(theClass);
                break;
            }
            default:
                break;
        }
    }
    return 0;
}