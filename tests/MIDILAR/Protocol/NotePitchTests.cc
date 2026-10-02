#include <gtest/gtest.h>

#include <MIDILAR.h>

using namespace MIDILAR::Protocol;
using MCC::Accidental;
using MCC::ChromaticIndex;
using MCC::Key;
using MCC::KeyMode;
using MCC::Letter;
using MCC::NoteName;
using MCC::Pitch;

static_assert(ToChromaticIndex(NoteNumber::FromValue(60)).Value() == 60, "MIDI 60 is index 60");
static_assert(!ToNoteNumber(ChromaticIndex(128)).IsValid(), "128 is out of range");

// SPEC-MIDI-7
TEST(MIDILARProtocolNotePitchTests, EveryNoteRoundTrips) {
    for (int32_t n = 0; n <= 127; ++n) {
        const NoteNumber note = NoteNumber::FromValue(n);
        EXPECT_EQ(ToChromaticIndex(note).Value(), n);
        EXPECT_EQ(ToNoteNumber(ChromaticIndex(n)), note);
        const Pitch pitch = ToPitch(note);
        ASSERT_TRUE(pitch.IsValid()) << n;
        EXPECT_EQ(ToNoteNumber(pitch), note);
    }
}

TEST(MIDILARProtocolNotePitchTests, MiddleCIsC4AndConcertAIsA4) {
    EXPECT_EQ(ToPitch(NoteNumber::FromValue(60)), Pitch(Letter::C, 4));
    EXPECT_EQ(ToPitch(NoteNumber::FromValue(69)), Pitch(Letter::A, 4));
    EXPECT_EQ(ToPitch(NoteNumber::FromValue(0)), Pitch(Letter::C, -1));
    EXPECT_EQ(ToPitch(NoteNumber::FromValue(127)), Pitch(Letter::G, 9));
    EXPECT_EQ(ToNoteNumber(Pitch(Letter::C, 4)), NoteNumber::FromValue(60));
}

TEST(MIDILARProtocolNotePitchTests, RejectsPitchesOutsideTheMidiRange) {
    EXPECT_FALSE(ToNoteNumber(ChromaticIndex(-1)).IsValid());
    EXPECT_FALSE(ToNoteNumber(ChromaticIndex(128)).IsValid());
    EXPECT_FALSE(ToNoteNumber(ChromaticIndex::Invalid()).IsValid());
    EXPECT_FALSE(ToNoteNumber(Pitch(Letter::B, -2)).IsValid());
    EXPECT_FALSE(ToNoteNumber(Pitch(NoteName(Letter::C, Accidental::Flat()), -1)).IsValid());
    EXPECT_FALSE(ToNoteNumber(Pitch(NoteName(Letter::G, Accidental::Sharp()), 9)).IsValid());
    EXPECT_FALSE(ToNoteNumber(Pitch::Invalid()).IsValid());
    EXPECT_FALSE(ToChromaticIndex(NoteNumber::Invalid()).IsValid());
    EXPECT_FALSE(ToPitch(NoteNumber::Invalid()).IsValid());
}

TEST(MIDILARProtocolNotePitchTests, EnharmonicSpellingsShareANote) {
    EXPECT_EQ(ToNoteNumber(Pitch(NoteName(Letter::C, Accidental::Flat()), 4)), NoteNumber::FromValue(59));
    EXPECT_EQ(ToNoteNumber(Pitch(NoteName(Letter::B, Accidental::Sharp()), 3)), NoteNumber::FromValue(60));
}

TEST(MIDILARProtocolNotePitchTests, SpellsInTheKey) {
    const NoteNumber cSharp = NoteNumber::FromValue(61);
    const Pitch sharp(NoteName(Letter::C, Accidental::Sharp()), 4);
    const Pitch flat(NoteName(Letter::D, Accidental::Flat()), 4);
    EXPECT_EQ(ToPitch(cSharp, Key(NoteName(Letter::D, Accidental::Natural()), KeyMode::Major)), sharp);
    EXPECT_EQ(ToPitch(cSharp, Key(NoteName(Letter::F, Accidental::Natural()), KeyMode::Major)), flat);
    EXPECT_EQ(ToPitch(NoteNumber::FromValue(59), Key(NoteName(Letter::G, Accidental::Flat()), KeyMode::Major)),
              Pitch(NoteName(Letter::C, Accidental::Flat()), 4));
    EXPECT_FALSE(ToPitch(cSharp, Key::Invalid()).IsValid());
}
