#include "DebugMidiSink.h"

Midi::DebugMidiSink::DebugMidiSink() : _isConnected(false) {}

void Midi::DebugMidiSink::send(Midi::MidiMessage message)
{
    if (_isConnected)
    {
        uint32_t buffer;
        size_t length = message.length();
        message.getData((uint8_t *)&buffer, 0, length);
        buffer = buffer >> 8;
        // ESP_LOGD(MidiDebugTAG, "%01u 0x%06X", length, buffer);
    }
}
void Midi::DebugMidiSink::start()
{
    _isConnected = true;
}
void Midi::DebugMidiSink::stop()
{
    _isConnected = false;
}
bool Midi::DebugMidiSink::isConnected()
{
    return _isConnected;
}