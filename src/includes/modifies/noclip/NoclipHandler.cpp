#include "NoclipHandler.hpp"
#include "Geode/ui/Notification.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void NoclipHandler::changeNoclip(bool changeNoclip) {
    geode::Notification::create("Noclip Keyframe Reached", NotificationIcon::Info)->show();
    Mod::get()->setSavedValue<bool>("isNoclip", changeNoclip);
}