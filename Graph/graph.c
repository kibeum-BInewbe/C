#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max_vertex 100  // �ִ� ���� ���� ����. �̴� �۾��� �׷����� ������ ������ ���� ū ����.
#define INFINITE_VAL 10000  // ���Ѵ� ��.(�׷����� ��� ��ũ���� ����� ���պ��� �ξ� ū ��.)
#define NUM_IMPOSSIBLE -99999   // ���� �������� ���� �� ���� ��.

// -------------------Ÿ�� �� �������� ����-----------------------
typedef struct stnode* type_node_ptr;   // ��������Ʈ�� ��� ����
typedef struct stnode {
    int no; // ������ȣ
    double length;  // ���� ����ġ
    struct stnode* next; // ���� ��忡 ���� ������
} type_node;

int Num_vertices;   // �۾��� �׷����� �� ���� ����.
double Cost_mat[Max_vertex][Max_vertex]; // �������. ��� ���� ���� INFINEITE_VAL �� �ʱ�ȭ.
type_node_ptr Adj_list[Max_vertex]; // ���� ����Ʈ. ó������ NULL�� �ʱ�ȭ.
double dist[Max_vertex]; // Digkstra �˰����� ����ϴ� �Ÿ�����.
int visited[Max_vertex]; // Ž�� �Լ����� ����ϴ� �湮���� ����.(1: �湮��; 0: �湮 ����)
int pred[Max_vertex]; // Ž�� �Լ����� ����� ������ ����.(-1�̸� ������ ����)
int S[Max_vertex]; // Dijkstra �˰����� ����ϴ� ó���Ϸ����� ����(1: �������; 0: �ȵ������)

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
    int ib; //�� ������ buffer ���� ���� ���� ������ ��ġ�� ������.
    char buffer[100], command[30], sub[10], filename[100]; // ��ɹ� ����, ��ɾ�, �����ɾ�.
    FILE* fps; // �׷��� ������ ������ ������.
    printf("\n�׷��� ������ ���� �̸���? (���� �̸��� 'graph_data.txt'�̴�.)");
    gets(filename);
    fps = fopen(filename, "r");
    if (!fps) {
        printf("���� ���� ����.\n");
        return;
    }
    // �ʱ�ȭ: �������, ��������Ʈ.
    for (i = 0; i < Max_vertex; i++) {
        Adj_list[i] = NULL;
        
        for (j = 0; j < Max_vertex; j++) {
            Cost_mat[i][j] = INFINITE_VAL; // �� ���� ��ũ�� ������ ��Ÿ��.
        }
    }
    read_and_construct_graph(filename); // �׷��� ������ ������ �о� �׷���ǥ���� �����.
    //��ɹ� ���� ����.
    while (1) {
        // �� ����� ���� ���� ���� visited, pred, S �� �ʱ�ȭ�Ѵ�.
        for (i = 0; i < Max_vertex; i++) {
            visited[i] = 0; pred[i] = -1; S[i] = 0;
        }
        printf("\n��ɹ�����: dfs a 0 / dfs s 0 7 / bfs a 0 / bfs s 0 7 / dij a 0 / dij s 0 7 / exit.\n");
        printf("��ɹ��� �����ÿ�> ");
        gets(buffer); // ��ɹ� �� ��(line)�� �о� �´�. ���ڿ� buf �� �о� ��.
        ib = 0; // ib�� ���� ó���� ���ڸ� ����Ų��. ���⼭�� ù ���ڸ� ����Ű�� �ʱ�ȭ.
        read_word(buffer, &ib, command); // ���� ��ɴܾ �д´�.

        if (strlen(command) == 0) {
            printf("��ɿ���. \n");
            continue; // ��ɴܾ ������ �ٽ� ��� ��û.
        }
        if (strcmp(command, "dij") == 0) { // Dijkstra algorithm �� ���� ��� ������ ��� ã��
            read_word(buffer, &ib, sub); // sub ��� �ܾ �д´�.
            if (strlen(command) == 0)
                continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
            if (sub[0] == 'a') {
                start_vertex = read_number(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                dijkstra(start_vertex); // Dijkstra �˰��� ����.
                print_paths_dijkstra(start_vertex, -1); // ��� ���������� ��� ���.
            }
            else if (sub[0] == 's') {
                start_vertex = read_number(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                destination_vertex = read_number(buffer, &ib); // �������� �б�.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                dijkstra(start_vertex); // Dijkstra �˰��� ����.
                print_paths_dijkstra(start_vertex, destination_vertex); // �� ��� ���.
            }
            else {
                printf("��� ����.\n");
                continue;
            }
        } // dij
        else if (strcmp(command, "dfs") == 0) { // dfs�� ���� ��� ������ ��� ã��
            read_word(buffer, &ib, sub); // sub ��� �ܾ �д´�.
            if (strlen(command) == 0)
                continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
            if (sub[0] == 'a') {
                start_vertex = read_number(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                pred[start_vertex] = -1; // ���������� ���� ������ ���� ������ ��.
                depth_first(start_vertex); // depth-first searach �˰��� ����.
                print_paths_dfs_bfs(start_vertex, -1); // ��� ���������� ��� ���.
            }
            else if (sub[0] == 's') {
                start_vertex = read_number(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                destination_vertex = read_number(buffer, &ib); // �������� �б�.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                pred[start_vertex] = -1; // ���������� ���� ������ ����.
                depth_first(start_vertex); // depth-first searach �˰��� ����.
                print_paths_dfs_bfs(start_vertex, destination_vertex); // �� ���� ��� ���.
            }
            else {
                printf("sub ��� ����.\n");
                continue;
            }
        } // dfs
        else if (strcmp(command, "bfs") == 0) { // bfs�� ���� ��� ������ ��� ã��
            read_word(buffer, &ib, sub); // sub ��� �ܾ �д´�.
            if (strlen(command) == 0)
                continue; // ���� ��ɴܾ ������ �ٽ� ��� ��û.
            if (sub[0] == 'a') {
                start_vertex = read_number(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                breadth_first(start_vertex); // breadth-first search �˰��� ����.
                print_paths_dfs_bfs(start_vertex, -1); // ��� ���������� ��� ���.
            }
            else if (sub[0] == 's') {
                start_vertex = read_number(buffer, &ib); // �������� �б�.
                if (start_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                destination_vertex = read_number(buffer, &ib); // �������� �б�.
                if (destination_vertex == NUM_IMPOSSIBLE) {
                    printf("Wrong command.\n"); continue;
                } // ���� �ȳ����� �ٽ� ��� ��û.
                breadth_first(start_vertex); // breadth-first search �˰��� ����.
                print_paths_dfs_bfs(start_vertex, destination_vertex); // �� ���� ��� ���.
            }
        else {
            printf("sub ��� ����.\n");
            continue;
        }
        } // bfs
        else if (strcmp(command, "exit") == 0) {
            break; // ���� ����
        }
        else {
            printf("��ɿ���. \n"); continue; // �߸��� ����� �����ϰ� �ٽ� ��û.
        }
    } // while(��ɹ� ���� ����)
    printf("\n���α׷��� �����մϴ�.\n");
} // main


// ----------------------functions------------------------
// S�� ���� �� �� dist�� �ּ��� ������ ã�´�. �� ������ȣ�� ��ȯ.
// S�� �� ��� �ִ� ������ �ϳ��� ������ -1�� ��ȯ.
int choose(int number_of_vertex) { // �Է� number_of_vertex�� ������ �� ������.
    int i, j, min, minpos;

    // S�� ���� ù ������ ã��.
    for (i = 0; i < number_of_vertex; i++) {
        if (S[i] == 0) break;
    }
    if (i == number_of_vertex) return -1; // �������� ��� S�� ����ִ�.
        
    minpos = i; // S�� ���� ù ������ȣ.
    min = dist[i]; // �� ������ dist.

    for (j = i+1; j < number_of_vertex; j++) {
        if (S[j] == 0 && dist[j] < min) { // S�� ���� �����̸� distance�� min���� �� ������ ����
            min = dist[j];
            minpos = j;
        }
    }
    return minpos;
} // choose

// �Ʒ� ť ���� �Լ����� ť�� ����ť�� �����Ͽ� �ۼ���.
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

// ���� v�� queue �ȿ� �̹� �����ϴ��� üũ�Ѵ�.
int exist_in_queue(int queue[], int front, int rear, int v) {
    int i, answer = 0;
    for (i = front + 1; i <= rear; i++) {
        if (queue[i] == v) answer = 1;
    }
    return answer; // �����ϸ� 1, �� �ϸ� 0�� ��ȯ.
} // exist_in_queue

void breadth_first(int start) {
    int queue[Max_vertex];
    int front = -1, rear = -1;
    int v, w;

    // ���� ������ ť�� �߰��ϰ� �湮 ǥ��
    addq(queue, &rear, start);
    visited[start] = 1;

    printf("\nBFS �湮 ����: ");
    while (!empty_queue(front, rear)) {
        // ť���� ������ ������ ���
        v = deleteq(queue, &front, rear);
        printf("%d ", v);

        // ���� v�� ������ ������ �߿��� ���� �湮���� ���� ������ ť�� �߰��ϰ� �湮 ǥ��
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
        printf("���� ���� ����\n");
        return;
    }

    int i, j;
    char buf[100];
    // ���Ͽ��� ���� �� �о�ͼ� Num_vertices�� �����ϸ�, atoi�Լ��� ���ڿ��� ������ ��ȯ
    fgets(buf, sizeof(buf), fp);
    Num_vertices = atoi(buf);

    // �� ������ ���� ���� ����Ʈ ����
    for (i = 0; i < Num_vertices; i++) {
        // list�� �ʱ�ȭ
        Adj_list[i] = NULL;
        // ���Ͽ��� �������� �о����, �����ٿ��� ���� i�� ����� ������� ������ ����ġ ������ �ִ�.
        fgets(buf, sizeof(buf), fp);
        int pos = 0;
        // �� ������ ���� ���� ���� ó��
        for (j = 0; j < Num_vertices; j++) {
            // ���ۿ��� ���� ���ڸ� �о�´�.
            int cost = read_number(buf, &pos);
            // NUM_IMPOSSIBLE���� �ٸ� ���(��ȿ�� ��), Cost �迭�� ����ġ ����
            if (cost != NUM_IMPOSSIBLE) {
                Cost_mat[i][j] = cost;
                // ���ο� ��忡 ������ȣ(j), ��������ġ(cost), Adj ����Ʈ�� ����Ű�� ������ ����
                type_node_ptr newNode = (type_node_ptr)malloc(sizeof(type_node));
                newNode->no = j;
                newNode->length = cost;
                newNode->next = Adj_list[i];
                // ���ο� ��带 Adj ����Ʈ�� ù��° ���� ����, ���� i�� ���� ��������Ʈ ����
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
    // ��� ������ �Ÿ� dist, �湮���� visited, ������ ���� pred�� �ʱ�ȭ�Ѵ�.
    // ��� ���� v0�� �Ÿ��� 0���� �����ϰ� �湮 ���θ� ǥ���Ѵ�.
    for (i = 0; i < Num_vertices; i++) {
        dist[i] = Cost_mat[v0][i];
        visited[i] = 0;
        pred[i] = v0;
    }
    visited[v0] = 1;
    dist[v0] = 0;

    // �־��� ������ ��ȸ�ϸ� �ִ� �Ÿ� �����Ѵ�.
    // �湮���� ���� ���� �߿� ���� �Ÿ��� ª�� ���� w�� �����Ѵ�.
    for (i = 0; i < Num_vertices; i++) {
        int min_dist = INFINITE_VAL;
        for (v = 0; v < Num_vertices; v++) {
            if (visited[v] == 0 && dist[v] < min_dist) {
                min_dist = dist[v];
                w = v;
            }
        }
        // w�� �湮�� ������ �ϰ�, w�� ����� �������� �Ÿ��� �����Ѵ�.
        visited[w] = 1;

        for (v = 0; v < Num_vertices; v++) {
            // ���ο� �Ÿ��� �� ª���� �� �Ÿ��� ������ ������ �����Ѵ�.
            if (visited[v] == 0 && dist[w] + Cost_mat[w][v] < dist[v]) {
                dist[v] = dist[w] + Cost_mat[w][v];
                pred[v] = w;
            }
        }
    }   // ��� ������ ��ȸ�ߴٸ�, dist �迭���� ������� v0���� �� ���������� �ִ� �Ÿ��� ����Ǿ� �ִ�.
}

void print_paths_dijkstra(int start, int desti) {
    if (start < 0 || start >= Num_vertices || desti < 0 || desti >= Num_vertices) {
        printf("�߸��� ���� �Ǵ� ������ �ε����Դϴ�.\n");
        return;
    }

    if (dist[desti] == INFINITE_VAL) {
        printf("���� �������� �������� ������ �� �����ϴ�.\n");
        return;
    }

    int path[Max_vertex]; // ��θ� �����ϴ� �迭
    int path_length = 0; // ����� ����

    int current = desti;
    while (current != start) {
        path[path_length++] = current;
        current = pred[current];
    }
    path[path_length++] = start;

    printf("�ִ� ���: ");
    for (int i = path_length - 1; i >= 0; i--) {
        printf("%d ", path[i]);
    }
    printf("\n");
}

void print_paths_dfs_bfs(int start, int desti) {
    if (start < 0 || start >= Num_vertices || desti < 0 || desti >= Num_vertices) {
        printf("�߸��� ���� �Ǵ� ������ �ε����Դϴ�.\n");
        return;
    }

    int visited[Max_vertex] = {0}; // �湮 ���θ� �����ϴ� �迭
    int path[Max_vertex]; // ��θ� �����ϴ� �迭
    int path_length = 0; // ����� ����

    depth_first(start);
    if (visited[desti]) {
        printf("���� �켱 Ž������ ã�� �ִ� ���: ");
        for (int i = path_length - 1; i >= 0; i--) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        printf("���� �������� �������� ������ �� �����ϴ�.\n");
    }

    for (int i = 0; i < Max_vertex; i++) {
        visited[i] = 0; // �湮 ���� �迭 �ʱ�ȭ
    }

    breadth_first(start);
    if (visited[desti]) {
        printf("�ʺ� �켱 Ž������ ã�� �ִ� ���: ");
        for (int i = path_length - 1; i >= 0; i--) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        printf("���� �������� �������� ������ �� �����ϴ�.\n");
    }
}