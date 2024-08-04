/*
 * Copyright (c) 2024 Marcel Licence
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
 * @file ml_delay.h
 * @author Marcel Licence
 * @date 06.01.2022
 *
 * @brief This file contains the declarations of functions of a simple delay effect
 */

#pragma once
#include <cstddef>
#include <stdint.h>
#include "SampleBuffer.h"
#include "ml_waveform.h"
#include <math.h>
#include "SignalTransformation.h"

namespace Synthesis
{
    class Delay : public SignalTransformation
    {
    protected:
        SampleBuffer &_buffer;
        float _outputLevel = 0;
        float _inputLevel = 1.0f;
        float _delayFeedback = 0;
        float _shift = 2.0f / 3.0f;
        uint32_t _delayLength = 11098;
        uint32_t _delayIn = 0;
        uint32_t _delayOut = 0;
        uint32_t _delayOut2 = 0;
        uint32_t _delayOut3 = 0;

    public:
        const uint32_t DefaultDelayLength = 11098;
        const float DefaultInputLevel = 1.0f;
        const float DefaultOutputLevel = 0.0f;
        const float DefaultFeedback = 0.0f;
        const float DefaultShift = 2.0f / 3.0f;

        Delay(SampleBuffer &buffer)
            : Delay(buffer, DefaultDelayLength, DefaultInputLevel, DefaultOutputLevel, DefaultFeedback, DefaultShift)
        {
        }
        Delay(SampleBuffer &buffer, uint32_t delayLength, float inputLevel, float outputLevel, float feedback, float shift) : _buffer(buffer),
                                                                                                                              _delayLength(delayLength),
                                                                                                                              _inputLevel(inputLevel),
                                                                                                                              _outputLevel(outputLevel),
                                                                                                                              _delayFeedback(feedback),
                                                                                                                              _shift(shift)
        {
            auto bufferLength = buffer.length();

            if (_delayLength > bufferLength)
            {
                _delayLength = bufferLength;
            }
            this->reset();
        }
        virtual void reset() override
        {
            auto bufferLength = _buffer.length();
            for (size_t i = 0; i < bufferLength; i++)
            {
                _buffer[i] = 0;
            }
        }

        virtual void process(SampleBuffer &inputSignal, SampleBuffer &outputSignal) override
        {
            auto signalLength = inputSignal.length();
            auto bufferLength = _buffer.length();
            for (int n = 0; n < signalLength; n++)
            {
                _buffer[_delayIn] = (((float)0x4000) * inputSignal[n] * _inputLevel);

                _delayOut = _delayIn + (1 + bufferLength - _delayLength);

                if (_delayOut >= bufferLength)
                {
                    _delayOut -= bufferLength;
                }

                outputSignal[n] += ((float)_buffer[_delayOut]) * _outputLevel / ((float)0x4000);

                _buffer[_delayIn] += (((float)_buffer[_delayOut]) * _delayFeedback);

                _delayIn++;

                if (_delayIn >= bufferLength)
                {
                    _delayIn = 0;
                }
            }
        }

        float getOutputLevel() { return _outputLevel; }
        float setOutputLevel(float newValue)
        {
            float oldValue = _outputLevel;
            _outputLevel = newValue;
            return oldValue;
        }

        float getInputLevel() { return _inputLevel; }
        float setInputLevel(float newValue)
        {
            float oldValue = _inputLevel;
            _inputLevel = newValue;
            return oldValue;
        }

        float getDelayFeedback() { return _delayFeedback; }
        float setDelayFeedback(float newValue)
        {
            float oldValue = _delayFeedback;
            _delayFeedback = newValue;
            return oldValue;
        }

        float getShift() { return _shift; }
        float setShift(float newValue)
        {
            float oldValue = _shift;
            _shift = newValue;
            return oldValue;
        }

        uint32_t getDelayLength() { return _delayLength; }
        uint32_t setDelayLength(uint32_t newValue)
        {
            uint32_t oldValue = _delayLength;
            _delayLength = newValue;
            return oldValue;
        }
    };
}