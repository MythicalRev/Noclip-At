#include "NoclipHandler.hpp"
#include "../../notif.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void NoclipHandler::changeNoclip(bool changeNoclip) {
    notifapi::info("Noclip Toggled");
    Mod::get()->setSavedValue<bool>("isNoclip", changeNoclip);
}