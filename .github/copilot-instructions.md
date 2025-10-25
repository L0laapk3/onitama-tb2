# Onitama Tablebase Generator - AI Coding Agent Instructions

## Project Overview
This is a high-performance tablebase generator for the Onitama board game built with heavy compile-time metaprogramming and bit manipulation. The goal is to pre-compute all forced win/draw positions for any set of 5 cards chosen from 16 possible cards.

**Key constraints**: Generating 10-men tablebases (~800E9 entries) with ~128GB RAM through extreme performance optimization.

## Architecture & Data Structures

### Bitboard Representation
- Game state stored in **32-bit bitboards**: 5×5 board = 25 bits, centered at bit 12
- `Board` class: `std::array<U32, 2> bbp` (piece bitboards per player) + `std::array<int, 2> ik` (king indices)
- `Move<REVERSE>` template: 32-bit centered move patterns where `Move{2}` = 1 spot forward, `Move{1<<31}` = 1 spot backward
- Cards defined as compile-time `constexpr` move patterns (see `card.hpp` lines 21-37)

### Memory Layout: Nested Template Hierarchy
The tablebase uses a **4-level nested structure** parameterized by piece counts:

```cpp
Table<HALFMEN>                           // e.g., Table<3> = 6 total pieces
├─ FixedMenTable<P0,P1>                  // Fixed piece count per player (e.g., <2,3>)
│  └─ CombArray<Row<P0,P1>, 25, P0>     // P0 pieces on 25 squares
│     └─ Row<P0,P1>                      // = CombArray<Cell<P0,P1>, 25-P0, P1>
│        └─ Cell<P0,P1>                  // = std::array<U32, P0*P1> (king permutations × 30 card perms)
```

- `PIECECOUNTS<HALFMEN>` generates all (P0,P1) pairs at compile time, sorted by sum (see `table.h:33-44`)
- Each `Cell` stores 30 card permutation bitsets (which 2 cards each player has)
- `static_assert(sizeof(Table<3>) / sizeof(U32) * 30 == 1166670000)` validates memory layout

### Combinatorial Indexing
- **Combinatorial number system** converts bitboards ↔ array indices (see `rank.hpp`)
- `rank<REVERSE>(U32 bits)`: bitboard → index using binomial coefficients
- `unrank<REVERSE>(U32 index)`: index → bitboard via precomputed lookup tables (`UNRANK_LUT`)
- **Critical**: Lookups are LUT-based for speed; `LUT_N=25` covers full board size

### Card Permutations
- 30 valid card permutations stored in `CARD_PERMUTATIONS` array (cards dealt 2-2-1 across players+sideboard)
- `CARDS_USED_IN<PLAYER>[cardI]` bitmask: which of 30 permutations contain `cardI` for `PLAYER`
- `CardPreCalc` stores both forward and reverse card moves for bidirectional search

## Critical Performance Techniques

### 1. PDEP/PEXT Intrinsics (pdep.hpp)
```cpp
template<U32 MAX_MASK_UNSET_BITS>
inline U32 pdep(U32 src, U32 mask)
```
- Emulates `_pdep_u32` with optimized loops for **small unset bit counts** (beats microcode on Zen/Zen2)
- Used to reconstruct `bb1` from compressed indices: `pdep<P0>(index, ~bb0)`
- **Set `USE_PDEP = true`** to use intrinsics; fallback optimized for specific architectures

### 2. Template Metaprogramming
- **All table sizes computed at compile time**: `binomial(N,K)`, `fact()`, `PIECECOUNTS<HALFMEN>` (see `math.h`)
- `#pragma unroll` hints on loops with known small iteration counts (`iterateKingPerms`, card loops)
- `if constexpr` for player-specific logic avoids runtime branches

### 3. Iteration Strategy
- **Iterate only unresolved positions**: `if (*entry == (1U<<30)-1) continue;` skips resolved cells (see `generate.cpp:52-53`)
- Win-in-1 initialization marks ~65% of positions before main loop (`initializeWinIn1`)
- Algorithm propagates forced losses backward through reverse moves (see README pseudocode)

### 4. Memory Access Patterns
- Outer loops iterate over `FixedMenTable` → `Row` → `Cell` to maximize cache locality
- `iterateKingPerms<P0,P1>` unrolls king permutation loops (typically ≤5 iterations)

## Build System (Bazel)

### Commands
```bash
# Debug build (recommended for development)
bazel build --config=dbg tb

# Release build (for generation runs)
bazel run --config=rel tb
```

### Configuration (.bazelrc)
- **Compiler**: Clang 22 with LLD linker (`--action_env=CC=clang-22`)
- **Target**: `znver5` architecture (`-march=znver5`)
- **C++ standard**: C++26 (`-std=c++26`, enables `std::rotr`, constexpr steps=10M)
- **Release flags**: `-O3 -ffast-math -flto` with LLVM optimization remarks
- **Debug flags**: `-g -O0 -D_GLIBCXX_DEBUG` with assertions enabled

### Compile Commands
- Run `bazel run //:compdb` to generate `compile_commands.json` for LSP/clangd

## Development Patterns

### Template Discipline
- Always use `template<bool PLAYER>` for player-specific logic (not runtime `if (player)`)
- Use `!PLAYER` for opponent instead of `1-PLAYER` (works with bool template params)
- **Template functions cannot be defined in .cpp**: Move to .hpp or use explicit instantiation

### Const Correctness
- Mark lookup tables `constexpr`: `MOVE_MASK`, `PTEMPLE`, `ALL_CARDS`
- Use `const auto&` when accessing nested tables to avoid copies
- Member functions: `const` overloads delegate to non-const via `const_cast` (see `Table::get<P0,P1>`)

### Bit Manipulation Idioms
```cpp
while (bb0_p) {           // Loop over set bits
    int pos = std::countr_zero(bb0_p);
    bb0_p &= bb0_p - 1;   // Clear lowest set bit
}
```
- Use `std::popcount`, `std::countr_zero`, `std::rotr` (C++20) over manual loops
- **Centering**: Moves centered at bit 12 to avoid wrapping on 5×5 grid

### Assertions
- Heavy use of `assert()` in debug builds to validate indexing assumptions
- `static_assert` for compile-time size checks (e.g., `sizeof(Table<3>)`)

## Common Gotchas

1. **King vs Pieces**: Kings are always part of the bitboard; `iterateKingPerms` determines which piece is king
2. **Reverse moves**: Win-in-1 returns `Move<!PLAYER>` (opponent's move that creates the winning position)
3. **Temple occupancy**: Check `isTempleFree<PLAYER>()` before `kingTemple<PLAYER>()`—own pieces block temple wins
4. **Bitboard bounds**: 25 bits used but stored in 32-bit words; mask operations critical
5. **`pdep` template param**: `pdep<P0>` where `P0` is the number of **unset** bits expected in mask

## Testing & Validation

- Win-in-1 counts validated via `assert` (see `initializeWinIn1:39-41`):
  - `HALFMEN=1`: 5,181 positions
  - `HALFMEN=2`: 3,787,692 positions
  - `HALFMEN=3`: 537,541,377 positions
- No formal test suite; validation through exhaustive generation assertions

---
**Key Files**: `table.h` (hierarchy), `index.hpp` (king perms), `rank.hpp` (indexing), `card.hpp` (game rules), `.bazelrc` (build config)
