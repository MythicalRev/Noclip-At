#include "NoclipData.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void NoclipData::createNewKeyframe(std::string levelName, float percent, bool toggle) {
    log::debug("Created Keyframe at {}", percent);
    log::debug("Toggle: {}", toggle);

    auto savedKeyframes = getMod()->getSavedValue<matjson::Value>(levelName);

    if (!savedKeyframes.isArray()) {
        savedKeyframes = matjson::Value(std::vector<matjson::Value>{});
    }

    auto& arr = savedKeyframes.asArray().unwrap();

    auto entry = matjson::makeObject({
        {"percent", percent},
        {"toggle", toggle }
    });

    arr.push_back(entry);

    getMod()->setSavedValue(levelName, arr);
}

void NoclipData::removeKeyframeByIndex(std::string levelName, int index) {
    auto savedKeyframes = getMod()->getSavedValue<matjson::Value>(levelName);

    if (!savedKeyframes.isArray()) return;

    auto arr = savedKeyframes.asArray().unwrap();

    if (index < 0 || index >= (int)arr.size()) return;

    arr.erase(arr.begin() + index);

    getMod()->setSavedValue(levelName, matjson::Value(arr));
}

void NoclipData::updateValues(std::string levelName, int index, float newPercent, bool newToggle) {
    auto savedKeyframes = getMod()->getSavedValue<matjson::Value>(levelName);

    if (!savedKeyframes.isArray()) return;

    auto arr = savedKeyframes.asArray().unwrap();

    if (index < 0 || index >= (int)arr.size()) return;

    arr[index]["percent"] = newPercent;
    arr[index]["toggle"] = newToggle;

    getMod()->setSavedValue(levelName, matjson::Value(arr));
}

std::vector<std::tuple<float, bool>> NoclipData::getNoclipKeyframes(std::string levelName) {
    auto savedKeyframes = getMod()->getSavedValue<matjson::Value>(levelName);
    std::vector<std::tuple<float, bool>> keyframes;

    if (!savedKeyframes.isArray()) return keyframes;

    for (auto& obj : savedKeyframes.asArray().unwrap()) {
        float percent = obj["percent"].asDouble().unwrap();
        bool toggle = obj["toggle"].asBool().unwrap();
        keyframes.push_back({percent, toggle});
    }

    return keyframes;
}