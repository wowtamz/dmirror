#include <fstream>
#include <unordered_map>
#include <string>

#include "config.h"

Config::Config()
{
    data = {};
}

void Config::save(const std::string& path)
{
    std::ofstream file(path);

    for (const auto& [key, value] : data) {
        file << key << "=" << value << '\n';
    }
}

bool Config::read(const std::string& path)
{
    data.clear();

    std::ifstream file(path);

    if (!file) {
        return false;
    }

    std::string line;

    while (std::getline(file, line))
    {
        auto pos = line.find('=');

        if (pos == std::string::npos)
            continue;
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos+1);

        data[key] = value;
    }

    return true;
}

void Config::add(const std::string& key, const std::string& value)
{
    data[key] = value;
}

void Config::update(const std::string& key, const std::string& value)
{
    if (data.find(key) != data.end()) {
        data[key] = value;
    }
}

void Config::remove(const std::string& key)
{
    data.erase(key);
}

std::optional<std::string> Config::get(const std::string& key)
{
    auto it = data.find(key);

    if (it == data.end())
        return std::nullopt;

    return it->second;
}
