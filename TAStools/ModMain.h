#pragma once
#include <ChairLoader/ModSDK/ChairloaderModBase.h>
#include "InputListener.h"
#include "TAS/TASFile.h"
#include "TAS/TASReplayer.h"


class ModMain : public ChairloaderModBase
{
	using BaseClass = ChairloaderModBase;

	//! Fills in the DLL info during initialization.
	virtual void FillModInfo(ModDllInfoEx& info) override;

    //! Retrieves an interface for the mod.
    void* QueryInterface(const char *ifaceName) override;

	//! Initializes function hooks before they are installed.
	virtual void InitHooks() override;

	//! Called during CSystem::Init, before any engine modules.
	//! Call order: TODO
	virtual void InitSystem(const ModInitInfo& initInfo, ModDllInfo& dllInfo) override;

    //! Called after CSystem::Init, after all engine modules and mods have been initialized. Allows your mod to get interfaces from other mods.
    void Connect(const std::vector<IChairloaderMod *> &mods) override;

private:
    //! Called after CGame::Init
	//! Call order: TODO
	virtual void InitGame(bool isHotReloading) override;

	//! Called before CGame::Shutdown.
	//! Call order: TODO
	virtual void ShutdownGame(bool isHotUnloading) override;

	//! Called before CSystem::Shutdown.
	//! Call order: TODO
	virtual void ShutdownSystem(bool isHotUnloading) override;

	//! Called just before MainUpdate to draw GUI. Only called when GUI is visible.
	virtual void Draw() override;

	//! Earliest point of update in a frame, before CGame::Update. The timer still tracks time for the previous frame.
	 virtual void UpdateBeforeSystem(unsigned updateFlags) override;

	//! Called before physics is updated for the new frame, best point for queuing physics jobs.
	//! This is like FixedUpdate() in Unity (but not FPS-independent). Use gEnv->pTimer->GetFrameTime() for time delta.
	 virtual void UpdateBeforePhysics(unsigned updateFlags) override;

	//! Called after entities have been updated but before FlowSystem and FlashUI.
	//! This is the main update where most game logic is expected to occur.
	//! Should be preferred if you don't need any special behavior.
	virtual void MainUpdate(unsigned updateFlags) override;

	//! Called after most of game logic has been updated, before CCryAction::PostUpdate.
	 virtual void LateUpdate(unsigned updateFlags) override;

    void pauseUpdate(bool pause);
public:

    bool m_bDrawGUI = false;
    bool m_bStep = false;
    bool m_bPause = false;

    bool m_bRecordingInputs = false;
    bool m_bVerifyingInputs = true;
    bool m_bFUCK = false;
    uint64_t m_frameCount = 0;
    std::unique_ptr<InputListener> m_inputRecorder;
    std::unique_ptr<TASFile> m_tasFile;
    std::unique_ptr<TASReplayer> m_tasReplayer;
    void DrawMenuBar();
};

extern ModMain* gMod;
