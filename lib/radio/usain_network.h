//
// Created by Jamie on 21-4-2018.
//

#ifndef RADIO_USAIN_NETWORK_H
#define RADIO_USAIN_NETWORK_H

#include "mbed.h"
#include "drv_rfm95w.h"
#include "usain_network_message.h"

class UsainNetwork
{
public:
    UsainNetwork();

    bool init();

    void send(const UsainNetworkMessage &message);

    void register_message_received(const Callback<void(const UsainNetworkMessage &msg, UsainNetwork *network)> &callback);

    void register_message_send(const Callback<void()> &callback);

private:
    SX1276_Radio   _radio;
    radio_events_t events;

    int                                         n_rx_callbacks;
    Callback<void(const UsainNetworkMessage &msg, UsainNetwork *network)> _rx_callbacks[32];

    int              n_tx_callbacks;
    Callback<void()> _tx_callbacks[32];

    void rx_done_handler(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

    void tx_done_handler();
};

#endif //RADIO_USAIN_NETWORK_H
