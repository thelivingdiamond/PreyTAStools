#include <sstream>
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
        CryWarning("Invalid argument: {}", e.what());
    } catch (std::out_of_range &e) {
        CryWarning("Out of range: {}", e.what());
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
    m_bBlankFrame = false;
    m_frameString = inputString;
    // segment the string until there are no more |
    auto segmentedString = inputString;
    bool error = false;
    while(!segmentedString.empty()) {
        // segments will always begin with a pipe, so the segment will be the first character after the pipe, until the next pipe OR the end of the string
        auto segment = segmentedString.substr(1, segmentedString.find('|', 1));
        // remove the segment from the string
        segmentedString = segmentedString.substr(segment.size() + 1);
        if(segment.empty()) {
            // if the segment is empty, then there was no pipe, so the string is invalid
            CryWarning("Segment Empty: {}", inputString.c_str());
            error = true;
            break;
        }
        // parse the segment
        if(segment[0] == 'K') {
            // keyboard input
            // each keyboard input is separated by a colon, loop through the string and parse each input
            auto keyboardSegment = segment.substr(1);
            while(!keyboardSegment.empty()) {
                // each input is terminated by a colon or the end of the string
                auto input = keyboardSegment.substr(0, keyboardSegment.find(':'));
                // remove the input from the string
                if(keyboardSegment.size() > input.size()) {
                    keyboardSegment = keyboardSegment.substr(input.size() + 1);
                } else {
                    keyboardSegment = "";
                }
                if(input.empty()) {
                    CryWarning("Empty Segment: {}", inputString.c_str());
                    error = true;
                    continue;
                }
                // parse the segment
                m_keyboardInputs.emplace_back(KeyboardInput(input));
            }
        } else if(segment[0] == 'M') {
            // mouse input
            //count the number of :, if there aren't 3 then the string is invalid
            auto colonCount = std::count(segment.begin(), segment.end(), ':');
            if(colonCount != 3) {
                CryWarning("Invalid frame input string: {}", inputString.c_str());
                error = true;
            } else {
                std::string mouseInputSegment = segment.substr(1);
                m_mouseInputs.emplace_back(MouseInput(mouseInputSegment));
            }
        } else {
            // invalid segment
            CryWarning("Invalid frame input string: {}", inputString.c_str());
            error = true;
        }
    }

    return !error;
}
