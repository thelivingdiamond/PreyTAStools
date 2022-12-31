//
// Created by theli on 12/28/2022.
//

#ifndef TASTOOLS_INPUTRECORDER_H
#define TASTOOLS_INPUTRECORDER_H
#include "Prey/CryInput/IInput.h"

class InputRecorder : public IInputEventListener {
public:
    InputRecorder();

    ~InputRecorder() override;

    bool OnInputEvent(const SInputEvent &event) override;

    bool OnInputEventUI(const SInputEvent &event) override;

    int GetPriority() override;

    void registerListener();

};


#endif //TASTOOLS_INPUTRECORDER_H
