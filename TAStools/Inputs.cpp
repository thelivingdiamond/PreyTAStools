#include "Inputs.h"

KeyboardInput::KeyboardInput(std::string & inputString) {
    m_type = Type::KEYBOARD;
    parseString(inputString);
}

void KeyboardInput::parseString(std::string &inputString) {
    //Keyboard Input will be handed as an unsigned hexadecimal string corresponding to the EKeyId
    try {
        m_keyId = static_cast<EKeyId>(std::stoul(inputString, nullptr, 16));
    } catch (std::invalid_argument &e) {
        CryWarning("Invalid argument: %s", e.what());
    } catch (std::out_of_range &e) {
        CryWarning("Out of range: %s", e.what());
    }
}

Input::Type KeyboardInput::getType() {
    return m_type;
}

MouseInput::MouseInput(std::string &inputString) {
    m_type = Type::MOUSE;
    parseString(inputString);
}

void MouseInput::parseString(std::string &inputString) {
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
        CryWarning("Invalid x pos argument: %s", e.what());
        m_xPos = 0.0f;
    }
    try {
        m_yPos = std::stof(y);
    } catch (std::invalid_argument &e) {
        CryWarning("Invalid y pos argument: %s", e.what());
        m_yPos = 0.0f
    }

    if (abs == "A") {
        m_Absolute = true;
    } else if (abs == "R") {
        m_Absolute = false;
    } else {
        CryWarning("Invalid absolute argument: %s", abs.c_str());
    }

    // the buttons will be pressed if each corresponding character == "1", not pressed if they are == "."
    if(buttons.size() < 5) {
        CryWarning("Invalid buttons argument: %s", buttons.c_str());
    } else {
        m_leftButton = buttons[0] != '.';
        m_rightButton = buttons[1] != '.';
        m_middleButton = buttons[2] != '.';
        m_xButton1 = buttons[3] != '.';
        m_xButton2 = buttons[4] != '.';
    }

}

Input::Type MouseInput::getType() {
    return m_type;
}

bool FrameInputs::
parseString(std::string &inputString) {
    // frame inputs will be |K...|M... where K... and M... are the inputs for the keyboard and mouse respectively
    // if no sections are specified then there is no inputs and the mouse doesn't move.
    if(inputString.empty()){
        m_bBlankFrame = true;
        return true;
    }
    // segment the string until there are no more |
    auto segmentedString = inputString;
    bool error = false;
    while(!segmentedString.empty()) {
        // find the next |
        auto nextPipe = segmentedString.find('|');
        // if there is no next pipe then the string is invalid
        if(nextPipe == std::string::npos) {
            CryWarning("Invalid frame input string: %s", inputString.c_str());
            error = true;
        }
        // get the next segment
        auto segment = segmentedString.substr(0, nextPipe);
        // if the segment is empty then the string is invalid
        if(segment.empty()) {
            CryWarning("Invalid frame input string: %s", inputString.c_str());
            error = true;
        }
        // parse the segment
        if(segment[0] == 'K') {
            // keyboard input
            // each keyboard input is separated by a colon, loop through the string and parse each input
            auto keyboardSegment = segment.substr(1);
            while(!keyboardSegment.empty()) {
                // find the next colon
                auto nextColon = keyboardSegment.find(':');
                // if there is no next colon then the string is invalid
                if(nextColon == std::string::npos) {
                    CryWarning("Invalid frame input string: %s", inputString.c_str());
                    error = true;
                    break;
                }
                // get the next segment
                auto keyboardInputSegment = keyboardSegment.substr(0, nextColon);
                // if the segment is empty then the string is invalid
                if(keyboardInputSegment.empty()) {
                    CryWarning("Invalid frame input string: %s", inputString.c_str());
                    error = true;
                    continue;
                }
                // parse the segment
                m_keyboardInputs.emplace_back(KeyboardInput(keyboardInputSegment));
                // remove the segment from the string
                keyboardSegment = keyboardSegment.substr(nextColon + 1);
            }
        } else if(segment[0] == 'M') {
            // mouse input
            //count the number of :, if there aren't 3 then the string is invalid
            auto colonCount = std::count(segment.begin(), segment.end(), ':');
            if(colonCount != 3) {
                CryWarning("Invalid frame input string: %s", inputString.c_str());
                error = true;
            } else {
                std::string mouseInputSegment = segment.substr(1);
                m_mouseInputs.emplace_back(MouseInput(mouseInputSegment));
            }
        } else {
            // invalid segment
            CryWarning("Invalid frame input string: %s", inputString.c_str());
            error = true;
        }
        // remove the segment from the string
        segmentedString = segmentedString.substr(nextPipe + 1);
    }

    return !error;
}
