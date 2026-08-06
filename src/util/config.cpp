#include <fstream>
#include <unordered_map>
#include <string>

#include "config.h"

Config::Config()
{
    data = {};
}

void Config::save(std::string& path)
{
    std::ofstream file(path);

    for (const auto& [key, value] : data) {
        file << key << "=" << value << '\n';
    }
}

void Config::read(std::string& path)
{
    data.clear();

    std::ifstream file(path);

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
}

void Config::add(std::string& key, std::string& value)
{
    data[key] = value;
}

void Config::update(std::string& key, std::string& value)
{
    if (data.find(key) != data.end()) {
        data[key] = value;
    }
}

void Config::remove(std::string& key)
{
    data.erase(key);
}

std::string Config::get(std::string& key)
{
    return data.find(key) == data.end() ? nullptr : data[key];
}
