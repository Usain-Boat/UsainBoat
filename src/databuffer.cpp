//
// Created by Bas on 23-4-2018.
//

#include "databuffer.h"


Databuffer::Databuffer(uint8_t *data, uint8_t size) {
    if (size > DATABUFFERSIZE){
        return;
    }
    memcpy(&_data, data, size);
    _flag_buffer_overflow = false;
}

void Databuffer::append(uint8_t data) {
    if(_index >= DATABUFFERSIZE){
        return;
    }
    _data[_index] = data;
    _index++;
}

void Databuffer::append(uint8_t *data, uint8_t size) {
    if(size + _index >= DATABUFFERSIZE){
        return;
    }
    memcpy(&_data[_index], data, size);
    _index += size;
}

void Databuffer::clear() {
    for(uint8_t i = 0; i < DATABUFFERSIZE; i++){
        _data[i] = 0;
    }
    _index = 0;
    _flag_buffer_overflow = false;
}

uint8_t *Databuffer::get_data() {
    return _data;
}

uint8_t Databuffer::get_index() {
    return _index;
}

uint8_t* Databuffer::operator[](const int index) {
    return &_data[index];
}






