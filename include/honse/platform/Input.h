#pragma once
#include <glm/glm.hpp>
#include "Window.h"

namespace honse {


    class Input
    {
    public:

        enum class Key : uint8_t
        {
            None = 0x00,
            ErrorRollOver = 0x01,
            POSTFail = 0x02,
            ErrorUndefined = 0x03,

            // Letters
            A = 0x04, B, C, D, E, F, G, H, I, J, K, L, M,
            N, O, P, Q, R, S, T, U, V, W, X, Y, Z,

            // Numbers
            One = 0x1E,
            Two,
            Three,
            Four,
            Five,
            Six,
            Seven,
            Eight,
            Nine,
            Zero,

            // Editing
            Enter = 0x28,
            Escape,
            Backspace,
            Tab,
            Space,

            // Symbols
            Minus,
            Equal,
            LeftBrace,
            RightBrace,
            Backslash,
            NonUSHash,
            Semicolon,
            Apostrophe,
            Grave,
            Comma,
            Period,
            Slash,

            // Locks
            CapsLock,

            // Function keys
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,

            PrintScreen,
            ScrollLock,
            Pause,

            Insert,
            Home,
            PageUp,
            Delete,
            End,
            PageDown,

            Right,
            Left,
            Down,
            Up,

            NumLock,

            KPDivide,
            KPMultiply,
            KPSubtract,
            KPAdd,
            KPEnter,
            KP1,
            KP2,
            KP3,
            KP4,
            KP5,
            KP6,
            KP7,
            KP8,
            KP9,
            KP0,
            KPDecimal,

            NonUSBackslash,
            Application,
            Power,
            KPEqual,

            // F13-F24
            F13,
            F14,
            F15,
            F16,
            F17,
            F18,
            F19,
            F20,
            F21,
            F22,
            F23,
            F24,

            Execute,
            Help,
            Menu,
            Select,
            Stop,
            Again,
            Undo,
            Cut,
            Copy,
            Paste,
            Find,
            Mute,
            VolumeUp,
            VolumeDown,

            LockingCapsLock,
            LockingNumLock,
            LockingScrollLock,

            KPComma,
            KPEqualSign,

            International1,
            International2,
            International3,
            International4,
            International5,
            International6,
            International7,
            International8,
            International9,

            LANG1,
            LANG2,
            LANG3,
            LANG4,
            LANG5,
            LANG6,
            LANG7,
            LANG8,
            LANG9,

            AlternateErase,
            SysReq,
            Cancel,
            Clear,
            Prior,
            Return,
            Separator,
            Out,
            Oper,
            ClearAgain,
            CrSel,
            ExSel,

            // HID usages 0xA5-0xDF are reserved

            LeftControl = 0xE0,
            LeftShift,
            LeftAlt,
            LeftMeta,
            RightControl,
            RightShift,
            RightAlt,
            RightMeta
        };

        enum KeyModifiers : uint8_t {
            
            LeftControl  = 1 << 0,
            LeftShift    = 1 << 1,
            LeftAlt      = 1 << 2,
            LeftMeta     = 1 << 3,
            RightControl = 1 << 4,
            RightShift   = 1 << 5,
            RightAlt     = 1 << 6,
            RightMeta    = 1 << 7,
        };

        static bool IsKeyDown(Key key);
        static bool IsMouseButtonDown(int button);

        static glm::vec2 GetMousePosition();

    };


}