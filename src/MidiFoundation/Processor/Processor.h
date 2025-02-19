#ifndef MIDILAR_MIDI_PROCESSOR_H
#define MIDILAR_MIDI_PROCESSOR_H

    #include <System/BuildSettings.h>
    #include <SystemFoundation/Clock/Clock.h>
    #include <SystemFoundation/CallbackHandler/CallbackHandler.h>
    #include <MidiFoundation/Message/Message.h>
    #include <stdint.h>

    #if __has_include(<vector>)
        #include <vector>
    #endif

    namespace MIDILAR::MidiFoundation {

        /////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * @brief Base class to handle MIDI processing, input, and output.
         * 
         * The `Processor` class is designed as a foundational component for implementing MIDI devices or processors.
         * It encapsulates the functionality required to process incoming MIDI messages, generate outgoing MIDI messages, 
         * and handle clock synchronization for time-based operations. 
         * 
         * This class is intended to be used as a base class for more specialized MIDI processors, allowing developers 
         * to extend and customize its behavior for specific devices or use cases.
         * 
         * ### Key Features:
         * 
         * - **MIDI Input and Output Handling**:
         *   - Accepts incoming MIDI messages either as raw buffers or as `std::vector` iterators.
         *   - Generates MIDI output messages and dispatches them via a user-defined callback mechanism.
         * 
         * - **Clock Synchronization**:
         *   - Integrates with an external clock (`SystemTools::Clock`) to enable time-based MIDI operations.
         *   - Provides hooks for processing each clock tick and periodically updating internal state.
         * 
         * - **Device Capabilities**:
         *   - Supports dynamic enumeration of device capabilities, such as MIDI input, MIDI output, 
         *     external clocking, and internal clocking.
         *   - Capabilities are managed internally, ensuring controlled access and modification 
         *     by derived classes.
         * 
         * ### Intended Usage:
         * 
         * - **Base Class for Derived Processors**:
         *   - This class provides virtual methods (`MidiInput`, `Update`, `ClockTick`) that can be overridden 
         *     in derived classes to implement custom MIDI processing behavior.
         * 
         * - **Dynamic Capabilities**:
         *   - Devices inheriting from this class can use the protected methods to dynamically adjust their capabilities 
         *     at runtime, enabling flexible behavior for different MIDI configurations.
         * 
         * - **Event-Driven Output**:
         *   - Implements an event-driven approach for MIDI output, using user-provided callback functions 
         *     to handle outgoing MIDI messages.
         * 
         * ### Example Use Case:
         * 
         * 1. Implement a MIDI filter that processes incoming MIDI messages and modifies them before outputting:
         *    - Override the `MidiInput` and `MidiOutput` methods to add filtering logic.
         * 
         * 2. Create a MIDI clock synchronizer:
         *    - Override the `ClockTick` and `Update` methods to implement clock-based message scheduling.
         * 
         * 3. Implement a dynamic device:
         *    - Use the capability system to enable or disable features like MIDI input, MIDI output, or 
         *      clock synchronization dynamically at runtime.
         */
        class Processor {     
        public:

            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Type definition for MIDI out event callback.
             * 
             * Defines the callback signature for handling MIDI output events.
             * 
             * @param Message Pointer to the MIDI message buffer.
             * @param size Size of the MIDI message buffer.
             */
                using MidiOut_CallbackType = MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t>::CallbackType;
                using MidiOut_CallbackReferenceType = MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t>::CallbackReferenceType;
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Enum class for device capabilities and status.
             * 
             * Describes the functionality supported by a MIDI device.
             */
                enum class Capabilities : uint32_t {
                    None = 0,                   ///< Empty Capabilities Container
                    MidiIn = 1 << 0,            ///< Indicates the device supports MIDI input.
                    MidiOut = 1 << 1,           ///< Indicates the device supports MIDI output.
                    ExtClock = 1 << 2,          ///< Indicates the device uses an external clock.
                    InternalClock = 1 << 3      ///< Indicates the device uses an internal clock.
                };
            //
            /////////////////////////////////////////////////////////////////////////////////////////////

        protected:

            /////////////////////////////////////////////////////////////////////////////////////////////
            //
                MIDILAR::SystemFoundation::CallbackHandler<void, const uint8_t*, size_t> _MidiOutHandler; ///< Callback for MIDI output.
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            //
                uint32_t _capabilities = 0; ///< Bitmask for device capabilities.
            //
            /////////////////////////////////////////////////////////////////////////////////////////////

        public:

            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Default constructor for the `Processor` class.
             * 
             * Initializes the `Processor` with no MIDI output handler and no capabilities.
             */
                Processor() = default;
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Links a MIDI output handler API callback.
             * 
             * @param MidiOutHandler The callback function to handle MIDI output events.
             */
                void BindMidiOut(MidiOut_CallbackReferenceType MidiOutHandler);
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Checks if the MIDI output API is linked.
             * 
             * @return True if the API is linked, false otherwise.
             */
                bool MidiOutStatus() const;
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Checks if a specific capability is supported.
             * 
             * @param capability The capability to check.
             * @return True if the capability is supported, false otherwise.
             */
                bool HasCapability(Capabilities capability) const;
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            //
                #if __has_include(<vector>)
                /**
                 * @brief Handles incoming MIDI input from a vector.
                 * 
                 * @param begin Iterator pointing to the beginning of the vector.
                 * @param end Iterator pointing to the end of the vector.
                 * 
                 * 
                 */
                void MidiInput(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
                #endif
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            //
            /**
             * @brief Handles incoming MIDI input from a raw buffer.
             * 
             * @param Message Pointer to the MIDI message buffer.
             * @param size Size of the MIDI message buffer.
             */
                void MidiInput(const MidiFoundation::Message&);
                virtual void MidiInput(const uint8_t* Data, size_t Size);
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Updates the processor state based on the system clock.
             * 
             * @param SystemTime The current time point from the system clock.
             */
                virtual void Update(SystemFoundation::Clock::TimePoint SystemTime);
            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Processes a single clock tick.
             */
                virtual void ClockTick();
            //
            /////////////////////////////////////////////////////////////////////////////////////////////

        protected:

            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Sets the device capabilities.
             * 
             * This method is protected to ensure only derived classes can modify capabilities.
             * 
             * @param capabilities Bitmask of capabilities.
             */

                void SetCapabilities(uint32_t capabilities);

                /////////////////////////////////////////////////////////////////////////////////////////
                //
                    #if __has_include(<vector>)
                        /**
                         * @brief Sends MIDI output using a vector.
                         * 
                         * @param begin Iterator pointing to the beginning of the vector.
                         * @param end Iterator pointing to the end of the vector.
                         */
                        void MidiOutput(std::vector<uint8_t>::iterator begin, std::vector<uint8_t>::iterator end);
                    #endif
                //
                /////////////////////////////////////////////////////////////////////////////////////////

            //
            /////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @brief Sends MIDI output using a raw buffer.
             * 
             * @param Message Pointer to the MIDI message buffer.
             * @param size Size of the MIDI message buffer.
             */
                void MidiOutput(const MidiFoundation::Message& message);
                void MidiOutput(const uint8_t* Data, size_t Size);
            //
            /////////////////////////////////////////////////////////////////////////////////////////////

        private:
            

        };

        inline Processor::Capabilities operator|(
            Processor::Capabilities lhs,
            Processor::Capabilities rhs
        ) {
            return static_cast<Processor::Capabilities>(
                static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs)
            );
        }

    } // namespace MIDILAR

#endif // MIDILAR_MIDI_PROCESSOR_H