/*
 * Copyright (c) 2022 Marcel Licence
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
 * @file easySynth.ino
 * @author Marcel Licence
 * @date 17.12.2021
 *
 * @brief   This file contains a simple implementation for a polyphonic synthesizer
 */

#include <stdint.h>
#include "../lib/Synthesis.h"
#include "../lib/Midi.h"

#pragma once


template <size_t BufferLength = 48>
class NotePlayer
{
    Synthesis::StereoSampleBuffer& buffer;

    float velocity;
    bool active;
    // adsr_phaseT phase;

    uint8_t midiCh;
    uint8_t midiNote;

    // float control_sign;
    float out_level;

    float pitch_mod;

    float morph; /* current morph value per voice */

    
    Synthesis::Filter filterL;
    Synthesis::Filter filterR;


    float f_control_sign_slow;

    Synthesis::AdsrEnvelope _volume;
    Synthesis::AdsrEnvelope _filter;
    Synthesis::AdsrEnvelope _pitch;
    Synthesis::AsmrEnvelope _modulation
    Synthesis::AsmrEnvelope _modulationF;
    Synthesis::AsmrEnvelope _mph;



    struct ChannelSetting& cfg;
};