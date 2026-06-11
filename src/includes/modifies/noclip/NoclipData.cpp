#include "NoclipData.hpp"
#include "Geode/utils/general.hpp"
#include <Geode/Enums.hpp>
#include <Geode/Geode.hpp>

#include <Geode/binding/GJGameLevel.hpp>
#include <cvolton.level-id-api/include/EditorIDs.hpp>

using namespace geode::prelude;

void NoclipData::createNewKeyframe(GJGameLevel* level, float percent, bool toggle) {
    log::debug("Created Keyframe at {}", percent);
    log::debug("Toggle: {}", toggle);

    auto type = level->m_levelType;
    std::string id;

    if (type == GJLevelType::Editor) {
        id = geode::utils::numToString(EditorIDs::getID(level));
    } else {
        id = geode::utils::numToString(level->m_levelID.value());
    }

    auto savedKeyframes = getMod()->getSavedValue<matjson::Value>(geode::utils::numToString(id));

    if (!savedKeyframes.isArray()) {
        savedKeyframes = matjson::Value(std::vector<matjson::Value>{});
    }

    std::vector<matjson::Value> empty;

    auto arr = savedKeyframes.asArray().unwrapOr(empty);

    auto entry = matjson::makeObject({
        {"percent", percent},
        {"toggle", toggle }
    });

    arr.push_back(entry);

    getMod()->setSavedValue(id, arr);
}

void NoclipData::removeKeyframeByIndex(GJGameLevel* level, int index) {
    auto type = level->m_levelType;
    std::string id;

    if (type == GJLevelType::Editor) {
        id = geode::utils::numToString(EditorIDs::getID(level));
    } else {
        id = geode::utils::numToString(level->m_levelID.value());
    }

    auto savedKeyframes = getMod()->getSavedValue<matjson::Value>(id);

    if (!savedKeyframes.isArray()) return;

    std::vector<matjson::Value> empty;

    auto arr = savedKeyframes.asArray().unwrapOr(empty);

    if (index < 0 || index >= (int)arr.size()) return;

    arr.erase(arr.begin() + index);

    getMod()->setSavedValue(id, matjson::Value(arr));
}

void NoclipData::updateValues(GJGameLevel* level, int index, float newPercent, bool newToggle) {
    auto type = level->m_levelType;
    std::string id;

    if (type == GJLevelType::Editor) {
        id = geode::utils::numToString(EditorIDs::getID(level));
    } else {
        id = geode::utils::numToString(level->m_levelID.value());
    }

    auto savedKeyframes = getMod()->getSavedValue<matjson::Value>(id);

    if (!savedKeyframes.isArray()) return;

    std::vector<matjson::Value> empty;

    auto arr = savedKeyframes.asArray().unwrapOr(empty);

    if (index < 0 || index >= (int)arr.size()) return;

    arr[index]["percent"] = newPercent;
    arr[index]["toggle"] = newToggle;

    getMod()->setSavedValue(id, matjson::Value(arr));

    log::debug("Updated toggle to {}", newToggle);
}

std::vector<std::tuple<float, bool>> NoclipData::getNoclipKeyframes(GJGameLevel* level) {
    auto type = level->m_levelType;
    std::string id;

    if (type == GJLevelType::Editor) {
        id = geode::utils::numToString(EditorIDs::getID(level));
    } else {
        id = geode::utils::numToString(level->m_levelID.value());
    }

    auto savedKeyframes = getMod()->getSavedValue<matjson::Value>(id);
    std::vector<std::tuple<float, bool>> keyframes;

    if (!savedKeyframes.isArray()) return keyframes;

    std::vector<matjson::Value> empty;

    for (auto& obj : savedKeyframes.asArray().unwrapOr(empty)) {
        float percent = obj["percent"].asDouble().unwrapOr(0.f);
        bool toggle = obj["toggle"].asBool().unwrapOr(false);
        keyframes.push_back({percent, toggle});
    }

    return keyframes;
}