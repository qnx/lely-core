/**@file
 * This header file is part of the utilities library; it contains C++
 * convenience functions for creating std::error_code instances and throwing
 * std::system_error exceptions corresponding to native error codes.
 *
 * @copyright 2018-2020 Lely Industries N.V.
 *
 * @author J. S. Seldenthuis <jseldenthuis@lely.com>
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

#ifndef LELY_UTIL_ERROR_HPP_
#define LELY_UTIL_ERROR_HPP_

#include <lely/util/error.h>

#include <string>
#include <system_error>

#ifndef throw_or_abort
/**
 * If exceptions are disabled, aborts the process instead of throwing an
 * exception.
 */
#if __cpp_exceptions
#define throw_or_abort(e) throw(e)
#else
#define throw_or_abort(e) throw_or_abort_impl((e).what())
#endif
#endif

extern "C" {

/// Aborts the process instead of throwing an exception.
_Noreturn void throw_or_abort_impl(const char* what);

}  // extern "C"

namespace lely {
namespace util {

/**
 * Creates an error code value corresponding to the specified or current
 * (thread-specific) native error code.
 */
inline ::std::error_code
make_error_code(int errc = get_errc()) noexcept {
  return {errc, ::std::generic_category()};
}

/**
 * Throws an std::system_error exception corresponding to the specified error
 * code.
 */
[[noreturn]] inline void
throw_error_code(::std::errc e) {
  throw ::std::system_error(::std::make_error_code(e));
}

/**
 * Throws an std::system_error exception corresponding to the specified error
 * code. The string returned by the `what()` method of the resulting exception
 * is guaranteed to contain <b>what_arg</b> as a substring.
 */
[[noreturn]] inline void
throw_error_code(const ::std::string& what_arg, ::std::errc e) {
  throw ::std::system_error(::std::make_error_code(e), what_arg);
}

/// @see throw_error_code(const ::std::string& what_arg, ::std::errc e)
[[noreturn]] inline void
throw_error_code(const char* what_arg, ::std::errc e) {
  throw ::std::system_error(::std::make_error_code(e), what_arg);
}

/**
 * Throws an std::system_error exception corresponding to the specified or
 * current (thread-specific) native error code.
 */
[[noreturn]] inline void
throw_errc(int errc = get_errc()) {
  throw ::std::system_error(make_error_code(errc));
}

/**
 * Throws an std::system_error exception corresponding to the specified or
 * current (thread-specific) native error code. The string returned by the
 * `what()` method of the resulting exception is guaranteed to contain
 * <b>what_arg</b> as a substring.
 */
[[noreturn]] inline void
throw_errc(const ::std::string& what_arg, int errc = get_errc()) {
  throw ::std::system_error(make_error_code(errc), what_arg);
}

/// @see throw_errc(const ::std::string& what_arg, int errc)
[[noreturn]] inline void
throw_errc(const char* what_arg, int errc = get_errc()) {
  throw ::std::system_error(make_error_code(errc), what_arg);
}

/**
 * Throws an std::system_error exception corresponding to the specified
 * platform-independent error code.
 */
[[noreturn]] inline void
throw_errnum(errnum_t errnum) {
  throw_errc(errnum2c(errnum));
}

/**
 * Throws an std::system_error exception corresponding to the specified
 * platform-independent error code. The string returned by the `what()`
 * method of the resulting exception is guaranteed to contain <b>what_arg</b> as
 * a substring.
 */
[[noreturn]] inline void
throw_errnum(const ::std::string& what_arg, errnum_t errnum) {
  throw_errc(what_arg, errnum2c(errnum));
}

/// @see throw_errnum(const ::std::string& what_arg, int errnum)
[[noreturn]] inline void
throw_errnum(const char* what_arg, errnum_t errnum) {
  throw_errc(what_arg, errnum2c(errnum));
}

}  // namespace util
}  // namespace lely

#endif  // !LELY_UTIL_ERROR_HPP_
