/*HEADER COMMENT*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"



#include <stdio.h>

/* Each key and value is stored in a  SymTableBinding. SymTableBindings 
are linked to form a list.  */
struct SymTableBinding
{
   /* The key. */
   const char *pcKey;

   void *pvValue;

   /* The address of the next SymTableBinding. */
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
      /*free(psCurrentBinding->pcKey);
      free(psCurrentBinding->pvValue);*/
      free(psCurrentBinding);
      /*potentially need to follow the value at the key?? like:
      free(psCurrentBinding->*pcKey);
      and then free the pointer
      but what if at the end of the key it's like a symtable or something?
      */
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

   if (SymTable_contains(oSymTable, pcKey)) return 0;

   psNewBinding = 
   (struct SymTableBinding*)malloc(sizeof(struct SymTableBinding));
   if (psNewBinding == NULL)
      return 0;

   psNewBinding->pcKey = pcKey;
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

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
      if (!strncmp(pcKey, psCurrentBinding->pcKey, strlen(pcKey))) {
         void *oldVal;
         oldVal = psCurrentBinding->pvValue;
         psCurrentBinding->pvValue = (void*) pvValue;
         return oldVal;
      }
   }
   return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) 
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
   
      if (psNextBinding != NULL && 
      !strncmp(pcKey, psCurrentBinding->pcKey, strlen(pcKey)))
      {
         void *oldVal;
         psCurrentBinding->psNextBinding = psNextBinding->psNextBinding;
         psCurrentBinding->pcKey = pcKey;
         oldVal = psNextBinding->pvValue;
         /*free(psNextBinding->pcKey);
         free(psNextBinding->pvValue);*/
         free(psNextBinding);
         
         return oldVal;
      }
   }
   return NULL;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
      printf("%s\n", pcKey);
      printf("%s\n", psCurrentBinding->pcKey);
      printf("%d\n", strcmp(pcKey, psCurrentBinding->pcKey));
      
      
      if (!strcmp(pcKey, psCurrentBinding->pcKey))
         /*psCurrentBinding->pcKey == (char*) pcKey) 
         !strncmp(pcKey, psCurrentBinding->pcKey, strlen(pcKey))) */{
         return (void*) psCurrentBinding->pvValue;
      }
   }
   return NULL;
   
   
   
   
   
   
   
   
   
   /*const void *pcKey;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);
   assert(oSymTable->psFirstBinding != NULL);

   pcKey = oSymTable->psFirstBinding->pcKey;
   psNextBinding = oSymTable->psFirstBinding->psNextBinding;
   free(oSymTable->psFirstBinding);
   oSymTable->psFirstBinding = psNextBinding;
   return (void*)pcKey;
*/
}

/*--------------------------------------------------------------------*/

int SymTable_contains(SymTable_T oSymTable, const char *pcKey)
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;

   assert(oSymTable != NULL);

   for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psNextBinding)
   {
      psNextBinding = psCurrentBinding->psNextBinding;
      if (!strcmp(psCurrentBinding->pcKey, pcKey)) {
         return 1;
      }
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

