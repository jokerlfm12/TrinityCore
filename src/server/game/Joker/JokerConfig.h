#ifndef JOKER_CONFIG_H
#define JOKER_CONFIG_H


#include "Define.h"
#include <string>
#include <vector>

class TC_COMMON_API JokerConfig
{
    JokerConfig() = default;
    JokerConfig(JokerConfig const&) = delete;
    JokerConfig& operator=(JokerConfig const&) = delete;
    ~JokerConfig() = default;

public:
    /// Method used only for loading main configuration files (authserver.conf and worldserver.conf)
    bool LoadInitial(std::string const& file, std::vector<std::string> args, std::string& error);

    static JokerConfig* instance();

    bool Reload(std::string& error);

    std::string GetStringDefault(std::string const& name, const std::string& def) const;
    bool GetBoolDefault(std::string const& name, bool def) const;
    int GetIntDefault(std::string const& name, int def) const;
    float GetFloatDefault(std::string const& name, float def) const;

    std::string const& GetFilename();
    std::vector<std::string> const& GetArguments() const;
    std::vector<std::string> GetKeysByString(std::string const& name);

private:
    template<class T>
    T GetValueDefault(std::string const& name, T def) const;

public:

    bool StartJokerSystem();

    uint32 Enable;

    float LevelUpEXPRate_Expansion_0;
    float LevelUpEXPRate_Expansion_1;
    float LevelUpEXPRate_Expansion_2;
    float LevelUpEXPRate_Expansion_3;

    std::unordered_set<uint32> RandomMoveCreatureEntrySet;

    uint32 NewPlayerExpansion;

    std::vector<std::string> SplitString(std::string srcStr, std::string delimStr, bool repeatedCharIgnored);
    std::string TrimString(std::string srcStr);

private:
    std::string RandomMoveCreatureEntries;

};

#define sJokerConfig JokerConfig::instance()

#endif
