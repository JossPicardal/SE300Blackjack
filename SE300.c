///////////////////////////////////////////////////////////////////////////////////////
//Blackjack SE300 Project
//Authors: Leilani, Alex, Javier, Joss
///////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct cards{
  char *rank;
  char *suite;
  int count;
};

typedef struct Manager{ // Leilani's note: I don't remember at all how I'm supposed to
                        // do this properly. Feel free to delete it if it's nonsense.
}Manager;

void mainMenu();
void game();
void tutorial();
void cardManager();

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
    cardManager();
    return;
}

void cardManager() {
  // Initializing Deck
  int deck[52];
  int aval[52];
  int dCount=0;
  const char *suites[]={"Spades","Hearts","Clubs","Diamonds"};
  const char *ranks[]={"Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten","Jack","Queen","King"};
  struct cards card[52];
  for(int i=0; i<52; i++){ // populating the deck
    deck[i]=i+1;
    aval[i]=1;

//    printf("Card %d, Card Value: %d\n",i+1,deck[i]);
  }
  for(int j=0;j<4;j++){ // assigning rank and suit to cards
    for(int jj=0;jj<13;jj++){
      card[dCount].count=jj+1;
      card[dCount].rank=ranks[jj];
      card[dCount].suite=suites[j];
//      printf("\nCard: %d; %s of %s",dCount+1,card[dCount].rank,card[dCount].suite);
      dCount++;
    }
  }

  // Dealing Cards
  int hCount=5;
  int hand[hCount];
  int min=0;
  int max=52;
  int exit=0;
  int loop=0;
  struct cards handCard[hCount];
  srand(time(NULL));
  for(int ii=0; ii<hCount; ii++){ // dealing ii amount of cards to hand
    int i=0;
    exit=0;
    hand[ii]=rand()%(max-min+1)+min;
    while(exit!=1){
      if(hand[ii]==deck[i]){
//        printf("\nCard found\n");
        exit=1;
        if(aval[i]==1){
          aval[i]=0;
//          printf("\nCard Avalible\n");
          loop=0;
          handCard[ii].rank=card[hand[ii]-1].rank;
          handCard[ii].suite=card[hand[ii]-1].suite;
        }
        else if(aval[i]==0){
//          printf("\nCard Unavalible\n");
          loop=1;
        }
      }
      else{
//        printf("\nCard not found %d\n",i);
        i++;
      }
    }
    if(loop==1){
      ii--;
    }
    else {
          printf("\nCard: %d, Card Value: %d",ii+1,hand[ii]);
          printf(", Card Rank: %s of %s\n",handCard[ii].rank,handCard[ii].suite);
        }
  }

}
void tutorial() {
    printf("\nTutorial\n");
    return;
}
