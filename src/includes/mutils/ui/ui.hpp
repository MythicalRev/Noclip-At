/*
    mUtils (created by miskaa)
    mUtils::ui::animation - easing functions for animations
    
    miskaas geode utils

    how to use:
    examples:

    mutils::ui::animation::ease::linearIn(0.5f);
    mutils::ui::animation::ease::sineInOut(0.5f);

    how its meant to be used:
    idk im lazy for now
*/

#include <Geode/Geode.hpp>

// animation
#include "animation/helper.hpp"

using namespace geode::prelude;

namespace mutils {
    namespace ui {
        namespace animation {
            class ease {
                public:
                    // LINEAR
                    static float linearIn(float t) { return linear::in(t); }
                    static float linearOut(float t) { return linear::out(t); }
                    static float linearInOut(float t) { return linear::inOut(t); }

                    // SINE
                    static float sineIn(float t) { return sine::in(t); }
                    static float sineOut(float t) { return sine::out(t); }
                    static float sineInOut(float t) { return sine::inOut(t); }

                    // QUAD
                    static float quadIn(float t) { return quad::in(t); }
                    static float quadOut(float t) { return quad::out(t); }
                    static float quadInOut(float t) { return quad::inOut(t); }

                    // CUBIC
                    static float cubicIn(float t) { return cubic::in(t); }
                    static float cubicOut(float t) { return cubic::out(t); }
                    static float cubicInOut(float t) { return cubic::inOut(t); }

                    // QUART
                    static float quartIn(float t) { return quart::in(t); }
                    static float quartOut(float t) { return quart::out(t); }
                    static float quartInOut(float t) { return quart::inOut(t); }

                    // QUINT
                    static float quintIn(float t) { return quint::in(t); }
                    static float quintOut(float t) { return quint::out(t); }
                    static float quintInOut(float t) { return quint::inOut(t); }

                    // EXPONENTIAL
                    static float expoIn(float t) { return expontential::in(t); }
                    static float expoOut(float t) { return expontential::out(t); }
                    static float expoInOut(float t) { return expontential::inOut(t); }

                    // CIRCULAR
                    static float circIn(float t) { return circular::in(t); }
                    static float circOut(float t) { return circular::out(t); }
                    static float circInOut(float t) { return circular::inOut(t); }

                    // BACK
                    static float backIn(float t) { return back::in(t); }
                    static float backOut(float t) { return back::out(t); }
                    static float backInOut(float t) { return back::inOut(t); }

                    // BOUNCE
                    static float bounceIn(float t) { return bounce::in(t); }
                    static float bounceOut(float t) { return bounce::out(t); }
                    static float bounceInOut(float t) { return bounce::inOut(t); }

                    // ELASTIC
                    static float elasticIn(float t) { return elastic::in(t); }
                    static float elasticOut(float t) { return elastic::out(t); }
                    static float elasticInOut(float t) { return elastic::inOut(t); }
            };
        }
    }
}

// The Unlicense license (do anything you want with this code loll)