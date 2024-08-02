#pragma once
#include <stdint.h>
#include <cstddef>
#include "MidiMessages.h"

#include <string>

namespace Midi
{

  /// @brief Writes MIDI messages out to a physical transport. Examples include sending midi over BLE, UART, or some custom physical transport layer
  class MidiSink
  {
  public:
    virtual void send(Midi::MidiMessage message) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isConnected() = 0;
  };

}
