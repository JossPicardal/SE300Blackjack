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

// Global deck state
int globalDeck[52];
int globalAvail[52];
struct cards globalCards[52];
int deckInitialized = 0;

void mainMenu();
void game();
void tutorial();
void bettingManager();
void initializeDeck();
struct cards* cardManager(int hCount);
void printCards(struct cards card[], int count);
void playerTurn(struct cards* hand, int* size, int* playerDone);
int handValue(struct cards* hand, int size);
void dealerTurn(struct cards** hand, int* size);
void determineWinner(int playerTotal, int dealerTotal);

int main(){
  printf("***********GAME START**********\n");
  srand(time(NULL)); // Initialize random seed once at start
  mainMenu();

  return 0;
}

/*---------------------------------------------------Deck Initialization-------------------------------------------------------*/
void initializeDeck() {
    const char *suites[] = {"Spades", "Hearts", "Clubs", "Diamonds"};
    const char *ranks[] = {"Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};

    int dCount = 0;
    for(int i = 0; i < 52; i++) {
        globalDeck[i] = i + 1;
        globalAvail[i] = 1; // Mark all cards as available
    }

    for(int j = 0; j < 4; j++) {
        for(int jj = 0; jj < 13; jj++) {
            globalCards[dCount].count = jj + 1;
            globalCards[dCount].rank = ranks[jj];
            globalCards[dCount].suite = suites[j];
            dCount++;
        }
    }

    deckInitialized = 1;
}

/*---------------------------------------------------Main Menu-------------------------------------------------------*/
void mainMenu() {  // Requirement FL1: The product shall include a Menu
    int input;
    int loop=1;
    int check=1;
    char term;
    printf("-----------------------------------------------\n");
    printf("            Welcome to Blackjack                \n");
    printf("-----------------------------------------------\n\n");
    printf("1. Start New Game\n"); // Requirement FR1: The system should provide a menu with at least three options (Play, How to Play, Exit)
    printf("2. Rules/Tutorial\n");
    printf("3. Exit Game\n");
    printf("Input: ");
    while(loop==1){
        while(check==1) {
            if(scanf("%d%c", &input, &term) != 2 || term != '\n') {  // Input buffer
                printf("\nInvalid Input\n\nInput: ");
                getchar();
                check=1;
            }
            else {
                check=0;
            }
        }
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
/*---------------------------------------------------Game Manager--------------------------------------------------------*/
void game() {
    printf("\n-----------------------------------------------\n");
    printf("              NEW GAME STARTING                \n");
    printf("-----------------------------------------------\n");

    // Reset deck for new game
    initializeDeck();

    int playerSize=2;
    int dealerSize=2;
    int playerDone=0;

    // Deal initial cards to player and dealer from shared deck
    struct cards* playerHand=cardManager(playerSize);
    struct cards* dealerHand=cardManager(dealerSize);

    printf("\nYour hand consists of:");
    printCards(playerHand,playerSize);
    printf("\n\nDealer shows: %s of %s", dealerHand[0].rank, dealerHand[0].suite);
    printf("\n(Dealer's second card is hidden)\n");

    // Player's turn
    playerTurn(playerHand,&playerSize,&playerDone);

    // If player didn't bust, dealer plays
    if(playerDone==1){
        int playerTotal=handValue(playerHand,playerSize);

        printf("\n-----------------------------------------------\n");
        printf("              DEALER'S TURN                    \n");
        printf("-----------------------------------------------\n");

        // Reveal dealer's full hand
        printf("\nDealer's full hand:");
        printCards(dealerHand,dealerSize);
        printf("\nDealer's hand value: %d\n",handValue(dealerHand,dealerSize));

        // Dealer draws cards
        dealerTurn(&dealerHand,&dealerSize);

        int dealerTotal=handValue(dealerHand,dealerSize);

        // Determine winner
        determineWinner(playerTotal,dealerTotal);
    }

    free(playerHand);
    free(dealerHand);

    printf("\n");
    mainMenu();
}
/*---------------------------------------------------Cards Section-------------------------------------------------------*/
void printCards(struct cards hand[], int size){
  for(int i=0; i<size;i++){
      printf("\n  %s of %s",hand[i].rank,hand[i].suite);
  }
}

struct cards* cardManager(int hCount) {
    struct cards *handCard = malloc(sizeof(struct cards) * hCount);
    if (handCard == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Make sure deck is initialized
    if (!deckInitialized) {
        initializeDeck();
    }

    // Deal cards from the shared deck
    for(int ii = 0; ii < hCount; ii++) {
        int cardIndex;
        int found = 0;

        // Keep trying until we find an available card
        while(!found) {
            cardIndex = rand() % 52;
            if(globalAvail[cardIndex] == 1) {
                globalAvail[cardIndex] = 0; // Mark card as used
                handCard[ii].rank = globalCards[cardIndex].rank;
                handCard[ii].suite = globalCards[cardIndex].suite;
                handCard[ii].count = globalCards[cardIndex].count;
                found = 1;
            }
        }
    }

    return handCard;
}

/*-----------------------------------------------------------------Player Turn--------------------------------------------*/
void playerTurn(struct cards* hand, int* size, int* playerDone){
  int total=handValue(hand, *size);
  int action;
  char term;
  int check=1;

  while(total<21){
    printf("\n\nYour hand value is: %d",total);
    printf("\nEnter 1 to hit\nEnter 2 to stand"); // Requirement FL3: The product shall include a UI to allow the play to hit, hold, split
    printf("\nInput: "); // Requirement FR3: The system should allow user actions (Hit, Hold, Double Down, Split)
    check=1;
    while(check==1) {
        if(scanf("%d%c", &action, &term) != 2 || term != '\n') {
            printf("\nInvalid Input\n\nInput: ");
            getchar();
            check=1;
        }
        else {
            check=0;
        }
    }
  if(action==1){
    *size+=1;
    hand=realloc(hand,sizeof(struct cards)*(*size));
    if(hand==NULL){
      fprintf(stderr,"\nMemory Allocation failed");
      exit(EXIT_FAILURE);
    }

    struct cards* newCard=cardManager(1);
    hand[*size-1]=newCard[0];
    free(newCard);

    printf("\nYou drew: %s of %s",hand[*size-1].rank,hand[*size-1].suite);

    total=handValue(hand,*size);

    if(total>21){
      printf("\n\nYour hand:");
      printCards(hand,*size); // Requirement FR7: The system should track and display point totals after every hand
      printf("\n\nTotal is over 21: %d",total);
      printf("\nBust! You lose!\n");
      *playerDone=0;
      mainMenu();
    }
  }
  else if (action==2){
    printf("\nYou stand with a total of: %d",total);
    *playerDone=1;
    return;
  }
  else {
    printf("\nInvalid Input...");
  }
  }

  // If we exit the loop with exactly 21
  if(total==21){
    printf("\n\nYou have 21!");
    *playerDone=1;
  }
}
/*---------------------------------------------------DealerTurn----------------------------------------------------*/
// Requirement FR6: The system should simulate dealer actions per the rules we set
void dealerTurn(struct cards** hand, int* size){
  int total=handValue(*hand,*size);

  // Dealer must hit on 16 or less, stand on 17 or more
  while(total<17){
    printf("\nDealer hits...");
    *size+=1;
    *hand=realloc(*hand,sizeof(struct cards)*(*size));
    if(hand==NULL){
      fprintf(stderr,"\nMemory Allocation failed");
      exit(EXIT_FAILURE);
    }

    struct cards* newCard=cardManager(1);
    (*hand)[*size-1]=newCard[0];
    free(newCard);

    printf("\nDealer drew: %s of %s",(*hand)[*size-1].rank,(*hand)[*size-1].suite);

    total=handValue(*hand,*size);
    printf("\nDealer's hand value: %d",total);
  }

  printf("\n\nDealer's final hand:");
  printCards(*hand,*size);
  printf("\nDealer's final total: %d\n",total);

  if(total>21){
    printf("\nDealer busts!\n");
  }
  else{
    printf("\nDealer stands.\n");
  }
}
/*---------------------------------------------------Determine Winner----------------------------------------------------*/
void determineWinner(int playerTotal, int dealerTotal){
  printf("\n-----------------------------------------------\n");
  printf("                  RESULTS                      \n");
  printf("-----------------------------------------------\n");
  printf("Your total: %d\n",playerTotal);
  printf("Dealer's total: %d\n",dealerTotal);

  if(dealerTotal>21){
    printf("\nDealer busts! You win!\n"); // Requirement FR5: The system should declare outcomes (Bust, Blackjack, Win, Loss, Tie)
  }
  else if(playerTotal>dealerTotal){
    printf("\nYou win!\n");
  }
  else if(playerTotal<dealerTotal){
    printf("\nDealer wins!\n");
  }
  else{
    printf("\nIt's a push (tie)!\n");
  }
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
      if(strcmp(hand[i].rank,"Two")==0){
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
    }
  }
  while (total>21&&ace>0){
    total-=10;
    ace--;
  }
  return total;
}
/*---------------------------------------------------Betting Manager----------------------------------------------------------------*/
void bettingManager() { // Requirement FL5: The product shall include a point system for betting
    int playerPoints=1000;
    int currentBet=0;

    printf("\n----------- Betting -----------\n");
    printf("Current Points: %d\n", playerPoints);
    printf("Enter your initial bet: ");

    char term;
    int check = 1;

    while(check == 1) {
        if(scanf("%d%c", &currentBet, &term) != 2 || term != '\n') {
            printf("\nInvalid Input\n\nEnter your bet: ");
            while(getchar() != '\n');
            check = 1;
        }
        else if(currentBet <= 0) { // Requirement FR4: The system should include input validation for bets and choices. There should be a min of 1 point and a max of 1000 points
            printf("\nBet must be greater than 0\n\nEnter your bet: "); 
            check = 1;
        }
        else if(currentBet > playerPoints) {
            printf("\nInsufficient points! You only have %d points.\n\nEnter your bet: ", playerPoints);
            check = 1;
        }
        else {
            check = 0;
        }
    }

    printf("\nYou bet %d points!\n", currentBet);
    game();
}

/*---------------------------------------------------Tutorial Section-------------------------------------------------------*/
void tutorial() { // Requirement FL2: The product shall include a Tutorial to teach the player how to play
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

    printf("American Blackjack Rules: \n"); // Requirement FL4: The product shall follow American blackjack rules
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
