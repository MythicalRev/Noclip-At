#include "NoclipData.hpp"
#include <Geode/Geode.hpp>
#include <Geode/binding/GJGameLevel.hpp>
#include <matjson.hpp>
#include <razoom.save_level_data_api/include/SaveLevelDataApi.hpp>

using namespace geode::prelude;

void NoclipData::createNewKeyframe(GJGameLevel* level, float percent, bool toggle) {
    log::debug("Created Keyframe at {}", percent);
    log::debug("Toggle: {}", toggle);
    

    auto savedKeyframes = SaveLevelDataAPI::getSavedValue(
        level,
        "keyframes",
        true,
        true
    ).unwrapOr(matjson::Value());

    if (!savedKeyframes.isArray()) {
        savedKeyframes = matjson::Value(std::vector<matjson::Value>{});
    }

    auto& arr = savedKeyframes.asArray().unwrap();

    auto entry = matjson::makeObject({
        {"percent", percent},
        {"toggle", toggle }
    });

    arr.push_back(entry);

    SaveLevelDataAPI::setSavedValue(
        level,
        "keyframes",
        arr,
        true,
        true
    );
}

void NoclipData::removeKeyframeByIndex(GJGameLevel* level, int index) {
    auto savedKeyframes = SaveLevelDataAPI::getSavedValue(
        level,
        "keyframes",
        true,
        true
    ).unwrapOr(matjson::Value());

    if (!savedKeyframes.isArray()) return;

    auto arr = savedKeyframes.asArray().unwrap();

    if (index < 0 || index >= (int)arr.size()) return;

    arr.erase(arr.begin() + index);

    SaveLevelDataAPI::setSavedValue(
        level,
        "keyframes",
        arr,
        true,
        true
    );
}

void NoclipData::updateValues(GJGameLevel* level, int index, float newPercent, bool newToggle) {
    auto savedKeyframes = SaveLevelDataAPI::getSavedValue(
        level,
        "keyframes",
        true,
        true
    ).unwrapOr(matjson::Value());

    if (!savedKeyframes.isArray()) return;

    auto arr = savedKeyframes.asArray().unwrap();

    if (index < 0 || index >= (int)arr.size()) return;

    arr[index]["percent"] = newPercent;
    arr[index]["toggle"] = newToggle;

    SaveLevelDataAPI::setSavedValue(
        level,
        "keyframes",
        arr,
        true,
        true
    );

    log::debug("Updated toggle to {}", newToggle);
}

std::vector<std::tuple<float, bool>> NoclipData::getNoclipKeyframes(GJGameLevel * level) {
    auto savedKeyframes = SaveLevelDataAPI::getSavedValue(
        level,
        "keyframes",
        true,
        true
    ).unwrapOr(matjson::Value());

    std::vector<std::tuple<float, bool>> keyframes;

    if (!savedKeyframes.isArray()) return keyframes;

    for (auto& obj : savedKeyframes.asArray().unwrap()) {
        float percent = obj["percent"].asDouble().unwrap();
        bool toggle = obj["toggle"].asBool().unwrap();
        keyframes.push_back({percent, toggle});
    }

    return keyframes;
}