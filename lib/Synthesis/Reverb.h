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
 * @file ml_reverb.h
 * @author Marcel Licence
 * @date 04.01.2022
 *
 * @brief This file contains an implementation of a simple reverb effect
 */

#pragma once

#include <stdint.h>

#include "SignalTransformation.h"
#include "AllPass.h"
#include "Comb.h"

namespace Synthesis
{

    template <size_t BufferLength = 48>
    class ReverbAllPass : public AllPass<BufferLength>
    {
    protected:
        virtual void updateWorkingCopy(size_t index, uint32_t &p, float &g, uint32_t &lim) override
        {
            if (p >= lim)
            {
                p = 0;
            }
        }
    };
    template <size_t SampleBufferLength = 96,
              size_t CombBufferLength_0 = 3460,
              size_t CombBufferLength_1 = 2988,
              size_t CombBufferLength_2 = 3882,
              size_t CombBufferLength_3 = 4312,
              size_t AllPassBufferLength_0 = 480,
              size_t AllPassBufferLength_1 = 161,
              size_t AllPassBufferLength_2 = 46>
    class Reverb : public SignalTransformation
    {
    private:
        float _rev_level;
        Comb<CombBufferLength_0 * CombBufferLength_0> _comb0;
        Comb<CombBufferLength_1 * CombBufferLength_1> _comb1;
        Comb<CombBufferLength_2 * CombBufferLength_2> _comb2;
        Comb<CombBufferLength_3 * CombBufferLength_3> _comb3;
        ReverbAllPass<AllPassBufferLength_0 * AllPassBufferLength_0> _allPass0;
        ReverbAllPass<AllPassBufferLength_1 * AllPassBufferLength_1> _allPass1;
        ReverbAllPass<AllPassBufferLength_2 * AllPassBufferLength_2> _allPass2;

    public:
        Reverb() : _comb1(0, 0.827f, (int)(1.0f * CombBufferLength_1 * CombBufferLength_1)),
                   _comb2(0, 0.783f, (int)(1.0f * CombBufferLength_2 * CombBufferLength_2)),
                   _comb3(0, 0.764f, (int)(1.0f * CombBufferLength_3 * CombBufferLength_3)),
                   _allPass0(0, 0.7f, (int)(1.0f * AllPassBufferLength_0 * AllPassBufferLength_0)),
                   _allPass1(0, 0.7f, (int)(1.0f * AllPassBufferLength_1 * AllPassBufferLength_1)),
                   _allPass2(0, 0.7f, (int)(1.0f * AllPassBufferLength_2 * AllPassBufferLength_2)),
                   _rev_time(1.0f),
                   _rev_level(0)
        {
        }

        Reverb(float rev_time, float rev_level) : _comb0(0, 0.805f, (int)(rev_time * CombBufferLength_0 * CombBufferLength_0)),
                                                  _comb1(0, 0.827f, (int)(rev_time * CombBufferLength_1 * CombBufferLength_1)),
                                                  _comb2(0, 0.783f, (int)(rev_time * CombBufferLength_2 * CombBufferLength_2)),
                                                  _comb3(0, 0.764f, (int)(rev_time * CombBufferLength_3 * CombBufferLength_3)),
                                                  _allPass0(0, 0.7f, (int)(rev_time * AllPassBufferLength_0 * AllPassBufferLength_0)),
                                                  _allPass1(0, 0.7f, (int)(rev_time * AllPassBufferLength_1 * AllPassBufferLength_1)),
                                                  _allPass2(0, 0.7f, (int)(rev_time * AllPassBufferLength_2 * AllPassBufferLength_2)),
                                                  _rev_level(rev_level)
        {
        }
        virtual void process(const SampleBuffer &inputSample, SampleBuffer &outputSample) override
        {

            StaticSampleBuffer<SampleBufferLength> newsample();

            newsample.clear();
            _comb0.process(inputCopy, newsample);
            _comb1.process(inputCopy, newsample);
            _comb2.process(inputCopy, newsample);
            _comb3.process(inputCopy, newsample);

            for (int n = 0; n < SampleBufferLength; n++)
            {
                newsample[n] *= 0.25f;
            }
            _allPass0.processInplace(newsample);
            _allPass1.processInplace(newsample);
            _allPass2.processInplace(newsample);

            /* apply reverb level */

            for (int n = 0; n < SampleBufferLength; n++)
            {
                newsample[n] *= _rev_level;
                out[n] = signal_l[n] + newsample[n];
            }
        }
        void setLevel(float level)
        {
            _rev_level = leve;
        }
    };
}