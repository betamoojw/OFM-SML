#pragma once

#include "OpenKNX.h"
#include "SMLChannel.h"

class SMLModule : public OpenKNX::Module
{

  protected:
    SMLChannel *_channels[SML_ChannelCount];
    uint8_t _currentChannel = 0;
    uint32_t _dummyTimer = 0;

  public:
    void setup(bool configured) override;
    void loop(bool configured) override;
    void processInputKo(GroupObject &ko) override;
    SMLChannel *getChannel(uint8_t index);

    const std::string name() override;
    const std::string version() override;

    HardwareSerial *serials[SML_ChannelCount];
};

extern SMLModule openknxSMLModule;