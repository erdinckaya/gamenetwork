

#ifndef GAMENETWORK_SANDBOX_FACTORY_MESSAGE_TESTMESSAGE_H_
#define GAMENETWORK_SANDBOX_FACTORY_MESSAGE_TESTMESSAGE_H_
#include <yojimbo.h>
#include <limits>
// Test message.
// All messages must derived from `yojimbo::Message` and also
// All messages must implement stream member function.
// Note that this class must have default constructor.
class TestMessage : public yojimbo::Message
{
 public:
  int m_id{0};
  char m_text[100]{};


  // This function is used for serialization and deserialization.
  // You can give limits your messages. Messages are serialized by bit by bit
  // so that it creates very tiny data to send. It is better than json.
  // Moreover you dont need to duplicate your message definition for client and server
  // thanks to GameNetwork design.
  // Only drawback of this serialization you have to know your string max limit.
  template <typename Stream>
  bool Serialize(Stream& stream)
  {
    yojimbo_serialize_int(stream, m_id, 0, std::numeric_limits<int>::max());
    yojimbo_serialize_string(stream, m_text, 512);
    return true;
  }

  YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
};
#endif //GAMENETWORK_SANDBOX_FACTORY_MESSAGE_TESTMESSAGE_H_
