#include "chip8.h"

int main(char** args,int argsv) {
    BYTE* prgm = m + PRGM_START;
    int file = open("file.ch8",O_RDONLY);
    int n_read = read(file,prgm,256);
    close(file);
    printf("FILE: %d\r\n",file);
    printf("NREAD: %d\r\n",n_read);
    pc = PRGM_START; // Set the program counter to the starting address of the program

    BYTE x; // Used often inside of FaE
    BYTE y; 


    // Initialize the keypad for use! (all 0)
    for(x=0;x<16;x++)
        keys[x] = 0;

    uint16_t sum;
    // Fetch and execute cycle
    for(;;) {
        uint16_t op = (prgm[pc++] << 8) | prgm[pc++];
        printf("0x%04X\r\n",op);
        switch(op & 0xF000) {
            case 0x0000:
                switch(op & 0x00FF) {
                    case 0x00E0: //CLS
                        // TODO CLS
                        break;
                    case 0x00EE: // RET
                        pc = stack[(--sp)];
                        break;
                }
                break;
            case 0x1000: // JP addr
                pc = op & 0x0FFF;
                break;
            case 0x2000: // CALL addr
                stack[sp++] = pc;
                pc = op & 0x0FFF;
                break;
            case 0x3000: // SE Vx, byte (Skip next instruction if Vx == byte)
                if(v[(op & 0x0F00) >> 8] == op & 0x00FF)
                    pc+=2;
                break;
            case 0x4000: // SNE Vx, byte
                if(v[(op & 0x0F00) >> 8] != op & 0x00FF)
                    pc+=2;
                break;
            case 0x5000: // SE Vx, Vy (0)
                if(v[(op & 0x0F00) >> 8] == v[(op & 0x0F0) >> 4])
                    pc+=2;
                break;
            case 0x6000: // LD Vx, byte
                v[(op & 0x0F00) >> 8] = op & 0x00FF;
                break;
            case 0x7000: // ADD Vx, byte
                v[(op & 0x0F00) >> 8] += op & 0x00FF;
                break;
            case 0x8000:
                switch (op & 0x000F) {
                    case 0x0000: // Store Vy in Vx
                        v[(op & 0x0F00) >> 8] = v[(op & 0x0F0) >> 4];
                        break;
                    case 0x0001: // OR Vx Vy
                        v[(op & 0x0F00) >> 8] |= v[(op & 0x0F0) >> 4];
                        break;
                    case 0x0002: // AND Vx Vy
                        v[(op & 0x0F00) >> 8] &= v[(op & 0x0F0) >> 4];
                        break;
                    case 0x0003: // XOR Vx Vy
                        v[(op & 0x0F00) >> 8] ^= v[(op & 0x0F0) >> 4];
                        break;
                    case 0x0004: // ADD Vx Vy
                        x = (op & 0x0F00) >> 8; // We need it twice, but why do it twice?
                        sum = v[x] + v[(op & 0x0F0) >> 4];
                        v[0xF] = sum > 255; // Carry flag
                        v[x] = (sum & 0xFF);
                        break;
                    case 0x0005: // SUB Vx Vy
                        x = (op & 0x0F00) >> 8; // We need it twice, but why do it twice?
                        y = (op & 0x0F0) >> 4;
                        sum = v[x] - v[y];
                        v[0xF] = v[x] > v[y]; // Carry flag
                        v[x] = (sum & 0xFF);
                        break;
                    case 0x0006: // SHIFTR Vx
                        x = (op & 0x0F00) >> 8; // We need it twice, but why do it twice?
                        v[0xF] = v[x] & 0x0001;
                        v[x] = v[x] >> 1;
                        break;
                    case 0x0007: // SUBN Vx Vy
                        x = (op & 0x0F00) >> 8; // We need it twice, but why do it twice?
                        y = (op & 0x0F0) >> 4;
                        sum = v[x] - v[y];
                        v[0xF] = v[x] < v[y]; // Carry flag
                        v[x] = (sum & 0xFF);
                        break;
                    case 0x0008: // SHIFTL Vx
                        x = (op & 0x0F00) >> 8; // We need it twice, but why do it twice?
                        v[0xF] = (v[x] & 0xC0) >> 6;
                        v[x] = v[x] << 1;
                        break;
                }
                break;
            case 0x9000: // SNE Vx, Vy (0)
                if(v[(op & 0x0F00) >> 8] != v[(op & 0x0F0) >> 4])
                    pc+=2;
                break;
            case 0xA000: // LD I, nnn
                i = (op & 0x0FFF);
                break;
            case 0xB000: // JP nnn + V0
                pc = (op & 0x0FFF) + v[0];
                break;
            case 0xC000: // RND Vx kk
                x = (op & 0x0F00) >> 8;
                v[x] = random_at_most(255) & (op & 0x00FF);
                break;
            case 0xD000: // DRW Vx, Vy, nibble ---- Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
                // TODO: Will do this later. Probably just a memcopy into a screen "array"
                // Then when the graphics update timer has gone to 0 (still not sure how to implement) draw the screen from the array.
                // OR if feeling sophisticated, only draw the changes.
                break;
            case 0xE000:
                switch(op & 0x00FF) {
                    case 0x009E: // SKE Vx, Key down ---- Skip next instruction if Key in Vx is down
                        if(keys[v[(op & 0x0F00) >> 8]] != 0)
                            pc+=2;
                        break;
                    case 0x00A1:
                        if(keys[v[(op & 0x0F00) >> 8]] -= 0)
                            pc+=2;
                        break;

                }
                break;
        } // End total switch
    } // End Fetch and Execute loop

    return 0;
} // End main, duh.



