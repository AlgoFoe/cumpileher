#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

FILE *out;

// store variables
char variables[100][50];
int var_count = 0;

// temp counter
int temp_count = 1;

void init_codegen() {
    out = fopen("temp.asm", "w");
}

// store variable if not already present
void declare_variable(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i], name) == 0)
            return;
    }
    strcpy(variables[var_count++], name);
}

// check if value is number
int is_number(const char *str) {
    return (str[0] >= '0' && str[0] <= '9');
}

// MOV generation
void gen_mov(const char *var, const char *val) {
    declare_variable(var);

    if (is_number(val)) {
        fprintf(out, "    mov dword [%s], %s\n", var, val);
    } else {
        fprintf(out, "    mov eax, [%s]\n", val);
        fprintf(out, "    mov [%s], eax\n", var);
    }
}

// ADD generation
void gen_add(const char *res, const char *op1, const char *op2) {
    declare_variable(res);

    if (is_number(op1)) {
        fprintf(out, "    mov eax, %s\n", op1);
    } else {
        fprintf(out, "    mov eax, [%s]\n", op1);
    }

    if (is_number(op2)) {
        fprintf(out, "    add eax, %s\n", op2);
    } else {
        fprintf(out, "    add eax, [%s]\n", op2);
    }

    fprintf(out, "    mov [%s], eax\n", res);
}

void gen_print(const char *var) {
    fprintf(out, "    mov eax, [%s]\n", var);
    fprintf(out, "    call print_int\n");
}

// final assembly + build executable
void finish_codegen() {
    fclose(out);

    FILE *final = fopen("final.asm", "w");

    // DATA SECTION
    fprintf(final, "section .data\n");
    for (int i = 0; i < var_count; i++) {
        fprintf(final, "%s dd 0\n", variables[i]);
    }

    fprintf(final, "\nsection .text\n");
    fprintf(final, "global _start\n\n_start:\n");

    // copy instructions
    FILE *temp = fopen("temp.asm", "r");
    char ch;
    while ((ch = fgetc(temp)) != EOF) {
        fputc(ch, final);
    }
    fclose(temp);

    // EXIT syscall
    fprintf(final, "\n    mov eax, 1\n");
    fprintf(final, "    int 0x80\n");

    // print_int subroutine: receives value in eax, prints it followed by newline
    fprintf(final, "\nprint_int:\n");
    fprintf(final, "    push ebx\n");
    fprintf(final, "    push esi\n");
    fprintf(final, "    push edi\n");
    fprintf(final, "    sub esp, 13\n");
    fprintf(final, "    mov byte [esp+12], 10\n");   // newline at end of buffer
    fprintf(final, "    mov esi, eax\n");             // esi = value to print
    fprintf(final, "    mov edi, 11\n");              // edi = write index (fills buffer right-to-left)
    fprintf(final, "    mov ebx, 10\n");              // divisor
    fprintf(final, ".pi_loop:\n");
    fprintf(final, "    xor edx, edx\n");
    fprintf(final, "    mov eax, esi\n");
    fprintf(final, "    div ebx\n");                  // eax = quotient, edx = remainder
    fprintf(final, "    mov esi, eax\n");             // update remaining value
    fprintf(final, "    add dl, 48\n");               // dl = '0' + digit
    fprintf(final, "    mov [esp+edi], dl\n");
    fprintf(final, "    dec edi\n");
    fprintf(final, "    test esi, esi\n");
    fprintf(final, "    jnz .pi_loop\n");
    fprintf(final, "    inc edi\n");                  // edi now points to first digit
    fprintf(final, "    lea ecx, [esp+edi]\n");       // ecx = buffer start
    fprintf(final, "    mov edx, 13\n");
    fprintf(final, "    sub edx, edi\n");             // edx = length (digits + newline)
    fprintf(final, "    mov eax, 4\n");               // sys_write
    fprintf(final, "    mov ebx, 1\n");               // stdout
    fprintf(final, "    int 0x80\n");
    fprintf(final, "    add esp, 13\n");
    fprintf(final, "    pop edi\n");
    fprintf(final, "    pop esi\n");
    fprintf(final, "    pop ebx\n");
    fprintf(final, "    ret\n");

    fclose(final);

    
    printf("Assembling...\n");
    system("nasm -f elf32 final.asm -o output.o");
    
    printf("\nAssembly generated: final.asm\n");
    printf("Linking...\n");
    system("ld -m elf_i386 output.o -o program");

    printf("Executable generated: ./program\n");
}