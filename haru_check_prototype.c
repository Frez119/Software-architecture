#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Node{        //가게 정보를 리스트로 저장
    struct sales* sls;           //가게 판매량 입력
    struct store* str;           //가게 이름 입력
    struct Node* link;
}Node;

typedef struct store{  
	char name[20];      //가게 이름을 입력
	char addr[30];      //가게 주소 입력
}store;

typedef struct sales{       //가게 판매량 입력
	char itemname[20];      //물품 입력
	int sale;               //판매량 입력
}sales;

Node* createNode(char* name, char* addr){
    Node* new = (Node*)malloc(sizeof(Node));
    if(!new){
        printf("fail create node");
        exit(1);
    }
    strcpy(name, new->str->name);
    strcpy(addr, new->str->addr);
    new->link = NULL;
    return new;
}

void insert_node(Node** p2head, Node* before, Node* new){
    if(*p2head == NULL){
        new->link = NULL;
        *p2head = new;
    }
    else if(before == NULL){
        new->link = *p2head;
        *p2head = new;
    }
    else{
        new ->link = before ->link;
        before->link = new;
    }
}

Node* search(Node* from, char* name){ 
    Node* ptr = from;
    while(ptr){
        if(ptr->str->name == name)
            break;
    }
    return ptr;
}

void EntSale(Node* from, char name[20], int sale){      //판매량 입력
    Node* ptr = search(from, name);
    strcpy(ptr->sls->itemname,name);
    ptr->sls->sale = sale;
}

int main(){
    int num = NULL;
    int task = NULL;
    char name[20];
    while(1){
        printf("할 작업을 입력하십시오.\n");
        printf("1. 가게 정보 입력\n");
        printf("2. 가게 정보 확인\n");
        scanf("%d",&num);
        if(num == 1){
            task = 1;
            break;
        }
        else if(num == 2){
            task = 2;
            break;
        }
        else{
            printf("올바른 값을 입력해주세요!\n");
            continue;
        }
    }

    if(task == 1){
        printf("할 작업을 선택해주세요!\n");
        printf("1. 가게 기본 정보 입력\n");
        printf("2. 가게 매출 입력\n");
        scanf("%d",&num);
        if(num == 1){

        }
        else if(num == 2){

        }
    }
    else if(task == 2){
        printf("정보를 확인할 가게의 이름을 입력해주세요!\n");
        scanf("%s",name);
    }



}

