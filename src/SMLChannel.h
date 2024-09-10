#pragma once
#include "OpenKNX.h"
#include "sml/sml_list.h"
#include "sml/sml_value.h"

#ifndef OPENKNX_SML_BUFFER
    #define OPENKNX_SML_BUFFER 2048
#endif

class SMLChannel : public OpenKNX::Channel
{

  protected:
    uint16_t _bufferSize = 0;
    uint8_t _buffer[OPENKNX_SML_BUFFER] = {};
    HardwareSerial *_serial = nullptr;

    int64_t _sentCounterIn = 0;
    int64_t _sentCounterInT1 = 0;
    int64_t _sentCounterInT2 = 0;
    int64_t _sentCounterOut = 0;
    int64_t _sentCounterOutT1 = 0;
    int64_t _sentCounterOutT2 = 0;
    uint32_t _sentCounterInTime = 0;
    uint32_t _sentCounterInT1Time = 0;
    uint32_t _sentCounterInT2Time = 0;
    uint32_t _sentCounterOutTime = 0;
    uint32_t _sentCounterOutT1Time = 0;
    uint32_t _sentCounterOutT2Time = 0;

    double _sentDataPower = 0;
    double _sentDataPowerL1 = 0;
    double _sentDataPowerL2 = 0;
    double _sentDataPowerL3 = 0;
    uint32_t _sentDataPowerTime = 0;
    uint32_t _sentDataPowerL1Time = 0;
    uint32_t _sentDataPowerL2Time = 0;
    uint32_t _sentDataPowerL3Time = 0;

    double _sentDataCurrentL1 = 0;
    double _sentDataCurrentL2 = 0;
    double _sentDataCurrentL3 = 0;
    uint32_t _sentDataCurrentL1Time = 0;
    uint32_t _sentDataCurrentL2Time = 0;
    uint32_t _sentDataCurrentL3Time = 0;

    double _sentDataVoltageL1 = 0;
    double _sentDataVoltageL2 = 0;
    double _sentDataVoltageL3 = 0;
    uint32_t _sentDataVoltageL1Time = 0;
    uint32_t _sentDataVoltageL2Time = 0;
    uint32_t _sentDataVoltageL3Time = 0;

    double _sentDataFrequency = 0;
    uint32_t _sentDataFrequencyTime = 0;

    // double _sentDataCurrentL1 = 0;
    // double _sentDataCurrentL2 = 0;
    // double _sentDataCurrentL3 = 0;
    // double _sentDataVoltageL1 = 0;
    // double _sentDataVoltageL2 = 0;
    // double _sentDataVoltageL3 = 0;
    // double _sentDataFrequency = 0;

    bool moveBuffer(uint16_t length);
    int findSequence(const uint8_t *sequence, const size_t length);
    void parseBuffer();
    uint16_t crc16(uint8_t &byte, uint16_t crc);
    uint16_t removeEscaping(uint16_t length);
    bool processFile(const uint8_t *message, const size_t length);

    void processDataPoint(sml_list_entry *entry);
    void processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, boolean value);
    void processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, double value);
    void processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, char *value, uint8_t len);

    HardwareSerial *serial();

  public:
    SMLChannel(uint8_t index);

    void serial(HardwareSerial *serial);
    void setup() override;
    void loop() override;
    void processInputKo(GroupObject &ko) override;
    const std::string name() override;
    void writeBuffer(uint8_t byte);
    void resetBuffer();
};
