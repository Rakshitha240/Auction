#ifndef AUCTION_H
#define AUCTION_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_PLAYERS 100
#define MAX_TEAMS 10
#define MAX_SQUAD 5
#define BID_END_SENTINEL 0

struct Players {
    char Name[50];
    int ID;
    char Category[30];
    int startingPrice;
    int sould;
    int souldToTeamID;
    int souldPrice;
};

struct Teams {
    char TeamName[50];
    int TeamID;
    int Budget;
    int NumberOfPlayers;
};

extern struct Players Bit[MAX_PLAYERS];
extern struct Teams Teams[MAX_TEAMS];

typedef enum {
    INPUT_OK = 0,
    INPUT_INVALID,
    INPUT_EOF
} InputStatus;

typedef enum {
    BID_OK = 0,
    BID_TEAM_NOT_FOUND,
    BID_TEAM_FULL,
    BID_BELOW_START,
    BID_NOT_HIGHER,
    BID_OVER_BUDGET
} BidResult;

int findTeamIndex(int teamID, const struct Teams *teams, int count);
int findPlayerIndex(int playerID, const struct Players *players, int count);

bool isValidPlayerCount(int count);
bool isValidTeamCount(int count);
bool canStartAuction(int number_of_players, int number_of_teams);

int addPlayerRecord(struct Players *players, int count,
                    const char *name, int id,
                    const char *category, int startingPrice);
int addTeamRecord(struct Teams *teams, int count,
                  const char *name, int id, int budget);

InputStatus readLine(const char *prompt, char *buf, size_t size);
InputStatus readInt(const char *prompt, int *out);

int AddPlayer(void);
int AddTeam(void);
void DisplayPlayers(int number_of_players);
void DisplayTeams(int number_of_teams);
void StartAuction(int number_of_players, int number_of_teams);

BidResult validateBid(const struct Teams *teams, int teamCount, int teamIndex,
                      int amount, int highestBid, int startingPrice);
int processSale(struct Players *players, int playerIndex,
                struct Teams *teams, int teamCount,
                int winningTeamID, int amount);
void markUnsold(struct Players *players, int playerIndex);
bool canAuctionPlayer(const struct Players *players, int playerIndex);

#endif
