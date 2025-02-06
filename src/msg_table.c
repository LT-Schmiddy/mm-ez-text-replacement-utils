#include "libc/string.h"
#include "msg_table.h"
#include "util.h"

MsgEntry* MsgEntry_Create(u16 textId) {
    MsgEntry* retVal = recomp_alloc(sizeof(MsgEntry));
    retVal->textId = textId;
    return retVal;
}

void MsgEntry_Destroy(MsgEntry* entry) {
    recomp_free(entry);
}

s32 MsgBuffer_Len(MsgBuffer* buf) {

    IF_DEBUG recomp_printf("Counting: ");
    int i = 0;
    while (buf->schar[i] != MSG_ENDING_CHAR && i < MESSAGE_BUFFER_SIZE) {
        IF_DEBUG recomp_printf( is_printable_char(buf->schar[i]) ? "%c" : "\\x%1X", buf->schar[i]);    
        i++;
    }

    IF_DEBUG recomp_printf(" -> %i\n", i + 2);  
    // Add 2 to include termination char, and to go from index to length.
    return i + 2;
}

s32 MsgBuffer_CopyFromCharStr(MsgBuffer* dst, char* src) {

    IF_DEBUG recomp_printf("Copying: ");
    int i = 0;

    bool should_end = false;
    while (!should_end && i < MESSAGE_CONTENT_SIZE) {

        IF_DEBUG recomp_printf( is_printable_char(src[i]) ? "%c" : "\\x%02X", src[i]);
        dst->schar[i + MESSAGE_HEADER_SIZE] = src[i];
        i++;

        should_end = src[i] == MSG_ENDING_CHAR;
    }
    
    IF_DEBUG recomp_printf(" -> %i\n", i); 
    // Add 1 to go from index to length. 
    return i + 1;
}

s32 MsgBuffer_CopyFromCharStr_PipeEscapeBytes(MsgBuffer* dst, char* src) {

    IF_DEBUG recomp_printf("Copying w/ Pipes: ");
    int src_pos = 0;
    int dst_pos = 0;
    bool should_end = false;
    while (!should_end && dst_pos < MESSAGE_CONTENT_SIZE) {

        if (src[src_pos] != PIPE_CHAR) {
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = src[src_pos];
            IF_DEBUG recomp_printf( is_printable_char(src[src_pos]) ? "%c" : "\\x%02X", src[src_pos]);
            should_end = src[src_pos] == MSG_ENDING_CHAR;
            src_pos++;
            dst_pos++;
        } else if (src[src_pos + 1] == PIPE_CHAR) {
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = PIPE_CHAR;
            src_pos += 2;
            dst_pos++;
        } else {
            // Parsing Byte Escape:
            IF_DEBUG recomp_printf("%c%c", src[src_pos + 1], src[src_pos + 2]);
            char write_byte = hex_to_byte(&src[src_pos + 1]);
            dst->schar[dst_pos + MESSAGE_HEADER_SIZE] = write_byte;
            should_end = write_byte == MSG_ENDING_CHAR;
            src_pos += 3;
            dst_pos++;
        }

    }
    // Copy Final Char:
    IF_DEBUG recomp_printf( is_printable_char(src[src_pos]) ? "%c" : "\\x%02X", src[src_pos]);
    // dst->schar[src_pos + MESSAGE_HEADER_SIZE] = src[src_pos];
    
    IF_DEBUG recomp_printf(" -> %i\n", src_pos); 
    // Add 1 to go from index to length. 
    return dst_pos + 1;
}

MsgTable* MsgTable_Create() {
    MsgTable* retVal = recomp_alloc(sizeof(MsgTable));
    
    // Setting Initial Values:
    retVal->entries = recomp_alloc( MSG_TABLE_START_SIZE * sizeof(MsgEntry*));
    retVal->count = 0;
    retVal->capacity = MSG_TABLE_START_SIZE;
    retVal->_automaticSorting = true;

    recomp_printf("%sMessage Table Created.\n", LOG_HEADER);
    return retVal;
}

void MsgTable_Destroy(MsgTable* table) {
    for (int i = 0; i < table->count; i++) {
        MsgEntry_Destroy(table->entries[i]);
    }
    recomp_free(table->entries);
    recomp_free(table);
}

void MsgTable_Expand(MsgTable* table) {
    MsgTable_ExpandTo(table, table->capacity * 2);
}

void MsgTable_ExpandTo(MsgTable* table, u32 new_capacity) {
    // Create the expanded table:
    MsgEntry** new_entries = recomp_alloc(new_capacity * sizeof(MsgEntry*));
    // Copy to it:
    memcpy(new_entries, table->entries, table->count * sizeof(MsgEntry*));
    // Trash the old table:
    recomp_free(table->entries);
    // Now use the new table:
    table->entries = new_entries;
    table->capacity = new_capacity;

    recomp_printf("%sExpand MsgTable capacity to %d entries.\n", LOG_HEADER, table->capacity);
}

MsgEntry* MsgTable_GetEntry(MsgTable* table, u16 id) {
    // Using Binary search. Thanks to the constant sorting, This should be faster than a linear search:
    if (table->count == 0) {
        IF_DEBUG recomp_printf("Empty Lookup:\n");
        return NULL;
    }    
    
    // To avoid bugs, don't use binary search until we've reached a minimum size.
    if (table->count < START_USING_BINARY_LOOKUP) {
        IF_DEBUG recomp_printf("Linear Lookup:\n");
        for (s32 i = 0; i < table->count; i++) {
            IF_DEBUG recomp_printf("\t%i\n", table->entries[i]->textId);
            if (table->entries[i]->textId == id) {
                return table->entries[i];
            }
        }
        return NULL;
    } else {
        s32 low = 0;
        s32 high = table->count - 1;
        MsgEntry** entries = table->entries;
        IF_DEBUG recomp_printf("Binary Lookup:\n");
        while (low <= high) {
            int mid = low + (high - low) / 2;
            // recomp_printf("High: %d, Low: %d, Central Point: %d\n", high, low, mid);
        
            if (entries[mid]->textId == id) {
                IF_DEBUG recomp_printf("\thigh: %d, Low: %d, Central Point: %d\n", high, low, mid);
                return entries[mid];
                
            }
            else if (entries[mid]->textId < id) {
                low = mid + 1;
            }
            else {
                high = mid - 1;
            }
        }

        return NULL;
    }
}

MsgBuffer* MsgTable_GetBuffer(MsgTable* table, u16 id) {
    MsgEntry* entry = MsgTable_GetEntry(table, id);
    if (entry == NULL) {
        return NULL;
    }

    return &entry->buf;
}

s32 MsgTable_GetBufferLen(MsgTable* table, u16 id) {
    MsgEntry* entry = MsgTable_GetEntry(table, id);
    if (entry == NULL) {
        return -1;
    }

    return MsgBuffer_Len(&entry->buf);
}

void MsgTable_SetBuffer(MsgTable* table, u16 textId, MsgBuffer* entry) {
    // Updating Existing Entry:
    MsgEntry* search = MsgTable_GetEntry(table, textId);
    if (search != NULL) {
        memcpy(&search->buf, entry, sizeof(MsgBuffer));
        return;
    }

    // Handle New Entry:
    if (table->capacity <= table->count) {
        MsgTable_Expand(table);
    }
    
    table->entries[table->count] = MsgEntry_Create(textId);
    search = table->entries[table->count];
    search->textId = textId;
    memcpy(&search->buf, entry, sizeof(MsgBuffer));
    table->count++;

    if (table->_automaticSorting) {
        MsgTable_BubbleSort(table);
    }

    recomp_printf("%sAdding text entry id %i\n", LOG_HEADER, search->textId);
}


void MsgTable_BubbleSort(MsgTable* table) {
    // Since we're always adding entried to the END of the table, going from greatest to least will be more efficient.
    MsgEntry** entries = table->entries;
    bool fully_sorted = false;

    while(!fully_sorted) {
        fully_sorted = true;
        for (int i = table->count - 1; i >= 1; i--) {
            MsgEntry* upper = entries[i];
            MsgEntry* lower = entries[i - 1];

            if (upper->textId < lower->textId) {
                MsgTable_Swap(&upper, &lower);
                fully_sorted = false;
            }
        }
    }
}

void MsgTable_Swap(MsgEntry** a, MsgEntry** b) {
    MsgEntry* t;
    memcpy(&t, a, sizeof(MsgEntry*));
    memcpy(a, b, sizeof(MsgEntry*));
    memcpy(b, &t, sizeof(MsgEntry*));
}
