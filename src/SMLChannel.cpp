#include "SMLChannel.h"
#include <sml/sml_file.h>
#include "SMLModule.h"

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

HardwareSerial *SMLChannel::serial()
{
    return openknxSMLModule.serials[_channelIndex];
}

const std::string SMLChannel::name()
{
    return "SML";
}

void SMLChannel::setup()
{
    if (serial() == nullptr) return;

    serial()->begin(9600);
}

void SMLChannel::loop()
{
    if (serial() == nullptr) return;

    while (serial()->available())
        writeBuffer(serial()->read());

    parseBuffer();
}

void SMLChannel::processInputKo(GroupObject &ko)
{
}

void SMLChannel::writeBuffer(uint8_t byte)
{
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
            logDebugP("Valid SmlFile found");
            // logHexDebugP(_buffer, endPos);

            // bereit weiter verarbeitung vor.
            moveBuffer(8); // Schmeiße den Anfang weg
            endPos -= 8;

            // entferne escaping ohne dabei das nachfolgende möglich telegram zu verschieben
            const uint16_t len = removeEscaping(endPos) - fillBytes - 8 /* ENDSEQ */;

            processFile(_buffer, len);
        }
        else
        {
            logErrorP("Invalid checksum %04X != %04X", crc, crc_received);
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
        // logDebugP("findSequence: %i (%i) %i", length, _bufferSize, range);
        for (int i = 0; i <= range; i++)
        {
            if (i + length <= _bufferSize)
            {
                if (memcmp(sequence, _buffer + i, length) == 0)
                {
                    // logDebugP("  findSequence: found");
                    return i;
                }
            }
        }
    }

    return -1;
}

void SMLChannel::resetBuffer()
{
    serial()->flush();
    _bufferSize = 0;
}

bool SMLChannel::moveBuffer(uint16_t move)
{
    // logDebugP("moveBuffer: %i (%i)", move, _bufferSize);

    if (move > _bufferSize)
    {
        logErrorP("moveBuffer ERROR %i / %i", move, _bufferSize);
        return false;
    }
    if (_bufferSize - move > 0)
    {
        // logDebugP("move: %i (%i)", move, _bufferSize - move);
        memmove(_buffer, _buffer + move, _bufferSize - move);
    }
    _bufferSize -= move;

    // logDebugP("moveBuffer: done: %i", _bufferSize);
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

    char obisCompactIdentifier[10] = {};
    snprintf(obisCompactIdentifier, 9, "%02d.%02d.%02d", c, d, e);

    if (a != 1) return; // Nur Stromzähler
    if (b != 0) return; // Mir nur einem Channel
    // f ist in DE ungenutzt und immer 255

    if (((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_INTEGER) ||
        ((entry->value->type & SML_TYPE_FIELD) == SML_TYPE_UNSIGNED))
    {
        // if ((c == 1 || c == 2) && d == 8) // Firmware
        // {
        //     logDebugP("%s:   %i (%i)", obisCompactIdentifier, (int64_t)*entry->value->data.int64, (uint8_t)*entry->scaler);
        // }
        // else
        // {
        double converted = sml_value_to_double(entry->value);

        int scaler = (entry->scaler) ? *entry->scaler : 0;
        int prec = -scaler;
        if (prec < 0)
            prec = 0;
        converted = converted * pow(10, scaler);

        logDebugP("%s:   %lf", obisCompactIdentifier, (double)converted);
        processDataPoint(a, b, c, d, e, f, (double)converted);
        // }
    }
    else if (entry->value->type == SML_TYPE_OCTET_STRING)
    {
        if (c == 0 && d == 2 && e == 0) // Firmware
        {
            char text[entry->value->data.bytes->len + 1] = "";
            memcpy(&text, entry->value->data.bytes->str, entry->value->data.bytes->len);

            logDebugP("%s:   %s",
                      obisCompactIdentifier,
                      text);
        }
        else if (c == 96 && d == 50 && e == 1) // Hersteller
        {
            logDebugP("%s:   %c%c%c",
                      obisCompactIdentifier,
                      entry->value->data.bytes->str[0], entry->value->data.bytes->str[1], entry->value->data.bytes->str[2]);
        }
        else if (c == 96 && d == 1 && e == 0) // Zähler ID
        {
            logDebugP("%s:   %i %c%c%c %02u %08u",
                      obisCompactIdentifier,
                      entry->value->data.bytes->str[1],                                                                                                                            // Sparte
                      entry->value->data.bytes->str[2], entry->value->data.bytes->str[3], entry->value->data.bytes->str[4],                                                        // Hersteller
                      entry->value->data.bytes->str[5],                                                                                                                            // Fabrikationsblock
                      entry->value->data.bytes->str[6] << 24 | entry->value->data.bytes->str[7] << 16 | entry->value->data.bytes->str[8] << 8 | entry->value->data.bytes->str[9]); // Frabrikationsnummer
        }
        else
        {
            char *converted;
            sml_value_to_strhex(entry->value, &converted, false);
            logDebugP("%s:   %s", obisCompactIdentifier, (char *)converted);
            processDataPoint(a, b, c, d, e, f, (char *)converted);
            free(converted);
        }
    }
    else if (entry->value->type == SML_TYPE_BOOLEAN)
    {
        logDebugP("%s:   %s", obisCompactIdentifier, ((boolean)entry->value->data.boolean ? "true" : "false"));
        processDataPoint(a, b, c, d, e, f, (boolean)entry->value->data.boolean);
    }
}

void SMLChannel::processDataPoint(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, boolean value)
{
}

void SMLChannel::processDataPoint(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, double value)
{
}

void SMLChannel::processDataPoint(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, char *value)
{
}
