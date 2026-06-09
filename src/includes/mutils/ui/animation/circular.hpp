/*
    mUtils (created by miskaa)
    
    will be used by ../ui.hpp

    miskaas geode utils
*/

namespace mutils {
    namespace ui {
        namespace animation {
            namespace circular {
                static float in(float t) { return 1.0f - sqrtf(1.0f - powf(t, 2.0f)); }
                static float out(float t) { return sqrtf(1.0f - powf(t - 1.0f, 2.0f)); }
                static float inOut(float t) { return t < 0.5f ? (1.0f - sqrtf(1.0f - powf(2.0f * t, 2.0f))) / 2.0f : (sqrtf(1.0f - powf(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f; }
            }
        }
    }
}

// The Unlicense license (do anything you want with this code loll)