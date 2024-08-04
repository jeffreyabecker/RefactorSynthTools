
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
 *
 * @brief   This file contains a simple implementation for a polyphonic synthesizer
 */

#pragma once
#include <cstddef>
#include "SampleBuffer.h"
#include <math.h>
#include <stdlib.h>

namespace Synthesis
{
    namespace WaveForms
    {
        class WaveForm
        {
        public:
            virtual float at(size_t offset) = 0;
        };

    };

    template <size_t BitLength>
    class DynamicWaveForm : public WaveForm, public SampleBuffer
    {
    private:
    protected:
        const size_t Mask = ((1 << BitLength) - 1);
        const size_t BufferLength = (1 << BitLength);
        float _bogusSampleForReturnFromOperator;
        virtual float generateValue(size_t index) = 0;

    public:
        DynamicWaveForm() {}
        virtual float at(size_t offset) override
        {
            return _samples[(((offset) >> (32 - BitLength)) & Mask)];
        }
        virtual size_t length() override { return (1 << BitLength); };
        virtual float &operator[](size_t index) override
        {
            _bogusSampleForReturnFromOperator = generateValue(index);
            return _bogusSampleForReturnFromOperator;
        }
        virtual void clear() override {}
    };

    template <size_t BitLength>
    class FixedWaveForm : public FixedSampleBuffer<(1 << BitLength)>, WaveForm
    {
    protected:
        const size_t Mask = ((1 << BitLength) - 1);
        const size_t BufferLength = (1 << BitLength);
        FixedWaveForm(DynamicWaveForm<BitLength> dynamicForm)
        {
        }

    public:
        virtual float at(size_t offset) override
        {
            return _samples[(((offset) >> (32 - BitLength)) & Mask)];
        }
    };

    template <size_t BitLength>
    class SineDynamicWaveForm : public DynamicWaveForm
    {
    protected:
        virtual float generateValue(size_t index) override { return (float)sin(i * 2.0 * 3.1415926535897932384626433832795 / BufferLength); }
    };

    template <size_t BitLength>
    class SineFixedWaveForm : public FixedWaveForm<BitLength>
    {
    public:
        SineFixedWaveForm() : FixedWaveForm(SineDynamicWaveForm()) {}
    };

    template <size_t BitLength>
    class SawToothDynamicWaveForm : public DynamicWaveForm
    {
    protected:
        virtual float generateValue(size_t index) override { return (2.0f * ((float)index) / ((float)BufferLength)) - 1.0f; }
    };

    template <size_t BitLength>
    class SawToothFixedWaveForm : public FixedWaveForm<BitLength>
    {
    public:
        SawToothFixedWaveForm() : FixedWaveForm(SawToothDynamicWaveForm()) {}
    };

    template <size_t BitLength>
    class SquareDynamicWaveForm : public DynamicWaveForm
    {
    protected:
        virtual float generateValue(size_t index) override { return (index > (BufferLength / 2)) ? 1 : -1; }
    };

    template <size_t BitLength>
    class SquareFixedWaveForm : public FixedWaveForm<BitLength>
    {
    public:
        SquareFixedWaveForm() : FixedWaveForm(SquareDynamicWaveForm){}
    };


    
    template <size_t BitLength>
    class PulseFixedWaveForm : public FixedWaveForm<BitLength>
    {
    public:
        PulseFixedWaveForm()
        {
            for (int i = 0; i < BufferLength; i++)
            {
                _samples[i] = (i > (BufferLength / 4)) ? 1.0f / 4.0f : -3.0f / 4.0f;
            }
        }
    };
    template <size_t BitLength>
    class TriangleFixedWaveForm : public FixedWaveForm<BitLength>
    {
    public:
        TriangleFixedWaveForm()
        {
            for (int i = 0; i < BufferLength; i++)
            {
                _samples[i] = ((i > (BufferLength / 2)) ? (((4.0f * (float)i) / ((float)BufferLength)) - 1.0f) : (3.0f - ((4.0f * (float)i) / ((float)BufferLength)))) - 2.0f;
            }
        }
    };
    template <size_t BitLength>
    class NoiseFixedWaveForm : public FixedWaveForm<BitLength>
    {
    public:
        NoiseFixedWaveForm()
        {
            for (int i = 0; i < BufferLength; i++)
            {
                _samples[i] = ((rand() % (1024)) / 512.0f) - 1.0f;
            }
        }
    };

    template <size_t BitLength>
    class SilenceWaveForm : public DynamicWaveForm<BitLength>
    {
    protected:
        virtual float generateValue(size_t index) override { reutrn 0; }
    };
}
