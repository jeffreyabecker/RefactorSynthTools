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
 * @file ml_lfo.h
 * @author Marcel Licence
 * @date 01.09.2023
 *
 * @brief   LFO declarations
 *
 * @see little demo: https://youtu.be/hqK_U22Jha8 (more details coming soon)
 */

#pragma once

#include <stdint.h>
#include "SampleBuffer.h"
#include "SignalTransformation.h"
namespace Synthesis
{
    class LowFrequencyOscillator : public SignalTransformation
    {
    private:
        float _sample_rate;
        float _frequency;
        float _phase;
        float *buffer;
        float bufferSize;

    public:
        void Process(const float *frequency_in, float *buffer, uint32_t len);
        void Process(const float *frequency_in, uint32_t len);

        LowFrequencyOscillator(float sample_rate) : _sample_rate(sample_rate), _phase(0.0f), _frequency(1.0f)
        {
        }
        virtual void reset() override
        {
            _phase = 0.0f;
            _frequency = 1.0f;
        }
        virtual void process(SampleBuffer &inputSignal, SampleBuffer &outputSignal) override
        {
            auto bufferLength = inputSignal.length();
            for (size_t n = 0; n < bufferLength; n++)
            {
                const float omega = inputSignal[n] * 2.0f * M_PI / (_sample_rate);

                _phase += omega;
                if (_phase >= 2.0f * M_PI)
                {
                    _phase -= 2.0f * M_PI;
                }
                outputSignal[n] = sinf(_phase);
            }
        }

        void setFrequency(float frequency)
        {
            this->_frequency = frequency;
        }

        void setPhase(float phase)
        {
            this->_phase = phase;
        }
    };
}