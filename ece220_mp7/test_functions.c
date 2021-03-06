#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//user file(s)
#include "sudoku.h"

//initialize test valid functions
int test_row(const int val, const int i, const int sudoku[9][9], int gold_response);
int test_col(const int val, const int j, const int sudoku[9][9], int gold_response);
int test_sect(const int val, const int i, const int j, const int sudoku[9][9], int gold_response);

int test_sol(const char fpath, const int sudoku[9][9]);



int file_count = 1;

/* check if is_val_in_row function is correct
 * if correct response, return 0
 * if incorrect, return 1
 */

int test_row(const int val, const int i, const int sudoku[9][9], int gold_response){
	int check = is_val_in_row(val, i, sudoku);
	if(check == gold_response)
	  return 0;
	
	
	printf("Error in is_val_in_row function\n");
	printf("Row: %d\n", i);
	printf("Number: %d\n", val);
	printf("Expected value: %d\n", gold_response);
	printf("Your value: %d\n", check);
	return 1;
}


/* check if is_col_in_row function is correct
 * if correct response, return 0
 * if incorrect, return 1
 */
int test_col(const int val, const int j, const int sudoku[9][9], int gold_response){
	int check = is_val_in_col(val, j, sudoku);
	if(check == gold_response)
	  return 0;
	
	
	printf("Error in is_val_in_col function\n");
	printf("Column: %d\n\n", j);
	printf("Number: %d\n", val);
	printf("Expected value: %d\n", gold_response);
	printf("Your value: %d\n", check);
	return 1;
}

/* check if is_col_in_row function is correct
 * if correct response, return 0
 * if incorrect, return 1
 */

int test_sect(const int val, const int sect, const int sudoku[9][9], int gold_response){
	int i = (sect/3)*3;
	int j = (sect%3)*3;

	int check = is_val_in_3x3_zone(val, i, j, sudoku);
	if(check == gold_response)
	  return 0;
	
	printf("Error in is_val_in_3x3_zone function\n");
	printf("Grid Number: %d\n\n", sect);
	printf("Expected value: %d\n", gold_response);
	printf("Your value: %d\n", check);
	return 1;
}

// function to test solve_sudoku function
// if return 1, output incorrect
// if return 0, output correct
int test_sol(char fpath[], int sudoku[9][9], int sudoku_gold[9][9]){
	int i;
	int j;
	parse_sudoku(fpath, sudoku_gold);
	solve_sudoku(sudoku);

	for(i=0; i<9; i++){
		for(j=0; j<9;j++){
			if(sudoku[i][j] != sudoku_gold[i][j]){
			  printf("Board incorrect at row %d, column %d\n", i, j);
			  return 1;
			}
		}
	}
	return 0;
	
	

}




int main(int argc, char const *argv[]){
	//pulling test files
	char filename[10];
	sprintf(filename, "board%d", file_count);
	char buffer[100];
	char board_dir[] = "./test_images/";
	FILE* fp;
	strcpy(buffer, board_dir);
	strcat(buffer, filename);
	strcat(buffer, ".in");

	fp = fopen(buffer, "r");
	if(fp = NULL){
	  printf("Error opening input file.\nTerminating Program");
	  return 1;
	}		

	// Iteration over each file
	int correct = 0;

	do{

		//get board content
		int sudoku[9][9];
	
		//using mp7's parse function to create board
		parse_sudoku(buffer, sudoku);
		print_sudoku(sudoku);
		


		// Iterate through valid check functions
		strcpy(buffer, board_dir);
		strcat(buffer, filename);
		strcat(buffer, ".series");
		
	
		//checking valid row function	
		FILE* row_ptr;
		row_ptr = fopen(buffer, "r");
		

		printf("Testing is_val_in_row against %s\n", buffer);
		if(row_ptr){
			char garbage[100];
			int val, row, proper_out;
		

			fgets(garbage, sizeof(garbage), row_ptr);
			while(fscanf(row_ptr, "%d %d %d\n", &row, &val, &proper_out) != 0){
			  int row_check = test_row(val, row, sudoku, proper_out);
			  if(row_check){
			    printf("is_val_in_row failed test %d.\nTerminating Program", file_count);
			    return 1;
			  }
			}
			printf("is_val_in_row passed test %d\n", file_count);


		}else{
		  printf("Error opening %s.\nTerminating Program.", buffer);
		  return 1;
		}
		
		
		//checking valid column function
		FILE* col_ptr;
		col_ptr = row_ptr;	
			
		printf("Testing is_val_in_col against %s\n", buffer);
		if(col_ptr){
			char garbage[100];
			int val, col, proper_out;


			fgets(garbage, sizeof(garbage), col_ptr);
			while(fscanf(col_ptr, "%d %d %d\n", &col, &val, &proper_out) != 0){
			int col_check = test_col(val, col, sudoku, proper_out);
			
			if(col_check){
			    printf("is_val_in_col failed test %d.\nTerminating Program", file_count);
			    return 1;
			  }
			}
			printf("is_val_in_col passed test %d\n", file_count);



		}else{
		  printf("Error opening %s.\nTerminating Program.", buffer);
		  return 1;
		}
	

		//checking valid sector function
		FILE* sect_ptr;
		sect_ptr = col_ptr;
		

		printf("Testing is_val_in_3x3_zone against %s\n", buffer);
		if(sect_ptr){
			char garbage[100];
			int val, sect, proper_out;


			fgets(garbage, sizeof(garbage), sect_ptr);
			while(fscanf(sect_ptr, "%d %d %d\n", &sect, &val, &proper_out) != EOF){
			int sect_check; test_sect(val, sect, sudoku, proper_out);
			if(sect_check){
			    printf("is_val_in_3x3_zone failed test %d.\nTerminating Program.", file_count);
			    return 1;
			  }
			}
			printf("is_val_in_3x3_zone passed test %d\n", file_count);

		}else{
		  printf("Error opening %s.\nTerminating Program.", buffer);
		  return 1;
		}
		
		fclose(sect_ptr);
		
		//check with solution board
		strcpy(buffer, board_dir);
		strcat(buffer, filename);
		strcat(buffer, ".sol");
		
		int sudoku_sol[9][9];
		int sol_check = test_sol(buffer, sudoku, sudoku_sol);
		
		if(sol_check){
		  printf("solve_sudoku function failed\n");
		  printf("Solution board:\n");
		  print_sudoku(sudoku_sol);
		  printf("Your Board:\n");
		  print_sudoku(sudoku);
		  return 1;
		}
		else{
		  printf("solve_sudoku function passed test %d\n", file_count);
		  printf("Solution:\n");
		  print_sudoku(sudoku);
		}
			
		
		//update conditions
		correct++;
		file_count++;
		 
		//i[date fo;e pointer to check if NULL
		sprintf(filename, "board%d", file_count);
		strcpy(buffer, board_dir);
		strcat(buffer, filename);
		strcat(buffer, ".in");
		fp = fopen(buffer, "r")

		;	
	}while(fp != NULL);
	
	
	printf("is_val_in_zone functions valid for all cases\n");
	printf("solve_sudoku function valid for all cases\n");

	return 0;
}		
