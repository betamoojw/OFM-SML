#pragma once
#include "OpenKNX.h"
#include "sml/sml_list.h"
#include "sml/sml_value.h"

#ifndef OPENKNX_SML_BUFFER
    #define OPENKNX_SML_BUFFER 2048
#endif

#define SML_ChannelCount 1
#define ParamSML_VisibleChannels 2

class SMLChannel : public OpenKNX::Channel
{

  protected:
    uint16_t _bufferSize = 0;
    uint8_t _buffer[OPENKNX_SML_BUFFER] = {};

    bool moveBuffer(uint16_t length);
    int findSequence(const uint8_t *sequence, const size_t length);
    void parseBuffer();
    uint16_t crc16(uint8_t &byte, uint16_t crc);
    uint16_t removeEscaping(uint16_t length);
    bool processFile(const uint8_t *message, const size_t length);

    void processDataPoint(sml_list_entry *entry);
    void processDataPoint(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, boolean value);
    void processDataPoint(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, double value);
    void processDataPoint(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, char *value);

  public:
    SMLChannel(uint8_t index);

    HardwareSerial *serial();
    void setup() override;
    void loop() override;
    void processInputKo(GroupObject &ko) override;
    const std::string name() override;
    void writeBuffer(uint8_t byte);
    void resetBuffer();
};
