#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auction.h"

struct Players Bit[MAX_PLAYERS];
struct Teams Teams[MAX_TEAMS];

// Function to find a team by its ID (returns -1 when no team matches)
int findTeamIndex(int teamID, const struct Teams *teams, int count){
    for(int i = 0; i < count; i++){
        if(teams[i].TeamID == teamID){
            return i;
        }
    }
    return -1;
}

// Function to find a player by its ID (returns -1 when no player matches)
int findPlayerIndex(int playerID, const struct Players *players, int count){
    for(int i = 0; i < count; i++){
        if(players[i].ID == playerID){
            return i;
        }
    }
    return -1;
}

bool isValidPlayerCount(int count){
    return count >= 1 && count <= MAX_PLAYERS;
}

bool isValidTeamCount(int count){
    return count >= 1 && count <= MAX_TEAMS;
}

bool canStartAuction(int number_of_players, int number_of_teams){
    return isValidPlayerCount(number_of_players) && isValidTeamCount(number_of_teams);
}

// Reads one bounded line and strips the newline. Returns INPUT_EOF at end of input.
InputStatus readLine(const char *prompt, char *buf, size_t size){
    if(prompt != NULL){
        printf("%s", prompt);
        fflush(stdout);
    }

    if(buf == NULL || size == 0){
        return INPUT_INVALID;
    }

    if(fgets(buf, (int)size, stdin) == NULL){
        return INPUT_EOF;
    }

    size_t length = strlen(buf);
    if(length > 0 && buf[length - 1] == '\n'){
        buf[length - 1] = '\0';
        if(length > 1 && buf[length - 2] == '\r'){
            buf[length - 2] = '\0';
        }
    } else if(!feof(stdin)){
        // Discard the rest of an over-long line.
        int c;
        while((c = getchar()) != '\n' && c != EOF){ }
    }

    return INPUT_OK;
}

// Reads and validates one integer. Invalid text is rejected instead of left in the buffer.
InputStatus readInt(const char *prompt, int *out){
    char buffer[64];
    InputStatus status = readLine(prompt, buffer, sizeof(buffer));

    if(status != INPUT_OK){
        return status;
    }

    if(buffer[0] == '\0'){
        return INPUT_INVALID;
    }

    errno = 0;
    char *end = NULL;
    long value = strtol(buffer, &end, 10);

    if(end == buffer){
        return INPUT_INVALID;
    }

    while(*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'){
        end++;
    }

    if(*end != '\0' || errno == ERANGE || value < INT_MIN || value > INT_MAX){
        return INPUT_INVALID;
    }

    *out = (int)value;
    return INPUT_OK;
}

// Adds one validated player record and returns the new count, or -1 on failure.
int addPlayerRecord(struct Players *players, int count,
                    const char *name, int id,
                    const char *category, int startingPrice){
    if(players == NULL || name == NULL || category == NULL){
        return -1;
    }
    if(count < 0 || count >= MAX_PLAYERS){
        return -1;
    }
    if(id <= 0 || startingPrice < 1){
        return -1;
    }
    if(name[0] == '\0' || category[0] == '\0'){
        return -1;
    }
    if(findPlayerIndex(id, players, count) >= 0){
        return -1;
    }

    snprintf(players[count].Name, sizeof(players[count].Name), "%s", name);
    players[count].ID = id;
    snprintf(players[count].Category, sizeof(players[count].Category), "%s", category);
    players[count].startingPrice = startingPrice;
    players[count].sould = 0;
    players[count].souldToTeamID = 0;
    players[count].souldPrice = 0;

    return count + 1;
}

// Adds one validated team record and returns the new count, or -1 on failure.
int addTeamRecord(struct Teams *teams, int count,
                  const char *name, int id, int budget){
    if(teams == NULL || name == NULL){
        return -1;
    }
    if(count < 0 || count >= MAX_TEAMS){
        return -1;
    }
    if(id <= 0 || budget < 0){
        return -1;
    }
    if(name[0] == '\0'){
        return -1;
    }
    if(findTeamIndex(id, teams, count) >= 0){
        return -1;
    }

    snprintf(teams[count].TeamName, sizeof(teams[count].TeamName), "%s", name);
    teams[count].TeamID = id;
    teams[count].Budget = budget;
    teams[count].NumberOfPlayers = 0;

    return count + 1;
}

// Function to add players
int AddPlayer(){
    int Number_of_Players;
    char countPrompt[64];

    snprintf(countPrompt, sizeof(countPrompt),
             "Enter the Number of Players (1-%d): ", MAX_PLAYERS);

    for(;;){
        InputStatus status = readInt(countPrompt, &Number_of_Players);

        if(status == INPUT_EOF){
            printf("\nEnd of input. No players were added.\n");
            return -1;
        }

        if(status == INPUT_INVALID){
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if(!isValidPlayerCount(Number_of_Players)){
            printf("Number of players must be between 1 and %d.\n", MAX_PLAYERS);
            continue;
        }

        break;
    }

    int count = 0;
    for(int i = 0; i < Number_of_Players; i++){
        char name[50];
        char category[30];
        int id;
        int startingPrice;
        InputStatus status;

        for(;;){
            status = readLine("Enter Player Name: ", name, sizeof(name));
            if(status == INPUT_EOF){
                printf("\nEnd of input. Player registration aborted.\n");
                return -1;
            }
            if(name[0] == '\0'){
                printf("Player name cannot be empty.\n");
                continue;
            }
            break;
        }

        for(;;){
            status = readInt("Enter Player ID: ", &id);
            if(status == INPUT_EOF){
                printf("\nEnd of input. Player registration aborted.\n");
                return -1;
            }
            if(status == INPUT_INVALID){
                printf("Invalid number, try again.\n");
                continue;
            }
            if(id <= 0){
                printf("Player ID must be positive.\n");
                continue;
            }
            if(findPlayerIndex(id, Bit, count) >= 0){
                printf("Player ID %d already exists.\n", id);
                continue;
            }
            break;
        }

        for(;;){
            status = readLine("Enter Player Category: ", category, sizeof(category));
            if(status == INPUT_EOF){
                printf("\nEnd of input. Player registration aborted.\n");
                return -1;
            }
            if(category[0] == '\0'){
                printf("Player category cannot be empty.\n");
                continue;
            }
            break;
        }

        for(;;){
            status = readInt("Enter Player Starting Price: ", &startingPrice);
            if(status == INPUT_EOF){
                printf("\nEnd of input. Player registration aborted.\n");
                return -1;
            }
            if(status == INPUT_INVALID){
                printf("Invalid number, try again.\n");
                continue;
            }
            if(startingPrice < 1){
                printf("Starting price must be at least 1.\n");
                continue;
            }
            break;
        }

        int newCount = addPlayerRecord(Bit, count, name, id, category, startingPrice);
        if(newCount < 0){
            printf("Could not add player. Please try again.\n");
            i--;
            continue;
        }

        count = newCount;
        printf("\n\n");
    }

    return count;
}

// Function to add teams
int AddTeam(){
    int number_of_teams;
    char countPrompt[64];

    snprintf(countPrompt, sizeof(countPrompt),
             "Enter the Number of Teams (1-%d): ", MAX_TEAMS);

    for(;;){
        InputStatus status = readInt(countPrompt, &number_of_teams);

        if(status == INPUT_EOF){
            printf("\nEnd of input. No teams were added.\n");
            return -1;
        }

        if(status == INPUT_INVALID){
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if(!isValidTeamCount(number_of_teams)){
            printf("Number of teams must be between 1 and %d.\n", MAX_TEAMS);
            continue;
        }

        break;
    }

    int count = 0;
    for(int i = 0; i < number_of_teams; i++){
        char name[50];
        int id;
        int budget;
        InputStatus status;

        for(;;){
            status = readLine("Enter Team Name: ", name, sizeof(name));
            if(status == INPUT_EOF){
                printf("\nEnd of input. Team registration aborted.\n");
                return -1;
            }
            if(name[0] == '\0'){
                printf("Team name cannot be empty.\n");
                continue;
            }
            break;
        }

        for(;;){
            status = readInt("Enter Team ID: ", &id);
            if(status == INPUT_EOF){
                printf("\nEnd of input. Team registration aborted.\n");
                return -1;
            }
            if(status == INPUT_INVALID){
                printf("Invalid number, try again.\n");
                continue;
            }
            if(id <= 0){
                printf("Team ID must be positive.\n");
                continue;
            }
            if(findTeamIndex(id, Teams, count) >= 0){
                printf("Team ID %d already exists.\n", id);
                continue;
            }
            break;
        }

        for(;;){
            status = readInt("Enter Team Budget: ", &budget);
            if(status == INPUT_EOF){
                printf("\nEnd of input. Team registration aborted.\n");
                return -1;
            }
            if(status == INPUT_INVALID){
                printf("Invalid number, try again.\n");
                continue;
            }
            if(budget < 0){
                printf("Team budget cannot be negative.\n");
                continue;
            }
            break;
        }

        int newCount = addTeamRecord(Teams, count, name, id, budget);
        if(newCount < 0){
            printf("Could not add team. Please try again.\n");
            i--;
            continue;
        }

        count = newCount;
        printf("\n\n");
    }

    return count;
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

// Central bid validation. Returns the first rule that the bid breaks.
BidResult validateBid(const struct Teams *teams, int teamCount, int teamIndex,
                      int amount, int highestBid, int startingPrice){
    if(teams == NULL || teamIndex < 0 || teamIndex >= teamCount || teamIndex >= MAX_TEAMS){
        return BID_TEAM_NOT_FOUND;
    }
    if(teams[teamIndex].NumberOfPlayers >= MAX_SQUAD){
        return BID_TEAM_FULL;
    }
    if(amount < startingPrice){
        return BID_BELOW_START;
    }
    if(amount <= highestBid){
        return BID_NOT_HIGHER;
    }
    if(amount > teams[teamIndex].Budget){
        return BID_OVER_BUDGET;
    }
    return BID_OK;
}

// Applies a winning bid, re-checking every rule so the budget can never go negative.
int processSale(struct Players *players, int playerIndex,
                struct Teams *teams, int teamCount,
                int winningTeamID, int amount){
    if(players == NULL || teams == NULL){
        return 0;
    }
    if(playerIndex < 0 || playerIndex >= MAX_PLAYERS){
        return 0;
    }
    if(teamCount < 0 || teamCount > MAX_TEAMS){
        return 0;
    }
    if(players[playerIndex].sould){
        return 0;
    }

    int teamIndex = findTeamIndex(winningTeamID, teams, teamCount);
    if(teamIndex < 0){
        return 0;
    }
    if(teams[teamIndex].NumberOfPlayers >= MAX_SQUAD){
        return 0;
    }
    if(amount < players[playerIndex].startingPrice){
        return 0;
    }
    if(amount > teams[teamIndex].Budget){
        return 0;
    }

    players[playerIndex].sould = 1;
    players[playerIndex].souldToTeamID = winningTeamID;
    players[playerIndex].souldPrice = amount;
    teams[teamIndex].Budget -= amount;
    teams[teamIndex].NumberOfPlayers++;

    return 1;
}

void markUnsold(struct Players *players, int playerIndex){
    if(players == NULL || playerIndex < 0 || playerIndex >= MAX_PLAYERS){
        return;
    }

    players[playerIndex].sould = 0;
    players[playerIndex].souldToTeamID = 0;
    players[playerIndex].souldPrice = 0;
}

bool canAuctionPlayer(const struct Players *players, int playerIndex){
    if(players == NULL || playerIndex < 0 || playerIndex >= MAX_PLAYERS){
        return false;
    }
    return players[playerIndex].sould == 0;
}

void StartAuction(int number_of_players, int number_of_teams) {
    if(!canStartAuction(number_of_players, number_of_teams)){
        printf("Cannot start auction: add players and teams first.\n");
        return;
    }

    printf("\n================ AUCTION STARTED ================\n");

    for (int i = 0; i < number_of_players; i++) {
        if(!canAuctionPlayer(Bit, i)){
            continue;
        }

        printf("\nAuctioning Player: %s (ID: %d, Category: %s, Starting Price: %d)\n",
               Bit[i].Name, Bit[i].ID, Bit[i].Category, Bit[i].startingPrice);

        int highestBid = 0;
        int winningTeamID = -1;

        while (1) {
            int teamID = 0;
            InputStatus status = readInt("Enter team ID (0 to end bidding for this player): ", &teamID);

            if(status == INPUT_EOF){
                printf("\nEnd of input. Ending bidding for this player.\n");
                break;
            }
            if(status == INPUT_INVALID){
                printf("Invalid input. Please enter a team ID or 0 to end.\n");
                continue;
            }
            if(teamID == BID_END_SENTINEL){
                break;
            }

            int teamIndex = findTeamIndex(teamID, Teams, number_of_teams);
            if(teamIndex < 0){
                printf("No such team: %d\n", teamID);
                continue;
            }

            int bidAmount = 0;
            int amountRead = 0;
            while(!amountRead){
                status = readInt("Enter bid amount: ", &bidAmount);
                if(status == INPUT_EOF){
                    printf("\nEnd of input. Ending bidding for this player.\n");
                    break;
                }
                if(status == INPUT_INVALID){
                    printf("Invalid input. Please enter a number.\n");
                    continue;
                }
                amountRead = 1;
            }
            if(!amountRead){
                break;
            }

            BidResult result = validateBid(Teams, number_of_teams, teamIndex,
                                           bidAmount, highestBid,
                                           Bit[i].startingPrice);

            switch(result){
                case BID_OK:
                    highestBid = bidAmount;
                    winningTeamID = Teams[teamIndex].TeamID;
                    printf("New highest bid: %d by Team %s.\n",
                           highestBid, Teams[teamIndex].TeamName);
                    break;
                case BID_TEAM_NOT_FOUND:
                    printf("No such team: %d\n", teamID);
                    break;
                case BID_TEAM_FULL:
                    printf("Team %s already has the maximum number of players (%d).\n",
                           Teams[teamIndex].TeamName, MAX_SQUAD);
                    break;
                case BID_BELOW_START:
                    printf("Bid must be at least the starting price (%d).\n",
                           Bit[i].startingPrice);
                    break;
                case BID_NOT_HIGHER:
                    printf("Bid must be higher than the current highest bid (%d).\n",
                           highestBid);
                    break;
                case BID_OVER_BUDGET:
                    printf("Team %s can only afford up to %d.\n",
                           Teams[teamIndex].TeamName, Teams[teamIndex].Budget);
                    break;
                default:
                    printf("Bid rejected.\n");
                    break;
            }
        }

        if (winningTeamID != -1) {
            if(processSale(Bit, i, Teams, number_of_teams, winningTeamID, highestBid)){
                printf("Player %s sold to Team ID %d for %d!\n",
                       Bit[i].Name, winningTeamID, highestBid);
            } else {
                markUnsold(Bit, i);
                printf("Sale failed for Player %s.\n", Bit[i].Name);
            }
        } else {
            markUnsold(Bit, i);
            printf("No valid bids received for Player %s.\n", Bit[i].Name);
        }
    }

    printf("\n================ AUCTION ENDED ================\n");
}


#ifndef TESTING
int main(){

    // Add Players and Teams
    int Number_of_Players = AddPlayer();
    if(Number_of_Players <= 0){
        return 1;
    }

    // Add Teams
    int number_of_teams = AddTeam();
    if(number_of_teams <= 0){
        return 1;
    }

    // Display players
    DisplayPlayers(Number_of_Players);

    // Display teams
    DisplayTeams(number_of_teams);

    //Start Auction
    StartAuction(Number_of_Players, number_of_teams);

    return 0;
}
#endif
