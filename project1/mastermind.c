#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	int program = 1;
	char prompt[2];
	char no[2];
	strcpy(no, "n");
	do{

		char option[7];
		strcpy(option, "roygbv");
		char correct[5];
		int game = 10;
		int i;
		int j;
		for (i = 0; i<=3; i++){	
			int value = rand() % (5 - 0 + 1); //high=5 low=0
			correct[i] = option[value];
		}
		//printf("%s\n", correct);
		printf("Welcome to Mastermind!\n");
		printf("Enter a guess: ");
		do{
			char guess[5];
			char temp[5];
			int score = 0;
	        	int score2 = 0;
			strcpy(temp, correct);
			scanf("%s", guess);
			printf("%s\n", guess);
			for (i = 0; i < strlen(guess); i++){
				if (guess[i] == correct [i]){
					score = score + 1;
					temp[i] = 'n';
				}
			}
			for (i = 0; i < strlen(guess); i++){
				for (j = 0; j < strlen(guess); j++){
	        	                if (guess[i] == temp[j]){
						score2 = score2 +1;
						temp[j] = 'x';
					}
        	               }
                	}
			printf("Colors is in the correct position: %d\n", score);
			printf("Colors correct but in wrong position: %d\n", score2);
			if (score == 4){
				printf("You win!\n");
				game = 0;
			}
			else{
				game--;
			}
		} while(game);
		printf("Play again? (y/n)\n");
		scanf("%s", prompt);
		if (strcmp(prompt,no) == 0 ){
			exit(0);
		}
	}while(program);
}
