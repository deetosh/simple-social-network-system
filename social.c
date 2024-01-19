/******************************************************************************
 * File: social.c
 * Author: Deetosh Kumar Kuila
 * Date: 15-01-2024
 * Description: Implementation of a simple social network system.
 ******************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"social.h"

int unique_id=100;

void print_node_types(){
    printf("1-Individual\n2-Group\n3-Business\n4-Organisation\n");
}

//==================================================================//
// Linked list functions
listNode* createListNode(Node* node){ 
    listNode* new_node= (listNode*)malloc(sizeof(listNode));
    new_node->n=node;
    new_node->next=NULL;
    return new_node;
}

listNode* insertListNode(listNode* head,Node* node){ 
    listNode* new_node= createListNode(node);
    new_node->next=head;
    head=new_node;
    return head;
}

listNode* eraseListNode(listNode* head, Node* node){ 
    listNode* prev=NULL;
    listNode* temp=head;

    while(temp!=NULL && temp->n!=node){
        prev=temp;
        temp=temp->next;
    }
    if(temp!=NULL && temp->n==node){
        if(temp==head){
            head=temp->next;
        }
        else{
            prev->next=temp->next;
        }
        temp->next=NULL;
        temp->n=NULL;
        free(temp);
    }
    return head;
}

void deleteLinkedList(listNode* head) { 
    listNode* current = head;
    listNode* nextNode;

    while (current != NULL) {
        nextNode = current->next; 
        free(current);            
        current = nextNode;   
    }
}

//==================================================================//
// Content Linked list functions
content_list* create_node_content(content* c){ 
    content_list* new_node= (content_list*)malloc(sizeof(content_list));
    new_node->c=c;
    new_node->next=NULL;
    return new_node;
}

content_list* insert_content(content_list* head,content* cont){ 
    content_list* new_node= create_node_content(cont);
    new_node->next=head;
    head=new_node;
    return head;
}

content_list* erase_content(content_list* head,content* cont){ 
    content_list* prev=NULL;
    content_list* temp=head;
    while(temp!=NULL && temp->c!=cont){
        prev=temp;
        temp=temp->next;
    }
    if(temp!=NULL && temp->c==cont){
        if(temp==head){
            head=temp->next;
        }
        else{
            prev->next=temp->next;
        }
        temp->next=NULL;
        temp->c=NULL;
        free(temp);
    }
    return head;
    
}

void delete_content_list(content_list* head, content_list* all_contents){ 
    content_list* current = head;
    content_list* nextNode;
    while (current != NULL) {
        nextNode = current->next; 
        all_contents= erase_content(all_contents,current->c);
        current->c=NULL;
        current->next=NULL;
        free(current);           
        current = nextNode;       
        head = nextNode; 
    }
}

//==================================================================//
//CREATING A NEW NODE [1]
Node* create_node(int type,int id, char* name, date creation_date){
    Node* new_node= (Node*)malloc(sizeof(Node));
    new_node->type=type;
    new_node->wallet=1; // used during 2-hop printing

    // individual
    if(type==1){ 
        new_node->kind.individual.id = id;
        new_node->kind.individual.creation_date = creation_date;
        strcpy(new_node->kind.individual.name, name);
        new_node->kind.individual.posts=NULL;
        new_node->kind.individual.group_i=NULL;
        new_node->kind.individual.organisation_i=NULL;
        new_node->kind.individual.business_i=NULL;
        strcpy(new_node->kind.individual.birthday.dmy, "000000");

        // Handle birthday input
        printf("Do You want to enter the birthday (Y\\N): \n");
        char c;
        c=getchar();
        if(c=='Y' || c=='y'){
            printf("Enter your birthday in order DD/MM/YYYY:\n");
            getchar(); // Consume the newline character left by previous input
            fgets(new_node->kind.individual.birthday.dmy,12,stdin);
            new_node->kind.individual.birthday.dmy[strlen(new_node->kind.individual.birthday.dmy)-1]='\0';
        }
    }
    else if(type==2){ //group
        new_node->kind.group.id = id;
        new_node->kind.group.creation_date = creation_date;
        strcpy(new_node->kind.group.name, name);
        new_node->kind.group.posts=NULL;
        new_node->kind.group.members=NULL;
    }
    else if(type==3){ //business
        new_node->kind.business.id = id;
        new_node->kind.business.creation_date = creation_date;
        strcpy(new_node->kind.business.name, name);
        new_node->kind.business.posts=NULL;
        new_node->kind.business.location.x=-1;
        new_node->kind.business.location.y=-1;
        new_node->kind.business.owners=NULL;
        new_node->kind.business.customers=NULL;
        new_node->kind.business.group_b=NULL;

        // Handle location input
        printf("Do You want to enter the location (Y\\N): \n");
        char c;
        c=getchar();
        if(c=='Y' || c=='y'){
            printf("Enter the 1st coordinate:\n");
            scanf("%lf",&new_node->kind.business.location.x);
            printf("Enter the 2nd coordinate:\n");
            scanf("%lf",&new_node->kind.business.location.y);
        }
    }
    else if(type==4){ //organisation
        new_node->kind.organisation.id = id;
        new_node->kind.organisation.creation_date = creation_date;
        strcpy(new_node->kind.organisation.name, name);
        new_node->kind.organisation.posts=NULL;
        new_node->kind.organisation.members=NULL;
        new_node->kind.organisation.location.x=-1;
        new_node->kind.organisation.location.y=-1;

        // Handle location input
        printf("Do You want to enter the location (Y\\N): \n");
        char c;
        c=getchar();
        if(c=='Y' || c=='y'){
            printf("Enter the 1st coordinate:\n");
            scanf("%lf",&new_node->kind.organisation.location.x);
            printf("Enter the 2nd coordinate:\n");
            scanf("%lf",&new_node->kind.organisation.location.y);
        }
    }
    return new_node;
}

//==================================================================//
//DELETING A NODE FROM A LIST [1]
listNode* delete_node_info(listNode* all_nodes, content_list* all_contents, Node* node){
    // if node is found
    if(node!=NULL){
        // if node is an individual
        if(node->type==1){
            Individual* individual = &(node->kind.individual);
            // Remove from linked groups
            listNode* groupNode = individual->group_i;
            while (groupNode != NULL) {
                Group* group = &(groupNode->n->kind.group);
                group->members = eraseListNode(group->members, node);
                groupNode = groupNode->next;
            }
            deleteLinkedList(groupNode);

            // Remove from linked organisations
            listNode* orgNode = individual->organisation_i;
            while (orgNode != NULL) {
                Organisation* organisation = &(orgNode->n->kind.organisation);
                organisation->members = eraseListNode(organisation->members, node);
                orgNode = orgNode->next;
            }
            deleteLinkedList(orgNode);

            // Remove from linked businesses
            listNode* businessNode = individual->business_i;
            while (businessNode != NULL) {
                Business* business = &(businessNode->n->kind.business);
                // Check if the individual is an owner or customer
                business->owners = eraseListNode(business->owners, node);
                business->customers = eraseListNode(business->customers, node);
                businessNode = businessNode->next;
            }
            deleteLinkedList(businessNode);

            delete_content_list(individual->posts,all_contents);
            
        }

        else if(node->type==2){
            Group* group = &(node->kind.group);

            listNode* memberNode = group->members;
            while (memberNode != NULL) {
                if(memberNode->n->type==1){
                     Individual* member = &(memberNode->n->kind.individual);
                     member->group_i = eraseListNode(member->group_i, node);
                }
                else if(memberNode->n->type==2){
                     Business* member = &(memberNode->n->kind.business);
                     member->group_b = eraseListNode(member->group_b, node); 
                }
                memberNode = memberNode->next;
            }
            deleteLinkedList(memberNode);

            delete_content_list(group->posts,all_contents);
            
        }

        else if(node->type==3){
            Business* business= &(node->kind.business);
            listNode* grouplist= business->group_b;
            while(grouplist!=NULL){
                Group* group = &(grouplist->n->kind.group);
                group->members= eraseListNode(group->members,node);
                grouplist=grouplist->next;
            }

            deleteLinkedList(grouplist);
            delete_content_list(business->posts,all_contents);
            
        }

        else if(node->type==4){
            Organisation* org = &(node->kind.organisation);

            listNode* memberNode = org->members;
            while (memberNode != NULL) {
                if(memberNode->n->type==1){
                     Individual* member = &(memberNode->n->kind.individual);
                     member->organisation_i = eraseListNode(member->organisation_i, node);
                }
                memberNode = memberNode->next;
            }
            deleteLinkedList(memberNode);

            delete_content_list(org->posts,all_contents);
            
        }
        all_nodes=eraseListNode(all_nodes,node); 
        free(node);  
    }
    return all_nodes;
}

//==================================================================//
//HELPING FUNCTIONS

//GETTING THE NAME OF THE NODE
char* get_name(Node* node){
    switch(node->type){
        case 1:
            return node->kind.individual.name;
            break;
        case 2:
            return node->kind.group.name;
            break;
        case 3:
            return node->kind.business.name;
            break;
        case 4:
            return node->kind.organisation.name;
            break;
    }
}

//GETTING THE ID OF THE USER OF THE NODE
int get_id(Node* node){
    switch(node->type){
        case 1:
            return node->kind.individual.id;
            break;
        case 2:
            return node->kind.group.id;
            break;
        case 3:
            return node->kind.business.id;
            break;
        case 4:
            return node->kind.organisation.id;
            break;
    }
}

//PRINTING THE INFORATION OF A NODE
void print_info_node(Node* node){
    printf("\n");
    switch(node->type){
        case 1:
            printf("ID: %d\n",node->kind.individual.id);
            printf("Name: %s\n",node->kind.individual.name);
            printf("Type: Individual\n");
            printf("Creation date: %s\n",node->kind.individual.creation_date.dmy);
            if(strcmp(node->kind.individual.birthday.dmy,"000000")!=0){
                printf("Birthday: %s\n",node->kind.individual.birthday.dmy);
            }
            break;
        case 2:
            printf("ID: %d\n",node->kind.group.id);
            printf("Name: %s\n",node->kind.group.name);
            printf("Type: Group\n");
            printf("Creation date: %s\n",node->kind.group.creation_date.dmy);
            break;
        case 3:
            printf("ID: %d\n",node->kind.business.id);
            printf("Name: %s\n",node->kind.business.name);
            printf("Type: Business\n");
            printf("Creation date: %s\n",node->kind.business.creation_date.dmy);
            if(node->kind.business.location.x!=-1 && node->kind.business.location.y!=-1){
                printf("Location: (%lf,%lf)\n",node->kind.business.location.x,node->kind.business.location.y);
            }
            break;
        case 4:
            printf("ID: %d\n",node->kind.organisation.id);
            printf("Name: %s\n",node->kind.organisation.name);
            printf("Type: Organisation\n");
            printf("Creation date: %s\n",node->kind.organisation.creation_date.dmy);
            if(node->kind.organisation.location.x!=-1 && node->kind.organisation.location.y!=-1){
                printf("Location: (%lf,%lf)\n",node->kind.organisation.location.x,node->kind.organisation.location.y);
            }
            break;
    }
}

//==================================================================//
//SEARCH FUNCTIONS [2]

//TAKING A NAME AS INPUT AND SEARCHGING BY NAME
void search_node_name(listNode* head){
    char name[MAX_NAME_LENGTH];
    printf("\nEnter the name of node to be searched:\n");
    getchar();
    fgets(name,sizeof(name),stdin);
    name[strlen(name)-1]='\0';

    int flag=0;
    listNode* temp=head;
    while(temp!=NULL){
        Node* curr= temp->n;
        switch (curr->type){
            case 1:
                if(strstr(curr->kind.individual.name,name)){
                    print_info_node(curr);
                    flag=1;
                }
                break;
            
            case 2:
                if(strstr(curr->kind.group.name,name)){
                    print_info_node(curr);
                    flag=1;
                }
                break;
            
            case 3:
                if(strstr(curr->kind.business.name,name)){
                    print_info_node(curr);
                    flag=1;
                }
                break;

            case 4:
                if(strstr(curr->kind.organisation.name,name)){
                    print_info_node(curr);
                    flag=1;
                }
                break;
        }
        temp=temp->next;
    }
    if(flag==0){
        printf("No nodes found with name %s\n",name);
    }
}

//SEARCHGING BY ID
Node* search_node_id(listNode* head,int id){
    listNode* temp=head;
    while(temp!=NULL){
        Node* curr= temp->n;
        switch (curr->type){
            case 1:
                if(curr->kind.individual.id==id){
                    return curr;
                }
                break;
            
            case 2:
                if(curr->kind.group.id==id){
                    return curr;
                }
                break;
            
            case 3:
                if(curr->kind.business.id==id){
                    return curr;
                }
                break;

            case 4:
                if(curr->kind.organisation.id==id){
                    return curr;
                }
                break;
        }
        temp=temp->next;
    }
    return NULL; // if not found
}

//TAKING A TYPE AS INPUT, SEARCHING AND PRINTING BY TYPE
void search_node_type(listNode* head){
    int type_;
    printf("\nEnter the type of node you want to search:\n");
    print_node_types();
    scanf("%d",&type_);
    if(type_ !=1 || type_!=2 || type_!=3 || type_!=4){
        printf("Wrong Input!!!\n");
        return;
    }
    printf("\nprinting the node of type %d\n",type_);
    listNode* temp=head;
    while(temp!=NULL){
        Node* curr= temp->n;
        if(curr->type==type_){
            printf("%s-%d\n",get_name(curr),get_id(curr));
        }
        temp=temp->next;
    }
}

//SEARCHING AND PRINTING BY BIRTHDAY
void search_node_birthday(listNode* head){
    char d[12];
    printf("\nEnter a date to search a node:\n");
    getchar();
    fgets(d,sizeof(d),stdin);
    d[strlen(d)-1]='\0';
    printf("\nprinting the node having birthday %s\n",d);

    listNode* temp=head;
    while(temp!=NULL){
        Node* curr= temp->n;
        if(curr->type==1 && strcmp(d,curr->kind.individual.birthday.dmy)==0){
            printf("%s-%d\n",get_name(curr),get_id(curr));
        }
        temp=temp->next;
    }
}

//==================================================================//
//PRINT ALL THE NODES IN THE LINKED LIST
void print_linked(listNode* head){
    listNode* temp=head;
    while(temp!=NULL){
        Node* curr= temp->n;
        switch (curr->type){
            case 1:
                printf("%s ",curr->kind.individual.name);
                break;
            
            case 2:
                printf("%s ",curr->kind.group.name);
                break;
            
            case 3:
                printf("%s ",curr->kind.business.name);
                break;

            case 4:
                printf("%s ",curr->kind.organisation.name);
                break;
        }
        temp=temp->next;
        if(temp)printf(",");
    }
    printf("\n");
}

//PRINT THE JUST CONNECTED(1-HOP) NODES
void print_1_hop(Node* node){
    printf("printing the nodes linked to %s\n",get_name(node));
    switch (node->type){
            case 1:
                printf("Linked Groups: \n");
                print_linked(node->kind.individual.group_i);
                printf("Linked Businesses: \n");
                print_linked(node->kind.individual.business_i);
                printf("Linked Organisations: \n");
                print_linked(node->kind.individual.organisation_i);
                break;
            
            case 2:
                printf("Linked Members: \n");
                print_linked(node->kind.group.members);
                break;
            
            case 3:
                printf("Linked Owners: \n");
                print_linked(node->kind.business.owners);
                printf("Linked Customers: \n");
                print_linked(node->kind.business.customers);
                printf("Linked Groups: \n");
                print_linked(node->kind.business.group_b);
                break;

            case 4:
                printf("Linked Members: \n");
                print_linked(node->kind.organisation.members);
                break;
        }
}

//==================================================================//
content* search_exact_content(content_list* head, char* content){
    content_list* temp=head;
    while(temp!=NULL){
        if(strcmp(temp->c->Content,content)==0){
            return temp->c;
        }
        temp=temp->next;
    }
    return NULL;
}

//CREATING A NEW POST/CONTENT
content* create_content(content_list* head){
    char new[MAX_CONTENT_LENGTH];
    printf("Enter a post in text:\n");
    getchar();
    fgets(new,sizeof(new),stdin);
    new[strlen(new)-1]='\0';
    content* search= search_exact_content(head,new);
    if(search==NULL){
        content* new_content= (content*)malloc(sizeof(content));
        strcpy(new_content->Content,new);
        return new_content;
    }
    else{
        printf("This content is reposted...\n");
        return search;
    }
}

// POSTING A NEW CONTENT
content_list* post(content_list* head,Node* node){
    content* new_post= create_content(head);
    switch (node->type){
            case 1:
                node->kind.individual.posts= insert_content(node->kind.individual.posts,new_post);
                break;
            
            case 2:
                node->kind.group.posts= insert_content(node->kind.group.posts,new_post);
                break;
            
            case 3:
                node->kind.business.posts= insert_content(node->kind.business.posts,new_post);
                break;

            case 4:
                node->kind.organisation.posts= insert_content(node->kind.organisation.posts,new_post);
                break;
        }
    
    head= insert_content(head,new_post);
    return head;
}

//DISPLAY THE CONTENTS POSTED BY A NODE
void display_content(Node* node){
    printf("Displaying all the contents posted by ");
    printf("%s :\n",get_name(node));
    content_list* temp;
    switch (node->type){
            case 1:
                temp= node->kind.individual.posts;
                break;
            
            case 2:
                temp= node->kind.group.posts;
                break;
            
            case 3:
                temp= node->kind.business.posts;
                break;

            case 4:
                temp= node->kind.organisation.posts;
                break;
        }
    while(temp!=NULL){
        printf("%s\n",temp->c->Content);
        temp=temp->next;
    }
}

//print all contents in a linked list
void print_all_contents(content_list* head){
    printf("\nPrinting all contents\n");
    content_list* temp=head;
    while(temp!=NULL){
        printf("%s\n",temp->c->Content);
        temp=temp->next;
    }
}

//==================================================================//
//PRINT A LINKED LIST
void print_list(listNode* head){
    listNode* temp=head;
    while(temp!=NULL){
        Node* curr= temp->n;
        print_info_node(curr);
        temp=temp->next;
    }
}

//==================================================================//
//CREATE THE SPECIAL LINKS
void add_indi_members(Node* indi, Node* node){
    if(indi->type==1 && node->type==2){ // link between individual and a group
        node->kind.group.members= insertListNode(node->kind.group.members,indi);
        indi->kind.individual.group_i= insertListNode(indi->kind.individual.group_i, node);
    }
    else if(indi->type==1 && node->type==4){ // link between individual and an organisation
        node->kind.organisation.members= insertListNode(node->kind.organisation.members,indi);
        indi->kind.individual.organisation_i= insertListNode(indi->kind.individual.organisation_i, node);
    }
    else{
        printf("This link can not be formed.");
    }
    printf("member added\n");
}

void add_biss_members(Node* biss, Node* grp){
    if(biss->type==3 && grp->type==2){
        grp->kind.group.members= insertListNode(grp->kind.group.members,biss);
        biss->kind.business.group_b= insertListNode(biss->kind.business.group_b,grp);
    }
    else{
        printf("This link can not be formed.");
    }
    printf("business member added\n");
}

void add_owners_biss(Node* indi, Node* biss){
    if(indi->type==1 && biss->type==3){
        biss->kind.business.owners= insertListNode(biss->kind.business.owners,indi);
        indi->kind.individual.business_i = insertListNode(indi->kind.individual.business_i, biss);
    }
    else{
        printf("This link can not be formed.");
    }
    printf("owner added\n");
}

void add_customers_biss(Node* indi, Node* biss){
    if(indi->type==1 && biss->type==3){
        biss->kind.business.customers = insertListNode(biss->kind.business.customers,indi);
        indi->kind.individual.business_i = insertListNode(indi->kind.individual.business_i, biss);
    }
    else{
        printf("This link can not be formed.");
    }
    printf("customer added\n");
}

// HANDLING INPUTS FROM USER AND LINKING TWO NODES
void linking(listNode* all_nodes){
    printf("\nEnter which kind of link you want to create:\n");
    printf("a- individual to group as member\n");
    printf("b- individual to organisation as member\n");
    printf("c- business to group as member\n");
    printf("d- individual to business as owner\n");
    printf("e- individual to business as customer\n");
    char c;
    getchar();
    scanf("%c",&c);
    int id1,id2;
    Node* a;
    Node* b;

    if(c=='a'||c=='b'||c=='e'||c=='d'){
        printf("Enter the id of an individual node: \n");
        scanf("%d",&id1);
        a=search_node_id(all_nodes,id1);
        if(a==NULL || a->type!=1){
            printf("wrong input!!!\n");
            return;
        }
    }
    else if(c=='c'){
        printf("Enter the id of a business node\n");
        scanf("%d",&id1);
        a=search_node_id(all_nodes,id1);
        if(a==NULL || a->type!=3){
            printf("wrong input!!!\n");
            return;
        }
    }
    else{
        printf("Wrong input!!!\n");
        return;
    }

    if(c=='a'||c=='c'){
        printf("Enter the id of a group node: \n");
        scanf("%d",&id2);
        b=search_node_id(all_nodes,id2);
        if(b==NULL || b->type!=2){
            printf("wrong input!!!\n");
            return;
        }
    }
    else if(c=='d'||c=='e'){
        printf("Enter the id of a business node: \n");
        scanf("%d",&id2);
        b=search_node_id(all_nodes,id2);
        if(b==NULL || b->type!=3){
            printf("wrong input!!!\n");
            return;
        }
    }
    else if(c=='b'){
        printf("Enter the id of a organisation node\n");
        scanf("%d",&id2);
        b=search_node_id(all_nodes,id2);
        if(b==NULL || b->type!=4){
            printf("wrong input!!!\n");
            return;
        }
    }
    else{
        printf("Wrong input!!!\n");
        return;
    }

    switch(c){
        case 'a':
            add_indi_members(a,b);
            break;
        case 'b':
            add_indi_members(a,b);
            break;
        case 'c':
            add_biss_members(a,b);
            break;
        case 'd':
            add_owners_biss(a,b);
            break;
        case 'e':
            add_customers_biss(a,b);
            break;
    }
    return; 
}

//==================================================================//
// CONVERTING A STRING TO LOWER CASE
char* convertToLowercase(char* temp,char *str) {
    strcpy(temp,str);
    for(int i = 0; temp[i]; i++) {
        if(temp[i]>='A' && temp[i]<='Z'){
            temp[i]=temp[i]-'A'+'a';
        }
    }
    return temp;
}

void search_content(content_list* head, char* content){
    content_list* temp=head;
    printf("\nShowing the searched strings !!!\n");
    char copy1[MAX_CONTENT_LENGTH];
    char copy2[MAX_CONTENT_LENGTH];
    while(temp!=NULL){
        // printf("%s %d\n",temp->c->Content,strlen(temp->c->Content));
        // printf("%s %d\n",content,strlen(content));
        if(strstr(convertToLowercase(copy1,temp->c->Content),convertToLowercase(copy2,content))){
            printf("%s\n",temp->c->Content);
        }
        temp=temp->next;
    }
}

//==================================================================//

void print_members(listNode* head, Node* node){
    listNode* temp=head;
    while(temp!=NULL){
        if(temp->n!=node && temp->n->type==1 && temp->n->wallet==1){
            printf("\nLinked node:");
            print_info_node(temp->n);
            temp->n->wallet=0;
            printf("Contents posted by %s:\n",get_name(temp->n));
            display_content(temp->n);
        }
        temp=temp->next;
    }
}

// print the 2-hop list
void print_2_hop(listNode* head,Node * node){
    printf("\nPrinting the 2hop members of %s\n",get_name(node));
    listNode* temp=head;
    listNode* group_list=node->kind.individual.group_i;
    listNode* org_list= node->kind.individual.organisation_i;
    while(group_list!=NULL){
        print_members(group_list->n->kind.group.members, node);
        group_list=group_list->next;
    }
    while(org_list!=NULL){
        print_members(org_list->n->kind.organisation.members, node);
        org_list=org_list->next;
    }
    while(temp!=NULL){
        temp->n->wallet=1;
        temp=temp->next;
    }
}

//==================================================================//
// HANDLING USER INPUT ANS ADD A NEW NODE TO THE LINKED LIST OF ALL NODES
listNode* add_node(listNode* all_nodes){
    printf("\nGive the following details to create a new node\n");
    int id,type;
    id=unique_id;
    unique_id++;
    char name[MAX_NAME_LENGTH];
    date d;
    printf("What is the type of node: \n1-Individual\n2-Group\n3-Business\n4-Organisation\n");
    scanf("%d",&type);
    printf("Enter the name: \n");
    getchar();
    fgets(name,MAX_NAME_LENGTH,stdin);
    name[strlen(name)-1]='\0';
    printf("Enter the creation date in order date/month/year: \n");
    // getchar();
    fgets(d.dmy,12,stdin);
    d.dmy[strlen(d.dmy)-1]='\0';
    Node* newnode= create_node(type,id,name,d);
    all_nodes=insertListNode(all_nodes,newnode);
    return all_nodes;
}

// a menu of commands available
void print_actions(){
    printf("=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#\n");
    printf("0- exit from program\n");
    printf("1- add node\n");
    printf("2- delete node\n");
    printf("3- search node by name\n");
    printf("4- search node by type\n");
    printf("5- Search node by birthday\n");
    printf("6- print all nodes\n");
    printf("7- post content\n");
    printf("8- Show all contents posted by a node\n");
    printf("9- To search content\n");
    printf("10- link two nodes\n");
    printf("11- print-1-hop neighbours\n");
    printf("12- print-2-hop neighbours\n");
    
    printf("=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#=#\n");
}

int main(){
    listNode* all_nodes=NULL; // storing all the nodes in this linked list
    content_list* all_contents=NULL;// stroring all the contents in this linked list
    printf("====== Hello !!! Welcome to the social network site ======\n");

    int type_,id_;
    char name_[MAX_NAME_LENGTH];
    Node* node_;
    char date_[12];
    int query;

    while(1){
        printf("\n= Enters the corresponding numbers to perform the required actions\n");
        print_actions();
        scanf("%d",&query);
        switch(query){
            case 0: // exit the program
                printf("Program ended\n");
                return 0;
                
            case 1: // add a node
                all_nodes=add_node(all_nodes);
                break;

            case 2: // delete a node
                printf("\nEnter the id of node which you want to delete:\n");
                scanf("%d",&id_);
                node_= search_node_id(all_nodes,id_);
                if(node_!=NULL)all_nodes=delete_node_info(all_nodes,all_contents,node_);
                else printf("No such node found\n");
                break;

            case 3: //search for a node using its name
                search_node_name(all_nodes);
                break;
            
            case 4:
                search_node_type(all_nodes);
                break;

            case 5:
                search_node_birthday(all_nodes);
                break;

            case 6:
                print_list(all_nodes);
                break;

            case 7:
                printf("\nEnter the id of node for which content is to be posted:\n");
                scanf("%d",&id_);
                node_= search_node_id(all_nodes,id_);
                if(node_!=NULL)all_contents=post(all_contents,node_);
                else printf("No such node found\n");
                break;

            case 8:
                printf("\nEnter the id of node whose contents are to be seen:\n");
                scanf("%d",&id_);
                node_= search_node_id(all_nodes,id_);
                if(node_!=NULL)display_content(node_);
                else printf("No such node found\n");
                break; 

            case 9:
                printf("\nEnter a string to search:\n");
                char str[200];
                getchar();
                fgets(str,200,stdin);
                str[strlen(str)-1]='\0';
                search_content(all_contents,str);
                break;
                
            case 10:
                linking(all_nodes);
                break;

            case 11:
                printf("\nEnter the id of node whose 1 hop neighbours are to be printed:\n");
                scanf("%d",&id_);
                node_= search_node_id(all_nodes,id_);
                if(node_!=NULL)print_1_hop(node_);
                else printf("No such node found\n");
                break;

            case 12:
                printf("\nEnter the id of node whose 2 hop neighbours are to be printed:\n");
                scanf("%d",&id_);
                node_= search_node_id(all_nodes,id_);
                if(node_!=NULL && node_->type==1)print_2_hop(all_nodes,node_);
                else printf("No such node found\n");
                break;

            default:
                printf("Wrong Input!!!\n");
        }
    }
}