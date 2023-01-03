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

    KeyboardAction(EKeyId key);
    std::string toString() override;
    void execute() override;
    Type getType() override;
    EKeyId m_keyId;
};

struct MouseAction : public Action {
MouseAction() {
        m_type = Type::MOUSE;
    }
    MouseAction(std::string & inputString);
    void parseString(std::string & inputString) override;

    MouseAction(float x, float y, bool abs, uint32_t buttons);
    std::string toString() override;
    void execute() override;
    Type getType() override;

    float m_xPos, m_yPos;
    bool m_Absolute;
    bool m_leftButton,
            m_rightButton,
            m_middleButton,
            m_xButton1,
            m_xButton2;
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
    void releaseKey(EKeyId keyId);
    void pressKey(EKeyId keyId);
    void moveMouse(float x, float y, bool abs);
}
#endif //TASTOOLS_ACTIONS_H
