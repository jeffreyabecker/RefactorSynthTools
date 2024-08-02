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
    void getData(uint8_t *buffer, size_t offset, size_t length)
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

  class ActiveSensingMessage : public MidiMessage
  {
  public:
    ActiveSensingMessage() : MidiMessage(0, MidiMessageStatus::ActiveSensing, 0x00, 0x00) {}
    ActiveSensingMessage(const ActiveSensingMessage &that) : MidiMessage(that) {}
  };

  class ChannelPressureMessage : public MidiChannelMessage
  {
  public:
    ChannelPressureMessage(uint8_t channel, uint8_t pressure) : MidiChannelMessage(MidiMessageStatus::ChannelPressure, channel, pressure) {}
    ChannelPressureMessage(const ChannelPressureMessage &that) : MidiChannelMessage(that) {}
    inline uint8_t pressure() { return _arg2; }
  };
  class ContinueMessage : public MidiMessage
  {
  public:
    ContinueMessage() : MidiMessage(0, MidiMessageStatus::Continue, 0x00, 0x00) {}
    ContinueMessage(const ContinueMessage &that) : MidiMessage(that) {}
  };
  class ControlChangeMessage : public MidiChannelMessage
  {
  public:
    ControlChangeMessage(uint8_t channel, uint8_t controlNumber, uint8_t value) : MidiChannelMessage(MidiMessageStatus::ControlChange, channel, controlNumber, value) {}
    ControlChangeMessage(const ControlChangeMessage &that) : MidiChannelMessage(that) {}
    inline uint8_t control() { return (uint8_t)(_arg1); }
    inline uint8_t velocity() { return _arg2; }
  };

  class EndofExclusiveMessage : public MidiMessage
  {
  public:
    EndofExclusiveMessage() : MidiMessage(0, MidiMessageStatus::EndofExclusive, 0x00, 0x00) {}
    EndofExclusiveMessage(const EndofExclusiveMessage &that) : MidiMessage(that) {}
  };
  class NoteOffMessage : public MidiChannelNoteMessage
  {
  public:
    NoteOffMessage(uint8_t channel, MidiNote note, uint8_t velocity) : MidiChannelNoteMessage(MidiMessageStatus::NoteOff, channel, note, velocity) {}
    NoteOffMessage(const NoteOffMessage &that) : MidiChannelNoteMessage(that) {}
  };
  class NoteOnMessage : public MidiChannelNoteMessage
  {
  public:
    NoteOnMessage(uint8_t channel, MidiNote note, uint8_t velocity) : MidiChannelNoteMessage(MidiMessageStatus::NoteOn, channel, note, velocity) {}
    NoteOnMessage(const NoteOnMessage &that) : MidiChannelNoteMessage(that) {}
  };
  class PitchBendChangeMessage : public MidiChannelMessage
  {
  public:
    PitchBendChangeMessage(uint8_t channel, int16_t value)
        : MidiChannelMessage(
              MidiMessageStatus::PitchBendChange,
              channel,
              ((value + 0x2000)),       // low 7 bits
              (((value + 0x2000) >> 7)) // high 7 bits
          )
    {
    }
    PitchBendChangeMessage(const PitchBendChangeMessage &that) : MidiChannelMessage(that) {}

    inline int16_t value() { return (int16_t)_arg1 + (((int16_t)_arg1) << 7) - 0x2000; }
  };
  class PolyphonicKeyPressureMessage : public MidiChannelNoteMessage
  {
  public:
    PolyphonicKeyPressureMessage(uint8_t channel, MidiNote note, uint8_t velocity) : MidiChannelNoteMessage(MidiMessageStatus::PolyphonicKeyPressure, channel, note, velocity) {}
    PolyphonicKeyPressureMessage(const PolyphonicKeyPressureMessage &that) : MidiChannelNoteMessage(that) {}
  };
  class ProgramChangeMessage : public MidiChannelMessage
  {
  public:
    ProgramChangeMessage(uint8_t channel, uint8_t programNumber) : MidiChannelMessage(MidiMessageStatus::ProgramChange, channel, programNumber) {}
    ProgramChangeMessage(const ProgramChangeMessage &that) : MidiChannelMessage(that) {}
    inline uint8_t programNumber() { return _arg2; }
  };
  class ResetMessage : public MidiMessage
  {
  public:
    ResetMessage() : MidiMessage(0, MidiMessageStatus::Reset, 0x00, 0x00) {}
    ResetMessage(const ResetMessage &that) : MidiMessage(that) {}
  };
  class SongPositionPointerMessage : public MidiMessage
  {
  public:
    SongPositionPointerMessage(uint16_t beats) : MidiMessage(2, MidiMessageStatus::SongPositionPointer, beats & 0x7F, (beats >> 7) & 0x7F) {}
    SongPositionPointerMessage(const SongPositionPointerMessage &that) : MidiMessage(that) {}
    inline uint16_t beats() { return ((uint16_t)((_arg2 & 0x7F) << 7)) + ((uint16_t)(_arg1 & 0x7F)); }
  };

  class SongSelectMessage : public MidiMessage
  {
  public:
    SongSelectMessage(uint8_t songNumber) : MidiMessage(1, MidiMessageStatus::SongSelect, songNumber, 0x00) {}
    SongSelectMessage(const SongSelectMessage &that) : MidiMessage(that) {}
    inline uint8_t songNumber() { return _arg1; }
  };
  class StartMessage : public MidiMessage
  {
  public:
    StartMessage() : MidiMessage(0, MidiMessageStatus::Start, 0x00, 0x00) {}
    StartMessage(const StartMessage &that) : MidiMessage(that) {}
  };
  class StopMessage : public MidiMessage
  {
  public:
    StopMessage() : MidiMessage(0, MidiMessageStatus::Stop, 0x00, 0x00) {}
    StopMessage(const StopMessage &that) : MidiMessage(that) {}
  };
  class TimeCodeQuarterFrameMessage : public MidiMessage
  {
  public:
    TimeCodeQuarterFrameMessage(uint8_t messageType, uint8_t values) : MidiMessage(1, MidiMessageStatus::TimeCodeQuarterFrame, ((messageType << 4) & 0x70) | (values & 0x0F), 0x00) {}
    TimeCodeQuarterFrameMessage(const TimeCodeQuarterFrameMessage &that) : MidiMessage(that) {}

    inline uint8_t messageType() { return _arg1 & 0x70 >> 4; }
    inline uint8_t values() { return _arg1 & 0x0F; }
  };
  class TimingClockMessage : public MidiMessage
  {
  public:
    TimingClockMessage() : MidiMessage(0, MidiMessageStatus::TimingClock, 0x00, 0x00) {}
    TimingClockMessage(const TimingClockMessage &that) : MidiMessage(that) {}
  };

  class TuneRequestMessage : public MidiMessage
  {
  public:
    TuneRequestMessage() : MidiMessage(0, MidiMessageStatus::TuneRequest, 0x00, 0x00) {}
    TuneRequestMessage(const TuneRequestMessage &that) : MidiMessage(that) {}
  };

  class MidiMessages
  {
  public:
    static inline NoteOffMessage NoteOff(uint8_t channel, MidiNote note, uint8_t velocity) { return NoteOffMessage(channel, note, velocity); }
    static inline NoteOnMessage NoteOn(uint8_t channel, MidiNote note, uint8_t velocity) { return NoteOnMessage(channel, note, velocity); }
    static inline PolyphonicKeyPressureMessage PolyphonicKeyPressure(uint8_t channel, MidiNote note, uint8_t velocity) { return PolyphonicKeyPressureMessage(channel, note, velocity); }

    static inline ControlChangeMessage ControlChange(uint8_t channel, uint8_t controlNumber, uint8_t value) { return ControlChangeMessage(channel, controlNumber, value); }
    static inline ProgramChangeMessage ProgramChange(uint8_t channel, uint8_t programNumber) { return ProgramChangeMessage(channel, programNumber); }
    static inline ChannelPressureMessage ChannelPressure(uint8_t channel, uint8_t pressure) { return ChannelPressureMessage(channel, pressure); }
    static inline PitchBendChangeMessage PitchBendChange(uint8_t channel, int16_t value) { return PitchBendChangeMessage(channel, value); }
    static inline TimeCodeQuarterFrameMessage TimeCodeQuarterFrame(uint8_t messageType, uint8_t values) { return TimeCodeQuarterFrameMessage(messageType, values); }
    static inline SongPositionPointerMessage SongPositionPointer(uint16_t beats) { return SongPositionPointerMessage(beats); }
    static inline SongSelectMessage SongSelect(uint8_t songNumber) { return SongSelectMessage(songNumber); }

    static const TuneRequestMessage TuneRequest();
    static const EndofExclusiveMessage EndofExclusive();
    static const TimingClockMessage TimingClock();
    static const StartMessage Start();
    static const ContinueMessage ContinueMessage();
    static const StopMessage Stop();
    static const ActiveSensingMessage ActiveSensing();
    static const ResetMessage Reset();

    // SystemExclusive = 0xf0,
  };
}
