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
#include "SignalTransformation.h"
#include "AllPass.h"
namespace Synthesis
{
    template <size_t BufferLength>
    class Phaser : public SignalTransformation
    {
    private:
        class PhaserAllPass : public AllPass<BufferLength>
        {
        protected:
            SampleBuffer &_lfoBuffer;
            float _phaserMod = 1.0f;
            virtual void updateWorkingCopy(size_t index, uint32_t &p, float &g, uint32_t &lim) override
            {
                uint32_t len = 3 + (1 + _lfoBuffer[index]) * _phaserMod * 48.0f;
                if (p >= len)
                {
                    p -= len;
                }
            }

        public:
            PhaserAllPass(SampleBuffer &lfoBuffer) : _lfoBuffer(lfoBuffer), _phaserMod(1.0) {}

            void reset()
            {
                _phaserMod = 1.0;
            }
        };

        PhaserAllPass _allPass;
        SampleBuffer &_lfoBuffer;
        float _inputLevel = 1.0f;
        float _depth = 1.0f;

    public:
        Phaser(SampleBuffer &phaserBuffer, SampleBuffer &lfoBuffer) : _lfoBuffer(lfoBuffer),
                                                                      _allPass(lfoBuffer)
        {
        }

        virtual void process(const SampleBuffer &inputSignal, SampleBuffer &outputSignal)
        {

            _allPass.reset();
            _allPass.process(inputSignal, outputSignal);
            for (int n = 0; n < BufferLength; n++)
            {
                outputSignal[n] = inputSignal[n] - outputSignal[n] * _depth;
            }
        }
        virtual void reset()
        {
            _allPass.reset();
        };

        void setInputLevel(float value)
        {
            _inputLevel = value;
        }

        void setDepth(float value)
        {
            _depth = value;
        }

        void setG(float value)
        {
            _allPass.setG(value);
        }
    };
}