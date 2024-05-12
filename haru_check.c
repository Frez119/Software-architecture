#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    struct sales* sls;
    struct store* str;
    struct Node* link;
} Node;

typedef struct store {
    char name[100];
    char addr[100];
} store;

typedef struct sales {
    char itemname[100];
    char sale[100];
} sales;

Node* createNode(char* name, char* addr) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        printf("fail create node");
        exit(1);
    }
    new_node->str = (store*)malloc(sizeof(store));
    if (!new_node->str) {
        printf("fail create store");
        exit(1);
    }
    strcpy(new_node->str->name, name);
    strcpy(new_node->str->addr, addr);
    new_node->sls = (sales*)malloc(sizeof(sales));
    if (!new_node->sls) {
        printf("fail create sales");
        exit(1);
    }
    new_node->link = NULL;
    return new_node;
}

void insert_node(Node** p2head, Node* before, Node* new) {
    if (*p2head == NULL) {
        new->link = NULL;
        *p2head = new;
    }
    else if (before == NULL) {
        new->link = *p2head;
        *p2head = new;
    }
    else {
        new->link = before->link;
        before->link = new;
    }
}

Node* search(Node* from, char* name) {
    Node* ptr = from;
    while (ptr) {
        if (strcmp(ptr->str->name, name) == 0) break;
        ptr = ptr->link;
    }
    return ptr;
}

void EntSale(Node* from, char* name, char* iname, char* sale) {
    Node* ptr = search(from, name);
    strcpy(ptr->sls->itemname, iname);
    strcpy(ptr->sls->sale, sale);
}

void printStoreInfo(Node* head) {
    Node* ptr = head;
    int count = 1;

    printf("\n");
    printf("현재 등록된 가게 정보:\n");
    while (ptr != NULL) {
        printf("%d. 상호명: %s, 주소: %s", count, ptr->str->name, ptr->str->addr);
        if (strlen(ptr->sls->itemname) > 0 && strlen(ptr->sls->sale) > 0) {
            printf(", 품목명: %s, 판매량: %s\n", ptr->sls->itemname, ptr->sls->sale);
        } else {
            printf("\n");
        }
        ptr = ptr->link;
        count++;
    }
}

int main() {
    Node* head = NULL;
    int num = 0;
    char sale[100];
    char name[100];
    char addr[100];
    char iname[100];
    char input[101];

    while (1) {
        printf("할 작업을 입력하십시오.\n");
        printf("1. 가게 정보 입력\n");
        printf("2. 가게 정보 확인\n");
        printf("3. 프로그램 종료\n");
        fgets(input, sizeof(input), stdin);
        sscanf(input, "%d", &num);

        switch (num) {
        case 1:
            printf("\n작업을 선택해주세요!\n");
            printf("1. 가게 기본 정보 입력\n");
            printf("2. 가게 매출 입력\n");
            fgets(input, sizeof(input), stdin);
            sscanf(input, "%d", &num);

            switch (num) {
            case 1:
                printf("\n가게 이름을 입력하세요 : ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                printf("가게 주소를 입력하세요 : ");
                fgets(addr, sizeof(addr), stdin);
                addr[strcspn(addr, "\n")] = '\0';
                Node* new_node = createNode(name, addr);
                insert_node(&head, NULL, new_node);
                printf("\n");
                break;
            case 2:
                printf("매출을 입력할 가게의 이름을 입력하세요 : ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                printf("매출을 입력할 품목명을 입력하세요 : ");
                fgets(iname, sizeof(iname), stdin);
                iname[strcspn(iname, "\n")] = '\0';
                printf("판매량을 입력하세요 : ");
                fgets(sale, sizeof(sale), stdin);
                sale[strcspn(sale, "\n")] = '\0';
                EntSale(head, name, iname, sale);
                break;
            default:
                printf("올바른 값을 입력해주세요!\n");
            }
            break;
        case 2:
            printStoreInfo(head);
            printf("\n");
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("올바른 값을 입력해주세요!\n");
        }
    }
    return 0;
}