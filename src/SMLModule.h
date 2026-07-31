#pragma once

#include "OpenKNX.h"
#include "SMLChannel.h"

class SMLModule : public OpenKNX::Module
{
    friend class SMLChannel;

  protected:
    SMLChannel *_channels[SML_ChannelCount] = {};
    uint8_t _currentChannel = 0;
    bool _debug = false;

  public:
    bool _lastReceivedStatus = false;
    uint32_t _lastReceivedByte = 0;
    uint32_t _lastReceivedFile = 0;
    OpenKNX::Led::FunctionGroup *_led = nullptr;
    void setup(bool configured) override;
    void loop(bool configured) override;
    void loopLed();
#ifdef OPENKNX_DUALCORE
    void setup1(bool configured) override;
    void loop1(bool configured) override;
#endif
    bool processCommand(const std::string command, bool diagnose) override;
    void showHelp() override;
    void processInputKo(GroupObject &ko) override;
    bool debug();
    SMLChannel *getChannel(uint8_t index);

    const std::string name() override;
    const std::string version() override;

    void ledHelper(OpenKNX::Led::FunctionGroup *_led, bool status, uint32_t &activity);

    HardwareSerial *serials[SML_ChannelCount];
};

extern SMLModule openknxSMLModule;