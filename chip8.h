#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include "chip8lib.h"

#define MEM_SIZE 4096
#define PRGM_START 0x200
#define ETI_PRGM_START 1536
#define BYTE uint8_t



BYTE m[MEM_SIZE]; // Memory array
uint16_t stack[16];
BYTE v[16]; // Register array
BYTE sp; // Stack pointer
uint16_t pc; // Program counter
BYTE dt; // Delay timer
BYTE st; // Sound timer
uint16_t i; // The "I" register, the only 16-bit register, usually for storing addresses
BYTE keys[16]; // The truth table for the keypad
