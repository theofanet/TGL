#pragma once


class ImGuiRenderer {
public:
	static void Init();
	static void Shutdown();

	static void Start();
	static void End();

private:
	static bool s_Initialized;
};