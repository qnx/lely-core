/*!\file
 * This header file is part of the CAN library; it contains the CAN frame
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

#ifndef LELY_CAN_MSG_H
#define LELY_CAN_MSG_H

#include <lely/libc/stdint.h>
#include <lely/can/can.h>

#include <stddef.h>

//! The mask used to extract the 11-bit Base Identifier from a CAN frame.
#define CAN_MASK_BID	UINT32_C(0x000007ff)

//! The mask used to extract the 29-bit Extended Identifier from a CAN frame.
#define CAN_MASK_EID	UINT32_C(0x1fffffff)

/*!
 * The Identifier Extension (IDE) flag. If this flag is set, the CAN Extended
 * Format (with a 29-bit identifier) is used, otherwise the CAN Base Format
 * (with an 11-bit identifier) is used.
 */
#define CAN_FLAG_IDE	0x01

/*!
 * The Remote Transmission Request (RTR) flag (unavailable in CAN FD format
 * frames). If this flag is set, the frame has no payload.
 */
#define CAN_FLAG_RTR	0x02

#ifndef LELY_NO_CANFD

/*!
 * The Extended Data Length (EDL) flag. This flag is set for CAN FD format
 * frames.
 */
#define CAN_FLAG_EDL	0x04

/*!
 * The Bit Rate Switch (BRS) flag (only available in CAN FD format frames). If
 * this flag is set, the bit rate is switched from the standard bit rate of the
 * arbitration phase to the preconfigured alternate bit rate of the data phase.
 */
#define CAN_FLAG_BRS	0x08

/*!
 * The Error State Indicator (ESI) flag (only available in CAN FD format
 * frames).
 */
#define CAN_FLAG_ESI	0x10

#endif // !LELY_NO_CANFD

//! The maximum number of bytes in the payload of a CAN format frame.
#define CAN_MAX_LEN	8

#ifndef LELY_NO_CANFD
//! The maximum number of bytes in the payload of a CAN FD format frame.
#define CANFD_MAX_LEN	64
#endif

//! A CAN or CAN FD format frame.
struct can_msg {
	//! The identifier (11 or 29 bits, depending on the #CAN_FLAG_IDE flag).
	uint32_t id;
	/*!
	 * The flags (any combination of #CAN_FLAG_IDE, #CAN_FLAG_RTR,
	 * #CAN_FLAG_EDL, #CAN_FLAG_BRS and #CAN_FLAG_ESI).
	 */
	uint8_t flags;
	/*!
	 * The number of bytes in #data (or the requested number of bytes in
	 * case of a remote frame). The maximum value is 8 for CAN format frames
	 * and 64 for CAN FD format frames.
	 */
	uint8_t len;
	//! The frame payload (in case of a data frame).
#ifdef LELY_NO_CANFD
	uint8_t data[CAN_MAX_LEN];
#else
	uint8_t data[CANFD_MAX_LEN];
#endif
};

//! The static initializer for a #can_msg struct.
#ifdef LELY_NO_CANFD
#define CAN_MSG_INIT	{ 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } }
#else
#define CAN_MSG_INIT \
	{ \
		0, 0, 0, \
		{ \
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
		} \
	}
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Prints the contents of a CAN or CAN FD format frame to a string buffer. The
 * output mimics that of candump.
 *
 * \param s   the address of the output buffer. If \a s is not NULL, at most
 *            `n - 1` characters are written, plus a terminating null byte.
 * \param n   the size (in bytes) of the buffer at \a s. If \a n is zero,
 *            nothing is written.
 * \param msg a pointer to the CAN frame to be printed.
 *
 * \returns the number of characters that would have been written had the
 * buffer been sufficiently large, not counting the terminating null byte, or a
 * negative number on error. In the latter case, the error number is stored in
 * `errno`.
 */
LELY_CAN_EXTERN int snprintf_can_msg(char *s, size_t n,
		const struct can_msg *msg);

/*!
 * Equivalent to snprintf_can_msg(), except that it allocates a string large
 * enough to hold the output, including the terminating null byte.
 *
 * \param ps  the address of a value which, on success, contains a pointer to
 *            the allocated string. This pointer SHOULD be passed to `free()` to
 *            release the allocated storage.
 * \param msg a pointer to the CAN frame to be printed.
 *
 * \returns the number of characters written, not counting the terminating null
 * byte, or a negative number on error. In the latter case, the error number is
 * stored in `errno`.
 */
LELY_CAN_EXTERN int asprintf_can_msg(char **ps, const struct can_msg *msg);

#ifdef __cplusplus
}
#endif

#endif

