#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 100

typedef struct {
    int top;
    double data[MAX_SIZE];
} OperandStack;

typedef struct {
    int top;
    char data[MAX_SIZE];
} OperatorStack;

// Initialize stacks
void initOperandStack(OperandStack* s) {
    s->top = -1;
}

void initOperatorStack(OperatorStack* s) {
    s->top = -1;
}

// Stack operations
void pushOperand(OperandStack* s, double value) {
    if (s->top < MAX_SIZE - 1) {
        s->data[++s->top] = value;
    }
    else {
        printf("Operand stack overflow\n");
        exit(EXIT_FAILURE);
    }
}

void pushOperator(OperatorStack* s, char op) {
    if (s->top < MAX_SIZE - 1) {
        s->data[++s->top] = op;
    }
    else {
        printf("Operator stack overflow\n");
        exit(EXIT_FAILURE);
    }
}

double popOperand(OperandStack* s) {
    if (s->top > -1) {
        return s->data[s->top--];
    }
    else {
        printf("Operand stack underflow\n");
        exit(EXIT_FAILURE);
    }
}

char popOperator(OperatorStack* s) {
    if (s->top > -1) {
        return s->data[s->top--];
    }
    else {
        printf("Operator stack underflow\n");
        exit(EXIT_FAILURE);
    }
}

// Check if character is a digit
int isDigit(char c) {
    return c >= '0' && c <= '9';
}

// Check if character is an operator
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

size_t strlen(const char* str) {
    const char* s;
    for (s = str; *s; ++s);
    return (s - str);
}

int isOperatorStackEmpty(OperatorStack* s) {
    return s->top == -1;
}

// Perform an operation
double performOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': return a / b;
    default:
        printf("Invalid operator\n");
        exit(EXIT_FAILURE);
    }
}

// Print the current state of both stacks
void printStacks(const OperandStack* operandStack, const OperatorStack* operatorStack) {
    printf("Operand Stack : ");
    for (int i = 0; i <= operandStack->top; i++) {
        printf("%.2f ", operandStack->data[i]);
    }
    printf("\nOperator Stack: ");
    for (int i = 0; i <= operatorStack->top; i++) {
        // Skip '(' and ')' when printing the operator stack
        if (operatorStack->data[i] != '(' && operatorStack->data[i] != ')') {
            printf("%c ", operatorStack->data[i]);
        }
    }
    printf("\n\n");
}
// Operator precedence
int precedence(char oper) {
    switch (oper) {
    case '(': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    default: return -1;
    }
}

void evaluate(OperandStack* operands, OperatorStack* operators) {
    while (!isOperatorStackEmpty(operators) &&
        precedence(operators->data[operators->top]) >= precedence('*')) {
        double right = popOperand(operands);
        double left = popOperand(operands);
        char op = popOperator(operators);
        double result = performOperation(left, right, op);
        pushOperand(operands, result);

        printStacks(operands, operators); // Print stacks after each operation
    }
}

double infix2StackEval(char* expr) {
    OperandStack operands;
    OperatorStack operators;

    operands.top = -1;
    operators.top = -1;

    // Evaluate each character of the expression
    for (int i = 0; expr[i] != '\0'; i++) {
        if (expr[i] == ' ') continue;

        // If it's a number, parse it fully
        if (isDigit(expr[i])) {
            double value = 0;
            while (isDigit(expr[i]) || expr[i] == '.') {
                if (expr[i] == '.') {
                    i++;
                    double decimal = 0.1;
                    while (isDigit(expr[i])) {
                        value += (expr[i] - '0') * decimal;
                        decimal /= 10;
                        i++;
                    }
                }
                else {
                    value = (value * 10) + (expr[i] - '0');
                    i++;
                }
            }
            i--; // Last increment will move to next character, step back
            pushOperand(&operands, value);
        }
        else if (expr[i] == '(') {
            pushOperator(&operators, expr[i]);
        }
        else if (expr[i] == ')') {
            while (operators.data[operators.top] != '(') {
                double right = popOperand(&operands);
                double left = popOperand(&operands);
                char op = popOperator(&operators);
                double result = performOperation(left, right, op);
                pushOperand(&operands, result);
            }
            popOperator(&operators); // Pop the '('
        }
        else if (isOperator(expr[i])) {
            evaluate(&operands, &operators);
            pushOperator(&operators, expr[i]);
        }

        printStacks(&operands, &operators); // Print stacks after each token
    }

    // Evaluate remaining operators
    evaluate(&operands, &operators);

    return popOperand(&operands); // Final result
}
int main() {
    char expr[MAX_SIZE];
    printf("Enter the infix expression (Max length:%d): ", MAX_SIZE);
    fgets(expr, MAX_SIZE, stdin);

    // Handling newline from fgets
    if (expr[strlen(expr) - 1] == '\n') {
        expr[strlen(expr) - 1] = '\0';
    }

    double result = infix2StackEval(expr);
    printf("Final result: %.2f\n", result);

    return 0;
}