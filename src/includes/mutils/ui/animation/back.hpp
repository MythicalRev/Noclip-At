/*
    mUtils (created by miskaa)
    
    will be used by ../ui.hpp

    miskaas geode utils
*/

namespace mutils {
    namespace ui {
        namespace animation {
            namespace back {
                static const float c1 = 1.70158f;
                static const float c2 = c1 * 1.525f;
                
                static float in(float t) { return c1 * t * t * t - c1 * t * t; }
                static float out(float t) { return 1.0f + c2 * powf(t - 1.0f, 3.0f) + c1 * powf(t - 1.0f, 2.0f); }
                static float inOut(float t) { 
                    return t < 0.5f ? 
                           (powf(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f : 
                           (powf(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f; 
                }
            }
        }
    }
}

// The Unlicense license (do anything you want with this code loll)