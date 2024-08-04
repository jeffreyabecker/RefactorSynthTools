#pragma once
#include <stdint.h>
#include <cstddef>
#include "MidiMessages.h"

#include <string>
#include "MidiSink.h"
namespace Midi
{

    /// @brief Writes MIDI messages out to a physical transport. Examples include sending midi over BLE, UART, or some custom physical transport layer
    class MidiMessageProcessor : public MidiSink
    {
    protected:
        bool _isConnected = false;
        virtual void HandleNoteOff(Messages::NoteOff &msg) {}
        virtual void HandleNoteOn(Messages::NoteOn &msg) {}
        virtual void HandlePolyphonicKeyPressure(Messages::PolyphonicKeyPressure &msg) {}
        virtual void HandleControlChange(Messages::ControlChange &msg) {}
        virtual void HandleProgramChange(Messages::ProgramChange &msg) {}
        virtual void HandleChannelPressure(Messages::ChannelPressure &msg) {}
        virtual void HandlePitchBendChange(Messages::PitchBendChange &msg) {}

        virtual void HandleTimeCodeQuarterFrame(Messages::TimeCodeQuarterFrame &msg) {}
        virtual void HandleSongPositionPointer(Messages::SongPositionPointer &msg) {}
        virtual void HandleSongSelect(Messages::SongSelect &msg) {}
        virtual void HandleTuneRequest(Messages::TuneRequest &msg) {}
        virtual void HandleEndofExclusive(Messages::EndofExclusive &msg) {}
        virtual void HandleTimingClock(Messages::TimingClock &msg) {}
        virtual void HandleStart(Messages::Start &msg) {}
        virtual void HandleContinue(Messages::Continue &msg) {}
        virtual void HandleStop(Messages::Stop &msg) {}
        virtual void HandleActiveSensing(Messages::ActiveSensing &msg) {}
        virtual void HandleReset(Messages::Reset &msg) {}

    public:
        MidiMessageProcessor() : _isConnected(false) {}
        virtual void send(Midi::MidiMessage message) override
        {
            if (_isConnected)
            {
                auto status = message.status();
                switch (status)
                {
                case MidiMessageStatus::NoteOff:
                    HandleNoteOff(static_cast<Messages::NoteOff &>(message));
                    break;
                case MidiMessageStatus::NoteOn:
                    HandleNoteOn(static_cast<Messages::NoteOn &>(message));
                    break;
                case MidiMessageStatus::PolyphonicKeyPressure:
                    HandlePolyphonicKeyPressure(static_cast<Messages::PolyphonicKeyPressure &>(message));
                    break;
                case MidiMessageStatus::ControlChange:
                    HandleControlChange(static_cast<Messages::ControlChange &>(message));
                    break;
                case MidiMessageStatus::ProgramChange:
                    HandleProgramChange(static_cast<Messages::ProgramChange &>(message));
                    break;
                case MidiMessageStatus::ChannelPressure:
                    HandleChannelPressure(static_cast<Messages::ChannelPressure &>(message));
                    break;
                case MidiMessageStatus::PitchBendChange:
                    HandlePitchBendChange(static_cast<Messages::PitchBendChange &>(message));
                    break;

                case MidiMessageStatus::TimeCodeQuarterFrame:
                    HandleTimeCodeQuarterFrame(static_cast<Messages::TimeCodeQuarterFrame &>(message));
                    break;
                case MidiMessageStatus::SongPositionPointer:
                    HandleSongPositionPointer(static_cast<Messages::SongPositionPointer &>(message));
                    break;
                case MidiMessageStatus::SongSelect:
                    HandleSongSelect(static_cast<Messages::SongSelect &>(message));
                    break;
                case MidiMessageStatus::TuneRequest:
                    HandleTuneRequest(static_cast<Messages::TuneRequest &>(message));
                    break;
                case MidiMessageStatus::EndofExclusive:
                    HandleEndofExclusive(static_cast<Messages::EndofExclusive &>(message));
                    break;
                case MidiMessageStatus::TimingClock:
                    HandleTimingClock(static_cast<Messages::TimingClock &>(message));
                    break;
                case MidiMessageStatus::Start:
                    HandleStart(static_cast<Messages::Start &>(message));
                    break;
                case MidiMessageStatus::Continue:
                    HandleContinue(static_cast<Messages::Continue &>(message));
                    break;
                case MidiMessageStatus::Stop:
                    HandleStop(static_cast<Messages::Stop &>(message));
                    break;
                case MidiMessageStatus::ActiveSensing:
                    HandleActiveSensing(static_cast<Messages::ActiveSensing &>(message));
                    break;
                case MidiMessageStatus::Reset:
                    HandleReset(static_cast<Messages::Reset &>(message));
                    break;
                default:
                    break;
                }
            }
        }
        virtual void start() override { _isConnected = true; }
        virtual void stop() override { _isConnected = false; }
        virtual bool isConnected() override { return _isConnected; }
    };

}
