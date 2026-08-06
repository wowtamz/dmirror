// config.h
#pragma once

class Config
{
    public:
        Config();
        void save(const std::string& path);
        bool read(const std::string& path);
        void add(const std::string& key, const std::string& value);
        void update(const std::string& key, const std::string& value);
        void remove(const std::string& key);
        std::string get(const std::string& key);
    
    private:
        std::unordered_map<std::string, std::string> data;

};