# Temporary compatibility adapter for the RoModularBuild migration.
# MIDILAR keeps its public cache variables while the shared implementation uses
# project-independent ROMODULAR_* names.

function(midilar_forward_toolchain_cache
    MIDILAR_NAME
    ROMODULAR_NAME
    CACHE_TYPE
    DEFAULT_VALUE
    DESCRIPTION
)
    if(NOT DEFINED ${MIDILAR_NAME})
        if(DEFINED ${ROMODULAR_NAME})
            set(${MIDILAR_NAME} "${${ROMODULAR_NAME}}" CACHE ${CACHE_TYPE}
                "${DESCRIPTION}")
        else()
            set(${MIDILAR_NAME} "${DEFAULT_VALUE}" CACHE ${CACHE_TYPE}
                "${DESCRIPTION}")
        endif()
    endif()

    if(DEFINED ${ROMODULAR_NAME} AND
       NOT "${${ROMODULAR_NAME}}" STREQUAL "${${MIDILAR_NAME}}")
        message(WARNING
            "Both ${MIDILAR_NAME} and ${ROMODULAR_NAME} are set; "
            "MIDILAR compatibility gives ${MIDILAR_NAME} precedence"
        )
    endif()

    set(${ROMODULAR_NAME} "${${MIDILAR_NAME}}" CACHE ${CACHE_TYPE}
        "${DESCRIPTION}" FORCE)
endfunction()
