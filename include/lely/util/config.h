/*!\file
 * This header file is part of the utilities library; it contains the
 * configuration functions.
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

#ifndef LELY_UTIL_CONFIG_H
#define LELY_UTIL_CONFIG_H

#include <lely/util/util.h>

#include <stddef.h>

struct __config;
#ifndef __cplusplus
//! An opaque configuration type.
typedef struct __config config_t;
#endif

enum {
	//! Section and key names are case-insensitive.
	CONFIG_CASE = 1 << 0
};

// The file location struct from <lely/util/diag.h>.
struct floc;

#ifdef __cplusplus
extern "C" {
#endif

LELY_UTIL_EXTERN void *__config_alloc(void);
LELY_UTIL_EXTERN void __config_free(void *ptr);
LELY_UTIL_EXTERN struct __config *__config_init(struct __config *config,
		int flags);
LELY_UTIL_EXTERN void __config_fini(struct __config *config);

/*!
 * Creates a new configuration struct with an unnamed empty root section.
 *
 * \param flags either 0 or #CONFIG_CASE.
 *
 * \returns a pointer to a new configuration struct, or NULL on error. In the
 * latter case, the error number can be obtained with `get_errnum()`.
 *
 * \see config_destroy()
 */
LELY_UTIL_EXTERN config_t *config_create(int flags);

//! Destroys a configuration struct. \see config_create()
LELY_UTIL_EXTERN void config_destroy(config_t *config);

/*!
 * Retrieves a key from a configuration struct.
 *
 * \param config  a pointer to a configuration struct.
 * \param section a pointer to the name of the section. If \a section is NULL or
 *                "", the root section is used instead.
 * \param key     a pointer to the name of the key.
 *
 * \returns a pointer to the value of the key, or NULL if not found.
 */
LELY_UTIL_EXTERN const char *config_get(const config_t *config,
		const char *section, const char *key);

/*!
 * Sets a key in or removes a key from a configuration struct.
 *
 * \param config  a pointer to a configuration struct.
 * \param section a pointer to the name of the section. If \a section is NULL or
 *                "", the root section is used instead.
 * \param key     a pointer to the name of the key. This name is duplicated on
 *                success, so the caller need not guarantee the lifetime of the
 *                string.
 * \param value   a pointer to the new value of the key. If \a value is NULL,
 *                the key is removed. Like \a key, the string is duplicated on
 *                success.
 *
 * \returns a pointer to the value (which will differ from \a value due to
 * duplication), or NULL on error or when the key is deleted. In case of an
 * error, the error number can be obtained with `get_errnum()`.
 */
LELY_UTIL_EXTERN const char *config_set(config_t *config,
		const char *section, const char *key, const char *value);

/*!
 * Parses an INI file and adds the keys to a configuration struct.
 *
 * \returns the number of characters read. Parsing errors are reported with
 * diag_at().
 *
 * \see config_parse_ini_text()
 */
LELY_UTIL_EXTERN size_t config_parse_ini_file(config_t *config,
		const char *filename);

/*!
 * Parses a string in INI-format and adds the keys to a configuration struct.
 *
 * \param config a pointer to a configuration struct.
 * \param begin  a pointer to the first character in the string.
 * \param end    a pointer to one past the last character in the string (can be
 *               NULL if the string is null-terminated).
 * \param at     an optional pointer to the file location of \a begin (used for
 *               diagnostic purposes). On exit, if `at != NULL`, *\a at points
 *               to one past the last character parsed.
 *
 * \returns the number of characters read. Parsing errors are reported with
 * diag_at().
 *
 * \see config_parse_ini_file()
 */
LELY_UTIL_EXTERN size_t config_parse_ini_text(config_t *config,
		const char *begin, const char *end, struct floc *at);

#ifdef __cplusplus
}
#endif

#endif

