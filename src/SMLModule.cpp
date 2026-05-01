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
    _led = openknx.ledFunctions.get(200);

    for (uint8_t i = 0; i < SML_ChannelCount; i++)
    {
        _channels[i] = new SMLChannel(i);
        _channels[i]->setup(configured);
    }

    ledHelper(_led, false, _lastReceivedByte);
}

void SMLModule::loop(bool configured)
{
    uint8_t processed = 0;
    do
        _channels[_currentChannel]->loop(configured);

    while (openknx.freeLoopIterate(SML_ChannelCount, _currentChannel, processed));

    loopLed();
}

void SMLModule::loopLed()
{
    if (_led == nullptr) return;

    if (!_lastReceivedStatus && _lastReceivedFile != 0 && !delayCheck(_lastReceivedFile, 5000))
    {
        _lastReceivedStatus = true;
        ledHelper(_led, true, _lastReceivedByte);
    }
    else if (_lastReceivedStatus && delayCheck(_lastReceivedFile, 5000))
    {
        _lastReceivedStatus = false;
        ledHelper(_led, false, _lastReceivedByte);
    }
}

#ifdef OPENKNX_DUALCORE
void SMLModule::setup1(bool configured)
{
    for (uint8_t i = 0; i < SML_ChannelCount; i++)
    {
        _channels[i]->setup1(configured);
    }
}
void SMLModule::loop1(bool configured)
{
    for (uint8_t i = 0; i < SML_ChannelCount; i++)
    {
        _channels[i]->loop1(configured);
    }
}
#endif

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

void SMLModule::ledHelper(OpenKNX::Led::FunctionGroup *_led, bool status, uint32_t &activity)
{
    if (status)
    {
        _led->color(OpenKNX::Led::Color::Green);
        _led->activity(activity, true);
    }
    else
    {
        _led->color(OpenKNX::Led::Color::Red);
        _led->activity(activity, true, OpenKNX::Led::Capability::COLOR);
        _led->activity(activity, false, OpenKNX::Led::Capability::MONOCHROME);
    }
}

SMLModule openknxSMLModule;