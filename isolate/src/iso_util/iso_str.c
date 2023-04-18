#include "iso_str.h"

#define STR_EXTEND_AMT 10


typedef enum {
	ISO_STR_LEN = -1,
	ISO_STR_CAP = -2
} iso_str_meta;

#define LEN ISO_STR_LEN * sizeof(iso_str_meta)
#define CAP ISO_STR_CAP * sizeof(iso_str_meta)


iso_str __iso_str_new(char* c_str, char* file, u32 line) {
	u32 meta_size = 2 * sizeof(iso_str_meta);
	u32 len = strlen(c_str);
	u32 size = len + meta_size;

	iso_str str = __iso_alloc(size, file, line);
	str += meta_size;

	str[LEN] = len;
	str[CAP] = len;

	strncpy(str, c_str, len);
	str[len] = '\0';

	return str;
}

void iso_str_delete(iso_str str) {
	u32 meta_size = 2 * sizeof(iso_str_meta);
	str -= meta_size;
	iso_free(str);
}

i32 iso_str_len(iso_str str) {
	return (i32) str[LEN];
}

i32 iso_str_cap(iso_str str) {
	return (i32) str[CAP];
}

void iso_str_clear(iso_str* str) {
	i32 l = iso_str_len(*str);
	i32 c = iso_str_cap(*str);
	memset(*str, 0, c);
	(*str)[LEN] = 0;
	(*str)[l] = '\0';
}

void iso_str_extend(iso_str* str, u32 amt) {
	i32 cap = iso_str_cap(*str);
	i32 len = iso_str_len(*str);

	char* tmp = iso_alloc(cap);
	strncpy(tmp, *str, len);
	iso_str_delete(*str);

	cap += amt;

	// Creating new string
	i32 meta_sz = 2 * sizeof(iso_str_meta);
	i32 new_sz = cap + meta_sz;
	*str = iso_alloc(new_sz);
	(*str) += meta_sz;

	(*str)[LEN] = len;
	(*str)[CAP] = cap;
	strncpy(*str, tmp, len);
	(*str)[len] = '\0';

	iso_free(tmp);
}

void iso_str_cpy(iso_str* dest, iso_str src) {
	i32 dc = iso_str_cap(*dest);
	i32 dl = iso_str_len(*dest);
	i32 sl = iso_str_len(src);

	if (dc <= sl) {
		iso_str_extend(dest, sl + STR_EXTEND_AMT);
	}

	iso_str_clear(dest);
	strncpy(*dest, src, sl);

	// Changing the length
	dl = sl;
	(*dest)[LEN] = dl;
	(*dest)[dl] = '\0';
}

void iso_str_cat(iso_str* dest, iso_str src) {
	i32 dc = iso_str_cap(*dest);
	i32 dl = iso_str_len(*dest);
	i32 sl = iso_str_len(src);
	printf("%d %d %d\n", dc, dl, sl);

	if (dc <= dl + sl) {
		iso_str_extend(dest, sl + STR_EXTEND_AMT);
		printf("%s\n", *dest);
	}

	printf("DEST: %s\n", *dest);
	strncat(*dest, src, sl);
	printf("DEST: %s\n", *dest);

	dl += sl;
	(*dest)[LEN] = dl;
	(*dest)[dl] = '\0';
}

b8 iso_str_cmp(iso_str str1, iso_str str2) {
	i32 res = strncmp(str1, str2, iso_str_len(str1));
	if (res == 0) return true;
	return false;
}

char iso_str_char_at(iso_str str, u32 i) {
	iso_assert(i < iso_str_len(str), "Index of `%d` used for string of length `%d`\n", i, iso_str_len(str));

	return str[i];
}
