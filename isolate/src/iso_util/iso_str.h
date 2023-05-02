#ifndef __ISO_STR_H__
#define __ISO_STR_H__

#include "iso_defines.h"
#include "iso_memory.h"

// TODO: Take pointer for all the functions
// TODO: Add slice and replace mechanisms

typedef char* iso_str;


/*
 * @brief Macro to convert anything to iso_str
 */

#define iso_to_str(x) iso_str_new(#x)


/*
 * @brief Function to create new string from c_str
 * @mem c_str = C style string
 * @return Returns iso_str
 */

#define iso_str_new(c_str) __iso_str_new(c_str, __FILE__, __LINE__)
ISO_API iso_str __iso_str_new(char* c_str, char* file, u32 line);


/*
 * @brief Function to delete the iso_str
 * @param str = iso_str struct
 */

ISO_API void iso_str_delete(iso_str str);


/*
 * @brief Function to get the length of string
 * @param str = iso_str
 * @return Returns length
 */

ISO_API i32 iso_str_len(iso_str str);

/*
 * @brief Function to get the total capacity of string
 * @param str = iso_str
 * @return Returns capacity
 */

ISO_API i32 iso_str_cap(iso_str str);


/*
 * @brief Function to clear the string
 * @param str = iso_str struct
 */

ISO_API void iso_str_clear(iso_str* str);


/*
 * @brief Function to extend the string
 * @param str = iso_str struct
 * @param amt = Amount to be extended
 */

ISO_API void iso_str_extend(iso_str* str, u32 amt);


/*
 * @brief Function to copy string from one to another
 * @param dest = String to be copied into
 * @param src  = String to copy from
 */

ISO_API void iso_str_cpy(iso_str* dest, iso_str src);


/*
 * @brief Function to concatinate strings
 * @param dest = String to concat into
 * @param src  = String to concat from
 */

ISO_API void iso_str_cat(iso_str* dest, iso_str src);


/*
 * @brief Function to compare two strings
 * @param str1, str2 = Strings to compare
 * @return Returns true if same else false
 */

ISO_API b8   iso_str_cmp(iso_str str1, iso_str str2);


/*
 * @brief Function to get character from certain index
 * @param str = iso_str struct
 * @param i   = index
 * @return Returns the character
 */

ISO_API char iso_str_char_at(iso_str str, u32 i);

#endif // __ISO_STR_H__
