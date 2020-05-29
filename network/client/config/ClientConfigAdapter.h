#pragma once

#include <yojimbo.h>
#include "../../../sandbox/factory/GameMessageFactory.h"
// This adapter creates connection with yojimbo, but its callbacks are really poor.
// It can be upgraded with adding new features into yojimbo source code.
class ClientConfigAdapter : public yojimbo::Adapter {
  yojimbo::MessageFactory *CreateMessageFactory(yojimbo::Allocator &allocator) override {

    return YOJIMBO_NEW(allocator, GameMessageFactory, allocator);
  }
};
