//
// Created by theli on 1/1/2023.
//

#ifndef TASTOOLS_INPUTNAMEMAP_H
#define TASTOOLS_INPUTNAMEMAP_H

#include <Prey/CryCore/StdAfx.h>

static const std::map<EKeyId, std::string> EKeyIdToString = {
    {eKI_Escape, "Escape"},
    {eKI_1, "1"},
    {eKI_2, "2"},
    {eKI_3, "3"},
    {eKI_4, "4"},
    {eKI_5, "5"},
    {eKI_6, "6"},
    {eKI_7, "7"},
    {eKI_8, "8"},
    {eKI_9, "9"},
    {eKI_0, "0"},
    {eKI_Minus, "-"},
    {eKI_Equals, "="},
    {eKI_Backspace, "Backspace"},
    {eKI_Tab, "Tab"},
    {eKI_Q, "Q"},
    {eKI_W, "W"},
    {eKI_E, "E"},
    {eKI_R, "R"},
    {eKI_T, "T"},
    {eKI_Y, "Y"},
    {eKI_U, "U"},
    {eKI_I, "I"},
    {eKI_O, "O"},
    {eKI_P, "P"},
    {eKI_LBracket, "["},
    {eKI_RBracket, "]"},
    {eKI_Enter, "Enter"},
    {eKI_LCtrl, "LCtrl"},
    {eKI_A, "A"},
    {eKI_S, "S"},
    {eKI_D, "D"},
    {eKI_F, "F"},
    {eKI_G, "G"},
    {eKI_H, "H"},
    {eKI_J, "J"},
    {eKI_K, "K"},
    {eKI_L, "L"},
    {eKI_Semicolon, ";"},
    {eKI_Apostrophe, "'"},
    {eKI_Tilde, "~"},
    {eKI_LShift, "LShift"},
    {eKI_Backslash, "\\"},
    {eKI_Z, "Z"},
    {eKI_X, "X"},
    {eKI_C, "C"},
    {eKI_V, "V"},
    {eKI_B, "B"},
    {eKI_N, "N"},
    {eKI_M, "M"},
    {eKI_Comma, ","},
    {eKI_Period, "."},
    {eKI_Slash, "/"},
    {eKI_RShift, "RShift"},
    {eKI_NP_Multiply, "NP_Multiply"},
    {eKI_LAlt, "LAlt"},
    {eKI_Space, "Space"},
    {eKI_CapsLock, "CapsLock"},
    {eKI_F1, "F1"},
    {eKI_F2, "F2"},
    {eKI_F3, "F3"},
    {eKI_F4, "F4"},
    {eKI_F5, "F5"},
    {eKI_F6, "F6"},
    {eKI_F7, "F7"},
    {eKI_F8, "F8"},
    {eKI_F9, "F9"},
    {eKI_F10, "F10"},
    {eKI_NumLock, "NumLock"},
    {eKI_ScrollLock, "ScrollLock"},
    {eKI_NP_7, "NP_7"},
    {eKI_NP_8, "NP_8"},
    {eKI_NP_9, "NP_9"},
    {eKI_NP_Substract, "NP_Substract"},
    {eKI_NP_4, "NP_4"},
    {eKI_NP_5, "NP_5"},
    {eKI_NP_6, "NP_6"},
    {eKI_NP_Add, "NP_Add"},
    {eKI_NP_1, "NP_1"},
    {eKI_NP_2, "NP_2"},
    {eKI_NP_3, "NP_3"},
    {eKI_NP_0, "NP_0"},
    {eKI_NP_Period, "NP_Period"},
    {eKI_F11, "F11"},
    {eKI_F12, "F12"},
    {eKI_F13, "F13"},
    {eKI_F14, "F14"},
    {eKI_F15, "F15"},
    {eKI_Colon, "Colon"},
    {eKI_Underline, "Underline"},
    {eKI_NP_Enter, "NP_Enter"},
    {eKI_RCtrl, "RCtrl"},
    {eKI_NP_Period, "NP_Period"},
    {eKI_NP_Divide, "NP_Divide"},
    {eKI_Print, "Print"},
    {eKI_RAlt, "RAlt"},
    {eKI_Pause, "Pause"},
    {eKI_Home, "Home"},
    {eKI_Up, "Up"},
    {eKI_PgUp, "PgUp"},
    {eKI_Left, "Left"},
    {eKI_Right, "Right"},
    {eKI_End, "End"},
    {eKI_Down, "Down"},
    {eKI_PgDn, "PgDn"},
    {eKI_Insert, "Insert"},
    {eKI_Delete, "Delete"},
    {eKI_LWin, "LWin"},
    {eKI_RWin, "RWin"},
    {eKI_Apps, "Apps"},
    {eKI_OEM_102, "OEM_102"},
    {eKI_Mouse1, "Mouse1"},
    {eKI_Mouse2, "Mouse2"},
    {eKI_Mouse3, "Mouse3"},
    {eKI_Mouse4, "Mouse4"},
    {eKI_Mouse5, "Mouse5"},
    {eKI_Mouse6, "Mouse6"},
    {eKI_Mouse7, "Mouse7"},
    {eKI_Mouse8, "Mouse8"},
    {eKI_MouseWheelUp, "MouseWheelUp"},
    {eKI_MouseWheelDown, "MouseWheelDown"},
    {eKI_MouseX, "MouseX"},
    {eKI_MouseY, "MouseY"},
    {eKI_MouseZ, "MouseZ"},
    {eKI_MouseXAbsolute, "MouseXAbsolute"},
    {eKI_MouseYAbsolute, "MouseYAbsolute"},
    {eKI_MouseLast, "MouseLast"},
    {eKI_XI_DPadUp, "XI_DPadUp"},
    {eKI_XI_DPadDown, "XI_DPadDown"},
    {eKI_XI_DPadLeft, "XI_DPadLeft"},
    {eKI_XI_DPadRight, "XI_DPadRight"},
    {eKI_XI_Start, "XI_Start"},
    {eKI_XI_Back, "XI_Back"},
    {eKI_XI_ThumbL, "XI_ThumbL"},
    {eKI_XI_ThumbR, "XI_ThumbR"},
    {eKI_XI_ShoulderL, "XI_ShoulderL"},
    {eKI_XI_ShoulderR, "XI_ShoulderR"},
    {eKI_XI_A, "XI_A"},
    {eKI_XI_B, "XI_B"},
    {eKI_XI_X, "XI_X"},
    {eKI_XI_Y, "XI_Y"},
    {eKI_XI_TriggerL, "XI_TriggerL"},
    {eKI_XI_TriggerR, "XI_TriggerR"},
    {eKI_XI_ThumbLX, "XI_ThumbLX"},
    {eKI_XI_ThumbLY, "XI_ThumbLY"},
    {eKI_XI_ThumbLUp, "XI_ThumbLUp"},
    {eKI_XI_ThumbLDown, "XI_ThumbLDown"},
    {eKI_XI_ThumbLLeft, "XI_ThumbLLeft"},
    {eKI_XI_ThumbLRight, "XI_ThumbLRight"},
    {eKI_XI_ThumbRX, "XI_ThumbRX"},
    {eKI_XI_ThumbRY, "XI_ThumbRY"},
    {eKI_XI_ThumbRUp, "XI_ThumbRUp"},
    {eKI_XI_ThumbRDown, "XI_ThumbRDown"},
    {eKI_XI_ThumbRLeft, "XI_ThumbRLeft"},
    {eKI_XI_ThumbRRight, "XI_ThumbRRight"},
    {eKI_XI_TriggerLBtn, "XI_TriggerLBtn"},
    {eKI_XI_TriggerRBtn, "XI_TriggerRBtn"},
    {eKI_XI_Connect, "XI_Connect"},
    {eKI_XI_Disconnect, "XI_Disconnect"},
    {eKI_Orbis_Select, "Orbis_Select"},
    {eKI_Orbis_L3, "Orbis_L3"},
    {eKI_Orbis_R3, "Orbis_R3"},
    {eKI_Orbis_Start, "Orbis_Start"},
    {eKI_Orbis_Up, "Orbis_Up"},
    {eKI_Orbis_Right, "Orbis_Right"},
    {eKI_Orbis_Down, "Orbis_Down"},
    {eKI_Orbis_Left, "Orbis_Left"},
    {eKI_Orbis_L2, "Orbis_L2"},
    {eKI_Orbis_R2, "Orbis_R2"},
    {eKI_Orbis_L1, "Orbis_L1"},
    {eKI_Orbis_R1, "Orbis_R1"},
    {eKI_Orbis_Triangle, "Orbis_Triangle"},
    {eKI_Orbis_Circle, "Orbis_Circle"},
    {eKI_Orbis_Cross, "Orbis_Cross"},
    {eKI_Orbis_Square, "Orbis_Square"},
    {eKI_Orbis_StickLX, "Orbis_StickLX"},
    {eKI_Orbis_StickLY, "Orbis_StickLY"},
    {eKI_Orbis_StickRX, "Orbis_StickRX"},
    {eKI_Orbis_StickRY, "Orbis_StickRY"},
    {eKI_Orbis_RotX, "Orbis_RotX"},
    {eKI_Orbis_RotY, "Orbis_RotY"},
    {eKI_Orbis_RotZ, "Orbis_RotZ"},
    {eKI_Orbis_RotX_KeyL, "Orbis_RotX_KeyL"},
    {eKI_Orbis_RotX_KeyR, "Orbis_RotX_KeyR"},
    {eKI_Orbis_RotZ_KeyD, "Orbis_RotZ_KeyD"},
    {eKI_Orbis_RotZ_KeyU, "Orbis_RotZ_KeyU"},
    {eKI_Orbis_Touch, "Orbis_Touch"},
    {eKI_SYS_Commit, "SYS_Commit"},
    {eKI_SYS_ConnectDevice, "SYS_ConnectDevice"},
    {eKI_SYS_DisconnectDevice, "SYS_DisconnectDevice"},
    {eKI_Unknown, "Unknown"},
};



#endif //TASTOOLS_INPUTNAMEMAP_H
