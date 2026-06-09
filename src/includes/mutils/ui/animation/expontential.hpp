/*
    mUtils (created by miskaa)
    
    will be used by ../ui.hpp

    miskaas geode utils
*/

namespace mutils {
    namespace ui {
        namespace animation {
            namespace expontential {
                static float in(float t) { return t == 0.0f ? 0.0f : powf(2.0f, 10.0f * t - 10.0f); }
                static float out(float t) { return t == 1.0f ? 1.0f : 1.0f - powf(2.0f, -10.0f * t); }
                static float inOut(float t) { 
                    return t == 0.0f ? 0.0f : 
                           t == 1.0f ? 1.0f : 
                           t < 0.5f ? powf(2.0f, 20.0f * t - 10.0f) / 2.0f : 
                           (2.0f - powf(2.0f, -20.0f * t + 10.0f)) / 2.0f; 
                }
            }
        }
    }
}

// The Unlicense license (do anything you want with this code loll)