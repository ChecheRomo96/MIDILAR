#ifndef MIDILAR_DSP_FOUNDATION_H
#define MIDILAR_DSP_FOUNDATION_H

    #include <MIDILAR_BuildSettings.h>
    
    #if __has_include(<SystemFoundation/CallbackHandler/CallbackHandler.h>)

        #ifndef MIDILAR_SYSTEM_CALLBACK_HANDLER
            define MIDILAR_SYSTEM_CALLBACK_HANDLER
        #endif
        #include <SystemFoundation/CallbackHandler/CallbackHandler.h>
    #endif


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
            #include <dspFoundation/LUT/Periodic/SineLUT.h>
            #include <dspFoundation/LUT/Periodic/TriangleLUT.h>
            #include <dspFoundation/LUT/Periodic/SawLUT.h>
            #include <dspFoundation/LUT/Periodic/SquareLUT.h>
        #endif
        
        #ifdef MIDILAR_LUT_SHAPING
                #include <dspFoundation/LUT/Shaping/GammaLUT.h>
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
            #include <dspFoundation/Generators/Windowing/Windowing.h>
        #endif

        #ifdef MIDILAR_GENERATORS_ENVELOPES
            #include <dspFoundation/Generators/Envelopes/Envelopes.h>
        #endif
    #endif
    
    #ifdef MIDILAR_STREAMING
        #include <dspFoundation/Streaming/StreamingBase.h>
    #endif

    #ifdef MIDILAR_TRANSFORMS
        
        #ifdef MIDILAR_DFT
            #include <dspFoundation/Transforms/DFT.h>
        #endif
    #endif

    
    

#endif//MIDILAR_DSP_FOUNDATION_H