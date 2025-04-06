#pragma once

struct moveStruct
{
    char piece; // 'p' for pawn, 'r' for rook, 'n' for knight, 'b' for bishop, 'q' for queen, 'k' for king
    char captured; // 'p' for pawn, 'r' for rook, 'n' for knight, 'b' for bishop, 'q' for queen, 'k' for king, '-' for none
    char color; // 'w' for white, 'b' for black
    int start[2]; // [row, column]
    int end[2]; // [row, column]
    char type; // 'm' for move, 'c' for castle, 'e' for en passant, 'r' for remove, 'a' for add, 'p' for promote, 'k' for playercapture
    char promotion; // 'q' for queen, 'r' for rook, 'b' for bishop, 'n' for knight
};
