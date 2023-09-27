#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct treenode* TreePointer;

typedef struct element {
    int sno;        // 학번
    char name[50];  // 학생 이름
} ele;

typedef struct treenode {
    ele data;   // data 노드
    TreePointer leftChild, rightChild;  // 좌우 자식노드 가르키는 포인터
} treenode;

// 함수
TreePointer create_node(ele data);
void print_tree(TreePointer node);
int get_height(TreePointer node);
void insert(TreePointer* root, ele data);
TreePointer delete_node(TreePointer node, int key);
TreePointer find_min(TreePointer node);
void count_leaves(TreePointer node, int* count);
void print_child_nodes(TreePointer node, const char* name);
void print_node(TreePointer node, const char* name);

// 노드생성 함수
TreePointer create_node(ele data) {
    TreePointer newNode = (treenode*)malloc(sizeof(treenode));
    newNode->data = data;   // 노드에 데이터를 삽입
    newNode->leftChild = NULL;  // 좌우 자식노드 포인터 초기화
    newNode->rightChild = NULL;
    return newNode;
}

// 전체 tree 출력 함수
void print_tree(TreePointer node) {
    if (node == NULL) return;
    printf("%s, %d\n", node->data.name, node->data.sno);
    print_tree(node->leftChild);
    print_tree(node->rightChild);
}

// tree 높이 출력 함수
int get_height(TreePointer node) {
    // tree가 비어있으면 0 반환
    if (node == NULL) return 0;
    int left_height = get_height(node->leftChild);
    int right_height = get_height(node->rightChild);
    return 1 + (left_height > right_height ? left_height : right_height); // 더 높은 쪽의 자식 높이에 1을 더한 값이 현재 노드의 높이
}

// 입력한 데이터를 노드에 저장하여 tree에 삽입하는 함수
void insert(TreePointer* root, ele data) {
    // tree가 비어있으면 root에 노드 저장
    if (*root == NULL) {
        *root = create_node(data);
        return;
    }
    // 삽입노드 key가 root의 key보다 작으면
    if (data.sno < (*root)->data.sno)
        // 왼쪽 자식노드에 저장
        insert(&(*root)->leftChild, data);
    // 삽입노드 key가 root의 key보다 크면
    else if (data.sno > (*root)->data.sno)
        // 오른쪽 자식노드에 저장
        insert(&(*root)->rightChild, data);
}

// 입력한 학번을 가진 노드를 찾아 tree에서 삭제하는 함수
TreePointer delete_node(TreePointer node, int key) {
    if (node == NULL) return node;
    // key값이 현재노드의 key값보다 작으면
    if (key < node->data.sno)
        // 왼쪽으로 내려간다.
        node->leftChild = delete_node(node->leftChild, key);
    // key값이 현재노드의 key값보다 크면
    else if (key > node->data.sno)
        // 오른쪽으로 내려간다.
        node->rightChild = delete_node(node->rightChild, key);
    // key값과 현재노드의 key값이 같다! = 노드 발견!
    else {
        if (node->leftChild == NULL) {
            TreePointer temp = node->rightChild;
            free(node);
            return temp;
        }
        else if (node->rightChild == NULL) {
            TreePointer temp = node->leftChild;
            free(node);
            return temp;
        }
        // 삭제 후, 부모노드가 없는 자식노드가 생기면 제일 작은 key값을 가진 노드를 찾아서 빈 공간 채운다.
        TreePointer temp = find_min(node->rightChild);
        node->data = temp->data;
        node->rightChild = delete_node(node->rightChild, temp->data.sno);
    }
    return node;
}

// 노드가 삭제되었을 때 빈 공간 채우기 위한 함수
TreePointer find_min(TreePointer node) {
    TreePointer current = node;
    // 부모노드가 비어있을 때, 왼쪽 자식노드도 비어있다면
    while (current && current->leftChild != NULL)
        // 왼쪽 자식노드가 부모노드로
        current = current->leftChild;
    return current;
}

// leaf 노드 갯수 출력함수
void count_leaves(TreePointer node, int* count) {
    if (node == NULL) {
        return;
    }
    // 왼쪽과 오른쪽 두 자식 노드가 비어있으면 leaf 노드!
    if (node->leftChild == NULL && node->rightChild == NULL) {
        (*count)++;
    }
    // 재귀함수
    count_leaves(node->leftChild, count);
    count_leaves(node->rightChild, count);
}


// 입력한 학생 이름의 노드를 찾아서 좌우 노드에서 학생 이름 정보를 얻어 출력하는 함수
void print_child_nodes(TreePointer node, const char* name) {
    if (node == NULL) {
        printf("그런 학생 없습니다.\n");
        return;
    }
    // 해당 노드 발견 시, 좌우 노드 정보 출력
    if (strcmp(node->data.name, name) == 0) {
        printf("학생 이름: %s\n", node->data.name);
        printf("left child: ");
        if (node->leftChild != NULL) {
            printf("%s\t", node->leftChild->data.name);
        } else {
            printf("없음\n");
        }
        printf("right child: ");
        if (node->rightChild != NULL) {
            printf("%s\n", node->rightChild->data.name);
        } else {
            printf("없음\n");
        }
    }
    // 
    else if (strcmp(name, node->data.name) < 0) {
        print_child_nodes(node->leftChild, name);
    } else {
        print_child_nodes(node->rightChild, name);
    }
}
// 이름을 입력받고 노드 정보를 출력한다.
void print_node(TreePointer node, const char* name) {
    if (node == NULL) {
        printf("그런 학생 없습니다.");
        return;
    }
    if (strcmp(node->data.name, name) == 0) {
        printf("이름: %s, 학번: %d, 레벨 위치: %d\n", node->data.name, node->data.sno, get_height(node));
    }
    // key값이 작으면 왼쪽으로, 아니면 오른쪽으로 탐색
    else if (strcmp(name, node->data.name) < 0) {
        print_node(node->leftChild, name);
    }
    else {
        print_node(node->rightChild, name);
    }
}

TreePointer find_node(TreePointer node, const char* name) {
    TreePointer current = node;
    if (node == NULL) {
        printf("그런 학생 없습니다.");
    }
    if (strcmp(node->data.name, name) == 0) {
        return current;
    }
    else if (strcmp(name, node->data.name) < 0) {
        find_node(node->leftChild, name);
    }
    else {
        find_node(node->rightChild, name);
    }
}

int get_level_position(TreePointer root, TreePointer node) {
    if (root == NULL || node == NULL) {
        return -1;
    }
    int level = get_height(root) - get_height(node);
    // 레벨이 0보다 작을 수는 없다.
    return level >= 0 ? level : -1;
}


int main(void) {
    TreePointer root = NULL;
    const char* file_name = "./sdata.txt";

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("파일 열기 실패.\n");
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        ele data;
        sscanf(line, "%d %s", &data.sno, data.name);
        insert(&root, data);
    }

    fclose(file);

    char command[3];
    int sno;
    char name[50];

    while (1) {
        printf("수행할 작업은 (in, sp, de, se, ht, ch, le, ex) ?");
        scanf("%s", command);

        // tree 내용 출력
        if (strcmp(command, "sp") == 0) {
            printf("화일의 내용은 다음과 같습니다:\n");
            print_tree(root);
        }
        // tree 높이 출력
        else if (strcmp(command, "ht") == 0) {
            printf("트리 높이: %d\n", get_height(root));
        }
        // 데이터 삽입 후 결과 출력한다.
        else if (strcmp(command, "in") == 0) {
            printf("학생 번호를 입력하세요: ");
            scanf("%d", &sno);
            printf("학생 이름을 입력하세요: ");
            scanf("%s", name);
            ele data;
            data.sno = sno;
            strncpy(data.name, name, sizeof(data.name));

            insert(&root, data);

            // 삽입된 노드의 레벨 위치를 출력
            TreePointer insertedNode = find_node(root, data.name);

            if (insertedNode != NULL) {
                printf("입력 성공!\n");
                printf("level = %d\n", get_level_position(root, insertedNode));
            } else {
            printf("입력 실패!\n");
            }
        }
        // 학번을 입력받고 그 노드를 삭제한다.
        else if (strcmp(command, "de") == 0) {
            printf("삭제할 학생 번호를 입력하세요: ");
            scanf("%d", &name);
            root = delete_node(root, sno);
            printf("노드가 트리에서 삭제되었습니다.\n");
        }
        // 이름을 입력받고 좌우 자식 노드 정보 출력한다.
        else if (strcmp(command, "ch") == 0) {
            char studentName[50];
            printf("학생 이름을 입력하세요: ");
            scanf("%s", studentName);
            print_child_nodes(root, studentName);
        }

        // 이름을 입력받고 그 학생의 노드 정보 출력한다.
        else if (strcmp(command, "se") == 0) {
            char studentName[50];
            printf("학생 이름을 입력하세요: ");
            scanf("%s", studentName);
            print_node(root, studentName);
        }

        // 전체 leaf 노드 갯수 출력한다.
        else if (strcmp(command, "le") == 0) {
            int leafCount = 0;
            count_leaves(root, &leafCount);
            printf("전체 leaf 노드 개수: %d\n", leafCount);
        }
        // 프로그램 종료
        else if (strcmp(command, "ex") == 0) {
            printf("프로그램을 종료합니다.\n");
            break;
        }
        // 명령어를 다시 입력 받는다.
        else {
            printf("잘못된 명령입니다. 다시 입력하세요.\n");
        }
    }

    return 0;
}