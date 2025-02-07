#include "MessageParser.h"

namespace MIDILAR::MidiFoundation {

    // **Constructor**
    MessageParser::MessageParser()
        : MessageParser(3)
    {
    }

    MessageParser::MessageParser(size_t BufferSize)
        : _Status(Status::Idle),
          _MessageBuffer(nullptr),
          _MessageBufferSize(0),
          _MessageSize(0) 
    {
    	ResizeBuffer(BufferSize);
    }

    // **Destructor**
    MessageParser::~MessageParser() {
        if (_MessageBuffer) {
            free(_MessageBuffer);
            _MessageBuffer = nullptr;
        }
    }

    // **Resize Message Buffer**
    bool MessageParser::ResizeBuffer(size_t NewSize) {
	    uint8_t* new_data = (uint8_t*)realloc(_MessageBuffer, NewSize * sizeof(uint8_t));
	    if (new_data == nullptr) {
	        return false; // Memory allocation failed
	    }

	    _MessageBuffer = new_data;
	    _MessageBufferSize = NewSize;

	    // Ensure that _MessageSize does not exceed buffer capacity
	    if (_MessageSize > NewSize) {
	        _MessageSize = NewSize;
	    }

	    return true;
    }

    // **Set Callback Handlers**
    void MessageParser::BindChannelVoiceCallback(
        CallbackReferenceType callback, 
        void* instance, 
        void (*invokeFunc)(void*, uint8_t*, size_t)
    ) {
        _channelVoiceCallback.bind(callback);
        _channelVoiceContext = {instance, invokeFunc};
    }

    // **Unbind Callback**
    void MessageParser::UnbindChannelVoiceCallback() {
        _channelVoiceCallback.unbind();
        _channelVoiceContext = {nullptr, nullptr}; 
    }

    // **Invoke Callback**
    bool MessageParser::InvokeChannelVoiceCallback(uint8_t* msg, size_t size) {
        if (_channelVoiceCallback.status()) {
            if (_channelVoiceContext.instance && _channelVoiceContext.invoke) {
                _channelVoiceContext.invoke(_channelVoiceContext.instance, msg, size);
            } else {
                _channelVoiceCallback.invoke(msg, size);
            }
            return true;
        }
        return false;
    }



    // **Set Callback Handlers**
    void MessageParser::BindControlChangeCallback(
        CallbackReferenceType callback, 
        void* instance, 
        void (*invokeFunc)(void*, uint8_t*, size_t)
    ) {
        _controlChangeCallback.bind(callback);
        _controlChangeContext = {instance, invokeFunc};
    }

    // **Unbind Callback**
    void MessageParser::UnbindControlChangeCallback() {
        _controlChangeCallback.unbind();
        _controlChangeContext = {nullptr, nullptr}; 
    }

    // **Invoke Callback**
    bool MessageParser::InvokeControlChangeCallback(uint8_t* msg, size_t size) {
        if (_controlChangeCallback.status()) {
            if (_controlChangeContext.instance && _controlChangeContext.invoke) {
                _controlChangeContext.invoke(_controlChangeContext.instance, msg, size);
            } else {
                _controlChangeCallback.invoke(msg, size);
            }
            return true;
        }
        return false;
    }



    // **Set Callback Handlers**
    void MessageParser::BindRealTimeCallback(
        CallbackReferenceType callback, 
        void* instance, 
        void (*invokeFunc)(void*, uint8_t*, size_t)
    ) {
        _realTimeCallback.bind(callback);
        _realTimeContext = {instance, invokeFunc};
    }

    // **Unbind Callback**
    void MessageParser::UnbindRealTimeCallback() {
        _realTimeCallback.unbind();
        _realTimeContext = {nullptr, nullptr}; 
    }

    // **Invoke Callback**
    bool MessageParser::InvokeRealTimeCallback(uint8_t* msg, size_t size) {
        if (_realTimeCallback.status()) {
            if (_realTimeContext.instance && _realTimeContext.invoke) {
                _realTimeContext.invoke(_realTimeContext.instance, msg, size);
            } else {
                _realTimeCallback.invoke(msg, size);
            }
            return true;
        }
        return false;
    }



    // **Set Callback Handlers**
    void MessageParser::BindSystemCommonCallback(
        CallbackReferenceType callback, 
        void* instance, 
        void (*invokeFunc)(void*, uint8_t*, size_t)
    ) {
        _systemCommonCallback.bind(callback);
        _systemCommonContext = {instance, invokeFunc};
    }

    // **Unbind Callback**
    void MessageParser::UnbindSystemCommonCallback() {
        _systemCommonCallback.unbind();
        _systemCommonContext = {nullptr, nullptr}; 
    }

    // **Invoke Callback**
    bool MessageParser::InvokeSystemCommonCallback(uint8_t* msg, size_t size) {
        if (_systemCommonCallback.status()) {
            if (_systemCommonContext.instance && _systemCommonContext.invoke) {
                _systemCommonContext.invoke(_systemCommonContext.instance, msg, size);
            } else {
                _systemCommonCallback.invoke(msg, size);
            }
            return true;
        }
        return false;
    }



    // **Set Callback Handlers**
    void MessageParser::BindSysExCallback(
        CallbackReferenceType callback, 
        void* instance, 
        void (*invokeFunc)(void*, uint8_t*, size_t)
    ) {
        _sysExCallback.bind(callback);
        _sysExContext = {instance, invokeFunc};
    }

    // **Unbind Callback**
    void MessageParser::UnbindSysExCallback() {
        _sysExCallback.unbind();
        _sysExContext = {nullptr, nullptr}; 
    }

    // **Invoke Callback**
    bool MessageParser::InvokeSysExCallback(uint8_t* msg, size_t size) {
        if (_sysExCallback.status()) {
            if (_sysExContext.instance && _sysExContext.invoke) {
                _sysExContext.invoke(_sysExContext.instance, msg, size);
            } else {
                _sysExCallback.invoke(msg, size);
            }
            return true;
        }
        return false;
    }



    // **Set Callback Handlers**
    void MessageParser::BindMTCCallback(
        CallbackReferenceType callback, 
        void* instance, 
        void (*invokeFunc)(void*, uint8_t*, size_t)
    ) {
        _mtcCallback.bind(callback);
        _mtcContext = {instance, invokeFunc};
    }

    // **Unbind Callback**
    void MessageParser::UnbindMTCCallback() {
        _mtcCallback.unbind();
        _mtcContext = {nullptr, nullptr}; 
    }

    // **Invoke Callback**
    bool MessageParser::InvokeMTCCallback(uint8_t* msg, size_t size) {
        if (_mtcCallback.status()) {
            if (_mtcContext.instance && _mtcContext.invoke) {
                _mtcContext.invoke(_mtcContext.instance, msg, size);
            } else {
                _mtcCallback.invoke(msg, size);
            }
            return true;
        }
        return false;
    }



    // **Set Callback Handlers**
    void MessageParser::BindMSCCallback(
        CallbackReferenceType callback, 
        void* instance, 
        void (*invokeFunc)(void*, uint8_t*, size_t)
    ) {
        _mscCallback.bind(callback);
        _mscContext = {instance, invokeFunc};
    }

    // **Unbind Callback**
    void MessageParser::UnbindMSCCallback() {
        _mscCallback.unbind();
        _mscContext = {nullptr, nullptr}; 
    }

    // **Invoke Callback**
    bool MessageParser::InvokeMSCCallback(uint8_t* msg, size_t size) {
        if (_mscCallback.status()) {
            if (_mscContext.instance && _mscContext.invoke) {
                _mscContext.invoke(_mscContext.instance, msg, size);
            } else {
                _mscCallback.invoke(msg, size);
            }
            return true;
        }
        return false;
    }



    // **Set Callback Handlers**
    void MessageParser::BindDefaultCallback(
        CallbackReferenceType callback, 
        void* instance, 
        void (*invokeFunc)(void*, uint8_t*, size_t)
    ) {
        _defaultCallback.bind(callback);
        _defaultContext = {instance, invokeFunc};
    }

    // **Unbind Callback**
    void MessageParser::UnbindDefaultCallback() {
        _defaultCallback.unbind();
        _defaultContext = {nullptr, nullptr}; 
    }

    // **Invoke Callback**
    bool MessageParser::InvokeDefaultCallback(uint8_t* msg, size_t size) {
        if (_defaultCallback.status()) {
            if (_defaultContext.instance && _defaultContext.invoke) {
                _defaultContext.invoke(_defaultContext.instance, msg, size);
            } else {
                _defaultCallback.invoke(msg, size);
            }
            return true;
        }
        return false;
    }


    // **Unbind All Callbacks at Once**
    void MessageParser::UnbindAll() {
        _channelVoiceCallback.unbind();
        _controlChangeCallback.unbind();
        _realTimeCallback.unbind();
        _systemCommonCallback.unbind();
        _sysExCallback.unbind();
        _mtcCallback.unbind();
        _mscCallback.unbind();
        _defaultCallback.unbind();
    }

    // **Process Incoming MIDI Data**
    void MessageParser::ProcessData(const uint8_t* data, size_t size) {
        if (size == 0) return;

        for (size_t i = 0; i < size; i++) {
            _StatusByteHandler(data[i]);

            // Append New Data To Buffer
            if (_Status == Status::Processing || _Status == Status::ProcessingSysex) {
                if (_MessageSize < _MessageBufferSize) {
                    _MessageBuffer[_MessageSize++] = data[i];
                } else {
                    // Buffer Overflow
                    _Status = Status::Idle;
                }
            }

            // Check Message
            if (_Status == Status::Processing) {
                _RealTimeHandler();
                _ChannelVoiceHandler();
                _SystemCommonHandler();
            }

            // SysEx Message Handling
            if (_Status == Status::ProcessingSysex) {
                _SysExHandler();
            }
        }
    }

    // **Status Byte Handling**
    void MessageParser::_StatusByteHandler(uint8_t data) {
        if (data & 0x80) { // **Status Byte Detected**
            if (_Status == Status::ProcessingSysex) {
                if(data != MIDI_SYSEX_END){
                    // **Unexpected Status Byte Inside SysEx → Discard Message**
                    _Status = Status::Idle;
                    _MessageSize = 0;
                }
                
                return;
            }

            if (data == MIDI_SYSEX_START) { 
                _Status = Status::ProcessingSysex;
                _MessageSize = 0;
            } else {
                _Status = Status::Processing;
                _MessageSize = 0;
            }
        }
    }

    // **Real-Time Message Handling**
    void MessageParser::_RealTimeHandler() {
        if (_MessageSize == 1) {
            switch (_MessageBuffer[0]) {
                case MIDI_REALTIME_TIMING_TICK:
                case MIDI_REALTIME_START:
                case MIDI_REALTIME_CONTINUE:
                case MIDI_REALTIME_STOP:
                case MIDI_REALTIME_ACTIVE_SENSING:
                case MIDI_REALTIME_SYSTEM_RESET:
                    if (!InvokeRealTimeCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    } 
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;
            }
        }
    }

    // **Channel Voice & Control Change Messages**
    void MessageParser::_ChannelVoiceHandler() {
        if (_MessageSize == 3) {
            switch (_MessageBuffer[0] & 0xF0) {
                case MIDI_CONTROL_CHANGE:

                    if (!InvokeControlChangeCallback(_MessageBuffer, _MessageSize)) {
                        if (!InvokeChannelVoiceCallback(_MessageBuffer, _MessageSize)) {
                            InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                        } 
                    } 

                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                case MIDI_NOTE_OFF:
                case MIDI_NOTE_ON:
                case MIDI_AFTER_TOUCH:
                case MIDI_PROGRAM_CHANGE:
                case MIDI_CHANNEL_PRESSURE:
                case MIDI_PITCH_BEND:

                    if (!InvokeChannelVoiceCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    }

                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                default: break;
            }
        }
    }

    // **System Common Messages**
    void MessageParser::_SystemCommonHandler() {
        if (_MessageSize == 1) {
            switch (_MessageBuffer[0]) {
                case MIDI_TUNING_REQUEST:
                    if (!InvokeSystemCommonCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;
            }
        } 
        else if (_MessageSize == 2) {
            switch (_MessageBuffer[0]) {
                case MIDI_MTC_QUARTER_FRAME:

                    if (!InvokeMTCCallback(_MessageBuffer, _MessageSize)) { 
                        if (!InvokeSystemCommonCallback(_MessageBuffer, _MessageSize)) {
                            InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                        }
                    }

                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                case MIDI_SONG_SELECT:
                    
                    if (!InvokeSystemCommonCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    }

                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                default: break;
            }
        }
        else if (_MessageSize == 3) {
            switch (_MessageBuffer[0]) {
                case MIDI_SONG_POSITION_POINTER:

                    if (!InvokeSystemCommonCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    }

                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                default: break;
            }
        }
    }

    // **SysEx Message Handling**
    void MessageParser::_SysExHandler() {
        if (_MessageSize > 0 && _MessageBuffer[_MessageSize - 1] == MIDI_SYSEX_END) { // SysEx message complete
            
            if (!InvokeSysExCallback(_MessageBuffer, _MessageSize)) {
                InvokeDefaultCallback(_MessageBuffer, _MessageSize);
            }

            _Status = Status::Idle;
            _MessageSize = 0;
        }
    }

}
