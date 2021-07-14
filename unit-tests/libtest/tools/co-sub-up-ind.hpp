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

#ifndef LELY_UNIT_TEST_CO_SUB_UP_IND_HPP_
#define LELY_UNIT_TEST_CO_SUB_UP_IND_HPP_

#include <lely/co/type.h>
#include <lely/co/sdo.h>

struct CoSubUpInd {
  static size_t num_called;
  static const co_sub_t* sub;
  static co_sdo_req* req;
  static co_unsigned32_t ac;
  static void* data;

  static co_unsigned32_t Func(const co_sub_t* sub_, co_sdo_req* req_,
                              co_unsigned32_t ac_, void* data_);
  static void Clear();

  static inline bool
  Called() {
    return num_called > 0;
  }
};

#endif  // LELY_UNIT_TEST_CO_SUB_UP_IND_HPP_
