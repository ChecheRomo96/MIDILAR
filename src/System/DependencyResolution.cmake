# ====================================================================
#  DependencyResolution.cmake
#  This file infers higher-level MIDILAR features based on leaf macros
# ====================================================================
    
function(resolve_feature_dependencies feature)
if(DEFINED CACHE{${feature}})
    if(${feature})
        set(dep_list "${${feature}_DEPS}")
        foreach(dep IN LISTS dep_list)
            if(NOT ${dep})
                set(${dep} ON CACHE BOOL "Resolved dependency" FORCE)
                message(STATUS " ${feature} : Enabling Required Submodule → ${dep}")
                resolve_feature_dependencies(${dep})  # Recursive
            endif()
        endforeach()
    endif()
endif()
endfunction()

    

# ================================================================
# SYSTEM FOUNDATION
# ================================================================


# ================================================================
# DSP FOUNDATION
# ================================================================

# --- LUT Subsystem ---
resolve_feature_dependencies(MIDILAR_LUT1D)
resolve_feature_dependencies(MIDILAR_LUT2D)
resolve_feature_dependencies(MIDILAR_LUT3D)
resolve_feature_dependencies(MIDILAR_GAMMA_LUT)


resolve_feature_dependencies(MIDILAR_STREAMING_EDGE_DETECTOR)
resolve_feature_dependencies(MIDILAR_CALLBACK_HANDLER)