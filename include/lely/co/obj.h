/*!\file
 * This header file is part of the CANopen library; it contains the object
 * dictionary declarations.
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

#ifndef LELY_CO_OBJ_H
#define LELY_CO_OBJ_H

#include <lely/co/type.h>

#include <stddef.h>

//! An object with no data fields.
#define CO_OBJECT_NULL		0x00

//! A large variable amount of data.
#define CO_OBJECT_DOMAIN	0x02

//! A type definitions.
#define CO_OBJECT_DEFTYPE	0x05

//! A record type definition.
#define CO_OBJECT_DEFSTRUCT	0x06

//! A single value.
#define CO_OBJECT_VAR		0x07

/*!
 * A multiple data field object where each data field is a simple variable of
 * the same basic data type.
 */
#define CO_OBJECT_ARRAY		0x08

/*!
 * A multiple data field object where the data fields may be any combination of
 * simple variables.
 */
#define CO_OBJECT_RECORD	0x09

//! Read-only access.
#define CO_ACCESS_RO	01

//! Write-only access.
#define CO_ACCESS_WO	02

//! Read/write access.
#define CO_ACCESS_RW	03

//! Read-only access (constant value).
#define CO_ACCESS_CONST	05

//! The lower limit of the object value is of the form `$NODEID { "+" number }`.
#define CO_OBJ_FLAGS_MIN_NODEID	0x01

//! The upper limit of the object value is of the form `$NODEID { "+" number }`.
#define CO_OBJ_FLAGS_MAX_NODEID	0x02

//! The default object value is of the form `$NODEID { "+" number }`.
#define CO_OBJ_FLAGS_DEF_NODEID	0x04

//! The current object value is of the form `$NODEID { "+" number }`.
#define CO_OBJ_FLAGS_VAL_NODEID	0x08

//! Refuse read on scan.
#define CO_OBJ_FLAGS_READ	0x10

//! Refuse write on download.
#define CO_OBJ_FLAGS_WRITE	0x20

//! The CANopen SDO upload/download request struct from <lely/co/sdo.h>.
struct co_sdo_req;

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * The type of a CANopen sub-object download indication function, invoked by an
 * SDO download request or Receive-PDO indication.
 *
 * \param sub  a pointer to a CANopen sub-object, containing the new value.
 * \param req  a pointer to a CANopen SDO download request. The \a size,
 *             \a buf, \a nbyte and \a offset members of *\a req are set by the
 *             caller.
 * \param data a pointer to user-specified data.
 *
 * \returns 0 on success, or an SDO abort code on error.
 */
typedef co_unsigned32_t co_sub_dn_ind_t(co_sub_t *sub, struct co_sdo_req *req,
		void *data);

/*!
 * The type of a CANopen sub-object upload indication function, invoked by an
 * SDO upload request.
 *
 * \param sub  a pointer to a CANopen sub-object, containing the new value.
 * \param req  a pointer to a CANopen SDO upload request. On the first
 *             invocation, the \a size member of *\a req is set to 0. All
 *             members MUST be initialized by the indication function.
 * \param data a pointer to user-specified data.
 *
 * \returns 0 on success, or an SDO abort code on error.
 */
typedef co_unsigned32_t co_sub_up_ind_t(const co_sub_t *sub,
		struct co_sdo_req *req, void *data);

LELY_CO_EXTERN void *__co_obj_alloc(void);
LELY_CO_EXTERN void __co_obj_free(void *ptr);
LELY_CO_EXTERN struct __co_obj *__co_obj_init(struct __co_obj *obj,
		co_unsigned16_t idx);
LELY_CO_EXTERN void __co_obj_fini(struct __co_obj *obj);

/*!
 * Creates a CANopen object.
 *
 * \param idx the object index.
 *
 * \returns a pointer to a new CANopen object, or NULL on error. In the latter
 * case, the error number can be obtained with `get_errnum()`.
 *
 * \see co_obj_destroy()
 */
LELY_CO_EXTERN co_obj_t *co_obj_create(co_unsigned16_t idx);

//! Destroys a CANopen object, including its sub-objects. \see co_obj_create()
LELY_CO_EXTERN void co_obj_destroy(co_obj_t *obj);

//! Returns a pointer to the CANopen device containing the specified object.
LELY_CO_EXTERN co_dev_t *co_obj_get_dev(const co_obj_t *obj);

//! Returns the index of a CANopen object.
LELY_CO_EXTERN co_unsigned16_t co_obj_get_idx(const co_obj_t *obj);

/*!
 * Retrieves a list of sub-indices in a CANopen object.
 *
 * \param obj       a pointer to a CANopen object.
 * \param maxsubidx the maximum number of sub-indices to return.
 * \param subidx    an array of at least \a maxsubidx indices (can be NULL). On
 *                  success, *\a idx contains the sub-indices.
 *
 * \returns the total number of sub-indices in the object (which may be
 * different from \a maxsubidx).
 */
LELY_CO_EXTERN co_unsigned8_t co_obj_get_subidx(const co_obj_t *obj,
		co_unsigned8_t maxsubidx, co_unsigned8_t *subidx);

/*!
 * Inserts a sub-object into a CANopen object. This function fails if the
 * sub-object is already part of another object, or of another sub-object with
 * the same sub-index already exists.
 *
 * \param obj a pointer to a CANopen object.
 * \param sub a pointer to the sub-object to be inserted.
 *
 * \returns 0 on success, or -1 on error.
 *
 * \see co_obj_remove_sub(), co_obj_find_sub()
 */
LELY_CO_EXTERN int co_obj_insert_sub(co_obj_t *obj, co_sub_t *sub);

/*!
 * Removes a sub-object from a CANopen object.
 *
 * \param obj a pointer to a CANopen object.
 * \param sub a pointer to the sub-object to be removed.
 *
 * \returns 0 on success, or -1 on error.
 *
 * \see co_obj_insert_sub()
 */
LELY_CO_EXTERN int co_obj_remove_sub(co_obj_t *obj, co_sub_t *sub);

/*!
 * Finds a sub-object in a CANopen object.
 *
 * \param obj    a pointer to a CANopen object.
 * \param subidx the object sub-index.
 *
 * \returns a pointer to the sub-object if found, or NULL if not.
 *
 * \see co_obj_insert_sub()
 */
LELY_CO_EXTERN co_sub_t *co_obj_find_sub(const co_obj_t *obj,
		co_unsigned8_t subidx);

//! Returns the name of a CANopen object. \see co_obj_set_name()
LELY_CO_EXTERN const char *co_obj_get_name(const co_obj_t *obj);

/*!
 * Sets the name of a CANopen object.
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 *
 * \see co_obj_get_name()
 */
LELY_CO_EXTERN int co_obj_set_name(co_obj_t *obj, const char *name);

//! Returns the object code of a CANopen object. \see co_obj_set_code()
LELY_CO_EXTERN co_unsigned8_t co_obj_get_code(const co_obj_t *obj);

/*!
 * Sets the code (type) of a CANopen object.
 *
 * \param obj  a pointer to a CANopen object.
 * \param code the object code (one of #CO_OBJECT_NULL, #CO_OBJECT_DOMAIN,
 *             #CO_OBJECT_DEFTYPE, #CO_OBJECT_DEFSTRUCT, #CO_OBJECT_VAR,
 *             #CO_OBJECT_ARRAY or #CO_OBJECT_RECORD).
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 *
 * \see co_obj_get_code()
 */
LELY_CO_EXTERN int co_obj_set_code(co_obj_t *obj, co_unsigned8_t code);

/*!
 * Returns the address of the value of a CANopen object. In case of compound
 * data types, this is a pointer to the struct or array containing the members.
 *
 * \see co_obj_sizeof_val()
 */
LELY_CO_EXTERN void *co_obj_addressof_val(const co_obj_t *obj);

/*!
 * Returns size (in bytes) of the value of a CANopen object.
 *
 * \see co_obj_addressof_val()
 */
LELY_CO_EXTERN size_t co_obj_sizeof_val(const co_obj_t *obj);

#define LELY_CO_DEFINE_TYPE(a, b, c, d) \
	LELY_CO_EXTERN co_##b##_t co_obj_get_val_##c(const co_obj_t *obj, \
			co_unsigned8_t subidx); \
	LELY_CO_EXTERN size_t co_obj_set_val_##c(co_obj_t *obj, \
			co_unsigned8_t subidx, co_##b##_t c);
#include <lely/co/def/basic.def>
#undef LELY_CO_DEFINE_TYPE

/*!
 * Sets the download indication function for a CANopen object. This function
 * invokes co_sub_set_dn_ind() for all sub-objects.
 *
 * \param obj  a pointer to a CANopen object.
 * \param ind  a pointer to the indication function. If \a ind is NULL, the
 *             default indication function will be used.
 * \param data a pointer to user-specified data (can be NULL). \a data is passed
 *             as the last parameter to \a func.
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with get_errnum().
 */
LELY_CO_EXTERN void co_obj_set_dn_ind(co_obj_t *obj, co_sub_dn_ind_t *ind,
		void *data);

/*!
 * Sets the upload indication function for a CANopen object. This function
 * invokes co_sub_set_up_ind() for all sub-objects.
 *
 * \param obj  a pointer to a CANopen object.
 * \param ind  a pointer to the indication function. If \a ind is NULL, the
 *             default indication function will be used.
 * \param data a pointer to user-specified data (can be NULL). \a data is passed
 *             as the last parameter to \a func.
 */
LELY_CO_EXTERN void co_obj_set_up_ind(co_obj_t *obj, co_sub_up_ind_t *ind,
		void *data);

LELY_CO_EXTERN void *__co_sub_alloc(void);
LELY_CO_EXTERN void __co_sub_free(void *ptr);
LELY_CO_EXTERN struct __co_sub *__co_sub_init(struct __co_sub *sub,
		co_unsigned8_t subidx, co_unsigned16_t type);
LELY_CO_EXTERN void __co_sub_fini(struct __co_sub *sub);

/*!
 * Creates a CANopen sub-object.
 *
 * \param subidx the object sub-index.
 * \param type   the data type of the sub-object value (in the range [1..27]).
 *               This MUST be the object index of one of the static data types.
 *
 * \returns a pointer to a new CANopen sub-object, or NULL on error. In the
 * latter case, the error number can be obtained with `get_errnum()`.
 *
 * \see co_sub_destroy()
 */
LELY_CO_EXTERN co_sub_t *co_sub_create(co_unsigned8_t subidx,
		co_unsigned16_t type);

//! Destroys a CANopen sub-object. \see co_sub_create()
LELY_CO_EXTERN void co_sub_destroy(co_sub_t *sub);

/*!
 * Returns the a pointer to the CANopen object containing the specified
 * sub-object.
 */
LELY_CO_EXTERN co_obj_t *co_sub_get_obj(const co_sub_t *sub);

//! Returns the sub-index of a CANopen sub-object.
LELY_CO_EXTERN co_unsigned8_t co_sub_get_subidx(const co_sub_t *sub);

//! Returns the name of a CANopen sub-object. \see co_sub_set_name()
LELY_CO_EXTERN const char *co_sub_get_name(const co_sub_t *sub);

/*!
 * Sets the name of a CANopen sub-object.
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 *
 * \see co_sub_get_name()
 */
LELY_CO_EXTERN int co_sub_set_name(co_sub_t *sub, const char *name);

//! Returns the data type of a CANopen sub-object.
LELY_CO_EXTERN co_unsigned16_t co_sub_get_type(const co_sub_t *sub);

/*!
 * Returns the address of the lower limit of the value of a CANopen sub-object.
 * In the case of strings or domains, this is the address of the first byte in
 * the array.
 *
 * \see co_sub_sizeof_min()
 */
LELY_CO_EXTERN const void *co_sub_addressof_min(const co_sub_t *sub);

/*!
 * Returns size (in bytes) of the lower limit of the value of a CANopen
 * sub-object. In the case of strings or domains, this is the number of bytes in
 * the array.
 *
 * \see co_sub_addressof_min()
 */
LELY_CO_EXTERN size_t co_sub_sizeof_min(const co_sub_t *sub);

/*!
 * Returns a pointer to the lower limit of the value of a CANopen sub-object. In
 * the case of strings or domains, this is the address of a pointer to the first
 * byte in the array.
 *
 * \see co_sub_set_min()
 */
LELY_CO_EXTERN const void *co_sub_get_min(const co_sub_t *sub);

/*!
 * Sets the lower limit of a value of a CANopen sub-object.
 *
 * \param sub a pointer to a CANopen sub-object.
 * \param ptr a pointer to the bytes to be copied. In case of strings or
 *            domains, \a ptr MUST point to the first byte in the array.
 * \param n   the number of bytes at \a ptr. In case of strings, \a n SHOULD
 *            exclude the terminating null byte(s).
 *
 * \returns the number of bytes copied (i.e., \a n), or 0 on error. In the
 * latter case, the error number can be obtained with `get_errnum()`.
 *
 * \see co_sub_get_min()
 */
LELY_CO_EXTERN size_t co_sub_set_min(co_sub_t *sub, const void *ptr, size_t n);

/*!
 * Returns the address of the upper limit of the value of a CANopen sub-object.
 * In the case of strings or domains, this is the address of the first byte in
 * the array.
 *
 * \see co_sub_sizeof_max()
 */
LELY_CO_EXTERN const void *co_sub_addressof_max(const co_sub_t *sub);

/*!
 * Returns size (in bytes) of the upper limit of the value of a CANopen
 * sub-object. In the case of strings or domains, this is the number of bytes in
 * the array.
 *
 * \see co_sub_addressof_max()
 */
LELY_CO_EXTERN size_t co_sub_sizeof_max(const co_sub_t *sub);

/*!
 * Returns a pointer to the upper limit of the value of a CANopen sub-object. In
 * the case of strings or domains, this is the address of a pointer to the first
 * byte in the array.
 *
 * \see co_sub_set_max()
 */
LELY_CO_EXTERN const void *co_sub_get_max(const co_sub_t *sub);

/*!
 * Sets the upper limit of a value of a CANopen sub-object.
 *
 * \param sub a pointer to a CANopen sub-object.
 * \param ptr a pointer to the bytes to be copied. In case of strings or
 *            domains, \a ptr MUST point to the first byte in the array.
 * \param n   the number of bytes at \a ptr. In case of strings, \a n SHOULD
 *            exclude the terminating null byte(s).
 *
 * \returns the number of bytes copied (i.e., \a n), or 0 on error. In the
 * latter case, the error number can be obtained with `get_errnum()`.
 *
 * \see co_sub_get_max()
 */
LELY_CO_EXTERN size_t co_sub_set_max(co_sub_t *sub, const void *ptr, size_t n);

/*!
 * Returns the address of the default value of a CANopen sub-object. In the case
 * of strings or domains, this is the address of the first byte in the array.
 *
 * \see co_sub_sizeof_def()
 */
LELY_CO_EXTERN const void *co_sub_addressof_def(const co_sub_t *sub);

/*!
 * Returns size (in bytes) of the default value of a CANopen sub-object. In the
 * case of strings or domains, this is the number of bytes in the array.
 *
 * \see co_sub_addressof_def()
 */
LELY_CO_EXTERN size_t co_sub_sizeof_def(const co_sub_t *sub);

/*!
 * Returns a pointer to the default value of a CANopen sub-object. In the case
 * of strings or domains, this is the address of a pointer to the first byte in
 * the array.
 *
 * \see co_sub_set_def()
 */
LELY_CO_EXTERN const void *co_sub_get_def(const co_sub_t *sub);

/*!
 * Sets the default value of a CANopen sub-object.
 *
 * \param sub a pointer to a CANopen sub-object.
 * \param ptr a pointer to the bytes to be copied. In case of strings or
 *            domains, \a ptr MUST point to the first byte in the array.
 * \param n   the number of bytes at \a ptr. In case of strings, \a n SHOULD
 *            exclude the terminating null byte(s).
 *
 * \returns the number of bytes copied (i.e., \a n), or 0 on error. In the
 * latter case, the error number can be obtained with `get_errnum()`.
 *
 * \see co_sub_get_def()
 */
LELY_CO_EXTERN size_t co_sub_set_def(co_sub_t *sub, const void *ptr, size_t n);

/*!
 * Returns the address of the current value of a CANopen sub-object. In the case
 * of strings or domains, this is the address of the first byte in the array.
 *
 * \see co_sub_sizeof_val()
 */
LELY_CO_EXTERN const void *co_sub_addressof_val(const co_sub_t *sub);

/*!
 * Returns size (in bytes) of the current value of a CANopen sub-object. In the
 * case of strings or domains, this is the number of bytes in the array.
 *
 * \see co_sub_addressof_val()
 */
LELY_CO_EXTERN size_t co_sub_sizeof_val(const co_sub_t *sub);

/*!
 * Returns a pointer to the current value of a CANopen sub-object. In the case
 * of strings or domains, this is the address of a pointer to the first byte in
 * the array.
 *
 * \see co_sub_set_val()
 */
LELY_CO_EXTERN const void *co_sub_get_val(const co_sub_t *sub);

/*!
 * Sets the current value of a CANopen sub-object.
 *
 * \param sub a pointer to a CANopen sub-object.
 * \param ptr a pointer to the bytes to be copied. In case of strings or
 *            domains, \a ptr MUST point to the first byte in the array.
 * \param n   the number of bytes at \a ptr. In case of strings, \a n SHOULD
 *            exclude the terminating null byte(s).
 *
 * \returns the number of bytes copied (i.e., \a n), or 0 on error. In the
 * latter case, the error number can be obtained with `get_errnum()`.
 *
 * \see co_sub_get_val()
 */
LELY_CO_EXTERN size_t co_sub_set_val(co_sub_t *sub, const void *ptr, size_t n);

#define LELY_CO_DEFINE_TYPE(a, b, c, d) \
	LELY_CO_EXTERN co_##b##_t co_sub_get_val_##c(const co_sub_t *sub); \
	LELY_CO_EXTERN size_t co_sub_set_val_##c(co_sub_t *sub, co_##b##_t c);
#include <lely/co/def/basic.def>
#undef LELY_CO_DEFINE_TYPE

//! Returns the access type of a CANopen sub-object. \see co_sub_set_access()
LELY_CO_EXTERN unsigned int co_sub_get_access(const co_sub_t *sub);

/*!
 * Sets the name of a CANopen sub-object.
 *
 * \param sub    a pointer to a CANopen sub-object.
 * \param access the access type (one of #CO_ACCESS_RO, #CO_ACCESS_WO,
 *               #CO_ACCESS_RW or #CO_ACCESS_CONST).
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 *
 * \see co_sub_get_access()
 */
LELY_CO_EXTERN int co_sub_set_access(co_sub_t *sub, unsigned int access);

/*!
 * Returns 1 if it is possible to map the specified CANopen sub-object into a
 * PDO, and 0 if not.
 *
 * \see co_sub_set_pdo_mapping()
 */
LELY_CO_EXTERN int co_sub_get_pdo_mapping(const co_sub_t *sub);

/*!
 * Enables or disables PDO mapping a CANopen sub-object.
 *
 * \see co_sub_get_pdo_mapping()
 */
LELY_CO_EXTERN void co_sub_set_pdo_mapping(co_sub_t *sub, int pdo_mapping);

//! Returns the object flags of a CANopen sub-object. \see co_sub_set_flags()
LELY_CO_EXTERN unsigned int co_sub_get_flags(const co_sub_t *sub);

//! Sets the object flags of a CANopen sub-object. \see co_sub_get_flags()
LELY_CO_EXTERN void co_sub_set_flags(co_sub_t *sub, unsigned int flags);

/*!
 * Retrieves the download indication function for a CANopen sub-object.
 *
 * \param sub   a pointer to a CANopen sub-object.
 * \param pind  the address at which to store a pointer to the indication
 *              function (can be NULL).
 * \param pdata the address at which to store a pointer to user-specified data
 *              (can be NULL).
 *
 * \see co_sub_set_dn_ind()
 */
LELY_CO_EXTERN void co_sub_get_dn_ind(const co_sub_t *sub,
		co_sub_dn_ind_t **pind, void **pdata);

/*!
 * Sets the download indication function for a CANopen sub-object. This function
 * is invoked by co_sub_dn_ind().
 *
 * \param sub  a pointer to a CANopen sub-object.
 * \param ind  a pointer to the indication function. If \a ind is NULL, the
 *             default indication function will be used.
 * \param data a pointer to user-specified data (can be NULL). \a data is passed
 *             as the last parameter to \a func.
 *
 * \see co_sub_get_dn_ind()
 */
LELY_CO_EXTERN void co_sub_set_dn_ind(co_sub_t *sub, co_sub_dn_ind_t *ind,
		void *data);

/*!
 * Retrieves the upload indication function for a CANopen sub-object.
 *
 * \param sub   a pointer to a CANopen sub-object.
 * \param pind  the address at which to store a pointer to the indication
 *              function (can be NULL).
 * \param pdata the address at which to store a pointer to user-specified data
 *              (can be NULL).
 *
 * \see co_sub_set_up_ind()
 */
LELY_CO_EXTERN void co_sub_get_up_ind(const co_sub_t *sub,
		co_sub_up_ind_t **pind, void **pdata);

/*!
 * Sets the upload indication function for a CANopen sub-object. This function
 * is invoked by co_sub_up_ind().
 *
 * \param sub  a pointer to a CANopen sub-object.
 * \param ind  a pointer to the indication function. If \a ind is NULL, the
 *             default indication function will be used.
 * \param data a pointer to user-specified data (can be NULL). \a data is passed
 *             as the last parameter to \a func.
 *
 * \see co_sub_get_up_ind()
 */
LELY_CO_EXTERN void co_sub_set_up_ind(co_sub_t *sub, co_sub_up_ind_t *ind,
		void *data);

/*!
 * Downloads (copies) a value into a CANopen sub-object if the
 * refuse-write-on-download flag (#CO_OBJ_FLAGS_WRITE) is _not_ set. This
 * function is invoked by the default download indication function.
 *
 * \param sub a pointer to a CANopen sub-object.
 * \param val a pointer to the value to be written. In the case of strings or
 *            domains, this MUST be the address of pointer (which is set to NULL
 *            if the value is copied).
 *
 * \returns 0 on success, or -1 on error. In the latter case, the error number
 * can be obtained with `get_errnum()`.
 *
 * \see co_val_move()
 */
LELY_CO_EXTERN int co_sub_dn(co_sub_t *sub, void *val);

#ifdef __cplusplus
}
#endif

#endif

