#include "minecraft/src-client/common/client/renderer/screen/MinecraftUIRenderContext.h"
#include "minecraft/src-client/common/client/gui/ScreenView.h"

#include "amethyst/events/EventManager.h"
#include "amethyst/InputManager.h"
#include "amethyst/HookManager.h"
#include "amethyst/Log.h"
#include "MinHook.h"

#include <Windows.h>

// Used only for events such as Initialize, Shutdown and RegisterInputs.
#define DllModFn extern "C" __declspec(dllexport)

static ClientInstance* gameClient = nullptr;

// Registers an input into the Minecraft options menu (and it must be a DllModFn). Check here for 0x key codes: https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
DllModFn void RegisterInputs(InputManager* inputManager) { inputManager->RegisterInput("example", 0x56); }

// Hooked into Minecraft::_update() and is called before Minecraft processes a new tick.
void OnTickBefore() {}

// Hooked into Minecraft::_update() and is called after Minecraft processes a new tick.
void OnTickAfter() {}

// ClientInstance::onStartJoinGame
void OnStartJoinGame(ClientInstance* instance) { gameClient = instance; }

// Hooked ScreenView::setupAndRender for mods to draw UI
void OnRenderUI(ScreenView* screenView, MinecraftUIRenderContext* ctx) {}

// Called when a mod is loaded by AmethystRuntime, this is called to create hooks.
DllModFn void Initialize(HookManager* hookManager, Amethyst::EventManager* eventManager, InputManager* inputManager) {
    MH_Initialize();

    Log::Info("[AModTemplate] Mod has been initialized!");

    // To register your functions you must define them above here otherwise the compiler won't recognize them.
    eventManager->onStartJoinGame.AddListener(OnStartJoinGame);
    eventManager->beforeTick.AddListener(OnTickBefore);
    eventManager->afterTick.AddListener(OnTickAfter);
    eventManager->onRenderUI.AddListener(OnRenderUI);

    // Registers your listeners for whenever a button is pressed or released.

    inputManager->AddButtonDownHandler("example", [](FocusImpact f, ClientInstance c) {
        Log::Info("Key has been pressed!");
    });

    inputManager->AddButtonUpHandler("example", [](FocusImpact f, ClientInstance c) { 
        Log::Info("Key has been released!");
    });
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) { return TRUE; }