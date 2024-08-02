#pragma once
#include <stdint.h>
#include <cstddef>
#include "MidiSink.h"

#include <string>

namespace Midi
{

  class DebugMidiSink : public MidiSink
  {
  private:
    bool _isConnected;

  public:
    DebugMidiSink();
    virtual void send(Midi::MidiMessage message);
    virtual void start();
    virtual void stop();
    virtual bool isConnected();
  };

}
