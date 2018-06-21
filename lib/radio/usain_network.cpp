//
// Created by Jamie on 21-4-2018.
//

#include <LoRaWANInterface.h>
#include "usain_network.h"

//SX1276_Radio UsainNetwork::_radio(D11, D12, D13, D10, D0, D8, D1, D2, NC, NC, NC);
//SX1276_Radio UsainNetwork::_radio(PA_7, PA_6, PA_5, PD_14, PG_9, PF_12, PG_14, PF_15, PE_13, PF_14, PE_11);

UsainNetwork::UsainNetwork() :
    _radio(PA_7, PA_6, PA_5, PD_14, NC, PF_12, NC, NC, NC, NC, NC),
    n_rx_callbacks(0),
    n_tx_callbacks(0)
{

}

bool UsainNetwork::init()
{
  if (!_radio.self_test())
  {
    return false;
  }

  events.rx_done = callback(this, &UsainNetwork::rx_done_handler);
  events.tx_done = callback(this, &UsainNetwork::tx_done_handler);

  _radio.init_radio(&events);

  _radio.set_channel(868000000);

  _radio.set_rx_config(MODEM_FSK, // mode,
                       50000, // bw
                       50000, // dr
                       0, // cr
                       50000, // afc
                       5, // preemb
                       0, // timout
                       false, // len
                       0, // payl len
                       true, // crc
                       false, // frq hop
                       0, // hop p
                       false, // iq inv
                       true); // cont

  _radio.set_tx_config(MODEM_FSK, // modem
                       20, // power
                       25000, // fdev
                       0, // bw
                       50000, // dr
                       0, // cr
                       5, // preemb
                       false, // len
                       true, // crc
                       false, // freq hop
                       0, // hop p
                       false, // iq
                       3000); // timeout

  // put radio in rx continuous mode
  _radio.receive(0);

  return true;
}

void UsainNetwork::send(const UsainNetworkMessage &message)
{
  uint8_t buffer[UsainNetworkMessage::MAX_PACK_SIZE];
  uint8_t packet_size = message.to_byte_array(buffer);

  _radio.lock();

  while (!_radio.perform_carrier_sense(MODEM_FSK, 868000000, -80, 50));

  _radio.send(buffer, packet_size);

  _radio.unlock();
}

void UsainNetwork::register_message_received(const Callback<void(const UsainNetworkMessage &,
                                                                 UsainNetwork *network)> &callback)
{
  if (n_rx_callbacks > 32)
    error("Network RX callback overflow\n");

  _rx_callbacks[n_rx_callbacks++] = callback;
}

void UsainNetwork::register_message_send(const Callback<void()> &callback)
{
  if (n_tx_callbacks > 32)
    error("Network TX callback overflow\n");

  _tx_callbacks[n_tx_callbacks++] = callback;
}

void UsainNetwork::rx_done_handler(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr)
{
  UsainNetworkMessage msg(payload, size);

  for (int i = 0; i < n_rx_callbacks; i++)
  {
    _rx_callbacks[i].call(msg, this);
  }
}

void UsainNetwork::tx_done_handler()
{
  _radio.receive(0);

  for (int i = 0; i < n_tx_callbacks; i++)
  {
    _tx_callbacks[i].call();
  }
}
