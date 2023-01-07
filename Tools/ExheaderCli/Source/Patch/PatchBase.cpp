#include "Patch/PatchBase.h"
#include "Types.h"
#include <fstream>
#include <iterator>
#include <sstream>

static std::vector<std::string> splitWhitespace(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    for (std::string s; iss >> s;)
        result.push_back(s);
    return result;
}

std::vector<const PatchBase*> collectPatchesFromMap(const std::vector<u8>& data, const std::string& mapFile)
{
    std::ifstream stream(mapFile);
    std::string line;

    std::getline(stream, line); // column name thingies

    std::vector<const PatchBase*> patches;

    bool isAtHooks = false;
    while (std::getline(stream, line)) {
        if (line.rfind(".text") != std::string::npos)
            break;

        if (isAtHooks && line.find("CMakeFiles") == std::string::npos) {
            std::vector<std::string> split = splitWhitespace(line);

            u32 address = std::stoi(split[0], 0, 16);

            patches.push_back(reinterpret_cast<const PatchBase*>(&data.at(address - sCodeStart)));

        } else if (line.rfind(".hk.hooks") != std::string::npos)
            isAtHooks = true;
    }

    return patches;
}
