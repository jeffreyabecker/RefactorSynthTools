#include "CompositeMidiSink.h"

Midi::CompositeMidiSink::CompositeMidiSink(MidiSink &a, MidiSink &b) : _a(a), _b(b) {}
void Midi::CompositeMidiSink::send(Midi::MidiMessage message)
{
    _a.send(message);
    _b.send(message);
}
void Midi::CompositeMidiSink::start()
{
    _a.start();
    _b.start();
}
void Midi::CompositeMidiSink::stop()
{
    _a.stop();
    _b.stop();
}
bool Midi::CompositeMidiSink::isConnected()
{
    return _a.isConnected() & _b.isConnected();
}
