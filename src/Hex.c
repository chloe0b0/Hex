#include <stdio.h>
#include <time.h>
#include <string.h>

#define OFFSET 16       // commonly 16 or 8
#define NIL '.'         // Non-ASCII byte
#define OCTETS_PER 2    // Octets per group
#define CHAR_PER_OCTET 2 
#define ROW_SIZE 9 + (CHAR_PER_OCTET*OFFSET) + (OFFSET/OCTETS_PER) + OFFSET + 5000

int main(int argc, char** argv){
    // 9 chars for addr + char_per_octet*octets + spaces + ASCII_characters + 1 (Null Terminate)
    // const size_t ROW_SIZE = 9 + (CHAR_PER_OCTET*OFFSET) + (OFFSET/OCTETS_PER) + OFFSET + 1;
    
    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL){
        fprintf(stderr, "Could not open %s\n", argv[1]);
        return 1;
    }
    
    int read, i;
    char buff[OFFSET] = "";
    unsigned long addr = 0;
    clock_t t = clock();
    size_t ix = 0;
    while ((read = fread(buff, 1, sizeof(buff), fp))){
        char row[ROW_SIZE] = "";
        // Print address
        char addr_str[11];
        sprintf(addr_str, "%08x: ", addr);
        strcat(row, addr_str);
        ix += 10;
        // Hex values
        // each byte is represented with a 2 digit hex code
        for (i = 0; i < OFFSET; ++i){
            if (i >= read){ row[ix++] = ' '; }
            else{
                char hex_str[CHAR_PER_OCTET];
                sprintf(hex_str, "%02x",(unsigned char) buff[i]);
                strcat(row, hex_str);
                ix+=CHAR_PER_OCTET;
            }
            if ((i + 1) % OCTETS_PER == 0){ row[ix++] = ' '; }
        }
        
        row[ix++] = '\t';
        if (OFFSET - read) row[ix++] = '\t'; 
        // Print ASCII representation or NIL if not representable as ASCII
        for (i = 0; i < read; ++i){
            row[ix++] = (buff[i] >= ' ' && buff[i] <= '~' ? buff[i] : NIL);
        }
        
        fputs(row, stdout);
        addr += OFFSET;
        putchar('\n');
        ix = 0;
    }
    t = clock() - t;
    printf("%f \n", ((double)t)/CLOCKS_PER_SEC);
    fclose(fp);
}
