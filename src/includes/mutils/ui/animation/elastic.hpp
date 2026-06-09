/*
    mUtils (created by miskaa)
    
    will be used by ../ui.hpp

    miskaas geode utils
*/

namespace mutils {
    namespace ui {
        namespace animation {
            namespace elastic {
                static float in(float t) {
                    const float c4 = (2.0f * M_PI) / 3.0f;
                    return t == 0.0f ? 0.0f : -powf(2.0f, 10.0f * t - 10.0f) * sinf((t * 10.0f - 10.75f) * c4);
                }
                
                static float out(float t) {
                    const float c4 = (2.0f * M_PI) / 3.0f;
                    return t == 1.0f ? 1.0f : powf(2.0f, -10.0f * t) * sinf((t * 10.0f - 0.75f) * c4) + 1.0f;
                }
                
                static float inOut(float t) {
                    const float c5 = (2.0f * M_PI) / 4.5f;
                    return t == 0.0f ? 0.0f : 
                           t == 1.0f ? 1.0f : 
                           t < 0.5f ? -(powf(2.0f, 20.0f * t - 10.0f) * sinf((20.0f * t - 11.125f) * c5)) / 2.0f : 
                           (powf(2.0f, -20.0f * t + 10.0f) * sinf((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
                }
            }
        }
    }
}

// The Unlicense license (do anything you want with this code loll)