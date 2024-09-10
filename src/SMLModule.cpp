#include "SMLModule.h"

const std::string SMLModule::name()
{
    return "SML";
}

const std::string SMLModule::version()
{
    return MODULE_SML_Version;
}

void SMLModule::setup(bool configured)
{
    for (uint8_t i = 0; i < SML_ChannelCount; i++)
    {
        _channels[i] = new SMLChannel(i);
        _channels[i]->setup();
    }
}

void SMLModule::loop(bool configured)
{
    uint8_t processed = 0;
    do
        _channels[_currentChannel]->loop();

    while (openknx.freeLoopIterate(SML_ChannelCount, _currentChannel, processed));
}

void SMLModule::processInputKo(GroupObject &ko)
{
}

SMLChannel *SMLModule::getChannel(uint8_t index)
{
    return _channels[index];
}

bool SMLModule::processCommand(const std::string command, bool diagnose)
{
    if (!diagnose && command == "sml debug")
    {
        _debug = !_debug;

        if (_debug)
            logInfoP("SML Debug enabled");
        else
            logInfoP("SML Debug disabled");

        return true;
    }
    return false;
}

bool SMLModule::debug()
{
    return _debug;
}

SMLModule openknxSMLModule;