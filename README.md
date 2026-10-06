# Auction TUI

A **Terminal User Interface (TUI) based auction management system written in C**.

The project is being built as a simple but structured simulation of a player auction, where teams have a fixed budget and compete to purchase players through bids.

The main goal of the project is not just to create an auction program, but to practice **C structures, functions, arrays, validation, program flow, and modular design** while building something interactive.

---

## Project Idea

The system simulates a player auction.

There are two main entities:

* **Teams** — have a team ID, name, and budget.
* **Players** — have a player ID, name, category, and starting price.

During the auction, teams can bid for players.

The system will:

* Register teams
* Register players
* Display the registered teams and players
* Conduct an auction for each player
* Validate bids
* Deduct the final winning amount from the winning team's budget
* Mark players as sold or unsold
* Keep track of which team purchased each player
* Display the final auction results and team summaries

---

## Planned Application Flow

The application will follow this general flow:

```text
                    AUCTION SYSTEM
                          │
                          ▼
                     Main Menu
                          │
             ┌────────────┼────────────┐
             ▼            ▼            ▼
       Team Management  Player       Auction
                        Management
             │            │            │
             ▼            ▼            ▼
         Add Teams     Add Players  Start Auction
         View Teams    View Players      │
         Search Teams  Search Players     ▼
                                  Player Auction
                                        │
                                        ▼
                                  Bid Validation
                                        │
                                        ▼
                                   Process Sale
                                        │
                                        ▼
                                  Auction Results
```

---

# Data Model

## Team

Each team will contain:

```text
Team
 ├── Team Name
 ├── Team ID
 ├── Budget
 └── Number of Players / Squad Information
```

The budget represents the amount of money the team currently has available for bidding.

---

## Player

Each player will contain:

```text
Player
 ├── Player Name
 ├── Player ID
 ├── Category
 ├── Starting Price
 ├── Sold / Unsold Status
 ├── Sold To Team
 └── Final Price
```

The additional auction-related information will allow the system to generate final results after the auction.

---

# Auction Logic

Each player will go through an individual auction.

The basic flow will be:

```text
Player Selected
      │
      ▼
Display Player Information
      │
      ▼
Start Bidding
      │
      ▼
Team Enters Bid
      │
      ▼
Validate Bid
      │
 ┌────┴─────┐
 │          │
Invalid     Valid
 │          │
 ▼          ▼
Reject    Highest Bid
             │
             ▼
       Continue Bidding
             │
             ▼
        Auction Ends
             │
       ┌─────┴─────┐
       │           │
   No Bids      Valid Bid
       │           │
       ▼           ▼
    UNSOLD       SOLD
                   │
                   ▼
             Process Sale
                   │
                   ▼
          Update Team Budget
```

---

# Bid Validation

A bid should only be accepted if it satisfies the auction rules.

The planned validation checks are:

## 1. Valid Team

The entered team ID must belong to a registered team.

```text
Team ID exists?

    ├── YES → Continue
    └── NO  → Reject bid
```

---

## 2. Sufficient Budget

The team must have enough remaining budget for the bid.

```text
Bid <= Team Budget
```

If the team does not have enough money, the bid is rejected.

---

## 3. Starting Price

The first valid bid must be at least the player's starting price.

```text
Bid >= Starting Price
```

---

## 4. Higher Than Current Bid

After bidding has started, every new bid must be greater than the current highest bid.

```text
New Bid > Current Highest Bid
```

---

## 5. Squad Limit

A team may eventually have a maximum number of players.

If the team has already reached its limit, it cannot purchase another player.

---

# Auction States

Each player can be considered to have one of three states:

```text
NOT AUCTIONED
      │
      ▼
UNDER AUCTION
      │
 ┌────┴────┐
 ▼         ▼
SOLD     UNSOLD
```

## SOLD

A player is sold when at least one valid bid is placed and the auction ends.

The system records:

```text
Sold = YES
Sold To = Team ID
Final Price = Winning Bid
```

The winning bid is then deducted from the team's budget.

---

## UNSOLD

If no valid bids are placed, the player remains unsold.

The system records:


```text
Sold = NO
Sold To = NONE
Final Price = 0
```




