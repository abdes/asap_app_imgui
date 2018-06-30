#include <common/common.h>

#include <iostream>

#include <asap/asap-version.h>

void Common::Hello() {
  std::cout << ASAP_NAME_VERSION << std::endl;
  TCommon<10> com;
  std::cout << com.Number() << std::endl;
}

