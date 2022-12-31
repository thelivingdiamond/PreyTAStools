//
// Created by theli on 12/28/2022.
//

#include "InputRecorder.h"

InputRecorder::~InputRecorder() {
    gEnv->pInput->RemoveEventListener(this);

}

bool InputRecorder::OnInputEvent(const SInputEvent &event) {
    return false;
}

bool InputRecorder::OnInputEventUI(const SInputEvent &event) {
    return false;
}

int InputRecorder::GetPriority() {
    return 0;
}

InputRecorder::InputRecorder() {

}

void InputRecorder::registerListener() {
    gEnv->pInput->AddEventListener(this);
}
