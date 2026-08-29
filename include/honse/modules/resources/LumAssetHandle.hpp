#pragma once
#include <lumpack.h>
#include <string_view>
#include <string>

namespace honse {

    
    class LumAssetHandle {
    public:

        LumAssetHandle() = default;

        explicit LumAssetHandle(LumAsset asset)
            : m_Asset(asset) {}

        ~LumAssetHandle() {
            lum_free(&m_Asset);
        }

        LumAssetHandle(const LumAssetHandle&) = delete;
        LumAssetHandle& operator=(const LumAssetHandle&) = delete;

        LumAssetHandle(LumAssetHandle&& other) noexcept
            : m_Asset(other.m_Asset) {
            other.m_Asset = {nullptr, nullptr};
        }

        LumAssetHandle& operator=(LumAssetHandle&& other) noexcept {
            if (this != &other) {
                lum_free(&m_Asset);
                m_Asset = other.m_Asset;
                other.m_Asset = {nullptr, nullptr};
            }

            return *this;
        }

        operator bool() {
            return lum_is_valid(&m_Asset);
        }

        const char* path() const {
            return m_Asset.path;
        }

        FILE* data() const {
            return m_Asset.data;
        }

    private:
        LumAsset m_Asset;
    };
    

}