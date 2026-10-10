#include <stdio.h>
#include <string.h>

#define MAX_PLAYERS 10
#define MAX_BIDS 20

struct Bid
{
    char bidder[30];
    int amount;
};

struct Player
{
    char name[30];
    char category[30];
    int base_price;
    int highest_bid;
    char winner[30];

    struct Bid bids[MAX_BIDS];
    int bid_count;
};

void addPlayer(struct Player p[], int *n)
{
    if (*n >= MAX_PLAYERS)
    {
        printf("\nMaximum number of players reached!\n");
        return;
    }

    printf("\n========== ADD PLAYER ==========\n");

    printf("Player name: ");
    scanf("%s", p[*n].name);

    printf("Category: ");
    scanf("%s", p[*n].category);

    printf("Base price: ");
    scanf("%d", &p[*n].base_price);

    p[*n].highest_bid = p[*n].base_price;
    p[*n].bid_count = 0;
    strcpy(p[*n].winner, "Not Sold");

    printf("\nPlayer added successfully!\n");

    (*n)++;
}

void placeBid(struct Player p[], int n)
{
    int playerNo;
    char bidder[30];
    int amount;

    if (n == 0)
    {
        printf("\nNo players available for auction!\n");
        return;
    }

    printf("\n========== AVAILABLE PLAYERS ==========\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d. %s | Base Price: %d | Current Bid: %d\n",
               i + 1,
               p[i].name,
               p[i].base_price,
               p[i].highest_bid);
    }

    printf("\nSelect player number: ");
    scanf("%d", &playerNo);

    if (playerNo < 1 || playerNo > n)
    {
        printf("Invalid player number!\n");
        return;
    }

    playerNo--;

    if (p[playerNo].bid_count >= MAX_BIDS)
    {
        printf("Maximum bids reached for this player!\n");
        return;
    }

    printf("Enter bidder name: ");
    scanf("%s", bidder);

    printf("Enter bid amount: ");
    scanf("%d", &amount);

    if (amount <= p[playerNo].highest_bid)
    {
        printf("\nInvalid bid!\n");
        printf("Your bid must be greater than %d.\n",
               p[playerNo].highest_bid);
        return;
    }

    strcpy(p[playerNo].bids[p[playerNo].bid_count].bidder, bidder);
    p[playerNo].bids[p[playerNo].bid_count].amount = amount;

    p[playerNo].bid_count++;

    p[playerNo].highest_bid = amount;
    strcpy(p[playerNo].winner, bidder);

    printf("\nBid placed successfully!\n");
    printf("Current highest bid: %d\n", amount);
}

void displayPlayers(struct Player p[], int n)
{
    if (n == 0)
    {
        printf("\nNo players available!\n");
        return;
    }

    printf("\n========== AUCTION PLAYERS ==========\n");

    for (int i = 0; i < n; i++)
    {
        printf("\nPlayer %d", i + 1);
        printf("\nName          : %s", p[i].name);
        printf("\nCategory      : %s", p[i].category);
        printf("\nBase Price    : %d", p[i].base_price);
        printf("\nHighest Bid   : %d", p[i].highest_bid);
        printf("\nCurrent Winner: %s\n", p[i].winner);
    }
}

void showBidHistory(struct Player p[], int n)
{
    int playerNo;

    if (n == 0)
    {
        printf("\nNo players available!\n");
        return;
    }

    printf("\nSelect player to view bid history:\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d. %s\n", i + 1, p[i].name);
    }

    printf("Enter player number: ");
    scanf("%d", &playerNo);

    if (playerNo < 1 || playerNo > n)
    {
        printf("Invalid player number!\n");
        return;
    }

    playerNo--;

    printf("\n========== BID HISTORY ==========\n");

    if (p[playerNo].bid_count == 0)
    {
        printf("No bids placed yet.\n");
        return;
    }

    for (int i = 0; i < p[playerNo].bid_count; i++)
    {
        printf("%d. %s -> %d\n",
               i + 1,
               p[playerNo].bids[i].bidder,
               p[playerNo].bids[i].amount);
    }
}

void showResults(struct Player p[], int n)
{
    if (n == 0)
    {
        printf("\nNo auction results available!\n");
        return;
    }

    printf("\n========================================\n");
    printf("           FINAL AUCTION RESULTS\n");
    printf("========================================\n");

    for (int i = 0; i < n; i++)
    {
        printf("\nPlayer       : %s", p[i].name);
        printf("\nCategory     : %s", p[i].category);
        printf("\nBase Price   : %d", p[i].base_price);
        printf("\nFinal Bid    : %d", p[i].highest_bid);
        printf("\nWinner       : %s", p[i].winner);

        if (p[i].bid_count == 0)
        {
            printf("\nStatus       : UNSOLD\n");
        }
        else
        {
            printf("\nStatus       : SOLD\n");
        }

        printf("----------------------------------------\n");
    }
}

int main()
{
    struct Player players[MAX_PLAYERS];

    int n = 0;
    int choice;

    do
    {
        printf("\n\n========================================\n");
        printf("        ONLINE AUCTION SYSTEM\n");
        printf("========================================\n");

        printf("1. Add Player\n");
        printf("2. Place Bid\n");
        printf("3. View Players\n");
        printf("4. View Bid History\n");
        printf("5. View Auction Results\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addPlayer(players, &n);
                break;

            case 2:
                placeBid(players, n);
                break;

            case 3:
                displayPlayers(players, n);
                break;

            case 4:
                showBidHistory(players, n);
                break;

            case 5:
                showResults(players, n);
                break;

            case 6:
                printf("\nThank you for using the Online Auction System!\n");
                break;

            default:
                printf("\nInvalid choice! Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}