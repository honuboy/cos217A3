/*A symbol table is an unordered collection of bindings. 
A binding consists of a key and a value. A key is a string that uniquely
identifies its binding; a value is data that is somehow pertinent to 
its key. A symbol table, with these declarations allows the client 
to insert (put) new bindings, to retrieve (get) the values of bindings 
with specified keys, perform functions on all of the bindings (map)
handle (free) memory, and to remove bindings with specified keys.
This implementation specifically uses a linked list to construct
a symbol table.*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

/* Each key and value is stored in a SymTableBinding. SymTableBindings 
are linked to form a list.  */
struct SymTableBinding
{
   const char *pcKey;

   void *pvValue;

   struct SymTableBinding *psNextBinding;
};

/*--------------------------------------------------------------------*/

/* A SymTable is a "dummy" Binding that points to the first 
SymTableBinding. */
struct SymTable
{
   /* The address of the first SymTableBinding. */
   struct SymTableBinding *psFirstBinding;
};

/*--------------------------------------------------------------------*/

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;

   oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL)
      return NULL;

   oSymTable->psFirstBinding = NULL;
   return oSymTable;
}

/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
      free((void*)psCurrentBinding->pcKey);
      free(psCurrentBinding);
   }

   free(oSymTable);
}

/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable) 
{
   size_t n = 0;
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
      n++;
   }

   return n;
}

/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
{
   struct SymTableBinding *psNewBinding;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   if (SymTable_contains(oSymTable, pcKey)) return 0;

   psNewBinding = 
   (struct SymTableBinding*)malloc(sizeof(struct SymTableBinding));
   if (psNewBinding == NULL)
      return 0;

   psNewBinding->pcKey = (char *)malloc(strlen(pcKey) + 1);
   if (psNewBinding->pcKey == NULL)
      return 0;

   strcpy((char*)psNewBinding->pcKey, pcKey);
   psNewBinding->pvValue = (void*) pvValue;
   psNewBinding->psNextBinding = oSymTable->psFirstBinding;
   oSymTable->psFirstBinding = psNewBinding;
   return 1;
}

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) 
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
      if (!strcmp(pcKey, psCurrentBinding->pcKey)) {
         void *oldVal;
         oldVal = psCurrentBinding->pvValue;
         psCurrentBinding->pvValue = (void*) pvValue;
         return oldVal;
      }
   }
   return NULL;
}

/*--------------------------------------------------------------------*/

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) 
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNext;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   psCurrentBinding = oSymTable->psFirstBinding;
   if (psCurrentBinding == NULL) return NULL;
   psNext = psCurrentBinding->psNextBinding;

   if (!strcmp(psCurrentBinding->pcKey, pcKey)) {

      void *oldVal = psCurrentBinding->pvValue;

      oSymTable->psFirstBinding = psNext;

      free((char*) psCurrentBinding->pcKey);
      free(psCurrentBinding);

      return oldVal;
   }

   while (psCurrentBinding->psNextBinding != NULL) {
      psNext = psCurrentBinding->psNextBinding;
      if (!strcmp(psNext->pcKey, pcKey)) {
         void *oldVal = psNext->pvValue;

         if (psNext->psNextBinding == NULL) 
            psCurrentBinding->psNextBinding = NULL;
         else 
         psCurrentBinding->psNextBinding = psNext->psNextBinding;
         
         free((char*)psNext->pcKey);
         free(psNext);

         return oldVal;
      }
      psCurrentBinding = psCurrentBinding->psNextBinding;
   }

   return NULL;
}

/*--------------------------------------------------------------------*/

void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
      
      if (!strcmp(pcKey, psCurrentBinding->pcKey)) {
         return psCurrentBinding->pvValue;
      }
   }
   return NULL;
}

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
      if (!strcmp(psCurrentBinding->pcKey, pcKey)) return 1;
   }
   return 0;
}

/*--------------------------------------------------------------------*/

void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra)
{
   struct SymTableBinding *psCurrentBinding;

   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psCurrentBinding->psNextBinding)
      (*pfApply)((void*)psCurrentBinding->pcKey, 
      (void*) psCurrentBinding->pvValue, (void*)pvExtra);
   
}