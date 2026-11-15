#include <stdio.h>
#include <string.h>

#include "auction.h"

static int passed = 0;
static int failed = 0;

static void check(const char *name, int condition){
    if(condition){
        printf("PASS: %s\n", name);
        passed++;
    } else {
        printf("FAIL: %s\n", name);
        failed++;
    }
}

static void resetPlayers(struct Players *players){
    memset(players, 0, sizeof(struct Players) * MAX_PLAYERS);
}

static void resetTeams(struct Teams *teams){
    memset(teams, 0, sizeof(struct Teams) * MAX_TEAMS);
}

static void test_players(void){
    struct Players players[MAX_PLAYERS];
    int count = 0;
    int ok = 1;

    resetPlayers(players);

    for(int i = 1; i <= MAX_PLAYERS; i++){
        char name[32];
        snprintf(name, sizeof(name), "Player%d", i);
        int next = addPlayerRecord(players, count, name, i, "AllRounder", 10);
        if(next != count + 1){
            ok = 0;
            break;
        }
        count = next;
    }

    check("Can add up to MAX_PLAYERS players", ok && count == MAX_PLAYERS);
    check("Adding more than MAX_PLAYERS players is rejected",
          addPlayerRecord(players, count, "Extra", MAX_PLAYERS + 1, "Batsman", 10) == -1);

    check("Player count 0 is rejected", !isValidPlayerCount(0));
    check("Negative player count is rejected", !isValidPlayerCount(-3));
    check("Player count above MAX_PLAYERS is rejected",
          !isValidPlayerCount(MAX_PLAYERS + 1));
    check("Player count 1 is accepted", isValidPlayerCount(1));
    check("Player count MAX_PLAYERS is accepted", isValidPlayerCount(MAX_PLAYERS));

    resetPlayers(players);
    count = addPlayerRecord(players, 0, "Virat Kohli", 7, "Batsman", 200);

    check("Player fields are stored correctly",
          count == 1 &&
          strcmp(players[0].Name, "Virat Kohli") == 0 &&
          players[0].ID == 7 &&
          strcmp(players[0].Category, "Batsman") == 0 &&
          players[0].startingPrice == 200);

    check("New player starts as not sold",
          players[0].sould == 0 &&
          players[0].souldToTeamID == 0 &&
          players[0].souldPrice == 0);

    check("Duplicate player ID is rejected",
          addPlayerRecord(players, count, "Duplicate", 7, "Bowler", 50) == -1);
    check("Player ID 0 is rejected",
          addPlayerRecord(players, count, "Zero", 0, "Bowler", 50) == -1);
    check("Negative player ID is rejected",
          addPlayerRecord(players, count, "Negative", -1, "Bowler", 50) == -1);
    check("Starting price 0 is rejected",
          addPlayerRecord(players, count, "Free", 8, "Bowler", 0) == -1);
    check("Negative starting price is rejected",
          addPlayerRecord(players, count, "NegativePrice", 9, "Bowler", -10) == -1);
    check("Empty player name is rejected",
          addPlayerRecord(players, count, "", 10, "Bowler", 10) == -1);
}

static void test_teams(void){
    struct Teams teams[MAX_TEAMS];
    int count = 0;
    int ok = 1;

    resetTeams(teams);

    for(int i = 1; i <= MAX_TEAMS; i++){
        char name[32];
        snprintf(name, sizeof(name), "Team%d", i);
        int next = addTeamRecord(teams, count, name, i, 1000);
        if(next != count + 1){
            ok = 0;
            break;
        }
        count = next;
    }

    check("Can add up to MAX_TEAMS teams", ok && count == MAX_TEAMS);
    check("Adding more than MAX_TEAMS teams is rejected",
          addTeamRecord(teams, count, "Extra", MAX_TEAMS + 1, 1000) == -1);

    check("Team count 0 is rejected", !isValidTeamCount(0));
    check("Negative team count is rejected", !isValidTeamCount(-2));
    check("Team count above MAX_TEAMS is rejected", !isValidTeamCount(MAX_TEAMS + 1));
    check("Team count 1 is accepted", isValidTeamCount(1));
    check("Team count MAX_TEAMS is accepted", isValidTeamCount(MAX_TEAMS));

    resetTeams(teams);
    count = addTeamRecord(teams, 0, "Mumbai Indians", 5, 500);

    check("Team fields are stored correctly",
          count == 1 &&
          strcmp(teams[0].TeamName, "Mumbai Indians") == 0 &&
          teams[0].TeamID == 5 &&
          teams[0].Budget == 500);

    check("New team starts with zero players", teams[0].NumberOfPlayers == 0);
    check("findTeamIndex finds an existing team", findTeamIndex(5, teams, count) == 0);
    check("findTeamIndex rejects an unknown team", findTeamIndex(999, teams, count) == -1);

    check("Duplicate team ID is rejected",
          addTeamRecord(teams, count, "Duplicate", 5, 500) == -1);
    check("Team ID 0 is rejected",
          addTeamRecord(teams, count, "Zero", 0, 500) == -1);
    check("Negative team ID is rejected",
          addTeamRecord(teams, count, "Negative", -1, 500) == -1);
    check("Negative team budget is rejected",
          addTeamRecord(teams, count, "Debt", 6, -1) == -1);
    check("Empty team name is rejected",
          addTeamRecord(teams, count, "", 7, 500) == -1);
}

static void test_budget(void){
    struct Teams teams[MAX_TEAMS];
    struct Players players[MAX_PLAYERS];

    resetTeams(teams);
    resetPlayers(players);

    addTeamRecord(teams, 0, "Rich Team", 1, 100);
    addPlayerRecord(players, 0, "Player One", 1, "Batsman", 10);
    addPlayerRecord(players, 1, "Player Two", 2, "Bowler", 10);

    check("Bid greater than team budget is rejected",
          validateBid(teams, 1, 0, 101, 0, 10) == BID_OVER_BUDGET);
    check("Bid equal to team budget is accepted",
          validateBid(teams, 1, 0, 100, 0, 10) == BID_OK);

    check("Winning bid is deducted from team budget",
          processSale(players, 0, teams, 1, 1, 100) == 1 &&
          teams[0].Budget == 0);
    check("Winning bid increments team player count",
          teams[0].NumberOfPlayers == 1);
    check("Team budget never goes negative",
          processSale(players, 1, teams, 1, 1, 1) == 0 &&
          teams[0].Budget == 0);
}

static void test_bidding(void){
    struct Teams teams[MAX_TEAMS];

    resetTeams(teams);
    addTeamRecord(teams, 0, "Team One", 1, 500);
    addTeamRecord(teams, 1, "Full Team", 2, 500);
    teams[1].NumberOfPlayers = MAX_SQUAD;

    check("Bid from unknown team is rejected",
          validateBid(teams, 2, -1, 100, 0, 10) == BID_TEAM_NOT_FOUND);
    check("Bid from out-of-range team index is rejected",
          validateBid(teams, 2, 2, 100, 0, 10) == BID_TEAM_NOT_FOUND);
    check("Bid from full team is rejected",
          validateBid(teams, 2, 1, 100, 0, 10) == BID_TEAM_FULL);
    check("First bid below starting price is rejected",
          validateBid(teams, 2, 0, 9, 0, 10) == BID_BELOW_START);
    check("First bid equal to starting price is accepted",
          validateBid(teams, 2, 0, 10, 0, 10) == BID_OK);
    check("Bid not higher than current highest is rejected",
          validateBid(teams, 2, 0, 10, 10, 10) == BID_NOT_HIGHER);
    check("Bid higher than current highest is accepted",
          validateBid(teams, 2, 0, 11, 10, 10) == BID_OK);
    check("Bid over remaining budget is rejected",
          validateBid(teams, 2, 0, 501, 10, 10) == BID_OVER_BUDGET);
}

static void test_empty_auction(void){
    check("Auction cannot start with no players", !canStartAuction(0, 1));
    check("Auction cannot start with no teams", !canStartAuction(1, 0));
    check("Auction cannot start with too many players",
          !canStartAuction(MAX_PLAYERS + 1, 1));
    check("Auction cannot start with too many teams",
          !canStartAuction(1, MAX_TEAMS + 1));
    check("Auction can start with players and teams", canStartAuction(1, 1));

    StartAuction(0, 0);
    check("Empty auction returns safely", 1);
}

static void test_sold_unsold(void){
    struct Teams teams[MAX_TEAMS];
    struct Players players[MAX_PLAYERS];

    resetTeams(teams);
    resetPlayers(players);

    addTeamRecord(teams, 0, "Buyers", 1, 500);
    addPlayerRecord(players, 0, "Sold Player", 1, "Batsman", 50);
    addPlayerRecord(players, 1, "Unsold Player", 2, "Bowler", 50);

    check("New player can be auctioned", canAuctionPlayer(players, 0));
    check("Valid sale succeeds",
          processSale(players, 0, teams, 1, 1, 50) == 1);
    check("Sold player records team and price",
          players[0].sould == 1 &&
          players[0].souldToTeamID == 1 &&
          players[0].souldPrice == 50);
    check("Sold player cannot be auctioned again", !canAuctionPlayer(players, 0));
    check("Already-sold player cannot be sold twice",
          processSale(players, 0, teams, 1, 1, 60) == 0);
    check("Sale deducts budget and increments squad",
          teams[0].Budget == 450 && teams[0].NumberOfPlayers == 1);

    markUnsold(players, 1);
    check("Unsold player has no team or price",
          players[1].sould == 0 &&
          players[1].souldToTeamID == 0 &&
          players[1].souldPrice == 0);
    check("Unsold player can be auctioned later", canAuctionPlayer(players, 1));
    check("Sale to unknown team is rejected",
          processSale(players, 1, teams, 1, 999, 50) == 0);
    check("Sale below starting price is rejected",
          processSale(players, 1, teams, 1, 1, 49) == 0);

    markUnsold(players, 0);
    check("markUnsold resets a sold player",
          players[0].sould == 0 &&
          players[0].souldToTeamID == 0 &&
          players[0].souldPrice == 0 &&
          canAuctionPlayer(players, 0));
}

int main(void){
    test_players();
    test_teams();
    test_budget();
    test_bidding();
    test_empty_auction();
    test_sold_unsold();

    printf("\n%d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}
