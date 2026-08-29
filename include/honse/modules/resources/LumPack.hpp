#pragma once
#include "LumAssetHandle.hpp"
#include <string_view>
#include <string>
#include <vector>
#include <unordered_map>

namespace honse {

    class LumPack {
    public:
        explicit LumPack(std::string path)
            : m_Path(std::move(path)) {}

        LumAssetHandle Extract(std::string id) 
        {
            return LumAssetHandle{
                lum_extract_asset(m_Path.c_str(), lum_hash(id.c_str()))
            };
        }

    private:
        std::string m_Path;
    };

    class LumPacks {
    public:
        static void Mount(std::string path)
        {
            m_Packs.emplace_back(std::move(path));
        }

        static const char* Path(std::string name)
        {
            uint64_t id = lum_hash(name.c_str());

            auto it = m_Extracted.find(id);

            if (it != m_Extracted.end())
                return it->second.path();

            for (auto pack = m_Packs.rbegin(); pack != m_Packs.rend(); ++pack) {
                auto asset = pack->Extract(name);

                if (asset) {
                    auto [inserted, _] =
                        m_Extracted.emplace(id, std::move(asset));

                    return inserted->second.path();
                }
            }

            return nullptr;
        }

    private:
        inline static std::vector<LumPack> m_Packs;
        inline static std::unordered_map<uint64_t, LumAssetHandle> m_Extracted;
    };

}