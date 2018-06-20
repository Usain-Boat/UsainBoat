//
// Created by Bas on 23-4-2018.
//

#ifndef BLINKY_DATABUFFER_H
#define BLINKY_DATABUFFER_H


#include <sched.h>
#include <cstring>

#define DATABUFFERSIZE  200

class Databuffer
{
public:
    Databuffer() : _index(0) { _flag_buffer_overflow = false; };

    Databuffer(uint8_t *data, uint8_t size);

    ~Databuffer() {};

    void append(uint8_t data);

    void append(uint8_t *data, uint8_t size);

    void clear();

    uint8_t *get_data();

    uint8_t get_index();

    bool get_overflow_status() { return _flag_buffer_overflow; };

    void set_overflow_status(bool status) { _flag_buffer_overflow = status; };

    uint8_t *operator[](const int index);

private:
    uint8_t _index;
    uint8_t _data[DATABUFFERSIZE];
    bool _flag_buffer_overflow;
};


#endif //BLINKY_DATABUFFER_H
