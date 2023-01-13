//
// Created by theli on 12/28/2022.
//

#ifndef TASTOOLS_INPUTLISTENER_H
#define TASTOOLS_INPUTLISTENER_H
#include "Prey/CryInput/IInput.h"

class InputListener : public IInputEventListener {
public:
    InputListener();

    ~InputListener() override;

    bool OnInputEvent(const SInputEvent &event) override;

    bool OnInputEventUI(const SInputEvent &event) override;

    int GetPriority() override;

    void registerListener();

    void getEvents(std::vector<SInputEvent>& events);
    void update();
    size_t getEventCount() { return m_events.size(); }
    std::queue<SInputEvent> m_events;
};


#endif //TASTOOLS_INPUTLISTENER_H
