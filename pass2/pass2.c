#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
int main() { 
    char opcode[20], operand[20], symbol[20], label[20], code[20], mnemonic[25]; 
    char add[20], objectcode[20]; 
    int flag, flag1, locctr, loc; 
    int startAddr = 0, progLen = 0; 
    FILE *fp1, *fp2, *fp3, *fp4; 
    fp1 = fopen("Out3.txt", "r"); 
    fp2 = fopen("Twoout.txt", "w"); 
    fp3 = fopen("Opcode.txt", "r"); 
    fp4 = fopen("Sym1.txt", "r"); 
    if (!fp1 || !fp2 || !fp3 || !fp4) { 
        printf("Error: One or more files could not be opened.\n"); 
        exit(1); 
    } 
    fscanf(fp1, "%s%s%s", label, opcode, operand); 
    if (strcmp(opcode, "START") == 0) { 
        startAddr = atoi(operand); 
        fprintf(fp2, "%s\t%s\t%s\n", label, opcode, operand); 
        fscanf(fp1, "%d%s%s%s", &locctr, label, opcode, operand); 
    } 
    char textRecord[500] = ""; 
    int textStart = locctr; 
    int textLen = 0; 
    while (strcmp(opcode, "END") != 0) { 
        flag = 0; 
        rewind(fp3); 
        while (fscanf(fp3, "%s%s", code, mnemonic) != EOF) { 
            if (strcmp(opcode, code) == 0) { 
                flag = 1; 
                break; 
            } 
        } 
        if (flag == 1) { 
            flag1 = 0; 
            rewind(fp4); 
            while (fscanf(fp4, "%s%d", symbol, &loc) != EOF) { 
                if (strcmp(symbol, operand) == 0) { 
                    flag1 = 1; 
                    break; 
                } 
            } 
            if (flag1 == 1) { 
                sprintf(add, "%04X", loc); 
                strcpy(objectcode, mnemonic); 
                strcat(objectcode, add); 
            } else { 
                strcpy(objectcode, mnemonic); 
                strcat(objectcode, "0000"); 
            } 
        } else if (strcmp(opcode, "BYTE") == 0) { 
            if (operand[0] == 'C') { 
                strcpy(objectcode, ""); 
                for (int i = 2; i < strlen(operand) - 1; i++) { 
                    char hex[3]; 
                    sprintf(hex, "%02X", operand[i]); 
                    strcat(objectcode, hex); 
                } 
            } else if (operand[0] == 'X') { 
                strncpy(objectcode, operand + 2, strlen(operand) - 3); 
                objectcode[strlen(operand) - 3] = '\0'; 
            } 
        } else if (strcmp(opcode, "WORD") == 0) sprintf(objectcode, "%06X", atoi(operand)); 
        else strcpy(objectcode, "-"); 
        fprintf(fp2, "%04X\t%s\t%s\t%s\t%s\n", locctr, label, opcode, operand, objectcode); 
        if (strcmp(objectcode, "-") != 0) { 
            strcat(textRecord, objectcode); 
            textLen += strlen(objectcode) / 2; 
        } 
        if (fscanf(fp1, "%d%s%s%s", &locctr, label, opcode, operand) == EOF) break; 
    } 
    fprintf(fp2, "\t%s\t%s\n", opcode, operand); 
    progLen = locctr - startAddr; 
    fprintf(fp2, "\nH^%s^%06X^%06X\n", label, startAddr, progLen); 
    fprintf(fp2, "T^%06X^%02X^%s\n", textStart, textLen, textRecord); 
    fprintf(fp2, "E^%06X\n", startAddr); 
    fclose(fp1); 
    fclose(fp2); 
    fclose(fp3); 
    fclose(fp4); 
    printf("Pass 2 Assembler completed with H T E records. Output written to twoout.txt\n"); 
    return 0;
}