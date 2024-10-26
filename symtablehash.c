/*HEADER COMMENT*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"


static size_t abucketCount[] = 
   {509, 1021, 2039, 4093, 8191, 16381, 32749, 65521};

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
   int bucketLevel;

   size_t bucketCount;

   /* The address of the first SymTableBinding. */
   struct SymTableBinding **psBuckets;
};

/*--------------------------------------------------------------------*/

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
   inclusive. */
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

/*--------------------------------------------------------------------*/

/*SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;

   oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL)
      return NULL;

   oSymTable->psFirstBinding = NULL;
   return oSymTable;
}*/

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;
   size_t hashNum;

   /*if i were to guess, we allocate uBucket[0] * sizeof() initially
   and maybe at each interval we have a pointer?? idk. like a symtable!
   then in the put function we'll have to hash stuff out*/

   oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL) return NULL;

   oSymTable->psBuckets = 
   malloc(sizeof(struct SymTableBinding) * abucketCount[0]);
   if (*oSymTable->psBuckets == NULL) return NULL;

   for (hashNum = 0; hashNum < abucketCount[0]; hashNum++) {
      oSymTable->psBuckets[hashNum]->psNextBinding = NULL;
   }

   oSymTable->bucketLevel = 0;

   return oSymTable;
}



/*--------------------------------------------------------------------*/

void SymTable_free(SymTable_T oSymTable)
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;
   size_t hashNum;

   assert(oSymTable != NULL);
   
   for (hashNum = 0; 
         hashNum < abucketCount[oSymTable->bucketLevel]; 
         hashNum++) 
   {
      for (psCurrentBinding = 
            oSymTable->psBuckets[hashNum]->psNextBinding;
            psCurrentBinding != NULL;
            psCurrentBinding = psNextBinding)
      {
         psNextBinding = psCurrentBinding->psNextBinding;
         free((void*)psCurrentBinding->pcKey);
         free(psCurrentBinding);
      }
   }

   free(oSymTable);
}

/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable) 
{
   assert(oSymTable != NULL);
   return abucketCount[oSymTable->bucketLevel];
}

/*--------------------------------------------------------------------*/

int SymTable_put(SymTable_T oSymTable,
     const char *pcKey, const void *pvValue)
{
   struct SymTableBinding *psNewBinding;
   size_t hashNum;

   assert(oSymTable != NULL);

   if (SymTable_contains(oSymTable, pcKey)) return 0;

   oSymTable->bucketCount++;

   psNewBinding = 
   (struct SymTableBinding*)malloc(sizeof(struct SymTableBinding));
   if (psNewBinding == NULL)
      return 0;

   psNewBinding->pcKey = (char *)malloc(strlen(pcKey) + 1);
   if (psNewBinding->pcKey == NULL)
      return 0;

   strcpy((char*)psNewBinding->pcKey, pcKey);
   psNewBinding->pvValue = (void*) pvValue;
   psNewBinding->psNextBinding = NULL;


   /*some big if statement here for expansion!
   need to change bucket count, bucket level, redish out everything, wow*/

   hashNum = 
   SymTable_hash(pcKey, abucketCount[oSymTable->bucketLevel]);


   if ((oSymTable->psBuckets[hashNum]->psNextBinding == NULL))
   oSymTable->psBuckets[hashNum]->psNextBinding = psNewBinding;
   else psNewBinding->psNextBinding = 
      oSymTable->psBuckets[hashNum]->psNextBinding;
   oSymTable->psBuckets[hashNum]->psNextBinding = psNewBinding;
   /*if something is there already, we need to move stuff around and put it
   in 1st place, and set it's next to the thing that was there*/
   /*psNewBinding->psNextBinding = oSymTable->psFirstBinding;
   oSymTable->psFirstBinding = psNewBinding;*/
   return 1;
}

/*--------------------------------------------------------------------*/

void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue) 
{
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;
   size_t hashNum;

   assert(oSymTable != NULL);

   hashNum = 
   SymTable_hash(pcKey, abucketCount[oSymTable->bucketLevel]);

   for (psCurrentBinding = oSymTable->psBuckets[hashNum]->psNextBinding;
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
   size_t hashNum;

   assert(oSymTable != NULL);

   hashNum = 
   SymTable_hash(pcKey, abucketCount[oSymTable->bucketLevel]);

   psCurrentBinding = oSymTable->psBuckets[hashNum]->psNextBinding;
   if (psCurrentBinding == NULL) return NULL;
   psNext = psCurrentBinding->psNextBinding;

   /*if the first binding is what is to be removed, we need the
   rest of the list to be together still*/
   if (!strcmp(psCurrentBinding->pcKey, pcKey)) {

      void *oldVal = psCurrentBinding->pvValue;

      oSymTable->psBuckets[hashNum]->psNextBinding = psNext;

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
   size_t hashNum;

   assert(oSymTable != NULL);

   hashNum = 
   SymTable_hash(pcKey, abucketCount[oSymTable->bucketLevel]);

   for (psCurrentBinding = oSymTable->psBuckets[hashNum]->psNextBinding;
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
   size_t hashNum;

   assert(oSymTable != NULL);

   hashNum = 
   SymTable_hash(pcKey, abucketCount[oSymTable->bucketLevel]);

   for (psCurrentBinding = oSymTable->psBuckets[hashNum]->psNextBinding;
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
   size_t hashNum;

   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   for (hashNum = 0; 
         hashNum < abucketCount[oSymTable->bucketLevel]; 
         hashNum++) 
   {
      for (psCurrentBinding = 
            oSymTable->psBuckets[hashNum]->psNextBinding;
            psCurrentBinding != NULL;
            psCurrentBinding = psCurrentBinding->psNextBinding)
      {
         (*pfApply)((void*)psCurrentBinding->pcKey, 
         (void*) psCurrentBinding->pvValue, (void*)pvExtra);
      }
   }
}