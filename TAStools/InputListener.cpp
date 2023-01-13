//
// Created by theli on 12/28/2022.
//

#include "InputListener.h"

InputListener::~InputListener() {
    gEnv->pInput->RemoveEventListener(this);

}

bool InputListener::OnInputEvent(const SInputEvent &event) {
    if(event.keyId != EKeyId::eKI_SYS_Commit)
        m_events.push(event);
    return false;
}

bool InputListener::OnInputEventUI(const SInputEvent &event) {
    if(event.keyId != EKeyId::eKI_SYS_Commit)
        m_events.push(event);
    return false;
}

int InputListener::GetPriority() {
    return 0;
}

InputListener::InputListener() {

}

void InputListener::registerListener() {
    gEnv->pInput->AddEventListener(this);
}

void InputListener::getEvents(std::vector<SInputEvent> &events) {
    while(!m_events.empty()){
        events.push_back(m_events.front());
        m_events.pop();
    }
}

void InputListener::update() {
    // if the events weren't fetched this frame, we should clear them
    while(!m_events.empty()){
        m_events.pop();
    }
    // now we are ready to record new events for the next frame

}
