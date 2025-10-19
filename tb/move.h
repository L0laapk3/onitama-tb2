#pragma once

#include "types.h"

// moves are U32's, centered around bit 0. Meaning to Move 1 spot, Move{2} would do that. Move{1 << 31} moves -1 spot.
using Move = U32;