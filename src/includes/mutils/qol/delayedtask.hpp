/*
    mUtils (created by miskaa)
    mUtils::delayedTask - runs a function after a delay

    miskaas geode utils

    how to use:
    mutils::DelayedTask::wait(1.0f, []() { // 1.0f here is the timeout/delay in seconds
        // do something
    });
    if you would like to use it with a string, do something like this:
    
    std::string string = "string"
    mutils::DelayedTask::wait(1.0f, [string]() {
        // do something with string
    });
    --------------
    you also need to include mutils/index.hpp to use this

    note: this is a one-time use thing, if you want to repeat it, you'll have to call it again, not really sure how to make it simple
*/

#include <Geode/Geode.hpp>
using namespace geode::prelude;

namespace mutils {
    class DelayedTask : public CCObject {
        public:
        std::function<void()> m_callback;

        static void wait(float d, std::function<void()> callback) {
            auto* obj = new DelayedTask();
            obj->m_callback = callback; // callback to run
            obj->autorelease();         // cocos plz manage my memory im lazy
            obj->retain();              // keep it alive

            CCScheduler::get()->scheduleSelector(
                schedule_selector(DelayedTask::onTrigger),
                obj, 0.0f, 0, d, false
            );
        }

        void onTrigger(float dt) {
            CCScheduler::get()->unscheduleSelector(
                schedule_selector(
                    DelayedTask::onTrigger
                ), this
            );
            
            if (m_callback) {
                m_callback();
            }
            
            release();
        }
    };
}

// The Unlicense license (do anything you want with this code loll)