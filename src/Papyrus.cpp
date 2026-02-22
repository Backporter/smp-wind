#include "Config.h"
#include "Wind.h"

namespace wind
{
	extern Config g_config;
	extern Config g_configDefault;
	extern Wind   g_wind;

	static bool getBool(RE::StaticFunctionTag*, int32_t id)
	{
		if (id >= 0 && id < Config::BOOL_COUNT) {
			return g_config.getb(id);
		}
		else {
			return false;
		}
	}

	static bool getBoolDefault(RE::StaticFunctionTag*, int32_t id)
	{
		if (id >= 0 && id < Config::BOOL_COUNT) {
			return g_configDefault.getb(id);
		}
		else {
			return false;
		}
	}

	static void setBool(RE::StaticFunctionTag*, int32_t id, bool b)
	{
		if (id >= 0 && id < Config::BOOL_COUNT) {
			g_config.set(id, b);
		}
	}

	static float getFloat(RE::StaticFunctionTag*, int32_t id)
	{
		if (id >= 0 && id < Config::FLOAT_COUNT) {
			return g_config.getf(id);
		}
		else {
			return 0.0f;
		}
	}

	static float getFloatDefault(RE::StaticFunctionTag*, int32_t id)
	{
		if (id >= 0 && id < Config::FLOAT_COUNT) {
			return g_configDefault.getf(id);
		}
		else {
			return 0.0f;
		}
	}

	static void setFloat(RE::StaticFunctionTag*, int32_t id, float f)
	{
		if (id >= 0 && id < Config::FLOAT_COUNT) {
			g_config.set(id, f);
		}
	}

	static int32_t getInt(RE::StaticFunctionTag*, int32_t id)
	{
		if (id >= 0 && id < Config::INT_COUNT) {
			return g_config.geti(id);
		}
		else {
			return 0;
		}
	}

	static int32_t getIntDefault(RE::StaticFunctionTag*, int32_t id)
	{
		if (id >= 0 && id < Config::INT_COUNT) {
			return g_configDefault.geti(id);
		}
		else {
			return 0;
		}
	}

	static void setInt(RE::StaticFunctionTag*, int32_t id, int32_t i)
	{
		if (id >= 0 && id < Config::INT_COUNT) {
			g_config.set(id, static_cast<int>(i));
		}

		if (id == Config::THREADS) {
			wind::g_wind.updateThreadCount();
		}
	}

	bool Register(RE::BSScript::Internal::VirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("GetBool", "JGWD_MCM", getBool, true);
		a_vm->RegisterFunction("GetBoolDefault", "JGWD_MCM", getBoolDefault, true);
		a_vm->RegisterFunction("GetFloat", "JGWD_MCM", getFloat, true);
		a_vm->RegisterFunction("GetFloatDefault", "JGWD_MCM", getFloatDefault, true);
		a_vm->RegisterFunction("GetInt", "JGWD_MCM", getInt, true);
		a_vm->RegisterFunction("GetIntDefault", "JGWD_MCM", getIntDefault, true);
		a_vm->RegisterFunction("SetBool", "JGWD_MCM", setBool);
		a_vm->RegisterFunction("SetFloat", "JGWD_MCM", setFloat);
		a_vm->RegisterFunction("SetInt", "JGWD_MCM", setInt);
		
		return true;
	}
}
