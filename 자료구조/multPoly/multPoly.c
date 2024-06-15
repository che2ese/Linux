#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    int coef; // 계수
    int expX; // x의 지수
    int expY; // y의 지수
} Term;

typedef struct {
    Term* terms; // 항들의 배열
    int size;    // 다항식의 항 개수
    int capacity; // 배열의 최대 크기
} Polynomial;

Polynomial createPolynomial(int capacity) {
    Polynomial p;
    p.size = 0;
    p.capacity = capacity;
    p.terms = (Term*)malloc(capacity * sizeof(Term));
    return p;
}

void freePolynomial(Polynomial* p) {
    free(p->terms);
    p->terms = NULL;
    p->size = 0;
    p->capacity = 0;
}

int compareTerms(const void* a, const void* b) {
    const Term* termA = (const Term*)a;
    const Term* termB = (const Term*)b;
    if (termA->expX == termB->expX) return termB->expY - termA->expY;
    return termB->expX - termA->expX;
}

Polynomial multPoly(Polynomial a, Polynomial b) {
    Polynomial result = createPolynomial(a.size * b.size);
    for (int i = 0; i < a.size; ++i) {
        for (int j = 0; j < b.size; ++j) {
            int newCoef = a.terms[i].coef * b.terms[j].coef;
            int newExpX = a.terms[i].expX + b.terms[j].expX;
            int newExpY = a.terms[i].expY + b.terms[j].expY;
            int found = 0;
            for (int k = 0; k < result.size; ++k) {
                if (result.terms[k].expX == newExpX && result.terms[k].expY == newExpY) {
                    result.terms[k].coef += newCoef;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                result.terms[result.size].coef = newCoef;
                result.terms[result.size].expX = newExpX;
                result.terms[result.size].expY = newExpY;
                result.size++;
            }
        }
    }
    return result;
}

void printPoly(Polynomial p) {
    int isFirstTerm = 1;
    for (int i = 0; i < p.size; ++i) {
        if (p.terms[i].coef == 0) continue;
        if (p.terms[i].coef > 0 && !isFirstTerm) printf(" + ");
        else if (p.terms[i].coef < 0) printf(" - ");
        printf("%d", abs(p.terms[i].coef));
        if (p.terms[i].expX > 0) printf("x^%d", p.terms[i].expX);
        if (p.terms[i].expY > 0) printf("y^%d", p.terms[i].expY);
        isFirstTerm = 0;
    }
    if (isFirstTerm) printf("0");
    printf("\n");
}

void sortPoly(Polynomial* p) {
    qsort(p->terms, p->size, sizeof(Term), compareTerms);
}

Polynomial parsePoly(char* input) {
    Polynomial p = createPolynomial(100); 
    char* token = strtok(input, " +");
    while (token) {
        int coef = 0, expX = 0, expY = 0;
        sscanf(token, "%d", &coef);
        char* xPart = strstr(token, "x^");
        char* yPart = strstr(token, "y^");
        if (xPart) sscanf(xPart, "x^%d", &expX);
        if (yPart) sscanf(yPart, "y^%d", &expY);
        p.terms[p.size++] = (Term){coef, expX, expY};
        token = strtok(NULL, " +");
    }
    return p;
}

int main() {
    char inputA[256], inputB[256];

    printf("Enter polynomial A: ");
    fgets(inputA, sizeof(inputA), stdin);
    Polynomial polyA = parsePoly(inputA);

    printf("Enter polynomial B: ");
    fgets(inputB, sizeof(inputB), stdin);
    Polynomial polyB = parsePoly(inputB);

    Polynomial result = multPoly(polyA, polyB);
    sortPoly(&result);
    printPoly(result);

    freePolynomial(&polyA);
    freePolynomial(&polyB);
    freePolynomial(&result);

    return 0;
}