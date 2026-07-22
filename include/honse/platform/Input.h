#pragma once
#include <glm/glm.hpp>

namespace hs {

    enum Key {
        None = 0x00,
        Overflow = 0x01,

        // MOD KEYS

        LeftControl = 0x01,
        LeftShift = 0x02,
        LeftAlt = 0x04,
        LeftMeta = 0x08,
        
        RightControl = 0x10,
        RightShift = 0x20,
        RightAlt = 0x40,
        RightMeta = 0x80,

        // Letters

        A = 0x04,
        B = 0x05,
        C = 0x06,
        D = 0x07,
        E = 0x08,
        F = 0x09,
        G = 0x0a,
        H = 0x0b,
        I = 0x0c,
        J = 0x0d,
        K = 0x0e,
        L = 0x0f,
        M = 0x10,
        N = 0x11,
        O = 0x12,
        P = 0x13,
        Q = 0x14,
        R = 0x15,
        S = 0x16,
        T = 0x17,
        U = 0x18,
        V = 0x19,
        W = 0x1a,
        X = 0x1b,
        Y = 0x1c,
        Z = 0x1d,

        // Numbers

        One = 0x1e,
        Two = 0x1f,
        Three = 0x20,
        Four = 0x21,
        Five = 0x22,
        Six = 0x23,
        Seven = 0x24,
        Eight = 0x25,
        Nine = 0x26,
        Zero = 0x27,

        // 

        Enter = 0x28,
        Escape = 0x29,
        Backspace = 0x2a,
        Tab = 0x2b,
        Space = 0x2c,
        Minus = 0x2d,
        Equal = 0x2e,
        LeftBrace = 0x2f,
        RightBrace = 0x30,
        Backslash,

    };

    class Input
    {
    public:
        static bool IsKeyDown(Key key);
        static bool IsMouseButtonDown(int button);

        static glm::vec2 GetMousePosition();

    };


}