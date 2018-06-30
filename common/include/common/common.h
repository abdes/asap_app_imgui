#pragma once

#include <common/asap_common_api.h>

class ASAP_COMMON_API Common {
 public:
  void Hello();
};

template<int N>
class ASAP_COMMON_TEMPLATE_API TCommon {
 public:
  int Number() {
    return N;
  }
};