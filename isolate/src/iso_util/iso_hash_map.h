#ifndef __ISO_HASH_MAP_H__
#define __ISO_HASH_MAP_H__

#include "iso_defines.h"
#include "iso_memory.h"

// Ignoring the incoatible-pointer-types error
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"


/*
 * @brief Macro to define a single entry of hashmap
 * @param k_type = data type of key
 * @param v_type = data type of value
 */

#define __iso_hmap_entry_def(k_type, v_type) \
	struct {                                   \
		k_type key;                              \
		v_type val;                              \
		void* parent;                            \
		void* next;                              \
	}

/*
 * @brief Macro to define the hashmap structure
 * @param k_type = data type of key
 * @param v_type = data type of value
 * @param size   = size of hash map
 */

#define iso_hmap_def(k_type, v_type, size)             \
	struct {                                             \
		__iso_hmap_entry_def(k_type, v_type)* data[size];  \
		__iso_hmap_entry_def(k_type, v_type)** tmp_entry;  \
		k_type tmp_key;                                    \
		v_type tmp_val;                                    \
	}*

/*
 * @brief Macro to delete the hashmap. Cleans up the allocated memories.
 * @param hmap = Pointer to the hashmap.
 */

#define iso_hmap_delete(hmap)                         \
	do {                                                \
		if (hmap != NULL) {                               \
			for (i32 i = 0; i < iso_hmap_size(hmap); i++) { \
				while (hmap->data[i] != NULL) {               \
					void* ptr = hmap->data[i];                  \
					hmap->data[i] = hmap->data[i]->next;        \
					iso_free(ptr);                              \
				}                                             \
			}                                               \
			iso_free(hmap);                                 \
		}                                                 \
	} while (0)

/*
 * @brief Macro to calculate the length of hashmap
 * @param hmap = Pointer to the hashmap
 */

#define iso_hmap_size(hmap) sizeof(hmap->data) / sizeof(hmap->data[0])

/*
 * @brief Function to calculate hash of the key
 * @param data = key to be hashed
 */

static i64 __iso_hash_data(char* data) {
	i64 hash = 5381;
	i32 c;
	while ((c = *data++)) {
		//hash = ((hash << 5) + hash) + c; // hash * 33 + c
		hash = hash * 37 + c;
	}
	return hash;
}

/*
 * @brief Macro to add new elements to the hashmap
 * @param hmap = Pointer to the hashmap
 * @param _k   = Key of the element
 * @param _v   = Value of the element
 */

#define iso_hmap_add(hmap, _k, _v)                                     \
	do {                                                                 \
                                                                       \
		/* Initializing hashmap if its null */                             \
		if (hmap == NULL) {                                                \
			hmap = iso_alloc(sizeof(*hmap));                                 \
			memset(hmap->data, 0, sizeof(hmap->data));                       \
		}                                                                  \
                                                                       \
		/* Generating hash of the key and creating index out of it */      \
		hmap->tmp_key = _k;                                                \
		i64 hash;                                                          \
		if (ISO_IS_POINTER(hmap->tmp_key)) {                               \
			hash = __iso_hash_data((char*) hmap->tmp_key);                   \
		} else {                                                           \
			hash = __iso_hash_data((char*) &hmap->tmp_key);                  \
		}                                                                  \
		i32 idx  = hash % (iso_hmap_size(hmap));                           \
                                                                       \
		/* Resolving the data collision */                                 \
		void* parent = NULL;                                               \
		hmap->tmp_entry = &hmap->data[idx];                                \
		while (*hmap->tmp_entry != NULL && hmap->data[idx]->key != _k) {   \
			parent = *hmap->tmp_entry;                                       \
			hmap->tmp_entry = &(*hmap->tmp_entry)->next;                     \
		}                                                                  \
		if (*hmap->tmp_entry != NULL) iso_free(*hmap->tmp_entry);          \
                                                                       \
		/* Allocating the data at that index and assigning the value */    \
		*hmap->tmp_entry = iso_alloc(sizeof(*hmap->data[idx]));            \
		(*hmap->tmp_entry)->key = _k;                                      \
		(*hmap->tmp_entry)->val = _v;                                      \
		(*hmap->tmp_entry)->parent = parent;                               \
		(*hmap->tmp_entry)->next = NULL;                                   \
                                                                       \
	} while (0)


/*
 * @brief Macro to search an item in hash map and gives the result and value.
 * @param hmap  = Pointer to the hashmap.
 * @param _k    = Key to be searched.
 * @param res   = Result of the search queue. ( 1 if found else 0 ).
 * @param entry = Pointer of the searched item is saved. Only if it found.
 */

#define iso_hmap_search(hmap, _k, res, entry)                      \
	do {                                                             \
		res = 0;                                                       \
                                                                   \
		/* Generating hash of the key and creating index out of it */  \
		hmap->tmp_key = _k;                                            \
		i64 hash;                                                      \
		if (ISO_IS_POINTER(hmap->tmp_key)) {                           \
			hash = __iso_hash_data((char*) hmap->tmp_key);               \
		} else {                                                       \
			hash = __iso_hash_data((char*) &hmap->tmp_key);              \
		}                                                              \
		i32 idx  = hash % (iso_hmap_size(hmap));                       \
                                                                   \
		/* Searching for the key in the following index */             \
		hmap->tmp_entry = &hmap->data[idx];                            \
		hmap->tmp_key = _k;                                            \
		while ((*hmap->tmp_entry) != NULL) {                           \
			if (ISO_IS_POINTER(hmap->tmp_key)) {                         \
				if (strcmp(_k, (*hmap->tmp_entry)->key) == 0) {            \
					res = 1;                                                 \
					entry = *hmap->tmp_entry;                                \
					break;                                                   \
				}                                                          \
			} else {                                                     \
				if (_k == (*hmap->tmp_entry)->key) {                       \
					res = 1;                                                 \
					entry = *hmap->tmp_entry;                                \
					break;                                                   \
				}                                                          \
			}                                                            \
			hmap->tmp_entry = &(*hmap->tmp_entry)->next;                 \
		}                                                              \
	} while (0)

/*
 * @brief Macro to search for items in hashmap
 * @param hmap = Pointer to the hash map
 * @param _k   = Key to be searched
 * @param res  = Variable where result is to be stored. `res` is 1 if found else 0.
 */

#define iso_hmap_exists(hmap, _k, res) iso_hmap_search(hmap, _k, res, *hmap->tmp_entry)

/*
 * @brief Macro to get the value of the searched key.
 * @param hmap  = Pointer to the hash map.
 * @param _k    = Key to be searched.
 * @param value = Value of searched item is stored here.
 */

#define iso_hmap_get(hmap, _k, value)                                \
	do {                                                               \
		b8 res;                                                          \
		iso_hmap_search(hmap, _k, res, *hmap->tmp_entry);                \
		iso_assert(res, "The searched key doesn`t exists in hashmap.\n");\
		value = (*hmap->tmp_entry)->val;                                 \
	} while (0)

/*
 * @brief Macro to remove and item from the hashmap.
 * @param hmap = Pointer to the hashmap.
 * @param _k   = Key to be removed.
 */

#define iso_hmap_remove(hmap, _k)                                               \
	do {                                                                          \
		b8 res;                                                                     \
		iso_hmap_search(hmap, _k, res, *hmap->tmp_entry);                           \
		iso_assert(res, "The key requested to remove doesn`t exists in hashmap.\n");\
                                                                                \
		void* next = (*hmap->tmp_entry)->next;                                      \
		void* child = *hmap->tmp_entry;                                             \
		*hmap->tmp_entry = (*hmap->tmp_entry)->parent;                              \
                                                                                \
		/* If the parent is not empty */                                            \
		if (*hmap->tmp_entry != NULL) {                                             \
			(*hmap->tmp_entry)->next = next;                                          \
		}                                                                           \
		/* If the parent is empty but has a child */                                \
		else if (*hmap->tmp_entry == NULL && next != NULL) {                        \
			hmap->tmp_key = _k;                                                       \
			i64 hash;                                                                 \
			if (ISO_IS_POINTER(hmap->tmp_key)) {                                      \
				hash = __iso_hash_data((char*) hmap->tmp_key);                          \
			} else {                                                                  \
				hash = __iso_hash_data((char*) &hmap->tmp_key);                         \
			}                                                                         \
			i32 idx  = hash % (iso_hmap_size(hmap));                                  \
			hmap->data[idx] = next;                                                   \
		}                                                                           \
                                                                                \
		iso_free(child);                                                            \
	} while (0)

/*
 * @brief Macro to print all the hashmap
 * @param hmap  = Pointer to the hashmap
 * @param k_fmt = Print format specifier for key
 * @param v_fmt = Print format specifier for value
 */

#define iso_hmap_dump(hmap, k_fmt, v_fmt)                                                        \
	do {                                                                                           \
                                                                                                 \
		printf("\n");                                                                                \
		for (i32 i = 0; i < iso_hmap_size(hmap); i++) {                                              \
			/* Printing the data that are not NULL */                                                  \
			if (hmap->data[i] != NULL) {                                                               \
				printf("%d: (" k_fmt ", " v_fmt ") ", i, hmap->data[i]->key, hmap->data[i]->val);        \
                                                                                                 \
				/* Getting the next entry of the data */                                                 \
				hmap->tmp_entry = &hmap->data[i]->next;                                                  \
				while ((*hmap->tmp_entry) != NULL) {                                                     \
					printf("(" k_fmt ", " v_fmt ") ", (*hmap->tmp_entry)->key, (*hmap->tmp_entry)->val);   \
					hmap->tmp_entry = &(*hmap->tmp_entry)->next;                                           \
				}                                                                                        \
                                                                                                 \
				printf("\n");                                                                            \
			}                                                                                          \
		}                                                                                            \
		printf("\n");                                                                                \
	} while (0)

#endif //__ISO_HASH_MAP_H__
