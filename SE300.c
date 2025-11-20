///////////////////////////////////////////////////////////////////////////////////////
//Blackjack SE300 Project
//Authors: Leilani, Alex, Javier, Joss
///////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct cards{
  const char *rank;
  const char *suite;
  int count;
};


void mainMenu();
void game();
void tutorial();
void bettingManager();
struct cards* cardManager(int hCount);
void printCards(struct cards card[], int count);
void playerTurn(struct cards* hand, int* size);
int handValue(struct cards* hand, int size);
void dealerTurn(struct cards card[], int* count);

int main(){
  printf("***********GAME START**********\n");

  mainMenu();

  return 0;
}
/*---------------------------------------------------Main Menu-------------------------------------------------------*/
void mainMenu() {  // Alex's note: probably a much better way to do this,
    int input;     // just wanted to get a start on how we could do it.
    int loop=1;
    int check=1;
    char term;
    printf("-----------------------------------------------\n");
    printf("            Welcome to Blackjack                \n");
    printf("-----------------------------------------------\n\n");
    printf("1. Start New Game\n");
    printf("2. Rules/Tutorial\n");
    printf("3. Exit Game\n");
    printf("Input: ");
    while(loop==1){
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
                printf("\nExiting Game...\n"); // Can't exit after playing around, just automatically starts a new game.
                loop=0;
                exit(0);
            default:
                printf("\nInvalid Input...\n\nPlease Re-Enter Input: ");
                while((getchar())!='\n');
                loop=1;
                check=1;
                break;
        }
    }
}
/*---------------------------------------------------Game Manager--------------------------------------------------------*/
void game() {
    int size=2;
    struct cards* hand=cardManager(size);
    struct cards* dealerHand=cardManager(size);

    dealerTurn(dealerHand,&size);

    printf("\nYour hand:");
    printCards(hand,size);
    playerTurn(hand,&size);

    free(hand);

//    dealerTurn(dealerHand,&size);
    playerTurn(hand,&size);
    free(hand);
}
/*---------------------------------------------------Cards Section-------------------------------------------------------*/
void printCards(struct cards hand[], int size){
  for(int i=0; i<size;i++){
      printf("\n  %s of %s",hand[i].rank,hand[i].suite);
  }
}
struct cards* cardManager(int hCount) {
  struct cards *handCard = malloc(sizeof(struct cards) * hCount); // Memory Allocation
  if (handCard == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
  }
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
      //printf("\nCard: %d; %s of %s",dCount+1,card[dCount].rank,card[dCount].suite);
      dCount++;
    }
  }
/*---------------------------------------------------Dealer-------------------------------------------------------*/
  // Dealing Cards
//  int hCount=5;
  int hand[hCount];
  int min=1;
  int max=52;
  int exit=0;
  int loop=0;
//  struct cards handCard[hCount];
///////////////////////////////////////////Card Dealing//////////////////////////////////////
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
//          printf("\nCard: %d, Card Value: %d",ii+1,hand[ii]);
//          printf(", Card Rank: %s of %s\n",handCard[ii].rank,handCard[ii].suite);
        }
  }
  return handCard;
}
/*-----------------------------------------------------------------Player Turn--------------------------------------------*/
void playerTurn(struct cards* hand, int* size){
  int total=handValue(hand, *size);
  int action;
  char term;
  int check=1;

  while(total<21){
    printf("\nYour hand value is: %d",total);
    printf("\nEnter 1 to hit\nEnter 2 to stand");
    printf("\nInput: ");
    while(check==1) {
//            printf("\nLoop\n");
        if(scanf("%d%c", &action, &term) != 2 || term != '\n') {  // Input buffer | Something breaks if you put an int above 2 idk y
            printf("\nInvalid Input\n\nInput: ");
            getchar();
            check=1;
        }
        else {
            check=0;
        }
    }
  if(action==1){ // Probably needs to be a switch statement for more player actions
    *size+=1;
    hand=realloc(hand,sizeof(struct cards)*(*size));

    struct cards* newCard=cardManager(1);
    hand[*size-1]=newCard[0];
    free(newCard);
    total=handValue(hand,*size);

    if(total>21){
      printf("\nTotal is over 21;\nBust!\n\n");
      mainMenu();
    }
  }
  else if (action==2){
    printf("\nYour final total is: %d\n\n",total);
    mainMenu();
  }
  else {
    printf("\nInvalid Input...");
    check=1;
  }
  }
}
/*---------------------------------------------------DealerTurn----------------------------------------------------*/
void dealerTurn(struct cards card[],int* count){
  int total=handValue(card,*count);
  printf("\nDealer Hand:");
  printCards(card,*count);
  printf("\nDealer Hand Value: %d",total);
  printf("\n\n-----------------------------------------------\n");
}
//////////////////////////////////Hand Value Calculations////////////////////////////////////////////

int handValue(struct cards* hand,int size){
  int total=0;
  int ace=0;

  for(int i=0;i<size;i++){
    if(strcmp(hand[i].rank,"Jack")==0 || strcmp(hand[i].rank,"Queen")==0 || strcmp(hand[i].rank,"King")==0) {
      total+=10;
    }
    else if(strcmp(hand[i].rank,"Ace")==0){
      total+=11;
      ace++;
    }
    else {
      if((hand[i].rank,"Two")==0){
        total+=2;
      }
      else if(strcmp(hand[i].rank,"Three")==0){
        total+=3;
      }
      else if(strcmp(hand[i].rank,"Four")==0){
        total+=4;
      }
      else if(strcmp(hand[i].rank,"Five")==0){
        total+=5;
      }
      else if(strcmp(hand[i].rank,"Six")==0){
        total+=6;
      }
      else if(strcmp(hand[i].rank,"Seven")==0){
        total+=7;
      }
      else if(strcmp(hand[i].rank,"Eight")==0){
        total+=8;
      }
      else if(strcmp(hand[i].rank,"Nine")==0){
        total+=9;
      }
      else if(strcmp(hand[i].rank,"Ten")==0){
        total+=10;
      }
      else {
        total+=2;
      }
    }
//    printf("\nCurrent total: %d\n",total);
  }
  while (total>21&&ace>0){
    total-=10;
    ace--;
  }
  return total;
}
/*---------------------------------------------------Betting Manager----------------------------------------------------------------*/
//void bettingManager() {
//    printf("\n----------- Betting -----------\n");
//    printf("Current Points: %d\n", playerPoints);
//    printf("Enter your initial bet: ");
//
//    char term;
//    int check = 1;
//
//    while(check == 1) {
//        if(scanf("%d%c", &currentBet, &term) != 2 || term != '\n') {
//            printf("\nInvalid Input\n\nEnter your bet: ");
//            while(getchar() != '\n');
//            check = 1;
//        }
//        else if(currentBet <= 0) {
//            printf("\nBet must be greater than 0\n\nEnter your bet: ");
//            check = 1;
//        }
//        else if(currentBet > playerPoints) {
//            printf("\nInsufficient points! You only have %d points.\n\nEnter your bet: ", playerPoints);
//            check = 1;
//        }
//        else {
//            check = 0;
//        }
//    }

//    printf("\nYou bet %d points!\n", currentBet);
//    game();
//}

/*---------------------------------------------------Tutorial Section-------------------------------------------------------*/
void tutorial() {
     printf("-----------------------------------------------\n");
    printf("                Rules/Tutorial                 \n");
    printf("-----------------------------------------------\n\n");

    printf("Objective: Beat the dealer by having your hands total value closer to 21 without busting (going over 21)\n\n");

    printf("Terminology: \n");
    printf(" Hit         - Get another card from dealer\n");
    printf(" Bust        - Card amount is greater than 21\n");
    printf(" Hold        - Skip your turn\n");
    printf(" Blackjack   - First 2 cards dealt equal 21\n");
    printf(" Double Down - Doubles current bet and deals a single card, can only be done after initial 2 cards are dealt\n");
    printf(" Split       - Split current hand into 2 hands; an equal bet to the starting bet must be on each hand\n");
    printf(" Hand        - Current set of cards a player has \n");
    printf(" Dealer      - Computer opponent that deals cards\n\n");

    printf("Card Values: \n");
    printf(" Number Cards (2-10) = Face value: \n");
    printf(" Face Cards (J, Q, K) = 10 \n");
    printf(" Ace (A) = 11 or 1 when hand total would be higher than 21 \n\n");

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

    mainMenu();
}
