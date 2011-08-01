#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

extern bool clipboard_set_text(char *text);
extern bool clipboard_get_text(char *text, size_t text_sz);

extern char* util_strsep(char **pinputstr, const char *delim);
extern size_t util_strlcpy(char *dst, const char *src, size_t dst_size);
extern size_t util_strlcat(char *dst, const char *src, size_t dst_size);
extern void util_chomp(char *str);

/* Registry stuff */
extern bool reg_read_key(char *key, char *val, void *buf, size_t buflen);

#endif // UTIL_H_INCLUDED
