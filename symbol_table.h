#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

void set_variable(const char* name, int value);
int get_variable(const char* name);
void reset_symbol_table();  

#endif
