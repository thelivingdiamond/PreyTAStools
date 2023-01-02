//
// Created by theli on 1/2/2023.
//

#include "Frame.h"
#include <regex>

Frame::Frame(std::string &inputString, uint64_t frameNumber) {
    parseString(inputString);
    m_frameNumber = frameNumber;
}

void Frame::parseString(std::string &inputString) {
    // frame inputs will be |K...|M... where K... and M... are the inputs for the keyboard and mouse respectively
    // if no sections are specified then there is no inputs and the mouse doesn't move.
    m_actions.clear();
    m_inputString = inputString;
    if(inputString.empty()){
        return;
    }
    // segment the string until there are no more |
    auto segmentedString = inputString;
    bool error = false;
    while(!segmentedString.empty()) {
        // segments will always begin with a pipe, so the segment will be the first character after the pipe, until the next pipe OR the end of the string
        auto segment = segmentedString.substr(1, segmentedString.find('|', 1) - 1);
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
                auto action = new KeyboardAction(input);
                addAction(action);
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
                auto action = new MouseAction(mouseInputSegment);
                addAction(action);
            }
        } else if (segment[0] == 'V'){
            // CVar input
            // string is in the format [cvarName]:[cvarValue]
            // verify that there is only one :, and that the brackets are present using regex
            std::regex cvarRegex(R"(\[.*\]:\[.*\])");
            if(!std::regex_match(segment, cvarRegex)) {
                CryWarning("Invalid frame input string: {}", inputString.c_str());
                error = true;
            } else {
                std::string cvarInputSegment = segment.substr(1);
                auto action = new CVarAction(cvarInputSegment);
                m_actions.push_back(action);
            }
            auto action = new CVarAction(segment);
            addAction(action);
        } else if (segment[0] == 'C'){
            // Comment input
            // string is in the format "comment"
            // verify there are quotes at the beginning and end of the string
            if(segment[1] != '"' || segment[segment.size() - 1] != '"') {
                CryWarning("Invalid comment input string: {}", inputString.c_str());
                error = true;
            } else {
                auto action = new CommentAction(segment);
                addAction(action);
            }
        } else {
            CryWarning("Invalid frame input prefix: {}", inputString.c_str());
            error = true;
        }
    }
}

void Frame::execute() {
    for (auto & action : m_actions) {
        action->execute();
    }
}

void Frame::addAction(Action *action) {
    m_actions.push_back(action);
}

void Frame::removeAction(Action *action) {
    m_actions.erase(std::remove(m_actions.begin(), m_actions.end(), action), m_actions.end());
}

void Frame::clearActions() {
    m_actions.clear();
}

std::string Frame::toString() {
    std::string outputString;
    for (auto & action : m_actions) {
        switch(action->getType()){
            case Action::Type::KEYBOARD:
                outputString += "|K" + action->toString();
                break;
            case Action::Type::MOUSE:
                outputString += "|M" + action->toString();
                break;
            case Action::Type::CVAR:
                outputString += "|V" + action->toString();
                break;
            case Action::Type::COMMENT:
                outputString += "|C" + action->toString();
                break;
        }
    }
    return outputString;
}
