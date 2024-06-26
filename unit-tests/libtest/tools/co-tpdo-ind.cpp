/**@file
 * This file is part of the CANopen Library Unit Test Suite.
 *
 * @copyright 2021 N7 Space Sp. z o.o.
 *
 * Unit Test Suite was developed under a programme of,
 * and funded by, the European Space Agency.
 *
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <CppUTest/TestHarness.h>

#include <lely/co/val.h>

#include "lely-cpputest-ext.hpp"

#include "co-tpdo-ind.hpp"

size_t CoTpdoInd::num_called_ = 0;

co_tpdo_t* CoTpdoInd::pdo_ = nullptr;
co_unsigned32_t CoTpdoInd::ac_ = CO_UNSIGNED32_MAX;
const void* CoTpdoInd::ptr_ = nullptr;
size_t CoTpdoInd::n_ = SIZE_MAX;
void* CoTpdoInd::data_ = nullptr;

void
CoTpdoInd::Func(co_tpdo_t* const pdo, const co_unsigned32_t ac,
                const void* const ptr, const size_t n, void* const data) {
  num_called_++;

  pdo_ = pdo;
  ac_ = ac;
  ptr_ = ptr;
  n_ = n;
  data_ = data;
}

void
CoTpdoInd::Clear() {
  num_called_ = 0u;

  pdo_ = nullptr;
  ac_ = CO_UNSIGNED32_MAX;
  ptr_ = nullptr;
  n_ = SIZE_MAX;
  data_ = nullptr;
}

void
CoTpdoInd::Check(const co_tpdo_t* const pdo, const co_unsigned32_t ac,
                 const void* const ptr, const size_t n,
                 const void* const data) {
  CHECK_COMPARE(num_called_, >, 0);
  POINTERS_EQUAL(pdo, pdo_);
  CHECK_EQUAL(ac, ac_);
  if (ptr != nullptr) POINTERS_EQUAL(ptr, ptr_);
  CHECK_EQUAL(n, n_);
  POINTERS_EQUAL(data, data_);
}

void
CoTpdoInd::CheckPtrNotNull(const co_tpdo_t* const pdo, const co_unsigned32_t ac,
                           const size_t n, const void* const data) {
  CHECK_COMPARE(num_called_, >, 0);
  POINTERS_EQUAL(pdo, pdo_);
  CHECK_EQUAL(ac, ac_);
  POINTER_NOT_NULL(ptr_);
  CHECK_EQUAL(n, n_);
  POINTERS_EQUAL(data, data_);
}
