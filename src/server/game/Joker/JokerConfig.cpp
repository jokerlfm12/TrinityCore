#include "JokerConfig.h"
#include "Log.h"
#include "Util.h"
#include <boost/property_tree/ini_parser.hpp>
#include <algorithm>
#include <memory>
#include <mutex>

#include <boost/filesystem/operations.hpp>

#ifndef JOKER_CONFIG_FILE_NAME
# define JOKER_CONFIG_FILE_NAME  "joker.conf"
#endif

namespace bpt = boost::property_tree;
namespace fs = boost::filesystem;

namespace
{
    std::string _filename;
    std::vector<std::string> _args;
    bpt::ptree _config;
    std::mutex _configLock;
}

bool JokerConfig::LoadInitial(std::string const& file, std::vector<std::string> args,
    std::string& error)
{
    std::lock_guard<std::mutex> lock(_configLock);

    _filename = file;
    _args = args;

    try
    {
        bpt::ptree fullTree;
        bpt::ini_parser::read_ini(file, fullTree);

        if (fullTree.empty())
        {
            error = "empty file (" + file + ")";
            return false;
        }

        // Since we're using only one section per config file, we skip the section and have direct property access
        _config = fullTree.begin()->second;
    }
    catch (bpt::ini_parser::ini_parser_error const& e)
    {
        if (e.line() == 0)
            error = e.message() + " (" + e.filename() + ")";
        else
            error = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
        return false;
    }

    return true;
}

JokerConfig* JokerConfig::instance()
{
    static JokerConfig instance;
    return &instance;
}

bool JokerConfig::Reload(std::string& error)
{
    return LoadInitial(_filename, std::move(_args), error);
}

template<class T>
T JokerConfig::GetValueDefault(std::string const& name, T def) const
{
    try
    {
        return _config.get<T>(bpt::ptree::path_type(name, '/'));
    }
    catch (bpt::ptree_bad_path const&)
    {
        TC_LOG_WARN("server.loading", "Missing name %s in config file %s, add \"%s = %s\" to this file",
            name.c_str(), _filename.c_str(), name.c_str(), std::to_string(def).c_str());
    }
    catch (bpt::ptree_bad_data const&)
    {
        TC_LOG_ERROR("server.loading", "Bad value defined for name %s in config file %s, going to use %s instead",
            name.c_str(), _filename.c_str(), std::to_string(def).c_str());
    }

    return def;
}

template<>
std::string JokerConfig::GetValueDefault<std::string>(std::string const& name, std::string def) const
{
    try
    {
        return _config.get<std::string>(bpt::ptree::path_type(name, '/'));
    }
    catch (bpt::ptree_bad_path const&)
    {
        TC_LOG_WARN("server.loading", "Missing name %s in config file %s, add \"%s = %s\" to this file",
            name.c_str(), _filename.c_str(), name.c_str(), def.c_str());
    }
    catch (bpt::ptree_bad_data const&)
    {
        TC_LOG_ERROR("server.loading", "Bad value defined for name %s in config file %s, going to use %s instead",
            name.c_str(), _filename.c_str(), def.c_str());
    }

    return def;
}

std::string JokerConfig::GetStringDefault(std::string const& name, const std::string& def) const
{
    std::string val = GetValueDefault(name, def);
    val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
    return val;
}

bool JokerConfig::GetBoolDefault(std::string const& name, bool def) const
{
    std::string val = GetValueDefault(name, std::string(def ? "1" : "0"));
    val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
    return StringToBool(val);
}

int JokerConfig::GetIntDefault(std::string const& name, int def) const
{
    return GetValueDefault(name, def);
}

float JokerConfig::GetFloatDefault(std::string const& name, float def) const
{
    return GetValueDefault(name, def);
}

std::string const& JokerConfig::GetFilename()
{
    std::lock_guard<std::mutex> lock(_configLock);
    return _filename;
}

std::vector<std::string> const& JokerConfig::GetArguments() const
{
    return _args;
}

std::vector<std::string> JokerConfig::GetKeysByString(std::string const& name)
{
    std::lock_guard<std::mutex> lock(_configLock);

    std::vector<std::string> keys;

    for (bpt::ptree::value_type const& child : _config)
        if (child.first.compare(0, name.length(), name) == 0)
            keys.push_back(child.first);

    return keys;
}

bool JokerConfig::StartJokerSystem()
{
    auto configFile = fs::absolute(JOKER_CONFIG_FILE_NAME);
    std::string configError;
    LoadInitial(configFile.generic_string(), std::vector<std::string>(), configError);
    Enable = GetIntDefault("Enable", 0);

    LevelUpEXPRate_Expansion_0 = GetFloatDefault("LevelUpEXPRate_Expansion_0", 1.0f);
    LevelUpEXPRate_Expansion_1 = GetFloatDefault("LevelUpEXPRate_Expansion_1", 1.0f);
    LevelUpEXPRate_Expansion_2 = GetFloatDefault("LevelUpEXPRate_Expansion_2", 1.0f);
    LevelUpEXPRate_Expansion_2 = GetFloatDefault("LevelUpEXPRate_Expansion_3", 1.0f);

    RandomMoveCreatureEntries = GetStringDefault("RandomMoveCreatureEntries", "");
    std::vector<std::string> entryVector = SplitString(RandomMoveCreatureEntries, ",", true);
    RandomMoveCreatureEntrySet.clear();
    for (std::vector<std::string>::iterator entryIT = entryVector.begin(); entryIT != entryVector.end(); entryIT++)
    {
        std::string eachEntryStr = *entryIT;
        uint32 eachEntry = std::atoi(eachEntryStr.c_str());
        RandomMoveCreatureEntrySet.insert(eachEntry);
    }

    if (Enable == 0)
    {
        return false;
    }

    return true;
}

std::vector<std::string> JokerConfig::SplitString(std::string srcStr, std::string delimStr, bool repeatedCharIgnored)
{
    std::vector<std::string> resultStringVector;
    std::replace_if(srcStr.begin(), srcStr.end(), [&](const char& c) {if (delimStr.find(c) != std::string::npos) { return true; } else { return false; }}/*pred*/, delimStr.at(0));
    size_t pos = srcStr.find(delimStr.at(0));
    std::string addedString = "";
    while (pos != std::string::npos) {
        addedString = srcStr.substr(0, pos);
        if (!addedString.empty() || !repeatedCharIgnored) {
            resultStringVector.push_back(addedString);
        }
        srcStr.erase(srcStr.begin(), srcStr.begin() + pos + 1);
        pos = srcStr.find(delimStr.at(0));
    }
    addedString = srcStr;
    if (!addedString.empty() || !repeatedCharIgnored) {
        resultStringVector.push_back(addedString);
    }
    return resultStringVector;
}

std::string JokerConfig::TrimString(std::string srcStr)
{
    std::string result = srcStr;
    if (!result.empty())
    {
        result.erase(0, result.find_first_not_of(" "));
        result.erase(result.find_last_not_of(" ") + 1);
    }

    return result;
}
