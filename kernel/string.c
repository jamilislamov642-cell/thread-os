#include "string.h"

size_t strlen(const char *s) {
    size_t len = 0;
    while (*s++) len++;
    return len;
}

int strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) {
        a++;
        b++;
    }
    return *(const unsigned char *)a - *(const unsigned char *)b;
}

int strncmp(const char *a, const char *b, size_t n) {
    while (n && *a && (*a == *b)) {
        a++;
        b++;
        n--;
    }
    if (n == 0) return 0;
    return *(const unsigned char *)a - *(const unsigned char *)b;
}

char *strcpy(char *dst, const char *src) {
    char *ret = dst;
    while ((*dst++ = *src++));
    return ret;
}

char *strncpy(char *dst, const char *src, size_t n) {
    char *ret = dst;
    while (n && (*dst++ = *src++)) n--;
    while (n--) *dst++ = '\0';
    return ret;
}

void *memcpy(void *dst, const void *src, size_t n) {
    uint8_t *d = (uint8_t *)dst;
    const uint8_t *s = (const uint8_t *)src;
    while (n--) *d++ = *s++;
    return dst;
}

void *memset(void *dst, int val, size_t n) {
    uint8_t *d = (uint8_t *)dst;
    while (n--) *d++ = (uint8_t)val;
    return dst;
}

int memcmp(const void *a, const void *b, size_t n) {
    const uint8_t *p1 = (const uint8_t *)a;
    const uint8_t *p2 = (const uint8_t *)b;
    while (n--) {
        if (*p1 != *p2) return *p1 - *p2;
        p1++;
        p2++;
    }
    return 0;
}

int atoi(const char *s) {
    int res = 0;
    int sign = 1;
    while (*s == ' ' || *s == '\t') s++;
    if (*s == '-') { sign = -1; s++; }
    else if (*s == '+') { s++; }
    while (*s >= '0' && *s <= '9') {
        res = res * 10 + (*s - '0');
        s++;
    }
    return sign * res;
}

static void reverse(char *s, int len) {
    int i = 0, j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++; j--;
    }
}

void itoa(int val, char *buf, int base) {
    if (base < 2 || base > 16) { buf[0] = '\0'; return; }
    int i = 0;
    int is_neg = 0;
    if (val == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    if (val < 0 && base == 10) { is_neg = 1; val = -val; }
    unsigned int uval = (unsigned int)val;
    while (uval > 0) {
        int rem = uval % base;
        buf[i++] = (rem < 10) ? ('0' + rem) : ('a' + rem - 10);
        uval /= base;
    }
    if (is_neg) buf[i++] = '-';
    buf[i] = '\0';
    reverse(buf, i);
}

void utoa(uint32_t val, char *buf, int base) {
    if (base < 2 || base > 16) { buf[0] = '\0'; return; }
    int i = 0;
    if (val == 0) { buf[0] = '0'; buf[1] = '\0'; return; }
    while (val > 0) {
        int rem = val % base;
        buf[i++] = (rem < 10) ? ('0' + rem) : ('a' + rem - 10);
        val /= base;
    }
    buf[i] = '\0';
    reverse(buf, i);
}
