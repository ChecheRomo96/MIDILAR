#ifndef MIDILAR_PROTOCOL_CHANNEL_H
#define MIDILAR_PROTOCOL_CHANNEL_H

#include <stdint.h>

namespace MIDILAR::Protocol {

/**
 * @brief A four-bit wire address in `0-15`, the representation shared by MIDI
 * channels and UMP groups (SPEC-MIDI-4).
 * @ingroup MIDILAR_Protocol
 *
 * Built only through `FromWire(0-15)` or `FromNumber(1-16)` so the numbering
 * is always explicit. Any other input is the single invalid value, and a valid
 * address is always one a message can carry. `Tag` keeps channels and groups
 * from being mixed.
 */
template <typename Tag>
class WireAddress {
    static constexpr uint8_t InvalidValue = 0xFF;

    uint8_t _wire;

    constexpr explicit WireAddress(uint8_t wire) noexcept : _wire(wire) {}

public:
    /** @brief Creates the invalid address (SPEC-RT-3). */
    constexpr WireAddress() noexcept : _wire(InvalidValue) {}

    /** @brief Returns the invalid address. */
    static constexpr WireAddress Invalid() noexcept { return WireAddress(); }

    /** @brief Creates the address with wire value `wire` in `0-15`. */
    static constexpr WireAddress FromWire(int32_t wire) noexcept {
        return wire >= 0 && wire <= 15 ? WireAddress(static_cast<uint8_t>(wire)) : WireAddress();
    }

    /** @brief Creates the address a musician calls `number`, in `1-16`. */
    static constexpr WireAddress FromNumber(int32_t number) noexcept {
        return FromWire(number - 1);
    }

    /** @brief Returns `true` unless this is the invalid address. */
    constexpr bool IsValid() const noexcept { return _wire != InvalidValue; }

    /** @brief Returns the wire value `0-15`, or `0xFF` when invalid. */
    constexpr uint8_t Wire() const noexcept { return _wire; }

    /** @brief Returns the musician's number `1-16`, or `0` when invalid. */
    constexpr uint8_t Number() const noexcept {
        return IsValid() ? static_cast<uint8_t>(_wire + 1) : static_cast<uint8_t>(0);
    }

    /** @brief Compares wire values; all invalid addresses are equal. */
    friend constexpr bool operator==(WireAddress a, WireAddress b) noexcept {
        return a._wire == b._wire;
    }

    /** @brief Negation of `operator==`. */
    friend constexpr bool operator!=(WireAddress a, WireAddress b) noexcept {
        return !(a == b);
    }
};

/** @brief Tag of `Channel`. @ingroup MIDILAR_Protocol */
struct ChannelTag {};

/** @brief Tag of `Group`. @ingroup MIDILAR_Protocol */
struct GroupTag {};

/** @brief A MIDI channel, wire `0-15` (musician's `1-16`). @ingroup MIDILAR_Protocol */
using Channel = WireAddress<ChannelTag>;

/** @brief A UMP group, wire `0-15` (musician's `1-16`). @ingroup MIDILAR_Protocol */
using Group = WireAddress<GroupTag>;

/**
 * @brief A set of the sixteen channels or groups (SPEC-MIDI-5).
 * @ingroup MIDILAR_Protocol
 *
 * Filters, receivers and routes use masks; Omni is `ChannelMask::All()`.
 */
template <typename Tag>
class AddressMask {
    uint16_t _bits;

    constexpr explicit AddressMask(uint16_t bits) noexcept : _bits(bits) {}

public:
    /** @brief Creates the empty set. */
    constexpr AddressMask() noexcept : _bits(0) {}

    /** @brief Returns the empty set. */
    static constexpr AddressMask None() noexcept { return AddressMask(); }

    /** @brief Returns the set of all sixteen addresses (Omni). */
    static constexpr AddressMask All() noexcept { return AddressMask(0xFFFFu); }

    /** @brief Returns the set holding only `address`; empty when invalid. */
    static constexpr AddressMask Only(WireAddress<Tag> address) noexcept {
        return address.IsValid()
            ? AddressMask(static_cast<uint16_t>(1u << address.Wire()))
            : AddressMask();
    }

    /** @brief Builds a set from its raw bits; bit `n` is wire value `n`. */
    static constexpr AddressMask FromBits(uint16_t bits) noexcept { return AddressMask(bits); }

    /** @brief Returns the raw bits; bit `n` is wire value `n`. */
    constexpr uint16_t Bits() const noexcept { return _bits; }

    /** @brief Returns `true` when `address` is valid and in the set. */
    constexpr bool Contains(WireAddress<Tag> address) const noexcept {
        return address.IsValid() && ((static_cast<uint32_t>(_bits) >> address.Wire()) & 1u) != 0;
    }

    /** @brief Returns `true` for the empty set. */
    constexpr bool IsEmpty() const noexcept { return _bits == 0; }

    /** @brief Returns the number of addresses in the set. */
    FOUNDATION_CONSTEXPR14 uint8_t Count() const noexcept {
        uint8_t count = 0;
        for (uint32_t bit = 0; bit < 16; ++bit) {
            count = static_cast<uint8_t>(count + ((static_cast<uint32_t>(_bits) >> bit) & 1u));
        }
        return count;
    }

    /** @brief Returns the union of two sets. */
    friend constexpr AddressMask operator|(AddressMask a, AddressMask b) noexcept {
        return AddressMask(static_cast<uint16_t>(a._bits | b._bits));
    }

    /** @brief Returns the intersection of two sets. */
    friend constexpr AddressMask operator&(AddressMask a, AddressMask b) noexcept {
        return AddressMask(static_cast<uint16_t>(a._bits & b._bits));
    }

    /** @brief Returns the complement of a set. */
    friend constexpr AddressMask operator~(AddressMask a) noexcept {
        return AddressMask(static_cast<uint16_t>(~a._bits));
    }

    /** @brief Compares the sets. */
    friend constexpr bool operator==(AddressMask a, AddressMask b) noexcept {
        return a._bits == b._bits;
    }

    /** @brief Negation of `operator==`. */
    friend constexpr bool operator!=(AddressMask a, AddressMask b) noexcept {
        return !(a == b);
    }
};

/** @brief A set of channels; Omni is `ChannelMask::All()`. @ingroup MIDILAR_Protocol */
using ChannelMask = AddressMask<ChannelTag>;

/** @brief A set of UMP groups. @ingroup MIDILAR_Protocol */
using GroupMask = AddressMask<GroupTag>;

} // namespace MIDILAR::Protocol

#endif // MIDILAR_PROTOCOL_CHANNEL_H
