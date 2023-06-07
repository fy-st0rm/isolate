#ifndef __ISO_LIST_H__
#define __ISO_LIST_H__

#include "iso_defines.h"
#include "iso_memory.h"

// Starting capacity of the iso_list
#define ISO_LIST_CAP 10

/*
 * @brief Macro that defines the iso_list according to type T
 * @mem elements  = Pointer to the array of elements
 * @mem len       = Length of the list
 * @mem cap       = Maximum capacity of the list
 * @mem tmp       = Temporary element for internal usage
 */

#define iso_list(T)  \
	struct {           \
		T* elements;     \
		size_t len;      \
		size_t cap;      \
		T tmp;           \
	}*


/*
 * @brief Macro to delete allocated iso_list
 * @param list = iso_list structure
 */

#define iso_list_delete(list)  \
	do {                         \
		iso_free(list->elements);  \
		iso_free(list);            \
	} while(0)


/*
 * @brief Macro to clear the iso_list
 * @param list = iso_list structure
 */

#define iso_list_clear(list)                                   \
	do {                                                         \
		list->len = 0;                                             \
		memset(list->elements, 0, sizeof(list->tmp) * list->cap);  \
	} while (0)


/*
 * @brief Macro to extend the capacity of the iso_list
 * @param list = iso_list structure
 */

#define iso_list_extend(list)                                    \
	do {                                                           \
		void* tmp = iso_alloc(sizeof(list->tmp) * list->len);        \
		memcpy(tmp, list->elements, sizeof(list->tmp) * list->len);  \
		iso_free(list->elements);                                    \
                                                                 \
		list->cap += ISO_LIST_CAP;                                   \
		list->elements = iso_alloc(sizeof(list->tmp) * list->cap);   \
		memcpy(list->elements, tmp, sizeof(list->tmp) * list->len);  \
		iso_free(tmp);                                               \
	} while(0)


/*
 * @brief Macro to add new element to the iso_list
 * @param list = iso_list structure
 * @param e    = element to be added
 */

#define iso_list_add(list, e)                                   \
 do {                                                           \
                                                                \
	if (list == NULL) {                                           \
		list = iso_alloc(sizeof(*list));                            \
		list->len = 0;                                              \
		list->cap = ISO_LIST_CAP;                                   \
		list->elements = iso_alloc(sizeof(list->tmp) * list->cap);  \
	}                                                             \
                                                                \
	if (list->len + 1 >= list->cap) {                             \
		iso_list_extend(list);                                      \
	}                                                             \
                                                                \
	list->elements[list->len++] = e;                              \
 } while (0)


/*
 * @brief Macro to remove an element from index from iso_list
 * @param list = iso_list structure
 * @param idx  = Index of that element
 */

#define iso_list_remove(list, idx)                                                                                               \
 do {                                                                                                                            \
	iso_assert(idx < list->len, "Tried removing element from index `%d` when the length of the list is `%lu`\n", idx, list->len);  \
	memmove(list->elements + idx, list->elements + idx + 1, sizeof(list->tmp) * (list->len - idx));                                \
	list->len--;                                                                                                                   \
 } while (0)


/*
 * @brief Macro to get an element from index from iso_list
 * @param list = iso_list structure
 * @param idx  = Index of that element
 * @return Returns the element
 */

#define iso_list_get(list, idx)                                                                                                     \
	({                                                                                                                                \
		iso_assert(idx < list->len, "Tried accessing element from index `%d` when the length of the list is `%lu`\n", idx, list->len);  \
		list->elements[idx];                                                                                                            \
	})


/*
 * @brief Macro to pop an element from index from iso_list
 * @param list = iso_list structure
 * @param idx  = Index of that element
 * @return Returns the element
 */

#define iso_list_pop(list, idx)           \
	({                                      \
		list->tmp = iso_list_get(list, idx);  \
		iso_list_remove(list, idx);           \
		list->tmp;                            \
	})


/*
 * @brief Macro to check if an element exists in iso_list
 * @param list = iso_list structure
 * @param e    = element to be searched
 * @return Returns true if exists else false
 */

#define iso_list_exists(list, e)           \
	({                                       \
		b8 res = false;                        \
		for (i32 i = 0; i < list->len; i++) {  \
			if (list->elements[i] == (e)) {      \
				res = true;                        \
			}                                    \
		}                                      \
		res;                                   \
	})


/*
 * @brief Macro to get index of an element from iso_list
 * @param list = iso_list structure
 * @param e    = element to be searched
 * @return Returns the index of the element
 */

#define iso_list_get_idx(list, e)          \
	({                                       \
		i32 res = -1;                          \
		for (i32 i = 0; i < list->len; i++) {  \
			if (list->elements[i] == (e)) {      \
				res = i;                           \
			}                                    \
		}                                      \
		res;                                   \
	})


/*
 * @brief Macro to print all elements from iso_list
 * @param list = iso_list structure
 * @param fmt  = Format specifier for printf
 */

#define iso_list_print(list, fmt)\
	do {\
		printf("-----------------\n");\
		for (size_t i = 0; i < list->len; i++) {\
			printf(fmt "\n", list->elements[i]);\
		}\
		printf("-----------------\n");\
	} while(0)

#endif //__ISO_LIST_H__
