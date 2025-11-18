///////////////////////////////////////////////////////////////////////////////////////
//Blackjack SE300 Project
//Authors: Leilani, Alex, Javier, Joss
///////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct Manager{ // Leilani's note: I don't remember at all how I'm supposed to
                        // do this properly. Feel free to delete it if it's nonsense.
}Manager;

void mainMenu();
void game();
void tutorial();

int main(){
int balance;
  Manager DealerManager;
  Manager TutorialManager;
  Manager GameManager;
  Manager BettingManager;
  printf("***********GAME START**********\n");

  mainMenu();

  return 0;
}

void mainMenu() {  // Alex's note: probably a much better way to do this,
    int input;     // just wanted to get a start on how we could do it.
    int loop=1;
    int check=1;
    char term;
    printf("Main Menu...\n");
    printf("Enter 1 to start new game\n");
    printf("Enter 2 to enter the tutorial\n");
    printf("Enter 3 to exit game\n");
    printf("Input: ");
    while(loop=1){
        while(check==1) {
//            printf("\nLoop\n");
            if(scanf("%d%c", &input, &term) != 2 || term != '\n') {  // Input buffer
                printf("\nInvalid Input\n\nInput: ");
                getchar();
                check=1;
            }
            else {
                check=0;
            }
        }
//        printf("Exit Loop\n");
        switch(input) {
            case 1:
                printf("\nStarting New Game...\n");
                loop=0;
                game();
                return;
            case 2:
                printf("\nStarting Tutorial...\n");
                loop=0;
                tutorial();
                return;
            case 3:
                printf("\nExiting Game...\n");
                loop=0;
                return;
            default:
                printf("\nInvalid Input...\n\nPlease Re-Enter Input: ");
                while((getchar())!='\n');
                loop=1;
                break;
        }
    }
}

void game() {
    printf("\nNew Game\n");
    return;
}

void tutorial() {
    printf("\nTutorial\n");
    return;
}
