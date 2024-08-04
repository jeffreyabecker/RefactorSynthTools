
#pragma once
#include <stdint.h>
#include <cstddef>
#include "MidiMessages.h"
#include "MidiSink.h"

#include <string>

namespace Midi
{
  class CompositeMidiSink : public MidiSink
  {
  private:
    MidiSink &_a;
    MidiSink &_b;

  public:
    CompositeMidiSink(MidiSink &a, MidiSink &b) : _a(a), _b(b) {}
    virtual void send(Midi::MidiMessage message)
    {
      {
        _a.send(message);
        _b.send(message);
      }
    }
    virtual void start()
    {
      _a.start();
      _b.start();
    }
    virtual void stop()
    {
      _a.stop();
      _b.stop();
    }
    virtual bool isConnected() { return _a.isConnected() & _b.isConnected(); }
  };

}
