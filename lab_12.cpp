//创建无向图类，存储结构使用邻接链表，提供操作：插入一条边，删除一条边，BFS，DFS。

//输入：
//第一行四个整数n，m，s，t。n (10≤n≤100000) 代表图中点的个数，m (10≤m≤200000) 代表接下来共有m个操作，s代表起始点，t代表终点。
//接下来m行，每行代表一次插入或删除边的操作，操作格式为：
//0 u v 在点u和v之间增加一条边；
//1 u v 删除点u和v之间的边。

//输出：
//第一行输出图中有多少个连通分量；
//第二行输出所有连通子图中最小点的编号（升序），编号间用空格分隔；
//第三行输出从s点开始的dfs序列长度；
//第四行输出从s点开始的字典序最小的dfs序列；
//第五行输出从t点开始的bfs序列的长度；
//第六行输出从t点开始字典序最小的bfs序列；
//第七行输出从s点到t点的最短路径，若是不存在路径则输出-1。

#include <iostream>
#include <stack>
#include <queue>
#include <sstream>

using namespace std;

template< class T >
class edge {
public:
    virtual ~edge() {};
    virtual int vertex1() const = 0;
    virtual int vertex2() const = 0;
    virtual T weight() const = 0;
};

template< class T >
class vertexIterator {
public:
    virtual ~vertexIterator() {}

    virtual int next() = 0;
    virtual int next(T &) = 0;
};


class unweightedEdge : public edge<bool>
{
public:
    unweightedEdge(int theV1, int theV2)
    {v1 = theV1; v2 = theV2;}
    ~unweightedEdge() {};
    int vertex1() const {return v1;}
    int vertex2() const {return v2;}
    bool weight() const {throw invalid_argument("weight() undefined");}
private:
    int v1, v2;
};


template< class T >
class graph {
public:
    virtual ~graph() {}

    virtual int numberOfVertices() const = 0;
    virtual int numberOfEdges() const = 0;
    virtual bool existsEdge(int, int) const = 0;
    virtual void insertEdge(edge<T> *) = 0;
    virtual void eraseEdge(int, int) = 0;
    virtual int degree(int) const = 0;
    virtual int inDegree(int) const = 0;
    virtual int outDegree(int) const = 0;
    virtual bool directed() const = 0;
    virtual bool weighted() const = 0;
    virtual vertexIterator<T> *iterator(int) = 0;
    virtual void output(ostream &) const = 0;

    // implementation independent methods
    virtual void bfs(int v, int reach[], int label) {// 宽度优先搜索
        queue<int> q;
        reach[v] = label;   // 起始顶点标记后入队列
        q.push(v);

        while ( !q.empty()) {
            // 获取队列首的顶点
            int w = q.front();
            q.pop();

            // 标记队首元素未被标记过的邻接顶点，并将其入队列
            vertexIterator<T> *iw = iterator(w);
            int u;
            while ((u = iw->next()) != 0 )
                // u是邻接顶点
                if ( reach[u] == 0 ) {// 未到达
                    q.push(u);
                    reach[u] = label; // mark reached
                }
            delete iw;
        }
    }

    virtual void dfs(int v, int reach[], int label) {// 深度优先搜索
        // vertices reachable from vertex v
        graph<T>::reach = reach;
        graph<T>::label = label;
        rDfs(v);
    }

    int labelComponents(int c[]) {// 标记连通分支，并返回连通分支的数目
        if ( directed())
            throw invalid_argument("graph::labelComponents() not defined for directed graphs");

        int n = numberOfVertices();

        // 初始化
        for ( int i = 1; i <= n; i++ )
            c[i] = 0;

        label = 0;  // 最后一个连通分支的标记

        for ( int i = 1; i <= n; i++ )
            if ( c[i] == 0 )  // 未被标记过的连通分支
            {
                label++;
                graph<T>::bfs(i, c, label); // 将该分支的所有顶点标记同样的序号
            }

        return label;
    }

protected:
    static int *reach;
    static int *path;
    static int label;
    static int length;
    static int destination;


    virtual void rDfs(int v) {// 递归DFS
        reach[v] = label;
        vertexIterator<T> *iv = iterator(v);
        int u;
        while ((u = iv->next()) != 0 )
            if ( reach[u] == 0 )
                rDfs(u);  // 对未标记的顶点，继续DFS
        delete iv;
    }


};


template< class T >
struct chainNode {
    T element;
    chainNode<T> *next;

    // methods
    chainNode() {}

    chainNode(const T &element) { this->element = element; }

    chainNode(const T &element, chainNode<T> *next) {
        this->element = element;
        this->next = next;
    }
};

template< class T >
class chain {
public:
    chain(int initialCapacity = 10);
    chain(const chain<T> &);
    ~chain();

    // ADT
    bool empty() const { return listSize == 0; }
    int size() const { return listSize; }
    T &get(int theIndex) const;
    int indexOf(const T &theElement) const;
    void erase(int theIndex);
    void insert(const T &theElement);
    void output(ostream &out) const;

protected:
    void checkIndex(int theIndex) const;
    chainNode<T> *firstNode;
    int listSize;
};

template< class T >
chain<T>::chain(int initialCapacity) {
    if ( initialCapacity < 1 ) throw ;

    firstNode = NULL;
    listSize = 0;
}

template< class T >
chain<T>::chain(const chain<T> &theList) {
    listSize = theList.listSize;

    if ( listSize == 0 ) {
        firstNode = NULL;
        return;
    }

    chainNode<T> *sourceNode = theList.firstNode;
    firstNode = new chainNode<T>(sourceNode->element);
    sourceNode = sourceNode->next;
    chainNode<T> *targetNode = firstNode;
    while ( sourceNode != NULL ) {
        targetNode->next = new chainNode<T>(sourceNode->element);
        targetNode = targetNode->next;
        sourceNode = sourceNode->next;
    }
    targetNode->next = NULL;
}

template< class T >
chain<T>::~chain() {
    while ( firstNode != NULL ) {
        chainNode<T> *nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

template< class T >
void chain<T>::checkIndex(int theIndex) const {
    if ( theIndex < 0 || theIndex >= listSize )throw ;
}

template< class T >
T &chain<T>::get(int theIndex) const {
    checkIndex(theIndex);

    chainNode<T> *currentNode = firstNode;
    for ( int i = 0; i < theIndex; i++ )
        currentNode = currentNode->next;

    return currentNode->element;
}

template< class T >
int chain<T>::indexOf(const T &theElement) const {
    chainNode<T> *currentNode = firstNode;
    int index = 0;
    while ( currentNode != NULL &&
            currentNode->element != theElement ) {
        currentNode = currentNode->next;
        index++;
    }

    if ( currentNode == NULL )
        return -1;
    else
        return index;
}

template< class T >
void chain<T>::erase(int theIndex) {
    checkIndex(theIndex);

    chainNode<T> *deleteNode;
    if ( theIndex == 0 ) {
        deleteNode = firstNode;
        firstNode = firstNode->next;
    } else {
        chainNode<T> *p = firstNode;
        for ( int i = 0; i < theIndex - 1; i++ )
            p = p->next;

        deleteNode = p->next;
        p->next = p->next->next;
    }
    listSize--;
    delete deleteNode;
}

template<typename T>
void chain<T>::insert(const T& theElement)
{

    chainNode<T>* p = firstNode,
            * tp = NULL; // tp trails p

    while (p != NULL && p->element < theElement)   //插在tp和p之间
    {
        tp = p;
        p = p->next;
    }

    if (p != NULL && p->element == theElement)    //已经有这条边了，两个点已经连起来了
    {
        return;
    }

    chainNode<T>* newNode = new chainNode<T>(theElement,p);

    if (tp == NULL) firstNode = newNode;
    else tp->next = newNode;

    listSize++;
    return;
}


template< class T >
void chain<T>::output(ostream &out) const {
    for ( chainNode<T> *currentNode = firstNode;
          currentNode != NULL;
          currentNode = currentNode->next )
        out << currentNode->element << "  ";
}

template< class T >
ostream &operator<<(ostream &out, const chain<T> &x) {
    x.output(out);
    return out;
}

template< class T >
class graphChain : public chain<T> {
public:
    friend class linkedDigraph;
    friend class linkedGraph;




    T *eraseElement(int theVertex) {    // 扩充chain类，按照元素删除

        chainNode<T> *current = this->firstNode,
                *trail = NULL;

        while ( current != NULL && current->element != theVertex ) {
            trail = current;
            current = current->next;
        }

        if ( current == NULL )
            return NULL;


        T *theElement = &current->element;

        if ( trail != NULL )
            trail->next = current->next;
        else
            this->firstNode = current->next;

        delete current;
        this->listSize--;
        return theElement;
    }
};

template<class T> int* graph<T>::reach;
template<class T> int graph<T>::label;
template<class T> int *graph<T>::path;



class linkedDigraph : public graph<bool>
{
protected:
    int n;                      // 顶点数
    int e;                      // 边数
    graphChain<int> *aList;     // 邻接链表的数组

public:
    linkedDigraph(int numberOfVertices = 0)
    {
        if (numberOfVertices < 0)
            throw invalid_argument
                    ("number of vertices must be >= 0");
        n = numberOfVertices;
        e = 0;
        aList = new graphChain<int> [n + 1];
    }

    ~linkedDigraph() {delete [] aList;}
    int numberOfVertices() const {return n;}
    int numberOfEdges() const {return e;}
    bool directed() const {return true;}
    bool weighted() const {return false;}
    bool existsEdge(int i, int j) const
    {
        if (i < 1 || j < 1 || i > n || j > n
            || aList[i].indexOf(j) == -1)
            return false;
        else
            return true;
    }

    void insertEdge(edge<bool> *theEdge)
    {
        int v1 = theEdge->vertex1();
        int v2 = theEdge->vertex2();
        if (v1 < 1 || v2 < 1 || v1 > n || v2 > n || v1 == v2)
        {
            ostringstream s;
            s << "(" << v1 << "," << v2
              << ") is not a permissible edge";
            throw invalid_argument(s.str());
        }

        if (aList[v1].indexOf(v2) == -1)    // 要插入的边不存在，插入对应的链表头
        {
            aList[v1].insert(v2);
            e++;
        }
    }

    void insertEdgeNoCheck(edge<bool> *theEdge)
    {
        aList[theEdge->vertex1()].insert(theEdge->vertex2());
        e++;
    }


    void eraseEdge(int i, int j)
    {
        if (i >= 1 && j >= 1 && i <= n && j <= n)
        {
            int *v = aList[i].eraseElement(j);
            if (v != NULL)  // 要删除的边存在，边数-1
                e--;
        }
    }

    void checkVertex(int theVertex) const
    {
        if (theVertex < 1 || theVertex > n)
        {
            ostringstream s;
            s << "no vertex " << theVertex;
            throw invalid_argument(s.str());
        }
    }

    int degree(int theVertex) const
    {throw invalid_argument("degree() undefined");}

    int outDegree(int theVertex) const
    {// 出度
        checkVertex(theVertex);
        return aList[theVertex].size();
    }

    int inDegree(int theVertex) const
    {
        checkVertex(theVertex);
        int sum = 0;
        for (int j = 1; j <= n; j++)
            if (aList[j].indexOf(theVertex) != -1)
                sum++;
        return sum;
    }

    class myIterator : public vertexIterator<bool>
    {
    public:
        myIterator(chainNode<int> *theNode) {currentNode = theNode;}

        ~myIterator() {}

        int next()
        {
            if (currentNode == NULL)
                return 0;
            int nextVertex = currentNode->element;
            currentNode = currentNode->next;
            return nextVertex;
        }

        int next(bool& theWeight)
        {
            if (currentNode == NULL)
                return 0;
            int nextVertex = currentNode->element;
            currentNode = currentNode->next;
            theWeight = true;
            return nextVertex;
        }

    protected:
        chainNode<int> *currentNode;
    };

    myIterator* iterator(int theVertex)
    {
        checkVertex(theVertex);
        return new myIterator(aList[theVertex].firstNode);
    }


    void output(ostream& out) const
    {
        for (int i = 1; i <= n; i++)
            out << i << " -> " << aList[i] << endl;
    }

    void bfs(int v, int reach[], int label)
    {   // 为链表定制的bfs
        for (int i = 0; i <= n; ++i) {
            reach[i] = 0;
        }
        queue<int> q;
        reach[v] = label;
        q.push(v);
        cout << v << " ";

        while (!q.empty())
        {
            int w = q.front();
            q.pop();

            for (chainNode<int>* u = aList[w].firstNode;
                 u != NULL; u = u->next)
                if (reach[u->element] == 0)
                {
                    q.push(u->element);
                    cout << u->element << " ";
                    reach[u->element] = label;
                }
        }
    }


    void dfs(int v1, int reach[], int label) {
        graph::reach = reach;
        graph::label = label;
        rDfs(v1);
    }
    void rDfs(int v1){
        reach[v1] = label;
        cout << v1 << " ";
        chainNode<int> *currentNode = aList[v1].firstNode;
        while (currentNode != NULL) {
            if (reach[currentNode->element] != label) // 该节点没有到达，继续dfs
                rDfs(currentNode->element);
            currentNode = currentNode->next;
        }
    }
};

// overload <<
ostream& operator<<(ostream& out, const linkedDigraph& x)
{x.output(out); return out;}



class linkedGraph : public linkedDigraph {
public:
    linkedGraph(int numberOfVertices = 0) : linkedDigraph(numberOfVertices) {}

    bool directed() const { return false; }

    void insertEdge(edge<bool> *theEdge) {
        int oldE = e;
        linkedDigraph::insertEdge(theEdge);
        if ( e > oldE ) {// new edge
            linkedDigraph::insertEdgeNoCheck(new unweightedEdge(theEdge->vertex2(), theEdge->vertex1()));
            e--;  // 插入反向边
        }
    }

    void insertEdgeNoCheck(edge<bool> *theEdge) {
        linkedDigraph::insertEdgeNoCheck(theEdge);
        linkedDigraph::insertEdgeNoCheck(new unweightedEdge(theEdge->vertex2(), theEdge->vertex1()));
        e--;
    }

    void eraseEdge(int i, int j) {
        int oldE = e;
        linkedDigraph::eraseEdge(i, j);
        if ( oldE > e )
        {
            linkedDigraph::eraseEdge(j, i);
            e++;
        }
    }

    int degree(int theVertex) const {
        checkVertex(theVertex);
        return aList[theVertex].size();
    }

    int outDegree(int theVertex) const { return degree(theVertex); }

    int inDegree(int theVertex) const { return degree(theVertex); }

    int shortestPath(int src, int dest, int path[]){
        if (src == dest) return 0;
        for (int i = 1; i <= n; ++i)
            path[i] = 0;

        int length = 1;
        int size = 1;
        queue<int> q;
        path[src] = 999;
        q.push(src);    // 起点入队列
        while (!q.empty()) {

            while (size > 0) {
                int u = q.front();
                q.pop();
                --size;

                for (chainNode<int> *iu = aList[u].firstNode; iu != NULL; iu = iu->next) {
                    int v = iu->element;
                    if (path[v] == 0) {
                        path[v] = u;
                        q.push(v);
                    }
                }
            }
            size = (int)q.size();

            if (path[dest] == 0) {
                ++length;
            }
        }

        if (path[dest]==0){  // 不存在路径
            return -1;
        }

        // 输出路径
        return length;
    }
};

bool exists(int *a, int n, int val){
    for (int i = 0; i < n; ++i) {
        if (a[i] == val) {
            return true;
        }
    }
    return false;
}

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    linkedGraph graph(n);

    for (int i = 0; i < m; ++i) {
        int ch, u, v;
        cin >> ch >> u >> v;
        switch (ch) {

            case 0:{
                unweightedEdge e(u, v);
                edge<bool> *pe = &e;
                graph.insertEdge(pe);
                break;
            }
            case 1:
                graph.eraseEdge(u, v);
                break;
            default:break;
        }
    }


    int *component_label = new int[n + 1];
    int num_component = graph.labelComponents(component_label);
    cout << num_component << endl;

    int *existLabel=new int [num_component]{0};
    int index = 0;
    for (int i = 1; i <= n; ++i) {
        if (!exists(existLabel, index, component_label[i])) {
            cout << i << " ";
            existLabel[index++] = component_label[i];
        }
    }
    cout << endl;


    int num = 0;
    for (int i = 1; i <= n; ++i) {
        if (component_label[i]==component_label[s])
            ++num;
    }
    cout << num << endl;

    int *dfsReach = new int[n + 1];
    graph.dfs(s, dfsReach, 999);
    cout << endl;


    num = 0;
    for (int i = 1; i <= n; ++i) {
        if (component_label[i]==component_label[t])
            ++num;
    }
    cout << num << endl;
    int *bfsReach = new int[n + 1];
    graph.bfs(t, bfsReach, 999);
    cout << endl;

    int *path = new int[n + 1];
    cout<<graph.shortestPath(s, t, path);

    return 0;
}
