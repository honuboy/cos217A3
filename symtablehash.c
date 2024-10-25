/*HEADER COMMENT*/
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

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
      free((void*)psCurrentBinding->pcKey);
      /*free(psCurrentBinding->pvValue);*/
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
   /*maybe just some global variable to dictate the index of array*/



/*fill correctly*/
   return 1;
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

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) 
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNext;

   assert(oSymTable != NULL);

   /*for (psCurrentBinding = oSymTable->psFirstBinding;
        psCurrentBinding != NULL;
        psCurrentBinding = psCurrentBinding->psNextBinding)
   {
      psNext = psCurrentBinding->psNextBinding;

      /*if (psNext != NULL)
      printf("%s 1 \n", (char*) psNext->pvValue);*/
/*
      if (!strcmp(pcKey, psNext->pcKey))
      {
         void *oldVal = psNext->pvValue;

         if (psNext->psNextBinding == NULL) 
            psCurrentBinding->psNextBinding = NULL;
         else psCurrentBinding->psNextBinding = psNext->psNextBinding;
         /*psNext = psNext->psNextBinding;*/
         
         /*free(psNextBinding->pcKey);
         free(psNextBinding->pvValue);*/
         
         /*free(psNext);*/
         /*
         printf("%s 2 \n", (char*) oldVal);

         return oldVal;

         
      }
   }*/

  if (!SymTable_contains(oSymTable, pcKey)) return NULL;

   psCurrentBinding = oSymTable->psFirstBinding;
   if (psCurrentBinding->psNextBinding != NULL)
   psNext = psCurrentBinding->psNextBinding;
   else psNext = NULL;

   if (!strcmp(psCurrentBinding->pcKey, pcKey)) {
      if (psNext != NULL)
      oSymTable->psFirstBinding = psNext;
      else oSymTable->psFirstBinding = NULL;
      /*some free stuff*/

      return psCurrentBinding->pvValue;
   }

   while (psCurrentBinding->psNextBinding != NULL) {
      psNext = psCurrentBinding->psNextBinding;
      if (!strcmp(psNext->pcKey, pcKey)) {
         void *oldVal = psNext->pvValue;
         if (psNext->psNextBinding == NULL) 
            psCurrentBinding->psNextBinding = NULL;
         else 
         psCurrentBinding->psNextBinding = psNext->psNextBinding;
         /*something with a free function here*/
         return oldVal;
      }
      psCurrentBinding = psCurrentBinding->psNextBinding;
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
      
      if (!strcmp(pcKey, psCurrentBinding->pcKey)) {
         return psCurrentBinding->pvValue;
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


static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}