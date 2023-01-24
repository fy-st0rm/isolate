#ifndef __ISO_HASH_MAP_H__
#define __ISO_HASH_MAP_H__

#include "iso_defines.h"
#include "iso_memory.h"

// Ignoring the incoatible-pointer-types error
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"


/*
 * @brief Macro to define a single entry of hashmap
 * @param k_type = data type of key
 * @param v_type = data type of value
 */

#define __iso_hmap_entry_def(k_type, v_type)\
	struct {\
		k_type key;\
		v_type val;\
		void* next;\
	}

/*
 * @brief Macro to define the hashmap structure
 * @param k_type = data type of key
 * @param v_type = data type of value
 * @param size   = size of hash map
 */

#define iso_hmap_def(k_type, v_type, size)\
	struct {\
		__iso_hmap_entry_def(k_type, v_type)* data[size];\
		__iso_hmap_entry_def(k_type, v_type)** tmp_entry;\
		k_type tmp_key;\
	}*

/*
 * @brief Macro to delete the hashmap. Cleans up the allocated memories.
 * @param hmap = Pointer to the hashmap.
 */

#define iso_hmap_delete(hmap)\
	do {\
		if (hmap == NULL) return;\
		for (i32 i = 0; i < iso_hmap_size(hmap); i++) {\
			while (hmap->data[i] != NULL) {\
				void* ptr = hmap->data[i];\
				hmap->data[i] = hmap->data[i]->next;\
				iso_free(ptr);\
			}\
		}\
		iso_free(hmap);\
	} while(0)

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
	while (c = *data++) {
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

#define iso_hmap_add(hmap, _k, _v)\
	do {\
\
		/* Initializing hashmap if its null */ \
		if (hmap == NULL) {\
			hmap = iso_alloc(sizeof(*hmap));\
			memset(hmap->data, 0, sizeof(hmap->data));\
		}\
\
		/* Generating hash of the key and creating index out of it */ \
		hmap->tmp_key = _k;\
		i64 hash;\
		if (ISO_IS_POINTER(hmap->tmp_key)) {\
			hash = __iso_hash_data((char*) hmap->tmp_key);\
		} else {\
			hash = __iso_hash_data((char*) &hmap->tmp_key);\
		}\
		i32 idx  = hash % (iso_hmap_size(hmap));\
\
		/* Resolving the data collision */ \
		hmap->tmp_entry = &hmap->data[idx];\
		while (*hmap->tmp_entry != NULL && hmap->data[idx]->key != _k) {\
			hmap->tmp_entry = &(*hmap->tmp_entry)->next;\
		}\
\
		/* Allocating the data at that index and assigning the value */ \
		*hmap->tmp_entry = iso_alloc(sizeof(*hmap->data[idx]));\
		(*hmap->tmp_entry)->key = _k;\
		(*hmap->tmp_entry)->val = _v;\
		(*hmap->tmp_entry)->next = NULL;\
\
	} while(0)

/*
 * @brief Macro to search for items in hashmap
 * @param hmap = Pointer to the hash map
 * @param _k = Key to be searched
 * @param res = Variable where result is to be stored. `res` is 1 if found else 0.
 */

#define iso_hmap_exists(hmap, _k, res)\
	do {\
		res = 0;\
		/* Generating hash of the key and creating index out of it */ \
		hmap->tmp_key = _k;\
		i64 hash;\
		if (ISO_IS_POINTER(hmap->tmp_key)) {\
			hash = __iso_hash_data((char*) hmap->tmp_key);\
		} else {\
			hash = __iso_hash_data((char*) &hmap->tmp_key);\
		}\
		i32 idx  = hash % (iso_hmap_size(hmap));\
\
		/* Searching for the key in the following index */\
		hmap->tmp_entry = &hmap->data[idx];\
		while ((*hmap->tmp_entry) != NULL) {\
			if (_k == (*hmap->tmp_entry)->key) {\
				res = 1;\
				break;\
			}\
			hmap->tmp_entry = &(*hmap->tmp_entry)->next;\
		}\
	} while(0)

/*
 * TODO: Implement iso_hmap_get
 */

#define iso_hmap_get(hmap, _k)\
	(\
		hmap->tmp_key = _k,\
		ISO_IS_POINTER(hmap->tmp_key) ? hmap->data[__iso_hash_data((char*) hmap->tmp_key) % (iso_hmap_size(hmap))]->val : hmap->data[__iso_hash_data((char*) &hmap->tmp_key) % (iso_hmap_size(hmap))]->val\
	);\
	i32 res = 0;\
	iso_hmap_exists(hmap, _k, res);\
	iso_assert(res, "The searched key doesn`t exists in hashmap.");\

/*
 * TODO: Implement iso_hmap_remove
 */

#define iso_hmap_remove(hmap, _k)

/*
 * @brief Macro to print all the hashmap
 * @param hmap  = Pointer to the hashmap
 * @param k_fmt = Print format specifier for key
 * @param v_fmt = Print format specifier for value
 */

#define iso_hmap_dump(hmap, k_fmt, v_fmt)\
	do {\
\
		for (i32 i = 0; i < iso_hmap_size(hmap); i++) {\
			/* Printing the data that are not NULL */\
			if (hmap->data[i] != NULL) {\
				printf("%d: (" k_fmt ", " v_fmt ") ", i, hmap->data[i]->key, hmap->data[i]->val);\
\
				/* Getting the next entry of the data */\
				hmap->tmp_entry = &hmap->data[i]->next;\
				while ((*hmap->tmp_entry) != NULL) {\
					printf("(" k_fmt ", " v_fmt ") ", (*hmap->tmp_entry)->key, (*hmap->tmp_entry)->val);\
					hmap->tmp_entry = &(*hmap->tmp_entry)->next;\
				}\
\
				printf("\n");\
			}\
		}\
	} while(0)

#endif //__ISO_HASH_MAP_H__
