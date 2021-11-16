#pragma once
#include <vector>
#include <map>
#include "HitObject.h"
#include "GameFile.h"
#include "TimingPoint.h"
#include "Event.h"
#include <string.h>

struct GameFile{
    std::map<std::string, std::string> configGeneral;
    std::map<std::string, std::string> configEditor;
    std::map<std::string, std::string> configMetadata;
    std::map<std::string, std::string> configDifficulty;
    std::map<std::string, std::string> configColours;
    std::vector<HitObject> hitObjects;
    std::vector<TimingPoint> timingPoints;
    std::vector<Event> events;
};