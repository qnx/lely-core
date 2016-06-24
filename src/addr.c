/*!\file
 * This file is part of the I/O library; it contains the implementation of the
 * network address functions.
 *
 * \see lely/io/addr.h
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

#include "io.h"
#include <lely/util/cmp.h>
#include <lely/util/errnum.h>
#include <lely/io/addr.h>

#include <assert.h>
#include <string.h>

LELY_IO_EXPORT int __cdecl
io_addr_cmp(const void *p1, const void *p2)
{
	if (p1 == p2)
		return 0;

	if (__unlikely(!p1))
		return -1;
	if (__unlikely(!p2))
		return 1;

	const io_addr_t *a1 = p1;
	const io_addr_t *a2 = p2;

	int cmp = memcmp(&a1->addr, &a2->addr, MIN(a1->addrlen, a2->addrlen));
	if (!cmp)
		cmp = (a2->addrlen < a1->addrlen) - (a1->addrlen < a2->addrlen);
	return cmp;
}

#if defined(_WIN32) || _POSIX_C_SOURCE >= 200112L

LELY_IO_EXPORT int
io_addr_get_ipv4_a(const io_addr_t *addr, char *ip, int *port)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr))) {
		set_errnum(ERRNUM_INVAL);
		return -1;
	}

	const struct sockaddr_in *addr_in =
			(const struct sockaddr_in *)&addr->addr;
	if (__unlikely(addr_in->sin_family != AF_INET)) {
		set_errnum(ERRNUM_AFNOSUPPORT);
		return -1;
	}

	if (port)
		*port = ntohs(addr_in->sin_port);
	if (ip && __unlikely(!inet_ntop(AF_INET, (void *)&addr_in->sin_addr, ip,
			IO_ADDR_IPV4_STRLEN)))
		return -1;

	return 0;
}

LELY_IO_EXPORT int
io_addr_set_ipv4_a(io_addr_t *addr, const char *ip, int port)
{
	assert(addr);

	memset(addr, 0, sizeof(*addr));
	addr->addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr->addr;

	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port);
	if (ip && *ip) {
		if (__unlikely(inet_pton(AF_INET, ip, &addr_in->sin_addr) != 1))
			return -1;
	} else {
		addr_in->sin_addr.s_addr = htonl(INADDR_ANY);
	}

	return 0;
}

LELY_IO_EXPORT int
io_addr_get_ipv4_n(const io_addr_t *addr, uint8_t ip[4], int *port)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr))) {
		set_errnum(ERRNUM_INVAL);
		return -1;
	}

	const struct sockaddr_in *addr_in =
			(const struct sockaddr_in *)&addr->addr;
	if (__unlikely(addr_in->sin_family != AF_INET)) {
		set_errnum(ERRNUM_AFNOSUPPORT);
		return -1;
	}

	if (port)
		*port = ntohs(addr_in->sin_port);
	if (ip)
		memcpy(ip, &addr_in->sin_addr.s_addr, 4);

	return 0;
}

LELY_IO_EXPORT void
io_addr_set_ipv4_n(io_addr_t *addr, const uint8_t ip[4], int port)
{
	assert(addr);

	memset(addr, 0, sizeof(*addr));
	addr->addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr->addr;

	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port);
	if (ip && *ip)
		memcpy(&addr_in->sin_addr.s_addr, ip, 4);
	else
		addr_in->sin_addr.s_addr = htonl(INADDR_ANY);
}

LELY_IO_EXPORT void
io_addr_set_ipv4_loopback(io_addr_t *addr, int port)
{
	assert(addr);

	memset(addr, 0, sizeof(*addr));
	addr->addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr->addr;

	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port);
	addr_in->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

LELY_IO_EXPORT void
io_addr_set_ipv4_broadcast(io_addr_t *addr, int port)
{
	assert(addr);

	memset(addr, 0, sizeof(*addr));
	addr->addrlen = sizeof(struct sockaddr_in);
	struct sockaddr_in *addr_in = (struct sockaddr_in *)&addr->addr;

	addr_in->sin_family = AF_INET;
	addr_in->sin_port = htons(port);
	addr_in->sin_addr.s_addr = htonl(INADDR_BROADCAST);
}

LELY_IO_EXPORT int
io_addr_get_ipv6_a(const io_addr_t *addr, char *ip, int *port)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr))) {
		set_errnum(ERRNUM_INVAL);
		return -1;
	}

	const struct sockaddr_in6 *addr_in6 =
			(const struct sockaddr_in6 *)&addr->addr;
	if (__unlikely(addr_in6->sin6_family != AF_INET6)) {
		set_errnum(ERRNUM_AFNOSUPPORT);
		return -1;
	}

	if (port)
		*port = ntohs(addr_in6->sin6_port);
	if (ip && __unlikely(!inet_ntop(AF_INET6, (void *)&addr_in6->sin6_addr,
			ip, IO_ADDR_IPV6_STRLEN)))
		return -1;

	return 0;
}

LELY_IO_EXPORT int
io_addr_set_ipv6_a(io_addr_t *addr, const char *ip, int port)
{
	assert(addr);

	memset(addr, 0, sizeof(*addr));
	addr->addrlen = sizeof(struct sockaddr_in6);
	struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&addr->addr;

	addr_in6->sin6_family = AF_INET6;
	addr_in6->sin6_port = htons(port);
	if (ip && *ip) {
		if (__unlikely(inet_pton(AF_INET6, ip, &addr_in6->sin6_addr)
				!= 1))
			return -1;
	} else {
		addr_in6->sin6_addr = in6addr_any;
	}

	return 0;
}

LELY_IO_EXPORT int
io_addr_get_ipv6_n(const io_addr_t *addr, uint8_t ip[16], int *port)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr))) {
		set_errnum(ERRNUM_INVAL);
		return -1;
	}

	const struct sockaddr_in6 *addr_in6 =
			(const struct sockaddr_in6 *)&addr->addr;
	if (__unlikely(addr_in6->sin6_family != AF_INET6)) {
		set_errnum(ERRNUM_AFNOSUPPORT);
		return -1;
	}

	if (port)
		*port = ntohs(addr_in6->sin6_port);
	if (ip)
		memcpy(ip, &addr_in6->sin6_addr.s6_addr, 16);

	return 0;
}

LELY_IO_EXPORT void
io_addr_set_ipv6_n(io_addr_t *addr, const uint8_t ip[16], int port)
{
	assert(addr);

	memset(addr, 0, sizeof(*addr));
	addr->addrlen = sizeof(struct sockaddr_in6);
	struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&addr->addr;

	addr_in6->sin6_family = AF_INET6;
	addr_in6->sin6_port = htons(port);
	if (ip && *ip)
		memcpy(&addr_in6->sin6_addr.s6_addr, ip, 16);
	else
		addr_in6->sin6_addr = in6addr_any;
}

LELY_IO_EXPORT void
io_addr_set_ipv6_loopback(io_addr_t *addr, int port)
{
	assert(addr);

	memset(addr, 0, sizeof(*addr));
	addr->addrlen = sizeof(struct sockaddr_in6);
	struct sockaddr_in6 *addr_in6 = (struct sockaddr_in6 *)&addr->addr;

	addr_in6->sin6_family = AF_INET6;
	addr_in6->sin6_port = htons(port);
	addr_in6->sin6_addr = in6addr_loopback;
}

LELY_IO_EXPORT int
io_addr_get_port(const io_addr_t *addr, int *port)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr))) {
		set_errnum(ERRNUM_INVAL);
		return -1;
	}

	switch (((const struct sockaddr *)&addr->addr)->sa_family) {
	case AF_INET: {
		const struct sockaddr_in *addr_in =
				(const struct sockaddr_in *)&addr->addr;
		if (port)
			*port = ntohs(addr_in->sin_port);
		return 0;
	}
	case AF_INET6: {
		const struct sockaddr_in6 *addr_in6 =
				(const struct sockaddr_in6 *)&addr->addr;
		if (port)
			*port = ntohs(addr_in6->sin6_port);
		return 0;
	}
	default:
		set_errnum(ERRNUM_AFNOSUPPORT);
		return -1;
	}
}

LELY_IO_EXPORT int
io_addr_set_port(io_addr_t *addr, int port)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr))) {
		set_errnum(ERRNUM_INVAL);
		return -1;
	}

	switch (((struct sockaddr *)&addr->addr)->sa_family) {
	case AF_INET:
		((struct sockaddr_in *)&addr->addr)->sin_port = htons(port);
		return 0;
	case AF_INET6:
		((struct sockaddr_in6 *)&addr->addr)->sin6_port = htons(port);
		return 0;
	default:
		set_errnum(ERRNUM_AFNOSUPPORT);
		return -1;
	}
}

LELY_IO_EXPORT int
io_addr_is_loopback(const io_addr_t *addr)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr)))
		return 0;

	switch (((const struct sockaddr *)&addr->addr)->sa_family) {
	case AF_INET: {
		const struct sockaddr_in *addr_in =
				(const struct sockaddr_in *)&addr->addr;
		return ntohl(addr_in->sin_addr.s_addr) == INADDR_LOOPBACK;
	}
	case AF_INET6: {
		const struct sockaddr_in6 *addr_in6 =
				(const struct sockaddr_in6 *)&addr->addr;
		return !memcmp(&addr_in6->sin6_addr, &in6addr_any,
				sizeof(in6addr_any));
	}
	default:
		return 0;
	}
}

LELY_IO_EXPORT int
io_addr_is_broadcast(const io_addr_t *addr)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr)))
		return 0;

	switch (((const struct sockaddr *)&addr->addr)->sa_family) {
	case AF_INET: {
		const struct sockaddr_in *addr_in =
				(const struct sockaddr_in *)&addr->addr;
		return ntohl(addr_in->sin_addr.s_addr) == INADDR_BROADCAST;
	}
	default:
		return 0;
	}
}

LELY_IO_EXPORT int
io_addr_is_multicast(const io_addr_t *addr)
{
	assert(addr);

	if (__unlikely(addr->addrlen < (int)sizeof(struct sockaddr)))
		return 0;

	switch (((const struct sockaddr *)&addr->addr)->sa_family) {
	case AF_INET: {
		const struct sockaddr_in *addr_in =
				(const struct sockaddr_in *)&addr->addr;
		return (ntohl(addr_in->sin_addr.s_addr) >> 28) == 0xe;
	}
	case AF_INET6: {
		const struct sockaddr_in6 *addr_in6 =
				(const struct sockaddr_in6 *)&addr->addr;
		return addr_in6->sin6_addr.s6_addr[0] == 0xff;
	}
	default:
		return 0;
	}
}

#endif // _WIN32 || _POSIX_C_SOURCE >= 200112L

