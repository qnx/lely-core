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

#include <cassert>

#include <CppUTest/TestHarness.h>

#include "can-send.hpp"

int CanSend::ret = 0;
void* CanSend::data = nullptr;
unsigned int CanSend::num_called = 0;
can_msg CanSend::msg = CAN_MSG_INIT;
int CanSend::bus_id = -1;
can_msg* CanSend::msg_buf = &CanSend::msg;
size_t CanSend::buf_size = 1u;

int
CanSend::Func(const can_msg* msg_, int bus_id_, void* data_) {
  assert(msg_);

  msg = *msg_;
  data = data_;
  bus_id = bus_id_;

  if (msg_buf != &msg && num_called < buf_size) {
    msg_buf[num_called] = *msg_;
  }
  num_called++;

  return ret;
}

void
CanSend::CheckMsg(const uint_least32_t id, const uint_least8_t flags,
                  const uint_least8_t len, const uint_least8_t* const data) {
  CHECK_EQUAL(id, msg.id);
  CHECK_EQUAL(flags, msg.flags);
  CHECK_EQUAL(len, msg.len);
  if (data != nullptr) {
    for (uint_least8_t i = 0; i < len; ++i) {
      CHECK_EQUAL(data[i], msg.data[i]);
    }
  }
}

void
CanSend::CheckSdoMsg(const co_unsigned32_t id_, const co_unsigned32_t flags_,
                     const uint_least8_t len_, const co_unsigned8_t cs_,
                     const co_unsigned16_t idx_, const co_unsigned8_t subidx_,
                     const co_unsigned32_t ac_) {
  CHECK_EQUAL(id_, msg.id);
  CHECK_EQUAL(flags_, msg.flags);
  CHECK_EQUAL(len_, msg.len);
  CHECK_SDO_CAN_MSG_CMD(cs_, msg.data);
  CHECK_SDO_CAN_MSG_IDX(idx_, msg.data);
  CHECK_SDO_CAN_MSG_SUBIDX(subidx_, msg.data);
  CHECK_SDO_CAN_MSG_AC(ac_, msg.data);
}

void
CanSend::Clear() {
  msg = CAN_MSG_INIT;
  data = nullptr;
  bus_id = -1;

  ret = 0;
  num_called = 0;

  buf_size = 1u;
  msg_buf = &msg;
}