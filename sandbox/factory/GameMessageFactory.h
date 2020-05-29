#pragma once

#include <yojimbo.h>
#include "message/GameMessageType.h"
#include "message/TestMessage.h"
// In order to use messages you have to define by using yojimbo macroes.
// They are basic macroes that overrides base message factory.
YOJIMBO_MESSAGE_FACTORY_START(GameMessageFactory, static_cast<int>(GameMessageType::COUNT))
YOJIMBO_DECLARE_MESSAGE_TYPE((int) GameMessageType::TEST, TestMessage)
YOJIMBO_MESSAGE_FACTORY_FINISH()