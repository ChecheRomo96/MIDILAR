#ifndef MIDILAR_DSP_FOUNDATION_H
#define MIDILAR_DSP_FOUNDATION_H

    #include <System/BuildSettings.h>
    

    #ifdef MIDILAR_LUT
    
        #ifdef MIDILAR_LUT1D
            #include <dspFoundation/LUT/LUT1D/LUT1D.h>
        #endif
        
        #ifdef MIDILAR_LUT2D
            #include <dspFoundation/LUT/LUT2D/LUT2D.h>
        #endif
        
        #ifdef MIDILAR_LUT3D
            #include <dspFoundation/LUT/LUT3D/LUT3D.h>
        #endif
        
        #ifdef MIDILAR_LUT_PERIODIC
            #ifdef MIDILAR_LUT_PERIODIC_SINE
                #include <dspFoundation/LUT/Periodic/SineLUT.h>
            #endif
            
            #ifdef MIDILAR_LUT_PERIODIC_TRIANGLE
                #include <dspFoundation/LUT/Periodic/TriangleLUT.h>
            #endif
            
            #ifdef MIDILAR_LUT_PERIODIC_SAW
                #include <dspFoundation/LUT/Periodic/SawLUT.h>
            #endif
            
            #ifdef MIDILAR_LUT_PERIODIC_SQUARE
                #include <dspFoundation/LUT/Periodic/SquareLUT.h>
            #endif
        #endif
        
        #ifdef MIDILAR_LUT_SHAPING
            #ifdef MIDILAR_GAMMA_LUT
                #include <dspFoundation/LUT/Periodic/SineLUT.h>
            #endif
            
            #ifdef MIDILAR_LUT_PERIODIC_TRIANGLE
                #include <dspFoundation/LUT/Periodic/TriangleLUT.h>
            #endif
            
            #ifdef MIDILAR_LUT_PERIODIC_SAW
                #include <dspFoundation/LUT/Periodic/SawLUT.h>
            #endif
            
            #ifdef MIDILAR_LUT_PERIODIC_SQUARE
                #include <dspFoundation/LUT/Periodic/SquareLUT.h>
            #endif
        #endif
    #endif

    #ifdef MIDILAR_INTERPOLATORS
        #ifdef MIDILAR_INTERPOLATORS_LINEAR
            #include <dspFoundation/Interpolators/Linear/Linear.h>
        #endif
        
        #ifdef MIDILAR_INTERPOLATORS_BILINEAR
            #include <dspFoundation/Interpolators/Bilinear/Bilinear.h>
        #endif
    
        #ifdef MIDILAR_INTERPOLATORS_TRILINEAR
            #include <dspFoundation/Interpolators/Trilinear/Trilinear.h>
        #endif
    #endif

    #ifdef MIDILAR_GENERATORS
        #ifdef MIDILAR_GENERATORS_PERIODIC
            #include <dspFoundation/Generators/Periodic/Periodic.h>
        #endif
        
        #ifdef MIDILAR_GENERATORS_SHAPING
            #include <dspFoundation/Generators/Shaping/Shaping.h>
        #endif
    
        #ifdef MIDILAR_GENERATORS_WINDOWING
            #include <dspFoundation/Generators/Windowing.h>
        #endif

        #ifdef MIDILAR_GENERATORS_ENVELOPES
            #include <dspFoundation/Generators/Envelopes/Envelopes.h>
        #endif
    #endif
    
    #ifdef MIDILAR_STREAMING
        #ifdef MIDILAR_STREAMING_LOGIC
            #ifdef MIDILAR_LEVEL_TRANSITION_TRACKER
                #include <dspFoundation/Streaming/Logic/LevelTransitionTracker.h>
                
                #ifdef MIDILAR_STREAMING_EDGE_DETECTOR
                    #include <dspFoundation/Streaming/Logic/EdgeDetector.h>
                #endif
                
                #ifdef MIDILAR_STREAMING_SCHMITT_TRIGGER
                    #include <dspFoundation/Streaming/Logic/SchmittTrigger.h>
                #endif
            #endif

            #ifdef MIDILAR_STATE_CHANGE_GATE
                #include <dspFoundation/Streaming/Logic/StateChangeGate.h>
            #endif
        #endif
    #endif

    #ifdef MIDILAR_TRANSFORMS
        
        #ifdef MIDILAR_DFT
            #include <dspFoundation/Transforms/DFT.h>
        #endif
    #endif

    
    

#endif//MIDILAR_DSP_FOUNDATION_H