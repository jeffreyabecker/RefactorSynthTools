#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "MidiNote.h"

namespace Midi
{

  // https://midi.org/summary-of-midi-1-0-messages
  enum class MidiMessageStatus : uint8_t
  {
    // Channel Specific messages
    NoteOff = 0x80,
    NoteOn = 0x90,
    PolyphonicKeyPressure = 0xA0,
    ControlChange = 0xB0, // has special values 120-127
    ProgramChange = 0xc0,
    ChannelPressure = 0xd0,
    PitchBendChange = 0xe0,
    // System Common Messages
    SystemExclusive = 0xf0,
    TimeCodeQuarterFrame = 0xf1,
    SongPositionPointer = 0xf2,
    SongSelect = 0xf3,
    TuneRequest = 0xf6,
    EndofExclusive = 0xf7,
    // System Realtime messages
    TimingClock = 0xf8,
    Start = 0xfa,
    Continue = 0xfb,
    Stop = 0xfc,
    ActiveSensing = 0xfe,
    Reset = 0xff,

  };

  class MidiMessage
  {
  public:
    MidiMessage(const MidiMessage &that) : _length(that._length),
                                           _status(that._status),
                                           _arg1(that._arg1),
                                           _arg2(that._arg2)
    {
    }
    inline MidiMessageStatus status() { return (((uint8_t)_status) & 0xF0) == 0xF0 ? _status : (MidiMessageStatus)(((uint8_t)_status) & 0xF0); }
    inline uint8_t length() { return _length; }
    virtual void getData(uint8_t *buffer, size_t offset, size_t length)
    {
      if (offset + length < _length)
      {
        uint8_t *source = (uint8_t *)((&(_status)) + offset);
        memcpy(buffer, source, length);
      }
    }

  protected:
    uint8_t _length;
    MidiMessageStatus _status;
    uint8_t _arg1;
    uint8_t _arg2;
    MidiMessage(uint8_t length, MidiMessageStatus status, uint8_t arg1, uint8_t arg2) : _length(length),
                                                                                        _status(status),
                                                                                        _arg1(arg1 & 0x7F),
                                                                                        _arg2(arg2 & 0x7F)
    {
    }
  };

  namespace Messages
  {
    class MidiChannelMessage : public MidiMessage
    {
    public:
      MidiChannelMessage(const MidiChannelMessage &that) : MidiMessage(that) {}
      inline uint8_t channel() { return (uint8_t)(_status) & 0x0F; }

    protected:
      MidiChannelMessage(MidiMessageStatus status, uint8_t channel, uint8_t arg1, uint8_t arg2) : MidiMessage(2, (MidiMessageStatus)((((uint8_t)status) & 0xF0) | (channel & 0x0F)), arg1, arg2) {}
      MidiChannelMessage(MidiMessageStatus status, uint8_t channel, uint8_t arg1) : MidiMessage(1, (MidiMessageStatus)((((uint8_t)status) & 0xF0) | (channel & 0x0F)), arg1, 0x00) {}
    };
    class MidiChannelNoteMessage : public MidiChannelMessage
    {
    public:
      MidiChannelNoteMessage(MidiMessageStatus status, uint8_t channel, MidiNote note, uint8_t velocity) : MidiChannelMessage(status, channel, (uint8_t)note, velocity) {}
      MidiChannelNoteMessage(const MidiChannelNoteMessage &that) : MidiChannelMessage(that) {}
      inline MidiNote note() { return MidiNote(_arg1); }
      inline uint8_t velocity() { return _arg2; }
    };
    class NoteOff : public MidiChannelNoteMessage
    {
    public:
      NoteOff(uint8_t channel, MidiNote note, uint8_t velocity) : MidiChannelNoteMessage(MidiMessageStatus::NoteOff, channel, note, velocity) {}
      NoteOff(const NoteOff &that) : MidiChannelNoteMessage(that) {}
    };
    class NoteOn : public MidiChannelNoteMessage
    {
    public:
      NoteOn(uint8_t channel, MidiNote note, uint8_t velocity) : MidiChannelNoteMessage(MidiMessageStatus::NoteOn, channel, note, velocity) {}
      NoteOn(const NoteOn &that) : MidiChannelNoteMessage(that) {}
    };


    class PolyphonicKeyPressure : public MidiChannelNoteMessage
    {
    public:
      PolyphonicKeyPressure(uint8_t channel, MidiNote note, uint8_t velocity) : MidiChannelNoteMessage(MidiMessageStatus::PolyphonicKeyPressure, channel, note, velocity) {}
      PolyphonicKeyPressure(const PolyphonicKeyPressure &that) : MidiChannelNoteMessage(that) {}
    };
    class ControlChange : public MidiChannelMessage
    {
    public:
      ControlChange(uint8_t channel, uint8_t controlNumber, uint8_t value) : MidiChannelMessage(MidiMessageStatus::ControlChange, channel, controlNumber, value) {}
      ControlChange(const ControlChange &that) : MidiChannelMessage(that) {}
      inline uint8_t control() { return (uint8_t)(_arg1); }
      inline uint8_t velocity() { return _arg2; }
    };

    class ChannelPressure : public MidiChannelMessage
    {
    public:
      ChannelPressure(uint8_t channel, uint8_t pressure) : MidiChannelMessage(MidiMessageStatus::ChannelPressure, channel, pressure) {}
      ChannelPressure(const ChannelPressure &that) : MidiChannelMessage(that) {}
      inline uint8_t pressure() { return _arg2; }
    };

    class ActiveSensing : public MidiMessage
    {
    public:
      ActiveSensing() : MidiMessage(0, MidiMessageStatus::ActiveSensing, 0x00, 0x00) {}
      ActiveSensing(const ActiveSensing &that) : MidiMessage(that) {}
    };


    class Continue : public MidiMessage
    {
    public:
      Continue() : MidiMessage(0, MidiMessageStatus::Continue, 0x00, 0x00) {}
      Continue(const Continue &that) : MidiMessage(that) {}
    };


    class EndofExclusive : public MidiMessage
    {
    public:
      EndofExclusive() : MidiMessage(0, MidiMessageStatus::EndofExclusive, 0x00, 0x00) {}
      EndofExclusive(const EndofExclusive &that) : MidiMessage(that) {}
    };

    class PitchBendChange : public MidiChannelMessage
    {
    public:
      PitchBendChange(uint8_t channel, int16_t value)
          : MidiChannelMessage(
                MidiMessageStatus::PitchBendChange,
                channel,
                ((value + 0x2000)),       // low 7 bits
                (((value + 0x2000) >> 7)) // high 7 bits
            )
      {
      }
      PitchBendChange(const PitchBendChange &that) : MidiChannelMessage(that) {}

      inline int16_t value() { return (int16_t)_arg1 + (((int16_t)_arg1) << 7) - 0x2000; }
    };

    class ProgramChange : public MidiChannelMessage
    {
    public:
      ProgramChange(uint8_t channel, uint8_t programNumber) : MidiChannelMessage(MidiMessageStatus::ProgramChange, channel, programNumber) {}
      ProgramChange(const ProgramChange &that) : MidiChannelMessage(that) {}
      inline uint8_t programNumber() { return _arg2; }
    };
    class Reset : public MidiMessage
    {
    public:
      Reset() : MidiMessage(0, MidiMessageStatus::Reset, 0x00, 0x00) {}
      Reset(const Reset &that) : MidiMessage(that) {}
    };
    class SongPositionPointer : public MidiMessage
    {
    public:
      SongPositionPointer(uint16_t beats) : MidiMessage(2, MidiMessageStatus::SongPositionPointer, beats & 0x7F, (beats >> 7) & 0x7F) {}
      SongPositionPointer(const SongPositionPointer &that) : MidiMessage(that) {}
      inline uint16_t beats() { return ((uint16_t)((_arg2 & 0x7F) << 7)) + ((uint16_t)(_arg1 & 0x7F)); }
    };

    class SongSelect : public MidiMessage
    {
    public:
      SongSelect(uint8_t songNumber) : MidiMessage(1, MidiMessageStatus::SongSelect, songNumber, 0x00) {}
      SongSelect(const SongSelect &that) : MidiMessage(that) {}
      inline uint8_t songNumber() { return _arg1; }
    };
    class Start : public MidiMessage
    {
    public:
      Start() : MidiMessage(0, MidiMessageStatus::Start, 0x00, 0x00) {}
      Start(const Start &that) : MidiMessage(that) {}
    };
    class Stop : public MidiMessage
    {
    public:
      Stop() : MidiMessage(0, MidiMessageStatus::Stop, 0x00, 0x00) {}
      Stop(const Stop &that) : MidiMessage(that) {}
    };
    class TimeCodeQuarterFrame : public MidiMessage
    {
    public:
      TimeCodeQuarterFrame(uint8_t messageType, uint8_t values) : MidiMessage(1, MidiMessageStatus::TimeCodeQuarterFrame, ((messageType << 4) & 0x70) | (values & 0x0F), 0x00) {}
      TimeCodeQuarterFrame(const TimeCodeQuarterFrame &that) : MidiMessage(that) {}

      inline uint8_t messageType() { return _arg1 & 0x70 >> 4; }
      inline uint8_t values() { return _arg1 & 0x0F; }
    };
    class TimingClock : public MidiMessage
    {
    public:
      TimingClock() : MidiMessage(0, MidiMessageStatus::TimingClock, 0x00, 0x00) {}
      TimingClock(const TimingClock &that) : MidiMessage(that) {}
    };

    class TuneRequest : public MidiMessage
    {
    public:
      TuneRequest() : MidiMessage(0, MidiMessageStatus::TuneRequest, 0x00, 0x00) {}
      TuneRequest(const TuneRequest &that) : MidiMessage(that) {}
    };

  }
}