Simulate 3 types of elections from the terminal. 

# Usage

## Plurality Election
```
$ ./plurality Alice Bob Charlie
Number of voters: 4
Vote: Alice
Vote: Bob
Vote: Charlie
Vote: Alice
Alice
```

In this voting system, the candidate who obtains more votes than anyone else is the winner.

## Instant Runoff (Ranked Choice)
```
Number of voters: 5
Rank 1: Alice
Rank 2: Bob
Rank 3: Charlie

Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Bob
Rank 2: Alice
Rank 3: Charlie

Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

Alice
```

The candidate with more votes may not reflect the majority. In Ranked Choice, voters rank the candidates in order of preference and voting takes place in multiple rounds.
The candidate(s) with the least votes after a round is eliminated. Voters who chose that candidate will have their next choice considered in the next round.

## Tideman Election (Ranked Pairs)
```
$ ./tideman Alice Bob Charlie
Number of voters: 5
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

Charlie
```
This is an alternative to both Plurality and Instant Runoff elections and produces a Condorcet winner. Voters still rank candidates in order of preference. Candidates are considered in pairs. The winner is the one who would win against every other candidate in a head to head race.

This type of election produces a graph. An arrow (edge) is drawn from a candidate to one they win over. After all the edges are drawn, the candidate with only arrows pointing away and no arrows pointing towards them (the source) is the winner.

It's possible that a graph can be produced with no source - each candidate has an arrow pointing towards and also away from them (a cycle). To prevent this, each head-to-head race is ranked according to the strength of victory. The arrows (edges) are "locked in" in order of the strength of victory. If an edge would produce a cycle, it is not locked in.

The graph is represented as an [adjacency matrix](https://en.wikipedia.org/wiki/Adjacency_matrix).
