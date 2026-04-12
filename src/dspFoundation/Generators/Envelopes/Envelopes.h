#ifndef MIDILAR_GENERATORS_ENVELOPES_H
#define MIDILAR_GENERATORS_ENVELOPES_H

#include <System/BuildSettings.h>
#include <MIDILAR_SystemFoundation.h>

namespace MIDILAR::dspFoundation::Generators::Envelopes {

    enum ADSRStage {
        IDLE,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

    template<typename T>
    class ADSR {
    private:
        T _attackTime;
        T _decayTime;
        T _sustainLevel;
        T _releaseTime;

        T _currentValue;
        ADSRStage _stage;

        SystemFoundation::Clock* _clock;
        SystemFoundation::Clock::TimePoint _lastUpdate;

    public:
        ADSR(SystemFoundation::Clock* clock)
            : _attackTime(0), _decayTime(0), _sustainLevel(0), _releaseTime(0),
              _currentValue(0), _stage(IDLE), _clock(clock), _lastUpdate(0) {}

        void SetAttack(T time)    { _attackTime = time; }
        void SetDecay(T time)     { _decayTime = time; }
        void SetSustain(T level)  { _sustainLevel = level; }
        void SetRelease(T time)   { _releaseTime = time; }

        void Trigger() {
            _stage = ATTACK;
            _lastUpdate = _clock->refresh();
        }

        void Release() {
            if (_stage != IDLE) {
                _stage = RELEASE;
                _lastUpdate = _clock->refresh();
            }
        }

        T Process() {
            SystemFoundation::Clock::TimePoint now = _clock->refresh();
            T elapsed = static_cast<T>(now - _lastUpdate);
            _lastUpdate = now;

            switch (_stage) {
                case ATTACK:
                    _currentValue += elapsed / _attackTime;
                    if (_currentValue >= 1.0f) {
                        _currentValue = 1.0f;
                        _stage = DECAY;
                    }
                    break;

                case DECAY:
                    _currentValue -= elapsed / _decayTime * (1.0f - _sustainLevel);
                    if (_currentValue <= _sustainLevel) {
                        _currentValue = _sustainLevel;
                        _stage = SUSTAIN;
                    }
                    break;

                case SUSTAIN:
                    _currentValue = _sustainLevel;
                    break;

                case RELEASE:
                    _currentValue -= elapsed / _releaseTime * _currentValue;
                    if (_currentValue <= 0.0f) {
                        _currentValue = 0.0f;
                        _stage = IDLE;
                    }
                    break;

                case IDLE:
                default:
                    _currentValue = 0.0f;
                    break;
            }

            return _currentValue;
        }

        ADSRStage GetStage() const {
            return _stage;
        }

        bool IsActive() const {
            return _stage != IDLE;
        }
    };

} // namespace MIDILAR::Generators::Envelopes

#endif // MIDILAR_GENERATORS_ENVELOPES_H
