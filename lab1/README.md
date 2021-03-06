# Lab1
### Data structures (\*\*\*\*)
Graphs based on adjacency structure and adjacency matrix, that can store a values of any data type in nodes and edges (`int`, `double`, `std::string`, `std::vector<>`).

**Methods implemented:**
- add node
- add edge
- find node by value
- delete node by index
- delete node by value
- delete edge
- check nodes adjacency
- check graph adjacency
- distance between two nodes (based on Floyd's algorithm)
- print

### Custom classes (\*\*\* + \*)
`Dice`:
A dice with face number N = 2, 4, 6, 8, 12, 20.
Every face has own probability of being dropped.

`DiceSet`:
A set of dices.

**Methods implemented:**
- count all possible sums in set (min and max)
- count probabilities for all possible sums in set
- count expected value of set

**Operators overloaded:**
- << for `std::ostream` and `Dice`
- << for `std::ostream` and `DiceSet`
- relational operators for two sets (comparing expected values)
