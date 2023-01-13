#include "ModMain.h"
#include <Prey/CryInput/IInput.h>
#include <Prey/CryInput/BaseInput.h>
#include <Prey/CrySystem/System.h>
#include <Prey/CryGame/IGameFramework.h>
#include <Prey/CryGame/Game.h>
#include <Prey/CryInput/IHardwareMouse.h>
#include <Prey/CryInput/mouse.h>
#include <Prey/GameDll/ark/player/ArkPlayer.h>
#include <Chairloader/IRenderAuxGeomEx.h>
#include "InputNameMap.h"


ModMain* gMod = nullptr;

auto CMouseHook = CMouse::FUpdate.MakeHook();
void CMouseHookFunc(CMouse* _this, bool bFocus) {
    if(gMod) {
        if (gMod->m_bFUCK) {
            gMod->m_tasFile->m_TASActions.executeFrame();
        }
        gMod->m_tasReplayer->Update();
    }
    CMouseHook.InvokeOrig(_this, bFocus);
}

#ifdef EXAMPLE
// You can define PreyDll.dll function using PreyFunction
// auto fnFuncName = PreyFunction<void(T funcArgs)>(functionOffset);
// Many functions are defined as static members with F prefix:
//  ArkPlayer.h
//     static inline auto FHasAbility = PreyFunction<bool(ArkPlayer* _this, uint64_t _abilityID)>(0x1550330);
static auto fn_ArkPlayer_HasAbility = PreyFunction<bool(ArkPlayer* _this, uint64_t _abilityID)>(0x1550330);

// Use MakeHook() method of PreyFunction to create a hook.
// See ModMain::InitHooks for more.
static auto s_hook_ArkPlayer_HasAbility = fn_ArkPlayer_HasAbility.MakeHook();

// This function will be called in place of ArkPlayer::HasAbility.
// See ModMain::InitHooks for more.
static bool ArkPlayer_HasAbility_Hook(ArkPlayer* _this, uint64_t _abilityID)
{
	// NOTE: This particular method of ability unlocking is unreliable.
	// NOTE: It's only used as an example of function hooking.

	if (_abilityID % 2 == 0)
	{
		// Player always has abilities whose ID is even.
		return true;
	}

	// Use InvokeOrig of PreyFunctionHook to call the original (non-hooked) function.
	return s_hook_ArkPlayer_HasAbility.InvokeOrig(_this, _abilityID);
}

#endif

void ModMain::FillModInfo(ModDllInfoEx& info)
{
	info.modName = "thelivingdiamond.TAStools"; // CHANGE ME
	info.logTag = "TAStools"; // CHANGE ME
	info.supportsHotReload = true; // TODO: Add comment/wiki link
}

void ModMain::InitHooks()
{
#ifdef EXAMPLE
	// Functions hooks are intalled early into mod loading process,
	// before any engine subsystem is initialized.
	// But for hook to succeed PreyFunctionHook needs to know the hook function
	// (that will be called in place of original one)
	// That's why ModMain::InitHooks exists.
	// Call SetHookFunc from here.
	s_hook_ArkPlayer_HasAbility.SetHookFunc(&ArkPlayer_HasAbility_Hook);
#endif
    CMouseHook.SetHookFunc(&CMouseHookFunc);
}

void ModMain::InitSystem(const ModInitInfo& initInfo, ModDllInfo& dllInfo)
{
	BaseClass::InitSystem(initInfo, dllInfo);
	// Your code goes here
}

void ModMain::InitGame(bool isHotReloading)
{
	BaseClass::InitGame(isHotReloading);
    m_inputRecorder = std::make_unique<InputListener>();
    m_inputRecorder->registerListener();

    m_tasFile = std::make_unique<TASFile>();
    m_tasReplayer = std::make_unique<TASReplayer>();
}

void ModMain::ShutdownGame(bool isHotUnloading)
{
	// Your code goes here
	BaseClass::ShutdownGame(isHotUnloading);
    m_inputRecorder.reset();
}

void ModMain::ShutdownSystem(bool isHotUnloading)
{
	// Your code goes here
	BaseClass::ShutdownSystem(isHotUnloading);
}

void ModMain::DrawMenuBar(){
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("TAStools"))
        {
            ImGui::MenuItem("Show", nullptr, &m_bDrawGUI);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}


void ModMain::Draw()
{
    if(ImGui::IsKeyPressed(ImGuiKey_LeftBracket)){
        m_bFUCK = !m_bFUCK;
    }
    if(ImGui::IsKeyPressed(ImGuiKey_RightBracket)){
        m_tasFile->m_TASActions.resetFrameNumber();
    }
    if(ImGui::IsKeyPressed(ImGuiKey_Backslash)){
        m_bRecordingInputs = !m_bRecordingInputs;
    }
    ImGui::BeginDisabled(m_bRecordingInputs | m_bFUCK);

	DrawMenuBar();
    if(!m_bDrawGUI) {
        ImGui::EndDisabled();
        return;
    }

    ImGui::SetNextWindowBgAlpha(1.0f);
	if (ImGui::Begin("TAS Tools", &m_bDrawGUI)){
        auto input = (CBaseInput*)gEnv->pInput;
        ImGui::Text("Input");
        ImGui::Checkbox("FUCK", &m_bFUCK);
        ImGui::Checkbox("Record", &m_bRecordingInputs);
        auto system = (CSystem*)gEnv->pSystem;
        ImGui::Text("NoUpdate: %u", system->m_bNoUpdate);
        ImGui::Text("Paused: %u", gEnv->pGame->GetIGameFramework()->IsGamePaused());
        ImGui::Text("Death Countdown: %llu", m_inputRecorder->getEventCount());
        if(ImGui::Button("No Update")) {
            pauseUpdate(true);
        }
        if(ImGui::Button("Update")) {
            pauseUpdate(false);
        }
        //step
        if(ImGui::Button("Step")) {
            m_bStep = true;
        }
        static std::string inputFileName = "inputs.txt";
        ImGui::InputText("Input File", &inputFileName);
        if(ImGui::Button("Load Inputs")){
            m_tasFile->loadTASFile(inputFileName);
        }
        if(ImGui::Button("Save Inputs")){
            m_tasFile->saveTASFile(inputFileName);
        }
        if(ImGui::Button("Clear Inputs")){
            m_tasFile->m_TASActions.clear();
        }
        ImGui::Text("Frame: %llu", m_tasFile->m_TASActions.m_currentFrameNumber);
        static int clipLimit = 100;
        ImGui::InputInt("Clip Limit", &clipLimit);
        int clip = 0;
        if(ImGui::BeginTable("Inputs", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY)){
            ImGui::TableSetupColumn("Frame", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Input");
            ImGui::TableHeadersRow();
            for (auto &frame: m_tasFile->m_TASActions.m_frames) {
                if(clip > clipLimit) {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Text("...");
                    break;
                }
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("Frame %llu", frame.m_frameNumber);
                ImGui::TableNextColumn();
                if(frame.empty()){
                    ImGui::Text("No Inputs");
                }
                for(auto& action: frame.m_actions){
                    switch(action->getType()){
                        case Action::Type::KEYBOARD:
                            ImGui::Text("Key: %s [%i]", gEnv->pInput->GetKeyName(static_cast<KeyboardAction*>(action)->m_keyId), static_cast<KeyboardAction*>(action)->m_keyState);
                            break;
                        case Action::Type::MOUSE:
                            ImGui::Text("XPos: %f, YPos: %f, %s", static_cast<MouseAction*>(action)->m_xPos, static_cast<MouseAction*>(action)->m_yPos, static_cast<MouseAction*>(action)->m_Absolute ? "Absolute" : "Relative");
                            ImGui::Text("M1: %i\n"
                                        "M2: %i\n"
                                        "M3: %i\n"
                                        "M4: %i\n"
                                        "M5: %i\n",
                                        static_cast<MouseAction*>(action)->m_leftButton,
                                        static_cast<MouseAction*>(action)->m_rightButton,
                                        static_cast<MouseAction*>(action)->m_middleButton,
                                        static_cast<MouseAction*>(action)->m_xButton1,
                                        static_cast<MouseAction*>(action)->m_xButton2);
                            break;
                        case Action::Type::CVAR:
                            ImGui::Text("CVar: %s, Value: %s", static_cast<CVarAction*>(action)->m_cvarName.c_str(), static_cast<CVarAction*>(action)->m_cvarValue.c_str());
                            break;
                        case Action::Type::COMMENT:
                            ImGui::Text("Comment: %s", static_cast<CommentAction*>(action)->m_comment.c_str());
                            break;
                    }
                }
                clip++;
            }
            ImGui::EndTable();
        }

	}
	ImGui::End();
    if(ImGui::Begin("Debug")){
        if(ImGui::Button("Test Add Player Position Frame")){
            m_tasFile->m_TASPositions.addFrame(ArkPlayer::GetInstancePtr()->GetEntity());
        }
        for(auto& position : m_tasFile->m_TASPositions.m_frames){
            ImGui::Text("Position: %f, %f, %f", position.m_position.x, position.m_position.y, position.m_position.z);
            ImGui::Text("Rotation: %f, %f, %f, %f", position.m_rotation.v.x, position.m_rotation.v.y, position.m_rotation.v.z, position.m_rotation.w);
            ImGui::Text("Velocity: %f, %f, %f", position.m_velocity.x, position.m_velocity.y, position.m_velocity.z);
        }
    }
    ImGui::End();
    ImGui::EndDisabled();
}

void ModMain::MainUpdate(unsigned updateFlags)
{
}



void *ModMain::QueryInterface(const char *ifaceName) {
#ifdef EXAMPLE
    // this is used to return an interface for your mod, if available.
    // Your mod class should inherit from the interface class. i.e: class ModMain : public ChairloaderModBase, public IExampleMod {
    // Then you can return the interface pointer here.
    if (!strcmp(ifaceName, "ExampleMod"))
        return static_cast<IExampleMod*>(this);
    // If you have multiple interfaces, you can return as many as you want for even potentially different objects.
    // if you don't have an interface, just return nullptr.
#endif
    return nullptr;
}

void ModMain::Connect(const std::vector<IChairloaderMod *> &mods) {
#ifdef EXAMPLE
    // Example of how to get a mod interface from the list of mods
    IOtherMod* otherMod = nullptr;
    for (auto & mod: mods) {
        otherMod = mod->QueryInterface("IOtherMod001"); // the interface name is defined in the other mod
        if (otherMod) {
            break;
        }
    }

    // do something with otherMod
#endif
}

void ModMain::pauseUpdate(bool pause) {
    auto pGameFw = gEnv->pGame->GetIGameFramework();
    if(pause != pGameFw->IsGamePaused())
        pGameFw->PauseGame(pause, true, 0, pause);
    m_bPause = pause;
}

void ModMain::UpdateBeforeSystem(unsigned int updateFlags) {
    if(!m_bRecordingInputs) {
        Vec3 last_position(ZERO);
        int i = 0;
        static const int frameSkip = 5;
        for (auto &position: m_tasFile->m_TASPositions.m_frames) {
            if (last_position.IsZero() || last_position == position.m_position) {
                last_position = position.m_position;
                continue;
            }
            if(i % frameSkip != 0) {
                i++;
                continue;
            }
            gEnv->pAuxGeomRenderer->DrawLine(last_position, ColorB(255, 0, 0, 255), position.m_position, ColorB(255, 0, 0, 255));
            last_position = position.m_position;
            i++;
//        gEnv->pAuxGeomRenderer->DrawSphere(position.m_position, 0.001f, ColorB(255, 0, 0, 255), false);
//        gEnv->pAuxGeomRenderer->DrawLine(position.m_position, ColorB(255, 0, 0, 255), position.m_position + position.m_velocity * 0.1f, ColorB(255, 255, 0, 255));
        }
    }
}

void ModMain::LateUpdate(unsigned int updateFlags) {
    if(m_bRecordingInputs) {
        std::vector<SInputEvent> events;
        m_inputRecorder->getEvents(events);
        m_tasFile->m_TASActions.addFrame(events);
        m_tasFile->m_TASPositions.addFrame(ArkPlayer::GetInstancePtr()->GetEntity());
    } else if(m_bFUCK && m_bVerifyingInputs) {
        std::vector<SInputEvent> events;
        m_inputRecorder->getEvents(events);
        if(!m_tasFile->m_TASActions.verifyFrame(events) && m_tasFile->m_TASActions.m_currentFrameNumber < m_tasFile->m_TASActions.m_frameCount) {
            CryError("Input verification failed! {}", m_tasFile->m_TASActions.m_currentFrameNumber);
        }
    } else {
        m_inputRecorder->update();
    }
}

void ModMain::UpdateBeforePhysics(unsigned int updateFlags) {
    //TODO:

    static int frameTimer = 0;
    static const int frameLimit = 3;
    static const int framesPressed = 2;
    if(false){
        //! WORKS!!!!!!!!!
      /*  ArkPlayer* player = ArkPlayer::GetInstancePtr();
        ArkPlayerInput* playerInput;
        if(player== nullptr)
            return;
        playerInput = &player->m_input;
        playerInput->OnActionRotatePitch(player->GetEntityId(), CCryName("rotatepitch"), 8, 2.0f);
        playerInput->OnActionRotateYaw(player->GetEntityId(), CCryName("rotateyaw"), 8, 2.0f);*/
      //! ALSO WORKS!!!!!!
        SInputEvent event{};
        //EInputDeviceType deviceType;
        //	EInputState state;
        //	wchar_t inputChar;
        //	TKeyName keyName;
        //	EKeyId keyId;
        //	int modifiers;
        //	float value;
        //	SInputSymbol *pSymbol;
        //	unsigned __int8 deviceIndex;
        /*event.deviceType = eIDT_Mouse;
        event.state = eIS_Changed;
        event.inputChar = L' ';
        event.keyName.key = "maxis_x";
        event.keyId = eKI_MouseX;
        event.pSymbol = nullptr;
        event.modifiers = 0;
        event.value = 2.0f;
        event.deviceIndex = 0;
        gEnv->pInput->PostInputEvent(&event, false);*/
        if(frameTimer > (frameLimit - framesPressed)) {
            SInputEvent event2{};
            event2.deviceType = eIDT_Mouse;
            event2.state = eIS_Pressed;
            event2.inputChar = L' ';
            event2.keyName.key = gEnv->pInput->GetKeyName(eKI_Mouse1);
            event2.keyId = eKI_Mouse1;
            event2.pSymbol = nullptr;
            event2.modifiers = 0;
            event2.value = 2.0f;
            event2.deviceIndex = 0;
            gEnv->pInput->PostInputEvent(&event2, false);
            if(frameTimer >= frameLimit){
                frameTimer = 0;
            } else {
                frameTimer++;
            }
        } else if (frameTimer == 0){
            SInputEvent event2{};
            event2.deviceType = eIDT_Mouse;
            event2.state = eIS_Released;
            event2.inputChar = L' ';
            event2.keyName.key = gEnv->pInput->GetKeyName(eKI_Mouse1);
            event2.keyId = eKI_Mouse1;
            event2.pSymbol = nullptr;
            event2.modifiers = 0;
            event2.value = 2.0f;
            event2.deviceIndex = 0;
            gEnv->pInput->PostInputEvent(&event2, false);
            frameTimer++;
        } else {
            frameTimer++;
        }

    }
}


extern "C" DLL_EXPORT IChairloaderMod* ClMod_Initialize()
{
	CRY_ASSERT(!gMod);
	gMod = new ModMain();
	return gMod;
}

extern "C" DLL_EXPORT void ClMod_Shutdown()
{
	CRY_ASSERT(gMod);
	delete gMod;
	gMod = nullptr;
}

// Validate that declarations haven't changed
static_assert(std::is_same_v<decltype(ClMod_Initialize), IChairloaderMod::ProcInitialize>);
static_assert(std::is_same_v<decltype(ClMod_Shutdown), IChairloaderMod::ProcShutdown>);
