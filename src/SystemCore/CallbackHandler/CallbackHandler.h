#ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER_H
#define MIDILAR_SYSTEM_CALLBACK_HANDLER_H

    #include <MIDILAR_BuildSettings.h>

    #ifdef MIDILAR_EXCEPTIONS
        #include <stdexcept>
    #endif

    namespace MIDILAR::SystemCore {

        //////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief A generic callback handler for function pointers with multiple parameters.
         * 
         * This template class provides a mechanism to store, bind, and invoke callbacks dynamically.
         * It supports both function pointers, lambda functions, and non-static member functions.
         * 
         * @tparam R Return type of the callback function.
         * @tparam Args Parameter types of the callback function.
         */

        template <typename R, typename... Args>
        class CallbackHandler {
            private:
                struct CallbackContext {
                    void* instance;  // Pointer to the instance (could be any class)
                    R (*invoke)(void*, Args... args);  // Function pointer for invocation
                };

            public:
                using CallbackType = R (*)(Args...);

                CallbackHandler() : _callback(nullptr), _context{nullptr, nullptr} {}
                
                inline void bind(CallbackType Callback) {
                    _callback = Callback;
                    _context = {nullptr, nullptr};
                }

                template <typename T, R (T::*Method)(Args...)>
                void bind(T* instance) {
                    _callback = nullptr;
                    _context.instance = static_cast<void*>(instance);
                    _context.invoke = [](void* obj, Args... args) -> R {
                        return (static_cast<T*>(obj)->*Method)(args...);
                    };
                }

                inline void unbind() {
                    _callback = nullptr;
                    _context = {nullptr, nullptr};
                }

                [[nodiscard]] inline bool status() const {
                    return (_callback != nullptr) || (_context.invoke && _context.instance);
                }

                R invoke(Args... args) const {
                    if (_context.invoke && _context.instance) {
                        return _context.invoke(_context.instance, args...);
                    }
                    if (_callback) {
                        return _callback(args...);
                    }
                    #ifdef MIDILAR_EXCEPTIONS
                        throw std::runtime_error("Callback is not bound");
                    #endif
                    return R();
                }

            private:
                CallbackType _callback;
                CallbackContext _context = {nullptr, nullptr};
        };

        //////////////////////////////////////////////////////////////////////////////////////////////
        /// \cond INTERNAL

        template <typename R>
        class CallbackHandler<R, void> {
            private:
                struct CallbackContext {
                    void* instance;
                    R (*invoke)(void*);
                };

            public:
                using CallbackType = R (*)();

                CallbackHandler() : _callback(nullptr), _context{nullptr, nullptr} {}
                
                inline void bind(CallbackType Callback) {
                    _callback = Callback;
                    _context = {nullptr, nullptr};
                }

                template <typename T, R (T::*Method)()>
                void bind(T* instance) {
                    _callback = nullptr;
                    _context.instance = static_cast<void*>(instance);
                    _context.invoke = [](void* obj) -> R {
                        return (static_cast<T*>(obj)->*Method)();
                    };
                }

                inline void unbind() {
                    _callback = nullptr;
                    _context = {nullptr, nullptr};
                }

                [[nodiscard]] inline bool status() const {
                    return (_callback != nullptr) || (_context.invoke && _context.instance);
                }

                R invoke() const {
                    if (_context.invoke && _context.instance) {
                        return _context.invoke(_context.instance);
                    }
                    if (_callback) {
                        return _callback();
                    }
                    #ifdef MIDILAR_EXCEPTIONS
                        throw std::runtime_error("Callback is not bound");
                    #endif
                    return R();
                }

            private:
                CallbackType _callback;
                CallbackContext _context = {nullptr, nullptr};
        };

        template <typename... Args>
        class CallbackHandler<void, Args...> {
            private:
                struct CallbackContext {
                    void* instance;
                    void (*invoke)(void*, Args...);
                };

            public:
                using CallbackType = void (*)(Args...);

                CallbackHandler() : _callback(nullptr), _context{nullptr, nullptr} {}
                
                inline void bind(CallbackType Callback) {
                    _callback = Callback;
                    _context = {nullptr, nullptr};
                }

                template <typename T, void (T::*Method)(Args...)>
                void bind(T* instance) {
                    _callback = nullptr;
                    _context.instance = static_cast<void*>(instance);
                    _context.invoke = [](void* obj, Args... args) {
                        (static_cast<T*>(obj)->*Method)(args...);
                    };
                }

                inline void unbind() {
                    _callback = nullptr;
                    _context = {nullptr, nullptr};
                }

                [[nodiscard]] inline bool status() const {
                    return (_callback != nullptr) || (_context.invoke && _context.instance);
                }

                void invoke(Args... args) const {
                    if (_context.invoke && _context.instance) {
                        _context.invoke(_context.instance, args...);
                        return;
                    }
                    if (_callback) {
                        _callback(args...);
                        return;
                    }
                    #ifdef MIDILAR_EXCEPTIONS
                        throw std::runtime_error("Callback is not bound");
                    #endif
                }

            private:
                CallbackType _callback;
                CallbackContext _context = {nullptr, nullptr};
        };

        template <>
        class CallbackHandler<void, void> {
            private:
                struct CallbackContext {
                    void* instance;
                    void (*invoke)(void*);
                };

            public:
                using CallbackType = void (*)();

                CallbackHandler() : _callback(nullptr), _context{nullptr, nullptr} {}
                
                inline void bind(CallbackType Callback) {
                    _callback = Callback;
                    _context = {nullptr, nullptr};
                }

                template <typename T, void (T::*Method)()>
                void bind(T* instance) {
                    _callback = nullptr;
                    _context.instance = static_cast<void*>(instance);
                    _context.invoke = [](void* obj) {
                        (static_cast<T*>(obj)->*Method)();
                    };
                }

                inline void unbind() {
                    _callback = nullptr;
                    _context = {nullptr, nullptr};
                }

                [[nodiscard]] inline bool status() const {
                    return (_callback != nullptr) || (_context.invoke && _context.instance);
                }

                void invoke() const {
                    if (_context.invoke && _context.instance) {
                        _context.invoke(_context.instance);
                        return;
                    }
                    if (_callback) {
                        _callback();
                        return;
                    }
                    #ifdef MIDILAR_EXCEPTIONS
                        throw std::runtime_error("Callback is not bound");
                    #endif
                }

            private:
                CallbackType _callback;
                CallbackContext _context = {nullptr, nullptr};
        };
        
        /// \endcond

    } // namespace MIDILAR::SystemCore

#endif // MIDILAR_SYSTEM_CALLBACK_HANDLER_H
