#include<stdio.h>
#include<stdlib.h>

#define MAX_PLAYERS 10

struct Players {
    char Name[50];
    int ID;
    char Category[30];
    int startingPrice;
    int sould;
    int souldToTeamID;
    int souldPrice;
}Bit[100];

struct Teams{
    char TeamName[50];
    int TeamID;
    int Budget;
    int NumberOfPlayers;
}Teams[10];

// Function to add players
int AddPlayer(){
    int Number_of_Players;
    printf("Enter the Number of Players: ");
    scanf("%d", &Number_of_Players);
    for(int i=0; i<Number_of_Players; i++){
        printf("Enter Player %d Name: ",i+1);
        scanf("%s", Bit[i].Name);
        printf("Enter Player ID: ");
        scanf("%d", &Bit[i].ID);
        printf("Enter Player Category: ");
        scanf("%s", Bit[i].Category);
        printf("Enter Player Starting Price: ");
        scanf("%d", &Bit[i].startingPrice);
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
    printf("Enter the Number of Teams: ");
    scanf("%d", &number_of_teams);
    for(int i=0; i<number_of_teams; i++){
        printf("Enter Team %d Name: ",i+1);
        scanf("%s", Teams[i].TeamName);
        printf("Enter Team ID: ");
        scanf("%d", &Teams[i].TeamID);
        printf("Enter Team Budget: ");
        scanf("%d", &Teams[i].Budget);
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
            printf("Enter team ID and amount (1010 to end bidding for this player): ");
            int teamID, bidAmount;
            scanf("%d", &teamID);
            if (teamID == 1010) {
                break;
            }
            scanf("%d", &bidAmount);
            int j;
            for (j = 0; j < number_of_teams; j++) {
                if (Teams[j].TeamID == teamID) {
                    break;
                }
            }

            if (bidAmount > highestBid && bidAmount <= Teams[j].Budget) {
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

    // Add Teams
    int number_of_teams = AddTeam();
    
    // Display players
    DisplayPlayers(Number_of_Players);

    // Display teams
    DisplayTeams(number_of_teams);  

    //Start Auction
    StartAuction(Number_of_Players, number_of_teams);
}

