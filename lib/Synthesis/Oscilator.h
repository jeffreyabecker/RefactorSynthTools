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

    class OscilatorConfig
    {
    protected:
        float _pitch;
        uint8_t _pitchOctave; /* multiplier to go to higher octaves */
        float _pitchMultiplier;
        float _volume;
        float _morph;
        WaveForms::WaveForm &_morphWaveForm;
        WaveForms::WaveForm &_oscilatorWaveForm;

    public:
        OscilatorConfig() : _pitch(1.0f),
                            _pitchOctave(1),
                            _pitchMultiplier(1.0f),
                            _volume(0.0),
                            _morph(0),
                            _morphWaveForm(WaveForms::All::sine()),
                            _oscilatorWaveForm(WaveForms::All::sawTooth()),
        {
        }
        float calculateSamplePitch()
        {
            return ((_pitchMultiplier)*_pitchOctave * _pitch);
        }
        inline void setPitchOctave(uint8_t value) { _pitchOctave = value; }
        inline uint8_t getPitchOctave() { return _pitchOctave; }
        inline void setPitchMultiplier(float value) { _pitchMultiplier = value; }
        inline float getPitchMultiplier() { return _pitchMultiplier; }
        inline void setVolume(float value) { _volume = value; }
        inline float getVolume() { return _volume; }
        inline void setPitch(float value) { _pitch = value; }
        inline float getPitch() { return _pitch; }

        inline void setMorph(float value) { _morph = value; }
        inline float getMorph() { return _morph; }
        inline void setMorphWaveForm(WaveForms::WaveForm &value) {_morphWaveForm = value); }
        inline WaveForms::WaveForm &getMorphWaveForm() { return _morphWaveForm; }
        inline void setOscilatorWaveForm(WaveForms::WaveForm &value) {_oscilatorWaveForm = value); }
        inline WaveForms::WaveForm &getOscilatorWaveForm() { return _oscilatorWaveForm; }

        inline float morphWaveFormAt(size_t offset)
        {
            return _morphWaveForm->at(offset);
        }
        inline float waveFormAt(size_t offset)
        {
            return _oscilatorWaveForm->at(offset);
        }
    };

    template <size_t BufferLength, uint8_t Voices>
    class Oscilator : SignalTransformation
    {
    protected:
        uint32_t _samplePos;
        uint32_t _addVal;
        float _pan;
        bool _panEnabled;
        float _pitchMod;
        OscilatorConfig _config;

    public:
        Oscilator() : _samplePos(0),
                      _pan(0),
                      _panEnabled(false),
                      _config(),
                      _pitchMod(1)

        {
        }

        virtual void process(const SampleBuffer &inputSignal, SampleBuffer &outputSignal) override
        {
            for (int i = 0; i < Voices; i++)
            {
                for (size_t j = 0U; j < BufferLength; j++)
                {
                    _samplePos += (uint32_t)(_config.calculateSamplePitch() * (float)_addVal * _pitchMod);

                    float morphMod = _config.morphWaveFormAt((uint32_t)_samplePos);
                    morphMod *= ((float)89478480);
                    morphMod *= (_config.getMorph()) * 64;
                    _samplePos += morphMod;

                    float sig = _config.waveFormAt(_samplePos);

                    sig *= _config.getVolume();
                    outputSignal[j] += sig * (_panEnabled ? _pan : 1.0f);
                }
            }
        }
        virtual void reset() override {}

        inline void setSamplePos(uint32_t value) { _samplePos = value; }
        inline uint32_t getSamplePos() { return _samplePos; }
        inline void setAddVal(uint32_t value) { _addVal = value; }
        inline uint32_t getAddVal() { return _addVal; }
        inline void setPan(float value) { _pan = value; }
        inline float getPan() { return _pan; }
        inline void setPanEnabled(bool value) { _panEnabled = value; }
        inline bool getPanEnabled() { return _panEnabled; }
        inline OscilatorConfig &getConfig() { return _config; }
    };

}