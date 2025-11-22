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
  const char *suit;
  int count;
};

struct bets{
  int balance;
  int bet;
};

// Global deck state
int globalDeck[52];
int globalAvail[52];
struct cards globalCards[52];
int deckInitialized = 0;

void mainMenu();
void game();
void tutorial();
struct bets bettingManager(int initial);
struct bets betting(struct bets betIn);
struct bets doubleDown(struct bets betIn);

void initializeDeck();
struct cards* cardManager(int hCount);
void printCards(struct cards card[], int count);
int playerTurn(struct cards** hand, int* size);
int handValue(struct cards* hand, int size);
int dealerTurn(struct cards** hand, int* size);
int determineWinner(int playerTotal, int playerSecondTotal, int dealerTotal);
int payOut(struct bets bet);
void delay(int mili);

int main(){
  printf("***********GAME START**********\n");
  srand(time(NULL)); // Initialize random seed once at start
  mainMenu();

  return 0;
}

/*---------------------------------------------------Deck Initialization-------------------------------------------------------*/
void initializeDeck() {
    const char *suits[] = {"Spades", "Hearts", "Clubs", "Diamonds"};
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
            globalCards[dCount].suit = suits[j];
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
    printf("            Welcome to Blackjack               \n");
    printf("          _____                                \n");
    printf("         |A .  | _____                         \n");
    printf("         | /.\\ ||A ^  | _____                 \n");
    printf("         |(_._)|| / \\ ||A _  | _____          \n");
    printf("         |  |  || \\ / || ( ) ||A_ _ |         \n");
    printf("         |____V||  .  ||(_'_)||( v )|          \n");
    printf("                |____V||  |  || \\ / |         \n");
    printf("                       |____V||  .  |          \n");
    printf("                              |____V|          \n");
    printf("Developed by Leilani, Alex, Javier, Joss\n");
    printf("(ACSII art credit: ejm98)\n");
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
    //delay(500);
    printf("\n-----------------------------------------------\n");
    printf("              NEW GAME STARTING                \n");
    printf("-----------------------------------------------\n");

    // Reset deck for new game
    initializeDeck();

    int playerSize=2;
    int dealerSize=2;
    int initialBet=1;
    int gameOver=0;


    struct bets playerBet=bettingManager(initialBet);
    initialBet=0;
    while(gameOver==0){
      playerBet=betting(playerBet);

      // Deal initial cards to player and dealer from shared deck
      struct cards* playerHand1=cardManager(playerSize);
      struct cards* playerHand2=cardManager(playerSize); // Leilani's note: I don't think this is quite right
      struct cards* dealerHand=cardManager(dealerSize);

      // Pending: implement if-statement to check for second hand and print it also if it's there
      printf("\nYour hand consists of:");
      printCards(playerHand1,playerSize);
      printf("\n\nDealer shows: %s of %s", dealerHand[0].rank, dealerHand[0].suit);
      printf("\n(Dealer's second card is hidden)\n");

      // Alternating turns between player and dealer
      int playerStanding = 0;
      int dealerStanding = 0;
      int playerBusted = 0;
      int dealerBusted = 0;
      int playerInitialBet;
      int playerAction;
      int repeat=1;
      while(!playerStanding || !dealerStanding) {
        // Player's turn (if not standing and not busted)
        if(!playerStanding && !playerBusted) {
          delay(500);
          printf("\n-----------------------------------------------\n");
          printf("              YOUR TURN                        \n");
          printf("-----------------------------------------------\n");

          while(repeat==1) {
            playerAction = playerTurn(&playerHand1, &playerSize);

            if(playerAction == 0) { // Player busted
              playerBusted = 1;
              playerStanding = 1;
              dealerStanding = 1; // End game if player busts
              repeat=0;
              break;
            }
            else if(playerAction==1){
              playerStanding=0;
              repeat=1;
            }
            else if(playerAction == 2) { // Player stands
              playerStanding = 1;
              repeat=0;
            }
            else if(playerAction==3){ // Player doubles down
              playerStanding=1;
              playerInitialBet=playerBet.bet;
              playerBet=doubleDown(playerBet);
              if(playerInitialBet==playerBet.bet){
                repeat=1;
              }
              else{
                repeat=0;
              }

            }
            else if(playerAction==4){ // Player splits
              playerStanding = 0; // Placeholder
              //
            }
          }
        }

        // Dealer's turn (if not standing and not busted)
        if(!dealerStanding && !dealerBusted && !playerBusted) {
          delay(500);
          printf("\n-----------------------------------------------\n");
          printf("              DEALER'S TURN                    \n");
          printf("-----------------------------------------------\n");

          int dealerAction = dealerTurn(&dealerHand, &dealerSize);

          if(dealerAction == 0) { // Dealer busted
            dealerBusted = 1;
            dealerStanding = 1;
            break;
          } else if(dealerAction == 2) { // Dealer stands
            dealerStanding = 1;
          }
          // dealerAction == 1 means dealer hit and continues
        }

        // If player is standing, dealer continues until standing
        if(playerStanding && !dealerStanding && !dealerBusted) {
          continue;
        }
      }

      // Determine winner
      int playerTotal = handValue(playerHand1, playerSize);
      int playerTotalSecondHand = handValue(playerHand2, playerSize);
      int dealerTotal = handValue(dealerHand, dealerSize);
      int result=determineWinner(playerTotal, playerTotalSecondHand, dealerTotal);
      switch(result){
        case 1:
          playerBet.balance+=payOut(playerBet);
//          printf("Balance after: %d",playerBet.balance);
          break;
        case 2:
          playerBet.balance+=playerBet.bet;
          break;
        default:
          break;
      }

      free(playerHand1);
      free(playerHand2);
      free(dealerHand);

      printf("\n");
      if(playerBet.balance<=0){
        printf("\n\e[1;31mYou're out of money! Game Over.\n");
        gameOver=1;
      }
      else{
        printf("\nStarting next round...\n");
        initializeDeck(); // Reset deck for next round
        playerSize = 2;
        dealerSize = 2;
        gameOver=0;
      }
    }
    mainMenu();
}
/*---------------------------------------------------Cards Section-------------------------------------------------------*/
void printCards(struct cards hand[], int size){
  for(int i=0; i<size;i++){
      printf("\n  %s of %s",hand[i].rank,hand[i].suit);
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
                handCard[ii].suit = globalCards[cardIndex].suit;
                handCard[ii].count = globalCards[cardIndex].count;
                found = 1;
            }
        }
    }

    return handCard;
}

/*-----------------------------------------------------------------Player Turn--------------------------------------------*/
// 0 = bust, 1 = hit, 2 = stand, 3 = double down, 4 = split
int playerTurn(struct cards** hand, int* size){
  int total=handValue(*hand, *size);
  int action;
  char term;
  int check=1;
  int val = 0;
  int isSplit = 0;

  printf("\nYour current hand:");
  printCards(*hand, *size);
  printf("\n\nYour hand value is: %d",total);
  printf("\nEnter 1 to Hit\nEnter 2 to Stand\nEnter 3 to Double Down\nEnter 4 to Split"); // Requirement FL3: The product shall include a UI to allow the play to hit, hold, split
  printf("\nInput: "); // Requirement FR3: The system should allow user actions (Hit, Hold, Double Down, Split)

  check=1;
  while(check==1) {
      if(scanf("%d%c", &action, &term) != 2 || term != '\n') {
          printf("\nInvalid Input\n\nInput: ");
          getchar();
          check=1;
      }
      else if(action != 1 && action != 2 && action != 3 && action != 4) {
          printf("\nInvalid Input (must be 1, 2, 3, or 4)\n\nInput: ");
          check=1;
      }
      else {
          check=0;
      }
  }

  if(action==1){ // Hit
    *size+=1;
    *hand=realloc(*hand,sizeof(struct cards)*(*size));
    if(*hand==NULL){
      fprintf(stderr,"\nMemory Allocation failed");
      exit(EXIT_FAILURE);
    }

    struct cards* newCard=cardManager(1);
    (*hand)[*size-1]=newCard[0];
    free(newCard);

    printf("\nYou drew: %s of %s",(*hand)[*size-1].rank,(*hand)[*size-1].suit);

    total=handValue(*hand,*size);
    //printf("\nYour new hand value: %d\n",total);

    if(total>21){
      printf("\n\nYour hand:");
      printCards(*hand,*size); // Requirement FR7: The system should track and display point totals after every hand
      printf("\n\nTotal is over 21: %d",total);
      printf("\n\e[1;31mBust! You lose!\n");
      return 0; // Busted
    }
    else if(total==21){
      printf("\nYou have 21! You must stand.\n");
      return 2; // Stand with 21
    }
    return 1; // Hit successfully
  }
  else if (action==2){ // Stand
    printf("\nYou stand with a total of: %d\n",total);
    return 2; // Standing
  }
  else if(action==3){ // Double Down
    *size+=1;
    *hand=realloc(*hand,sizeof(struct cards)*(*size));
    if(*hand==NULL){
      fprintf(stderr,"\nMemory Allocation failed");
      exit(EXIT_FAILURE);
    }

    struct cards* newCard=cardManager(1);
    (*hand)[*size-1]=newCard[0];
    free(newCard);

    printf("\nYou drew: %s of %s",(*hand)[*size-1].rank,(*hand)[*size-1].suit);

    total=handValue(*hand,*size);
    //printf("\nYour new hand value: %d\n",total);
    return 3;
  }
  else if(action==4){ // Split
    if (isSplit == 1){
      printf("\nYou've split already.\n");
    }
    
    else if ((*hand)[0].rank == (*hand)[1].rank){
      isSplit = 1;
      struct cards **secondHand;
      *secondHand=malloc(sizeof(struct cards)*(*size));
      secondHand[0] = hand[1];
      //free(hand[1]);
      //*hand=realloc(*hand,sizeof(struct cards)*(*size));
      //total=handValue(*secondHand,*size); // Segmentation fault error indicates this is a problem
      printf("\nYour second hand value: %s\n",secondHand);
      
    }
    else{
      printf("\nYou can't split this hand.\n");
      delay(500);
    }
    return 4;
  }
  return 2; // Default to standing
}

/*---------------------------------------------------DealerTurn----------------------------------------------------*/
// Requirement FR6: The system should simulate dealer actions per the rules we set
// Returns: 0 = busted, 1 = hit, 2 = stand
int dealerTurn(struct cards** hand, int* size){
  int total=handValue(*hand,*size);

  printf("\nDealer's current hand:");
  printCards(*hand, *size);
  printf("\nDealer's hand value: %d",total);

  // Dealer must hit on 16 or less, stand on 17 or more
  if(total<17){
    printf("\nDealer hits...");
    *size+=1;
    *hand=realloc(*hand,sizeof(struct cards)*(*size));
    if(*hand==NULL){
      fprintf(stderr,"\nMemory Allocation failed");
      exit(EXIT_FAILURE);
    }

    struct cards* newCard=cardManager(1);
    (*hand)[*size-1]=newCard[0];
    free(newCard);

    printf("\nDealer drew: %s of %s",(*hand)[*size-1].rank,(*hand)[*size-1].suit);

    total=handValue(*hand,*size);
    printf("\nDealer's new hand value: %d\n",total);

    if(total>21){
      printf("\nDealer busts!\n");
      return 0; // Busted
    }
    return 1; // Hit successfully
  }
  else{
    printf("\nDealer stands with %d.\n",total);
    return 2; // Standing
  }
}

/*---------------------------------------------------Determine Winner----------------------------------------------------*/
int determineWinner(int playerTotal, int playerSecondTotal, int dealerTotal){
  delay(500);
  printf("\n-----------------------------------------------\n");
  printf("                  RESULTS                      \n");
  printf("-----------------------------------------------\n\n");
  printf("Your total: %d\n",playerTotal);
  printf("Dealer's total: %d\n",dealerTotal);

  if(playerTotal>21){
    printf("\nYou busted! Dealer wins!\n");
    return 0;
  }
  else if(dealerTotal>21){
    printf("\nDealer busts! \e[1;32mYou win!\n"); // Requirement FR5: The system should declare outcomes (Bust, Blackjack, Win, Loss, Tie)
    return 1;
  }
  else if(playerTotal>dealerTotal){
    printf("\n\e[1;32mYou win!\n");
    return 1;
  }
  else if(playerTotal<dealerTotal){
    printf("\nDealer wins!\n");
    return 0;
  }
  else{
    printf("\nIt's a push (tie)!\n");
    return 2;
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
struct bets bettingManager(int initial) {
  char term;
  int check = 1;
  struct bets bet;
  if (initial==1){
    printf("\n----------- Betting Initialization -----------\n");
    printf("Please enter your total balance: ");
    while(check==1){
      if(scanf("%d%c",&bet.balance,&term)!=2||term!='\n'){
        printf("\nInvalid Input\n\nEnter total balance: ");
        check=1;
      }
      else if(bet.balance<=0){
        printf("\nBalance must be above 0\n\nEnter total Balance: ");
        check=1;
      }
      else {
        check=0;
        printf("\n");
        return bet;
      }
    }
  }
}

struct bets betting(struct bets betIn){
  int check=1;
  char term;
  printf("\n----------- Betting -----------\n");
  printf("Current Points: %d\n",betIn.balance);
  printf("Enter your bet: ");

  while(check==1){
    if(scanf("%d%c",&betIn.bet,&term)!=2||term!='\n'){
      printf("\nInvalid Input\n\nEnter your bet: ");
      while(getchar()!='\n');
      check=1;
    }
    else if(betIn.bet<=0){
      printf("\nBet must be greater than 0\n\nEnter your bet: ");
      check=1;
    }
    else if(betIn.bet>betIn.balance){
      printf("\nInsufficient points! You only have %d points.\n\nEnter your bet: ",betIn.balance);
      check=1;
    }
    else {
      check=0;
      betIn.balance=betIn.balance-betIn.bet;
    }
  }
  printf("\nYou bet %d points!\n",betIn.bet);
  printf("\nYou have %d points remaining\n",betIn.balance);
  return betIn;
}
struct bets doubleDown(struct bets betIn){
  if (betIn.bet*2>betIn.balance){
    printf("\nCan't increase bet\nYour bet is still %d\n",betIn.bet);
    return betIn;
  }
  betIn.balance-=betIn.bet;
  betIn.bet+=betIn.bet;
  printf("\nYou double down and change your bet to %d\n",betIn.bet);
  return betIn;
}

int payOut(struct bets bet){
//  printf("\nBalance before: %d",bet.balance);
  int pay=bet.bet+bet.bet/2;
  return pay;
}

/*int split(){

}*/


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
    printf("- American Blackjack payouts for Blackjack are 3:2\n");
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

void delay(int mili) {
  int start=clock();
  int delay=mili;
  while(clock()<start+delay);
}
