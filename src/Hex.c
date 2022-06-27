#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define OFFSET 16   // commonly 16 or 8
#define NIL '.'     // Empty byte

int main(int argc, char** argv){
    int i;
    unsigned long addr = 0;

    FILE* fp = fopen(argv[1], "rb");
    if (!fp){
        fprintf(stderr, "Could not open %s\n", argv[1]);
        exit(1);
    }
  
    int read;
    char buff[OFFSET] = "";
    while ((read = fread(buff, 1, sizeof(buff)+1, fp))){
        // Print address
        printf("%08x: ", addr);

        // Hex values
        // each byte is represented with a 2 digit hex code
        for (i = 0; i < OFFSET; ++i){
            printf("%02x",(unsigned char) buff[i]);
            if (i % 2 == 1){ putchar(' '); }
        }
        
        // Print ASCII representation or NIL if not representable as ASCII
        putchar('\t');
        for (i = 0; i < OFFSET; ++i){
            printf("%c", (buff[i] >= ' ' && buff[i] <= '~' ? buff[i] : NIL));
        }

        addr += OFFSET;
        putchar('\n');
    }
    
    fclose(fp);
}
