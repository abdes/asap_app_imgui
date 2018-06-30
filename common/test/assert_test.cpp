//        Copyright The Authors 2018.
//    Distributed under the 3-Clause BSD License.
//    (See accompanying file LICENSE or copy at
//   https://opensource.org/licenses/BSD-3-Clause)

#include <catch2/catch.hpp>

#include <common/assert.h>

#if 0
// TODO: Rewrite these tests with ASSERT macros throwing instead of abort
TEST(AssertTests, TestAssertFail) {
  ASSERT_DEATH(ASAP_ASSERT_FAIL(), "Assertion failed.*");
}

TEST(AssertTests, TestAssertPrecond) {
  /*
  A precondition of a blocxxi function has been violated.
  This indicates a bug in the client application using blocxxi

  file: 'e:\dev\projects\blocxxi\common\test\assert_test.cpp'
  line: 11
  function: AssertTests_TestAssertPrecond_Test::TestBody
  expression: 1 > 10

  stack:
  */
  auto regex =
      "A precondition.*\n"
      "This indicates.*\n"
      ".*\n"
      "file:.*\n"
      "line:.*\n"
      "function:.*\n"
      "expression: 1 > 10\n";
  ASSERT_DEATH(ASAP_ASSERT_PRECOND(1 > 10), regex);
}

TEST(AssertTests, TestAssert) {
  /*
  Assertion failed. Please file a bugreport at
  https://github.com/xxxxxxxxxxx/issues

  file: 'e:\dev\projects\blocxxi\common\test\assert_test.cpp'
  line: 57
  function: AssertTests_TestAssert_Test::TestBody
  expression: true == false

  stack:
  */
  auto regex =
      "Assertion.*\n"
      ".*\n"
      "file:.*\n"
      "line:.*\n"
      "function:.*\n"
      "expression: true == false\n"
      ".*\n"
      "stack:\n";
  ASSERT_DEATH(ASAP_ASSERT(true == false), regex);
}

TEST(AssertTests, TestAssertVal) {
  /*
  Assertion failed. Please file a bugreport at
  https://github.com/xxxxxxxxxxx/issues

  file: 'e:\dev\projects\blocxxi\common\test\assert_test.cpp'
  line: 63
  function: AssertTests_TestAssertVal_Test::TestBody
  expression: 1 > 10
  str: Value

  stack:
  */
  auto var = 100;
  auto regex =
      "Assertion.*\n"
      ".*\n"
      "file:.*\n"
      "line:.*\n"
      "function:.*\n"
      "expression: 1 > 10\n"
      "var:.*100\n.*"
      ".*\n"
      "stack:\n";
  ASSERT_DEATH(ASAP_ASSERT_VAL(1 > 10, var), regex);
}

TEST(AssertTests, TestAssertFailVal) {
  auto str = "Value";
  auto regex =
      "Assertion failed.*\n"
      ".*\n"
      "file:.*\n"
      "line:.*\n"
      "function:.*\n"
      "expression:.*\n"
      "str:.*Value\n";
  ASSERT_DEATH(ASAP_ASSERT_FAIL_VAL(str), regex);
}
#endif // 0

