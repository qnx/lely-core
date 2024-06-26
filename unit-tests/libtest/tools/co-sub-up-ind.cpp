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

#include <lely/co/dev.h>
#include <lely/co/detail/obj.h>
#include <lely/co/obj.h>

#include "co-sub-up-ind.hpp"

size_t CoSubUpInd::num_called = 0u;
const co_sub_t* CoSubUpInd::sub = nullptr;
co_sdo_req* CoSubUpInd::req = nullptr;
co_unsigned32_t CoSubUpInd::ac = 0u;
void* CoSubUpInd::data = nullptr;
co_unsigned32_t CoSubUpInd::ret = 0u;
co_sdo_req* CoSubUpInd::ret_req = nullptr;

co_unsigned32_t
CoSubUpInd::Func(const co_sub_t* const sub_, co_sdo_req* const req_,
                 const co_unsigned32_t ac_, void* const data_) {
  num_called++;

  sub = sub_;
  req = req_;
  ac = ac_;
  data = data_;

  if (ac != 0) {
    return ac;
  } else if (ret != 0u) {
    return ret;
  } else {
    CHECK_EQUAL(0, co_sub_on_up(sub, req, &ac));

    if (ret_req != nullptr) {
      req->offset = ret_req->offset;
      req->nbyte = ret_req->nbyte;
      req->size = ret_req->size;
    }

    return ac;
  }
}

void
CoSubUpInd::Clear() {
  num_called = 0;

  sub = nullptr;
  req = nullptr;
  ac = 0;
  data = nullptr;

  ret = 0;
  ret_req = nullptr;
}
