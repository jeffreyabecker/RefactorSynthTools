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
    class Tremolo: public SignalTransformation
    {
    private:
        float _sample_rate;
        float _speed;
        float _phase_shift;
        float _value;
        float _depth;
        float _depthInv;

    public:
        Tremolo(float sample_rate, float phaseShift)
            : _sample_rate(sample_rate),
              _value(0),
              _depth(0),
              _depthInv(1),
              _phase_shift(phaseShift)
        {

            setSpeed(6.5f);
            setPhaseShift(0.5f);
        }

        void process(float *left, float *right, int32_t len)
        {
            for (int n = 0; n < len; n++)
            {

                right[n] *= _depthInv + _depth * sinf(_phase_shift + _value);
                _value += _speed;

                /* avoid the runaway */
                if (_value >= 2 * M_PI)
                {
                    _value -= 2 * M_PI;
                }
            }
        }

        void setSpeed(float speed)
        {
            this->_speed = speed * 2 / _sample_rate * M_PI;
        }

        void setPhaseShift(float shift)
        {
            _phase_shift = shift * 2 * M_PI;
        }

        void setDepth(float new_depth)
        {
            this->_depth = new_depth;
            this->_depthInv = new_depth == 0 ? 1 : (1 / new_depth);
        }
    };

}