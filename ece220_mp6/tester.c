// Standard libraries in C
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

// File user's file
#include "updateBoard.h"

// Function prototypes
int test_neighbors(int* board, int row, int column, int numRows, int numCols, int golden_response, char* spawn_file);
int test_update(int* golden_board, int* user_board, int numRows, int numCols, char* spawn_file);

// Struct used to create linked list to store all the filenames
typedef struct filename{
    char name[20];
    struct filename* next;
}filename_t;

/* Function to check whether the user's check neighbor function is working correctly
 * @return: 0: if the user's function matches the proper number of neighbors
 *          1: if the user's function does not match the proper number of neighbors
 */
int test_neighbors(int* board, int row, int column, int numRows, int numCols, int golden_response, char* spawn_file){
    int result = countLiveNeighbor(board, numRows, numCols, row, column);

    if(result == golden_response)
        return 0;


    printf("\nError in countLiveNeighbor()\nInput File: ");
    int c;
    printf("%c", spawn_file[0]);
    for (c = 1; spawn_file[c] != '.'; c++)
        printf("%c", spawn_file[c]);
    printf("%s\n", ".in");
    
    printf("\n");
    int rr, cc;
    for(rr = 0; rr < numRows; rr++){
        for(cc = 0; cc < numCols; cc++){
           printf("%d ", board[numCols*rr+cc]);
        }
        printf("\n");
    }
    printf("\n");

    printf("Row: %d\n", row);
    printf("Column: %d\n\n", column);
    printf("Expected Return: %d\n", golden_response);
    printf("Your Return: %d\n\n", result);

    return 1;
}
/* Function to check whether the user's updateBoard function is working properly
 * @return: 0: if the user's function matches the golden board
 *          1: if the user's function does not match the golden board
 */
int test_update(int* golden_board, int* user_board, int numRows, int numCols, char* spawn_file){
    // Run the user function
    // Compare the user's board to that of the actual
    return 0;
}
 
int main(int argc, char const *argv[]){

    // Pull all the file names and store into an linked list
    FILE* fp;
    fp = fopen("files.txt", "r");

    filename_t* top_ptr = (filename_t*) malloc(sizeof(filename_t));
    char buffer[50];
    if(fp){
        fscanf(fp, "%s", top_ptr->name);
        top_ptr->next = NULL;                             

        while(fscanf(fp, "%s", buffer) != EOF){
            filename_t* new_top = (filename_t*) malloc(sizeof(filename_t));
            strcpy(new_top->name, buffer);
            new_top->next = top_ptr;
            top_ptr = new_top;
        }
        fclose(fp);
    }else{
        printf("Error opening the input file.\nTerminating the program.");
        return 1;
    }

    // Iteration over the linked list
    if(top_ptr == NULL){
        printf("Error in parsing the linked list in test script.\nTerminating the program");
        return 1;
    }else{
        int correct, total = 0;
        do{
            char* filename = top_ptr->name;
            strcpy(buffer, "./txt_boards/");
            strcat(buffer, filename);
            strcat(buffer, ".in");

            // Open the input file and read in the intial board
            FILE* fp;
            fp = fopen(buffer, "r");

            // Grab metadata and board contents
            int rows, cols;
            if(fp){
                fscanf(fp, "%d", &rows);
                fscanf(fp, "%d", &cols);
                // Decrement to account for a bug in neighbors storage
                rows--;
                cols--;

                int* game_board = malloc(rows*cols*sizeof(int));
                int* golden_board = malloc(rows*cols*sizeof(int));
                int i, j, temp_b;
                for(i = 0; i < rows; i++){
                    for(j = 0; j < cols; j++){
                        fscanf(fp, "%d", &temp_b);
                        game_board[cols*i+j] = temp_b;
                        golden_board[cols*i+j] = temp_b;
                    }
                }
                // Iterate through and test the neighbors function
                strcpy(buffer, "./txt_boards/");
                strcat(buffer, filename);
                strcat(buffer, ".neighbors");
                FILE* neighbor_ptr;
                neighbor_ptr = fopen(buffer, "r");

                printf("Testing neighbors against %s\n", buffer);
                if(neighbor_ptr){
                    int r_in, c_in, proper_out;
                    total++;
                    while(fscanf(neighbor_ptr, "%d, %d, %d\n", &r_in, &c_in, &proper_out) != EOF){
                        int test_failure = test_neighbors(game_board, r_in, c_in, rows, cols, proper_out, buffer);
                        if(test_failure)
                            return 0; 
                    }
                    correct++;
                }else{
                    printf("Error opening %s.\nTerminating the program.", buffer);
                    return 1;
                }
                printf("Test score: (%d/%d)\n\n", correct, total);
                fclose(neighbor_ptr);

                // Iterate through and test the board as it changes
                strcpy(buffer, "./txt_boards/");
                strcat(buffer, filename);
                strcat(buffer, ".changes");
                FILE* changes_ptr;
                changes_ptr = fopen(buffer, "r");
                if(changes_ptr){
                    char line_buffer[50];
                    // Code to iterate over the board changes, updating the golden board
                    // Code to check the stayAlive function
                }else{
                    printf("Error opening %s.\nTerminating the program.", buffer);
                    return 1;
                }
                fclose(changes_ptr);

                free(game_board);
                free(golden_board);
            }else{
                printf("Error opening %s.\nTerminating the program.", buffer);
                return 1;
            }
            fclose(fp);

            free(top_ptr);
            top_ptr = top_ptr->next;
        }while(top_ptr != NULL);
        free(top_ptr);
    }
    return 0;
}