#ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER_H
#define MIDILAR_SYSTEM_CALLBACK_HANDLER_H

#include <MIDILAR_BuildSettings.h>

#ifdef MIDILAR_EXCEPTIONS
    #include <exception>
#endif

namespace MIDILAR::SystemFoundation {

    //////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief A generic callback handler for function pointers
     * 
     * This template class provides a mechanism to store, bind, and invoke callbacks dynamically.
     * It supports both function pointers and lambda functions.
     * 
     * @tparam R Return type of the callback function.
     * @tparam P Parameter type of the callback function.
     */

        template <typename R, typename P>
        class CallbackHandler {
            public:

                using ReturnType = R;
                using ParameterType = P;
                using CallbackType = R (*)(P);

                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Constructs an empty callback handler.
                 */
                    CallbackHandler() : _callback(nullptr) {}
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Binds a callback function.
                 * @param Callback Function pointer or lambda to be stored.
                 */
                    inline void bind(CallbackType Callback) { _callback = Callback; }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Unbinds the currently stored callback.
                 */
                    inline void unbind() { _callback = nullptr; }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Checks if a callback is currently bound.
                 * @return `true` if a callback is bound, otherwise `false`.
                 */
                    [[nodiscard]] inline bool status() const { return static_cast<bool>(_callback); }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Invokes the bound callback function with a parameter.
                 * @param parameter The parameter to pass to the callback function.
                 * @return The result of the callback function execution.
                 * @throws std::runtime_error if no callback is bound and exceptions are enabled.
                 */
                    R invoke(P parameter) const {
                        if (_callback) {
                            return _callback(parameter);
                        }
                        #ifdef MIDILAR_EXCEPTIONS
                            throw std::runtime_error("Callback is not bound");
                        #endif
                        return R(); // Default return value in case exceptions are disabled
                    }
                //
                //////////////////////////////////////////////////////////////////////////////////////////

            private:
                CallbackType _callback;  ///< Stores the function pointer.
        };
    //
    //////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Specialization for callbacks with a `void` return type.
     * @tparam P Parameter type of the callback function.
     */
        template <typename P>
        class CallbackHandler<void, P> {
            public:
                using ReturnType = void;
                using ParameterType = P;
                using CallbackType = void (*)(P);

                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Constructs an empty callback handler.
                 */
                    CallbackHandler() : _callback(nullptr) {}
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Binds a callback function.
                 * @param Callback Function pointer or lambda to be stored.
                 */
                    inline void bind(CallbackType Callback) { _callback = Callback; }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Unbinds the currently stored callback.
                 */
                    inline void unbind() { _callback = nullptr; }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Checks if a callback is currently bound.
                 * @return `true` if a callback is bound, otherwise `false`.
                 */
                    [[nodiscard]] inline bool status() const { return static_cast<bool>(_callback); }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Invokes the bound callback function with a parameter.
                 * @param parameter The parameter to pass to the callback function.
                 * @throws std::runtime_error if no callback is bound and exceptions are enabled.
                 */
                    void invoke(P parameter) const {
                        if (_callback) {
                            _callback(parameter);
                        } else {
                            #ifdef MIDILAR_EXCEPTIONS
                                throw std::runtime_error("Callback is not bound");
                            #endif
                        }
                    }
                //
                //////////////////////////////////////////////////////////////////////////////////////////

            private:
                CallbackType _callback;
        };
    //
    //////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Specialization for callbacks with `void` return type and no parameters.
     */
        template <>
        class CallbackHandler<void, void> {
            public:
                using ReturnType = void;
                using CallbackType = void (*)();

                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Constructs an empty callback handler.
                 */
                    CallbackHandler() : _callback(nullptr) {}
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Binds a callback function.
                 * @param Callback Function pointer or lambda to be stored.
                 */
                    inline void bind(CallbackType Callback) { _callback = Callback; }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Unbinds the currently stored callback.
                 */
                    inline void unbind() { _callback = nullptr; }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Checks if a callback is currently bound.
                 * @return `true` if a callback is bound, otherwise `false`.
                 */
                    [[nodiscard]] inline bool status() const { return static_cast<bool>(_callback); }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Invokes the bound callback function with a parameter.
                 * @param parameter The parameter to pass to the callback function.
                 * @throws std::runtime_error if no callback is bound and exceptions are enabled.
                 */
                    void invoke() const {
                        if (_callback) {
                            _callback();
                        } else {
                            #ifdef MIDILAR_EXCEPTIONS
                                throw std::runtime_error("Callback is not bound");
                            #endif
                        }
                    }
                //
                //////////////////////////////////////////////////////////////////////////////////////////

            private:
                CallbackType _callback;
        };
    //
    //////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Specialization for callbacks with `void` return type and no parameters.
     */
        template <typename R>
        class CallbackHandler<R, void> {
            public:
                using ReturnType = R;
                using CallbackType = R (*)();

                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Constructs an empty callback handler.
                 */
                    CallbackHandler() : _callback(nullptr) {}
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Binds a callback function.
                 * @param Callback Function pointer or lambda to be stored.
                 */
                    inline void bind(CallbackType Callback) { _callback = Callback; }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Unbinds the currently stored callback.
                 */
                    inline void unbind() { _callback = nullptr; }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Checks if a callback is currently bound.
                 * @return `true` if a callback is bound, otherwise `false`.
                 */
                    [[nodiscard]] inline bool status() const { return static_cast<bool>(_callback); }
                //
                //////////////////////////////////////////////////////////////////////////////////////////
                /**
                 * @brief Invokes the bound callback function.
                 * @throws std::runtime_error if no callback is bound and exceptions are enabled.
                 */
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
                //
                //////////////////////////////////////////////////////////////////////////////////////////

            private:
                CallbackType _callback;
        };
    //
    //////////////////////////////////////////////////////////////////////////////////////////////

} // namespace MIDILAR::SystemFoundation

#endif // MIDILAR_SYSTEM_CALLBACK_HANDLER_H
