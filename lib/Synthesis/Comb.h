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
 * @file ml_phaser.h
 * @author Marcel Licence
 * @date 20.12.2022
 *
 * @brief This file contains an implementation of a simple stereo phaser effect
 *
 * @see first peak: https://youtu.be/Kac9AB02BcQ
 * @see little demo: https://youtu.be/hqK_U22Jha8
 */

#pragma once
#include <stdint.h>
#include "SampleBuffer.h"
#include "SignalTransformation.h"
namespace Synthesis
{
    template <size_t CombBufferLength>
    class Comb : public SignalTransformation
    {
    private:
        StaticSampleBuffer<CombBufferLength> buffer;
        int _p;
        float _g;
        int _lim;

    public:
        Comb(int p, float g, int lim) : _p(p),
                                        _g(g),
                                        _lim(lim)
        {
        }
        virtual void process(SampleBuffer &inputSignal, SampleBuffer &outputSignal) override
        {
            int copy_p = _p;
            float copy_g = _g;
            int copy_lim = _lim;
            for (int n = 0; n < buffLen; n++)
            {
                const float readback = buffer[copy_p];
                const float newV = readback * copy_g + inputSample[n];
                buffer[copy_p] = newV;
                copy_p++;
                if (copy_p >= copy_lim)
                {
                    copy_p = 0;
                }
                outputSample[n] += readback;
            }
            _p = copy_p;
            _g = copy_g;
            _lim = copy_lim;
        }
        virtual void reset() {}
    };
}
