//
// Created by theli on 12/28/2022.
//

#include "TASMouseInputDevice.h"

TASMouseInputDevice::~TASMouseInputDevice() {

}

const char *TASMouseInputDevice::GetDeviceName() {
    return m_deviceName;
}

EInputDeviceType TASMouseInputDevice::GetDeviceType() {
    return eIDT_Mouse;
}

unsigned __int64 TASMouseInputDevice::GetDeviceId() {
    return 69420;
}

int TASMouseInputDevice::GetDeviceIndex() {
    return 1;
}

bool TASMouseInputDevice::Init() {
    return false;
}

void TASMouseInputDevice::PostInit() {

}

void TASMouseInputDevice::Update(bool b) {

}

bool TASMouseInputDevice::SetForceFeedback(IFFParams params) {
    return false;
}

bool TASMouseInputDevice::InputState(const TKeyName *name, EInputState state) {
    return false;
}

bool TASMouseInputDevice::SetExclusiveMode(bool b) {
    return false;
}

void TASMouseInputDevice::ClearKeyState() {

}

void TASMouseInputDevice::ClearAnalogKeyState(std::vector<SInputSymbol *> *vector) {

}

const char *TASMouseInputDevice::GetKeyName(const EKeyId id) {
    return "";
}

const char *TASMouseInputDevice::GetKeyName(const SInputEvent *event) {
    return "";
}

char TASMouseInputDevice::GetInputCharAscii(const SInputEvent *event) {
    return 0;
}

const wchar_t *TASMouseInputDevice::GetOSKeyName(const SInputEvent *event) {
    return L" ";
}

SInputSymbol *TASMouseInputDevice::LookupSymbol(EKeyId id) {
    return nullptr;
}

const SInputSymbol *TASMouseInputDevice::GetSymbolByName(const char *string1) {
    return nullptr;
}

bool TASMouseInputDevice::IsOfDeviceType(EInputDeviceType deviceType) {
    return false;
}

void TASMouseInputDevice::Enable(bool b) {

}

bool TASMouseInputDevice::IsEnabled() {
    return false;
}

bool TASMouseInputDevice::IsConnected() {
    return false;
}

void TASMouseInputDevice::OnLanguageChange() {

}

void TASMouseInputDevice::SetDeadZone(float d) {

}

void TASMouseInputDevice::RestoreDefaultDeadZone() {

}

void TASMouseInputDevice::ClearBufferedKeyState() {

}

bool TASMouseInputDevice::IsSteamInputDevice() {
    return false;
}

bool TASMouseInputDevice::ShowBindingsPanel() {
    return false;
}

bool TASMouseInputDevice::IsSteamInBigPictureMode() {
    return false;
}

unsigned int TASMouseInputDevice::GetSteamActionOrigin(const char *const string1) {
    return 0;
}

const char *TASMouseInputDevice::GetGlyphFromActionOrigin(unsigned int i) {
    return "";
}

void TASMouseInputDevice::ActivateSteamActionSet(const char *const string1) {

}
