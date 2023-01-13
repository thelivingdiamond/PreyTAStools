//
// Created by theli on 1/2/2023.
//

#include <sstream>
#include <utility>
#include "Actions.h"
#include <Prey/CryInput/IHardwareMouse.h>
#include <regex>

KeyboardAction::KeyboardAction(std::string &inputString) {
    parseString(inputString);
    m_type = Type::KEYBOARD;
}

void KeyboardAction::parseString(std::string &inputString) {

    //Keyboard Input will be handed as an unsigned hexadecimal string, with a final number in brackets representing the key input state
    // Example: 10[1] = 0x10 (KEY_W) pressed
    // parse the string using regex
    std::regex regex("([0-9A-Fa-f]+)\\[([0-9]+)\\]");
    std::smatch match;
    if(std::regex_search(inputString, match, regex)) {
        try {
            m_keyId = static_cast<EKeyId>(std::stoul(match[1], nullptr, 16));
            m_keyState = static_cast<EInputState>(std::stoul(match[2], nullptr, 16));
        } catch (std::invalid_argument &e) {
            CryWarning("Invalid argument: {}", e.what());
        } catch (std::out_of_range &e) {
            CryWarning("Out of range: {}", e.what());
        }
    } else {
        CryWarning("Invalid Keyboard Action: {}", inputString);
    }
}

void KeyboardAction::execute() {
    Actions::KeyAction(m_keyId, m_keyState);
}

Action::Type KeyboardAction::getType() {
    return m_type;
}

KeyboardAction::KeyboardAction(EKeyId key, EInputState state) {
    m_keyId = key;
    m_type = Type::KEYBOARD;
    m_keyState = state;
}

std::string KeyboardAction::toString() {
    return fmt::format("{:X}[{:X}]", m_keyId, m_keyState);
}

MouseAction::MouseAction(std::string &inputString) {
    parseString(inputString);
    m_type = Type::MOUSE;
}

void MouseAction::parseString(std::string &inputString) {
    //Mouse inputs will be passed as xpos:ypos:X:12345 where 12345 correspond to the mouse buttons, and X will be A or R for absolute or relative. Each of the last 5 digits will either be a 1 or a . for pressed or not pressed respectively.
    //example: 50.0:50.0:A:11111 will move the mouse to [50,50] on the screen and press all 5 buttons.
    // split the string
    std::string x, y, abs, buttons;
    // use regex
    std::regex re("([0-9.]+):([0-9.]+):([AR]):([0-9.]+)");
    std::smatch match;
    if (std::regex_search(inputString, match, re)) {
        x = match[1];
        y = match[2];
        abs = match[3];
        buttons = match[4];
    } else {
        CryWarning("Invalid mouse input string: {}", inputString);
        return;
    }
    // parse the values
    try {
        m_xPos = std::stof(x);
    } catch (std::invalid_argument &e) {
        CryWarning("Invalid x pos argument: {}", e.what());
        m_xPos = 0.0f;
    }
    try {
        m_yPos = std::stof(y);
    } catch (std::invalid_argument &e) {
        CryWarning("Invalid y pos argument: {}", e.what());
        m_yPos = 0.0f;
    }

    if (abs == "A") {
        m_Absolute = true;
    } else if (abs == "R") {
        m_Absolute = false;
    } else {
        CryWarning("Invalid absolute argument: {}", abs.c_str());
    }

    // each integer represents the state of a button
    if(buttons.size() < 5) {
        CryWarning("Invalid buttons argument: {}", buttons.c_str());
    } else {
        if(buttons[0] == '.') {
            m_leftButton = -1;
        } else {
            m_leftButton = std::stoi(buttons.substr(0, 1), nullptr, 16);
        }
        if(buttons[1] == '.') {
            m_rightButton = -1;
        } else {
            m_rightButton = std::stoi(buttons.substr(1, 1), nullptr, 16);
        }
        if(buttons[2] == '.') {
            m_middleButton = -1;
        } else {
            m_middleButton = std::stoi(buttons.substr(2, 1), nullptr, 16);
        }
        if(buttons[3] == '.') {
            m_xButton1 = -1;
        } else {
            m_xButton1 = std::stoi(buttons.substr(3, 1), nullptr, 16);
        }
        if(buttons[4] == '.') {
            m_xButton2 = -1;
        } else {
            m_xButton2 = std::stoi(buttons.substr(4, 1), nullptr, 16);
        }
    }
}

void MouseAction::execute() {
    Actions::moveMouse(m_xPos, m_yPos, m_Absolute);
    if (m_leftButton != -1) {
        try {
            Actions::KeyAction(eKI_Mouse1, static_cast<EInputState>(m_leftButton));
        } catch (std::invalid_argument &e) {
            CryWarning("Invalid argument: {}", e.what());
        } catch (std::out_of_range &e) {
            CryWarning("Out of range: {}", e.what());
        }
    }
    if (m_rightButton != -1) {
        try {
            Actions::KeyAction(eKI_Mouse2, static_cast<EInputState>(m_rightButton));
        } catch (std::invalid_argument &e) {
            CryWarning("Invalid argument: {}", e.what());
        } catch (std::out_of_range &e) {
            CryWarning("Out of range: {}", e.what());
        }
    }
    if (m_middleButton != -1) {
        try {
            Actions::KeyAction(eKI_Mouse3, static_cast<EInputState>(m_middleButton));
        } catch (std::invalid_argument &e) {
            CryWarning("Invalid argument: {}", e.what());
        } catch (std::out_of_range &e) {
            CryWarning("Out of range: {}", e.what());
        }
    }
    if (m_xButton1 != -1) {
        try {
            Actions::KeyAction(eKI_Mouse4, static_cast<EInputState>(m_xButton1));
        } catch (std::invalid_argument &e) {
            CryWarning("Invalid argument: {}", e.what());
        } catch (std::out_of_range &e) {
            CryWarning("Out of range: {}", e.what());
        }
    }
    if (m_xButton2 != -1) {
        try {
            Actions::KeyAction(eKI_Mouse4, static_cast<EInputState>(m_xButton2));
        } catch (std::invalid_argument &e) {
            CryWarning("Invalid argument: {}", e.what());
        } catch (std::out_of_range &e) {
            CryWarning("Out of range: {}", e.what());
        }
    }

}

Action::Type MouseAction::getType() {
    return m_type;
}

MouseAction::MouseAction(float x, float y, bool abs, int left, int right, int middle, int x1, int x2) {
    m_xPos = x;
    m_yPos = y;
    m_Absolute = abs;
    m_leftButton = left;
    m_rightButton = right;
    m_middleButton = middle;
    m_xButton1 = x1;
    m_xButton2 = x2;
    m_type = Type::MOUSE;
}

std::string MouseAction::toString() {
    auto leftButton = m_leftButton == -1 ? "." : fmt::format("{:X}", m_leftButton);
    return fmt::format("{:.1f}:{:.1f}:{}:{}{}{}{}{}", m_xPos, m_yPos, m_Absolute ? "A" : "R",
                       m_leftButton == -1 ? "." : fmt::format("{:X}", m_leftButton),
                       m_rightButton == -1 ? "." : fmt::format("{:X}", m_rightButton),
                       m_middleButton == -1 ? "." : fmt::format("{:X}", m_middleButton),
                       m_xButton1 == -1 ? "." : fmt::format("{:X}", m_xButton1),
                       m_xButton2 == -1 ? "." : fmt::format("{:X}", m_xButton2));
}

CVarAction::CVarAction(std::string &inputString) {
    parseString(inputString);
    m_type = Type::CVAR;
}

void CVarAction::parseString(std::string &inputString) {
    //CVar inputs will be passed as <CVARNAME, CVARVALUE>
    //example: <"g_godmode", "1"> will set g_godmode to 1
    // split the string using regex, extract the cvar name and value without the quotes
    std::regex re("<\"(.+)\",\"(.+)\">");
    std::smatch match;
    if (std::regex_search(inputString, match, re)) {
        m_cvarName = match[1];
        m_cvarValue = match[2];
    } else {
        CryWarning("Invalid cvar input string: {}", inputString);
    }

}

void CVarAction::execute() {
//    gEnv->pConsole->GetCVar(m_cvarName.c_str())->Set(m_cvarValue.c_str());
        //TODO: implement me
}

Action::Type CVarAction::getType() {
    return m_type;
}

CVarAction::CVarAction(std::string cvar, std::string value) {
    m_cvarName = std::move(cvar);
    m_cvarValue = std::move(value);

}

std::string CVarAction::toString() {
    return "<\"" + m_cvarName + "\",\"" + m_cvarValue + "\">";
}

CommentAction::CommentAction(std::string &inputString) {
    parseString(inputString);
    m_type = Type::COMMENT;

}

void CommentAction::parseString(std::string &inputString) {
    //Comments will be passed as "comment"
    // remove the quotes
    m_comment = inputString.substr(1, inputString.size() - 2);
}

void CommentAction::execute() {
    //Do nothing, this is a comment
}

Action::Type CommentAction::getType() {
    return m_type;
}

std::string CommentAction::toString() {
    return "\"" + m_comment + "\"";
}



void Actions::releaseKey(EKeyId keyId) {
    if(keyId == EKeyId::eKI_Mouse1 || keyId == EKeyId::eKI_Mouse2 || keyId == EKeyId::eKI_Mouse3 || keyId == EKeyId::eKI_Mouse4 || keyId == EKeyId::eKI_Mouse5){
        SInputEvent event{};
        event.deviceType = eIDT_Mouse;
        event.keyId = keyId;
        event.keyName.key = gEnv->pInput->GetKeyName(keyId);
        event.state = eIS_Released;
        event.modifiers = 0;
        event.deviceIndex = 0;
        event.value = 0;
        event.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&event, true);
    } else {
        SInputEvent event{};
        event.deviceType = eIDT_Keyboard;
        event.keyId = keyId;
        event.keyName.key = gEnv->pInput->GetKeyName(keyId);
        event.state = eIS_Released;
        event.modifiers = 0;
        event.deviceIndex = 0;
        event.value = 0;
        event.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&event, true);
    }
}

void Actions::pressKey(EKeyId keyId) {
    if(keyId == EKeyId::eKI_Mouse1 || keyId == EKeyId::eKI_Mouse2 || keyId == EKeyId::eKI_Mouse3 || keyId == EKeyId::eKI_Mouse4 || keyId == EKeyId::eKI_Mouse5){
        SInputEvent event{};
        event.deviceType = eIDT_Mouse;
        event.keyId = keyId;
        event.keyName.key = gEnv->pInput->GetKeyName(keyId);
        event.state = eIS_Pressed;
        event.modifiers = 0;
        event.deviceIndex = 0;
        event.value = 0;
        event.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&event, true);
    } else {
        SInputEvent event{};
        event.deviceType = eIDT_Keyboard;
        event.keyId = keyId;
        event.keyName.key = gEnv->pInput->GetKeyName(keyId);
        event.state = eIS_Pressed;
        event.modifiers = 0;
        event.deviceIndex = 0;
        event.value = 0;
        event.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&event, true);
    }
}

void Actions::moveMouse(float x, float y, bool abs) {
    if(abs) {
        // used for menu navigation
        gEnv->pHardwareMouse->SetHardwareMouseClientPosition(x, y);
    } else {
        // used for aiming
        SInputEvent eventX{};
        eventX.deviceType = eIDT_Mouse;
        eventX.keyId = EKeyId::eKI_MouseX;
        eventX.keyName.key = gEnv->pInput->GetKeyName(EKeyId::eKI_MouseX);
        eventX.state = eIS_Changed;
        eventX.modifiers = 0;
        eventX.deviceIndex = 0;
        eventX.value = x;
        eventX.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&eventX, true);

        SInputEvent eventY{};
        eventY.deviceType = eIDT_Mouse;
        eventY.keyId = EKeyId::eKI_MouseY;
        eventY.keyName.key = gEnv->pInput->GetKeyName(EKeyId::eKI_MouseY);
        eventY.state = eIS_Changed;
        eventY.modifiers = 0;
        eventY.deviceIndex = 0;
        eventY.value = y;
        eventY.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&eventY, true);
    }

}

void Actions::downKey(EKeyId keyId) {
    if(keyId == EKeyId::eKI_Mouse1 || keyId == EKeyId::eKI_Mouse2 || keyId == EKeyId::eKI_Mouse3 || keyId == EKeyId::eKI_Mouse4 || keyId == EKeyId::eKI_Mouse5){
        SInputEvent event{};
        event.deviceType = eIDT_Mouse;
        event.keyId = keyId;
        event.keyName.key = gEnv->pInput->GetKeyName(keyId);
        event.state = eIS_Down;
        event.modifiers = 0;
        event.deviceIndex = 0;
        event.value = 0;
        event.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&event, true);
    } else {
        SInputEvent event{};
        event.deviceType = eIDT_Keyboard;
        event.keyId = keyId;
        event.keyName.key = gEnv->pInput->GetKeyName(keyId);
        event.state = eIS_Down;
        event.modifiers = 0;
        event.deviceIndex = 0;
        event.value = 0;
        event.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&event, true);
    }

}

void Actions::KeyAction(EKeyId key, EInputState state) {
    if(key == EKeyId::eKI_Mouse1 || key == EKeyId::eKI_Mouse2 || key == EKeyId::eKI_Mouse3 || key == EKeyId::eKI_Mouse4 || key == EKeyId::eKI_Mouse5){
        SInputEvent event{};
        event.deviceType = eIDT_Mouse;
        event.keyId = key;
        event.keyName.key = gEnv->pInput->GetKeyName(key);
        event.state = state;
        event.modifiers = 0;
        event.deviceIndex = 0;
        event.value = 0;
        event.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&event, true);
    } else {
        SInputEvent event{};
        event.deviceType = eIDT_Keyboard;
        event.keyId = key;
        event.keyName.key = gEnv->pInput->GetKeyName(key);
        event.state = state;
        event.modifiers = 0;
        event.deviceIndex = 0;
        event.value = 0;
        event.pSymbol = nullptr;
        gEnv->pInput->PostInputEvent(&event, true);
    }
}
