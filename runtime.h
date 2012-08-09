/*
 * runtime.h
 * Copyright (c) 2012 Christina Brooks
 *
 * xxx
 */

#ifndef libkern_runtime_h
#define libkern_runtime_h

#include "mach_types.h"

typedef __darwin_size_t	size_t;

#define __unused

#ifdef __cplusplus
extern "C" {
#endif

/* starndard stuff */
	void panic(const char *fmt, ...);
	void printk(const char *fmt, ...);
	int snprintf(char *buf, size_t size, const char *fmt, ...);
	
/* mem */
	void kfree(void* ptr);
	void* kalloc(size_t size);
	void bzero(void* base, size_t size);
	void bcopy(const void *src, void *dest, size_t n);
	
/* string */
	int strcmp(const char *s1, const char *s2);
	int strncmp(const char *s1, const char *s2, size_t n);
	size_t strlen(const char *s);
	
#ifdef __cplusplus
}

/* now for C++ syms */
void kfree(void* ptr, size_t size);

#endif

#define assert(x) if (!x) { panic("[%s:%d] assertion failed '%s'", __FILE__, __LINE__, #x); }

#endif
