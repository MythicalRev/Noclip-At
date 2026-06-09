/*
    mUtils (created by miskaa)
    
    will be used by ../ui.hpp

    miskaas geode utils
*/

namespace mutils {
    namespace ui {
        namespace animation {
            namespace bounce {
                static float out(float t) {
                    const float n1 = 7.5625f;
                    const float d1 = 2.75f;
                    
                    if (t < 1.0f / d1) {
                        return n1 * t * t;
                    } else if (t < 2.0f / d1) {
                        return n1 * (t -= 1.5f / d1) * t + 0.75f;
                    } else if (t < 2.5f / d1) {
                        return n1 * (t -= 2.25f / d1) * t + 0.9375f;
                    } else {
                        return n1 * (t -= 2.625f / d1) * t + 0.984375f;
                    }
                }
                
                static float in(float t) { return 1.0f - out(1.0f - t); }
                
                static float inOut(float t) {
                    return t < 0.5f ? 
                           (1.0f - out(1.0f - 2.0f * t)) / 2.0f : 
                           (1.0f + out(2.0f * t - 1.0f)) / 2.0f;
                }
            }
        }
    }
}

// The Unlicense license (do anything you want with this code loll)