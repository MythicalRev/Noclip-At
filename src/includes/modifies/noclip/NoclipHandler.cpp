#include "NoclipHandler.hpp"
#include "Geode/ui/Notification.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void NoclipHandler::changeNoclip(bool changeNoclip) {
    Mod::get()->setSavedValue<bool>("isNoclip", changeNoclip);
}