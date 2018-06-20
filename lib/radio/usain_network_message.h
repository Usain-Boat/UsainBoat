//
// Created by Jamie on 21-4-2018.
//

#ifndef RADIO_USAIN_NETWORK_MESSAGE_H
#define RADIO_USAIN_NETWORK_MESSAGE_H

#include <mbed.h>

class UsainNetworkMessage
{
 public:
  static const int MAX_PACK_SIZE = 256;

  typedef struct
  {
    char name[32];
    char value[16];
  } paramater_t;

  typedef enum
  {
    GET,
    POST,
    RESP,
    BCST,
    ERR
  } message_type_t;

  typedef struct
  {
    uint8_t type;
    uint16_t sequence;
    uint8_t source;
    uint8_t destination;
    uint8_t data_size;
    uint8_t data[246];
  }__attribute__((packed)) message_t;

  UsainNetworkMessage();

  explicit UsainNetworkMessage(uint8_t *src, uint8_t size);

  message_t get() const;

  message_type_t get_type() const;

  uint8_t get_source() const;

  uint8_t get_destination() const;

  uint8_t get_data_size() const;

  const uint8_t *get_data() const;

  void set_type(UsainNetworkMessage::message_type_t type);

  void set_source(uint8_t source);

  void set_destination(uint8_t destination);

  void set_data(uint8_t data[], uint8_t size);

  // used for GET messages
  void add_parameter(const char *name);

  void add_parameter(const char *name, char *value);

  void add_parameter(const char *name, int value);

  void add_parameter(const char *name, float value);

  int get_paramaters(UsainNetworkMessage::paramater_t *dest, uint8_t size) const;

  void from_byte_array(uint8_t src[], uint8_t size);

  uint8_t to_byte_array(uint8_t dst[]) const;

 private:
  message_t _current_message;
};

#endif //RADIO_USAIN_NETWORK_MESSAGE_H
