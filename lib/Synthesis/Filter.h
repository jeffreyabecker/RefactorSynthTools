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
 * @brief This file contains the declarations and structures of the filter module
 *
 * @see First time used here: https://youtu.be/WJGOIgaY-1s
 *
 * Look here: https://www.musicdsp.org/en/latest/Filters/229-lpf-24db-oct.html
 */

#pragma once
#include <cstddef>
#include "SampleBuffer.h"
#include "WaveForms.h"
#include <math.h>
#include "SignalTransformation.h"
namespace Synthesis
{
    class FilterCoefficent
    {
    protected:
        float _bNorm[3];
        float _aNorm[2];

    public:
        inline float aNorm(uint8_t idx) { return _aNorm[idx]; }
        inline float bNorm(uint8_t idx) { return _bNorm[idx]; }
    };

    class LowPassFilterCoefficent : public FilterCoefficent
    {
    public:
        LowPassFilterCoefficent(float c, float reso, WaveForms::WaveForm *sine)
        {
            /*
             * calculate coefficients of the 2nd order IIR filter
             */
            float Q = reso;
            float cosOmega, omega, sinOmega, alpha, a[3], b[3];

            /*
             * change curve of cutoff a bit
             * maybe also log or exp function could be used
             */
            c = c * c * c;

            if (c >= 1.0f)
            {
                omega = 1.0f;
            }
            else if (c < 0.0025f)
            {
                omega = 0.0025f;
            }
            else
            {
                omega = c;
            }

            /*
             * use lookup here to get quicker results
             */
            cosOmega = sine->at((size_t)((float)((1ULL << 31) - 1) * omega + (float)((1ULL << 30) - 1)));
            sinOmega = sine->at((size_t)((float)((1ULL << 31) - 1) * omega));

            alpha = sinOmega / (2.0 * Q);
            b[0] = (1 - cosOmega) / 2;
            b[1] = 1 - cosOmega;
            b[2] = b[0];
            a[0] = 1 + alpha;
            a[1] = -2 * cosOmega;
            a[2] = 1 - alpha;

            float factor = 1.0f / a[0];

            _aNorm[0] = a[1] * factor;
            _aNorm[1] = a[2] * factor;

            _bNorm[0] = b[0] * factor;
            _bNorm[1] = b[1] * factor;
            _bNorm[2] = b[2] * factor;
        }
    };

    class NotchFilter : public FilterCoefficent
    {
        /*
         * @see https://www.atlantis-press.com/article/5887.pdf
         */
    public:
        NotchFilter(float c, WaveForms::WaveForm *sine)
        {

            float cosOmega, omega, a[3], b[3];

            /*
             * change curve of cutoff a bit
             * maybe also log or exp function could be used
             */

            if (c >= 1.0f)
            {
                omega = 1.0f;
            }
            else if (c < 0.0025f)
            {
                omega = 0.0025f;
            }
            else
            {
                omega = c;
            }

            float r;

            /*
             * use lookup here to get quicker results
             */
            cosOmega = sine->at((size_t)((float)((1ULL << 31) - 1) * omega + (float)((1ULL << 30) - 1)));

            b[0] = 1;
            b[1] = -2 * cosOmega;
            b[2] = 1;
            r = 0.99f;
            a[0] = 1;
            a[1] = -2 * r * cosOmega;
            a[2] = r * r;

            float factor = 1.0f / a[0];

            _aNorm[0] = a[1] * factor;
            _aNorm[1] = a[2] * factor;

            _bNorm[0] = b[0] * factor;
            _bNorm[1] = b[1] * factor;
            _bNorm[2] = b[2] * factor;
        }
    };

    template <size_t BufferLength = 48>
    class Filter : public SignalTransformation
    {
    protected:
        FilterCoefficent &_coefficent float _w[2];

    public:
        virtual void reset() override
        {
            _w[0] = 0.0;
            _w[1] = 0.0;
        }

        virtual void process(const SampleBuffer &inputSignal, SampleBuffer &outputSignal) override
        {

            for (size_t n = 0; n < BufferLength; n++)
            {
                const float out = _coeffcient.bNorm(0) * inputSignal[n] + _w[0];
                _w[0] = _coeffcient.bNorm(1) * inputSignal[n] - _coeffcient.aNorm(0) * out + _w[1];
                _w[1] = _coeffcient.bNorm(2) * inputSignal[n] - _coeffcient.aNorm(1) * out;
                outputSignal[n] = out;
            }
        }
    };

}