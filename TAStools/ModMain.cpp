#include "ModMain.h"
#include <Prey/CryInput/IInput.h>
#include <Prey/CryInput/BaseInput.h>
#include <Prey/CrySystem/System.h>
#include <Prey/CryGame/IGameFramework.h>
#include <Prey/CryGame/Game.h>
#include <Prey/CryInput/IHardwareMouse.h>
#include <Prey/CryInput/mouse.h>
#include <Prey/GameDll/ark/player/ArkPlayer.h>


ModMain* gMod = nullptr;

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
}

void ModMain::InitSystem(const ModInitInfo& initInfo, ModDllInfo& dllInfo)
{
	BaseClass::InitSystem(initInfo, dllInfo);
	// Your code goes here
}

void ModMain::InitGame(bool isHotReloading)
{
	BaseClass::InitGame(isHotReloading);
    m_inputRecorder = std::make_unique<InputRecorder>();
    m_inputRecorder->registerListener();
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
	// Modders, please move to a method, it's just an example
	DrawMenuBar();
    if(!m_bDrawGUI)
        return;

	if (ImGui::Begin("TAS tools", &m_bDrawGUI)){
        auto input = (CBaseInput*)gEnv->pInput;
        ImGui::Text("Input");
        ImGui::Text("Event Posting Enabled: %s", input->m_enableEventPosting ? "true" : "false");
        if(ImGui::Button("Toggle Event Posting")){
            input->m_enableEventPosting = !input->m_enableEventPosting;

        }
        ImGui::Checkbox("FUCK", &m_bFUCK);
        auto system = (CSystem*)gEnv->pSystem;

        ImGui::Text("NoUpdate: %u", system->m_bNoUpdate);
        ImGui::Text("Paused: %u", gEnv->pGame->GetIGameFramework()->IsGamePaused());
        if(ImGui::Button("No Update")){
            pauseUpdate(true);
        }
        if(ImGui::Button("Update")){
            pauseUpdate(false);
        }
        //step
        if(ImGui::Button("Step")){
            m_bStep = true;
        }

	}

	ImGui::End();
}

void ModMain::MainUpdate(unsigned updateFlags)
{
    if(ImGui::IsKeyPressed(ImGuiKey_LeftBracket)){
        m_bStep = true;
    }
    if(m_bStep){
        m_bStep = false;
        pauseUpdate(false);
    } else {
        pauseUpdate(m_bPause);
    }
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
}

void ModMain::LateUpdate(unsigned int updateFlags) {
}

void ModMain::UpdateBeforePhysics(unsigned int updateFlags) {
    if(m_bFUCK){
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
        event.deviceType = eIDT_Mouse;
        event.state = eIS_Changed;
        event.inputChar = L' ';
        event.keyName.key = "maxis_x";
        event.keyId = eKI_Space;
        event.pSymbol = nullptr;
        event.modifiers = 0;
        event.value = 2.0f;
        event.deviceIndex = 0;
        gEnv->pInput->PostInputEvent(&event, false);

        SInputEvent event2{};
        event2.deviceType = eIDT_Mouse;
        event2.state = eIS_Changed;
        event2.inputChar = L' ';
        event2.keyName.key = "maxis_y";
        event2.keyId = eKI_Space;
        event2.pSymbol = nullptr;
        event2.modifiers = 0;
        event2.value = 2.0f;
        event2.deviceIndex = 0;
        gEnv->pInput->PostInputEvent(&event2, false);

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
