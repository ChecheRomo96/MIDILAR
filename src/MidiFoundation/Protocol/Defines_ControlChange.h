#ifndef MIDILAR_PROTOCOL_CONTROL_CHANGE_DEFINES_H
#define MIDILAR_PROTOCOL_CONTROL_CHANGE_DEFINES_H

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Control Change Messages

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // General CC's
        
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_CC_BankSelect
             * @brief Preprocessor macro for constructing MIDI CC Bank Select Messages
             */
               #define MIDI_BANK_SELECT 0x00
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_CC_Modulation
             * @brief Preprocessor macro for constructing MIDI CC Modulation Messages
             */
               #define MIDI_MODULATION 0x01
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_CC_BREATH_CONTROL
             * @brief Preprocessor macro for constructing MIDI CC Breath Control Messages
             */
               #define MIDI_BREATH_CONTROL 0x02
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_CC_FOOT_PEDAL
             * @brief Preprocessor macro for constructing MIDI CC Foot Pedal Messages
             */
               #define MIDI_FOOT_PEDAL 0x04
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_CC_PORTAMENTO
             * @brief Preprocessor macro for constructing MIDI CC Portamento Messages
             */
               #define MIDI_PORTAMENTO 0x05
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_CC_VOLUME
             * @brief Preprocessor macro for constructing MIDI CC Volume Messages
             */
               #define MIDI_VOLUME 0x07
            // 
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_CC_BALANCE
             * @brief Preprocessor macro for constructing MIDI CC Balance Messages
             */
               #define MIDI_BALANCE 0x08
            //  
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Pan 

               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**
                * @ingroup MIDI_CC_PAN
                * @brief Preprocessor macro for constructing MIDI CC Pan Messages
                *
                * The MIDI Control Change (CC) number for pan is used to control stereo panning of a sound.
                */
                  #define MIDI_PAN 0x0A
               //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**
                * @ingroup MIDI_CC_PAN
                * @brief MIDI CC value for pan fully to the right.
                */
                  #define MIDI_PAN_R 0x7F
               //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**
                * @ingroup MIDI_CC_PAN
                * @brief MIDI CC value for pan in the center.
                */
                  #define MIDI_PAN_CENTER 0x40
               //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**
                * @ingroup MIDI_CC_PAN
                * @brief MIDI CC value for pan fully to the left.
                */
                  #define MIDI_PAN_L 0x00
               //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_CC_EXPRESSION
             * @brief Preprocessor macro for constructing MIDI CC Expression Messages
             */
               #define MIDI_EXPRESSION 0x0B
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Channel Mode Messages

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // All Sound Off
            /**
             * @ingroup MIDI_CC_ALL_SOUND_OFF
             */
               #define MIDI_ALL_SOUND_OFF 0x78
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Reset All Controllers
            /**
             * @ingroup MIDI_CC_RESET_ALL_CONTROLLERS
             * @brief Preprocessor macro for constructing MIDI Reset All Controllers Message
             */
               #define MIDI_RESET_ALL_CONTROLLERS 0x79
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Local Control
               
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**
                * @ingroup MIDI_CC_LOCAL_CONTROL
                * @brief Preprocessor macro for constructing MIDI Local Control Messages (CC #122).
                */
                  #define MIDI_LOCAL_CONTROL 0x7A
               //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**
                * @ingroup MIDI_CC_LOCAL_CONTROL
                * @brief Disables local sound generation (Local Control Off).
                */
                  #define MIDI_LOCAL_CONTROL_OFF 0x00
               //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
               /**
                * @ingroup MIDI_CC_LOCAL_CONTROL
                * @brief Enables local sound generation (Local Control On).
                */
                  #define MIDI_LOCAL_CONTROL_ON  0x7F
               //
               ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // All Notes Off Message
            /**
             * @ingroup MIDI_CC_ALL_NOTES_OFF
             * @brief Preprocessor macro for constructing MIDI All  Notes Off Message
             */
               #define MIDI_ALL_NOTES_OFF 0x7B
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Omni On
            /**
             * @ingroup MIDI_CC_OMNI
             * @brief Preprocessor macro for constructing MIDI Omni On Messages
             */
               #define MIDI_OMNI_OFF 0x7C
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Omni Off
            /**
             * @ingroup MIDI_CC_OMNI
             * @brief Preprocessor macro for constructing MIDI Omni Off Messages
             */
               #define MIDI_OMNI_ON 0x7D
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Reset All Controllers
            /**
             * @ingroup MIDI_CC_POLYPHONY
             * @brief Preprocessor macro for constructing MIDI CC All Sound Off Message
             */
               #define MIDI_MONO_ON 0x7E
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Reset All Controllers
            /**
             * @ingroup MIDI_CC_POLYPHONY
             * @brief Preprocessor macro for constructing MIDI CC All Sound Off Message
             */
               #define MIDI_POLY_ON 0x7F
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // NRPN and RPN Messages

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_DATA_ENTRY
             * @brief Preprocessor macro for constructing NRPN and RPM Data Entry Messages 
             */
                #define MIDI_DATA_ENTRY_MSB 0x06
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_DATA_ENTRY
             * @brief Preprocessor macro for constructing NRPN and RPM Data Entry Messages 
             */
                #define MIDI_DATA_ENTRY_LSB 0x26
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_INCREMENT_INCREMENT_MESSAGES
             * @brief Preprocessor macro for constructing NRPN and RPM Data Increment Messages 
             */
                #define MIDI_NRPN_DATA_INCREMENT 0x60
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_INCREMENT_DECREMENT_MESSAGES
             * @brief Preprocessor macro for constructing NRPN and RPM Data Decrement Messages 
             */
                #define MIDI_NRPN_DATA_DECREMENT 0x61
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_NRPN_MESSAGES
             * @brief Preprocessor macro for constructing NRPN Messages
             */
                #define MIDI_NRPN_LSB 0x62
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_NRPN_MESSAGES
             * @brief Preprocessor macro for constructing NRPN Messages
             */
                #define MIDI_NRPN_MSB 0x63
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_RPN_MESSAGES
             * @brief Preprocessor macro for constructing RPN Messages
             */
                #define MIDI_RPN_LSB 0x64
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            /**
             * @ingroup MIDI_RPN_MESSAGES
             * @brief Preprocessor macro for constructing RPN Messages
             */
                #define MIDI_RPN_MSB 0x65
            //
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif//MIDILAR_PROTOCOL_CONTROL_CHANGE_DEFINES_H