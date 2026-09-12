#pragma once

// clang-format off
enum MSAASamples {
    MSAA_DISABLE = 0,       // Disables MSAA.
    MSAA_2x = 2,    // Uses 2 MSAA samples. Little improvement, but inexpensive. 
    MSAA_4x = 4,    // Uses 4 MSAA samples. Ideal quality/performance balance for expensive rendering purposes.
    MSAA_8x = 8,    // Uses 8 MSAA samples. Recommended value. High quality, but slightly more expensive.
    MSAA_16x = 16   // Uses 16 MSAA samples.
};

class WindowPreferences {

    public:
        void SetTransparentFramebuffer(bool value);
        void SetDecorated(bool value);
        void SetVSync(bool value);

        [[nodiscard]] bool GetTransparentFramebuffer() const;
        [[nodiscard]] bool GetDecorated() const;
        [[nodiscard]] bool GetVSync() const;

    private:
        bool transparentFramebuffer = false;    // Makes the window allow transparency (dependent on clear color). Currently MSAA is unsupported for this.
        bool decorated = true;                  // Includes the top bar for the window.
        bool vsync = true;                      // Enables VSync. 
};

class PhysicsPreferences {

};

class VideoPreferences {
    public:

        void SetMSAASamples(MSAASamples value);

        [[nodiscard]] MSAASamples GetMSAASamples() const;

    private:
        MSAASamples msaaSamples = MSAA_DISABLE; // MSAA sample count. If set to 0x, MSAA is disabled (fastest). Maximum recommended value is 8x (slowest).
};

/**
 * @brief Project preferences wrapper.
 *
 * Provides real-time modifiable preferences for the application project, including physics, video 
 * and window preferences. 
 */
struct ProjectPreferences {
    VideoPreferences videoPreferences;
    WindowPreferences windowPreferences;
};