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
    void MessageParser::BindChannelVoiceCallback(CallbackReferenceType callback) { _channelVoiceCallback.bind(callback); }
    void MessageParser::BindControlChangeCallback(CallbackReferenceType callback) { _controlChangeCallback.bind(callback); }
    void MessageParser::BindRealTimeCallback(CallbackReferenceType callback) { _realTimeCallback.bind(callback); }
    void MessageParser::BindSystemCommonCallback(CallbackReferenceType callback) { _systemCommonCallback.bind(callback); }
    void MessageParser::BindSysExCallback(CallbackReferenceType callback) { _sysExCallback.bind(callback); }
    void MessageParser::BindMTCCallback(CallbackReferenceType callback) { _mtcCallback.bind(callback); }
    void MessageParser::BindMSCCallback(CallbackReferenceType callback) { _mscCallback.bind(callback); }
    void MessageParser::BindDefaultCallback(CallbackReferenceType callback) { _defaultCallback.bind(callback); }

    void MessageParser::UnbindChannelVoiceCallback() { _channelVoiceCallback.unbind(); }
    void MessageParser::UnbindControlChangeCallback() { _controlChangeCallback.unbind(); }
    void MessageParser::UnbindRealTimeCallback() { _realTimeCallback.unbind(); }
    void MessageParser::UnbindSystemCommonCallback() { _systemCommonCallback.unbind(); }
    void MessageParser::UnbindSysExCallback() { _sysExCallback.unbind(); }
    void MessageParser::UnbindMTCCallback() { _mtcCallback.unbind(); }
    void MessageParser::UnbindMSCCallback() { _mscCallback.unbind(); }
    void MessageParser::UnbindDefaultCallback() { _defaultCallback.unbind(); }

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
            _message.SetRawData(_MessageBuffer, _MessageSize);
            switch (_MessageBuffer[0]) {
                case MIDI_REALTIME_TIMING_TICK:
                case MIDI_REALTIME_START:
                case MIDI_REALTIME_CONTINUE:
                case MIDI_REALTIME_STOP:
                case MIDI_REALTIME_ACTIVE_SENSING:
                case MIDI_REALTIME_SYSTEM_RESET:
                    if (_realTimeCallback.status()) {
                        _realTimeCallback.invoke(_message);
                    } else if (_defaultCallback.status()) {
                        _defaultCallback.invoke(_message);
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
            _message.SetRawData(_MessageBuffer, _MessageSize);

            switch (_MessageBuffer[0] & 0xF0) {
                case MIDI_CONTROL_CHANGE:
                    if (_controlChangeCallback.status()) {
                        _controlChangeCallback.invoke(_message);
                    } 
                    else if (_channelVoiceCallback.status()) {
                        _channelVoiceCallback.invoke(_message);
                    } 
                    else if (_defaultCallback.status()) {
                        _defaultCallback.invoke(_message);
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
                    if (_channelVoiceCallback.status()) {
                        _channelVoiceCallback.invoke(_message);
                    } else if (_defaultCallback.status()) {
                        _defaultCallback.invoke(_message);
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
            _message.SetRawData(_MessageBuffer, _MessageSize);
            switch (_MessageBuffer[0]) {
                case MIDI_TUNING_REQUEST:
                    if (_systemCommonCallback.status()) {
                        _systemCommonCallback.invoke(_message);
                    } else if (_defaultCallback.status()) {
                        _defaultCallback.invoke(_message);
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;
            }
        } 
        else if (_MessageSize == 2) {
            _message.SetRawData(_MessageBuffer, _MessageSize);
            switch (_MessageBuffer[0]) {
                case MIDI_MTC_QUARTER_FRAME:
                    if (_mtcCallback.status()) {
                        _mtcCallback.invoke(_message);
                    }
                    else if (_systemCommonCallback.status()) {
                        _systemCommonCallback.invoke(_message);
                    } 
                    else if (_defaultCallback.status()) {
                        _defaultCallback.invoke(_message);
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                case MIDI_SONG_SELECT:
                    if (_systemCommonCallback.status()) {
                        _systemCommonCallback.invoke(_message);
                    } else if (_defaultCallback.status()) {
                        _defaultCallback.invoke(_message);
                    }
                    _Status = Status::Idle;
                    _MessageSize = 0;
                    break;

                default: break;
            }
        }
        else if (_MessageSize == 3) {
            _message.SetRawData(_MessageBuffer, _MessageSize);
            switch (_MessageBuffer[0]) {
                case MIDI_SONG_POSITION_POINTER:
                    if (_systemCommonCallback.status()) {
                        _systemCommonCallback.invoke(_message);
                    } else if (_defaultCallback.status()) {
                        _defaultCallback.invoke(_message);
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
            _message.SetRawData(_MessageBuffer, _MessageSize);

            if (_sysExCallback.status()) {
                _sysExCallback.invoke(_message);
            } else if (_defaultCallback.status()) {
                _defaultCallback.invoke(_message);
            }

            _Status = Status::Idle;
            _MessageSize = 0;
        }
    }

}
