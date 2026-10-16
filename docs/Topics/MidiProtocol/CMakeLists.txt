# Retain the historical MIDI topic hierarchy, including its nested groups.
file(GLOB_RECURSE MIDILAR_MIDI_TOPICS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_LIST_DIR}/*.dox")
midilar_add_dox(${MIDILAR_MIDI_TOPICS})
