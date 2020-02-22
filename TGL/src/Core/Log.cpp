#include "tglpch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"


std::shared_ptr<spdlog::logger> Log::s_Instance;

void Log::Init() {
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_Instance = spdlog::stdout_color_mt("TGL");
	s_Instance->set_level(spdlog::level::trace);
}