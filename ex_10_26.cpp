#include <iostream>
#include <string>

using namespace std;

template<class K, class E>
class hashTable {
public:
    hashTable(int theDivisor = 11);

    ~hashTable() { delete[] table; }

    bool empty() const { return dSize == 0; }

    int size() const { return dSize; }

    int find(const K &) const;

    void insert(const pair<const K, E> &);

    void erase(const K &);

    void output(ostream &out) const;

protected:
    int search(const K &) const;

    pair<const K, E> **table;  // hash table
    hash<K> hash;              // maps type K to nonnegative integer
    int dSize;                 // number of pairs in dictionary
    int divisor;               // hash function divisor
};

template<class K, class E>
hashTable<K, E>::hashTable(int theDivisor) {
    divisor = theDivisor;
    dSize = 0;

    // allocate and initialize hash table array
    table = new pair<const K, E> *[divisor];
    for (int i = 0; i < divisor; i++)
        table[i] = NULL;
}

template<class K, class E>
int hashTable<K, E>::search(const K &theKey) const {// Search an open addressed hash table for a pair with key theKey.
    // Return location of matching pair if found, otherwise return
    // location where a pair with key theKey may be inserted
    // provided the hash table is not full.

    int i = (int) hash(theKey) % divisor;  // home bucket
    int j = i;    // start at home bucket
    do {
        if (table[j] == NULL || table[j]->first == theKey)
            return j;
        j = (j + 1) % divisor;  // next bucket
    } while (j != i);          // returned to home bucket?

    return j;  // table full
}

template<class K, class E>
int hashTable<K, E>::find(const K &theKey) const {// Return pointer to matching pair.
    // Return NULL if no matching pair.
    // search the table
    int b = search(theKey);

    // see if a match was found at table[b]
    if (table[b] == NULL || table[b]->first != theKey)
        return NULL;           // no match

    return table[b];  // matching pair
}

template<class K, class E>
void hashTable<K, E>::insert(const pair<const K, E> &thePair) {// Insert thePair into the dictionary. Overwrite existing
    // pair, if any, with same key.
    // Throw hashTableFull exception in case table is full.
    // search the table for a matching pair
    int b = search(thePair.first);

    // check if matching pair found
    if (table[b] == NULL) {
        // no matching pair and table not full
        table[b] = new pair<const K, E>(thePair);
        dSize++;
    } else {// check if duplicate or table full
        if (table[b]->first == thePair.first) {// duplicate, change table[b]->second
            table[b]->second = thePair.second;
        } else // table is full
            throw invalid_argument("Hash Table Full.");
    }
}

template<class K, class E>
void hashTable<K, E>::output(ostream &out) const {// Insert the hash table into the stream out.
    for (int i = 0; i < divisor; i++)
        if (table[i] == NULL)
            cout << "NULL" << endl;
        else
            cout << table[i]->first << " "
                 << table[i]->second << endl;
}

template<class K, class E>
void hashTable<K, E>::erase(const K &theKey) {
    int thePlace = search(theKey); //找到该元素的位置
    if (table[thePlace] == NULL || table[thePlace]->first != theKey) return;  //表中没有该元素，返回

    //删除该元素
    int blank = thePlace; //找到该元素的位置
    delete table[blank];
    table[blank] = NULL;

    // 移动该移动的元素
    int now = (blank + 1) % divisor;    //遍历table
    while (table[now] != NULL && now != thePlace) {
        int place = (int) hash(table[now]->first) % divisor;    //当前元素应放置的位置

        // 如有必要，将当前元素移至空位
        if (blank < now && (place <= blank || place > now)) {
            swap(table[blank], table[now]);
            blank = now;
        } else if (place > now && place <= blank) {
            swap(table[blank], table[now]);
            blank = now;
        }
        //当前元素后移一位
        now = (now + 1) % divisor;
    }
}

// overload <<
template<class K, class E>
ostream &operator<<(ostream &out, const hashTable<K, E> &x) {
    x.output(out);
    return out;
}


int main() {
    hashTable<int, int> a(11);
    a.insert({80, 100});
    a.insert({40, 700});
    a.insert({65, 100});
    a.insert({58, 700});
    a.insert({24, 100});
    a.insert({35, 700});
    a.insert({98, 100});
    a.insert({46, 700});
    a.insert({18, 100});
    a.insert({79, 700});
    a.insert({31, 100});

    cout << a << endl;

    a.erase(58);

    cout << a << endl;

}