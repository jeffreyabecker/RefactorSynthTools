
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
#include "ml_waveform.h"
#include <math.h>

namespace Synthesis
{

    class SignalTransformation
    {
    public:
        virtual void processInplace(SampleBuffer &signal)
        {
            process(signal, signal);
        }
        virtual void process(SampleBuffer &inputSignal, SampleBuffer &outputSignal) = 0;
        virtual void reset() = 0;
    };

}