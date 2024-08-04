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
 * @brief This file contains the declarations and structures of the envelope module
 *
 * @see First time used here: https://youtu.be/WJGOIgaY-1s
 */

#pragma once

namespace Synthesis
{
    enum class EnvelopePhase
    {
        attack,
        decay,
        sustain,
        release
    };

    class AdsrEnvelope
    {
    protected:
        float _cfg_sample_rate;
        float _cfg_attack;
        float _cfg_decay;
        float _cfg_sustain;
        float _cfg_release;
        float _cfg_w;
        float _sample_rate;
        float _ctrl;
        EnvelopePhase _phase;

    public:
        /*
         * very bad and simple implementation of ADSR
         * - but it works for the start
         */
        virtual bool process(/*const struct adsrT *ctrl, struct adsr_cfg_t *adsr*/)
        {
            switch (_phase)
            {
            case EnvelopePhase::attack:
                _ctrl += _cfg_attack;
                if (_ctrl > 1.0f)
                {
                    _ctrl = 1.0f;
                    _phase = EnvelopePhase::decay;
                }
                break;
            case EnvelopePhase::decay:
                _ctrl -= _cfg_decay;
                if (_ctrl < _cfg_sustain)
                {
                    _ctrl = _cfg_sustain;
                    _phase = EnvelopePhase::sustain;
                }
                break;
            case EnvelopePhase::sustain:
                break;
            case EnvelopePhase::release:
                _ctrl -= _cfg_release;
                if (_ctrl < 0.0f)
                {
                    _ctrl = 0.0f;
                    return false;
                }
            }
            return true;
        }

        void start(const struct adsrT *ctrl, struct adsr_cfg_t *adsr)
        {
            _ctrl = _cfg_attack;
            if (_cfg_attack == 1.0f) /* not sure if that check is necessary */
            {
                _phase = EnvelopePhase::decay;
            }
            else
            {
                _phase = EnvelopePhase::attack;
            }
        }
    };

    class AsmrEnvelope : public AdsrEnvelope
    {

    public:
        virtual bool process() override
        {
            switch (_phase)
            {
            case EnvelopePhase::attack:
                _ctrl += _cfg_attack;
                if (_ctrl > 1.0f)
                {
                    _ctrl = 1.0f;
                    _phase = EnvelopePhase::decay;
                }
                break;
            case EnvelopePhase::decay:
                _ctrl -= _cfg_decay;
                if (_ctrl < 0)
                {
                    _ctrl = 0;
                    _phase = EnvelopePhase::sustain;
                }
                break;
            case EnvelopePhase::sustain:
                break;
            case EnvelopePhase::release:
                _ctrl -= _cfg_release;
                if (_ctrl < 0.0f)
                {
                    _ctrl = 0.0f;
                    return false;
                }
            }
            return true;
        }
    };

} // namespace Synthesis
