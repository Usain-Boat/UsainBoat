//
// Created by Bas on 18-4-2018.
//

#define STX '$'
#define ETX1 0x0D
#define ETX2 0x0A

#include "drv_gps.h"
#include <string.h>


const uint8_t GPS_ACK[] = "001";
const uint8_t GPS_SYSTEM_MESSAGE[] = "010";
const uint8_t GPS_TXT_MESSAGE[] = "011";
const uint8_t GPS_CMD_HOT_START[] = "101";
const uint8_t GPS_CMD_WARM_START[] = "102";
const uint8_t GPS_CMD_COLD_START[] = "103";
const uint8_t GPS_CMD_FULL_COLD_START[] = "104";
const uint8_t GPS_SET_UPDATERATE[] = "220";
const uint8_t GPS_SET_BAUDRATE[] = "251";


DigitalOut testled(LED2);
//Serial UART(USBTX, USBRX, 57600);

uint8_t hex2asc0(uint8_t c);

uint8_t hex2asc1(uint8_t c);

AdafruitUltimateGPS::AdafruitUltimateGPS() : _UART(D1, D0, 9600)
//AdafruitUltimateGPS::AdafruitUltimateGPS() : _UART(USBTX, USBRX, 57600)
{
    _UART.attach(callback(this, &AdafruitUltimateGPS::_RXInterrupt), Serial::RxIrq);
}

AdafruitUltimateGPS::~AdafruitUltimateGPS() {

}

void AdafruitUltimateGPS::writeregister(uint8_t *packet_type) {
    _sendstring.clear();

    _sendstring.append((uint8_t *) "$PMTK", 5);
    _sendstring.append(packet_type, 3);
    _sendstring.append('*');

    uint8_t checksum = 0;
    for (uint8_t i = 1; i < 50; i++)
    {
        if (*_sendstring[i] == '*')
        {
            break;
        }
        checksum ^= *_sendstring[i];
    }

    _sendstring.append(hex2asc0(checksum));
    _sendstring.append(hex2asc1(checksum));
    _sendstring.append('\r');
    _sendstring.append('\n');

    for (uint8_t i = 0; i < _sendstring.get_index(); i++)
    {
        _UART.putc(*_sendstring[i]);
    }
}

void AdafruitUltimateGPS::writeregister(uint8_t *packet_type, uint8_t *parameter1_data, uint8_t parameter1_size) {
    _sendstring.clear();

    _sendstring.append((uint8_t *) "$PMTK", 5);
    _sendstring.append(packet_type, 3);
    _sendstring.append(',');
    _sendstring.append(parameter1_data, parameter1_size);
    _sendstring.append('*');

    uint8_t checksum = 0;
    for (uint8_t i = 1; i < 80; i++)
    {
        if (*_sendstring[i] == '*')
        {
            break;
        }
        checksum ^= *_sendstring[i];
    }
    _sendstring.append(hex2asc0(checksum));
    _sendstring.append(hex2asc1(checksum));
    _sendstring.append('\r');
    _sendstring.append('\n');

    for (uint8_t i = 0; i < _sendstring.get_index(); i++)
    {
        _UART.putc(*_sendstring[i]);
    }
}

void AdafruitUltimateGPS::writeregister(uint8_t *packet_type, uint8_t *parameter1_data, uint8_t parameter1_size,
                                        uint8_t *parameter2_data, uint8_t parameter2_size) {
    _sendstring.clear();

    _sendstring.append((uint8_t *) "$PMTK", 5);
    _sendstring.append(packet_type, 3);
    _sendstring.append(',');
    _sendstring.append(parameter1_data, parameter1_size);
    _sendstring.append(',');
    _sendstring.append(parameter2_data, parameter2_size);
    _sendstring.append('*');

    uint8_t checksum = 0;
    for (uint8_t i = 1; i < 80; i++)
    {
        if (*_sendstring[i] == '*')
        {
            break;
        }
        checksum ^= *_sendstring[i];
    }
    _sendstring.append(hex2asc0(checksum));
    _sendstring.append(hex2asc1(checksum));
    _sendstring.append('\r');
    _sendstring.append('\n');

    for (uint8_t i = 0; i < _sendstring.get_index(); i++)
    {
        _UART.putc(*_sendstring[i]);
    }
}


void AdafruitUltimateGPS::parsedata() {
    if (_flag_received_valid_string)
    {
        //check if message is a PMTK message or an GPRMC message, else reset flags and array
        if (memcmp((char *) _receivestring.get_data(), "PMTK", 4) &&
            memcmp((char *) _receivestring.get_data(), "GPRMC", 5))
        {
            _receivestring.clear();
            _flag_received_valid_string = false;
            return;
        }

        //CRC check
        _CRCCheck((char *) _receivestring.get_data());

        char *p_begin, *p_end;

        p_begin = (char *) _receivestring[0];
        p_end = strchr((char *) _receivestring[0], ','); // search for first ',' in string
        if (p_end == NULL)
        {
            p_end = strchr((char *) _receivestring[0], '*');// if comma isn't found, seach for '*'
            if (p_end == NULL)
            {
                _receivestring.clear();
                _flag_received_valid_string = false;
                return; //error end of string not found
            }
        }
        memcpy(_last_received_message.messagetype, p_begin, p_end - p_begin); // copy string
        _last_received_message.messagetype[p_end - p_begin] = 0;// end array with a 0 character

        _last_received_message.number_of_parameters = 0;    //reset number of parameters

        p_begin = strchr((char *) _receivestring[0], ','); //look for first parameter in string
        while (p_begin != NULL)     //if no more parameters, stop while loop
        {
            p_end = strchr(p_begin + 1, ','); // search for first ',' in string
            if (p_end == NULL)
            {
                p_end = strchr(p_begin + 1, '*'); // if comma isn't found, seach for '*'
                if (p_end == NULL)
                {
                    _receivestring.clear();
                    _flag_received_valid_string = false;
                    return; //error end of string not found
                }
            }

            //copy the parameter and put an 0 character on the end.
            memcpy(&_last_received_message.parameters[_last_received_message.number_of_parameters][0], p_begin + 1,
                   p_end - p_begin - 1);
            _last_received_message.parameters[_last_received_message.number_of_parameters][p_end - p_begin - 1] = 0;
            _last_received_message.number_of_parameters++; //raise the number of parameters

            if (*p_end == '*')  // if last character is an '*', break while loop
            {
                break;
            }
            p_begin = p_end;
        }
        if (!memcmp((char *) &_last_received_message.messagetype, "GPRMC", 5))
        {
            memcpy((char *) &_last_received_gprmc.time_stamp, (char *) &_last_received_message.parameters[0][0], 10);
            memcpy((char *) &_last_received_gprmc.validity, (char *) &_last_received_message.parameters[1][0], 10);
            memcpy((char *) &_last_received_gprmc.latitude, (char *) &_last_received_message.parameters[2][0], 10);
            memcpy((char *) &_last_received_gprmc.north_south, (char *) &_last_received_message.parameters[3][0], 10);
            memcpy((char *) &_last_received_gprmc.longitude, (char *) &_last_received_message.parameters[4][0], 10);
            memcpy((char *) &_last_received_gprmc.east_west, (char *) &_last_received_message.parameters[5][0], 10);
            memcpy((char *) &_last_received_gprmc.speed, (char *) &_last_received_message.parameters[6][0], 10);
            memcpy((char *) &_last_received_gprmc.course, (char *) &_last_received_message.parameters[7][0], 10);
            memcpy((char *) &_last_received_gprmc.date_stamp, (char *) &_last_received_message.parameters[8][0], 10);
            memcpy((char *) &_last_received_gprmc.variation_degrees, (char *) &_last_received_message.parameters[9][0],
                   10);
            memcpy((char *) &_last_received_gprmc.variation_east_west,
                   (char *) &_last_received_message.parameters[10][0], 10);
            memcpy((char *) &_last_received_gprmc.mode_indicator, (char *) &_last_received_message.parameters[11][0],
                   10);

            float degrees =
                    ((_last_received_gprmc.latitude[0] - 0x30) * 10) + (_last_received_gprmc.latitude[1] - 0x30);
            float length = strtof((char *) &_last_received_gprmc.latitude[2], NULL);
            _last_received_gprmc.latitude_fixed = degrees + (length / 60);
            degrees =
                    ((_last_received_gprmc.longitude[0] - 0x30) * 100) + ((_last_received_gprmc.longitude[1] - 0x30) * 10) + (_last_received_gprmc.longitude[2] - 0x30);
            length = strtof((char *) &_last_received_gprmc.longitude[3], NULL);

            _last_received_gprmc.longitude_fixed = degrees + (length / 60);

            _flag_gprmc_message_received = true;

        }
        else
        {
            _flag_pktm_message_received = true;
        }

        //clear receive string and flag, so the interrupt can receive a new message
        _receivestring.clear();
        _flag_received_valid_string = false;


    }
}

void AdafruitUltimateGPS::_RXInterrupt() {
    static bool _flag_stx_received = false; //local flag if begin message char is  received
    static bool _flag_etx1_received = false; //local flag if begin message char is  received

    uint8_t getchar = _UART.getc();

    //eror check
    if (_flag_received_valid_string)
    {
        return; //the received string must be parsed first before another string can be
    }
    if (_receivestring.get_overflow_status())
    {
        _receivestring.clear(); //if _receivestring overflows, reset the string
    }


    if (!_flag_stx_received && getchar == STX)
    {
        _flag_stx_received = true;  // if begin of message is received, start filling the _receivestring array
        _flag_etx1_received = false;
        _receivestring.clear();
    }
    else if (_flag_stx_received && _flag_etx1_received)
    {
        if (getchar != ETX2)
        {
            _receivestring.clear();
            _flag_etx1_received = false;
            _flag_stx_received = false;
            return;
        }
        _flag_stx_received = false;
        _flag_etx1_received = false;
        _flag_received_valid_string = true;
    }
    else if (_flag_stx_received && getchar == ETX1)
    {
        _flag_etx1_received = true;
    }
    else if (_flag_stx_received)
    {
        _receivestring.append(getchar);
    }
}

bool AdafruitUltimateGPS::_CRCCheck(char *gps_string) {

    char *p_begin, *p_end;

    p_end = strchr(gps_string, '*');    //search for the end of the message
    if (p_end == NULL)  //if '*' is not found, return false. message isn't valid
    {
        return false;
    }

    int calculated_crc = 0;
    for (p_begin = gps_string; p_begin < p_end; p_begin++)
    {
        calculated_crc ^= *p_begin; //CRC is based on an exor function from the begin to '*'
    }
    int crc = strtol(p_end + 1, NULL, 16);  // convert the number after * to a integer.

    if (crc != calculated_crc)  // check if calculated CRC is equal to the CRC in the message
    {
        return false;
    }
    return true;
}

bool AdafruitUltimateGPS::ReceievedNewGPRMC() {
    return _flag_gprmc_message_received;
}

void AdafruitUltimateGPS::ReceievedNewGPRMC(bool status) {
    _flag_gprmc_message_received = status;
}

void AdafruitUltimateGPS::GetLastGprmcData(gprmc_data *gpsdata) {
    memcpy(gpsdata, &_last_received_gprmc, sizeof(gprmc_data));
}

char AdafruitUltimateGPS::SetBaudrate(char *baudrate) {

    writeregister((uint8_t *) GPS_SET_BAUDRATE, (uint8_t *) baudrate, strlen(baudrate));

    Timer lookforflags;
    lookforflags.start();
    while (lookforflags.read() < 1)
    {
        parsedata();
        if (_flag_pktm_message_received && !memcmp(_last_received_message.messagetype + 4, GPS_ACK, 3) &&
            !memcmp(&_last_received_message.parameters[0][0], GPS_SET_BAUDRATE, 3))
        {
            return _last_received_message.parameters[1][0];
        }
    }
    return '4';
}

char AdafruitUltimateGPS::SetUpdaterate(char *updaterate) {

    writeregister((uint8_t *) GPS_SET_UPDATERATE, (uint8_t *) updaterate, strlen(updaterate));

    Timer lookforflags;
    lookforflags.start();
    while (lookforflags.read() < 1)
    {
        parsedata();
        if (_flag_pktm_message_received && !memcmp(_last_received_message.messagetype + 4, GPS_ACK, 3) &&
            !memcmp(&_last_received_message.parameters[0][0], GPS_SET_UPDATERATE, 3))
        {
            return _last_received_message.parameters[1][0];
        }
    }
    return '4';
}


uint8_t hex2asc0(uint8_t c) {
    uint8_t datah = c >> 4;

    if (datah < 10)
    {
        datah += 48;
    }
    else
    {
        datah += 55;
    }

    return datah;
}

uint8_t hex2asc1(uint8_t c) {
    uint8_t datal = c & 0x0F;

    if (datal < 10)
    {
        datal += 48;
    }
    else
    {
        datal += 55;
    }

    return datal;
}