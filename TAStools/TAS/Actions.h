//
// Created by theli on 1/2/2023.
//

#ifndef TASTOOLS_ACTIONS_H
#define TASTOOLS_ACTIONS_H


struct Action {
    enum class Type {
        KEYBOARD,
        MOUSE,
        CVAR,
        COMMENT,
    };
    virtual void parseString(std::string & inputString) = 0;
    virtual std::string toString() = 0;
    virtual void execute() = 0;
    virtual Type getType() = 0;
    Type m_type;
};

struct KeyboardAction : public Action {
    KeyboardAction() {
        m_type = Type::KEYBOARD;
    }
    KeyboardAction(std::string & inputString);
    void parseString(std::string & inputString) override;

    KeyboardAction(EKeyId key, EInputState state);
    std::string toString() override;
    void execute() override;
    Type getType() override;
    EKeyId m_keyId;
    EInputState m_keyState;
};

struct MouseAction : public Action {
MouseAction() {
        m_type = Type::MOUSE;
    }
    MouseAction(std::string & inputString);
    void parseString(std::string & inputString) override;

    MouseAction(float x, float y, bool abs, int left, int right, int middle, int x1, int x2);
    std::string toString() override;
    void execute() override;
    Type getType() override;

    float m_xPos = 0.0f, m_yPos = 0.0f;
    bool m_Absolute = false;
    int     m_leftButton = -1,
             m_rightButton = -1,
             m_middleButton = -1,
             m_xButton1 = -1,
             m_xButton2 = -1;
};

struct CVarAction : public Action {
    CVarAction() {
        m_type = Type::CVAR;
    }
    CVarAction(std::string & inputString);
    void parseString(std::string & inputString) override;

    CVarAction(std::string cvar, std::string value);
    std::string toString() override;
    void execute() override;
    Type getType() override;
    std::string m_cvarName;
    std::string m_cvarValue;
};

struct CommentAction : public Action {
    CommentAction() {
        m_type = Type::COMMENT;
    }
    CommentAction(std::string & inputString);
    void parseString(std::string & inputString) override;

    std::string toString() override;
    void execute() override;
    Type getType() override;
    std::string m_comment;
};
namespace Actions {
    void KeyAction(EKeyId key, EInputState state);
    void releaseKey(EKeyId keyId);
    void pressKey(EKeyId keyId);
    void downKey(EKeyId keyId);
    void moveMouse(float x, float y, bool abs);
}
#endif //TASTOOLS_ACTIONS_H
