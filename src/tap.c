/*!\file
 * This file is part of the Test Anything Protocol (TAP) library; it contains
 * the implementation of the TAP functions.
 *
 * \see lely/tap/tap.h
 *
 * \copyright 2016 Lely Industries N.V.
 *
 * \author J. S. Seldenthuis <jseldenthuis@lely.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "tap.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static int tap_num;

static void tap_vprintf(const char *format, va_list ap);

LELY_TAP_EXPORT void
__tap_plan(int n, const char *format, ...)
{
	if (__unlikely(tap_num))
		return;

	printf("1..%d", n);
	if (!n) {
		printf(" # SKIP");
		if (format && *format) {
			fputc(' ', stdout);
			va_list ap;
			va_start(ap, format);
			tap_vprintf(format, ap);
			va_end(ap);
		}
	}
	fputc('\n', stdout);
}

LELY_TAP_EXPORT int
__tap_test(int test, const char *expr, const char *file, int line,
		const char *format, ...)
{
	assert(expr);
	assert(file);
	assert(format);

	printf(test ? "ok %d" : "not ok %d", ++tap_num);
	if (*format) {
		fputc(' ', stdout);
		va_list ap;
		va_start(ap, format);
		tap_vprintf(format, ap);
		va_end(ap);
	}
	fputc('\n', stdout);

	if (!test && *expr)
		printf("# %s:%d: Test `%s' failed.\n", file, line, expr);

	return test;
}

LELY_TAP_EXPORT void
__tap_diag(const char *format, ...)
{
	assert(format);

	va_list ap;
	va_start(ap, format);
	tap_vprintf(format, ap);
	va_end(ap);
	fputc('\n', stdout);
}

LELY_TAP_EXPORT _Noreturn void
__tap_abort(const char *format, ...)
{
	assert(format);

	printf("Bail out!");
	if (*format) {
		fputc(' ', stdout);
		va_list ap;
		va_start(ap, format);
		tap_vprintf(format, ap);
		va_end(ap);
	}
	fputc('\n', stdout);

	exit(EXIT_FAILURE);
}

static void
tap_vprintf(const char *format, va_list ap)
{
	assert(format);

	va_list aq;
	va_copy(aq, ap);
	int n = vsnprintf(NULL, 0, format, aq);
	va_end(aq);
	if (__unlikely(n < 0))
		return;
	char s[n + 1];
	vsnprintf(s, n + 1, format, ap);

	for (char *cp = s; cp < s + n; cp++) {
		switch (*cp) {
		case '\r':
			if (cp + 1 < s + n && cp[1] == '\n')
				cp++;
		case '\n':
			fputc('\n', stdout);
			fputc('#', stdout);
			fputc(' ', stdout);
			break;
		default:
			fputc(*cp, stdout);
		}
	}
}

