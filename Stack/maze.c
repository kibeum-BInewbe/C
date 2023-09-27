    #define CRT_SECURE_NO_WARNINGS

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdbool.h>
    #include <windows.h>    // gotoxy() 함수를 사용하기 위한 헤더

    /* 12*12 maze 행렬 */
    #define m 12
    #define p 12
    #define Max_dir 8   // 총 가능한 이동방향의 수
    #define Timestep 1000   // 시간단위는 ms 이므로 1000이면 1초를 멈추게 됨.
    #define MAX_STACK_SIZE 100
    #define Max_dir 8   // 현 위치에서 이동 가능한 방향 수

    /* 수직 수평 이동 구조체 */
    typedef struct Aoff {
        short int vert;     // 수직이동
        short int horiz;    // 수평이동
    } offsets;

    /* 스택에 저장할 데이터 */
    typedef struct St_element {
        short int row;  // 스택에 저장된 row위치.
        short int col;  // 스택에 저장된 column위치.
        short int dir;  // 해당 위치에서 움직인 방향.
    } ty_element;

    /* 연결스택의 노드 */
    typedef struct ListNode* listPointer;
    typedef struct ListNode {
        ty_element data;    // 노드에 저장된 데이터.
        listPointer link;   // 다음 노드의 메모리 주소.
    } listNode;

    typedef struct stack {
        listPointer top;
        int length;
    } Stack;

    offsets move[Max_dir] = { {-1,0}, {-1,1}, {0,1}, {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1} };

    int maze[m+2][p+2]; // maze 행렬
    int mark[m+2][p+2]; // mark 행렬

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

    /* gotoxy 함수 */
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

        // 텍스트 파일에서 미로 데이터를 읽어들인다.
        for (int y = 0; y < m + 2; y++)
            for (int x = 0; x < p + 2; x++)
                fscanf(f, " %c", &maze[y][x]);

        fclose(f);

        int sx, sy, dx, dy;

        // 무한 반복
        for (;;) {
            printf("출발점, 목표점은? (4개의 정수; 종료는 Control+D Enter): ");

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
        // 추가할 새 노드 생성.
        listNode* newNode = (listNode*)malloc(sizeof(listNode));

        // 새 노드에 값이 없는 경우.
        if (newNode == NULL) {
            printf("\n*** Failed to add a new node! ***\n");
            
            return;
        }

        // 새 노드에 data를 담고, link는 top의 값을 저장.
        newNode->data = data;
        newNode->link = s->top;
        // 새 노드가 stack의 첫번째 요소가 되므로 top은 새 노드를 가르킨다.
        s->top = newNode;
        s->length++;
    }

    /* delete stack */
    ty_element pop(Stack* s) {
        if (is_empty(s)) {
            // return 값을 0으로 하고 싶은데, 함수 자료형이 int가 아닌 경우, memest 함수 이용하여 자료형 변환.
            ty_element emptyElement;
            memset(&emptyElement, 0, sizeof(emptyElement));
            return emptyElement;
        } else {
            // 삭제될 노드를 가르키는 변수 node 지정.
            listNode* node = s->top;
            // 삭제될 노드의 데이터 값을 저장하는 임의 변수 생성.
            ty_element data = node->data;

            // 두번째 노드 위치를 top에게 할당하여 연결 리스트 유지.
            s->top = node->link;

            // node를 반납하고,
            free(node);
            s->length--;
            // data를 반환한다.
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
            // 한 줄에 최대 6개의 좌표만 출력한다.
            if ((i = (i + 1) % 6) == 0) printf("\n");
            */
        }

        printf("\n\n경로길이= %d, 백트랙킹수= %d\n", count);
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
    /* 미로 경로 탐색 함수. */
    /* [sy,sx] 에서 시작하여 [dy,dx] 에 도달하는 경로를 찾는다. */
    void path(int sy, int sx, int dy, int dx) {
        Stack s, path;

        // stack 초기화
        init(&s), init(&path);

        // 미로가 막혀 있으면 에러 메세지 출력 후 리턴한다.
        if (maze[sy][sx] == 1 || maze[dy][dx] == 1) {
            printf("\n*** 출발 지점 또는 도착 지점이 막혀 있습니다! ***\n");

            return;
        }
        /*
        // 미로를 그린다.
        for (int y = 0; y < m + 2; y++) {
            for (int x = 0; x < p + 2; x++)
                printf("%c", maze[y][x]);

            puts("");
        }
        */

        // 스택에 출발 지점을 넣는다.
        push(&s, (ty_element) { sy, sx, 0 });
        push(&path, (ty_element) { sy, sx, 0 });

        // 방문 기록을 초기화한다.
        for (int y = 0; y < m + 2; y++)
            for (int x = 0; x < p + 2; x++)
                mark[y][x] = 0;

        // 출발 지점을 방문 표시한다.
        mark[sy][sx] = 1;

        // 경로를 찾았는가?
        bool found = false;

        // 더 이상 방문할 지점이 없을 때까지 반복한다.
        while (!is_empty(&s)) {
            // 스택에서 현재 지점을 꺼낸다.
            ty_element e = pop(&s);

            // 이 지점을 방문 표시한다.
            mark[e.row][e.col] = 1;

            if (e.row == dy && e.col == dx) {
                found = true;

                break;
            }

            // 이 지점과 인접한 8개 방향의 지점을 탐색한다.
            for (int dir = 0; dir < Max_dir; dir++) {
                int mx = e.col + move[dir].horiz;
                int my = e.row + move[dir].vert;

                // 선택한 지점이 벽이 아니고, 아직 방문하지 않았다면?
                if (maze[my][mx] == '0' && !mark[my][mx]) {
                    push(&s, (ty_element) { my, mx, dir });
                    push(&path, (ty_element) { my, mx, dir });
                }
            }
        }

        print(&path);
    }

    /* 매개변수 x에 원하는 셀의 col, y에 그 셀의 row 정보를 주면서 호출한다. */
    /* 미로 좌표 상에서 커서를 [y,x] 셀 위치에 놓는다. */
    void gotoxy(int x, int y) {
        COORD Pos = {y, x};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
    }
    // 위 함수를 이용하여 원하는 셀 위치에 커서를 놓고, 다음 printf 문으로 거기에 원하는 문자 1개를 덮어 쓸 수 있다.