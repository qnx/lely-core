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

#ifndef LELY_UNIT_TEST_CO_NMT_CS_IND_HPP_
#define LELY_UNIT_TEST_CO_NMT_CS_IND_HPP_

#include <functional>
#include <vector>

#include <lely/co/nmt.h>

/// Spy for #co_nmt_cs_ind_t indication function.
class CoNmtCsInd {
 public:
  using Seq = std::vector<co_unsigned8_t>;

  static void Func(co_nmt_t* nmt, co_unsigned8_t cs, void* data);
  static void Clear();
  static void Check(const co_nmt_t* const nmt, const co_unsigned8_t cs,
                    const void* const data);

  static inline size_t
  GetNumCalled() {
    return num_called_;
  }

  static void SetCheckSeq(const co_nmt_t* const nmt, const Seq& csSeq);

 private:
  static size_t num_called_;

  static const co_nmt_t* nmt_;
  static co_unsigned8_t cs_;
  static const void* data_;

  static std::function<co_nmt_cs_ind_t> checkFunc;
};

#endif  // LELY_UNIT_TEST_CO_NMT_CS_IND_HPP_
