//
// Created by henry on 2/11/23.
//

#ifndef HAGAME2_CONFIG_H
#define HAGAME2_CONFIG_H

#include <string>
#include <charconv>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "string.h"
#include <string_view>
#include "../math/interval.h"

namespace hg::utils {
    /*

			The ConfigFile and MultiConfigFile classes provide an interface for saving / loading
			configuration data from files (or other sources) as well as accessing type safe variables.
			The philosophy behind config files (rather than using JSON / xml) is to encourage
			storing the simplest data types possible for any given configurable object.
			A config section consists of "Sections". Each section has a set of key/value pairs.
			A section looks like this:
			[SectionName]
			key1=1.000000,2.000000,3.000000
			key2={1,2}{2,3}
			key3=Hello World
			std::vectors and std::vector of std::vectors are the only aggregate data types supported at this time.
			A Multi Config consists of several "Pages" of configuration.
			A page looks like this:
			~Page 1~
			[SectionA]
			key1=Hello
			[SectionB]
			key1=World

		*/

#define GET_CONFIG(name, type) name = config.get<type>(section, #name);
#define SET_CONFIG(name, type) config.set<type>(section, #name, name);

    using ConfigSection = std::unordered_map<std::string, std::string>;
    using ConfigData = std::unordered_map<std::string, ConfigSection>;
    using MultiConfigData = std::unordered_map<std::string, ConfigData>;


    template<typename T>
    inline T strToT(std::string str) {
        throw std::runtime_error("Unimplented parse type");
    }

    template<>
    inline double strToT<double>(std::string str) {
        return std::atof(str.c_str());
    }

    template<>
    inline float strToT<float>(std::string str) {
        return std::atof(str.c_str());
    }

    template<>
    inline int strToT<int>(std::string str) {
        return std::atoi(str.c_str());
    }

    template<>
    inline bool strToT<bool>(std::string str) {
        return (bool) std::atoi(str.c_str());
    }


    class Config {
    public:

        Config() {}
        Config(ConfigData data):
            m_data(data) {}

        static Config Parse(std::vector<std::string> lines);

        bool hasSection(std::string section);
        bool has(std::string section, std::string key);

        std::vector<std::string> sections();
        std::vector<std::string> keys(std::string section);
        void addSection(std::string section);

        void concat(Config config);

        template <typename T>
        void set(std::string section, std::string key, T value) {
            m_data[section].insert(std::make_pair(key, std::to_string(value)));
        }

        std::string getRaw(std::string section, std::string key) {
            return m_data[section][key];
        }

        void setRaw(std::string section, std::string key, std::string value) {
            m_data[section].insert(std::make_pair(key, value));
        }

        template <typename T>
        T get(std::string section, std::string key) {
            return strToT<T>(m_data[section][key]);
        }

        template <typename T, size_t N>
        void setArray(std::string section, std::string key, T arr[N]) {
            m_data[section].insert(std::make_pair(key, arrToString<T, N>(arr)));
        }

        template <typename T>
        void setInterval(std::string section, std::string key, hg::math::Interval<T> interval) {
            std::array<T, 2> intervalArr = {interval.lower, interval.upper};
            setArray<T, 2>(section, key, intervalArr.data());
        }

        template <typename T>
        hg::math::Interval<T> getInterval(std::string section, std::string key) {
            auto intervalArr = getArray<T, 2>(section, key);
            return hg::math::Interval<T>(intervalArr[0], intervalArr[1]);
        }

        template <typename T, size_t N>
        std::array<T, N> getArray(std::string section, std::string key) {
            std::array<T, N> arr;
            auto parts = s_split(m_data[section][key], ',');
            for (int i = 0; i < parts.size(); i++) {
                arr[i] = strToT<T>(parts[i]);
            }
            return arr;
        }

        template <typename T, size_t N>
        void getArray(std::string section, std::string key, T arr[N]) {
            auto parts = s_split(m_data[section][key], ',');
            for (int i = 0; i < parts.size(); i++) {
                arr[i] = strToT<T>(parts[i]);
            }
        }

        template <typename T, size_t N>
        void setListOfArrays(std::string section, std::string key, std::vector<std::array<T, N>> list) {
            std::string raw = "";
            for (const auto arr : list) {
                raw += "{" + arrToString(arr) + "}";
            }
            m_data[section].insert(std::make_pair(key, raw));
        }

        template <typename T, size_t N>
        std::vector<std::array<T, N>> getListOfArrays(std::string section, std::string key) {
            std::vector<std::array<T, N>> out;
            auto chunks = s_partition(m_data[section][key], '{', '}');

            for (const auto& chunk : chunks) {
                auto parts = s_split(chunk, ',');
                auto arr = std::array<T, N>();
                for (int i = 0; i < parts.size(); i++) {
                    arr[i] = strToT<T>(parts[i]);
                }
                out.push_back(arr);
            }

            return out;
        }

        std::string toString() const;

    private:

        template <typename T, size_t N>
        std::string arrToString(T arr[N]) {
            std::string raw = "";
            for (int i = 0; i < N; i++) {
                raw += std::to_string(arr[i]);
                if (i < N - 1) {
                    raw += ",";
                }
            }
            return raw;
        }

        /*template <typename T>
        T strToT(std::string_view const str) {
            auto value = T{};
            auto [ptr, err] = std::from_chars(str.data(), str.data() + str.size(), value);
            if (err != std::errc{}) {
                throw std::runtime_error("Failed to convert line to numeric value");
            }
            return value;
        }
         */

        ConfigData m_data;
    };

    class MultiConfig {
    public:
        MultiConfig() {}
        MultiConfig(std::unordered_map<std::string, ConfigData> data) {
            for (const auto& [key, config] : data) {
                m_configs.insert(std::make_pair(key, Config(config)));
            }
        }

        static MultiConfig Parse(std::string file);

        void addPage(std::string name) {
            m_configs.insert(std::make_pair(name, Config()));
        }

        void addPage(std::string name, Config config) {
            m_configs.insert(std::make_pair(name, config));
        }

        Config* getPage(std::string name) {
            return &m_configs[name];
        }

        std::string toString() const;

    private:
        std::unordered_map<std::string, Config> m_configs;
    };

    class Configurable {
    public:
        virtual void save(Config& config) = 0;
        virtual void load(Config config) = 0;
    };

    template <typename T>
    concept IsConfigurable = std::is_base_of<Configurable, T>::value;
}

#endif //HAGAME2_CONFIG_H
