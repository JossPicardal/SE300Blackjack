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
     printf("-----------------------------------------------\n");
    printf("                Rules/Tutorial                 \n");
    printf("-----------------------------------------------\n\n");

    printf("Objective: Beat the dealer by having your hands total value closer to 21 without busting (going over 21)\n");

    printf("Terminology: \n");
    printf(" Hit- Get another card from dealer\n");
    printf(" Bust- Card amount is greater than 21\n");
    printf(" Hold- Skip your turn\n");
    printf(" Blackjack- First 2 cards dealt equal 21\n");
    printf(" Double Down- Doubles current bet and deals a single card, can only be done after initial 2 cards are dealt\n");
    printf(" Split- Split current hand into 2 hands; an equal bet to the starting bet must be on each hand\n");
    printf(" Hand- Current set of cards a player has \n");
    printf(" Dealer- Computer opponent that deals cards\n\n");

    printf("Card Values: \n");
    printf(" Number Cards (2-10) = Face value: \n");
    printf(" Face Cards (J, Q, K) = 10 \n");
    printf(" Ace (A) = 11 or 1 when hand total would be higher than 21 \n");

    printf("Betting Rules: \n");

    printf("American Blackjack Rules: \n");
    printf("- American Blackjack payouts for Blackjack are 3:2 or 6:5\n");
    printf("- The hole card allows the dealer to peek for Blackjack on Ace up-cards and 10s\n");
    printf("- The dealer must hold on a soft 17\n");
    printf("- Players can double down on any hand sum\n");
    printf("- Players can split hands up to three times\n");
    printf("- Players can double down after a split\n");
    printf("- Aces can only be split once\n");
    printf("- 10-value cards can be split, even if they are different\n");
    printf("- Six or eight decks of cards are used\n");
    printf("- Players can surrender late\n\n");

    printf("How to Play: \n");
    printf(" 1. Place your bet\n");
    printf(" 2. You and dealer each receive 2 cards\n");
    printf(" 3. Dealer shows 1 card face up\n");
    printf(" 4. Choose your action: Hit, Hold, Double Down, or Split\n");
    printf(" 5. Dealer reveals hidden card and plays\n");
    printf(" 6. Winner determined by the closest to 21 without busting wins\n\n");

  
    return;
}


