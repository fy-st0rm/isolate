#include "iso_str.h"

#define STR_EXTEND_AMT 10

iso_str __iso_str_new(char* c_str, char* file, u32 line) {
	iso_str str = __iso_alloc(sizeof(__iso_str_def), file, line);
	u32 len = strlen(c_str);

	str->len = len;
	str->cap = len;
	str->c_str = __iso_alloc(len, file, line);
	strncpy(str->c_str, c_str, len);
	str->c_str[str->len] = '\0';

	return str;
}

void iso_str_delete(iso_str str) {
	iso_free(str->c_str);
	iso_free(str);
}

void iso_str_clear(iso_str str) {
	iso_assert(str, "String is NULL\n");
	memset(str->c_str, 0, str->cap);
	str->len = 0;
	str->c_str[str->len] = '\0';
}

void iso_extend_str(iso_str str, u32 amt) {
	iso_assert(str, "String is NULL\n");

	char* tmp = iso_alloc(str->cap);
	strncpy(tmp, str->c_str, str->len);

	iso_free(str->c_str);
	str->cap += amt;

	str->c_str = iso_alloc(str->cap);
	strncpy(str->c_str, tmp, str->len);

	iso_free(tmp);
}

void iso_str_cpy(iso_str dest, iso_str src) {
	iso_assert(dest, "Destination string is NULL\n");
	iso_assert(src, "Source string is NULL\n");

	if (dest->cap <= src->len) {
		iso_extend_str(dest, src->len + STR_EXTEND_AMT);
	}

	iso_str_clear(dest);
	strncpy(dest->c_str, src->c_str, src->len);
	dest->len = src->len;

	dest->c_str[dest->len] = '\0';
}

void iso_str_cat(iso_str dest, iso_str src) {
	iso_assert(dest, "Destination string is NULL\n");
	iso_assert(src, "Source string is NULL\n");

	if (dest->cap <= dest->len + src->len) {
		u32 diff = (dest->len + src->len) - dest->cap;
		iso_extend_str(dest, diff + STR_EXTEND_AMT);
	}

	strcat(dest->c_str, src->c_str);
	dest->len += src->len;
	dest->c_str[dest->len] = '\0';
}

b8 iso_str_cmp(iso_str str1, iso_str str2) {
	iso_assert(str1, "First string is NULL\n");
	iso_assert(str2, "Second string is NULL\n");

	i32 res = strncmp(str1->c_str, str2->c_str, str1->len);
	if (res == 0) return true;
	return false;
}

char iso_str_char_at(iso_str str, u32 i) {
	iso_assert(str, "String is NULL\n");
	iso_assert(i < str->len, "Index of `%d` used for string of length `%d`\n", i, str->len);

	return str->c_str[i];
}
