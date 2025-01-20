#ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER_H
#define MIDILAR_SYSTEM_CALLBACK_HANDLER_H

#include <MIDILAR_BuildSettings.h>

#ifdef MIDILAR_EXCEPTIONS
    #include <exception>
#endif

namespace MIDILAR::SystemFoundation {

    template <typename R, typename P>
    class CallbackHandler {
    public:
        using ReturnType = R;
        using ParameterType = P;
        using CallbackType = R (*)(P);

        CallbackHandler() : _callback(nullptr) {}

        void bind(CallbackType Callback) { _callback = Callback; }
        void unbind() { _callback = nullptr; }
        bool status() const { return _callback != nullptr; }

        R invoke(P parameter) const {
            if (_callback) {
                return _callback(parameter);
            }
            #ifdef MIDILAR_EXCEPTIONS
                throw std::runtime_error("Callback is not bound");
            #endif
            return R(); // Default return value in case exceptions are disabled
        }

    private:
        CallbackType _callback;
    };

    // Specialization for void return type (no return value)
    template <typename P>
    class CallbackHandler<void, P> {
    public:
        using ReturnType = void;
        using ParameterType = P;
        using CallbackType = void (*)(P);

        CallbackHandler() : _callback(nullptr) {}

        void bind(CallbackType Callback) { _callback = Callback; }
        void unbind() { _callback = nullptr; }
        bool status() const { return _callback != nullptr; }

        void invoke(P parameter) const {
            if (_callback) {
                _callback(parameter);
            } else {
                #ifdef MIDILAR_EXCEPTIONS
                    throw std::runtime_error("Callback is not bound");
                #endif
            }
        }

    private:
        CallbackType _callback;
    };

    // Specialization for void return type and void parameter (callback with no parameters)
    template <>
    class CallbackHandler<void, void> {
    public:
        using ReturnType = void;
        using CallbackType = void (*)();

        CallbackHandler() : _callback(nullptr) {}

        void bind(CallbackType Callback) { _callback = Callback; }
        void unbind() { _callback = nullptr; }
        bool status() const { return _callback != nullptr; }

        void invoke() const {
            if (_callback) {
                _callback();
            } else {
                #ifdef MIDILAR_EXCEPTIONS
                    throw std::runtime_error("Callback is not bound");
                #endif
            }
        }

    private:
        CallbackType _callback;
    };

    // Specialization for non-void return type and void parameter (callback that returns a value but takes no parameters)
    template <typename R>
    class CallbackHandler<R, void> {
    public:
        using ReturnType = R;
        using CallbackType = R (*)();

        CallbackHandler() : _callback(nullptr) {}

        void bind(CallbackType Callback) { _callback = Callback; }
        void unbind() { _callback = nullptr; }
        bool status() const { return _callback != nullptr; }

        R invoke() const {
            if (_callback) {
                return _callback();
            } else {
                #ifdef MIDILAR_EXCEPTIONS
                    throw std::runtime_error("Callback is not bound");
                #endif
                return R(); // Default return value in case exceptions are disabled
            }
        }

    private:
        CallbackType _callback;
    };

} // namespace MIDILAR::SystemFoundation

#endif // MIDILAR_SYSTEM_CALLBACK_HANDLER_H
