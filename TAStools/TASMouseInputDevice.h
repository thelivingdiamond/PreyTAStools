//
// Created by theli on 12/28/2022.
//

#ifndef TASTOOLS_TASMOUSEINPUTDEVICE_H
#define TASTOOLS_TASMOUSEINPUTDEVICE_H
#include <Prey/CryInput/IInput.h>

class TASMouseInputDevice : public IInputDevice{
public:
    ~TASMouseInputDevice() override;

    const char *GetDeviceName() override;

    EInputDeviceType GetDeviceType() override;

    unsigned __int64 GetDeviceId() override;

    int GetDeviceIndex() override;

    bool Init() override;

    void PostInit() override;

    void Update(bool b) override;

    bool SetForceFeedback(IFFParams params) override;

    bool InputState(const TKeyName *name, EInputState state) override;

    bool SetExclusiveMode(bool b) override;

    void ClearKeyState() override;

    void ClearAnalogKeyState(std::vector<SInputSymbol *> *vector) override;

    const char *GetKeyName(const EKeyId id) override;

    const char *GetKeyName(const SInputEvent *event) override;

    char GetInputCharAscii(const SInputEvent *event) override;

    const wchar_t *GetOSKeyName(const SInputEvent *event) override;

    SInputSymbol *LookupSymbol(EKeyId id) override;

    const SInputSymbol *GetSymbolByName(const char *string1) override;

    bool IsOfDeviceType(EInputDeviceType deviceType) override;

    void Enable(bool b) override;

    bool IsEnabled() override;

    bool IsConnected() override;

    void OnLanguageChange() override;

    void SetDeadZone(float d) override;

    void RestoreDefaultDeadZone() override;

    void ClearBufferedKeyState() override;

    bool IsSteamInputDevice() override;

    bool ShowBindingsPanel() override;

    bool IsSteamInBigPictureMode() override;

    unsigned int GetSteamActionOrigin(const char *const string1) override;

    const char *GetGlyphFromActionOrigin(unsigned int i) override;

    void ActivateSteamActionSet(const char *const string1) override;


    const char* m_deviceName = "TASMouseInputDevice";
};


#endif //TASTOOLS_TASMOUSEINPUTDEVICE_H
