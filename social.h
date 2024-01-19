/******************************************************************************
 * File: social.h
 * Author: Deetosh Kumar Kuila
 * Date: 15-01-2024
 * Description: Defining the required structures/functions for a simple social network system
 ******************************************************************************/

/* The program implements a simple social network system, managing nodes representing
   individuals, groups, businesses, and organizations. It provides functionalities such as
   adding, deleting, and searching nodes, posting content, linking nodes, and printing neighbors
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_NAME_LENGTH 51 // Maximum length allowed for any name (50 characters)
#define MAX_CONTENT_LENGTH 201 // Maximum length allowed for any content in a post (200 characters)


//==================================================================//
// Structs for date, content, coordinate
typedef struct { // Contains a string in the form DD/MM/YYYY representing a date
    char dmy[12];
}date;

typedef struct { // Contains a string representing the content of a post.
    char Content[MAX_CONTENT_LENGTH];
}content;

typedef struct { // Contains the location of a point with x and y coordinates. 
    double x; 
    double y;
}coordinates;

// Forward declaration
struct node_; 
typedef struct node_ Node;

//==================================================================//
// Linked list node 
typedef struct listnode{ //  Structure for a node in a linked list, containing a pointer to a Node and a reference to the next node.
    Node* n;
    struct listnode* next;
}listNode ;



//==================================================================//
// Linked list for content
typedef struct ContentListNode{ // Structure for a node in a linked list for content, containing a pointer to a content and a reference to the next content node.
    content* c;
    struct ContentListNode* next;
}content_list ;

//==================================================================//
//DEFINING DIFF TYPES OF NODES
typedef struct Individual_{
    int id;
    char name[MAX_NAME_LENGTH];
    date creation_date;
    content_list* posts;
    date birthday;    
    listNode* group_i;  // list of groups where individual is present
    listNode* organisation_i; // list of organisations where individual is present
    listNode* business_i; // list of businesses where individual is owner/customer
}Individual;

typedef struct Business_{
    int id;
    char name[MAX_NAME_LENGTH];
    date creation_date;
    content_list* posts;
    coordinates location;
    listNode* owners;
    listNode* customers;
    listNode* group_b; // list of groups whose menber is this business
}Business;

typedef struct Organisation_{
    int id;
    char name[MAX_NAME_LENGTH];
    date creation_date;
    content_list* posts;
    coordinates location;
    listNode* members;
}Organisation;

typedef struct Group_{
    int id;
    char name[MAX_NAME_LENGTH];
    date creation_date;
    content_list* posts;
    listNode* members;
}Group;

struct node_{ // Structure representing a node in the social network.
    int type; // 1-individual, 2- group, 3- business, 4-organization
    union{
        Individual individual;
        Business business;
        Group group;
        Organisation organisation;
    }kind;
    int wallet;
};

//==================================================================//
// prototypes of the functions

// LINKED LIST FUNCTIONS'
listNode* createListNode(Node* node);
// Creates a new linked list node with the provided Node pointer
listNode* insertListNode(listNode* head,Node* node);
// Inserts a new node at the beginning of the linked list
listNode* eraseListNode(listNode* head, Node* node);
// Removes a node from the linked list
void deleteLinkedList(listNode* head);
// Deletes the entire linked list and frees the memory

// CONTENTS LINKED LIST FUNCTIONS
content_list* create_node_content(content* c);
// Creates a new content linked list node with the provided content pointer.
content_list* insert_content(content_list* head,content* cont);
// Inserts a new content node at the beginning of the linked list
content_list* erase_content(content_list* head,content* cont);
// Removes a content node from the linked list
void delete_content_list(content_list* head, content_list* all_contents);
// Deletes the entire content linked list and frees the memory

// [1] CREATE AND DELETE NODES
Node* create_node(int type,int id, char* name, date creation_date);
// Creates a new node with the specified parameters
listNode* delete_node_info(listNode* all_nodes, content_list* all_contents, Node* node);
// Deletes a node and erases it from all relevant places

// SOME HELPING FUNCTIONS
char* get_name(Node* node);
// Gets the name of a node as a string 
int get_id(Node* node);
// Gets the name of a node as a string 
void print_info_node(Node* node);
// Prints necessary information of a node

// [2] FUNCTIONS FOR SEARCHING A NODE BASED ON ITS CHARACTERISTICS
void search_node_name(listNode* head);
// Prints information of nodes with a given name
Node* search_node_id(listNode* head,int id);
// Returns a node with the specified ID
void search_node_type(listNode* head);
// Prints information of nodes with a given type
void search_node_birthday(listNode* head);
// Prints information of individual nodes with a given birthday

// [3] PRINT ALL 1-HOP LINKED NODES
void print_linked(listNode* head);
// Prints the names of nodes in a linked list
void print_1_hop(Node* node);
// Prints nodes linked to a given node

// [4] CREATE AND POST CONTENT BY A NODE
content* search_exact_content(content_list* head, char* content);
// Searches for exact content to prevent duplication
content* create_content();
// Creates a new content from user input
content_list* post(content_list* head,Node* node);
// Posts content by adding it to the linked list of all contents
void display_content(Node* node);
// Prints all posts of a given node
void print_all_contents(content_list* head);
// Prints all contents in a linked list

// [5] SEARCH FOR CONTENTS
char* convertToLowercase(char*  temp,char *str);
// Converts a given string to lowercase so that the search results are good
void search_content(content_list* head, char* content);
// Searches for a given string in any content posted on the site (a substring search)


// LINKING THE NODES
void print_list(listNode* head);
// Prints all nodes in a linked list
void add_indi_members(Node* indi, Node* node);
// Creates a link between an individual and a group/organisation
void add_biss_members(Node* biss, Node* grp);
// Creates a link between a business and a group
void add_owners_biss(Node* indi, Node* biss);
// Creates a link between an individual and a business as an owner
void add_customers_biss(Node* indi, Node* biss);
// Creates a link between an individual and a business as a customer
void linking(listNode* all_nodes);
// Establishes links between nodes based on user input and type

// [6] Display all content posted by individuals linked to a given individual
void print_members(listNode* head, Node* node);
// Prints all content posted by individuals linked to a given individual
void print_2_hop(listNode* head,Node * node);
// Prints nodes linked to a given node and their respective content

// HANDLING USER INPUTS AND ADDING A NEW NODE
listNode* add_node(listNode* all_nodes);
