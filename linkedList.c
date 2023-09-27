#define _CRT_SECURE_NO_WARNINGS

/* 
    `true`, `false` 등의 논리 자료형 값을
    사용하기 위해 필요한 헤더 파일.
*/
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100

/* 연결 리스트의 노드 데이터를 나타내는 구조체. */
typedef struct Listdata {
    int id;             // 학번.
    char name[30];      // 이름.
    char phone[20];     // 전화 번호.
    char province[30];  // 거주 지역.
} listData;

/* 연결 리스트의 노드의 메모리 주소를 나타내는 자료형. */
typedef struct listNode *type_listNode_pointer;

/* 연결 리스트의 노드. */
typedef struct listNode {
    type_listNode_pointer link;  // 다음 노드의 메모리 주소.
    listData data;    // 노드에 저장된 데이터.
} listNode;

/* 연결 리스트를 나타내는 추상 자료형 (abstract data type, ADT). */
typedef struct Linkedlist {
    type_listNode_pointer head;  // 연결 리스트의 첫 번째 노드.
    int length;       // 연결 리스트의 총 노드 개수.
} linkedList;

/* 연결 리스트에 새로운 노드를 추가한다. */
static void insertLast(linkedList *L, listData x);

/* 연결 리스트의 내용을 차례대로 출력한다. */
static void printList(linkedList *L);

/* 연결 리스트에서 학번이 `x`인 노드를 찾는다. */
static type_listNode_pointer search(linkedList *L, int x);

/* 노드 데이터가 `x`인 연결 리스트 노드를 생성하고, `p` 뒤에 삽입한다. */
static void insert(linkedList *L, listNode *p, listData x);

/* 연결 리스트에서 학번이 `x`인 노드를 찾아 삭제한다. */
static void delete(linkedList *L, int x);

/* 연결 리스트의 모든 노드를 역순으로 배치한다.*/
static void reverse(linkedList *L);

/* 연결 리스트의 총 노드 개수를 반환한다. */
static int getLength(linkedList *L);

/* 입력 데이터의 파일 이름. */
const char *file_name = "student_data.txt";

int main(void) {
    linkedList L = { 
        .head = NULL,
        .length = 0
    };

    {
        // 파일에서 입력 데이터를 읽는다.
        FILE *fp = fopen(file_name, "r");

        if (fp == NULL) {
            fprintf(stderr, "Error! can't open the file.\n", file_name);

            return 1;
        }

        char buffer[MAX_INPUT_SIZE];

        // 입력 데이터를 한 줄씩 읽는다.
        while (fgets(buffer, MAX_INPUT_SIZE, fp) != NULL) {
            listData x = { .id = -1 };

            sscanf(
                buffer,
                "%s %d %s %s",
                x.name,     // 이름.
                &x.id,      // 학번.
                x.phone,    // 전화 번호.
                x.province  // 거주 지역.
            );

            // 새로운 노드를 추가한다. 
            insertLast(&L, x);
        }

        fclose(fp);
    }

    puts("=== LinkedList program ===\n");

    // 무한 반복
    for (;;) {
        printf("Insert commend: ");
        
        char input[MAX_INPUT_SIZE];

        // 표준 입력 스트림으로부터 명령어를 한 줄 통째로 받는다.
        fgets(input, MAX_INPUT_SIZE, stdin);

        // 마지막 줄의 개행 문자를 제거한다.
        input[strcspn(input, "\r\n")] = 0;

        // 사용자로부터 입력받은 데이터.
        listData x = { .id = -1 };

        // `true`일 경우, 프로그램을 종료한다.
        bool should_exit = false;

        // 명령 줄의 문자열 토큰.
        char *token = strtok(input, " ");

        // 사용자로부터 추가로 입력받은 학번 정보.
        int query_id = -1;
        
        // 명령어의 첫 번째 글자부터 확인한다.
        switch (input[0]) {
            case 'p':
                // `print`
                if (strncmp(input, "print", 5) != 0) {
                    puts("Error! print failed. please try again.");

                    break;
                }
                
                printList(&L);
                
                break;

            case 's':
                // `search`
                if (strncmp(input, "search", 6) != 0) {
                    puts("Error! search failed. please try again.");

                    break;
                }
                
                token = strtok(NULL, " ");

                if (token == NULL) {
                    puts("Commend: search (id)\n");

                    break;
                }

                x.id = atoi(token);

                listNode *ptr = search(&L, x.id);

                if (ptr != NULL) printf("Node is located at %p.\n", ptr);
                else printf("Error! no such node.\n");
                
                break;

            case 'i':
                // `insert`
                if (strncmp(input, "insert", 6) != 0) {
                    puts("Error! insertion failed. please try again.");

                    break;
                }

                token = strtok(NULL, ", "), query_id = atoi(token);
                token = strtok(NULL, ", "), x.id = atoi(token);
                token = strtok(NULL, ", "), strcpy(x.name, token);
                token = strtok(NULL, ", "), strcpy(x.phone, token);
                token = strtok(NULL, ", "), strcpy(x.province, token);

                // 먼저 학번이 `query_id`인 노드를 찾는다.
                listNode *node = search(&L, query_id);

                insert(&L, node, x);
                
                break;

            case 'd':
                // `delete`
                if (strncmp(input, "delete", 6) != 0) {
                    puts("Error! deletion failed. please try again.");

                    break;
                }
                // token = strtok(NULL, " "), x.id = atoi(token);
                // token = strtok(NULL, " "), x.id = atoi(token);
                sscanf(input, "%*s %d", &input, &x);
                // gets();

                delete(&L, x.id);
                
                break;

            case 'r':
                // `reverse`
                if (strncmp(input, "reverse", 7) != 0) {
                    puts("Error! reverse failed. please try again.");

                    break;
                }

                reverse(&L);

                printList(&L);
                
                break;

            case 'g':
                // `getLength`
                if (strncmp(input, "getLength", 9) != 0) {
                    puts("Error! getLength failed. please try again.");

                    break;
                }

                printf("There are %d nodes in the list.\n", getLength(&L));
                
                break;

            case 'e':
                // `exit`
                if (strncmp(input, "exit", 4) != 0) {
                    puts("Error! exit failed. please try again.");

                    break;
                }

                puts("프로그램을 종료합니다...");

                should_exit = true;

                break;
            
            default:
                puts("Error! please check the commend.");

                break;
        }

        if (should_exit) break;

        puts("");
    }

    while (L.head != NULL) {
        listNode *next = L.head->link;

        free(L.head);

        L.head = next;
    }

    return 0;
}

/* 연결 리스트에 새로운 노드를 추가한다. */
static void insertLast(linkedList *L, listData x) {
    if (L == NULL) return;

    // 새로운 노드를 생성한다.
    listNode *node = malloc(sizeof(*node));

    node->data = x, node->link = NULL;

    // 연결 리스트가 비어 있는가?
    if (L->head == NULL) {
        // 그렇다면 방금 만든 노드를 연결 리스트의 첫 번째 노드로 설정한다.

        L->head = node;
    } else {
        listNode *head = L->head;

        // 첫 번째 노드에서부터 마지막 노드까지 이동한다.
        while (head->link != NULL)
            head = head->link;

        // 방금 만든 노드가 연결 리스트의 마지막 노드가 된다.
        head->link = node;
    }

    L->length++;
}

/* 연결 리스트의 내용을 차례대로 출력한다. */
static void printList(linkedList *L) {
    if (L == NULL) return;

    listNode *head = L->head;

    while (head != NULL) {
        printf(
            ":: %s, %d, %s, %s\n",
            head->data.name,
            head->data.id,
            head->data.phone,
            head->data.province
        );

        head = head->link;
    }
}

/* 연결 리스트에서 학번이 `x`인 노드를 찾는다. */
static type_listNode_pointer search(linkedList *L, int x) {
    if (L == NULL) return NULL;

    listNode *head = L->head;

    // 노드를 찾았다면 `head`는 `NULL`이 아닌 값일 것이다.
    while (head != NULL) {
        if (head->data.id == x) 
            break;

        head = head->link;
    }

    return head;
}

/* 노드 데이터가 `x`인 연결 리스트 노드를 생성하고, `p` 뒤에 삽입한다. */
static void insert(linkedList *L, listNode *p, listData x) {
    if (L == NULL) return;
    else if (p == NULL) {
        printf("Error! no such node found!\n");

        return;
    } else {
        listNode *node = malloc(sizeof(*node));

        node->data.id = x.id;

        strcpy(node->data.name, x.name);
        strcpy(node->data.phone, x.phone);
        strcpy(node->data.province, x.province);

        const listNode *next = p->link;

        /*
            [P]는 `p`, [Q]는 `next`, 즉 `p`의 다음 노드, [N]은 `node`라고 할 때,

            우리가 해야 할 일은?

            1. [P] -> [N]          ([P]의 다음 노드를 [N]으로 설정한다.)
            2. [P] -> [N] -> [Q]   ([N]의 다음 노드를 [Q]로 설정한다.)
        */

        p->link = node;
        node->link = (listNode*)next;

        printList(L);
        
        L->length++;
    }
}

/* 연결 리스트에서 학번이 `x`인 노드를 찾아 삭제한다. */
static void delete(linkedList* L, int x) {
    if (L == NULL) {
        printf("Error! List is empty!\n");

        return;
    }

    listNode* head = L->head;
    listNode* pre = NULL;

    while (head != NULL) {
        if (head->data.id == x) break;
        pre = head, head = head->link;
    }
    if (head != NULL) {
        listNode* next = head->link;

        free(head);

        pre->link = next;

        printf("List after the deletion.\n");
        printList(L);
        L->length--;
    } else {
        printf("Error! no such node in the list.\n");
    }
}

/* 연결 리스트의 모든 노드를 역순으로 배치한다.*/
static void reverse(linkedList *L) {
    if (L == NULL) return;

    listNode *ptr = L->head, *pre = NULL, *nxt = NULL;

    while (ptr != NULL) {
        /*
            [ A ] -> [ B ] -> [ C ] -> [ D ] -> [ E ]
            `pre`    `ptr`    `nxt`
        */

        nxt = ptr->link;

        /*
            [ A ] <- [ B ]    [ C ] -> [ D ] -> [ E ]
            `pre`    `ptr`    `nxt`
        */

        ptr->link = pre;

        /*
            [ A ] <- [ B ]    [ C ] -> [ D ] -> [ E ]
                     `pre`    `ptr`    `nxt`
        */

        pre = ptr, ptr = nxt;
    }

    L->head = pre;
}

/* 연결 리스트의 총 노드 개수를 반환한다. */
static int getLength(linkedList *L) {
    if (L != NULL) {
        return L->length;
    }
}