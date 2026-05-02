#ifndef MIDILAR_MIDI_MESSAGE_PARSER_H
#define MIDILAR_MIDI_MESSAGE_PARSER_H

#include <MIDILAR_BuildSettings.h>
#include <SystemCore/CallbackHandler/CallbackHandler.h>
#include <MidiCore/Message/Message.h>
#include <stdlib.h>

namespace MIDILAR::MidiCore {

    /**
     * @class MessageParser
     * @brief Parses raw MIDI byte streams and dispatches complete messages through category-specific callbacks.
     *
     * This class receives raw MIDI data and groups bytes into complete messages according to the
     * MIDI protocol. Once a message is recognized, it is routed to the corresponding callback based
     * on its category, such as Channel Voice, Control Change, Real-Time, System Common, SysEx,
     * MIDI Time Code, or MIDI Show Control.
     *
     * The parser uses an internal buffer to accumulate incoming bytes. The buffer size can be defined
     * at construction time or resized later with ResizeBuffer().
     *
     * Callback binding supports both standalone function pointers and instance-bound methods through
     * the CallbackHandler utility.
     */
    class MessageParser {

    public:

        /**
         * @brief Callback type used by all parser output handlers.
         *
         * The callback receives a pointer to the parsed message bytes and the size of the message.
         */
        using CallbackType = MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t>::CallbackType;

    private:

        MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t> _channelVoiceCallback;

        /**
         * @brief Invokes the Channel Voice callback if one is bound.
         * @param Data Pointer to the parsed MIDI message.
         * @param Size Size of the parsed message in bytes.
         * @return True if a callback was invoked successfully, false otherwise.
         */
        bool InvokeChannelVoiceCallback(const uint8_t* Data, size_t Size);

        MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t> _controlChangeCallback;

        /**
         * @brief Invokes the Control Change callback if one is bound.
         * @param Data Pointer to the parsed MIDI message.
         * @param Size Size of the parsed message in bytes.
         * @return True if a callback was invoked successfully, false otherwise.
         */
        bool InvokeControlChangeCallback(const uint8_t* Data, size_t Size);

        MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t> _realTimeCallback;

        /**
         * @brief Invokes the Real-Time callback if one is bound.
         * @param Data Pointer to the parsed MIDI message.
         * @param Size Size of the parsed message in bytes.
         * @return True if a callback was invoked successfully, false otherwise.
         */
        bool InvokeRealTimeCallback(const uint8_t* Data, size_t Size);

        MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t> _systemCommonCallback;

        /**
         * @brief Invokes the System Common callback if one is bound.
         * @param Data Pointer to the parsed MIDI message.
         * @param Size Size of the parsed message in bytes.
         * @return True if a callback was invoked successfully, false otherwise.
         */
        bool InvokeSystemCommonCallback(const uint8_t* Data, size_t Size);

        MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t> _sysExCallback;

        /**
         * @brief Invokes the System Exclusive callback if one is bound.
         * @param Data Pointer to the parsed MIDI message.
         * @param Size Size of the parsed message in bytes.
         * @return True if a callback was invoked successfully, false otherwise.
         */
        bool InvokeSysExCallback(const uint8_t* Data, size_t Size);

        MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t> _mtcCallback;

        /**
         * @brief Invokes the MIDI Time Code callback if one is bound.
         * @param Data Pointer to the parsed MIDI message.
         * @param Size Size of the parsed message in bytes.
         * @return True if a callback was invoked successfully, false otherwise.
         */
        bool InvokeMTCCallback(const uint8_t* Data, size_t Size);

        MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t> _mscCallback;

        /**
         * @brief Invokes the MIDI Show Control callback if one is bound.
         * @param Data Pointer to the parsed MIDI message.
         * @param Size Size of the parsed message in bytes.
         * @return True if a callback was invoked successfully, false otherwise.
         */
        bool InvokeMSCCallback(const uint8_t* Data, size_t Size);

        MIDILAR::SystemCore::CallbackHandler<void, const uint8_t*, size_t> _defaultCallback;

        /**
         * @brief Invokes the default callback if one is bound.
         * @param Data Pointer to the parsed MIDI message.
         * @param Size Size of the parsed message in bytes.
         * @return True if a callback was invoked successfully, false otherwise.
         */
        bool InvokeDefaultCallback(const uint8_t* Data, size_t Size);

        /**
         * @brief Internal parser state.
         */
        enum class Status : uint8_t {
            Idle = 0,             /**< Parser is idle and waiting for a new message. */
            Processing = 1,       /**< Parser is currently accumulating a standard MIDI message. */
            ProcessingSysex = 2   /**< Parser is currently accumulating a SysEx message. */
        };

        Status _Status;             /**< Current parser state. */
        size_t _MessageSize;        /**< Current number of bytes stored in the message buffer. */
        uint8_t* _MessageBuffer;    /**< Internal message accumulation buffer. */
        size_t _MessageBufferSize;  /**< Total capacity of the internal message buffer. */

    public:

        /**
         * @brief Constructs a parser with default configuration.
         */
        MessageParser();

        /**
         * @brief Constructs a parser with a user-defined internal buffer size.
         * @param BufferSize Size of the internal message buffer in bytes.
         */
        MessageParser(size_t BufferSize);

        /**
         * @brief Destroys the parser and releases its internal resources.
         */
        ~MessageParser();

        /**
         * @brief Resizes the internal message buffer.
         * @param NewSize New buffer size in bytes.
         * @return True if the resize operation succeeded, false otherwise.
         */
        bool ResizeBuffer(size_t NewSize);

        /**
         * @brief Binds a standalone callback for Channel Voice messages.
         * @param callback Callback function to bind.
         */
        void BindChannelVoiceCallback(CallbackType callback);

        /**
         * @brief Binds an instance method for Channel Voice messages.
         * @tparam T Class type of the instance.
         * @tparam Method Member function to bind.
         * @param instance Pointer to the instance that owns the method.
         */
        template <typename T, void (T::*Method)(const uint8_t*, size_t)>
        inline void BindChannelVoiceCallback(T* instance) {
            _channelVoiceCallback.bind<T, Method>(instance);
        }

        /**
         * @brief Binds a standalone callback for Control Change messages.
         * @param callback Callback function to bind.
         */
        void BindControlChangeCallback(CallbackType callback);

        /**
         * @brief Binds an instance method for Control Change messages.
         * @tparam T Class type of the instance.
         * @tparam Method Member function to bind.
         * @param instance Pointer to the instance that owns the method.
         */
        template <typename T, void (T::*Method)(const uint8_t*, size_t)>
        inline void BindControlChangeCallback(T* instance) {
            _controlChangeCallback.bind<T, Method>(instance);
        }

        /**
         * @brief Binds a standalone callback for Real-Time messages.
         * @param callback Callback function to bind.
         */
        void BindRealTimeCallback(CallbackType callback);

        /**
         * @brief Binds an instance method for Real-Time messages.
         * @tparam T Class type of the instance.
         * @tparam Method Member function to bind.
         * @param instance Pointer to the instance that owns the method.
         */
        template <typename T, void (T::*Method)(const uint8_t*, size_t)>
        inline void BindRealTimeCallback(T* instance) {
            _realTimeCallback.bind<T, Method>(instance);
        }

        /**
         * @brief Binds a standalone callback for System Common messages.
         * @param callback Callback function to bind.
         */
        void BindSystemCommonCallback(CallbackType callback);

        /**
         * @brief Binds an instance method for System Common messages.
         * @tparam T Class type of the instance.
         * @tparam Method Member function to bind.
         * @param instance Pointer to the instance that owns the method.
         */
        template <typename T, void (T::*Method)(const uint8_t*, size_t)>
        inline void BindSystemCommonCallback(T* instance) {
            _systemCommonCallback.bind<T, Method>(instance);
        }

        /**
         * @brief Binds a standalone callback for System Exclusive messages.
         * @param callback Callback function to bind.
         */
        void BindSysExCallback(CallbackType callback);

        /**
         * @brief Binds an instance method for System Exclusive messages.
         * @tparam T Class type of the instance.
         * @tparam Method Member function to bind.
         * @param instance Pointer to the instance that owns the method.
         */
        template <typename T, void (T::*Method)(const uint8_t*, size_t)>
        inline void BindSysExCallback(T* instance) {
            _sysExCallback.bind<T, Method>(instance);
        }

        /**
         * @brief Binds a standalone callback for MIDI Time Code messages.
         * @param callback Callback function to bind.
         */
        void BindMTCCallback(CallbackType callback);

        /**
         * @brief Binds an instance method for MIDI Time Code messages.
         * @tparam T Class type of the instance.
         * @tparam Method Member function to bind.
         * @param instance Pointer to the instance that owns the method.
         */
        template <typename T, void (T::*Method)(const uint8_t*, size_t)>
        inline void BindMTCCallback(T* instance) {
            _mtcCallback.bind<T, Method>(instance);
        }

        /**
         * @brief Binds a standalone callback for MIDI Show Control messages.
         * @param callback Callback function to bind.
         */
        void BindMSCCallback(CallbackType callback);

        /**
         * @brief Binds an instance method for MIDI Show Control messages.
         * @tparam T Class type of the instance.
         * @tparam Method Member function to bind.
         * @param instance Pointer to the instance that owns the method.
         */
        template <typename T, void (T::*Method)(const uint8_t*, size_t)>
        inline void BindMSCCallback(T* instance) {
            _mscCallback.bind<T, Method>(instance);
        }

        /**
         * @brief Binds a standalone callback for uncategorized or fallback messages.
         * @param callback Callback function to bind.
         */
        void BindDefaultCallback(CallbackType callback);

        /**
         * @brief Binds an instance method for uncategorized or fallback messages.
         * @tparam T Class type of the instance.
         * @tparam Method Member function to bind.
         * @param instance Pointer to the instance that owns the method.
         */
        template <typename T, void (T::*Method)(const uint8_t*, size_t)>
        inline void BindDefaultCallback(T* instance) {
            _defaultCallback.bind<T, Method>(instance);
        }

        /**
         * @brief Unbinds the Channel Voice callback.
         */
        void UnbindChannelVoiceCallback();

        /**
         * @brief Unbinds the Control Change callback.
         */
        void UnbindControlChangeCallback();

        /**
         * @brief Unbinds the Real-Time callback.
         */
        void UnbindRealTimeCallback();

        /**
         * @brief Unbinds the System Common callback.
         */
        void UnbindSystemCommonCallback();

        /**
         * @brief Unbinds the System Exclusive callback.
         */
        void UnbindSysExCallback();

        /**
         * @brief Unbinds the MIDI Time Code callback.
         */
        void UnbindMTCCallback();

        /**
         * @brief Unbinds the MIDI Show Control callback.
         */
        void UnbindMSCCallback();

        /**
         * @brief Unbinds the default callback.
         */
        void UnbindDefaultCallback();

        /**
         * @brief Unbinds all callbacks registered in the parser.
         */
        void UnbindAll();

        /**
         * @brief Processes a block of raw MIDI data.
         *
         * The input data is parsed byte by byte and grouped into complete MIDI messages.
         * When a message is completed, it is dispatched through the corresponding callback.
         *
         * @param data Pointer to the raw MIDI data buffer.
         * @param size Number of bytes available in the buffer.
         */
        void ProcessData(const uint8_t* data, size_t size);

    private:

        /**
         * @brief Handles an incoming MIDI status byte.
         * @param data Status byte to process.
         */
        void _StatusByteHandler(uint8_t data);

        /**
         * @brief Handles a completed Real-Time message.
         */
        void _RealTimeHandler();

        /**
         * @brief Handles a completed Channel Voice message.
         */
        void _ChannelVoiceHandler();

        /**
         * @brief Handles a completed System Common message.
         */
        void _SystemCommonHandler();

        /**
         * @brief Handles a completed System Exclusive message.
         */
        void _SysExHandler();
    };

}

#endif // MIDILAR_MIDI_MESSAGE_PARSER_H