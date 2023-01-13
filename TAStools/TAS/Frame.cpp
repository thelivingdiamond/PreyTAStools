//
// Created by theli on 1/2/2023.
//

#include "Frame.h"
#include "TASReplayer.h"
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
    // the string will be passed as a series of segments that begin with a pipe, the first letter after the pipe indicates what type of segment it is
    // the rest of the segment is the data for that segment
    // split the string into segments using regex
    std::regex segmentRegex("\\|([A-Z])([^|]*)");
    std::smatch segmentMatch;
    std::string segmentString;
    while(std::regex_search(inputString, segmentMatch, segmentRegex)) {
        segmentString = segmentMatch[0];
        inputString = segmentMatch.suffix();
        // the first character of the segment string is the type of segment
        char segmentType = segmentString[1];
        // the rest of the segment string is the data for the segment
        std::string segmentData = segmentString.substr(2);
        if(segmentType == 'K'){
            // split the segment by colon to get each key
            std::regex keyRegex("([^:]+):?");
            std::smatch keyMatch;
            std::string keyString;
            while(std::regex_search(segmentData, keyMatch, keyRegex)) {
                keyString = keyMatch.str(1);
                segmentData = keyMatch.suffix();
                //TODO: see if this works
                m_actions.push_back(new KeyboardAction(keyString));
            }

        } else if(segmentType =='M') {
            // the entire data segment can just be passed to the mouse action
            m_actions.push_back(new MouseAction(segmentData));
        } else if(segmentType =='V') {
            // the CVAR segment is a colon separated list of CVARs, each in the form of <CVARNAME, CVARVALUE>, we just need to pass everything between the colons to the CVAR action, the action will parse the data
            // split the segment by colon to get each key
            // the regex needs to group items like <"", "">, and each group is separated by a colon
            std::regex cvarRegex("([^:]+):?");
            std::smatch cvarMatch;
            std::string cvarString;
            while(std::regex_search(segmentData, cvarMatch, cvarRegex)) {
                cvarString = cvarMatch.str(1);
                segmentData = cvarMatch.suffix();
                addAction(new CVarAction(cvarString));
            }

        } else if (segmentType == 'C'){
            // pass the segment minus the first character to the comment action
            auto commentString = segmentString.substr(2);
            m_actions.push_back(new CommentAction(commentString));
//            m_actions.push_back(new CommentAction(segmentString));
        }
    }

    /*// segment the string until there are no more |
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
    }*/
}

void Frame::execute() {
    for (auto & action : m_actions) {
        TASReplayer::getInstance()->submitAction(action);
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
    std::string KeyboardString, MouseString, CVarString, CommentString;
    for (auto & action : m_actions) {
        switch(action->getType()){
            case Action::Type::KEYBOARD:
                if(KeyboardString.empty()) {
                    KeyboardString += "|K";
                    KeyboardString += action->toString();
                } else {
                    KeyboardString += ":";
                    KeyboardString += action->toString();
                }
                break;
            case Action::Type::MOUSE:
                MouseString = "|M" + action->toString();
                break;
            case Action::Type::CVAR:
                if(CVarString.empty()) {
                    CVarString += "|V";
                    CVarString += action->toString();
                } else {
                    CVarString += ":";
                    CVarString += action->toString();
                }
                break;
            case Action::Type::COMMENT:
                CommentString = "|C" + action->toString();
                break;
        }
    }
    if(!KeyboardString.empty()) {
        outputString += KeyboardString;
    }
    if(!MouseString.empty()) {
        outputString += MouseString;
    }
    if(!CVarString.empty()) {
        outputString += CVarString;
    }
    if(!CommentString.empty()) {
        outputString += CommentString;
    }
    return outputString;
}

Frame::Frame(std::vector<SInputEvent> &events, uint64_t frameNumber) {
    parseEvents(events);
    m_frameNumber = frameNumber;
}

void Frame::parseEvents(std::vector<SInputEvent> &events) {
    m_actions.clear();
    if(events.empty()){
        return;
    }
    // mouse variables
    float xPos = 0, yPos = 0;
    int left = -1, right = -1, middle = -1, x1 = -1, x2 = -1;
    bool bMouseChanged = false;
    // TODO: absolute mouse position is not supported yet
    for (auto & event : events) {
        if(event.state == eIS_UI){
            //convert from keyName to keyID FUCK why isn't there a function for this
            std::wstring string;
            string.resize(1);
            string[0] = event.inputChar;
            auto symbol = gEnv->pInput->LookupSymbolByName(event.deviceType, 0, fs::path(string).u8string().c_str());
            if(symbol != nullptr){
                event.keyId = symbol->keyId;
            } else {
                CryWarning("Invalid key name: {}", event.keyName.key);
            }
        }
        if(event.deviceType == eIDT_Keyboard) {
            // construct a string from the event
            KeyboardAction *action = new KeyboardAction(event.keyId, event.state);
            addAction(action);
        } else if(event.deviceType == eIDT_Mouse) {
            if(event.keyId == EKeyId::eKI_MouseX) {
                xPos = event.value;
                bMouseChanged |= true;
            } else if (event.keyId == EKeyId::eKI_MouseY) {
                yPos = event.value;
                bMouseChanged |= true;
            } else if (event.keyId == EKeyId::eKI_Mouse1) {
                left = event.state;
                bMouseChanged |= true;
            } else if (event.keyId == EKeyId::eKI_Mouse2) {
                right = event.state;
                bMouseChanged |= true;
            } else if (event.keyId == EKeyId::eKI_Mouse3) {
                middle = event.state;
                bMouseChanged |= true;
            } else if (event.keyId == EKeyId::eKI_Mouse4) {
                x1 = event.state;
                bMouseChanged |= true;
            } else if (event.keyId == EKeyId::eKI_Mouse5) {
                x2 = event.state;
                bMouseChanged |= true;
            }
        }
    }
    if(bMouseChanged){
        auto action = new MouseAction(xPos, yPos, false, left, right, middle, x1, x2);
        addAction(action);
    }
}

Frame::Frame(uint64_t frameNumber) {
    m_frameNumber = frameNumber;
}

bool Frame::verify(std::vector<SInputEvent> inputs) {
    // go through each input that was recorded and make sure it matches the frame
    // if it doesn't match, return false
    // first check for the empty frame case both ways
    if(inputs.empty() && m_actions.empty()){
        return true;
    } else if(inputs.empty() || m_actions.empty()){
        return false;
    }
    for(auto & input : inputs){
        if(input.deviceType == eIDT_Keyboard){
            // check if the keyboard input is in the frame
            bool found = false;
            for(auto & action : m_actions){
                if(action->getType() == Action::Type::KEYBOARD){
                    auto keyboardAction = static_cast<KeyboardAction*>(action);
                    if(keyboardAction->m_keyId == input.keyId && keyboardAction->m_keyState == input.state){
                        found = true;
                        break;
                    }
                }
            }
            if(!found){
                return false;
            }
        } else if(input.deviceType == eIDT_Mouse){
            // check if the mouse input is in the frame
            bool found = false;
            for(auto & action : m_actions){
                if(action->getType() == Action::Type::MOUSE){
                    auto mouseAction = static_cast<MouseAction*>(action);
                    if(input.keyId == EKeyId::eKI_MouseX){
                        if(mouseAction->m_xPos == input.value){
                            found = true;
                            break;
                        }
                    } else if(input.keyId == EKeyId::eKI_MouseY){
                        if(mouseAction->m_yPos == input.value){
                            found = true;
                            break;
                        }
                    } else if(input.keyId == EKeyId::eKI_Mouse1){
                        if(mouseAction->m_leftButton == input.state){
                            found = true;
                            break;
                        }
                    } else if(input.keyId == EKeyId::eKI_Mouse2){
                        if(mouseAction->m_rightButton == input.state){
                            found = true;
                            break;
                        }
                    } else if(input.keyId == EKeyId::eKI_Mouse3){
                        if(mouseAction->m_middleButton == input.state){
                            found = true;
                            break;
                        }
                    } else if(input.keyId == EKeyId::eKI_Mouse4){
                        if(mouseAction->m_xButton1 == input.state){
                            found = true;
                            break;
                        }
                    } else if(input.keyId == EKeyId::eKI_Mouse5){
                        if(mouseAction->m_xButton2 == input.state){
                            found = true;
                            break;
                        }
                    }
                }
            }
            if(!found){
                return false;
            }
        }
    }
    return true;
}
