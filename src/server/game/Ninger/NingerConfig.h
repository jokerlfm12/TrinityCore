#ifndef NINGER_CONFIG_H
#define NINGER_CONFIG_H

#include "Define.h"
#include <string>
#include <vector>

class TC_COMMON_API NingerConfig
{
    NingerConfig()
    {
        Enable = 0;
        AccountNamePrefix = "";
        DPSDelay = 0;
        OnlineCheckDelay = 0;
        OfflineCheckDelay = 0;
        NingerCountEachLevel = 0;
    }
    NingerConfig(NingerConfig const&) = delete;
    NingerConfig& operator=(NingerConfig const&) = delete;
    ~NingerConfig() = default;

public:
    /// Method used only for loading main configuration files (authserver.conf and worldserver.conf)
    bool LoadInitial(std::string const& file, std::vector<std::string> args, std::string& error);

    static NingerConfig* instance();

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
    bool StartNinger();
    uint32 Enable;
    std::string AccountNamePrefix;
    uint32 DPSDelay;
    uint32 OnlineCheckDelay;
    uint32 OfflineCheckDelay;
    uint32 NingerCountEachLevel;    
};

#define sNingerConfig NingerConfig::instance()

#endif
