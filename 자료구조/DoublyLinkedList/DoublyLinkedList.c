#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

// 노드 생성 함수
Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("메모리 할당 실패.\n");
        return NULL;
    }
    new_node->data = data;
    new_node->prev = new_node->next = NULL;
    return new_node;
}

// 리스트의 끝에 노드 추가 함수
void append(Node** head, int data) {
    Node* new_node = create_node(data);
    if (!new_node) return;
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node* last = *head;
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    new_node->prev = last;
}

// 두 노드의 위치를 교환하는 함수
void swap_nodes(Node** head, int index1, int index2) {
    if (index1 == index2) return;

    Node* node1 = NULL, * node2 = NULL, * temp = *head;
    int i = 0;
    while (temp != NULL) {
        if (i == index1) node1 = temp;
        if (i == index2) node2 = temp;
        temp = temp->next;
        i++;
    }

    if (node1 == NULL || node2 == NULL) {
        printf("에러: 지정된 인덱스가 범위를 벗어났습니다.\n");
        return;
    }

    // 노드 위치 교환
    if (node1->prev) node1->prev->next = node2;
    if (node2->prev) node2->prev->next = node1;
    if (node1->next) node1->next->prev = node2;
    if (node2->next) node2->next->prev = node1;

    Node* tempPrev = node1->prev;
    Node* tempNext = node1->next;
    node1->prev = node2->prev;
    node1->next = node2->next;
    node2->prev = tempPrev;
    node2->next = tempNext;

    if (node1 == *head) *head = node2;
    else if (node2 == *head) *head = node1;
}

// 연결 리스트 출력 함수
void print_list(Node* head) {
    Node* temp = head;
    while (temp != NULL) {
        printf("%d", temp->data);
        if (temp->next != NULL) {
            printf(" > ");
        }
        temp = temp->next;
    }
    printf("\n");
}

// 연결 리스트 메모리 해제 함수
void free_list(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

int main() {
    Node* head = NULL;
    int data, index1, index2;
    char term;

    printf("자연수 리스트 입력(최대 20개, -1로 종료): ");
    while (scanf("%d%c", &data, &term) == 2 && data != -1) {
        if (data < 0 || term != ' ') {
            printf("에러: 잘못된 입력이 있습니다. 자연수만 입력해 주세요.\n");
            while (getchar() != '\n'); // 입력 버퍼 클리어
            printf("자연수 리스트 입력(최대 20개, -1로 종료): ");
            continue;
        }
        append(&head, data);
        if (term == '\n') break;
    }

    do {
        print_list(head);
        printf("위치 변경 (형식: 인덱스1 인덱스2, 종료: q): ");
        if (scanf("%d %d", &index1, &index2) != 2) {
            char ch;
            scanf("%c", &ch);
            if (ch == 'q' || ch == 'Q') break; // Check for 'q' or 'Q' to exit
            printf("잘못된 입력입니다. 인덱스 두 개를 입력하거나 'q'로 종료하세요.\n");
            while (getchar() != '\n'); // 입력 버퍼 클리어
            continue;
        }
        swap_nodes(&head, index1, index2);
    } while (1);

    print_list(head); // 최종 리스트 출력
    free_list(head); // 동적으로 할당된 모든 메모리 해제
    return 0;
}
