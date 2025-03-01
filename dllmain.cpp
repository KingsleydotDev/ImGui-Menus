#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <thread>
#include <cstdint>

#include "hooks.h"

void Setup(const HMODULE instance)
{
	try
	{
		AllocConsole();
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout); // Gimme cout logs

		gui::Setup();
		hooks::Setup();


	}
	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(
			0,
			error.what(),
			"Error",
			MB_OK | MB_ICONEXCLAMATION
		);

		goto UNLOAD;
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

UNLOAD:
	hooks::Destroy();
	gui::Destroy();

	FreeLibraryAndExitThread(instance, 0);

}


BOOL WINAPI DllMain(
	const HMODULE instance,
	const std::uintptr_t reason,
	const void* reserved
)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(instance);

		const auto thread = CreateThread(
			nullptr,
			0,
			reinterpret_cast<LPTHREAD_START_ROUTINE>(Setup),
			instance,
			0,
			nullptr
		);

		if (thread)
			CloseHandle(thread);
	}

	return true;
}

