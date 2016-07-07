/*!\file
 * This header file is part of the utilities library; it contains the daemon
 * declarations.
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

#ifndef LELY_UTIL_DAEMON_H
#define LELY_UTIL_DAEMON_H

#include <lely/util/util.h>

enum {
	//! The status indicating the daemon has started.
	DAEMON_START,
	/*!
	 * The signal/status indicating the daemon MUST terminate/has
	 * terminated.
	 */
	DAEMON_STOP,
	//! The signal/status indicating the daemon SHOULD pause/has paused.
	DAEMON_PAUSE,
	/*!
	 * The signal/status indicating the daemon SHOULD continue/has continued
	 * normal operation.
	 */
	DAEMON_CONTINUE,
	//! The signal indicating the daemon SHOULD reload its configuration.
	DAEMON_RELOAD,
	//! The smallest possible value of a user-defined signal.
	DAEMON_USER_MIN,
	//! The largest possible value of a user-defined signal.
#ifdef _WIN32
	DAEMON_USER_MAX = DAEMON_USER_MIN + 128
#else
	DAEMON_USER_MAX = 255
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * The function type of a handler for daemon_signal(). Note that daemon handlers
 * are executed in a separate thread and do not interrupt the execution of the
 * main program.
 *
 * \param sig    the signal specified to daemon_signal().
 * \param handle the handle argument specified to daemon_set_handler().
 */
typedef void daemon_handler_t(int sig, void *handle);

/*!
 * Executes the supplied function as a POSIX daemon or Windows service. This
 * function MUST be called only once, before any threads have been created or
 * any files, sockets, etc. have been opened. On successful execution, this
 * function does not return until the daemon terminates.
 *
 * \param name a pointer to the name of the daemon. Depending on the platform,
 *             the name is used for diagnostic or registration purposes.
 * \param init a pointer to the initialization function executed before \a main
 *             (can be NULL).
 * \param main a pointer to the function to be executed as a daemon.
 * \param fini a pointer to the finalization function executed after \a main
 *             (can be NULL).
 * \param argc the number of arguments in \a argv (unused on Windows as the
 *             daemon receives its arguments directly from the Service Control
 *             Manager).
 * \param argv an array of pointers to arguments to \a init (unused on Windows).
 *             The array MUST be NULL-terminated, i.e., `argv[argc] == NULL`.
 *
 * \returns 0 on completion, or -1 on error. In the latter case, the error
 * number can be obtained with `get_errnum()`.
 */
LELY_UTIL_EXTERN int daemon_start(const char *name, int (*init)(int, char **),
		void (*main)(void), void (*fini)(void), int argc, char *argv[]);

/*!
 * Sends the stop signal to the daemon handler. This function is equivalent to
 * `daemon_signal(DAEMON_STOP)`. It is the responsibility of the user to invoke
 * `daemon_status(DAEMON_STOP)` once the daemon is stopped.
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 */
LELY_UTIL_EXTERN int daemon_stop(void);

/*!
 * Sends the pause signal to the daemon handler. This function is equivalent to
 * `daemon_signal(DAEMON_PAUSE)`. It is the responsibility of the user to invoke
 * `daemon_status(DAEMON_PAUSE)` once the daemon is paused.
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 */
LELY_UTIL_EXTERN int daemon_pause(void);

/*!
 * Sends the continue signal to the daemon handler. This function is equivalent
 * to `daemon_signal(DAEMON_CONTINUE)`. It is the responsibility of the user to
 * invoke `daemon_status(DAEMON_CONTINUE)` once the daemon continues its normal
 * operation.
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 */
LELY_UTIL_EXTERN int daemon_continue(void);

/*!
 * Sends the reload signal to the daemon handler. This function is equivalent to
 * `daemon_signal(DAEMON_RELOAD)`. The user does _not_ need to invoke
 * daemon_status() to acknowledge execution of the reload operation.
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 */
LELY_UTIL_EXTERN int daemon_reload(void);

/*!
 * Sends a signal to a daemon, triggering the execution of the daemon handler.
 * On POSIX platforms, the handler is executed asynchronously (in a separate
 * thread), and this function can safely be called from a `signal()` handler.
 */
LELY_UTIL_EXTERN int daemon_signal(int sig);

/*!
 * Sets the current daemon status (one of #DAEMON_START, #DAEMON_STOP,
 * #DAEMON_PAUSE or #DAEMON_CONTINUE). It is the responsibility of the user to
 * update the status in a timely manner after receiving a start, stop, pause or
 * continue signal.
 */
LELY_UTIL_EXTERN int daemon_status(int status);

/*!
 * Retrieves current daemon handler and handle argument.
 *
 * \param phandler the address at which to store a pointer to the current daemon
 *                 handler (can be NULL).
 * \param phandle  the address at which to store a pointer to the handle
 *                 argument of the current daemon handler (can be NULL).
 *
 * \see daemon_set_handler()
 */
LELY_UTIL_EXTERN void daemon_get_handler(daemon_handler_t **phandler,
		void **phandle);

/*!
 * Sets the current daemon handler and its (optional) handle argument. The
 * handler is invoked asynchronously (in a separate thread) after the reception
 * of a signal.
 *
 * \see daemon_get_handler()
 */
LELY_UTIL_EXTERN void daemon_set_handler(daemon_handler_t *handler,
		void *handle);

//! The default daemon_signal() handler. \see daemon_handler_t
LELY_UTIL_EXTERN void default_daemon_handler(int sig, void *handle);

#ifdef __cplusplus
}
#endif

#endif

