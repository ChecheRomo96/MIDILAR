function(midilar_add_macro visibility)
    target_compile_definitions(MIDILAR ${visibility} ${ARGN})
    set_property(TARGET MIDILAR APPEND PROPERTY MIDILAR_DOXYGEN_PREDEFS ${ARGN})
endfunction()

function(midilar_add_dox)
    set_property(TARGET MIDILAR APPEND PROPERTY MIDILAR_DOXYGEN_INPUTS ${ARGN})
endfunction()