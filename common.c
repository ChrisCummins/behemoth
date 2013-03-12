#include "common.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((noreturn))
void fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vfprintf(stderr, format, ap);
	va_end(ap);

	abort();
}
