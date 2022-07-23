#pragma once

class ImGuiRenderer {
public:
	static void Init();
	static void Shutdown();

	static void Start();
	static void End();

	static void StartWorkspace(bool &dockOpen, bool fullScr = true);
	static void EndWorkSpace();

private:
	static bool s_Initialized;
};