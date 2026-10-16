#pragma once
#include "../Detail/Address.h"
namespace MIDILAR::Protocol::Ump {
/** @brief UMP group, FromWire(0-15) or FromNumber(1-16). @ingroup MIDILAR_Ump */
using Group = Detail::WireAddress<Detail::GroupTag>;
/** @brief Set of UMP groups. @ingroup MIDILAR_Ump */
using GroupMask = Detail::AddressMask<Detail::GroupTag>;
}
