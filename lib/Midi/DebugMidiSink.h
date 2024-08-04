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
    DebugMidiSink() : _isConnected(false) {}
    virtual void send(Midi::MidiMessage message) override
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
    virtual void start() override { _isConnected = true; }
    virtual void stop() override { _isConnected = false; }
    virtual bool isConnected() override { return _isConnected; }
  };

}
