#include "PluginHelper.h"
#include "Config.h"
#include "Wind.h"
#include "Papyrus.h"

constexpr unsigned long VERSION_MAJOR{ 2 };
constexpr unsigned long VERSION_MINOR{ 3 };
constexpr unsigned long VERSION_PATCH{ 0 };
constexpr unsigned long VERSION = (VERSION_MAJOR & 0xFF) << 24 | (VERSION_MINOR & 0xFF) << 16 | (VERSION_PATCH & 0xFF) << 8;

inline static hdt::PluginInterface::Version interfaceMin{ 1, 0, 0 };
inline static hdt::PluginInterface::Version interfaceMax{ 3, 0, 0 };

inline static hdt::PluginInterface::Version bulletMin{ hdt::PluginInterface::BULLET_VERSION };
inline static hdt::PluginInterface::Version bulletMax{ hdt::PluginInterface::BULLET_VERSION.major + 1, 0, 0 };

namespace wind
{
	Config g_config;
	Config g_configDefault;
	Wind   g_wind;
}

namespace
{
	void InitializeLog()
	{
#ifndef NDEBUG
		auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
		auto path = logger::log_directory();
		if (!path) {
			util::report_and_fail("Failed to find standard logging directory"sv);
		}

		*path /= fmt::format("{}.log"sv, Plugin::NAME);
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

#ifndef NDEBUG
		const auto level = spdlog::level::trace;
#else
		const auto level = spdlog::level::info;
#endif

		//
		auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));
		log->set_level(level);
		log->flush_on(level);

		spdlog::set_default_logger(std::move(log));
		spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);
	}
}

void SMP_MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg && a_msg->type == hdt::PluginInterface::MSG_STARTUP && a_msg->data) 
	{
		auto* smp = static_cast<hdt::PluginInterface*>(a_msg->data);
		const auto& info = smp->getVersionInfo();

		//
		if (info.interfaceVersion >= interfaceMin && info.interfaceVersion < interfaceMax) 
		{
			if (info.bulletVersion >= bulletMin && info.bulletVersion < bulletMax) 
			{
				logger::info(".\n");

				//
				logger::info("Loading settings...");
				if (wind::g_config.load("SKSE\\Plugins\\SMP Wind.ini")) 
				{
					logger::info("Settings loaded.\n");
				} 
				else 
				{
					logger::warn("WARNING: Failed to load config file. Settings will not be saved.\n");
				}
		
				wind::g_wind.init(wind::g_config);
				smp->addListener(&wind::g_wind);
				logger::info("Initialisation complete.\n");
			} 
			else 
			{
				logger::error("ERROR: Incompatible Bullet version.");
			}
		} 
		else 
		{
			logger::error("ERROR: Incompatible HDT-SMP interface.");
		}
	}
}

void SKSE_MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	if (a_msg && a_msg->type == SKSE::MessagingInterface::kPostPostLoad)
	{
		const auto messaging = SKSE::GetMessagingInterface();
		if (messaging)
		{
			messaging->RegisterListener("hdtSMP64", SMP_MessageHandler);
		}
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = Plugin::NAME.data();
	a_info->version = Plugin::VERSION.pack();

	if (a_skse->IsEditor()) 
	{
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	return true;
}

extern "C" DLLEXPORT constinit auto SKSEPlugin_Version = []() 
{
	SKSE::PluginVersionData v;

	v.PluginVersion(Plugin::VERSION);
	v.PluginName(Plugin::NAME);
	v.UsesAddressLibrary();
	v.CompatibleVersions({ SKSE::RUNTIME_SSE_LATEST });
	v.UsesNoStructs();

	return v;
}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

	InitializeLog();
	logger::info("{} v{}"sv, Plugin::NAME, Plugin::VERSION.string());

	const auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", SKSE_MessageHandler)) 
	{
		return false;
	}

	const auto papyrus = SKSE::GetPapyrusInterface();
	if (papyrus) 
	{
		papyrus->Register(wind::Register);
	}

	return true;
}
