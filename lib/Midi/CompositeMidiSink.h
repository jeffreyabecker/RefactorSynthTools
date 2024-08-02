
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
    CompositeMidiSink(MidiSink &a, MidiSink &b);
    virtual void send(Midi::MidiMessage message);
    virtual void start();
    virtual void stop();
    virtual bool isConnected();
  };

}
