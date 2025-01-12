#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab.h"  // Include header file to access all functions and structures defined in lab.h


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_filename output_filename\n", argv[0]);
        return 1;
    }

    Node *student_list = NULL; // Pointer to the head of the linked list, renamed to student_list for clarity
    read_data(argv[1], &student_list); // Read data from the input file

    int menu_choice;
    do {
        print_menu(); // Display menu options

        if (scanf("%d", &menu_choice) != 1) { // Check if the input is valid
            printf("Invalid input. Please enter a number between 1 and 10.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }

        // Execute the chosen option, pass the address of the head pointer
        execute_option(menu_choice, &student_list);

    } while (menu_choice != 10); // Assuming option 10 is to exit

    // Write data to file and free memory before exiting
    write_data(student_list, argv[2]);
    free_memory(student_list);

    return 0;
}

