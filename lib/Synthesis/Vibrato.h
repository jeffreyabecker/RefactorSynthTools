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
 * @file Vibrato.h
 * @author Marcel Licence
 * @date 01.09.2023
 *
 * @brief   Vibrato declarations
 *
 * @see little demo: https://youtu.be/hqK_U22Jha8
 */

#pragma once

#include <stdint.h>
#include <math.h>

#include <stdint.h>
#include "SampleBuffer.h"
#include "SignalTransformation.h"

namespace Synthesis
{
    template <size_t BufferLength>
    class Vibrato : public SignalTransformation
    {
    private:
        static FixedValueSampleBuffer<BufferLength> _emptyModBuffer(0.0f);
        StaticSampleBuffer<BufferLength> _buffer;
        SampleBuffer &_modBuffer;

        float _sample_rate;
        float _depth;
        float _depthInv;

        float _mod_multiplier;
        float _mod_multiplier_curr;
        int32_t _inCnt;

    public:
        Vibrato(float sample_rate, SampleBuffer &modBuffer) : _sample_rate(sample_rate),
                                                              _modBuffer(modBuffer)
        {
            this->reset();
        }
        Vibrato(float sample_rate) : : _sample_rate(sample_rate),
                                       _modBuffer(_emptyModBuffer)
        {
            this->reset();
        }
        virtual void reset() override
        {
            _inCnt = 0;
            _depth = 1.0f;
            _depthInv = 0.0f;
            _mod_multiplier = 0.5f * (_buffer.length() - 2);
            _mod_multiplier_curr = _mod_multiplier;
            _buffer.clear();
        }

        virtual void process(SampleBuffer &inputSignal, SampleBuffer &outputSignal)
        {

            if (_mod_multiplier_curr > _mod_multiplier)
            {
                _mod_multiplier_curr--;
            }
            if (_mod_multiplier_curr < _mod_multiplier)
            {
                _mod_multiplier_curr++;
            }
            auto bufferSize = _buffer.length();
            for (uint32_t n = 0; n < bufferSize; n++)
            {
                float mod = (1.0f + _modBuffer[n]) * _mod_multiplier_curr;
                int outCnt = _inCnt - mod;
                if (outCnt < 0)
                {
                    outCnt += bufferSize;
                }

                _buffer[_inCnt] = inputSignal[n];
                outputSignal[n] = _depth * _buffer[outCnt] + _depthInv * inputSignal[n];

                _inCnt++;
                if (_inCnt >= bufferSize)
                {
                    _inCnt -= bufferSize;
                }
            }
        }

        void setDepth(float depth)
        {
            this->_depth = depth;
            this->_depthInv = 1.0f - depth;
        }

        void setIntensity(float intensity)
        {
            _mod_multiplier = 0.5f * (_buffer.length() - 2) * intensity;
        }
    };

}
