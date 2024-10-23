#include "SMLChannel.h"
#include "SMLModule.h"
#include <sml/sml_file.h>

const uint16_t SML_CRC_TABLE[256] PROGMEM =
    {0x0000, 0x1189, 0x2312, 0x329B, 0x4624, 0x57AD, 0x6536, 0x74BF, 0x8C48,
     0x9DC1, 0xAF5A, 0xBED3, 0xCA6C, 0xDBE5, 0xE97E, 0xF8F7, 0x1081, 0x0108,
     0x3393, 0x221A, 0x56A5, 0x472C, 0x75B7, 0x643E, 0x9CC9, 0x8D40, 0xBFDB,
     0xAE52, 0xDAED, 0xCB64, 0xF9FF, 0xE876, 0x2102, 0x308B, 0x0210, 0x1399,
     0x6726, 0x76AF, 0x4434, 0x55BD, 0xAD4A, 0xBCC3, 0x8E58, 0x9FD1, 0xEB6E,
     0xFAE7, 0xC87C, 0xD9F5, 0x3183, 0x200A, 0x1291, 0x0318, 0x77A7, 0x662E,
     0x54B5, 0x453C, 0xBDCB, 0xAC42, 0x9ED9, 0x8F50, 0xFBEF, 0xEA66, 0xD8FD,
     0xC974, 0x4204, 0x538D, 0x6116, 0x709F, 0x0420, 0x15A9, 0x2732, 0x36BB,
     0xCE4C, 0xDFC5, 0xED5E, 0xFCD7, 0x8868, 0x99E1, 0xAB7A, 0xBAF3, 0x5285,
     0x430C, 0x7197, 0x601E, 0x14A1, 0x0528, 0x37B3, 0x263A, 0xDECD, 0xCF44,
     0xFDDF, 0xEC56, 0x98E9, 0x8960, 0xBBFB, 0xAA72, 0x6306, 0x728F, 0x4014,
     0x519D, 0x2522, 0x34AB, 0x0630, 0x17B9, 0xEF4E, 0xFEC7, 0xCC5C, 0xDDD5,
     0xA96A, 0xB8E3, 0x8A78, 0x9BF1, 0x7387, 0x620E, 0x5095, 0x411C, 0x35A3,
     0x242A, 0x16B1, 0x0738, 0xFFCF, 0xEE46, 0xDCDD, 0xCD54, 0xB9EB, 0xA862,
     0x9AF9, 0x8B70, 0x8408, 0x9581, 0xA71A, 0xB693, 0xC22C, 0xD3A5, 0xE13E,
     0xF0B7, 0x0840, 0x19C9, 0x2B52, 0x3ADB, 0x4E64, 0x5FED, 0x6D76, 0x7CFF,
     0x9489, 0x8500, 0xB79B, 0xA612, 0xD2AD, 0xC324, 0xF1BF, 0xE036, 0x18C1,
     0x0948, 0x3BD3, 0x2A5A, 0x5EE5, 0x4F6C, 0x7DF7, 0x6C7E, 0xA50A, 0xB483,
     0x8618, 0x9791, 0xE32E, 0xF2A7, 0xC03C, 0xD1B5, 0x2942, 0x38CB, 0x0A50,
     0x1BD9, 0x6F66, 0x7EEF, 0x4C74, 0x5DFD, 0xB58B, 0xA402, 0x9699, 0x8710,
     0xF3AF, 0xE226, 0xD0BD, 0xC134, 0x39C3, 0x284A, 0x1AD1, 0x0B58, 0x7FE7,
     0x6E6E, 0x5CF5, 0x4D7C, 0xC60C, 0xD785, 0xE51E, 0xF497, 0x8028, 0x91A1,
     0xA33A, 0xB2B3, 0x4A44, 0x5BCD, 0x6956, 0x78DF, 0x0C60, 0x1DE9, 0x2F72,
     0x3EFB, 0xD68D, 0xC704, 0xF59F, 0xE416, 0x90A9, 0x8120, 0xB3BB, 0xA232,
     0x5AC5, 0x4B4C, 0x79D7, 0x685E, 0x1CE1, 0x0D68, 0x3FF3, 0x2E7A, 0xE70E,
     0xF687, 0xC41C, 0xD595, 0xA12A, 0xB0A3, 0x8238, 0x93B1, 0x6B46, 0x7ACF,
     0x4854, 0x59DD, 0x2D62, 0x3CEB, 0x0E70, 0x1FF9, 0xF78F, 0xE606, 0xD49D,
     0xC514, 0xB1AB, 0xA022, 0x92B9, 0x8330, 0x7BC7, 0x6A4E, 0x58D5, 0x495C,
     0x3DE3, 0x2C6A, 0x1EF1, 0x0F78};

const uint8_t SML_ESCAPE[8] = {0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B};
const uint8_t SML_START[8] = {0x1B, 0x1B, 0x1B, 0x1B, 0x01, 0x01, 0x01, 0x01};
const uint8_t SML_END[5] = {0x1B, 0x1B, 0x1B, 0x1B, 0x1A};

SMLChannel::SMLChannel(uint8_t index)
{
    _channelIndex = index;
}

HardwareSerial *SMLChannel::getSerial()
{
    return _serial;
}

void SMLChannel::setSerial(HardwareSerial *serial)
{
    _serial = serial;
    getSerial()->begin(9600);
}

const std::string SMLChannel::name()
{
    return "SML";
}

void SMLChannel::setup()
{
}

void SMLChannel::loop()
{
    if (getSerial() == nullptr) return;

    while (getSerial()->available())
        writeBuffer(getSerial()->read());

    parseBuffer();
}

void SMLChannel::processInputKo(GroupObject &ko)
{
}

void SMLChannel::writeBuffer(uint8_t byte)
{
    openknxSMLModule.lastReceived = millis();

    if (_bufferSize >= OPENKNX_SML_BUFFER) moveBuffer(1);

    _buffer[_bufferSize] = byte;
    _bufferSize++;
}

void SMLChannel::parseBuffer()
{
    for (size_t i = 0; i < 10; i++)
    {
        if (_bufferSize < 8) return; // Kann noch keine Startsequenz haben da wenier als 8 zeichen

        int startPos = findSequence((const uint8_t *)&SML_START, 8);

        if (startPos < 0) // keine Startsequenz gefunden
        {
            moveBuffer(_bufferSize - 7); // nehme die letzte 7 zeichen und verwerfe alles davor
            return;
        };

        // sequenz weiter hinten gefunden daher schiebe an anfang
        if (startPos > 0)
        {
            moveBuffer(startPos);
        }

        int endPos = findSequence((const uint8_t *)&SML_END, 5);

        if (endPos < 0) break;
        endPos += 8;

        // found end but check if cheksum already founded
        if (endPos > _bufferSize) break;

        const uint8_t *checksum = _buffer + endPos - 2;
        const uint16_t crc_received = (checksum[1] << 8) | checksum[0];
        const uint8_t fillBytes = _buffer[endPos - 3];

        // berechne crc
        uint16_t crc = 0xFFFF; // reset
        for (size_t i = 0; i < endPos - 2; i++)
            crc = crc16(_buffer[i], crc);
        crc ^= 0xFFFF;

        if (crc == crc_received)
        {
            if (openknxSMLModule.debug())
            {
                logInfoP("Valid sml file found");
                logIndentUp();
                logHexInfoP(_buffer, endPos);
            }

            // bereit weiter verarbeitung vor.
            moveBuffer(8); // Schmeiße den Anfang weg
            endPos -= 8;

            // entferne escaping ohne dabei das nachfolgende möglich telegram zu verschieben
            const uint16_t len = removeEscaping(endPos) - fillBytes - 8 /* ENDSEQ */;

            processFile(_buffer, len);

            if (openknxSMLModule.debug())
            {
                logIndentDown();
            }
        }
        else
        {
            logErrorP("Invalid sml file (checksum %04X != %04X)", crc, crc_received);
            logHexErrorP(_buffer, endPos);
        }
        moveBuffer(endPos);
    }
}

int SMLChannel::findSequence(const uint8_t *sequence, const size_t length)
{
    int range = _bufferSize - length;
    if (range >= 0)
    {
        for (int i = 0; i <= range; i++)
        {
            if (i + length <= _bufferSize)
            {
                if (memcmp(sequence, _buffer + i, length) == 0)
                {
                    return i;
                }
            }
        }
    }

    return -1;
}

void SMLChannel::resetBuffer()
{
    getSerial()->flush();
    _bufferSize = 0;
}

bool SMLChannel::moveBuffer(uint16_t move)
{
    if (move > _bufferSize)
    {
        logErrorP("moveBuffer ERROR %i / %i", move, _bufferSize);
        return false;
    }
    if (_bufferSize - move > 0)
    {
        memmove(_buffer, _buffer + move, _bufferSize - move);
    }
    _bufferSize -= move;

    return true;
}

uint16_t SMLChannel::removeEscaping(uint16_t length)
{
    uint8_t found = 0;
    for (size_t i = 0; i < length; i++)
    {
        if (_buffer[i] == 0x1b)
            found++;
        else
            found = 0;

        // wenn 8x 0x1b gefunden wurde wurde eine escapesequenz escaped und muss nun entfernt werden
        // es wird nur innerhalb des bereiches gemove nicht wie bei moveTo der komplette buffer
        if (found == 8)
        {
            found = 0;
            memmove(_buffer + i + 1 - 4, _buffer + i + 1, length - i - 4);
            length -= 4;
            i -= 4;
        }
    }
    return length;
}

uint16_t SMLChannel::crc16(uint8_t &byte, uint16_t crc)
{
    return pgm_read_word_near(&SML_CRC_TABLE[(byte ^ crc) & 0xff]) ^ (crc >> 8 & 0xff);
}

bool SMLChannel::processFile(const uint8_t *message, const size_t length)
{
    sml_file *file = sml_file_parse((uint8_t *)message, length);
    for (int i = 0; i < file->messages_len; i++)
    {
        sml_message *message = file->messages[i];
        if (*message->message_body->tag == SML_MESSAGE_GET_LIST_RESPONSE)
        {
            sml_list *entry;
            sml_get_list_response *body;
            body = (sml_get_list_response *)message->message_body->data;
            for (entry = body->val_list; entry != NULL; entry = entry->next)
            {
                if (!entry->value)
                { // do not crash on null value
                    continue;
                }

                processDataPoint(entry);
            }
        }
    }

    // free the malloc'd memory
    sml_file_free(file);
    // logHexInfoP(_buffer, messageLen);

    return true;
}

void SMLChannel::processDataPoint(sml_list_entry *entry)
{
    const uint8_t a = entry->obj_name->str[0];
    const uint8_t b = entry->obj_name->str[1];
    const uint8_t c = entry->obj_name->str[2];
    const uint8_t d = entry->obj_name->str[3];
    const uint8_t e = entry->obj_name->str[4];
    const uint8_t f = entry->obj_name->str[5];

    char obis[10] = {};
    snprintf(obis, 9, "%02d.%02d.%02d", c, d, e);

    if (a != 1) return; // nur Stromzähler erlaubt

    if (((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_INTEGER) ||
        ((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_UNSIGNED))
    {

        double converted = sml_value_to_double(entry->value);

        int scaler = (entry->scaler) ? *entry->scaler : 0;
        int prec = -scaler;
        if (prec < 0) prec = 0;
        converted = converted * pow(10, scaler);

        processDataPoint(obis, a, b, c, d, e, f, (double)converted);
    }
    else if (entry->value->type == SML_TYPE_OCTET_STRING)
    {
        processDataPoint(obis, a, b, c, d, e, f, (char *)entry->value->data.bytes->str, entry->value->data.bytes->len);
    }
    else if (entry->value->type == SML_TYPE_BOOLEAN)
    {
        processDataPoint(obis, a, b, c, d, e, f, (bool)entry->value->data.boolean);
    }
}

void SMLChannel::processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, bool value)
{
    if (openknxSMLModule.debug())
        logInfoP("%s: %s", obis, (value ? "true" : "false"));
}

void SMLChannel::processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, double value)
{
    bool send = false;
    if (d == 8)
    {
        double counter = value;
        int64_t counterKwh = counter / 1000;
        int64_t counterWh = counter;
        if (openknxSMLModule.debug()) logInfoP("%s: %.3f kWh", obis, counter / 1000);

        if (knx.configured() && openknx.afterStartupDelay() && ParamSML_cType && ParamSML_cCounter)
        {
            if (c == 1 && e == 0)
            {
                if (ParamSML_cCounterChange && abs(_sentCounterIn - counterKwh) >= ParamSML_cCounterChangeV)
                    send = true;

                if (ParamSML_cCounterCyclic && (!_sentCounterInTime || delayCheck(_sentCounterInTime, ParamSML_cCounterCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentCounterIn = counterKwh;
                    _sentCounterInTime = millis();
                    KoSML_cCounterF1In.value(counterKwh, DPT_ActiveEnergy_kWh);
                }
                else
                {
                    KoSML_cCounterF1In.valueNoSend(counterKwh, DPT_ActiveEnergy_kWh);
                }

                if (SML_cCounterWh)
                    KoSML_cCounterF2In.valueCompare(counterWh, DPT_ActiveEnergy);
            }
            else if (c == 1 && e == 1 && (ParamSML_cType == 2 || ParamSML_cType == 4))
            {
                if (ParamSML_cCounterChange && abs(_sentCounterInT1 - counterKwh) >= ParamSML_cCounterChangeV)
                    send = true;

                if (ParamSML_cCounterCyclic && (!_sentCounterInT1Time || delayCheck(_sentCounterInT1Time, ParamSML_cCounterCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentCounterInT1 = counterKwh;
                    _sentCounterInT1Time = millis();
                    KoSML_cCounterF1InT1.value(counterKwh, DPT_ActiveEnergy_kWh);
                }
                else
                {
                    KoSML_cCounterF1InT1.valueNoSend(counterKwh, DPT_ActiveEnergy_kWh);
                }

                if (SML_cCounterWh)
                    KoSML_cCounterF2InT1.valueCompare(counterWh, DPT_ActiveEnergy);
            }
            else if (c == 1 && e == 2 && (ParamSML_cType == 2 || ParamSML_cType == 4))
            {
                if (ParamSML_cCounterChange && abs(_sentCounterInT2 - counterKwh) >= ParamSML_cCounterChangeV)
                    send = true;

                if (ParamSML_cCounterCyclic && (!_sentCounterInT2Time || delayCheck(_sentCounterInT2Time, ParamSML_cCounterCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentCounterInT2 = counterKwh;
                    _sentCounterInT2Time = millis();
                    KoSML_cCounterF1InT2.value(counterKwh, DPT_ActiveEnergy_kWh);
                }
                else
                {
                    KoSML_cCounterF1InT2.valueNoSend(counterKwh, DPT_ActiveEnergy_kWh);
                }

                if (SML_cCounterWh)
                    KoSML_cCounterF2InT2.valueCompare(counterWh, DPT_ActiveEnergy);
            }
            else if (c == 2 && e == 0 && (ParamSML_cType == 3 || ParamSML_cType == 4))
            {
                if (ParamSML_cCounterChange && abs(_sentCounterOut - counterKwh) >= ParamSML_cCounterChangeV)
                    send = true;

                if (ParamSML_cCounterCyclic && (!_sentCounterOutTime || delayCheck(_sentCounterOutTime, ParamSML_cCounterCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentCounterOut = counterKwh;
                    _sentCounterOutTime = millis();
                    KoSML_cCounterF1Out.value(counterKwh, DPT_ActiveEnergy_kWh);
                }
                else
                {
                    KoSML_cCounterF1Out.valueNoSend(counterKwh, DPT_ActiveEnergy_kWh);
                }

                if (SML_cCounterWh)
                    KoSML_cCounterF2Out.valueCompare(counterWh, DPT_ActiveEnergy);
            }
            else if (c == 2 && e == 1 && ParamSML_cType == 4)
            {
                if (ParamSML_cCounterChange && abs(_sentCounterOutT1 - counterKwh) >= ParamSML_cCounterChangeV)
                    send = true;

                if (ParamSML_cCounterCyclic && (!_sentCounterOutT1Time || delayCheck(_sentCounterOutT1Time, ParamSML_cCounterCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentCounterOutT1 = counterKwh;
                    _sentCounterOutT1Time = millis();
                    KoSML_cCounterF1OutT1.value(counterKwh, DPT_ActiveEnergy_kWh);
                }
                else
                {
                    KoSML_cCounterF1OutT1.valueNoSend(counterKwh, DPT_ActiveEnergy_kWh);
                }

                if (SML_cCounterWh)
                    KoSML_cCounterF2OutT1.valueCompare(counterWh, DPT_ActiveEnergy);
            }
            else if (c == 2 && e == 2 && ParamSML_cType == 4)
            {
                if (ParamSML_cCounterChange && abs(_sentCounterOutT2 - counterKwh) >= ParamSML_cCounterChangeV)
                    send = true;

                if (ParamSML_cCounterCyclic && (!_sentCounterOutT2Time || delayCheck(_sentCounterOutT2Time, ParamSML_cCounterCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentCounterOutT2 = counterKwh;
                    _sentCounterOutT2Time = millis();
                    KoSML_cCounterF1OutT2.value(counterKwh, DPT_ActiveEnergy_kWh);
                }
                else
                {
                    KoSML_cCounterF1OutT2.valueNoSend(counterKwh, DPT_ActiveEnergy_kWh);
                }

                if (SML_cCounterWh)
                    KoSML_cCounterF2OutT2.valueCompare(counterWh, DPT_ActiveEnergy);
            }
        }
    }
    else if (d == 7 && (c == 16 || c == 36 || c == 56 || c == 76))
    {
        if (openknxSMLModule.debug()) logInfoP("%s: %i Watt", obis, (int)value);

        if (knx.configured() && openknx.afterStartupDelay())
        {
            if (c == 16 && ParamSML_cPowerSum)
            {
                if (ParamSML_cPowerSumChange && fabs(_sentDataPower - value) >= ParamSML_cPowerSumChangeV)
                    send = true;

                if (ParamSML_cPowerSumCyclic && (!_sentDataPowerTime || delayCheck(_sentDataPowerTime, ParamSML_cPowerSumCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataPower = value;
                    _sentDataPowerTime = millis();
                    KoSML_cPower.value(value, DPT_Value_Power);
                }
                else
                {
                    KoSML_cPower.valueNoSend(value, DPT_Value_Power);
                }
            }
            else if (c == 36 && ParamSML_cPower)
            {
                if (ParamSML_cPowerChange && fabs(_sentDataPowerL1 - value) >= ParamSML_cPowerChangeV)
                    send = true;

                if (ParamSML_cPowerCyclic && (!_sentDataPowerL1Time || delayCheck(_sentDataPowerL1Time, ParamSML_cPowerCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataPowerL1 = value;
                    _sentDataPowerL1Time = millis();
                    KoSML_cPowerL1.value(value, DPT_Value_Power);
                }
                else
                {
                    KoSML_cPowerL1.valueNoSend(value, DPT_Value_Power);
                }
            }
            else if (c == 56 && ParamSML_cPower)
            {
                if (ParamSML_cPowerChange && fabs(_sentDataPowerL2 - value) >= ParamSML_cPowerChangeV)
                    send = true;

                if (ParamSML_cPowerCyclic && (!_sentDataPowerL2Time || delayCheck(_sentDataPowerL2Time, ParamSML_cPowerCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataPowerL2 = value;
                    _sentDataPowerL2Time = millis();
                    KoSML_cPowerL2.value(value, DPT_Value_Power);
                }
                else
                {
                    KoSML_cPowerL2.valueNoSend(value, DPT_Value_Power);
                }
            }
            else if (c == 76 && ParamSML_cPower)
            {
                if (ParamSML_cPowerChange && fabs(_sentDataPowerL3 - value) >= ParamSML_cPowerChangeV)
                    send = true;

                if (ParamSML_cPowerCyclic && (!_sentDataPowerL3Time || delayCheck(_sentDataPowerL3Time, ParamSML_cPowerCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataPowerL3 = value;
                    _sentDataPowerL3Time = millis();
                    KoSML_cPowerL3.value(value, DPT_Value_Power);
                }
                else
                {
                    KoSML_cPowerL3.valueNoSend(value, DPT_Value_Power);
                }
            }
        }
    }
    else if (d == 7 && (c == 31 || c == 51 || c == 71))
    {
        if (openknxSMLModule.debug()) logInfoP("%s: %.2f Ampere", obis, value);
        if (knx.configured() && openknx.afterStartupDelay() && ParamSML_cCurrent)
        {
            if (c == 31)
            {
                if (ParamSML_cCurrentChange && fabs(_sentDataCurrentL1 - value) >= (double)ParamSML_cCurrentChangeV / 10)
                    send = true;

                if (ParamSML_cCurrentCyclic && (!_sentDataCurrentL1Time || delayCheck(_sentDataCurrentL1Time, ParamSML_cCurrentCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataCurrentL1 = value;
                    _sentDataCurrentL1Time = millis();
                    KoSML_cCurrentL1.value(value, DPT_Value_Electric_Current);
                }
                else
                {
                    KoSML_cCurrentL1.valueNoSend(value, DPT_Value_Electric_Current);
                }
            }
            else if (c == 51)
            {
                if (ParamSML_cCurrentChange && fabs(_sentDataCurrentL2 - value) >= (double)ParamSML_cCurrentChangeV / 10)
                    send = true;

                if (ParamSML_cCurrentCyclic && (!_sentDataCurrentL2Time || delayCheck(_sentDataCurrentL2Time, ParamSML_cCurrentCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataCurrentL2 = value;
                    _sentDataCurrentL2Time = millis();
                    KoSML_cCurrentL2.value(value, DPT_Value_Electric_Current);
                }
                else
                {
                    KoSML_cCurrentL2.valueNoSend(value, DPT_Value_Electric_Current);
                }
            }
            else if (c == 71)
            {
                if (ParamSML_cCurrentChange && fabs(_sentDataCurrentL3 - value) >= (double)ParamSML_cCurrentChangeV / 10)
                    send = true;

                if (ParamSML_cCurrentCyclic && (!_sentDataCurrentL3Time || delayCheck(_sentDataCurrentL3Time, ParamSML_cCurrentCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataCurrentL3 = value;
                    _sentDataCurrentL3Time = millis();
                    KoSML_cCurrentL3.value(value, DPT_Value_Electric_Current);
                }
                else
                {
                    KoSML_cCurrentL3.valueNoSend(value, DPT_Value_Electric_Current);
                }
            }
        }
    }
    else if (d == 7 && (c == 32 || c == 52 || c == 72))
    {
        if (openknxSMLModule.debug()) logInfoP("%s: %.1f Volt", obis, value);
        if (knx.configured() && openknx.afterStartupDelay() && ParamSML_cVoltage)
        {
            if (c == 32)
            {
                if (ParamSML_cVoltageChange && fabs(_sentDataVoltageL1 - value) >= (double)ParamSML_cVoltageChangeV / 10)
                    send = true;

                if (ParamSML_cVoltageCyclic && (!_sentDataVoltageL1Time || delayCheck(_sentDataVoltageL1Time, ParamSML_cVoltageCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataVoltageL1 = value;
                    _sentDataVoltageL1Time = millis();
                    KoSML_cVoltageL1.value(value, DPT_Value_Electric_Potential);
                }
                else
                {
                    KoSML_cVoltageL1.valueNoSend(value, DPT_Value_Electric_Potential);
                }
            }
            else if (c == 52)
            {
                if (ParamSML_cVoltageChange && fabs(_sentDataVoltageL2 - value) >= (double)ParamSML_cVoltageChangeV / 10)
                    send = true;

                if (ParamSML_cVoltageCyclic && (!_sentDataVoltageL2Time || delayCheck(_sentDataVoltageL2Time, ParamSML_cVoltageCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataVoltageL2 = value;
                    _sentDataVoltageL2Time = millis();
                    KoSML_cVoltageL2.value(value, DPT_Value_Electric_Potential);
                }
                else
                {
                    KoSML_cVoltageL2.valueNoSend(value, DPT_Value_Electric_Potential);
                }
            }
            else if (c == 72)
            {
                if (ParamSML_cVoltageChange && fabs(_sentDataVoltageL3 - value) >= (double)ParamSML_cVoltageChangeV / 10)
                    send = true;

                if (ParamSML_cVoltageCyclic && (!_sentDataVoltageL3Time || delayCheck(_sentDataVoltageL3Time, ParamSML_cVoltageCyclicTimeMS)))
                    send = true;

                if (send)
                {
                    _sentDataVoltageL3 = value;
                    _sentDataVoltageL3Time = millis();
                    KoSML_cVoltageL3.value(value, DPT_Value_Electric_Potential);
                }
                else
                {
                    KoSML_cVoltageL3.valueNoSend(value, DPT_Value_Electric_Potential);
                }
            }
        }
    }
    else if (c == 14 && d == 7)
    {
        if (openknxSMLModule.debug()) logInfoP("%s: %.1f Herz", obis, value);
        if (knx.configured() && openknx.afterStartupDelay() && ParamSML_cFrequency)
        {
            if (ParamSML_cFrequencyChange && fabs(_sentDataFrequency - value) >= (double)ParamSML_cFrequencyChangeV / 10)
                send = true;

            if (ParamSML_cFrequencyCyclic && (!_sentDataFrequencyTime || delayCheck(_sentDataFrequencyTime, ParamSML_cFrequencyCyclicTimeMS)))
                send = true;

            if (send)
            {
                _sentDataFrequency = value;
                _sentDataFrequencyTime = millis();
                KoSML_cFrequency.value(value, DPT_Value_Frequency);
            }
            else
            {
                KoSML_cFrequency.valueNoSend(value, DPT_Value_Frequency);
            }
        }
    }
    else if (c == 81 && d == 7)
    {
        uint16_t degree = value;
        if (openknxSMLModule.debug()) logInfoP("%s: %i °", obis, degree);
    }
    else
    {
        if (openknxSMLModule.debug()) logInfoP("%s: %lf", obis, value);
    }
}

void SMLChannel::processDataPoint(char *obis, const uint8_t &a, const uint8_t &b, const uint8_t &c, const uint8_t &d, const uint8_t &e, const uint8_t &f, char *value, uint8_t len)
{
    char value2[len + 1] = "";
    memcpy(&value2, value, len);

    if (c == 96 && d == 50 && e == 1) // Hersteller
    {
        char vendor[4];
        sprintf(vendor, "%c%c%c", value[0], value[1], value[2]);

        if (openknxSMLModule.debug()) logInfoP("%s: %s", obis, vendor);
    }
    else if (c == 96 && d == 1 && e == 0) // Zähler ID
    {
        char identifier[15] = {};
        sprintf(identifier, "%i%c%c%c%02u%08u",
                value[1],                                                    // Sparte
                value[2], value[3], value[4],                                // Hersteller
                value[5],                                                    // Fabrikationsblock
                value[6] << 24 | value[7] << 16 | value[8] << 8 | value[9]); // Frabrikationsnummer

        if (openknxSMLModule.debug()) logInfoP("%s: %s", obis, identifier);

        if (knx.configured() && openknx.afterStartupDelay() && ParamSML_cIdentifikation)
        {
            if (KoSML_cIdentifier.initialized())
                KoSML_cIdentifier.valueNoSend(identifier, DPT_String_8859_1);
            else
                KoSML_cIdentifier.value(identifier, DPT_String_8859_1);
        }
    }
    else
    {
        if (openknxSMLModule.debug()) logInfoP("%s: %X", obis, value2);
    }
}
