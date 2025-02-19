#ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER_H
#define MIDILAR_SYSTEM_CALLBACK_HANDLER_H

    #include <System/BuildSettings.h>

    #ifdef MIDILAR_EXCEPTIONS
        #include <exception>
    #endif

    namespace MIDILAR::SystemFoundation {

        //////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief A generic callback handler for function pointers with multiple parameters.
         * 
         * This template class provides a mechanism to store, bind, and invoke callbacks dynamically.
         * It supports both function pointers and lambda functions.
         * 
         * @tparam R Return type of the callback function.
         * @tparam Args Parameter types of the callback function.
         */

            template <typename R, typename... Args>
            class CallbackHandler {
                public:
                    using CallbackType = R (*)(Args...);
                    using CallbackReferenceType = R (&)(Args...);

                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Constructs an empty callback handler.
                     */
                        CallbackHandler() : _callback(nullptr) {}
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Binds a callback function.
                     * @param Callback Function pointer or lambda to be stored.
                     */
                        inline void bind(CallbackReferenceType Callback) { _callback = Callback; }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Unbinds the currently stored callback.
                     */
                        inline void unbind() { _callback = nullptr; }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Checks if a callback is currently bound.
                     * @return `true` if a callback is bound, otherwise `false`.
                     */
                        [[nodiscard]] inline bool status() const { return static_cast<bool>(_callback); }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Invokes the bound callback function with parameters.
                     * @param args The parameters to pass to the callback function.
                     * @return The result of the callback function execution.
                     * @throws std::runtime_error if no callback is bound and exceptions are enabled.
                     */
                        R invoke(Args... args) const {
                            if (_callback) {
                                return _callback(args...);
                            }
                            #ifdef MIDILAR_EXCEPTIONS
                                throw std::runtime_error("Callback is not bound");
                            #endif
                            return R(); // Default return value in case exceptions are disabled
                        }
                    //
                    //////////////////////////////////////////////////////////////////////////////////

                private:
                    CallbackType _callback;  ///< Stores the function pointer.
            };
        //
        //////////////////////////////////////////////////////////////////////////////////////////////    
        /**
         * @brief Specialization for callbacks with no parameters.
         */
            template <typename R>
            class CallbackHandler<R,void> {
                public:
                    using CallbackType = R (*)();
                    using CallbackReferenceType = R (&)();

                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Constructs an empty callback handler.
                     */
                        CallbackHandler() : _callback(nullptr) {}
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Binds a callback function.
                     * @param Callback Function pointer or lambda to be stored.
                     */
                        inline void bind(CallbackReferenceType Callback) { _callback = Callback; }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Unbinds the currently stored callback.
                     */
                        inline void unbind() { _callback = nullptr; }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Checks if a callback is currently bound.
                     * @return `true` if a callback is bound, otherwise `false`.
                     */
                        [[nodiscard]] inline bool status() const { return static_cast<bool>(_callback); }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Invokes the bound callback function.
                     * @throws std::runtime_error if no callback is bound and exceptions are enabled.
                     */
                        R invoke() const {
                            if (_callback) {
                                return _callback();
                            }
                            #ifdef MIDILAR_EXCEPTIONS
                                throw std::runtime_error("Callback is not bound");
                            #endif
                            return R(); // Default return value in case exceptions are disabled
                        }
                    //
                    //////////////////////////////////////////////////////////////////////////////////

                private:
                    CallbackType _callback;
            };
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Specialization for callbacks with a `void` return type.
         * @tparam Args Parameter types of the callback function.
         */
            template <typename... Args>
            class CallbackHandler<void, Args...> {
                public:
                    using CallbackType = void (*)(Args...);
                    using CallbackReferenceType = void (&)(Args...);

                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Constructs an empty callback handler.
                     */
                        CallbackHandler() : _callback(nullptr) {}
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Binds a callback function.
                     * @param Callback Function pointer or lambda to be stored.
                     */
                        inline void bind(CallbackReferenceType Callback) { _callback = Callback; }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Unbinds the currently stored callback.
                     */
                        inline void unbind() { _callback = nullptr; }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Checks if a callback is currently bound.
                     * @return `true` if a callback is bound, otherwise `false`.
                     */
                        [[nodiscard]] inline bool status() const { return static_cast<bool>(_callback); }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Invokes the bound callback function with parameters.
                     * @param args The parameters to pass to the callback function.
                     * @throws std::runtime_error if no callback is bound and exceptions are enabled.
                     */
                        void invoke(Args... args) const {
                            if (_callback) {
                                _callback(args...);
                            } else {
                                #ifdef MIDILAR_EXCEPTIONS
                                    throw std::runtime_error("Callback is not bound");
                                #endif
                            }
                        }
                    //
                    //////////////////////////////////////////////////////////////////////////////////

                private:
                    CallbackType _callback;
            };
        //
        //////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Specialization for `void` return type with **no parameters**
         */
            template <>
            class CallbackHandler<void, void> {
                public:
                    using CallbackType = void (*)();
                    using CallbackReferenceType = void (&)();

                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Constructs an empty callback handler.
                     */
                        CallbackHandler() : _callback(nullptr) {}
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Binds a callback function.
                     * @param Callback Function pointer or lambda to be stored.
                     */
                        inline void bind(CallbackReferenceType Callback) { _callback = Callback; }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Unbinds the currently stored callback.
                     */
                        inline void unbind() { _callback = nullptr; }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Checks if a callback is currently bound.
                     * @return `true` if a callback is bound, otherwise `false`.
                     */
                        [[nodiscard]] inline bool status() const { return static_cast<bool>(_callback); }
                    //
                    //////////////////////////////////////////////////////////////////////////////////
                    /**
                     * @brief Invokes the bound callback function with parameters.
                     * @param args The parameters to pass to the callback function.
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
                    //////////////////////////////////////////////////////////////////////////////////

                private:
                    CallbackType _callback;
            };
        //
        //////////////////////////////////////////////////////////////////////////////////////////////

    } // namespace MIDILAR::SystemFoundation

#endif // MIDILAR_SYSTEM_CALLBACK_HANDLER_H
