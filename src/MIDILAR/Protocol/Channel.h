#pragma once
#include "Detail/Address.h"
namespace MIDILAR::Protocol {
/** @brief MIDI channel, FromWire(0-15) or FromNumber(1-16). @ingroup MIDILAR_Protocol */
using Channel = Detail::WireAddress<Detail::ChannelTag>;
/** @brief Channel set; Omni is All(). @ingroup MIDILAR_Protocol */
using ChannelMask = Detail::AddressMask<Detail::ChannelTag>;
}
