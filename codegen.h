#ifndef CODEGEN_H
#define CODEGEN_H

void init_codegen();
void declare_variable(const char *name);

void gen_mov(const char *var, const char *val);
void gen_add(const char *res, const char *op1, const char *op2);
void gen_print(const char *var);

void finish_codegen();

#endif