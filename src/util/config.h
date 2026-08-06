// config.h
#pragma once

class Config
{
    public:
        Config();
        void save(std::string& path);
        void read(std::string& path);
        void add(std::string& key, std::string& value);
        void update(std::string& key, std::string& value);
        void remove(std::string& key);
        std::string get(std::string& key);
    
    private:
        std::unordered_map<std::string, std::string> data;

};