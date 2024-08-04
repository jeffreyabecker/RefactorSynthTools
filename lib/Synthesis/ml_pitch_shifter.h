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
 * @file ml_pitch_shifter.h
 * @author Marcel Licence
 * @date 01.09.2023
 *
 * @brief   PitchShifter declarations
 *
 * @see little demo: https://youtu.be/hqK_U22Jha8
 */

#pragma once

#include <stdint.h>
#include <math.h>

#include <stdint.h>
#include "SampleBuffer.h"
#define i32_abs(x) ((x) > 0 ? (x) : -(x))
namespace Synthesis
{

    class PitchShifter
    {
    private:
        float _sample_rate;
        float _depth;
        SampleBuffer &_buffer;
        int32_t inCnt;
        float outCnt;
        float _speed;
        float _dryV;
        float _wetV;
        float _feedback;
        /**
         * Works like a cross fader.
         * Mix = 0.0 : dry = 1.0f, wet = 0.0f
         * Mix = 0.5 : dry = 1.0f, wet = 1.0f
         * Mix = 1.0 : dry = 0.0f, wet = 1.0f
         *
         * @param mix
         * @param dry
         * @param wet
         */

    public:
        PitchShifter(SampleBuffer &buffer, float sample_rate) : _buffer(buffer),
                                                                _sample_rate(sample_rate),
                                                                _depth(1.0f),
                                                                _speed(1),
                                                                _wetV(1.0f),
                                                                _dryV(0.0f),
                                                                _feedback(0.125f)
        {

            inCnt = 0;
            outCnt = 0;
        }

        uint32_t minDistance(int32_t pointerA, int32_t pointerB)
        {
            uint32_t distance1 = i32_abs(pointerA - pointerB);
            uint32_t distance2 = _buffer.length() - distance1;
            return (distance1 < distance2) ? distance1 : distance2;
        }

        void Process(const float *in, float *out, uint32_t count)
        {
            auto bufferLength = _buffer.length();

            for (uint32_t n = 0; n < count; n++)
            {
                float outCnt2 = outCnt + (_buffer.length() / 2);
                if (outCnt2 >= bufferLength)
                {
                    outCnt2 -= bufferLength;
                }

                _buffer[inCnt] = in[n];
                uint32_t outU = floor(outCnt);
                uint32_t outU2 = floor(outCnt2);
                uint32_t diffU = minDistance(inCnt, outU);
                float diff = diffU;
                diff *= 1.0f / (bufferLength / 2);
                float diffI = 1.0f - diff;
                out[n] = (diff * _buffer[outU] + diffI * _buffer[outU2]) * _wetV + in[n] * _dryV;

                _buffer[inCnt] += _feedback * out[n];

                inCnt++;
                if (inCnt >= bufferLength)
                {
                    inCnt -= bufferLength;
                }

                outCnt += _speed;
                outCnt++;
                if (outCnt >= bufferLength)
                {
                    outCnt -= bufferLength;
                }
                if (outCnt < 0)
                {
                    outCnt += bufferLength;
                }
            }
        }

        void setDepth(float depth)
        {
            this->_depth = depth;
        }

        void setSpeed(float speed)
        {
            this->_speed = speed;
        }

        void setFeedback(float feedback)
        {
            this->_feedback = feedback;
        }

        void setMix(float mix)
        {
            _dryV = (mix >= 0.5f) ? ((1.0f - mix) * 2.0f) : 1.0f;
            _wetV = (mix >= 0.5f) ? ((mix) * 2.0f) : 1.0f;
        }
    };

}
