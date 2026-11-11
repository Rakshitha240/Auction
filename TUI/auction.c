#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>

#define MAX_TEAMS 10
#define MAX_PLAYERS 100

struct Players {
    char Name[50];
    int ID;
    char Category[30];
    int startingPrice;
    int sould;
    int souldToTeamID;
    int souldPrice;
}Bit[MAX_PLAYERS];

struct Teams{
    char TeamName[50];
    int TeamID;
    int Budget;
    int NumberOfPlayers;
}Teams[MAX_TEAMS];

// Reads one integer. Returns true on success, false on invalid input.
bool readInt(const char *prompt, int *out) {
    printf("%s", prompt);
    if (scanf("%d", out) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }  // flush bad input
        return false;
    }
    return true;
}

// Function to add players
int AddPlayer(){
    int Number_of_Players;
    while(1){
       if (!readInt("Enter the Number of Players (1-100): ",
                &Number_of_Players)) {
    printf("Invalid input! Please enter a number.\n");
    continue;
}

    // Check array capacity
    if (Number_of_Players < 1 || Number_of_Players > MAX_PLAYERS) {
        printf("Number of players must be between 1 and %d.\n",
               MAX_PLAYERS);
        continue;
    }
    break;
}
    for(int i=0; i<Number_of_Players; i++){
        printf("Enter Player %d Name: ",i+1);
        scanf("%49s", Bit[i].Name);

        while (!readInt("Enter Player ID: ", &Bit[i].ID))
        printf("Invalid number, try again.\n");

        printf("Enter Player Category: ");
        scanf("%29s", Bit[i].Category);

        while (!readInt("Enter Player Starting Price: ", &Bit[i].startingPrice))
        printf("Invalid number, try again.\n");

         Bit[i].sould = 0;
        Bit[i].souldToTeamID = 0;
        Bit[i].souldPrice = 0;
        printf("\n\n");
    }
    return Number_of_Players;
}

// Function to add teams
int AddTeam(){
    int number_of_teams;
    while(1){
    if(!readInt("Enter the Number of Teams (1-10): ",
                &number_of_teams)) {
    printf("Invalid input! Please enter a number.\n");
    continue;
    }

    // Check array capacity
    if (number_of_teams < 1 || number_of_teams > MAX_TEAMS) {
        printf("Number of teams must be between 1 and %d.\n",
               MAX_TEAMS);
        continue;
    }
    break;
}
    for(int i=0; i<number_of_teams; i++){
        printf("Enter Team %d Name: ",i+1);
        scanf("%49s", Teams[i].TeamName);
        while (!readInt("Enter Team ID: ", &Teams[i].TeamID)) {
         printf("Invalid number! Try again.\n");
          }  
        while (!readInt("Enter Team Budget: ", &Teams[i].Budget)) {
         printf("Invalid number! Try again.\n");
         }
        Teams[i].NumberOfPlayers = 0;
        printf("\n\n");
    }
    return number_of_teams;
}

// Function to display players
void DisplayPlayers(int number_of_players) {
    printf("\n================ PLAYER LIST ================\n");

    printf("%-20s %-8s %-15s %-14s %-8s %-12s %-10s\n",
           "Name", "ID", "Category", "Start Price",
           "Sold", "Team ID", "Sold Price");

    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < number_of_players; i++) {
        printf("%-20s %-8d %-15s %-14d %-8d %-12d %-10d\n",
               Bit[i].Name,
               Bit[i].ID,
               Bit[i].Category,
               Bit[i].startingPrice,
               Bit[i].sould,
               Bit[i].souldToTeamID,
               Bit[i].souldPrice);
    }

    printf("--------------------------------------------------------------------------------\n");
}

// Function to display teams
void DisplayTeams(int number_of_teams) {
    printf("\n================ TEAM LIST ================\n");

    printf("%-20s %-8s %-12s %-15s\n",
           "Team Name", "Team ID", "Budget", "Number of Players");

    printf("-------------------------------------------------------------\n");

    for (int i = 0; i < number_of_teams; i++) {
        printf("%-20s %-8d %-12d %-15d\n",
               Teams[i].TeamName,
               Teams[i].TeamID,
               Teams[i].Budget,
               Teams[i].NumberOfPlayers);
    }

    printf("-------------------------------------------------------------\n");
}

void StartAuction(int number_of_players, int number_of_teams) {
    printf("\n================ AUCTION STARTED ================\n");

    for (int i = 0; i < number_of_players; i++) {
        printf("\nAuctioning Player: %s (ID: %d, Category: %s, Starting Price: %d)\n",
               Bit[i].Name, Bit[i].ID, Bit[i].Category, Bit[i].startingPrice);

        int highestBid = 0;
        int winningTeamID = -1;

        while (1) {
            int teamID, bidAmount;
           if (!readInt("Enter Team ID (1010 to end bidding): ", &teamID)) {
           printf("Invalid Team ID! Try again.\n");
          continue;
           }

          if (teamID == 1010) {
          break;
           }

            int j;
            for (j = 0; j < number_of_teams; j++) {
                if (Teams[j].TeamID == teamID) {
                    break;
                }
            }
         // Check whether the team ID exists
       if (j == number_of_teams) {
          printf("Invalid Team ID! Try again.\n");
        continue;
}
//To read bid amount safely 
while (!readInt("Enter Bid Amount: ", &bidAmount)) {
    printf("Invalid bid amount! Try again.\n");
}

if (bidAmount < Bit[i].startingPrice) {
    printf("Bid must be at least the starting price!\n");
}
else if (bidAmount <= highestBid) {
    printf("Bid must be higher than the current highest bid!\n");
}
else if (bidAmount > Teams[j].Budget) {
    printf("Insufficient team budget!\n");
}
else {
    highestBid = bidAmount;
    winningTeamID = Teams[j].TeamID;
}


        }

        if (winningTeamID != -1) {
            // Update player details
            Bit[i].sould = 1;
            Bit[i].souldToTeamID = winningTeamID;
            Bit[i].souldPrice = highestBid;

            // Update team budget and player count
            for (int j = 0; j < number_of_teams; j++) {
                if (Teams[j].TeamID == winningTeamID) {
                    Teams[j].Budget -= highestBid;
                    Teams[j].NumberOfPlayers++;
                    break;
                }
            }

            printf("Player %s sold to Team ID %d for %d!\n", Bit[i].Name, winningTeamID, highestBid);
        } else {
            printf("No valid bids received for Player %s.\n", Bit[i].Name);
        }
    }

    printf("\n================ AUCTION ENDED ================\n");
}


int main(){

    // Add Players and Teams
    int Number_of_Players = AddPlayer();
    if (Number_of_Players == -1) {
        return 1;
    }
    // Add Teams
    int number_of_teams = AddTeam();
    if (number_of_teams == -1) {
        return 1;
    }
    
    // Display players
    DisplayPlayers(Number_of_Players);

    // Display teams
    DisplayTeams(number_of_teams);  

    //Start Auction
    StartAuction(Number_of_Players, number_of_teams);
}

