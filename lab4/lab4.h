#ifndef LAB4_H
#define LAB4_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>  

typedef struct Cat_Grade {
    float score1;
    float score2;
    float score3;
    float Cumulative;
} Cat_Grade;

typedef struct Data {
    char student_name[40];
    int student_ID;
    Cat_Grade Cat1;
    Cat_Grade Cat2;
    Cat_Grade Cat3;
    Cat_Grade Cat4;
    float Current_Grade;
    float Final_Grade;
} Data;

typedef struct Node {
    Data Student;
    struct Node *next;
} Node;

// Node management and memory operations
Node* malloc_node(Data studentData);
Node* insert_node(Node *head, Data studentData);
Node* delete_node(Node *head, int student_ID);
void free_memory(Node *head);

// Data I/O functions
void write_data(const Node *head, const char* filename);
void read_data(const char* filename, Node **head);

// Student grade calculation and score operations
void calculate_grade(Node *head);
void recalulate(Node *head, int studentID);
void recalulate_all(Node *head);
void calculate_final(Node *head);
void insert_score(Node *head, int student_ID, int category, int position, float score);

// Record printing functions
void print_student_records(Node *head);
void print_single_student(Node *head, int student_ID);
void print_single_student2(Node *head, const char *lastname);
void print_all_students(Node *head);
void print_average(Node *head);

// Student management functions
void add_new_student(Node** head);
void delete_student(Node** head);

// User menu and option functions
void print_menu();
void execute_option(int choice, Node **head);  
void option1(Node *head);
void option2(Node *head);
void option3(Node *head);
void option4(Node *head);
void option5(Node *head);
void option6(Node *head);
void option7(Node *head);
void option8(Node *head);
void option9(Node *head);
void option10(Node *head);

#endif

