/*A symbol table is an unordered collection of bindings. 
A binding consists of a key and a value. A key is a string that uniquely
identifies its binding; a value is data that is somehow pertinent to 
its key. A symbol table, with these declarations allows the client 
to insert (put) new bindings, to retrieve (get) the values of bindings 
with specified keys, perform functions on all of the bindings (map)
handle (free) memory, and to remove bindings with specified keys.*/

#include <stddef.h>

#ifndef SYMTAB_INCLUDED
#define SYMTAB_INCLUDED

/* A SymTable_T is a pointer to a SymTable ADT*/
typedef struct SymTable *SymTable_T;

/*SymTable_new returns a new SymTable object that contains no bindings, 
or NULL if insufficient memory is available.*/
SymTable_T SymTable_new(void);

/*SymTable_free frees all memory occupied by oSymTable.*/
void SymTable_free(SymTable_T oSymTable);

/*SymTable_getLength returns the number of bindings in oSymTable.*/
size_t SymTable_getLength(SymTable_T oSymTable);

/*If oSymTable does not contain a binding with key pcKey, then 
SymTable_put adds a new binding to oSymTable consisting of key pcKey 
and value pvValue and return 1 (TRUE). Otherwise the function leaves
oSymTable unchanged and return 0 (FALSE). If insufficient memory is 
available, then the function leaves oSymTable unchanged and 
return 0 (FALSE).*/
int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*SymTable_replace replaces and returns a pointer to the old value
associated with the key pcKey in oSymTable, and inserts pvValue, 
the new value pointer*/
void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/*SymTable_contains returns 1 if oSymTable contains the key pcKey,
and a 0 if not*/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*SymTable_get retrieves and returns the pointer to the value associated
with key pcKey within oSymTable*/
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*SymTable_remove removes the binding associated with pcKey within
oSymTable, and returns the value pointer that was associated with 
key pcKey. If oSymTable does not contain pcKey, the function 
returns NULL*/
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*SymTable_map applies the function *pfApply to each binding in 
oSymTable, passing pvExtra as an extra parameter. That is, 
the function calls (*pfApply)(pcKey, pvValue, pvExtra) 
for each pcKey/pvValue binding in oSymTable.*/
void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);

 #endif