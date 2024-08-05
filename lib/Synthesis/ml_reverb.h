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

#ifndef SRC_ML_REVERB_H_
#define SRC_ML_REVERB_H_

#include <stdint.h>

#ifdef REVERB_DIV
#define REV_MUL(a) (a / REVERB_DIV)
#else
#define REV_MUL(a) (a)
#endif

#define l_CB0 REV_MUL(3460)
#define l_CB1 REV_MUL(2988)
#define l_CB2 REV_MUL(3882)
#define l_CB3 REV_MUL(4312)
#define l_AP0 REV_MUL(480)
#define l_AP1 REV_MUL(161)
#define l_AP2 REV_MUL(46)

#define REV_BUFF_SIZE (l_CB0 + l_CB1 + l_CB2 + l_CB3 + l_AP0 + l_AP1 + l_AP2)

#endif /* SRC_ML_REVERB_H_ */

#pragma once
#include "SignalTransformation.h"
#include "AllPass.h"
#include "Comb.h"
#define REVERB_BUFFER_MAX_INTERNAL (a, b)((a) > (b) ? (a) : (b))
#define REVERB_BUFF_MAX(COMBBUFFERLENGTH_0, COMBBUFFERLENGTH_1, COMBBUFFERLENGTH_2, COMBBUFFERLENGTH_3, ALLPASSBUFFERLENGTH_0, ALLPASSBUFFERLENGTH_1, ALLPASSBUFFERLENGTH_2) REVERB_BUFFER_MAX_INTERNAL(REVERB_BUFFER_MAX_INTERNAL(REVERB_BUFFER_MAX_INTERNAL(REVERB_BUFFER_MAX_INTERNAL(REVERB_BUFFER_MAX_INTERNAL(REVERB_BUFFER_MAX_INTERNAL(COMBBUFFERLENGTH_0, COMBBUFFERLENGTH_1), COMBBUFFERLENGTH_2), COMBBUFFERLENGTH_3), ALLPASSBUFFERLENGTH_0), ALLPASSBUFFERLENGTH_1), ALLPASSBUFFERLENGTH_2)
namespace Synthesis
{

    template <size_t BufferLength>
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
        Comb<CombBufferLength_0 * CombBufferLength_0> _comb0();
        Comb<CombBufferLength_1 * CombBufferLength_1> _comb1();
        Comb<CombBufferLength_2 * CombBufferLength_2> _comb2();
        Comb<CombBufferLength_3 * CombBufferLength_3> _comb3();
        ReverbAllPass<AllPassBufferLength_0 * AllPassBufferLength_0> _allPass0();
        ReverbAllPass<AllPassBufferLength_1 * AllPassBufferLength_1> _allPass1();
        ReverbAllPass<AllPassBufferLength_2 * AllPassBufferLength_2> _allPass2();

    public:
        virtual void process(SampleBuffer &inputSample, SampleBuffer &outputSample) override
        {
            static float inSample[96];
            for (int n = 0; n < buffLen; n++)
            {
                /* create mono sample */
                inSample[n] = signal_l[n]; /* it may cause unwanted audible effects */
            }
            StaticSampleBuffer<SampleBufferLength> newsample();
            newsample.clear();
            _comb0.process(inputSample, newsample);
            _comb1.process(inputSample, newsample);
            _comb2.process(inputSample, newsample);
            _comb3.process(inputSample, newsample);

            for (int n = 0; n < SampleBufferLength; n++)
            {
                newsample[n] *= 0.25f;
            }
            _allPass0.processInplace(newsample);
            _allPass1.processInplace(newsample);
            _allPass2.processInplace(newsample);


            /* apply reverb level */
            const float level = rev_level;
            for (int n = 0; n < SampleBufferLength; n++)
            {
                newsample[n] *= level;
                out[n] = signal_l[n] + newsample[n];
            }
        }
    };
}