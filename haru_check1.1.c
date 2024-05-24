#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    struct sales *sls_head;
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
    char date[100];
    struct sales* next;
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
    new_node->sls_head = NULL; // 매출 정보 연결 리스트 초기화
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

void EntSale(Node *from, char *name, char *iname, char *sale, char *date)
{
    Node *ptr = search(from, name);

    // 새로운 매출 정보 노드 생성
    sales *new_sls = (sales *)malloc(sizeof(sales));
    strcpy(new_sls->itemname, iname);
    strcpy(new_sls->sale, sale);
    strcpy(new_sls->date, date); // 날짜 정보 저장
    new_sls->next = NULL;

    // 매출 정보 연결 리스트에 추가
    if (ptr->sls_head == NULL)
    {
        ptr->sls_head = new_sls;
    }
    else
    {
        sales *curr = ptr->sls_head;
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = new_sls;
    }
    printf("\n");
}

void printStoreList(Node *head)
{
    Node *ptr = head;
    int count = 1;

    printf("\n");
    printf("현재 등록된 가게 정보:\n");
    while (ptr != NULL)
    {
        printf("%d. 상호명: %s, 주소: %s", count, ptr->str->name, ptr->str->addr);

        // 가장 최신 매출 정보 출력
        sales *curr = ptr->sls_head;
        sales *latest = NULL;
        while (curr != NULL)
        {
            if (latest == NULL || strcmp(curr->date, latest->date) > 0)
            {
                latest = curr;
            }
            curr = curr->next;
        }

        if (latest != NULL)
        {
            printf(", 품목명: %s, 판매량: %s, 날짜: %s", latest->itemname, latest->sale, latest->date);
        }

        printf("\n");
        ptr = ptr->link;
        count++;
    }
    printf("\n");
}

void printSalesRecord(Node *head, char *name)
{
    Node *ptr = search(head, name);

    if (ptr == NULL)
    {
        printf("해당 가게 정보가 없습니다.\n");
        return;
    }

    printf("\n%s 가게의 판매 기록:\n", ptr->str->name);

    sales *curr = ptr->sls_head;
    int count = 1;
    while (curr != NULL)
    {
        printf("%d. 품목명: %s, 판매량: %s, 날짜: %s\n", count, curr->itemname, curr->sale, curr->date);
        curr = curr->next;
        count++;
    }

    if (count == 1)
    {
        printf("판매 기록이 없습니다.\n");
    }

    printf("\n");
}

void saveData(Node *head, const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("Failed to open file for writing.\n");
        return;
    }

    Node *curr = head;
    while (curr != NULL)
    {
        fprintf(fp, "%s,%s\n", curr->str->name, curr->str->addr);
        sales *salesNode = curr->sls_head;
        while (salesNode != NULL)
        {
            fprintf(fp, "%s,%s,%s,%s\n", curr->str->name, salesNode->itemname, salesNode->sale, salesNode->date);
            salesNode = salesNode->next;
        }
        curr = curr->link;
    }

    fclose(fp);
}

Node *loadData(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Failed to open file for reading.\n");
        return NULL;
    }

    Node *head = NULL;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        char name[100], addr[100];
        if (sscanf(line, "%[^,],%s", name, addr) == 2)
        {
            Node *new_node = createNode(name, addr);
            insert_node(&head, NULL, new_node);
        }
        else
        {
            char storeName[100], itemname[100], sale[100], date[11];
            if (sscanf(line, "%[^,],%[^,],%[^,],%s", storeName, itemname, sale, date) == 4)
            {
                Node *ptr = search(head, storeName);
                if (ptr != NULL)
                {
                    EntSale(head, storeName, itemname, sale, date);
                }
            }
        }
    }

    fclose(fp);
    return head;
}

int main() {
    Node *head = loadData("store_data.txt");
    int num = 0;
    char sale[100];
    char name[100];
    char addr[100];
    char iname[100];
    char date[100];
    char input[101];
    char flag[10];

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
                    while (1)
                    {
                        name[strcspn(name, "\n")] = '\0';
                        printf("매출을 입력할 품목명을 입력하세요 : ");
                        fgets(iname, sizeof(iname), stdin);
                        iname[strcspn(iname, "\n")] = '\0';
                        printf("판매량을 입력하세요 : ");
                        fgets(sale, sizeof(sale), stdin);
                        sale[strcspn(sale, "\n")] = '\0';
                        printf("날짜를 입력하세요 (YYYY-MM-DD) : ");
                        fgets(date, sizeof(date), stdin);
                        date[strcspn(date, "\n")] = '\0';

                        EntSale(head, name, iname, sale, date); // 매출 입력

                        printf("추가 매출을 입력 하시겠습니꺄? (y/n) : ");
                        fgets(flag, sizeof(flag), stdin);
                        flag[strcspn(flag, "\n")] = '\0';
                        
                        if (strcmp(flag, "n") == 0 || strcmp(flag, "N") == 0)
                        {
                            break;
                        }
                    }
                    break;
            default:
                printf("올바른 값을 입력해주세요!\n");
            }
            break;
        case 2:
            printf("\n작업을 선택해주세요!\n");
            printf("1. 가게리스트 보기\n");
            printf("2. 가게 판매기록 보기\n");
            fgets(input, sizeof(input), stdin);
            sscanf(input, "%d", &num);

            switch (num)
            {
            case 1:
                printStoreList(head);
                break;
            case 2:
                printf("판매 기록을 확인할 가게 이름을 입력하세요: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0';
                printSalesRecord(head, name);
                break;
            default:
                printf("올바른 값을 입력해주세요!\n");
            }
            break;
        case 3:
            exit(0);
            break;
        default:
            printf("올바른 값을 입력해주세요!\n");
        }
    }
    saveData(head, "store_data.txt");
    return 0;
}