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
            if(NewSize == _MessageBufferSize) {
                return true; // No change needed
            }
            else if(NewSize == 0) {
                free(_MessageBuffer);
                _MessageBuffer = nullptr;
                _MessageBufferSize = 0;
                _MessageSize = 0;
                return true;
            }

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
        void MessageParser::BindChannelVoiceCallback( CallbackType callback ){
            _channelVoiceCallback.bind(callback);
        }

        void MessageParser::UnbindChannelVoiceCallback() {
            _channelVoiceCallback.unbind();
        }

        bool MessageParser::InvokeChannelVoiceCallback(const uint8_t* data, size_t size) {
            if (_channelVoiceCallback.status()) {
                _channelVoiceCallback.invoke(data, size);
                return true;
            }
            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindControlChangeCallback( CallbackType callback ){
            _controlChangeCallback.bind(callback);
        }

        void MessageParser::UnbindControlChangeCallback() {
            _controlChangeCallback.unbind();
        }

        bool MessageParser::InvokeControlChangeCallback(const uint8_t* data, size_t size) {
            if (_controlChangeCallback.status()) {
                _controlChangeCallback.invoke(data, size);
                return true;
            }
            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindRealTimeCallback( CallbackType callback ){
            _realTimeCallback.bind(callback);
        }

        void MessageParser::UnbindRealTimeCallback() {
            _realTimeCallback.unbind();
        }

        bool MessageParser::InvokeRealTimeCallback(const uint8_t* data, size_t size) {
            if (_realTimeCallback.status()) {
                _realTimeCallback.invoke(data, size);
                return true;
            }
            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindSystemCommonCallback( CallbackType callback ){
            _systemCommonCallback.bind(callback);
        }

        void MessageParser::UnbindSystemCommonCallback() {
            _systemCommonCallback.unbind();
        }

        bool MessageParser::InvokeSystemCommonCallback(const uint8_t* data, size_t size) {
            if (_systemCommonCallback.status()) {
                _systemCommonCallback.invoke(data, size);
                return true;
            }
            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindSysExCallback( CallbackType callback ){
            _sysExCallback.bind(callback);
        }

        void MessageParser::UnbindSysExCallback() {
            _sysExCallback.unbind();
        }

        bool MessageParser::InvokeSysExCallback(const uint8_t* data, size_t size) {
            if (_sysExCallback.status()) {
                _sysExCallback.invoke(data, size);
                return true;
            }
            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindMTCCallback( CallbackType callback ){
            _mtcCallback.bind(callback);
        }

        void MessageParser::UnbindMTCCallback() {
            _mtcCallback.unbind();
        }

        bool MessageParser::InvokeMTCCallback(const uint8_t* data, size_t size) {
            if (_mtcCallback.status()) {
                _mtcCallback.invoke(data, size);
                return true;
            }
            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindMSCCallback( CallbackType callback ){
            _mscCallback.bind(callback);
        }

        void MessageParser::UnbindMSCCallback() {
            _mscCallback.unbind();
        }

        bool MessageParser::InvokeMSCCallback(const uint8_t* data, size_t size) {
            if (_mscCallback.status()) {
                _mscCallback.invoke(data, size);
                return true;
            }
            return false;
        }



    // **Set Callback Handlers**
        void MessageParser::BindDefaultCallback( CallbackType callback ){
            _defaultCallback.bind(callback);
        }

        void MessageParser::UnbindDefaultCallback() {
            _defaultCallback.unbind(); 
        }

        bool MessageParser::InvokeDefaultCallback(const uint8_t* data, size_t size) {
            if (_defaultCallback.status()) {
                _defaultCallback.invoke(data, size);
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
                        _MessageSize = 0;
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
        if (_MessageSize == 2) {
            switch (_MessageBuffer[0] & 0xF0) {
                case MIDI_CHANNEL_PRESSURE:
                case MIDI_PROGRAM_CHANGE:
                    if (!InvokeChannelVoiceCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                default:
                    break;
            }
        }

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
                case MIDI_PITCH_BEND:
                    if (!InvokeChannelVoiceCallback(_MessageBuffer, _MessageSize)) {
                        InvokeDefaultCallback(_MessageBuffer, _MessageSize);
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                default:
                    break;
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
