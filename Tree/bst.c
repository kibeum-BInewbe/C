#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct treenode* TreePointer;

typedef struct element {
    int sno;        // �й�
    char name[50];  // �л� �̸�
} ele;

typedef struct treenode {
    ele data;   // data ���
    TreePointer leftChild, rightChild;  // �¿� �ڽĳ�� ����Ű�� ������
} treenode;

// �Լ�
TreePointer create_node(ele data);
void print_tree(TreePointer node);
int get_height(TreePointer node);
void insert(TreePointer* root, ele data);
TreePointer delete_node(TreePointer node, int key);
TreePointer find_min(TreePointer node);
void count_leaves(TreePointer node, int* count);
void print_child_nodes(TreePointer node, const char* name);
void print_node(TreePointer node, const char* name);

// ������ �Լ�
TreePointer create_node(ele data) {
    TreePointer newNode = (treenode*)malloc(sizeof(treenode));
    newNode->data = data;   // ��忡 �����͸� ����
    newNode->leftChild = NULL;  // �¿� �ڽĳ�� ������ �ʱ�ȭ
    newNode->rightChild = NULL;
    return newNode;
}

// ��ü tree ��� �Լ�
void print_tree(TreePointer node) {
    if (node == NULL) return;
    printf("%s, %d\n", node->data.name, node->data.sno);
    print_tree(node->leftChild);
    print_tree(node->rightChild);
}

// tree ���� ��� �Լ�
int get_height(TreePointer node) {
    // tree�� ��������� 0 ��ȯ
    if (node == NULL) return 0;
    int left_height = get_height(node->leftChild);
    int right_height = get_height(node->rightChild);
    return 1 + (left_height > right_height ? left_height : right_height); // �� ���� ���� �ڽ� ���̿� 1�� ���� ���� ���� ����� ����
}

// �Է��� �����͸� ��忡 �����Ͽ� tree�� �����ϴ� �Լ�
void insert(TreePointer* root, ele data) {
    // tree�� ��������� root�� ��� ����
    if (*root == NULL) {
        *root = create_node(data);
        return;
    }
    // ���Գ�� key�� root�� key���� ������
    if (data.sno < (*root)->data.sno)
        // ���� �ڽĳ�忡 ����
        insert(&(*root)->leftChild, data);
    // ���Գ�� key�� root�� key���� ũ��
    else if (data.sno > (*root)->data.sno)
        // ������ �ڽĳ�忡 ����
        insert(&(*root)->rightChild, data);
}

// �Է��� �й��� ���� ��带 ã�� tree���� �����ϴ� �Լ�
TreePointer delete_node(TreePointer node, int key) {
    if (node == NULL) return node;
    // key���� �������� key������ ������
    if (key < node->data.sno)
        // �������� ��������.
        node->leftChild = delete_node(node->leftChild, key);
    // key���� �������� key������ ũ��
    else if (key > node->data.sno)
        // ���������� ��������.
        node->rightChild = delete_node(node->rightChild, key);
    // key���� �������� key���� ����! = ��� �߰�!
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
        // ���� ��, �θ��尡 ���� �ڽĳ�尡 ����� ���� ���� key���� ���� ��带 ã�Ƽ� �� ���� ä���.
        TreePointer temp = find_min(node->rightChild);
        node->data = temp->data;
        node->rightChild = delete_node(node->rightChild, temp->data.sno);
    }
    return node;
}

// ��尡 �����Ǿ��� �� �� ���� ä��� ���� �Լ�
TreePointer find_min(TreePointer node) {
    TreePointer current = node;
    // �θ��尡 ������� ��, ���� �ڽĳ�嵵 ����ִٸ�
    while (current && current->leftChild != NULL)
        // ���� �ڽĳ�尡 �θ����
        current = current->leftChild;
    return current;
}

// leaf ��� ���� ����Լ�
void count_leaves(TreePointer node, int* count) {
    if (node == NULL) {
        return;
    }
    // ���ʰ� ������ �� �ڽ� ��尡 ��������� leaf ���!
    if (node->leftChild == NULL && node->rightChild == NULL) {
        (*count)++;
    }
    // ����Լ�
    count_leaves(node->leftChild, count);
    count_leaves(node->rightChild, count);
}


// �Է��� �л� �̸��� ��带 ã�Ƽ� �¿� ��忡�� �л� �̸� ������ ��� ����ϴ� �Լ�
void print_child_nodes(TreePointer node, const char* name) {
    if (node == NULL) {
        printf("�׷� �л� �����ϴ�.\n");
        return;
    }
    // �ش� ��� �߰� ��, �¿� ��� ���� ���
    if (strcmp(node->data.name, name) == 0) {
        printf("�л� �̸�: %s\n", node->data.name);
        printf("left child: ");
        if (node->leftChild != NULL) {
            printf("%s\t", node->leftChild->data.name);
        } else {
            printf("����\n");
        }
        printf("right child: ");
        if (node->rightChild != NULL) {
            printf("%s\n", node->rightChild->data.name);
        } else {
            printf("����\n");
        }
    }
    // 
    else if (strcmp(name, node->data.name) < 0) {
        print_child_nodes(node->leftChild, name);
    } else {
        print_child_nodes(node->rightChild, name);
    }
}
// �̸��� �Է¹ް� ��� ������ ����Ѵ�.
void print_node(TreePointer node, const char* name) {
    if (node == NULL) {
        printf("�׷� �л� �����ϴ�.");
        return;
    }
    if (strcmp(node->data.name, name) == 0) {
        printf("�̸�: %s, �й�: %d, ���� ��ġ: %d\n", node->data.name, node->data.sno, get_height(node));
    }
    // key���� ������ ��������, �ƴϸ� ���������� Ž��
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
        printf("�׷� �л� �����ϴ�.");
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
    // ������ 0���� ���� ���� ����.
    return level >= 0 ? level : -1;
}


int main(void) {
    TreePointer root = NULL;
    const char* file_name = "./sdata.txt";

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("���� ���� ����.\n");
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
        printf("������ �۾��� (in, sp, de, se, ht, ch, le, ex) ?");
        scanf("%s", command);

        // tree ���� ���
        if (strcmp(command, "sp") == 0) {
            printf("ȭ���� ������ ������ �����ϴ�:\n");
            print_tree(root);
        }
        // tree ���� ���
        else if (strcmp(command, "ht") == 0) {
            printf("Ʈ�� ����: %d\n", get_height(root));
        }
        // ������ ���� �� ��� ����Ѵ�.
        else if (strcmp(command, "in") == 0) {
            printf("�л� ��ȣ�� �Է��ϼ���: ");
            scanf("%d", &sno);
            printf("�л� �̸��� �Է��ϼ���: ");
            scanf("%s", name);
            ele data;
            data.sno = sno;
            strncpy(data.name, name, sizeof(data.name));

            insert(&root, data);

            // ���Ե� ����� ���� ��ġ�� ���
            TreePointer insertedNode = find_node(root, data.name);

            if (insertedNode != NULL) {
                printf("�Է� ����!\n");
                printf("level = %d\n", get_level_position(root, insertedNode));
            } else {
            printf("�Է� ����!\n");
            }
        }
        // �й��� �Է¹ް� �� ��带 �����Ѵ�.
        else if (strcmp(command, "de") == 0) {
            printf("������ �л� ��ȣ�� �Է��ϼ���: ");
            scanf("%d", &name);
            root = delete_node(root, sno);
            printf("��尡 Ʈ������ �����Ǿ����ϴ�.\n");
        }
        // �̸��� �Է¹ް� �¿� �ڽ� ��� ���� ����Ѵ�.
        else if (strcmp(command, "ch") == 0) {
            char studentName[50];
            printf("�л� �̸��� �Է��ϼ���: ");
            scanf("%s", studentName);
            print_child_nodes(root, studentName);
        }

        // �̸��� �Է¹ް� �� �л��� ��� ���� ����Ѵ�.
        else if (strcmp(command, "se") == 0) {
            char studentName[50];
            printf("�л� �̸��� �Է��ϼ���: ");
            scanf("%s", studentName);
            print_node(root, studentName);
        }

        // ��ü leaf ��� ���� ����Ѵ�.
        else if (strcmp(command, "le") == 0) {
            int leafCount = 0;
            count_leaves(root, &leafCount);
            printf("��ü leaf ��� ����: %d\n", leafCount);
        }
        // ���α׷� ����
        else if (strcmp(command, "ex") == 0) {
            printf("���α׷��� �����մϴ�.\n");
            break;
        }
        // ��ɾ �ٽ� �Է� �޴´�.
        else {
            printf("�߸��� ����Դϴ�. �ٽ� �Է��ϼ���.\n");
        }
    }

    return 0;
}