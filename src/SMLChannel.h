#pragma once
#include "OpenKNX.h"
#include "sml/sml_list.h"
#include "sml/sml_value.h"
#ifdef ARDUINO_ARCH_RP2040
    #include "pico/sync.h"
#endif
#ifdef ARDUINO_ARCH_ESP32
    #include "freertos/semphr.h"
#endif

// Nimmt genau ein Telegramm zwischen Start- und Endsequenz auf. Übliche Zähler liegen
// bei rund 370 Byte, ein Dreiphasenzähler mit allen Messwerten deutlich darüber. Läuft
// der Puffer über, meldet der Kanal das als "No end sequence within ... bytes".
#ifndef OPENKNX_SML_BUFFER
    #define OPENKNX_SML_BUFFER 1024
#endif

#ifndef OPENKNX_SML_STALE_TIMEOUT
    #define OPENKNX_SML_STALE_TIMEOUT 5000
#endif

// Platzhalter für "keine Escape-Argumentgruppe gesehen", liegt außerhalb jeder Position
#define SML_NO_ESCAPE_ARG 0xFFFF

class SMLChannel : public OpenKNX::Channel
{

  protected:
    // Schützt die Übergabe von _smlBuffer zwischen writeBuffer() und processFile(),
    // die im Dualcore-Betrieb auf unterschiedlichen Kernen laufen.
#ifdef ARDUINO_ARCH_RP2040
    mutex_t _mutex;
#elif defined(ARDUINO_ARCH_ESP32)
    SemaphoreHandle_t _mutex = nullptr;
#endif
    uint16_t _bufferPos = 0;
    uint8_t _buffer[OPENKNX_SML_BUFFER] = {};
    // uint8_t *_currentFile = nullptr;
    // uint16_t _currentFileSize = 0;
    HardwareSerial *_serial = nullptr;
    sml_buffer *_smlBuffer = NULL;

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
    bool _capture = false;
    // Escape-Zustand der Rahmenerkennung: _escapePending ist gesetzt, sobald eine
    // Escape-Marke gelesen wurde und die nächste 4-Byte-Gruppe deren Argument ist.
    // _escapeArgPos merkt sich die zuletzt als Argument verbrauchte Gruppe.
    bool _escapePending = false;
    uint16_t _escapeArgPos = SML_NO_ESCAPE_ARG;
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

    bool _lastReceivedStatus = false;
    uint32_t _lastReceivedByte = 0;
    uint32_t _lastReceivedFile = 0;
    OpenKNX::Led::FunctionGroup *_led = nullptr;

    struct
    {
        uint8_t maxTariff = 0; // 0-255, in der Praxis nur einstellig
        bool bidirectional : 1; // Einspeisung (2.8.0) vorhanden
        bool power : 1;
        bool voltage : 1;
        bool current : 1;
        bool frequency : 1;
    } _features = {};

    // double _sentDataCurrentL1 = 0;
    // double _sentDataCurrentL2 = 0;
    // double _sentDataCurrentL3 = 0;
    // double _sentDataVoltageL1 = 0;
    // double _sentDataVoltageL2 = 0;
    // double _sentDataVoltageL3 = 0;
    // double _sentDataFrequency = 0;

    void beginCapture(uint16_t start);
    bool moveBuffer(uint16_t length);
    uint16_t crc16(uint8_t byte, uint16_t crc);
    void removeEscaping();
    void lockBuffer();
    bool tryLockBuffer();
    void unlockBuffer();
    void processFile();

    void processDataPoint(sml_list_entry *entry);
    void processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, boolean value);
    void processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, double value);
    void processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, char *value, uint8_t len);

  public:
    SMLChannel(uint8_t index);

    void setSerial(HardwareSerial *serial);
    HardwareSerial *getSerial();
    void setup(bool configured) override;
    void loop(bool configured) override;
    void loopStatus();
#if defined(OPENKNX_DUALCORE)
    void setup1(bool configured) override;
    void loop1(bool configured) override;
#endif
    void processInputKo(GroupObject &ko) override;
    const std::string name() override;
    void writeBuffer(uint8_t byte);
    bool isActive();
    // true, solange innerhalb von OPENKNX_SML_STALE_TIMEOUT ein Telegramm ankam
    bool receiveStatus();
    const std::string diagnoseInfo();
};
