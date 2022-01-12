#include <iostream>

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
class arrayStack{
public:
    explicit arrayStack(int initialCapacity = 10);
    ~arrayStack() { delete[] stack; }

    bool empty() const { return stackTop == -1; }
    int size() const { return stackTop + 1; }
    T &top();
    void pop();
    void push(const T &theElement);

private:
    int stackTop;         // current top of stack
    int arrayLength;      // stack capacity
    T *stack;           // element array
};

template<class T>
arrayStack<T>::arrayStack(int initialCapacity) {// Constructor.
    if (initialCapacity < 1) {
        throw invalid_argument("Initial Capacity must be > 0.");
    }
    arrayLength = initialCapacity;
    stack = new T[arrayLength];
    stackTop = -1;
}

template<class T>
void arrayStack<T>::push(const T &theElement) {
    if (stackTop == arrayLength - 1) {
        changeLength1D(stack, arrayLength, 2 * arrayLength);
        arrayLength *= 2;
    }
    stack[++stackTop] = theElement;
}

template<class T>
T &arrayStack<T>::top() {
    if (stackTop == -1)
        throw invalid_argument("Stack Empty.");
    return stack[stackTop];
}

template<class T>
void arrayStack<T>::pop() {
    if (stackTop == -1)
        throw invalid_argument("Stack Empty.");
    stack[stackTop--].~T();
}

int stackNumber(char c){
    if (c == '(' || c == ')')return 0;
    else if (c == '[' || c == ']')return 1;
    else return -1;
}

void printMatchedPairs(const string& expr){
    arrayStack<int> stack;
    int length = (int)expr.size();
    for (int i = 0; i < length; ++i) {
        switch (expr.at(i)) {
            case '(':
            case '[': stack.push(i);break;
            case ']':{
                // 遇到右中括号，弹出其与左中括号之间的所有小括号
                while (!stack.empty() && expr.at(stack.top()) == '(') {
                    cout << "No matching for ( at " << stack.top() << endl;
                    stack.pop();
                }

                if (!stack.empty()) {   //如果弹出小括号后栈顶元素为中括号，则匹配
                    cout << "[" << stack.top() << " " << i << "]" << endl;
                    stack.pop();
                } else {    //否则不匹配
                    cout << "No matching for " << expr.at(i) << " at " << i << endl;
                }
                break;
            }
            case ')':{
                if (!stack.empty() && expr.at(stack.top()) == '(') {
                    cout << "(" << stack.top() << " " << i << ")" << endl;
                    stack.pop();
                } else{
                    cout << "No matching for " << expr.at(i) << " at " << i << endl;
                }
                break;
            }
            default:break;
        };
    }
    while (!stack.empty()) {    // 输出栈中剩下的不匹配的括号
        cout << "No matching for " << expr.at(stack.top()) << " at " << stack.top() << endl;
        stack.pop();
    }
}

int main(){
//    string s = "[au(gal((ak)])";
//    string s = "((  (((((((((((((([))))))))))))";
string  s="(a+b)*[(b+c))*i]]";
    printMatchedPairs(s);
    return 0;
}
