#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    struct sales *sls_head;
    struct store *str;
    struct Node *link;
} Node;

typedef struct store
{
    char name[100];
    char addr[100];
} store;

typedef struct sales
{
    char itemname[100];
    char sale[100];
    char date[100];
    struct sales *next;
} sales;

Node *createNode(char *name, char *addr)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
    {
        printf("fail create node");
        exit(1);
    }
    new_node->str = (store *)malloc(sizeof(store));
    if (!new_node->str)
    {
        printf("fail create store");
        exit(1);
    }
    strcpy(new_node->str->name, name);
    strcpy(new_node->str->addr, addr);
    new_node->sls_head = NULL; // ���� ���� ���� ����Ʈ �ʱ�ȭ
    new_node->link = NULL;
    return new_node;
}

void insert_node(Node **p2head, Node *before, Node *new)
{
    if (*p2head == NULL)
    {
        new->link = NULL;
        *p2head = new;
    }
    else if (before == NULL)
    {
        new->link = *p2head;
        *p2head = new;
    }
    else
    {
        new->link = before->link;
        before->link = new;
    }
}

Node *search(Node *from, char *name)
{
    Node *ptr = from;
    while (ptr)
    {
        if (strcmp(ptr->str->name, name) == 0)
            break;
        ptr = ptr->link;
    }
    return ptr;
}

void EntSale(Node *from, char *name, char *iname, char *sale, char *date)
{
    Node *ptr = search(from, name);

    // ���ο� ���� ���� ��� ����
    sales *new_sls = (sales *)malloc(sizeof(sales));
    strcpy(new_sls->itemname, iname);
    strcpy(new_sls->sale, sale);
    strcpy(new_sls->date, date); // ��¥ ���� ����
    new_sls->next = NULL;

    // ���� ���� ���� ����Ʈ�� �߰�
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
    printf("���� ��ϵ� ���� ����:\n");
    while (ptr != NULL)
    {
        printf("%d. ��ȣ��: %s, �ּ�: %s", count, ptr->str->name, ptr->str->addr);

        // ���� �ֽ� ���� ���� ���
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
            printf(", ��¥: %s, ǰ���: %s, �Ǹŷ�: %s", latest->date, latest->itemname, latest->sale);
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
        printf("�ش� ���� ������ �����ϴ�.\n");
        return;
    }

    printf("\n%s ������ �Ǹ� ���:\n", ptr->str->name);

    sales *curr = ptr->sls_head;
    int count = 1;
    while (curr != NULL)
    {
        printf("%d. ��¥: %s, �Ǹŷ�: %s, ǰ���: %s\n", count, curr->date, curr->sale, curr->itemname);
        curr = curr->next;
        count++;
    }

    if (count == 1)
    {
        printf("�Ǹ� ����� �����ϴ�.\n");
    }

    printf("\n");
}

void saveData(Node *head, const char *filename)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
    {
        printf("���� ���� ����\n");
        return;
    }

    Node *curr = head;
    while (curr != NULL)
    {
        fwrite(curr->str, sizeof(store), 1, fp);

        sales *salesNode = curr->sls_head;
        while (salesNode != NULL)
        {
            fwrite(salesNode, sizeof(sales), 1, fp);
            salesNode = salesNode->next;
        }

        // ���� �������� ���� ǥ���ϱ� ���� �� sales ����ü�� ����մϴ�.
        sales end_marker = {"", "", "", NULL};
        fwrite(&end_marker, sizeof(sales), 1, fp);

        curr = curr->link;
    }

    fclose(fp);
}

Node *loadData(Node **head, const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("���� ���� ����\n");
        return NULL;
    }

    store s;
    sales sls;
    while (fread(&s, sizeof(store), 1, fp) == 1)
    {
        Node *new_node = createNode(s.name, s.addr);
        insert_node(head, NULL, new_node);

        while (fread(&sls, sizeof(sales), 1, fp) == 1)
        {
            // �� ���� �����ͷ� ���� �˸�
            if (strcmp(sls.itemname, "") == 0 && strcmp(sls.sale, "") == 0 && strcmp(sls.date, "") == 0)
            {
                break;
            }

            sales *new_sales = (sales *)malloc(sizeof(sales));
            if (!new_sales)
            {
                printf("Failed to create sales record\n");
                exit(1);
            }
            strcpy(new_sales->itemname, sls.itemname);
            strcpy(new_sales->sale, sls.sale);
            strcpy(new_sales->date, sls.date);
            new_sales->next = new_node->sls_head;
            new_node->sls_head = new_sales;
        }
    }

    fclose(fp);
    return *head;
}

int main()
{
    Node *head = NULL;
    int num = 0;
    int user = 0;
    char sale[100];
    char name[100];
    char addr[100];
    char iname[100];
    char date[100];
    char input[101];
    char flag[10];
    const char *filename = "store_data.txt";

    loadData(&head, filename);

    printf("�α����Ͻ÷��� 1, �Ϲ� ����ڸ� 2�� �Է��ϼ���.\n");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &user);
    switch (user)
    {
    case 1:
        while (1)
        {
            printf("�� �۾��� �Է��Ͻʽÿ�.\n");
            printf("1. ���� ���� �Է�\n");
            printf("2. ���� ���� Ȯ��\n");
            printf("3. ���α׷� ����\n");
            fgets(input, sizeof(input), stdin);
            sscanf(input, "%d", &num);

            switch (num)
            {
            case 1:
                printf("\n�۾��� �������ּ���!\n");
                printf("1. ���� �⺻ ���� �Է�\n");
                printf("2. ���� ���� �Է�\n");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &num);

                switch (num)
                {
                case 1:
                {
                    printf("\n���� �̸��� �Է��ϼ��� : ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    printf("���� �ּҸ� �Է��ϼ��� : ");
                    fgets(addr, sizeof(addr), stdin);
                    addr[strcspn(addr, "\n")] = '\0';
                    Node *new_node = createNode(name, addr);
                    insert_node(&head, NULL, new_node);
                    printf("\n");
                    break;
                }
                case 2:
                {
                    printf("������ �Է��� ������ �̸��� �Է��ϼ��� : ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    Node *checkstore = search(head, name);
                    if (checkstore == NULL)
                    {
                        printf("�ش� ���Ը� ã�� �� �����ϴ�.\n");
                        break;
                    }
                    while (1)
                    {
                        printf("������ �Է��� ǰ����� �Է��ϼ��� : ");
                        fgets(iname, sizeof(iname), stdin);
                        iname[strcspn(iname, "\n")] = '\0';
                        printf("�Ǹŷ��� �Է��ϼ��� : ");
                        fgets(sale, sizeof(sale), stdin);
                        sale[strcspn(sale, "\n")] = '\0';
                        printf("��¥�� �Է��ϼ��� (YYYY-MM-DD) : ");
                        fgets(date, sizeof(date), stdin);
                        date[strcspn(date, "\n")] = '\0';

                        EntSale(head, name, iname, sale, date);

                        printf("�߰� ������ �Է� �Ͻðڽ��ϲ�? (y/n) : ");
                        fgets(flag, sizeof(flag), stdin);
                        flag[strcspn(flag, "\n")] = '\0';

                        if (strcmp(flag, "y") == 0 || strcmp(flag, "Y") == 0)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
                default:
                    printf("�ùٸ� ���� �Է����ּ���!\n");
                }
                break;
            case 2:
            {
                printf("\n�۾��� �������ּ���!\n");
                printf("1. ���Ը���Ʈ ����\n");
                printf("2. ���� �Ǹű�� ����\n");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &num);

                switch (num)
                {
                case 1:
                    printStoreList(head);
                    break;
                case 2:
                    printf("�Ǹ� ����� Ȯ���� ���� �̸��� �Է��ϼ���: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    printSalesRecord(head, name);
                    break;
                default:
                    printf("�ùٸ� ���� �Է����ּ���!\n");
                }
                break;
            }
            case 3:
                saveData(head, filename);
                exit(0);
                break;
            default:
                printf("�ùٸ� ���� �Է����ּ���!\n");
            }
        }
    case 2:
        while (1)
        {
            printf("�� �۾��� �Է��Ͻʽÿ�.\n");
            printf("1. ���� ���� Ȯ��\n");
            printf("2. ���α׷� ����\n");
            fgets(input, sizeof(input), stdin);
            sscanf(input, "%d", &num);

            switch (num)
            {
            case 1:
            {
                printf("\n�۾��� �������ּ���!\n");
                printf("1. ���Ը���Ʈ ����\n");
                printf("2. ���� �Ǹű�� ����\n");
                fgets(input, sizeof(input), stdin);
                sscanf(input, "%d", &num);

                switch (num)
                {
                case 1:
                    printStoreList(head);
                    break;
                case 2:
                    printf("�Ǹ� ����� Ȯ���� ���� �̸��� �Է��ϼ���: ");
                    fgets(name, sizeof(name), stdin);
                    name[strcspn(name, "\n")] = '\0';
                    printSalesRecord(head, name);
                    break;
                default:
                    printf("�ùٸ� ���� �Է����ּ���!\n");
                }
                break;
            }
            case 2:
                saveData(head, filename);
                exit(0);
                break;
            default:
                printf("�ùٸ� ���� �Է����ּ���!\n");
            }
        }
    default:
        break;
    }
    return 0;
}