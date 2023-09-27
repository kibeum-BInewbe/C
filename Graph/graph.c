#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max_vertex 100  // 최대 가능 정점 개수. 이는 작업할 그래프의 정점의 수보다 많이 큰 수임.
#define INFINITE_VAL 10000  // 무한대 수.(그래프의 모든 아크들의 비용의 총합보다 훨씬 큰 수.)
#define NUM_IMPOSSIBLE -99999   // 절대 등장하지 않을 것 같은 수.

// -------------------타입 및 전역변수 선언-----------------------
typedef struct stnode* type_node_ptr;   // 인접리스트의 노드 선언
typedef struct stnode {
    int no; // 정점번호
    double length;  // 간선 가중치
    struct stnode* next; // 다음 노드에 대한 포인터
} type_node;

int Num_vertices;   // 작업할 그래프의 총 정점 개수.
double Cost_mat[Max_vertex][Max_vertex]; // 인접행렬. 모든 셀의 값을 INFINEITE_VAL 로 초기화.
type_node_ptr Adj_list[Max_vertex]; // 인접 리스트. 처음에는 NULL로 초기화.
double dist[Max_vertex]; // Digkstra 알고리즘이 사용하는 거리정보.
int visited[Max_vertex]; // 탐색 함수들이 사용하는 방문여부 정보.(1: 방문함; 0: 방문 안함)
int pred[Max_vertex]; // 탐색 함수들이 사용할 선행자 정보.(-1이면 선행자 없음)
int S[Max_vertex]; // Dijkstra 알고리즘이 사용하는 처리완료정점 집합(1: 들어있음; 0: 안들어있음)

// -------------------Prototypes of functions-------------------
void addq(int queue[], int* rear, int item);
void breadth_first(int start);
int choose(int nmber_of_vertex);
int deleteq(int queue[], int* front, int rear);
void depth_first(int current);
void dijkstra(int v0);
int empty_queue(int front, int rear);
int exist_in_queue(int queue[], int front, int rear, int v);
void print_paths_dfs_bfs(int start, int desti);
void print_paths_dijkstra(int start, int desti);
void read_and_construct_graph(char* fileName);
int read_number(char buf[], int* pos);
void read_word(char buf[], int* pos, char name[]);


// -----------------------main-----------------------------
void main() {
    int i, j, start_vertex, destination_vertex;
    int ib; //이 변수는 buffer 에서 다음 읽을 글자의 위치를 가진다.
    char buffer[100], command[30], sub[10], filename[100]; // 명령문 버퍼, 명령어, 서브명령어.
    FILE* fps; // 그래프 데이터 파일의 포인터.
    printf("\n그래프 데이터 파일 이름은? (파일 이름은 'graph_data.txt'이다.)");
    gets(filename);
    fps = fopen(filename, "r");
    if (!fps) {
        printf("파일 열기 실패.\n");
        return;
    }
    // 초기화: 인접행렬, 인접리스트.
    for (i = 0; i < Max_vertex; i++) {
        Adj_list[i] = NULL;
        
        for (j = 0; j < Max_vertex; j++) {
            Cost_mat[i][j] = INFINITE_VAL; // 이 값은 아크가 없음을 나타냄.
        }
    }
    read_and_construct_graph(filename); // 그래프 데이터 파일을 읽어 그래프표현을 만든다.
    //명령문 수행 루프.
    while (1) {
        // 한 명령의 수행 전에 먼저 visited, pred, S 를 초기화한다.
        for (i = 0; i < Max_vertex; i++) {
            visited[i] = 0; pred[i] = -1; S[i] = 0;
        }
        printf("\n명령문종류: dfs a 0 / dfs s 0 7 / bfs a 0 / bfs s 0 7 / dij a 0 / dij s 0 7 / exit.\n");
        printf("명령문을 넣으시오> ");
        gets(buffer); // 명령문 한 줄(line)을 읽어 온다. 문자열 buf 로 읽어 옴.
        ib = 0; // ib는 다음 처리할 글자를 가리킨다. 여기서는 첫 글자를 가리키게 초기화.
        read_word(buffer, &ib, command); // 먼저 명령단어를 읽는다.

        if (strlen(command) == 0) {
            printf("명령오류. \n");
            continue; // 명령단어가 없으면 다시 명령 요청.
        }
        if (strcmp(command, "dij") == 0) { // Dijkstra algorithm 에 의해 모든 노드로의 경로 찾기
            read_word(buffer, &ib, sub); // sub 명령 단어를 읽는다.
            if (strlen(command) == 0)
                continue; // 서브 명령단어가 없으면 다시 명령 요청.
            if (sub[0] == 'a') {
                start_vertex = read_number(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                dijkstra(start_vertex); // Dijkstra 알고리즘 수행.
                print_paths_dijkstra(start_vertex, -1); // 모든 정점으로의 경로 출력.
            }
            else if (sub[0] == 's') {
                start_vertex = read_number(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                destination_vertex = read_number(buffer, &ib); // 종착정점 읽기.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                dijkstra(start_vertex); // Dijkstra 알고리즘 수행.
                print_paths_dijkstra(start_vertex, destination_vertex); // 한 경로 출력.
            }
            else {
                printf("명령 오류.\n");
                continue;
            }
        } // dij
        else if (strcmp(command, "dfs") == 0) { // dfs에 의해 모든 노드로의 경로 찾기
            read_word(buffer, &ib, sub); // sub 명령 단어를 읽는다.
            if (strlen(command) == 0)
                continue; // 서브 명령단어가 없으면 다시 명령 요청.
            if (sub[0] == 'a') {
                start_vertex = read_number(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                pred[start_vertex] = -1; // 시작정점의 선행 정점은 없는 것으로 함.
                depth_first(start_vertex); // depth-first searach 알고리즘 수행.
                print_paths_dfs_bfs(start_vertex, -1); // 모든 정점으로의 경로 출력.
            }
            else if (sub[0] == 's') {
                start_vertex = read_number(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                destination_vertex = read_number(buffer, &ib); // 종착정점 읽기.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                pred[start_vertex] = -1; // 시작정점의 선행 정점은 없다.
                depth_first(start_vertex); // depth-first searach 알고리즘 수행.
                print_paths_dfs_bfs(start_vertex, destination_vertex); // 한 정점 경로 출력.
            }
            else {
                printf("sub 명령 오류.\n");
                continue;
            }
        } // dfs
        else if (strcmp(command, "bfs") == 0) { // bfs에 의해 모든 노드로의 경로 찾기
            read_word(buffer, &ib, sub); // sub 명령 단어를 읽는다.
            if (strlen(command) == 0)
                continue; // 서브 명령단어가 없으면 다시 명령 요청.
            if (sub[0] == 'a') {
                start_vertex = read_number(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                breadth_first(start_vertex); // breadth-first search 알고리즘 수행.
                print_paths_dfs_bfs(start_vertex, -1); // 모든 정점으로의 경로 출력.
            }
            else if (sub[0] == 's') {
                start_vertex = read_number(buffer, &ib); // 시작정점 읽기.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                destination_vertex = read_number(buffer, &ib); // 종착정점 읽기.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // 수가 안나오면 다시 명령 요청.
                breadth_first(start_vertex); // breadth-first search 알고리즘 수행.
                print_paths_dfs_bfs(start_vertex, destination_vertex); // 한 정점 경로 출력.
            }
        else {
            printf("sub 명령 오류.\n");
            continue;
        }
        } // bfs
        else if (strcmp(command, "exit") == 0) {
            break; // 루프 종료
        }
        else {
            printf("명령오류. \n"); continue; // 잘못된 명령은 무시하고 다시 요청.
        }
    } // while(명령문 수행 루프)
    printf("\n프로그램을 종료합니다.\n");
} // main


// ----------------------functions------------------------
// S에 없는 것 중 dist가 최소인 정점을 찾는다. 이 정점번호를 반환.
// S에 안 들어 있는 정점이 하나도 없으면 -1을 반환.
int choose(int number_of_vertex) { // 입력 number_of_vertex는 정점의 총 개수임.
    int i, j, min, minpos;

    // S에 없는 첫 정점을 찾자.
    for (i = 0; i < number_of_vertex; i++) {
        if (S[i] == 0) break;
    }
    if (i == number_of_vertex) return -1; // 정점들이 모두 S에 들어있다.
        
    minpos = i; // S에 없는 첫 정점번호.
    min = dist[i]; // 그 정점의 dist.

    for (j = i+1; j < number_of_vertex; j++) {
        if (S[j] == 0 && dist[j] < min) { // S에 없는 정점이며 distance가 min보다 더 작으면 변경
            min = dist[j];
            minpos = j;
        }
    }
    return minpos;
} // choose

// 아래 큐 관련 함수들은 큐가 선형큐라 가정하여 작성됨.
void addq(int queue[], int* rear, int item) {
    if (*rear == Max_vertex - 1) {
        printf("adding to queue has failed since queue is full.\n");
        return;
    }
    else queue[++*rear] = item;
} // addq

int deleteq(int queue[], int* front, int rear) {
    if (*front == rear) {
        printf("delete failed since queue is empty.\n");
        return -999;
    }
    else return queue[++*front];
} // deleteq

int empty_queue(int front, int rear) {
    if (front == rear) return 1;
    else return 0;
} // empty_queue

// 정점 v가 queue 안에 이미 존재하는지 체크한다.
int exist_in_queue(int queue[], int front, int rear, int v) {
    int i, answer = 0;
    for (i = front + 1; i <= rear; i++) {
        if (queue[i] == v) answer = 1;
    }
    return answer; // 존재하면 1, 안 하면 0을 반환.
} // exist_in_queue

void breadth_first(int start) {
    int queue[Max_vertex];
    int front = -1, rear = -1;
    int v, w;

    // 시작 정점을 큐에 추가하고 방문 표시
    addq(queue, &rear, start);
    visited[start] = 1;

    printf("\nBFS 방문 순서: ");
    while (!empty_queue(front, rear)) {
        // 큐에서 정점을 꺼내서 출력
        v = deleteq(queue, &front, rear);
        printf("%d ", v);

        // 정점 v와 인접한 정점들 중에서 아직 방문하지 않은 정점을 큐에 추가하고 방문 표시
        for (w = 0; w < Num_vertices; w++) {
            if (Cost_mat[v][w] != INFINITE_VAL && !visited[w]) {
                addq(queue, &rear, w);
                visited[w] = 1;
            }
        }
    }

    printf("\n");
}

void read_word(char buf[], int* pos, char name[]) {
    int i = 0;
    while (buf[*pos] != '\0' && buf[*pos] != ' ' && buf[*pos] != '\n') {
        name[i++] = buf[(*pos)++];
    }
    name[i] = '\0';
}

int read_number(char buf[], int* pos) {
    int num = 0;
    while (buf[*pos] != '\0' && buf[*pos] != ' ' && buf[*pos] != '\n') {
        if (buf[*pos] < '0' || buf[*pos] > '9') {
            return NUM_IMPOSSIBLE;
        }
        num = num * 10 + (buf[(*pos)++] - '0');
    }
    return num;
}

void read_and_construct_graph(char* fileName) {
    FILE* fp = fopen(fileName, "r");
    if (!fp) {
        printf("파일 열기 실패\n");
        return;
    }

    int i, j;
    char buf[100];
    // 파일에서 정점 수 읽어와서 Num_vertices에 저장하며, atoi함수로 문자열을 정수로 전환
    fgets(buf, sizeof(buf), fp);
    Num_vertices = atoi(buf);

    // 각 정점에 대한 인접 리스트 구성
    for (i = 0; i < Num_vertices; i++) {
        // list를 초기화
        Adj_list[i] = NULL;
        // 파일에서 다음줄을 읽어오고, 다음줄에는 정점 i에 연결된 정점들과 간선의 가중치 정보가 있다.
        fgets(buf, sizeof(buf), fp);
        int pos = 0;
        // 각 정점에 대한 간선 정보 처리
        for (j = 0; j < Num_vertices; j++) {
            // 버퍼에서 다음 숫자를 읽어온다.
            int cost = read_number(buf, &pos);
            // NUM_IMPOSSIBLE값과 다른 경우(유효한 값), Cost 배열에 가중치 저장
            if (cost != NUM_IMPOSSIBLE) {
                Cost_mat[i][j] = cost;
                // 새로운 노드에 정점번호(j), 간선가중치(cost), Adj 리스트를 가르키는 포인터 저장
                type_node_ptr newNode = (type_node_ptr)malloc(sizeof(type_node));
                newNode->no = j;
                newNode->length = cost;
                newNode->next = Adj_list[i];
                // 새로운 노드를 Adj 리스트의 첫번째 노드로 지정, 정점 i에 대한 인접리스트 구성
                Adj_list[i] = newNode;
            }
        }
    }

    fclose(fp);
}

void depth_first(int current) {
    visited[current] = 1;
    type_node_ptr temp = Adj_list[current];
    while (temp != NULL) {
        int next = temp->no;
        if (visited[next] == 0) {
            pred[next] = current;
            depth_first(next);
        }
        temp = temp->next;
    }
}

void dijkstra(int v0) {
    int i, v, w;
    // 모든 정점의 거리 dist, 방문여부 visited, 선행자 정보 pred를 초기화한다.
    // 출발 정점 v0의 거리를 0으로 설정하고 방문 여부를 표시한다.
    for (i = 0; i < Num_vertices; i++) {
        dist[i] = Cost_mat[v0][i];
        visited[i] = 0;
        pred[i] = v0;
    }
    visited[v0] = 1;
    dist[v0] = 0;

    // 주어진 정점을 순회하며 최단 거리 갱신한다.
    // 방문하지 않은 정점 중에 가장 거리가 짧은 정점 w를 선택한다.
    for (i = 0; i < Num_vertices; i++) {
        int min_dist = INFINITE_VAL;
        for (v = 0; v < Num_vertices; v++) {
            if (visited[v] == 0 && dist[v] < min_dist) {
                min_dist = dist[v];
                w = v;
            }
        }
        // w를 방문한 것으로 하고, w와 연결된 정점들의 거리를 갱신한다.
        visited[w] = 1;

        for (v = 0; v < Num_vertices; v++) {
            // 새로운 거리가 더 짧으면 먼 거리와 선행자 정보를 갱신한다.
            if (visited[v] == 0 && dist[w] + Cost_mat[w][v] < dist[v]) {
                dist[v] = dist[w] + Cost_mat[w][v];
                pred[v] = w;
            }
        }
    }   // 모든 정점을 순회했다면, dist 배열에는 출발정점 v0에서 각 정점까지의 최단 거리가 저장되어 있다.
}

void print_paths_dijkstra(int start, int desti) {
    if (start < 0 || start >= Num_vertices || desti < 0 || desti >= Num_vertices) {
        printf("잘못된 시작 또는 목적지 인덱스입니다.\n");
        return;
    }

    if (dist[desti] == INFINITE_VAL) {
        printf("시작 정점에서 목적지로 도달할 수 없습니다.\n");
        return;
    }

    int path[Max_vertex]; // 경로를 저장하는 배열
    int path_length = 0; // 경로의 길이

    int current = desti;
    while (current != start) {
        path[path_length++] = current;
        current = pred[current];
    }
    path[path_length++] = start;

    printf("최단 경로: ");
    for (int i = path_length - 1; i >= 0; i--) {
        printf("%d ", path[i]);
    }
    printf("\n");
}

void print_paths_dfs_bfs(int start, int desti) {
    if (start < 0 || start >= Num_vertices || desti < 0 || desti >= Num_vertices) {
        printf("잘못된 시작 또는 목적지 인덱스입니다.\n");
        return;
    }

    int visited[Max_vertex] = {0}; // 방문 여부를 저장하는 배열
    int path[Max_vertex]; // 경로를 저장하는 배열
    int path_length = 0; // 경로의 길이

    depth_first(start);
    if (visited[desti]) {
        printf("깊이 우선 탐색으로 찾은 최단 경로: ");
        for (int i = path_length - 1; i >= 0; i--) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        printf("시작 정점에서 목적지로 도달할 수 없습니다.\n");
    }

    for (int i = 0; i < Max_vertex; i++) {
        visited[i] = 0; // 방문 여부 배열 초기화
    }

    breadth_first(start);
    if (visited[desti]) {
        printf("너비 우선 탐색으로 찾은 최단 경로: ");
        for (int i = path_length - 1; i >= 0; i--) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        printf("시작 정점에서 목적지로 도달할 수 없습니다.\n");
    }
}