//
// Created by theli on 1/2/2023.
//

#include <sstream>
#include <utility>
#include "Actions.h"
#include <Prey/CryInput/IHardwareMouse.h>

KeyboardAction::KeyboardAction(std::string &inputString) {
    parseString(inputString);
    m_type = Type::KEYBOARD;
}

void KeyboardAction::parseString(std::string &inputString) {
    //Keyboard Input will be handed as an unsigned hexadecimal string corresponding to the EKeyId
    try {
        m_keyId = static_cast<EKeyId>(std::stoul(inputString, nullptr, 16));
    } catch (std::invalid_argument &e) {
        CryWarning("Invalid argument: {}", e.what());
    } catch (std::out_of_range &e) {
        CryWarning("Out of range: {}", e.what());
    }
}

void KeyboardAction::execute() {
    Actions::pressKey(m_keyId);
}

Action::Type KeyboardAction::getType() {
    return m_type;
}

KeyboardAction::KeyboardAction(EKeyId key) {
    m_keyId = key;
}

std::string KeyboardAction::toString() {
    return std::to_string(m_keyId);
}

MouseAction::MouseAction(std::string &inputString) {
    parseString(inputString);
    m_type = Type::MOUSE;
}

void MouseAction::parseString(std::string &inputString) {
    //Mouse inputs will be passed as xpos:ypos:X:12345 where 12345 correspond to the mouse buttons, and X will be A or R for absolute or relative
    // split the string
    std::string x, y, abs, buttons;
    std::istringstream iss(inputString);
    std::getline(iss, x, ':');
    std::getline(iss, y, ':');
    std::getline(iss, abs, ':');
    std::getline(iss, buttons);
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

    // the buttons will be pressed if each corresponding character == "1", not pressed if they are == "."
    if(buttons.size() < 5) {
        CryWarning("Invalid buttons argument: {}", buttons.c_str());
    } else {
        m_leftButton = buttons[0] != '.';
        m_rightButton = buttons[1] != '.';
        m_middleButton = buttons[2] != '.';
        m_xButton1 = buttons[3] != '.';
        m_xButton2 = buttons[4] != '.';
    }
}

void MouseAction::execute() {
    Actions::moveMouse(m_xPos, m_yPos, m_Absolute);
    if (m_leftButton) {
        Actions::pressKey(EKeyId::eKI_Mouse1);
    }
    if (m_rightButton) {
        Actions::pressKey(EKeyId::eKI_Mouse2);
    }
    if (m_middleButton) {
        Actions::pressKey(EKeyId::eKI_Mouse3);
    }
    if (m_xButton1) {
        Actions::pressKey(EKeyId::eKI_Mouse4);
    }
    if (m_xButton2) {
        Actions::pressKey(EKeyId::eKI_Mouse5);
    }

}

Action::Type MouseAction::getType() {
    return m_type;
}

MouseAction::MouseAction(float x, float y, bool abs, uint32_t buttons) {
    m_xPos = x;
    m_yPos = y;
    m_Absolute = abs;
    m_leftButton = buttons & 1;
    m_rightButton = buttons & 2;
    m_middleButton = buttons & 4;
    m_xButton1 = buttons & 8;
    m_xButton2 = buttons & 16;
}

std::string MouseAction::toString() {
    std::string abs = m_Absolute ? "A" : "R";
    std::string buttons;
    buttons += m_leftButton ? "1" : ".";
    buttons += m_rightButton ? "1" : ".";
    buttons += m_middleButton ? "1" : ".";
    buttons += m_xButton1 ? "1" : ".";
    buttons += m_xButton2 ? "1" : ".";
    return std::to_string(m_xPos) + ":" + std::to_string(m_yPos) + ":" + abs + ":" + buttons;
}

CVarAction::CVarAction(std::string &inputString) {
    parseString(inputString);
    m_type = Type::CVAR;
}

void CVarAction::parseString(std::string &inputString) {
    //CVar inputs will be passed as [cvarName]:[cvarValue]
    // split the string
    std::string name, value;
    std::istringstream iss(inputString);
    std::getline(iss, name, ':');
    std::getline(iss, value);
    // remove the brackets (they are at the beginning and end of the string)
    name = name.substr(1, name.size() - 2);
    value = value.substr(1, value.size() - 2);

    m_cvarName = name;
    m_cvarValue = value;
}

void CVarAction::execute() {
    gEnv->pConsole->GetCVar(m_cvarName.c_str())->Set(m_cvarValue.c_str());
}

Action::Type CVarAction::getType() {
    return m_type;
}

CVarAction::CVarAction(std::string cvar, std::string value) {
    m_cvarName = std::move(cvar);
    m_cvarValue = std::move(value);

}

std::string CVarAction::toString() {
    return "[" + m_cvarName + "]:[" + m_cvarValue + "]";
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
