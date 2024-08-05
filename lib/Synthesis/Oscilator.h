/*
 * Copyright (c) 2023 Marcel Licence
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
 * der GNU General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
 * veröffentlichten Version, weiter verteilen und/oder modifizieren.
 *
 * Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich sein wird, jedoch
 * OHNE JEDE GEWÄHR,; sogar ohne die implizite
 * Gewähr der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU General Public License für weitere Einzelheiten.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.
 */

/**
 * @file ml_env.h
 * @author Marcel Licence
 * @date 16.01.2022
 *
 * @brief This file contains the declarations and structures of the oscillator module
 *
 * @see First time used here: https://youtu.be/WJGOIgaY-1s
 */

#pragma once
#include <cstddef>
#include "SampleBuffer.h"
#include "WaveForms.h"
#include <math.h>
#include "SignalTransformation.h"

namespace Synthesis
{
    template <size_t BufferLength>
    class Oscilator : SignalTransformation
    {
    protected:
        WaveForms::WaveForm &_morphWaveForm;
        WaveForms::WaveForm &_oscilatorWaveForm;
        uint8_t _oscilators;
        uint32_t _samplePos;
        uint32_t _addVal;
        float _pan;
        bool _panEnabled;
        uint8_t _pitchOctave; /* multiplier to go to higher octaves */
        float _pitchMultiplier;
        float _volume;
        float _pitch;
        float _pitchMod;
        float _morph;

        inline void OscProcessSingle(SampleBuffer &inputSignal, SampleBuffer &outputSignal)
        {

            for (size_t n = 0U; n < BufferLength; n++)
            {
                _samplePos += (uint32_t)((_pitchMultiplier) * ((float)_addVal) * _pitchOctave * _pitch * _pitchMod);
                uint32_t samplePos = _samplePos;
                float morphMod = _morphWaveForm.at(_samplePos);
                morphMod *= ((float)89478480);
                morphMod *= (_morph) * 64;
                _samplePos += morphMod;

                float sig = _oscilatorWaveForm.at(_samplePos);

                sig *= _volume;
                outputSignal[n] += sig * (_panEnabled ? _pan : 1.0f);
            }
        }

    public:
        Oscilator(
            WaveForms::WaveForm &morphWaveForm,
            WaveForms::WaveForm &oscilatorWaveForm,
            uint8_t voices) : _morphWaveForm(morphWaveForm),
                              _oscilatorWaveForm(oscilatorWaveForm),
                              _oscilators(voices * 3),
                              _samplePos(0)

        {
        }

        virtual void process(const SampleBuffer &inputSignal, SampleBuffer &outputSignal) override
        {

            for (int i = 0; i < _oscilators; i++)
            {
                OscProcessSingle(inputSignal, outputSignal);
            }
        }
        virtual void reset() override {}

        uint32_t getAddVal() { return _addVal; }
        uint32_t setAddVal(uint32_t newValue)
        {
            uint32_t oldValue = _addVal;
            _addVal = newValue;
            return oldValue;
        }

        float getPan() { return _pan; }
        float setPan(float newValue)
        {
            float oldValue = _pan;
            _pan = newValue;
            return oldValue;
        }

        bool getPanEnabled() { return _panEnabled; }
        bool setPanEnabled(bool newValue)
        {
            bool oldValue = _panEnabled;
            _panEnabled = newValue;
            return oldValue;
        }

        uint8_t getPitchOctave() { return _pitchOctave; }
        uint8_t setPitchOctave(uint8_t newValue)
        {
            uint8_t oldValue = _pitchOctave;
            _pitchOctave = newValue;
            return oldValue;
        }

        float getPitchMultiplier() { return _pitchMultiplier; }
        float setPitchMultiplier(float newValue)
        {
            float oldValue = _pitchMultiplier;
            _pitchMultiplier = newValue;
            return oldValue;
        }

        float getVolume() { return _volume; }
        float setVolume(float newValue)
        {
            float oldValue = _volume;
            _volume = newValue;
            return oldValue;
        }

        float getPitch() { return _pitch; }
        float setPitch(float newValue)
        {
            float oldValue = _pitch;
            _pitch = newValue;
            return oldValue;
        }

        float getPitchMod() { return _pitchMod; }
        float setPitchMod(float newValue)
        {
            float oldValue = _pitchMod;
            _pitchMod = newValue;
            return oldValue;
        }

        float getMorph() { return _morph; }
        float setMorph(float newValue)
        {
            float oldValue = _morph;
            _morph = newValue;
            return oldValue;
        }
    };

}