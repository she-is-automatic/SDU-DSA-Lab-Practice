// 输入数学表达式，输出表达式的计算结果。
// 数学表达式由单个数字和运算符“+”、“-”、“*”、“/”、“(”、“) ”构成
// 例如 2+3*(4+5)–6/4。
#include <iostream>
#include <iomanip>

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
class arrayStack {
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


double calculateTool(double a, double b, char signal) {
    switch (signal) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            break;
    }
}

void updateStack(arrayStack<double> &dataStack, arrayStack<char> &operatorStack){
    double b = dataStack.top();
    dataStack.pop();
    double a = dataStack.top();
    dataStack.pop();
    char signal = operatorStack.top();
    operatorStack.pop();
    dataStack.push(calculateTool(a, b, signal));
}

double calculator(const string &expr) {
    arrayStack<double> dataStack;
    arrayStack<char> operatorStack;
    for (int i = 0; i < expr.size(); ++i) {
        char c = expr.at(i);
        if (c >= '0' && c <= '9') { //数直接入数栈
            dataStack.push((double) (c - '0'));
        }
        else if (c == '(' || c == '+') {    //左括号直接入栈，加法运算优先级低，直接入栈
            operatorStack.push(c);
        }
        else if (c == '-') {  //由于栈先进后出，故最后表达式倒序运算，故须将其转化为加法
            if (expr.at(i + 1) != '(') { // 将a-b转化为a+(-b)
                dataStack.push((-1) * (double) (expr.at(++i) - '0'));
                operatorStack.push('+');
            } else {    //将 a-(b+c)*d 转化为 a+(-1)*(b+c)*d
                operatorStack.push('+');
                operatorStack.push('*');
                dataStack.push(-1);
            }
        }
        else if (c == '*' || c == '/') {   //乘除法
            if (expr.at(i + 1) == '(') operatorStack.push(c);   //下一位是小括号，直接入栈
            else {   //若乘除号的下一位不是小括号，而是数字，就将下一位数字与数栈顶的元素进行运算后压栈
                double a = dataStack.top();
                dataStack.pop();
                dataStack.push(calculateTool(a, (double) (expr.at(++i) - '0'), c));
            }
        }
        else if (c == ')') {
            while (operatorStack.top() != '(') {    //运算括号中的结果，数栈弹出2个元素，符号栈弹出一个符号，运算后压入数栈，直到遇到左括号为止
                updateStack(dataStack, operatorStack);
            }
            operatorStack.pop();    //弹出左括号
            if (!operatorStack.empty() && (operatorStack.top() == '*' || operatorStack.top() == '/')) {
                updateStack(dataStack, operatorStack);
            }
        }
    }
    while (!operatorStack.empty()) {    //栈中剩余元素运算
        updateStack(dataStack, operatorStack);
    }
    return dataStack.top();
}

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        cout << setiosflags(ios::fixed) << setprecision(2) << calculator(s) << endl;
    }
    return 0;
}