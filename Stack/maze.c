    #define CRT_SECURE_NO_WARNINGS

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <windows.h>    // gotoxy() �Լ��� ����ϱ� ���� ���

    /* 12*12 maze ��� */
    #define m 12
    #define p 12
    #define Max_dir 8   // �� ������ �̵������� ��
    #define Timestep 1000   // �ð������� ms �̹Ƿ� 1000�̸� 1�ʸ� ���߰� ��.
    #define MAX_STACK_SIZE 100
    #define Max_dir 8   // �� ��ġ���� �̵� ������ ���� ��

    /* ���� ���� �̵� ����ü */
    typedef struct Aoff {
        short int vert;     // �����̵�
        short int horiz;    // �����̵�
    } offsets;

    /* ���ÿ� ������ ������ */
    typedef struct St_element {
        short int row;  // ���ÿ� ����� row��ġ.
        short int col;  // ���ÿ� ����� column��ġ.
        short int dir;  // �ش� ��ġ���� ������ ����.
    } ty_element;

    /* ���ὺ���� ��� */
    typedef struct ListNode* listPointer;
    typedef struct ListNode {
        ty_element data;    // ��忡 ����� ������.
        listPointer link;   // ���� ����� �޸� �ּ�.
    } listNode;

    typedef struct stack {
        listPointer top;
        int length;
    } Stack;

    offsets move[Max_dir] = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };

    int maze[m+2][p+2]; // maze ���
    int mark[m+2][p+2]; // mark ���

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // code for stack
    /* initialize stack */
    void init(Stack* s);

    /* if stack is empty */
    int is_empty(Stack* s);

    /* add stack */
    void push(Stack* s, ty_element data);

    /* delete stack */
    ty_element pop(Stack* s);

    /* get latest data in stack */
    ty_element peek(Stack* s);

    /* stack length */
    int length(Stack* s);

    /* print stack */
    void print(Stack* s);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // code for maze
    /* search path */
    void path(int sy, int sx, int dy, int dx);

    /* gotoxy �Լ� */
    void gotoxy(int x, int y);

    /* file name */
    const char* file_name = "./maze_14_2.txt";

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int main(void) {
        FILE *f = fopen(file_name, "r");

        if (f == NULL) {
            printf("*** Failed to open file: %s ***\n", file_name);

            system("pause");

            return 1;
        }

        // �ؽ�Ʈ ���Ͽ��� �̷� �����͸� �о���δ�.
        for (int y = 0; y < m + 2; y++)
            for (int x = 0; x < p + 2; x++)
                fscanf(f, " %c", &maze[y][x]);

        fclose(f);

        int sx, sy, dx, dy;

        // ���� �ݺ�
        for (;;) {
            printf("�����, ��ǥ����? (4���� ����; ����� Control+D Enter): ");

            int res = scanf("%d %d %d %d", &sy, &sx, &dy, &dx);

            if (res != 4) break;

            puts("");

            path(sy, sx, dy, dx);
        }

        system("pause");

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // code for stack
    /* initialize stack */
    void init(Stack* s) {
        s->top = NULL;
    }

    /* if stack is empty */
    int is_empty(Stack* s) {
        return s->top == NULL;
    }

    /* add stack */
    void push(Stack* s, ty_element data) {
        // �߰��� �� ��� ����.
        listNode* newNode = (listNode*)malloc(sizeof(listNode));

        // �� ��忡 ���� ���� ���.
        if (newNode == NULL) {
            printf("\n*** Failed to add a new node! ***\n");
            
            return;
        }

        // �� ��忡 data�� ���, link�� top�� ���� ����.
        newNode->data = data;
        newNode->link = s->top;
        // �� ��尡 stack�� ù��° ��Ұ� �ǹǷ� top�� �� ��带 ����Ų��.
        s->top = newNode;
        s->length++;
    }

    /* delete stack */
    ty_element pop(Stack* s) {
        if (is_empty(s)) {
            // return ���� 0���� �ϰ� ������, �Լ� �ڷ����� int�� �ƴ� ���, memest �Լ� �̿��Ͽ� �ڷ��� ��ȯ.
            ty_element emptyElement;
            memset(&emptyElement, 0, sizeof(emptyElement));
            return emptyElement;
        } else {
            // ������ ��带 ����Ű�� ���� node ����.
            listNode* node = s->top;
            // ������ ����� ������ ���� �����ϴ� ���� ���� ����.
            ty_element data = node->data;

            // �ι�° ��� ��ġ�� top���� �Ҵ��Ͽ� ���� ����Ʈ ����.
            s->top = node->link;

            // node�� �ݳ��ϰ�,
            free(node);
            s->length--;
            // data�� ��ȯ�Ѵ�.
            return data;
        }
    }

    /* get latest data in stack */
    ty_element peek(Stack* s) {
        if (is_empty(s)) {
            ty_element emptyElement;
            memset(&emptyElement, 0, sizeof(emptyElement));
            return emptyElement;
        } else {
            return s->top->data;
        }
    }

    /* print stack */
    void print(Stack* s) {
        listNode *node = NULL, *top = s->top;
        int count = 0;

        while (top != NULL) {
            listNode *current = top;

            top = top->link;
            current->link = node;

            node = current;
        }

        int i = 0;
        while (node != NULL) {
            
            printf("[%2d, %2d] ", node->data.row, node->data.col);
            count++;
            maze[node->data.row][node->data.col] = '*';

            node = node->link;
            /*
            // �� �ٿ� �ִ� 6���� ��ǥ�� ����Ѵ�.
            if ((i = (i + 1) % 6) == 0) printf("\n");
            */
        }

        printf("\n\n��α���= %d, ��Ʈ��ŷ��= %d\n", count);
        printf("\n");

        for (int y = 0; y < m + 2; y++) {
            for (int x = 0; x < p + 2; x++)
                printf("%c", maze[y][x]);

            puts("");
        }
        printf("\n");
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // code for maze
    /* �̷� ��� Ž�� �Լ�. */
    /* [sy,sx] ���� �����Ͽ� [dy,dx] �� �����ϴ� ��θ� ã�´�. */
    void path(int sy, int sx, int dy, int dx) {
        Stack s, path;

        // stack �ʱ�ȭ
        init(&s), init(&path);

        // �̷ΰ� ���� ������ ���� �޼��� ��� �� �����Ѵ�.
        if (maze[sy][sx] == 1 || maze[dy][dx] == 1) {
            printf("\n*** ��� ���� �Ǵ� ���� ������ ���� �ֽ��ϴ�! ***\n");

            return;
        }
        /*
        // �̷θ� �׸���.
        for (int y = 0; y < m + 2; y++) {
            for (int x = 0; x < p + 2; x++)
                printf("%c", maze[y][x]);

            puts("");
        }
        */

        // ���ÿ� ��� ������ �ִ´�.
        push(&s, (ty_element) { sy, sx, 0 });
        push(&path, (ty_element) { sy, sx, 0 });

        // �湮 ����� �ʱ�ȭ�Ѵ�.
        for (int y = 0; y < m + 2; y++)
            for (int x = 0; x < p + 2; x++)
                mark[y][x] = 0;

        // ��� ������ �湮 ǥ���Ѵ�.
        mark[sy][sx] = 1;

        // ��θ� ã�Ҵ°�?
        bool found = false;

        // �� �̻� �湮�� ������ ���� ������ �ݺ��Ѵ�.
        while (!is_empty(&s)) {
            // ���ÿ��� ���� ������ ������.
            ty_element e = pop(&s);

            // �� ������ �湮 ǥ���Ѵ�.
            mark[e.row][e.col] = 1;

            if (e.row == dy && e.col == dx) {
                found = true;

                break;
            }

            // �� ������ ������ 8�� ������ ������ Ž���Ѵ�.
            for (int dir = 0; dir < Max_dir; dir++) {
                int mx = e.col + move[dir].horiz;
                int my = e.row + move[dir].vert;

                // ������ ������ ���� �ƴϰ�, ���� �湮���� �ʾҴٸ�?
                if (maze[my][mx] == '0' && !mark[my][mx]) {
                    push(&s, (ty_element) { my, mx, dir });
                    push(&path, (ty_element) { my, mx, dir });
                }
            }
        }

        print(&path);
    }

    /* �Ű����� x�� ���ϴ� ���� col, y�� �� ���� row ������ �ָ鼭 ȣ���Ѵ�. */
    /* �̷� ��ǥ �󿡼� Ŀ���� [y,x] �� ��ġ�� ���´�. */
    void gotoxy(int x, int y) {
        COORD Pos = {y, x};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
    }
    // �� �Լ��� �̿��Ͽ� ���ϴ� �� ��ġ�� Ŀ���� ����, ���� printf ������ �ű⿡ ���ϴ� ���� 1���� ���� �� �� �ִ�.