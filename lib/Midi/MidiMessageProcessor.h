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
        virtual void HandleNoteOff(MidiMessage msg) {}
        virtual void HandleNoteOn(MidiMessage msg) {}
        virtual void HandlePolyphonicKeyPressure(MidiMessage msg) {}
        virtual void HandleControlChange(MidiMessage msg) {}
        virtual void HandleProgramChange(MidiMessage msg) {}
        virtual void HandleChannelPressure(MidiMessage msg) {}
        virtual void HandlePitchBendChange(MidiMessage msg) {}
        virtual void HandleSystemExclusive(MidiMessage msg) {}
        virtual void HandleTimeCodeQuarterFrame(MidiMessage msg) {}
        virtual void HandleSongPositionPointer(MidiMessage msg) {}
        virtual void HandleSongSelect(MidiMessage msg) {}
        virtual void HandleTuneRequest(MidiMessage msg) {}
        virtual void HandleEndofExclusive(MidiMessage msg) {}
        virtual void HandleTimingClock(MidiMessage msg) {}
        virtual void HandleStart(MidiMessage msg) {}
        virtual void HandleContinue(MidiMessage msg) {}
        virtual void HandleStop(MidiMessage msg) {}
        virtual void HandleActiveSensing(MidiMessage msg) {}
        virtual void HandleReset(MidiMessage msg) {}

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
                    HandleNoteOff(message);
                    break;
                case MidiMessageStatus::NoteOn:
                    HandleNoteOn(message);
                    break;
                case MidiMessageStatus::PolyphonicKeyPressure:
                    HandlePolyphonicKeyPressure(message);
                    break;
                case MidiMessageStatus::ControlChange:
                    HandleControlChange(message);
                    break;
                case MidiMessageStatus::ProgramChange:
                    HandleProgramChange(message);
                    break;
                case MidiMessageStatus::ChannelPressure:
                    HandleChannelPressure(message);
                    break;
                case MidiMessageStatus::PitchBendChange:
                    HandlePitchBendChange(message);
                    break;
                case MidiMessageStatus::SystemExclusive:
                    HandleSystemExclusive(message);
                    break;
                case MidiMessageStatus::TimeCodeQuarterFrame:
                    HandleTimeCodeQuarterFrame(message);
                    break;
                case MidiMessageStatus::SongPositionPointer:
                    HandleSongPositionPointer(message);
                    break;
                case MidiMessageStatus::SongSelect:
                    HandleSongSelect(message);
                    break;
                case MidiMessageStatus::TuneRequest:
                    HandleTuneRequest(message);
                    break;
                case MidiMessageStatus::EndofExclusive:
                    HandleEndofExclusive(message);
                    break;
                case MidiMessageStatus::TimingClock:
                    HandleTimingClock(message);
                    break;
                case MidiMessageStatus::Start:
                    HandleStart(message);
                    break;
                case MidiMessageStatus::Continue:
                    HandleContinue(message);
                    break;
                case MidiMessageStatus::Stop:
                    HandleStop(message);
                    break;
                case MidiMessageStatus::ActiveSensing:
                    HandleActiveSensing(message);
                    break;
                case MidiMessageStatus::Reset:
                    HandleReset(message);
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
