#define _CRT_SECURE_NO_WARNINGS

/* 
    `true`, `false` ���� �� �ڷ��� ����
    ����ϱ� ���� �ʿ��� ��� ����.
*/
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 100

/* ���� ����Ʈ�� ��� �����͸� ��Ÿ���� ����ü. */
typedef struct Listdata {
    int id;             // �й�.
    char name[30];      // �̸�.
    char phone[20];     // ��ȭ ��ȣ.
    char province[30];  // ���� ����.
} listData;

/* ���� ����Ʈ�� ����� �޸� �ּҸ� ��Ÿ���� �ڷ���. */
typedef struct listNode *type_listNode_pointer;

/* ���� ����Ʈ�� ���. */
typedef struct listNode {
    type_listNode_pointer link;  // ���� ����� �޸� �ּ�.
    listData data;    // ��忡 ����� ������.
} listNode;

/* ���� ����Ʈ�� ��Ÿ���� �߻� �ڷ��� (abstract data type, ADT). */
typedef struct Linkedlist {
    type_listNode_pointer head;  // ���� ����Ʈ�� ù ��° ���.
    int length;       // ���� ����Ʈ�� �� ��� ����.
} linkedList;

/* ���� ����Ʈ�� ���ο� ��带 �߰��Ѵ�. */
static void insertLast(linkedList *L, listData x);

/* ���� ����Ʈ�� ������ ���ʴ�� ����Ѵ�. */
static void printList(linkedList *L);

/* ���� ����Ʈ���� �й��� `x`�� ��带 ã�´�. */
static type_listNode_pointer search(linkedList *L, int x);

/* ��� �����Ͱ� `x`�� ���� ����Ʈ ��带 �����ϰ�, `p` �ڿ� �����Ѵ�. */
static void insert(linkedList *L, listNode *p, listData x);

/* ���� ����Ʈ���� �й��� `x`�� ��带 ã�� �����Ѵ�. */
static void delete(linkedList *L, int x);

/* ���� ����Ʈ�� ��� ��带 �������� ��ġ�Ѵ�.*/
static void reverse(linkedList *L);

/* ���� ����Ʈ�� �� ��� ������ ��ȯ�Ѵ�. */
static int getLength(linkedList *L);

/* �Է� �������� ���� �̸�. */
const char *file_name = "student_data.txt";

int main(void) {
    linkedList L = { 
        .head = NULL,
        .length = 0
    };

    {
        // ���Ͽ��� �Է� �����͸� �д´�.
        FILE *fp = fopen(file_name, "r");

        if (fp == NULL) {
            fprintf(stderr, "Error! can't open the file.\n", file_name);

            return 1;
        }

        char buffer[MAX_INPUT_SIZE];

        // �Է� �����͸� �� �پ� �д´�.
        while (fgets(buffer, MAX_INPUT_SIZE, fp) != NULL) {
            listData x = { .id = -1 };

            sscanf(
                buffer,
                "%s %d %s %s",
                x.name,     // �̸�.
                &x.id,      // �й�.
                x.phone,    // ��ȭ ��ȣ.
                x.province  // ���� ����.
            );

            // ���ο� ��带 �߰��Ѵ�. 
            insertLast(&L, x);
        }

        fclose(fp);
    }

    puts("=== LinkedList program ===\n");

    // ���� �ݺ�
    for (;;) {
        printf("Insert commend: ");
        
        char input[MAX_INPUT_SIZE];

        // ǥ�� �Է� ��Ʈ�����κ��� ��ɾ �� �� ��°�� �޴´�.
        fgets(input, MAX_INPUT_SIZE, stdin);

        // ������ ���� ���� ���ڸ� �����Ѵ�.
        input[strcspn(input, "\r\n")] = 0;

        // ����ڷκ��� �Է¹��� ������.
        listData x = { .id = -1 };

        // `true`�� ���, ���α׷��� �����Ѵ�.
        bool should_exit = false;

        // ��� ���� ���ڿ� ��ū.
        char *token = strtok(input, " ");

        // ����ڷκ��� �߰��� �Է¹��� �й� ����.
        int query_id = -1;
        
        // ��ɾ��� ù ��° ���ں��� Ȯ���Ѵ�.
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

                // ���� �й��� `query_id`�� ��带 ã�´�.
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

                puts("���α׷��� �����մϴ�...");

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

/* ���� ����Ʈ�� ���ο� ��带 �߰��Ѵ�. */
static void insertLast(linkedList *L, listData x) {
    if (L == NULL) return;

    // ���ο� ��带 �����Ѵ�.
    listNode *node = malloc(sizeof(*node));

    node->data = x, node->link = NULL;

    // ���� ����Ʈ�� ��� �ִ°�?
    if (L->head == NULL) {
        // �׷��ٸ� ��� ���� ��带 ���� ����Ʈ�� ù ��° ���� �����Ѵ�.

        L->head = node;
    } else {
        listNode *head = L->head;

        // ù ��° ��忡������ ������ ������ �̵��Ѵ�.
        while (head->link != NULL)
            head = head->link;

        // ��� ���� ��尡 ���� ����Ʈ�� ������ ��尡 �ȴ�.
        head->link = node;
    }

    L->length++;
}

/* ���� ����Ʈ�� ������ ���ʴ�� ����Ѵ�. */
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

/* ���� ����Ʈ���� �й��� `x`�� ��带 ã�´�. */
static type_listNode_pointer search(linkedList *L, int x) {
    if (L == NULL) return NULL;

    listNode *head = L->head;

    // ��带 ã�Ҵٸ� `head`�� `NULL`�� �ƴ� ���� ���̴�.
    while (head != NULL) {
        if (head->data.id == x) 
            break;

        head = head->link;
    }

    return head;
}

/* ��� �����Ͱ� `x`�� ���� ����Ʈ ��带 �����ϰ�, `p` �ڿ� �����Ѵ�. */
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
            [P]�� `p`, [Q]�� `next`, �� `p`�� ���� ���, [N]�� `node`��� �� ��,

            �츮�� �ؾ� �� ����?

            1. [P] -> [N]          ([P]�� ���� ��带 [N]���� �����Ѵ�.)
            2. [P] -> [N] -> [Q]   ([N]�� ���� ��带 [Q]�� �����Ѵ�.)
        */

        p->link = node;
        node->link = (listNode*)next;

        printList(L);
        
        L->length++;
    }
}

/* ���� ����Ʈ���� �й��� `x`�� ��带 ã�� �����Ѵ�. */
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

/* ���� ����Ʈ�� ��� ��带 �������� ��ġ�Ѵ�.*/
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

/* ���� ����Ʈ�� �� ��� ������ ��ȯ�Ѵ�. */
static int getLength(linkedList *L) {
    if (L != NULL) {
        return L->length;
    }
}