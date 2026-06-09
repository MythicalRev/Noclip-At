/*
    mUtils (created by miskaa)
    
    will be used by ../ui.hpp

    miskaas geode utils
*/

namespace mutils {
    namespace ui {
        namespace animation {
            namespace quad {
                static float in(float t) { return t * t; }
                static float out(float t) { return 1.0f - (1.0f - t) * (1.0f - t); }
                static float inOut(float t) { return t < 0.5f ? 2.0f * t * t : 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f; }
            }
        }
    }
}

// The Unlicense license (do anything you want with this code loll)