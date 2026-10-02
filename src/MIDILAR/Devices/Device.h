#ifndef MIDILAR_DEVICES_DEVICE_H
#define MIDILAR_DEVICES_DEVICE_H

#include <stdint.h>

#include <Foundation/Functional/Callback.h>
#include <MIDILAR/Protocol/Packet.h>

namespace MIDILAR::Devices {

/**
 * @brief Receives one UMP packet; devices are chained by binding an output
 * to the next device's `Process()`.
 * @ingroup MIDILAR_Devices
 *
 * @code
 * transpose.Output().Bind<MIDILAR::Devices::ScaleFilter, &MIDILAR::Devices::ScaleFilter::Process>(&scale);
 * @endcode
 */
using PacketCallback = Foundation::Functional::Callback<void, const Protocol::Packet&>;

/**
 * @brief Base of the devices: one output, no virtual functions and no
 * allocation (SPEC-RT-1, SPEC-DEV-1).
 * @ingroup MIDILAR_Devices
 *
 * Each device has `void Process(const Protocol::Packet&)` and emits zero or
 * more packets to `Output()`. An unbound output drops packets.
 */
class Device {
public:
    /** @brief Returns the output to bind to the next device or sink. */
    PacketCallback& Output() noexcept { return _output; }

protected:
    /** @brief Sends `packet` to the output, if bound and valid. */
    void Emit(const Protocol::Packet& packet) const {
        if (packet.IsValid() && _output.IsBound()) {
            _output.Invoke(packet);
        }
    }

private:
    PacketCallback _output;
};

/**
 * @brief Copies every packet to up to `Outputs` sinks, in order.
 * @ingroup MIDILAR_Devices
 */
template <uint8_t Outputs>
class Router {
    static_assert(Outputs > 0, "a router needs at least one output");

public:
    /** @brief Returns output `index`, or `nullptr` when out of range. */
    PacketCallback* Output(uint8_t index) noexcept { return index < Outputs ? &_outputs[index] : nullptr; }

    /** @brief Sends `packet` to every bound output. */
    void Process(const Protocol::Packet& packet) const {
        if (!packet.IsValid()) {
            return;
        }
        for (uint8_t i = 0; i < Outputs; ++i) {
            if (_outputs[i].IsBound()) {
                _outputs[i].Invoke(packet);
            }
        }
    }

private:
    PacketCallback _outputs[Outputs];
};

} // namespace MIDILAR::Devices

#endif // MIDILAR_DEVICES_DEVICE_H
