#ifndef MIDILAR_SYSTEM_TASK_H
#define MIDILAR_SYSTEM_TASK_H

#include <MIDILAR_BuildSettings.h>
#include <SystemCore/CallbackHandler.h>
#include <stdint.h>

namespace MIDILAR::SystemCore {

    class TaskBase {
    protected:
        SystemCore::CallbackHandler<void, void> _callback;
        bool _enabled;
        uint8_t _priority;

    public:
        virtual bool ShouldRun(uint32_t nowTicks) = 0;

        virtual void Run(uint32_t nowTicks) {
            (void)nowTicks;

            if (_enabled && _callback.status()) {
                _callback.invoke();
            }
        }
    };

}

#endif // MIDILAR_SYSTEM_TASK_H