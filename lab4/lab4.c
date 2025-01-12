#include "lab4.h"
#include <stdio.h>
#include <stdlib.h>
/*

 *
 * Description:
 * This program implements a grade management system for a class, where data is read from an input file
 * and stored in a linked list. Each student has scores in four categories, and the program calculates 
 * cumulative, current, and final grades based on weighted scores. The user can interact with the system 
 * through a menu to display, update, and delete student records.
 */
Node* insert_node(Node *head, struct Data studentData) {
    // Allocate memory for the new node
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Memory allocation failed for new node.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the new node with student data
    newNode->Student = studentData;
    newNode->next = NULL;

    // Insert at the beginning if list is empty or ID is smallest
    if (head == NULL || newNode->Student.student_ID < head->Student.student_ID) {
        newNode->next = head;
        return newNode; // New node is the new head
    }

    // Find insertion point based on ascending ID order
    Node *current = head;
    while (current->next != NULL && current->next->Student.student_ID < newNode->Student.student_ID) {
        current = current->next;
    }

    // Insert new node in the sorted position
    newNode->next = current->next;
    current->next = newNode;

    return head; // Return the head of the list
}

//reads data from the input file, also with calculation of cumulative score
void read_data(const char* filename, Node **head) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open the file for reading");
        exit(EXIT_FAILURE);
    }

    // Skip the header line
    char buffer[1024];
    fgets(buffer, sizeof(buffer), file);

    while (fgets(buffer, sizeof(buffer), file)) {
        struct Data studentData = {0};

        // Ignore empty lines
        if (strcmp(buffer, "\n") == 0) {
            continue;
        }

        // Remove trailing newline character from student name
        buffer[strcspn(buffer, "\n")] = 0;
        strncpy(studentData.student_name, buffer, sizeof(studentData.student_name) - 1);

        // Read student ID; stop if reading fails
        if (fscanf(file, "%d\n", &studentData.student_ID) != 1) {
            break;
        }

        // Read scores for each category
        float* scores[] = {
            &studentData.Cat1.score1, &studentData.Cat1.score2, &studentData.Cat1.score3,
            &studentData.Cat2.score1, &studentData.Cat2.score2, &studentData.Cat2.score3,
            &studentData.Cat3.score1, &studentData.Cat3.score2, &studentData.Cat3.score3,
            &studentData.Cat4.score1, &studentData.Cat4.score2, &studentData.Cat4.score3
        };

        // Loop through and read scores for each category
        for (int i = 0; i < 12; i += 3) {
            if (fscanf(file, "%f %f %f\n", scores[i], scores[i + 1], scores[i + 2]) != 3) {
                break;
            }
        }

        // Compute cumulative score and add student to linked list
        calculate_grade(*head);
        *head = insert_node(*head, studentData);
    }

    fclose(file);
}

void write_data(const Node *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for writing");
        exit(EXIT_FAILURE);
    }

    // Write header with category labels
    fprintf(file, "Quizzes Midterms Homework Final\n");

    const Node *current = head;
    while (current) {
        // Output student name and ID
        fprintf(file, "%s\n%d\n", current->Student.student_name, current->Student.student_ID);

        // Output scores, handling missing scores (-1)
        fprintf(file, "%.2f\t%.2f\t%.2f\n", current->Student.Cat1.score1, current->Student.Cat1.score2, current->Student.Cat1.score3 == -1 ? -1 : current->Student.Cat1.score3);
        fprintf(file, "%.2f\t%.2f\t%.2f\n", current->Student.Cat2.score1, current->Student.Cat2.score2, current->Student.Cat2.score3 == -1 ? -1 : current->Student.Cat2.score3);
        fprintf(file, "%.2f\t%.2f\t%.2f\n", current->Student.Cat3.score1, current->Student.Cat3.score2, current->Student.Cat3.score3 == -1 ? -1 : current->Student.Cat3.score3);
        fprintf(file, "%.2f\t%.2f\t%.2f\n", current->Student.Cat4.score1, current->Student.Cat4.score2, current->Student.Cat4.score3 == -1 ? -1 : current->Student.Cat4.score3);

        current = current->next; // Proceed to the next record
    }

    fclose(file);
}

//free memory
void free_memory(Node *head) {
    while (head != NULL) {
        Node *temp = head;
        head = head->next; // Move to the next node
        free(temp); // Free the current node
    }
}

void calculate_grade(Node *head) {
    float weights[4] = {0.15, 0.30, 0.20, 0.35}; // Weights for each category
    while (head != NULL) {
        float currentGrade = 0;
        float cumulativeScores[4] = {0}; // Temporary storage for cumulative scores
        
        Cat_Grade *categories[4] = {
            &head->Student.Cat1, 
            &head->Student.Cat2, 
            &head->Student.Cat3, 
            &head->Student.Cat4
        };

        for (int i = 0; i < 4; i++) { // Loop through each category
            float sum = 0;
            int validScores = 0;
            float scores[] = {categories[i]->score1, categories[i]->score2, categories[i]->score3};

            for (int j = 0; j < 3; j++) { // Calculate the sum of valid scores in category
                if (scores[j] != -1) {
                    sum += scores[j];
                    validScores++;
                }
            }

            // Calculate cumulative score, use default 100 if no valid scores
            cumulativeScores[i] = (validScores > 0) ? (sum / validScores) : 100;
            categories[i]->Cumulative = cumulativeScores[i]; // Store cumulative score

            // Calculate weighted current grade for each category
            currentGrade += cumulativeScores[i] * weights[i];
        }

        // Update student grade information
        head->Student.Current_Grade = currentGrade;
        head->Student.Final_Grade = -1; // Reset final grade
        head = head->next; // Move to the next node in the list
    }
}

void calculate_final(Node *head) {
    float weights[4] = {0.15, 0.30, 0.20, 0.35}; // Weights for each category

    while (head != NULL) {
        float currentGrade = 0; // Store weighted sum for final grade calculation
        Cat_Grade *categories[4] = {
            &head->Student.Cat1,
            &head->Student.Cat2,
            &head->Student.Cat3,
            &head->Student.Cat4
        };

        for (int i = 0; i < 4; i++) { // Loop through each category
            float sum = 0;
            float scores[] = {categories[i]->score1, categories[i]->score2, categories[i]->score3};

            for (int j = 0; j < 3; j++) { // Sum scores, treating -1 as 0
                sum += (scores[j] != -1) ? scores[j] : 0;
            }

            // Calculate average for category and update cumulative score
            categories[i]->Cumulative = sum / 3;
            currentGrade += categories[i]->Cumulative * weights[i]; // Add weighted score
        }

        // Update student's final grade
        head->Student.Final_Grade = currentGrade;
        head = head->next; // Move to the next node in the list
    }
}

// Function to display the score with specific formatting
void printScore(float value) {
    if (value == -1) {
        // When the score is set to -1, print "n/a" to indicate no score available
        printf("n/a\t");
    } else if (fabsf(value - (int)value) < 0.01) {
        // If the score is close to an integer, print without decimals
        printf("%.0f\t", value);
    } else {
        // If the score has a decimal component, display with two decimal places
        printf("%.2f\t", value);
    }
}

void print_single_student(Node *head, int student_ID) {
    Node *current = head;
    int found = 0; // Track if the student is found

    while (current != NULL) {
        if (current->Student.student_ID == student_ID) {
            // Print student's name and ID
            printf("Student Name: %s\n", current->Student.student_name);
            printf("Student ID: %d\n", current->Student.student_ID);
            
            // Print headers for scores
            printf("            Quizzes  Midterms  Homework  Cumulative\n");

            // Function to print scores for each category
            Cat_Grade *categories[4] = {
                &current->Student.Cat1, 
                &current->Student.Cat2, 
                &current->Student.Cat3, 
                &current->Student.Cat4
            };

            for (int i = 0; i < 4; i++) { // Loop through each category
                printf("Category %d : ", i + 1);
                printScore(categories[i]->score1);
                printScore(categories[i]->score2);
                printScore(categories[i]->score3);
                printScore(categories[i]->Cumulative);
                printf("\n");
            }

            // Print current and final grades
            printf("Current Grade: ");
            printScore(current->Student.Current_Grade);
            printf("\nFinal Grade: ");
            printScore(current->Student.Final_Grade);
            printf("\n\n");

            found = 1; // Mark as found
            break; // Exit loop after finding the student
        }
        current = current->next; // Move to the next node
    }

    if (!found) {
        printf("Student with ID %d not found.\n", student_ID);
    }
}

void print_single_student2(Node *head, const char *lastname) {
    Node *current = head;
    int found = 0; // Track if the student is found

    while (current != NULL) {
        // Locate last name in student name (assumes format: "Last, First")
        char *comma = strchr(current->Student.student_name, ',');
        if (comma != NULL) {
            size_t lastNameLength = comma - current->Student.student_name;
            char extractedLastName[40]; // Buffer for extracted last name

            // Copy last name portion from full name
            strncpy(extractedLastName, current->Student.student_name, lastNameLength);
            extractedLastName[lastNameLength] = '\0';

            // Compare extracted last name with input
            if (strncmp(extractedLastName, lastname, lastNameLength) == 0) {
                // Print student details if last name matches
                printf("Student Name: %s\n", current->Student.student_name);
                printf("Student ID: %d\n", current->Student.student_ID);
                printf("            Quizzes  Midterms  Homework  Cumulative\n");

                // Array of category pointers for streamlined printing
                Cat_Grade *categories[4] = {
                    &current->Student.Cat1,
                    &current->Student.Cat2,
                    &current->Student.Cat3,
                    &current->Student.Cat4
                };

                for (int i = 0; i < 4; i++) { // Print each category's scores
                    printf("Category %d : ", i + 1);
                    printScore(categories[i]->score1);
                    printScore(categories[i]->score2);
                    printScore(categories[i]->score3);
                    printScore(categories[i]->Cumulative);
                    printf("\n");
                }

                // Print current and final grades
                printf("Current Grade: ");
                printScore(current->Student.Current_Grade);
                printf("\nFinal Grade: ");
                printScore(current->Student.Final_Grade);
                printf("\n\n");

                found = 1; // Mark as found
                break; // Exit loop after finding the student
            }
        }
        current = current->next; // Move to the next node
    }

    // If no matching student is found, print a message
    if (!found) {
        printf("Student with last name '%s' not found.\n", lastname);
    }
}

//print all students' information
void print_all_students(Node *head) {
    if (head == NULL) {
        printf("The list is empty.\n");
        return;
    }

    Node *current = head;
    while (current != NULL) {
        // Print student details
        printf("Student Name: %s\n", current->Student.student_name);
        printf("Student ID: %d\n", current->Student.student_ID);
        printf("            Quizzes  Midterms  Homework  Cumulative\n");

        // Array of category pointers for easier looping
        Cat_Grade *categories[4] = {
            &current->Student.Cat1,
            &current->Student.Cat2,
            &current->Student.Cat3,
            &current->Student.Cat4
        };

        for (int i = 0; i < 4; i++) { // Print each category's scores
            printf("Category %d : ", i + 1);
            printScore(categories[i]->score1);
            printScore(categories[i]->score2);
            printScore(categories[i]->score3);
            printScore(categories[i]->Cumulative);
            printf("\n");
        }

        // Print current and final grades
        printf("Current Grade: ");
        printScore(current->Student.Current_Grade);
        printf("\nFinal Grade: ");
        printScore(current->Student.Final_Grade);
        printf("\n\n");

        current = current->next; // Move to the next student
    }
}


//print average score of each category
void print_average(Node* head) {
    float sumScores[4] = {0}; // Cumulative scores for each category
    int validScores[4] = {0};  // Valid score counts for each category

    // Traverse the list and accumulate scores for each category
    Node* current = head;
    while (current != NULL) {
        Cat_Grade categories[] = {current->Student.Cat1, current->Student.Cat2, current->Student.Cat3, current->Student.Cat4};

        for (int i = 0; i < 4; i++) { // Process each category
            if (categories[i].Cumulative != -1) { // Only include valid scores
                sumScores[i] += categories[i].Cumulative;
                validScores[i]++;
            }
        }
        current = current->next; // Move to the next student
    }

    // Print the calculated average for each category
    printf("\nCategory Averages:\n");
    for (int i = 0; i < 4; i++) {
        if (validScores[i] > 0)
            printf("Category %d: %.2f\n", i + 1, sumScores[i] / validScores[i]);
        else
            printf("Category %d: n/a\n", i + 1); // If no valid scores, mark as not available
    }
    printf("\n");
}

//insert a score to an existing student
void insert_score(Node* head, int student_ID, int category, int position, float score) {
    // Traverse the list to find the student by ID
    while (head != NULL) {
        if (head->Student.student_ID == student_ID) {
            Cat_Grade* selectedCategory = NULL;

            // Select the appropriate category
            switch (category) {
                case 1: selectedCategory = &head->Student.Cat1; break;
                case 2: selectedCategory = &head->Student.Cat2; break;
                case 3: selectedCategory = &head->Student.Cat3; break;
                case 4: selectedCategory = &head->Student.Cat4; break;
                default:
                    printf("Invalid category.\n");
                    return;
            }

            // Insert the score at the specified position
            if (selectedCategory != NULL) {
                switch (position) {
                    case 1: selectedCategory->score1 = score; break;
                    case 2: selectedCategory->score2 = score; break;
                    case 3: selectedCategory->score3 = score; break;
                    default:
                        printf("Invalid score position.\n");
                        return;
                }
            }
            return; // Exit after updating the score
        }
        head = head->next; // Move to the next node if ID does not match
    }

    printf("Student with ID %d not found.\n", student_ID); // Message if student ID not found
}

//recalculate cumulative scores based on updated inputs
void recalculate(Node *head, int studentID) {
    while (head != NULL) {
        if (head->Student.student_ID == studentID) {
            // Recalculate cumulative and current grades for the student
            calculate_grade(head);

            // Display updated cumulative scores and current grade
            printf("Student Name: %s  ID: %d\n", head->Student.student_name, head->Student.student_ID);
            printf("Category 1 Cumulative: ");
            printScore(head->Student.Cat1.Cumulative);
            printf("\nCategory 2 Cumulative: ");
            printScore(head->Student.Cat2.Cumulative);
            printf("\nCategory 3 Cumulative: ");
            printScore(head->Student.Cat3.Cumulative);
            printf("\nCategory 4 Cumulative: ");
            printScore(head->Student.Cat4.Cumulative);
            printf("\nCurrent Grade: %.2f\n\n", head->Student.Current_Grade);

            return; // Exit once student is found and processed
        }
        head = head->next; // Move to the next node if ID does not match
    }

    printf("Student with ID %d not found.\n", studentID); // Message if student ID not found
}

//recalculate scores for all students in the list
void recalculate_all(Node* head) {
    if (head == NULL) {
        printf("No students found.\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        // Recalculate grades for the current student
        calculate_grade(current);

        // Display updated cumulative scores and current grade
        printf("Student Name: %s  ID: %d\n", current->Student.student_name, current->Student.student_ID);
        printf("Category 1 Cumulative: ");
        printScore(current->Student.Cat1.Cumulative);
        printf("\nCategory 2 Cumulative: ");
        printScore(current->Student.Cat2.Cumulative);
        printf("\nCategory 3 Cumulative: ");
        printScore(current->Student.Cat3.Cumulative);
        printf("\nCategory 4 Cumulative: ");
        printScore(current->Student.Cat4.Cumulative);
        printf("\nCurrent Grade: %.2f\n\n", current->Student.Current_Grade);

        current = current->next; // Move to the next student
    }
}

//recalculate the final score for students
void recalculate_final(Node* head) {
    if (head == NULL) {
        printf("No students found.\n");
        return;
    }

    Node* current = head;
    while (current != NULL) {
        // Recalculate final grade for the current student
        calculate_final(current);

        // Display updated cumulative scores and final grade
        printf("Student Name: %s  ID: %d\n", current->Student.student_name, current->Student.student_ID);
        printf("Category 1 Cumulative: ");
        printScore(current->Student.Cat1.Cumulative);
        printf("\nCategory 2 Cumulative: ");
        printScore(current->Student.Cat2.Cumulative);
        printf("\nCategory 3 Cumulative: ");
        printScore(current->Student.Cat3.Cumulative);
        printf("\nCategory 4 Cumulative: ");
        printScore(current->Student.Cat4.Cumulative);
        printf("\nFinal Grade: %.2f\n\n", current->Student.Final_Grade);

        current = current->next; // Move to the next student
    }
}

//add a new student with grades to the list
void add_new_student(Node** head) {
    Data newStudent;
    
    // Input student details
    printf("Please enter the student's name: ");
    scanf(" %[^\n]%*c", newStudent.student_name); // Reads until newline is encountered

    printf("Pleace enter the student's ID: ");
    scanf("%d", &newStudent.student_ID);

    printf("Enter scores for Category 1 (separated by spaces): ");
    scanf("%f %f %f", &newStudent.Cat1.score1, &newStudent.Cat1.score2, &newStudent.Cat1.score3);

    printf("Enter scores for Category 2 (separated by spaces): ");
    scanf("%f %f %f", &newStudent.Cat2.score1, &newStudent.Cat2.score2, &newStudent.Cat2.score3);

    printf("Enter scores for Category 3 (separated by spaces): ");
    scanf("%f %f %f", &newStudent.Cat3.score1, &newStudent.Cat3.score2, &newStudent.Cat3.score3);

    printf("Enter scores for Category 4 (separated by spaces): ");
    scanf("%f %f %f", &newStudent.Cat4.score1, &newStudent.Cat4.score2, &newStudent.Cat4.score3);

    // Allocate memory for the new node
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Unable to allocate memory for the new student.\n");
        return;
    }

    // Assign the student data to the new node
    newNode->Student = newStudent;
    newNode->next = *head;
    *head = newNode; // Insert the new node at the beginning of the list

    // Calculate the grade for the new student
    calculate_grade(*head);

    // Confirm addition
    printf("New student added successfully: %s, ID: %d\n", newStudent.student_name, newStudent.student_ID);
    printf("Student list now starts with: %s, ID: %d\n", (*head)->Student.student_name, (*head)->Student.student_ID);
}

//deletes a student 
void delete_student(Node** head) {
    if (*head == NULL) {
        printf("No students found in the list. Deletion not possible.\n");
        return;
    }

    int studentID;
    printf("Please etner the student's ID to delete: ");
    scanf("%d", &studentID);

    Node *temp = *head, *prev = NULL;

    // Traverse to find the student by ID
    while (temp != NULL && temp->Student.student_ID != studentID) {
        prev = temp;
        temp = temp->next;
    }

    // If student with the given ID is not found
    if (temp == NULL) {
        printf("The student with ID %d can not found.\n", studentID);
        return;
    }

    // If the student to be deleted is at the head
    if (temp == *head) {
        *head = temp->next; // Update head
    } else {
        prev->next = temp->next; // Unlink the node from the list
    }

    free(temp); // Free memory for the node being deleted
    printf("Student with ID %d has been deleted.\n", studentID);
}

void option1(Node *head) {
    int student_ID;
    printf("Please enter the Student ID: ");
    scanf("%d", &student_ID);
    print_single_student(head, student_ID);
    printf("Option 1 executed.\n");
}

void option2(Node *head) {
    char lastname[40]; // Assuming last names won't exceed 40 characters
    printf("Please enter this Student's Last Name: ");
    scanf("%39s", lastname);
    print_single_student2(head, lastname);
    printf("Option 2 executed.\n");

}

void option3(Node *head) {
    print_all_students(head);
    print_average(head);
    printf("Option 3 executed.\n");
}

void option4(Node *head) {
int student_ID;
    printf("Please enter the Student ID: ");
    scanf("%d", &student_ID);
    recalculate(head, student_ID);
    printf("Option 4 executed.\n");
}

void option5(Node *head) {
    recalculate_all(head);
    printf("Option 5 executed.\n");
}
void option6(Node *head) {
    //prompt for user input
    int student_ID;
    printf("Please enter the Student ID: ");
    scanf("%d", &student_ID);

    int category;
    printf("Enter category: (1 ~ 4) ");
    scanf("%d", &category);

    int position;
    printf("Please enter the category: (quiz 1, midterm 2, homework 3) ");
    scanf("%d", &position);

    float newScore;
    printf("Please enter the new score ");
    scanf("%f", &newScore);

    insert_score(head, student_ID, category, position, newScore);
    printf("Option 6 executed.\n");
}

void option7(Node *head) {
    recalculate_final(head);
    printf("Option 7 executed.\n");
}

void option8(Node *head) {
	add_new_student(&head);
        printf("Option 8 executed.\n");
}

void option9(Node *head) {
    delete_student(&head);
    printf("Option 9 executed.\n");
	
}

void option10(Node *head) {
    printf("Option 10 executed.\n");
}

void print_menu() {
    printf("\n");
    printf("Select an option from the menu:\n");
    printf("1. Display a single student's record by ID.\n");
    printf("2. Display a single student's record by last name.\n");
    printf("3. Show all student records.\n");
    printf("4. Update all grades for a specific student.\n");
    printf("5. Update grades for all students.\n");
    printf("6. Add a new score to a student’s record.\n");
    printf("7. Calculate final grades for every student.\n");
    printf("8. Add a new student to the class list.\n");
    printf("9. Remove a student record.\n");
    printf("10. Quit the program.\n");
}

//execute options based on user's input
void execute_option(int choice, Node **head) {
    // Array of function pointers for all options except delete and add, since those need Node** as input
    void (*option_functions[])(Node *) = {option1, option2, option3, option4, option5, option6, option7};
    int index;

    switch (choice) {
        case 8:
            add_new_student(head); // Add a new student (requires modifying head)
            break;
        case 9:
            delete_student(head); // Delete a student (requires modifying head)
            break;
        default:
            index = choice - 1;
            if (index >= 0 && index < 7) {
                (*option_functions[index])(*head); // Execute options 1 to 7
            } else if (choice != 10) {
                printf("Option not recognized. Please enter a number between 1 and 10.\n");
            }
            break;
    }
}
