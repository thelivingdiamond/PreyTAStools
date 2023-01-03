//
// Created by theli on 1/2/2023.
//

#include "TASReplayer.h"
#include "Actions.h"
static TASReplayer* m_instance = nullptr;

void TASReplayer::Update() {
    if (m_actions.empty()) {
        return;
    }

    while(!m_actions.empty()){
        auto action = m_actions.front();
        m_actions.pop();
        action->execute();
        switch(action->getType()){
            case Action::Type::KEYBOARD:
                m_keysPressed.push_back(static_cast<KeyboardAction*>(action)->m_keyId);
                break;
            case Action::Type::MOUSE:
                if(static_cast<MouseAction*>(action)->m_leftButton){ m_keysPressed.push_back(EKeyId::eKI_Mouse1); }
                if(static_cast<MouseAction*>(action)->m_rightButton){ m_keysPressed.push_back(EKeyId::eKI_Mouse2); }
                if(static_cast<MouseAction*>(action)->m_middleButton){ m_keysPressed.push_back(EKeyId::eKI_Mouse3); }
                if(static_cast<MouseAction*>(action)->m_xButton1){ m_keysPressed.push_back(EKeyId::eKI_Mouse4); }
                if(static_cast<MouseAction*>(action)->m_xButton2){ m_keysPressed.push_back(EKeyId::eKI_Mouse5); }
                break;
            case Action::Type::CVAR:
            case Action::Type::COMMENT:
                // do nothing
                break;
        }
    }
    // determine which keys need to be released
    for(auto & key : m_keysToRelease){
        if(std::find(m_keysPressed.begin(), m_keysPressed.end(), key) == m_keysPressed.end()){
            m_keysPressed.erase(std::remove(m_keysPressed.begin(), m_keysPressed.end(), key), m_keysPressed.end());
            Actions::releaseKey(key);
        }
    }
    // now that we've released the keys, we can clear the vector
    m_keysToRelease.clear();

    // we should move the keys pressed this frame to the release vector
    std::move(m_keysPressed.begin(), m_keysPressed.end(), std::back_inserter(m_keysToRelease));
    m_keysPressed.clear();
}

void TASReplayer::submitAction(Action *action) {
    m_actions.push(action);
}

TASReplayer *TASReplayer::getInstance() {
    return m_instance;
}

TASReplayer::TASReplayer() {
    if(m_instance == nullptr){
        m_instance = this;
    }
    else{
        CryFatalError("TASReplayer already constructed!");
    }
}

TASReplayer::~TASReplayer() {
    m_instance = nullptr;
}
