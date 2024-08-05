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
 * @file ml_tremolo.h
 * @author Marcel Licence
 * @date 08.12.2022
 *
 * @brief   Tremolo stereo implementation
 *
 * @see little demo: https://youtu.be/zu2xtRKlNVU
 */

#pragma once

#include <stdint.h>
#include <math.h>

#include <stdint.h>
#include "SignalTransformation.h"
namespace Synthesis
{
    template <size_t BufferLength>
    class Tremolo : public SignalTransformation
    {
    private:
        float _phase_shift;
        float _value;
        float _depth;
        float _depthInv;
        SampleBuffer &_modulationBuffer;

    public:
        Tremolo(SampleBuffer &modulationBuffer, float phaseShift)
            : _modulationBuffer(modulationBuffer),

              _value(0),
              _depth(0),
              _depthInv(1),
              _phase_shift(1)
        {

            setSpeed(6.5f);
        }

        virtual void process(const SampleBuffer &inputSample, SampleBuffer &outputSample) override
        {
            for (size_t n = 0; n < BufferLength; n++)
            {
                outputSample[n] = (inputSample[n] * _depthInv) + (inputSample[n] * (1.0f + (_phase_shift * modulationBuffer[n])) * 0.5f * _depth);
            }
        }

        void setPhaseShift(float shift)
        {
            _phase_shift = shift;
        }

        void setDepth(float new_depth)
        {
            this->_depth = new_depth;
            this->_depthInv = new_depth == 0 ? 1 : (1 / new_depth);
        }
    };

}