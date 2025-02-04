#include "msg_table_quicksort.h"

// Sorting Functions - QuickSort Implementation:
// Partition function
static int _MsgTable_SortPartition(MsgEntry* entries, int low, int high) {
    
    // Choose the pivot
    MsgEntry pivot = entries[high];
    
    // Index of smaller element and indicates 
    // the right position of pivot found so far
    s32 i = low - 1;

    // Traverse arr[low..high] and move all smaller
    // elements to the left side. Elements from low to 
    // i are smaller after every iteration
    for (s32 j = low; j <= high - 1; j++) {
        if (entries[j].textId < pivot.textId) {
            i++;
            MsgTable_Swap(&entries[i], &entries[j]);
        }
    }
    
    // Move pivot after smaller elements and
    // return its position
    MsgTable_Swap(&entries[i + 1], &entries[high]);  
    return i + 1;
}

// The QuickSort function implementation
static void _MsgTable_SortRecurse(MsgEntry* entries, int low, int high) {
    if (low < high) {
        
        // pi is the partition return index of pivot
        s32 pi = _MsgTable_SortPartition(entries, low, high);

        // Recursion calls for smaller elements
        // and greater or equals elements
        _MsgTable_SortRecurse(entries, low, pi - 1);
        _MsgTable_SortRecurse(entries, pi + 1, high);
    }
}

void MsgTable_QuickSort(MsgTable* table) {
    _MsgTable_SortRecurse(table->entries, 0, table->count - 1);
}