#pragma once

#include <stdint.h>
namespace Midi
{

    // from : https://inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies
    class MidiNote
    {
    private:
        uint8_t _value;
        static const uint8_t OctaveMask = 0x7F;
        static const float _frequencies[128];
        static inline uint8_t offset(uint8_t octaveNumber) { return (12 * ((octaveNumber & OctaveMask) - 1)); }

    public:
        MidiNote(uint8_t value) : _value(value & OctaveMask) {}
        MidiNote(const MidiNote &that) : _value(that._value) {}
        // These functions are named like this because one-letter names seem to break Arduino
        static inline MidiNote Note_G(uint8_t octaveNumber) { return MidiNote(31 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_FSharp(uint8_t octaveNumber) { return MidiNote(30 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_Gb(uint8_t octaveNumber) { return MidiNote(30 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_F(uint8_t octaveNumber) { return MidiNote(29 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_E(uint8_t octaveNumber) { return MidiNote(28 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_DSharp(uint8_t octaveNumber) { return MidiNote(27 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_Eb(uint8_t octaveNumber) { return MidiNote(27 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_D(uint8_t octaveNumber) { return MidiNote(26 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_CSharp(uint8_t octaveNumber) { return MidiNote(25 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_Db(uint8_t octaveNumber) { return MidiNote(25 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_C(uint8_t octaveNumber) { return MidiNote(24 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_B(uint8_t octaveNumber) { return MidiNote(35 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_ASharp(uint8_t octaveNumber) { return MidiNote(34 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_Bb(uint8_t octaveNumber) { return MidiNote(34 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_A(uint8_t octaveNumber) { return MidiNote(33 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_GSharp(uint8_t octaveNumber) { return MidiNote(32 + MidiNote::offset(octaveNumber)); }
        static inline MidiNote Note_Ab(uint8_t octaveNumber) { return MidiNote(32 + MidiNote::offset(octaveNumber)); }
        // Note that conversion-type-id "int" is the implied return type.
        // Returns by value so "const" is a better fit in this case.
        operator uint8_t() const
        {
            return _value;
        }
        inline float frequency() { return _frequencies[_value]; }
    };
    const float MidiNote::_frequencies[128] = {
        8.1758 /*Hz*/, 8.662 /*Hz*/, 9.177 /*Hz*/, 9.7227 /*Hz*/, 10.3009 /*Hz*/, 10.9134 /*Hz*/, 11.5623 /*Hz*/, 12.2499 /*Hz*/, 12.9783 /*Hz*/, 13.75 /*Hz*/, 14.5676 /*Hz*/, 15.4339 /*Hz*/,
        16.3516 /*Hz*/, 17.3239 /*Hz*/, 18.354 /*Hz*/, 19.4454 /*Hz*/, 20.6017 /*Hz*/, 21.8268 /*Hz*/, 23.1247 /*Hz*/, 24.4997 /*Hz*/, 25.9565 /*Hz*/, 27.5 /*Hz*/, 29.1352 /*Hz*/, 30.8677 /*Hz*/,
        32.7032 /*Hz*/, 34.6478 /*Hz*/, 36.7081 /*Hz*/, 38.8909 /*Hz*/, 41.2034 /*Hz*/, 43.6535 /*Hz*/, 46.2493 /*Hz*/, 48.9994 /*Hz*/, 51.9131 /*Hz*/, 55 /*Hz*/, 58.2705 /*Hz*/, 61.7354 /*Hz*/,
        65.4064 /*Hz*/, 69.2957 /*Hz*/, 73.4162 /*Hz*/, 77.7817 /*Hz*/, 82.4069 /*Hz*/, 87.3071 /*Hz*/, 92.4986 /*Hz*/, 97.9989 /*Hz*/, 103.8262 /*Hz*/, 110 /*Hz*/, 116.5409 /*Hz*/, 123.4708 /*Hz*/,
        130.8128 /*Hz*/, 138.5913 /*Hz*/, 146.8324 /*Hz*/, 155.5635 /*Hz*/, 164.8138 /*Hz*/, 174.6141 /*Hz*/, 184.9972 /*Hz*/, 195.9977 /*Hz*/, 207.6523 /*Hz*/, 220 /*Hz*/, 233.0819 /*Hz*/, 246.9417 /*Hz*/,
        261.6256 /*Hz*/, 277.1826 /*Hz*/, 293.6648 /*Hz*/, 311.127 /*Hz*/, 329.6276 /*Hz*/, 349.2282 /*Hz*/, 369.9944 /*Hz*/, 391.9954 /*Hz*/, 415.3047 /*Hz*/, 440 /*Hz*/, 466.1638 /*Hz*/, 493.8833 /*Hz*/,
        523.2511 /*Hz*/, 554.3653 /*Hz*/, 587.3295 /*Hz*/, 622.254 /*Hz*/, 659.2551 /*Hz*/, 698.4565 /*Hz*/, 739.9888 /*Hz*/, 783.9909 /*Hz*/, 830.6094 /*Hz*/, 880 /*Hz*/, 932.3275 /*Hz*/, 987.7666 /*Hz*/,
        1046.5023 /*Hz*/, 1108.7305 /*Hz*/, 1174.6591 /*Hz*/, 1244.5079 /*Hz*/, 1318.5102 /*Hz*/, 1396.9129 /*Hz*/, 1479.9777 /*Hz*/, 1567.9817 /*Hz*/, 1661.2188 /*Hz*/, 1760 /*Hz*/, 1864.655 /*Hz*/, 1975.5332 /*Hz*/,
        2093.0045 /*Hz*/, 2217.461 /*Hz*/, 2349.3181 /*Hz*/, 2489.0159 /*Hz*/, 2637.0205 /*Hz*/, 2793.8259 /*Hz*/, 2959.9554 /*Hz*/, 3135.9635 /*Hz*/, 3322.4376 /*Hz*/, 3520 /*Hz*/, 3729.3101 /*Hz*/, 3951.0664 /*Hz*/,
        4186.009 /*Hz*/, 4434.9221 /*Hz*/, 4698.6363 /*Hz*/, 4978.0317 /*Hz*/, 5274.0409 /*Hz*/, 5587.6517 /*Hz*/, 5919.9108 /*Hz*/, 6271.927 /*Hz*/, 6644.8752 /*Hz*/, 7040 /*Hz*/, 7458.6202 /*Hz*/, 7902.1328 /*Hz*/,
        8372.0181 /*Hz*/, 8869.8442 /*Hz*/, 9397.2726 /*Hz*/, 9956.0635 /*Hz*/, 10548.0818 /*Hz*/, 11175.3034 /*Hz*/, 11839.8215 /*Hz*/, 12543.854 /*Hz*/
    };
}