
# Basic idea
1. pick a state where the opponent is to move
1. Check if all of the possible opponent moves result in a loss for him
	- It is impossible that one of the moves results in a win for the opponent, then the starting board would've been marked as a win as well.
1. if so. mark all reverse moves as win and then the board itself (in that order to avoid race condition with the above)


# Race conditions
(wrong)
for the algo to be correct: read(y) missing write(y), cannot be allowed to happen if read(x) received write(x)
1. solution: release write(y) acquire read(y)
2. writes need to be atomic or else write(y) can not happen entirely due to thread conflicts
3. read(x) can be non atomic because theres no way in fuck read(x) happens before read(y) in practice & also bits are never cleared, only set
	- Just try it for speed

# Pseudocode
```cpp
void singlePass() {
	for (U64 i = 0; i < 10000000; i += kingPermutations()) { // loop over entire state space
		std::array<U32, 25> wonBoards = ~tableBase[i]; // read(y)
		if (!wonBoards) // if resolved
			continue;

		auto board = indexToBoard(i); // board but you don't know which pieces are the king and you also dont know which cards are where

		usefulCards = findUsefulCards(wonBoards); // skip cards that have already been resolved
		for (auto move : or(usefulCards)) {
			for (auto newBoard : applyMoveToEachPiece(board, move)) {
				auto index = boardToIndex(newBoard);
				std::array<U32, 25>& kingPermutationBlock = tablebase[index]; // random access

				// "25" constexpr max amount of permutations. Sometimes could be 20 (~2% chance) or 16 (.04%). In total skips 9% of permutations
				// but with fixed 25, loop could be vectorized
				for (auto perm : kingPermutations()) {
					auto& lookup = kingPermutationBlock[perm]
					auto newPerm = countKingOvertakes(move, perm);
					for (auto slot : [0, 1])
						wonBoards[newPerm] &= shuffleBits<slot>(lookup, move) | ~applicableBits<slot>[move]; // read(x) check for forced loss
				}
			}
		}

		if (!wonBoards)
			continue

		for (auto move : or(reverse(usefulCards))) {
			for (auto newBoard : applyMoveToEachPiece(board, move)) {
				for (auto taking : [0, 1]) {
					auto index = boardToIndex(newBoard);
					std::array<U32, 25>& kingPermutationBlock = tablebase[index]; // random access
					for (auto perm : kingPermutations()) {
						auto& lookup = kingPermutationBlock[perm]
						auto newPerm = countKingOvertakes(move, perm);
						for (auto slot : [0, 1])
							lookup |= shuffleBits<slot>(wonBoards[newPerm], move) & applicableBits<slot>[move]; // write(y) having the option of winning
					}
				}
			}
		}

		for (auto perm : kingPermutations())
			tableBase[i] |= wonBoards[perm] // write(x) forced loss
	}
}
```