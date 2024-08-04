#pragma once
#include <stdint.h>
#include <cstddef>
#include "MidiMessages.h"
#include <functional>
#include <string>

namespace Midi
{

  /// @brief reads MidiMessages from the transport layer and processes them
  class MidiSource
  {
  protected:
    std::function<void(const MidiMessage &)> _recievedCallback = nullptr;

  public:
    MidiSource(std::function<void(const MidiMessage &)> recievedCallback) : _recievedCallback(recievedCallback) {}
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isConnected() = 0;
  };

}
