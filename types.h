#pragma once

#include <cstdint>
#include <vector>
#include <set>
#include <unordered_map>

using bytes_t = std::vector<unsigned char>;
using address_t = std::uint64_t;
using height_t = std::uint64_t;

template<typename type_of_key, typename type_of_value>
class tracking_unordered_map : public std::unordered_map<type_of_key, type_of_value>
{
public:
    type_of_value& operator[](const type_of_key& key)
    {
        dirtyKeys_.insert(key);
        return std::unordered_map<type_of_key, type_of_value>::operator[](key);
    }

    size_t erase(const type_of_key& key)
    {
        dirtyKeys_.erase(key);
        return std::unordered_map<type_of_key, type_of_value>::erase(key);
    }

    std::set<type_of_key> dirtyKeys() const
    {
        return dirtyKeys_;
    }

    void resetDirtyKeys()
    {
        dirtyKeys_.clear();
    }

private:
    std::set<type_of_key> dirtyKeys_;
};
