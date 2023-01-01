#include <Prey/CryCore/StdAfx.h>

struct Input {
    enum class Type {
        KEYBOARD,
        MOUSE,
        GAMEPAD
    };
    virtual EKeyId getKeyId() {
        return m_keyId;
    }
    virtual void parseString(std::string & inputString) = 0;
    virtual Type getType() = 0;
    EKeyId m_keyId;
    Type m_type;
};

struct KeyboardInput : public Input {
    KeyboardInput() {
        m_type = Type::KEYBOARD;
    }
    KeyboardInput(std::string & inputString);
    void parseString(std::string & inputString) override;
    Type getType() override;
};
struct MouseInput : public Input {
    MouseInput() {
        m_type = Type::MOUSE;
    }
    MouseInput(std::string & inputString);
    void parseString(std::string & inputString) override;
    Type getType() override;

    float m_xPos, m_yPos;
    bool m_Absolute;
    bool m_leftButton,
            m_rightButton,
            m_middleButton,
            m_xButton1,
            m_xButton2;
};

// FIXME:UNIMPLEMENTED
struct GamepadInput : public Input {
    GamepadInput() {
        m_type = Type::GAMEPAD;
    }
    GamepadInput(std::string &);
    EKeyId getKeyId() override;
    void parseString(std::string &) override;
    Type getType() override;
};

struct FrameInputs {
    std::vector<KeyboardInput> m_keyboardInputs;
    std::vector<MouseInput> m_mouseInputs;
    //TODO: add gamepad inputs
    uint64_t m_frameNumber;
    bool m_bBlankFrame;
    std::string m_frameString;
    bool parseString(std::string & inputString);
    void setFrameNumber(uint64_t frameNumber) { m_frameNumber = frameNumber; }

    // create operators for sorting and searching by frame number
    inline bool operator<(const FrameInputs & other) const { return m_frameNumber < other.m_frameNumber; }
    inline bool operator==(const FrameInputs & other) const { return m_frameNumber == other.m_frameNumber; }
    inline bool operator!=(const FrameInputs & other) const { return !(*this == other); }
    inline bool operator>(const FrameInputs & other) const { return !(*this < other) && (*this != other); }
    inline bool operator<=(const FrameInputs & other) const { return (*this < other) || (*this == other); }
    inline bool operator>=(const FrameInputs & other) const { return !(*this < other); }

    // operators for sorting and searching by frame number compared to a uint64_t
    inline bool operator<(const uint64_t & other) const { return m_frameNumber < other; }
    inline bool operator==(const uint64_t & other) const { return m_frameNumber == other; }
    inline bool operator!=(const uint64_t & other) const { return !(*this == other); }
    inline bool operator>(const uint64_t & other) const { return !(*this < other) && (*this != other); }
    inline bool operator<=(const uint64_t & other) const { return (*this < other) || (*this == other); }
    inline bool operator>=(const uint64_t & other) const { return !(*this < other); }
};