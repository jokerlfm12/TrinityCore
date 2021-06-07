#ifndef MING_CONFIG_H
#define MING_CONFIG_H

#include "Define.h"
#include <string>
#include <vector>

class TC_COMMON_API MingConfig
{
    MingConfig()
    {
        Enable = 0;
        Expansion = 0;
    }
    MingConfig(MingConfig const&) = delete;
    MingConfig& operator=(MingConfig const&) = delete;
    ~MingConfig() = default;

public:
    /// Method used only for loading main configuration files (authserver.conf and worldserver.conf)
    bool LoadInitial(std::string const& file, std::vector<std::string> args, std::string& error);

    static MingConfig* instance();

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
    bool StartMing();
    uint32 Enable;    
    uint32 Expansion;
};

#define sMingConfig MingConfig::instance()

#endif
