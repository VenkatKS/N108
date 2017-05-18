// filename *************************heap.c ************************
// Implements memory heap for dynamic memory allocation.
// Follows standard malloc/calloc/realloc/free interface
// for allocating/unallocating memory.

// Jacob Egner 2008-07-31
// modified 8/31/08 Jonathan Valvano for style
// modified 12/16/11 Jonathan Valvano for 32-bit machine

/* This example accompanies the book
   Embedded Systems: Real-Time Operating Systems for the Arm Cortex-M3, Volume 3,  
   ISBN: 978-1466468863, Jonathan Valvano, copyright (c) 2012

   Program xx.x, section xx.x

 Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains

 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// Implementation Notes:
// This is a Knuth Heap. Each block has a header and a trailer, which I shall
// call the meta-sections.  The meta-sections are each a single long that tells
// how many longs/words can be stored between the header and trailer.
// If the block is used, the meta-sections record the room as a positive
// number.  If the block is unused, the meta-sections record the room as a
// negative number.

#include "heap.h"

#define HEAP_START (Heap)
#define HEAP_END (HEAP_START + HEAP_SIZE_WORDS)

//The actual heap is just a big array.
static long Heap[HEAP_SIZE_WORDS];

static long inHeapRange(long* address);
static long blockUsed(long* block);
static long blockUnused(long* block);
static long blockRoom(long* block);
//static long blockSize(long* block);
static long* blockHeader(long* blockEnd);
static long* blockTrailer(long* blockStart);
static long* nextBlockHeader(long* blockStart);
static long* previousBlockHeader(long* blockStart);
static long markBlockUsed(long* blockStart);
static long markBlockUnused(long* blockStart);
static long splitAndMarkBlockUsed(long* upperBlockStart, long desiredRoom);
static void mergeBlockWithBelow(long* upperBlockStart);
//static long byteIndex(long* ptr);

//******** Heap_Init *************** 
// Initialize the Heap
// input: none
// output: always HEAP_OK
// notes: Initializes/resets the heap to a clean state where no memory
//  is allocated.
long Heap_Init(void){
  long* blockStart = HEAP_START;
  long* blockEnd = (HEAP_START + HEAP_SIZE_WORDS - 1);
  *blockStart = -(long)(HEAP_SIZE_WORDS - 2);  
  *blockEnd = -(long)(HEAP_SIZE_WORDS - 2);
  return HEAP_OK;
}


//******** Heap_Malloc *************** 
// Allocate memory, data not initialized
// input: 
//   desiredBytes: desired number of bytes to allocate
// output: void* pointing to the allocated memory or will return NULL
//   if there isn't sufficient space to satisfy allocation request
void* Heap_Malloc(long desiredBytes){
  long desiredWords = (desiredBytes + sizeof(long) - 1) / sizeof(long);
  long* blockStart = HEAP_START;  // implements first fit
  if(desiredWords <= 0){
    return 0; //NULL
  }
  while(inHeapRange(blockStart)){
  // one pass through the heap
  // choose first block that is big enough
    if(blockUnused(blockStart) && desiredWords <= blockRoom(blockStart)){
      if(splitAndMarkBlockUsed(blockStart, desiredWords)){
        return 0; //NULL
      }
      return blockStart + 1;
    }
    blockStart = nextBlockHeader(blockStart);
  }
  return 0; //NULL
}


//******** Heap_Calloc *************** 
// Allocate memory, data are initialized to 0
// input:
//   desiredBytes: desired number of bytes to allocate
// output: void* pointing to the allocated memory block or will return NULL
//   if there isn't sufficient space to satisfy allocation request
//notes: the allocated memory block will be zeroed out
void* Heap_Calloc(long desiredBytes){  
  long* blockPtr;
  long wordsToClear;
  long i;
  
  //malloc a block
  blockPtr = Heap_Malloc(desiredBytes);
  //did malloc fail?
  if(blockPtr == 0){
    return 0; //NULL
  }
  wordsToClear = *(blockPtr - 1); //get room from header
  //clear out block
  for(i = 0; i < wordsToClear; i++){
    blockPtr[i] = 0;
  }
  return blockPtr;
}


//******** Heap_Realloc *************** 
// Reallocate buffer to a new size
//input: 
//  oldBlock: pointer to a block
//  desiredBytes: a desired number of bytes for a new block
//    where the contents of the old block will be copied to
// output: void* pointing to the new block or will return NULL
//   if there is any reason the reallocation can't be completed
// notes: the given block will be unallocated after its contents
//   are copied to the new block
void* Heap_Realloc(void* oldBlock, long desiredBytes){
  long* oldBlockPtr;
  long* oldBlockStart;
  long* newBlockPtr;
  long oldBlockRoom;
  long newBlockRoom;
  long wordsToCopy;
  long i;
  
  oldBlockPtr = (long*) oldBlock;
  // error if...
  // 1) oldBlockPtr doesn't point in the heap
  // 2) oldBlockPtr points to an unused block
  oldBlockStart = oldBlockPtr - 1;
  if(!inHeapRange(oldBlockStart) || blockUnused(oldBlockStart)){
    return 0; // NULL
  }

  newBlockPtr = Heap_Malloc(desiredBytes);
  // did Malloc fail?
  if(newBlockPtr == 0){
    return 0; // NULL
  }
  
  oldBlockRoom = blockRoom(oldBlockStart);
  newBlockRoom = blockRoom(newBlockPtr - 1);
  if(oldBlockRoom < newBlockRoom){
    wordsToCopy = oldBlockRoom;
  }
  else{
    wordsToCopy = newBlockRoom;
  }  
  for(i = 0; i < wordsToCopy; i++){
    newBlockPtr[i] = oldBlockPtr[i];
  }
  if(Heap_Free(oldBlockPtr)){
    return 0; // NULL Free failed
  }
  return newBlockPtr;
}


//******** Heap_Free *************** 
// return a block to the heap
// input: pointer to memory to unallocate
// output: HEAP_OK if everything is ok;
//  HEAP_ERROR_POINTER_OUT_OF_RANGE if pointer points outside the heap;
//  HEAP_ERROR_CORRUPTED_HEAP if heap has been corrupted or trying to
//  unallocate memory that has already been unallocated;
long Heap_Free(void* pointer){
  long* blockStart;
  long* blockEnd;
  long* nextBlockStart;
  
  blockStart = ((long*)pointer) - 1;

  //-----Begin error checking-------
  if(!inHeapRange(blockStart)){
    return HEAP_ERROR_POINTER_OUT_OF_RANGE;
  }
  if(blockUnused(blockStart)){
    return HEAP_ERROR_CORRUPTED_HEAP;
  }
  blockEnd = blockTrailer(blockStart);
  if(!inHeapRange(blockEnd) || blockUnused(blockEnd)){
    return HEAP_ERROR_CORRUPTED_HEAP;
  }
  //-----End error checking-------

  if(markBlockUnused(blockStart)){
    return HEAP_ERROR_CORRUPTED_HEAP;
  }

  // time to possibly merge with block above
  // first, make sure there IS a block above us
  if(blockStart > HEAP_START){ 
    long* previousBlockStart = previousBlockHeader(blockStart);
    // second, make sure we only merge with an unused block
    if(blockUnused(previousBlockStart)){
      mergeBlockWithBelow(previousBlockStart);
      blockStart = previousBlockStart; // start of block has moved
    }
  }

  // possibly merge with block below
  nextBlockStart = nextBlockHeader(blockStart);
  if(inHeapRange(nextBlockStart) && blockUnused(nextBlockStart)){
    mergeBlockWithBelow(blockStart);
  }
  return HEAP_OK;
}


//******** Heap_Test *************** 
// Test the heap
// input: none
// output: validity of the heap - either HEAP_OK or HEAP_ERROR_HEAP_CORRUPTED
long Heap_Test(void){
  long lastBlockWasUnused = 0;
  long* blockStart = HEAP_START;
  while(inHeapRange(blockStart)){
    long* blockEnd;
    
    //shouldn't have any blocks holding zero words
    if(*blockStart == 0){
      return HEAP_ERROR_CORRUPTED_HEAP;
    }
    blockEnd = blockTrailer(blockStart);
    //error if blockEnd is not in the heap or blockend disagrees with blockStart
    if(!inHeapRange(blockEnd) || *blockStart != *blockEnd){
      return HEAP_ERROR_CORRUPTED_HEAP;
    }
    //error if we have two adjacent unused blocks
    if(lastBlockWasUnused && blockUnused(blockStart)){
      return HEAP_ERROR_CORRUPTED_HEAP;
    }
    lastBlockWasUnused = blockUnused(blockStart);
    blockStart = blockEnd + 1;
  }
  //traversing the heap should end exactly where the heap ends
  if(blockStart != HEAP_END){
    return HEAP_ERROR_CORRUPTED_HEAP;
  }
  return HEAP_OK;
}


//******** Heap_Stats *************** 
// return the current status of the heap
// input: none
// output: a heap_stats_t that describes the current usage of the heap
heap_stats_t Heap_Stats(void){
  long* blockStart;
  heap_stats_t stats;
  
  stats.wordsAllocated = 0;
  stats.wordsAvailable = 0;
  stats.blocksUsed = 0;
  stats.blocksUnused = 0;

  //just go through each block to get stats on heap usage
  blockStart = HEAP_START;
  while(inHeapRange(blockStart)){
    if(blockUsed(blockStart)){
      stats.wordsAllocated += blockRoom(blockStart);
      stats.blocksUsed++;
    }
    else{
      stats.wordsAvailable += blockRoom(blockStart);
      stats.blocksUnused++;
    }
    blockStart = nextBlockHeader(blockStart);
  }
  stats.wordsOverhead = HEAP_SIZE_WORDS - stats.wordsAllocated - stats.wordsAvailable;
  return stats;
}


// inHeapRange
// input: a pointer
// output: whether or not the pointer points inside the heap
static long inHeapRange(long* address){
  return address >= HEAP_START && address < HEAP_END;
}


// blockUsed
// input: pointer to the header or trailer of a block
// output: whether or not the block is marked as used/allocated
static long blockUsed(long* block){
  return *block > 0;
}


// blockUnused
// input: pointer to the header or trailer of a block
// output: whether or not the block is marked as unused/unallocated
static long blockUnused(long* block){
  return *block < 0;
}


// blockRoom
// input: pointer to the header or trailer of a block
// output: how many words of data the block can hold
static long blockRoom(long* block){
  if(*block > 0){
    return *block;
  }
  return -*block;
}


// // blockSize
// // input: pointer to the header or trailer of a block
// // output: the size of a block in words, including header and trailer
// static long blockSize(long* block){
//   if(*block > 0){
//     return *block + 2;
//   }
//   return -*block + 2;
// }


// blockHeader
// input: pointer to the trailer of a block
// output: pointer to the header of the same block
static long* blockHeader(long* blockEnd){
  return blockEnd - blockRoom(blockEnd) - 1;
}


// blockTrailer
// input: pointer to the header of a block
// output: pointer to the trailer of the same block
static long* blockTrailer(long* blockStart){
  return blockStart + blockRoom(blockStart) + 1;
}


// nextBlockHeader
// input: pointer to the header of a block
// output: pointer the the header of the next block in the heap
// notes: given the header of the last block in the heap, will point to HEAP_END,
//   which is not a valid block; be careful
static long* nextBlockHeader(long* blockStart){
  return blockTrailer(blockStart) + 1;
}


// previousBlockHeader
// input: pointer to the header of a block
// output: pointer the the header of the previous block in the heap
// notes: given the header of the first block in the heap, this function
//   will go crazy and return a proportionally crazy address!
static long* previousBlockHeader(long* blockStart){
  return blockHeader(blockStart - 1);
}


// markBlockUsed
// input: pointer to the header of a block
// output: a heap flag - HEAP_OK if everything is ok or HEAP_ERROR_CORRUPTEDHEAP
//   if there is something obviously wrong with the block
//notes: marks the block as used/allocated
static long markBlockUsed(long* blockStart){
  long* blockEnd = blockTrailer(blockStart);
  if(blockUsed(blockStart) || *blockStart != *blockEnd){
    return HEAP_ERROR_CORRUPTED_HEAP;
  }
  *blockStart = -*blockStart;
  *blockEnd = -*blockEnd;
  return HEAP_OK;
}


// markBlockUnused
// input: pointer to the header of a block
// output: a heap flag - HEAP_OK if everything is ok or HEAP_ERROR_CORRUPTEDHEAP
//  if there is something obviously wrong with the block
// notes: marks the block as unused/unallocated
static long markBlockUnused(long* blockStart){
  long* blockEnd = blockTrailer(blockStart);
  if(blockUnused(blockStart) || *blockStart != *blockEnd){
    return HEAP_ERROR_CORRUPTED_HEAP;
  }
  *blockStart = -*blockStart;
  *blockEnd = -*blockEnd;
  return HEAP_OK;
}


// splitAndMarkBlockUsed
// input: 
//  uppterBlockStart: header of a block 
//  desiredRoom: desired amount of words to be in the new upper block
// output: none
// notes: splits the block given so that the new upper block holds desiredRoom
//  words (or more).  Marks the upper block as used, lower block as unused.
//  Will not split a block if the leftover room is insufficient to make another
//  useful block.
static long splitAndMarkBlockUsed(long* upperBlockStart, long desiredRoom){
  long leftoverRoom = blockRoom(upperBlockStart) - desiredRoom - 2;
  // only split block if leftovers could actually make another useful block
  if(leftoverRoom > 0){
    long* upperBlockEnd = upperBlockStart + desiredRoom + 1;
    long* lowerBlockStart = upperBlockEnd + 1;
    long* lowerBlockEnd = blockTrailer(upperBlockStart);
    *upperBlockStart = desiredRoom; // marked used
    *upperBlockEnd = desiredRoom;
    *lowerBlockStart = -leftoverRoom; // marked unused
    *lowerBlockEnd = -leftoverRoom;
  }
  // can't split block - just mark it at used
  else{
    if(markBlockUsed(upperBlockStart)){
      return 0; // NULL Free failed
    }
  }
  return HEAP_OK;
}


// mergeBlockWithBelow
// input: pointer to the header of a block
// output: none
// notes: will merge the given block with the block below it.
//  WARNING: Does not check that the block below actually exists.
static void mergeBlockWithBelow(long* upperBlockStart){
  long* upperBlockEnd = blockTrailer(upperBlockStart);
  long* lowerBlockStart = upperBlockEnd + 1;
  long* lowerBlockEnd = blockTrailer(lowerBlockStart);

  long room = lowerBlockEnd - upperBlockStart - 1;
  *upperBlockStart = -room;
  *lowerBlockEnd = -room;
  return;
}


