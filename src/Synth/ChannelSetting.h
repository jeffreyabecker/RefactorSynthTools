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
#define NOTE_STACK_MAX 8

template <size_t MaxNotes>
class ChannelSetting
{
private:
    float soundFiltReso;
    float soundNoiseLevel;
    float pitchEnv;
    /* modulation */
    float modulationDepth;
    float modulationSpeed;
    float modulationPitch;
    /* pitchbend */
    float pitchBendValue;
    float pitchMultiplier;
    /* morph */
    float morph;
    float morph_lfo;
    /* mono mode variables */
    bool mono;

    float portAdd;
    float port;
    float noteA;
    float noteB;
    float modulation;

    
    Synthesis::AdsrEnvelope adsr_vol;
    Synthesis::AdsrEnvelope adsr_fil;
    Synthesis::AdsrEnvelope adsr_pit;
    Synthesis::AsmrEnvelope adsr_mod;
    Synthesis::AsmrEnvelope adsr_mof;
    Synthesis::AsmrEnvelope adsr_mph;

    uint32_t noteCnt;
    std::array<Midi::MidiNote, MaxNotes> notes;
    
    public:
    ChannelSetting():
        soundFiltReso(0.5f),
        soundNoiseLevel(0.0f),
        pitchEnv(0.5f),
        modulationDepth(0.0f),
        modulationSpeed(5.0f),
        modulationPitch(1.0f),
        pitchBendValue(0.0f),
        pitchMultiplier(1.0f),
        morph(0.0f),
        morph_lfo(0.0f),
        mono(false),
        portAdd(0.01f), /*!< speed of portamento */
        port(1.0f),
        noteA(0),
        noteB(0),
        
        adsr_vol(1.0f, 0.25f, 1.0f, 0.01f),
        adsr_fil(1.0f, 0.25f, 1.0f, 0.01f),
        adsr_pit(1.0f, 0.25f, 1.0f, 0.01f),
        adsr_mod(1.0f, 0.25f, 1.0f, 0.01f),
        adsr_mof(1.0f, 0.25f, 1.0f, 0.01f),
        adsr_mph(1.0f, 0.25f, 1.0f, 0.01f),
        noteCnt(0)
    {}
    /*
     * store configuration for three different oscillators
     */
    // synth_osc_cfg_s oscCfg[3];
};
