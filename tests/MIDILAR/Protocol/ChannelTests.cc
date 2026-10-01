#include <gtest/gtest.h>

#include <MIDILAR.h>

using MIDILAR::Protocol::Channel;
using MIDILAR::Protocol::ChannelMask;
using MIDILAR::Protocol::Group;
using MIDILAR::Protocol::GroupMask;

static_assert(Channel::FromNumber(10).Wire() == 9, "drum channel");
static_assert(!Channel().IsValid(), "default channel is invalid");

// SPEC-MIDI-4
TEST(MIDILARProtocolChannelTests, WireAndNumberAreOneApart) {
    for (int32_t wire = 0; wire <= 15; ++wire) {
        const Channel channel = Channel::FromWire(wire);
        ASSERT_TRUE(channel.IsValid());
        EXPECT_EQ(channel.Wire(), wire);
        EXPECT_EQ(channel.Number(), wire + 1);
        EXPECT_EQ(Channel::FromNumber(wire + 1), channel);
        EXPECT_EQ(Group::FromWire(wire).Number(), wire + 1);
    }
}

TEST(MIDILARProtocolChannelTests, OutOfRangeInputIsInvalid) {
    const int32_t wires[] = {-1, 16, 255, 256, -256};
    for (int32_t wire : wires) {
        EXPECT_FALSE(Channel::FromWire(wire).IsValid());
        EXPECT_FALSE(Group::FromWire(wire).IsValid());
    }
    EXPECT_FALSE(Channel::FromNumber(0).IsValid());
    EXPECT_FALSE(Channel::FromNumber(17).IsValid());
    EXPECT_EQ(Channel::Invalid().Number(), 0);
    EXPECT_EQ(Channel::Invalid().Wire(), 0xFF);
    EXPECT_EQ(Channel::FromWire(16), Channel::Invalid());
}

// SPEC-MIDI-5
TEST(MIDILARProtocolChannelTests, MasksHoldSetsOfAddresses) {
    EXPECT_TRUE(ChannelMask::None().IsEmpty());
    EXPECT_EQ(ChannelMask::All().Count(), 16);
    for (int32_t wire = 0; wire <= 15; ++wire) {
        const Channel channel = Channel::FromWire(wire);
        EXPECT_TRUE(ChannelMask::All().Contains(channel));
        EXPECT_FALSE(ChannelMask::None().Contains(channel));
        const ChannelMask only = ChannelMask::Only(channel);
        EXPECT_EQ(only.Count(), 1);
        EXPECT_EQ(only.Bits(), 1u << wire);
        for (int32_t other = 0; other <= 15; ++other) {
            EXPECT_EQ(only.Contains(Channel::FromWire(other)), other == wire);
        }
    }
    EXPECT_FALSE(ChannelMask::All().Contains(Channel::Invalid()));
    EXPECT_TRUE(ChannelMask::Only(Channel::Invalid()).IsEmpty());
}

TEST(MIDILARProtocolChannelTests, MasksCombine) {
    const ChannelMask a = ChannelMask::Only(Channel::FromNumber(1)) | ChannelMask::Only(Channel::FromNumber(2));
    const ChannelMask b = ChannelMask::Only(Channel::FromNumber(2)) | ChannelMask::Only(Channel::FromNumber(3));
    EXPECT_EQ((a | b).Count(), 3);
    EXPECT_EQ(a & b, ChannelMask::Only(Channel::FromNumber(2)));
    EXPECT_EQ((~a).Count(), 14);
    EXPECT_EQ(a | ~a, ChannelMask::All());
    EXPECT_EQ(ChannelMask::FromBits(0x8001u).Count(), 2);
    EXPECT_TRUE(GroupMask::All().Contains(Group::FromNumber(16)));
}
