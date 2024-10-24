/*comment here describing interface*/

#include <stddef.h>

#ifndef SYMTAB_INCLUDED
#define SYMTAB_INCLUDED


/* A SymTable_T is a ........... FINISH!!*/
typedef struct SymTable *SymTable_T;

/*SymTable_new returns a new SymTable object that contains no bindings, 
or NULL if insufficient memory is available.*/
SymTable_T SymTable_new(void);

/*SymTable_free frees all memory occupied by oSymTable.*/
void SymTable_free(SymTable_T oSymTable);

/*SymTable_getLength returns the number of bindings in oSymTable.*/
size_t SymTable_getLength(SymTable_T oSymTable);

/**/
int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/**/
void *SymTable_replace(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue);

/**/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/**/
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/**/
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/**/
void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);


    /*If oSymTable does not contain a binding with key pcKey, then 
SymTable_put adds a new binding to oSymTable consisting of key pcKey 
and value pvValue and return 1 (TRUE). Otherwise the function leaves
 oSymTable unchanged and return 0 (FALSE). If insufficient memory is 
 available, then the function leaves oSymTable unchanged and 
 return 0 (FALSE).*/


 #endif