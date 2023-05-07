#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "data_types.h"

// ========  YOUR COMPULSORY (BUT SPECIFIC TO THE PROBLEM) COMMANDS ============
#define HASH_TABLE_BASED_SIZE 25
#define HASH_TABLE_INCREASING_RATE 70
#define MAX_KEY_SIZE 3

// ================== YOU DO NOT NEED TO CHANGE THIS PART ======================
typedef struct {
    unsigned int size;
    unsigned int count;
    unsigned char** State_Key;
} Hash_Table;


// =========  YOUR COMPULSORY (BUT SPECIFIC TO THE PROBLEM) DECLARATIONS =======
// Do not change the prototype. You should update the block of it in HashTable.c
void Generate_HashTable_Key(const State *const state, unsigned char* key);


// ======= YOU DO NOT NEED TO CHANGE THIS COMPULSORY DECLARATIONS ==============
Hash_Table* New_Hash_Table(const int size);
void Resize_Hash_Table(Hash_Table *ht, const int size);
void Delete_Hash_Table(Hash_Table *ht);
void ht_insert(Hash_Table *ht, const State *const state);
void ht_insert_key(Hash_Table *ht, const char *key);
int ht_search(Hash_Table *ht, const State *const state);
void Show_Hash_Table(Hash_Table *ht);


#endif
