/*HEADER COMMENT*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"





#include <stdio.h>



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
   struct SymTableBinding *psFirstBucket;
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

SymTable_T SymTable_new(void)
{
   SymTable_T oSymTable;
   size_t hashNum;

   oSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymTable == NULL) return NULL;

   oSymTable->bucketLevel = 0;
   oSymTable->bucketCount = 0;

   oSymTable->psFirstBucket = (struct SymTableBinding *) 
   malloc(sizeof(struct SymTableBinding) * abucketCount[0]);
   if (oSymTable->psFirstBucket == NULL) {
      free(oSymTable);
      return NULL;
   }





   /*do i need the following instances of resetting!???*/










   for (hashNum = 0; hashNum < abucketCount[0]; hashNum++) {
      (oSymTable->psFirstBucket + hashNum)->psNextBinding = NULL;
   }

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
            (oSymTable->psFirstBucket + hashNum)->psNextBinding;
            psCurrentBinding != NULL;
            psCurrentBinding = psNextBinding)
      {
         psNextBinding = psCurrentBinding->psNextBinding;
         free((void*)psCurrentBinding->pcKey);
         free(psCurrentBinding);
      }
   }

   free(oSymTable->psFirstBucket);
   free(oSymTable);
}

/*--------------------------------------------------------------------*/

size_t SymTable_getLength(SymTable_T oSymTable) 
{
   assert(oSymTable != NULL);
   return oSymTable->bucketCount;
}

/*--------------------------------------------------------------------*/

/*this function


FUNCTION COMMENT



*/
static SymTable_T SymTable_rehash(SymTable_T oSymTable)
{
   SymTable_T nSymTable;
   struct SymTableBinding *psCurrentBinding;
   struct SymTableBinding *psNextBinding;
   size_t hashNum;
   size_t rehashNum;

   assert(oSymTable != NULL);
   

   nSymTable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (nSymTable == NULL) return NULL;

   nSymTable->bucketLevel = oSymTable->bucketLevel + 1;
   nSymTable->bucketCount = oSymTable->bucketCount;

   nSymTable->psFirstBucket = (struct SymTableBinding *) 
   malloc(sizeof(struct SymTableBinding) * 
   abucketCount[nSymTable->bucketLevel]);
   if (nSymTable->psFirstBucket == NULL) {
      free(nSymTable);
      return NULL;
   } 

   for (hashNum = 0; hashNum < abucketCount[nSymTable->bucketLevel];
    hashNum++) {
      (nSymTable->psFirstBucket + hashNum)->psNextBinding = NULL;
   }

   for (hashNum = 0; 
         hashNum < abucketCount[oSymTable->bucketLevel]; hashNum++) 
      {
         
         
         
         
         
         psCurrentBinding = 
            (oSymTable->psFirstBucket + hashNum)->psNextBinding;

         while (psCurrentBinding != NULL) {
            psNextBinding = psCurrentBinding->psNextBinding;
            rehashNum = 
               SymTable_hash(psCurrentBinding->pcKey, 
               abucketCount[nSymTable->bucketLevel]);

            psCurrentBinding->psNextBinding = 
               (nSymTable->psFirstBucket + rehashNum)->psNextBinding;
            (nSymTable->psFirstBucket + rehashNum)->psNextBinding = 
               psCurrentBinding;

            psCurrentBinding = psNextBinding;
         }

      /*
         for (psCurrentBinding = 
            (oSymTable->psFirstBucket + hashNum)->psNextBinding;
            psCurrentBinding != NULL;
            psCurrentBinding = psNextBinding)
      {
         psNextBinding = psCurrentBinding->psNextBinding;
         rehashNum = 
         SymTable_hash(psCurrentBinding->pcKey, 
         abucketCount[nSymTable->bucketLevel]);

         psCurrentBinding->psNextBinding = 
         (nSymTable->psFirstBucket + rehashNum)->psNextBinding;
         (nSymTable->psFirstBucket + rehashNum)->psNextBinding = 
         psCurrentBinding;
      }
      */
      }

   /*free(oSymTable->psFirstBucket);
   free(oSymTable);*/
   return nSymTable;
}


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

   hashNum = 
   SymTable_hash(pcKey, abucketCount[oSymTable->bucketLevel]);

   psNewBinding->psNextBinding =
   (oSymTable->psFirstBucket + hashNum)->psNextBinding;
   (oSymTable->psFirstBucket + hashNum)->psNextBinding = psNewBinding;

   if ((oSymTable->bucketCount > abucketCount[oSymTable->bucketLevel]) 
         && oSymTable->bucketLevel != 7) {
            SymTable_T dummyST = SymTable_rehash(oSymTable);
            if (dummyST != NULL)
            oSymTable = dummyST;
         }

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

   for (psCurrentBinding = (oSymTable->psFirstBucket + hashNum)->psNextBinding;
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

   psCurrentBinding = 
      (oSymTable->psFirstBucket + hashNum)->psNextBinding;
   if (psCurrentBinding == NULL) return NULL;
   psNext = psCurrentBinding->psNextBinding;

   /*if the first binding is what is to be removed, we need the
   rest of the list to be together still*/
   if (!strcmp(psCurrentBinding->pcKey, pcKey)) {

      void *oldVal = psCurrentBinding->pvValue;

      (oSymTable->psFirstBucket + hashNum)->psNextBinding = psNext;

      free((char*) psCurrentBinding->pcKey);
      free(psCurrentBinding);

      oSymTable->bucketCount--;

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

         oSymTable->bucketCount--;

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

   for (psCurrentBinding = 
         (oSymTable->psFirstBucket + hashNum)->psNextBinding;
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

   for (psCurrentBinding = (oSymTable->psFirstBucket + hashNum)->psNextBinding;
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
            (oSymTable->psFirstBucket + hashNum)->psNextBinding;
            psCurrentBinding != NULL;
            psCurrentBinding = psCurrentBinding->psNextBinding)
      {
         (*pfApply)((void*)psCurrentBinding->pcKey, 
         (void*) psCurrentBinding->pvValue, (void*)pvExtra);
      }
   }
}