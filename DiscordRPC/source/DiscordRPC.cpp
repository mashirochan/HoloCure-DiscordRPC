#include <YYToolkit/Shared.hpp>
#include "DiscordSDK/discord.h"

#include <map>
#include <random>

using namespace Aurie;
using namespace YYTK;

static YYTKInterface* g_ModuleInterface = nullptr;

/*
		Inline Functions
*/
template <typename... TArgs>
inline static void Print(CmColor Color, std::string_view Format, TArgs&&... Args) {
	g_ModuleInterface->Print(Color, Format, Args...);
}

template <typename... TArgs>
inline static void PrintError(std::string_view Filepath, int Line, std::string_view Format, TArgs&&... Args) {
	g_ModuleInterface->PrintError(Filepath, Line, Format, Args...);
}

inline static RValue CallBuiltin(const char* FunctionName, std::vector<RValue> Arguments) {
	return g_ModuleInterface->CallBuiltin(FunctionName, Arguments);
}

/*
		GML Inline Functions
*/
inline static RValue variable_instance_get(CInstance* instance_id, const char* name) {
	RValue id, result;
	AurieStatus status = g_ModuleInterface->GetBuiltin("id", instance_id, NULL_INDEX, id);
	if (AurieSuccess(status)) {
		result = CallBuiltin("variable_instance_get", { id, name });
	}
	return result;
}

inline static RValue variable_global_get(const char* name) {
	return CallBuiltin("variable_global_get", { name });
}

/*
		Discord SDK Variables
*/
constexpr auto APPLICATION_ID = 1151654619244666951;
discord::Core* core{};
discord::Result result;
std::string current_state;
int UUUUUUUChance = 10;
std::map<double, std::pair<std::string, std::string>> stage_map;

static void LogProblemsFunction(discord::LogLevel level, std::string message) {
	PrintError(__FILE__, __LINE__, "Discord: %d - %s", static_cast<int>(level), message.c_str());
}

std::random_device rd;
std::mt19937 gen(rd());

static std::string GetRandomTitleIcon() {
	std::uniform_int_distribution<int> distribution(1, 6);
	int randomValue = distribution(gen);
	std::string randomStr = "titlescreen_icon_" + std::to_string(randomValue);
	return randomStr;
}

static std::string GetFormattedCharName(std::string charName) {
	// Special Cases
	if (charName == "AZKi") return "AZKi";
	if (charName == "IRYS") return "IRyS";
	if (charName == "NINOMAE INANIS") return "Ninomae Ina'nis";

	bool newWord = true;

	for (char& c : charName) {
		if (newWord && std::isalpha(c)) {
			c = std::toupper(c);
			newWord = false;
		} else {
			c = std::tolower(c);
		}

		if (std::isspace(c)) {
			newWord = true;
		}
	}

	return charName;
}

static std::string ConvertToIconName(const std::string& input) {
	std::string str = input;
	str.erase(std::remove(str.begin(), str.end(), '('), str.end());
	str.erase(std::remove(str.begin(), str.end(), ')'), str.end());
	std::replace(str.begin(), str.end(), ' ', '_');
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	str += "_icon";
	return str;
}

/*
		Frame Callback Event
*/
static AurieStatus FrameCallback(
	IN FWFrame& FrameContext
)
{
	UNREFERENCED_PARAMETER(FrameContext);

	// As long as Discord API is saying it's okay, run API Callbacks
	if (result == discord::Result::Ok) {
		result = ::core->RunCallbacks();
	}

	return AURIE_SUCCESS;
}

/*
		Code Callback Events
*/
static AurieStatus CodeCallback(
	IN FWCodeEvent& CodeContext
)
{
	// Call original first
	CodeContext.Call();

	// Get the necessary arguments from the Code Event
	CInstance* Self = std::get<0>(CodeContext.Arguments());
	CCode* Code = std::get<2>(CodeContext.Arguments());

	// Check if CCode argument has a valid name
	if (!Code->GetName()) {
		PrintError(__FILE__, __LINE__, "[DiscordRPC] - Failed to find Code Event name!");
		return AURIE_MODULE_INTERNAL_ERROR;
	}

	// Cast Name arg to an std::string for easy usage
	std::string Name = Code->GetName();

	/*
			Title Screen Create Event
	*/
	if (Name == "gml_Object_obj_TitleScreen_Create_0") {
		// Create and set a new Discord Activity saying we're on the Title Screen
		if (result == discord::Result::Ok && current_state != "title") {
			current_state = "title";
			discord::Activity activity{};
			activity.SetState("On Title Screen");
			std::time_t current_time;
			std::time(&current_time);
			activity.GetTimestamps().SetStart((int)current_time);
			activity.GetAssets().SetLargeImage(GetRandomTitleIcon().c_str());
			activity.GetAssets().SetLargeText("On Title Screen");
			core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
		}
	}
	/*
			Player Manager Create Event
	*/
	else if (Name == "gml_Object_obj_PlayerManager_Create_0") {
		if (result == discord::Result::Ok && current_state != "stage") {
			current_state = "stage";
			
			// Get the "charName" instance variable from the PlayerManager object - this is the name of the Holomem we're currently playing as
			std::string char_name(variable_instance_get(Self, "charName").AsString());

			std::string state_str = "Playing " + GetFormattedCharName(char_name);
			std::string char_icon_str = ConvertToIconName(char_name);

			// Get the "bgmPlay" global variable - this is the index of the currently playing BGM
			double bgm_play = variable_global_get("bgmPlay").AsReal();

			// Get the "gameMode" global variable - this is what game mode we're currently playing
			double game_mode = variable_global_get("gameMode").AsReal();
			std::string game_mode_str = "";
			if (game_mode == 1) game_mode_str = " - Endless";
			std::string stage_name = stage_map[bgm_play].first + game_mode_str;

			// Create and set a Discord Activity saying various info about our current run
			discord::Activity activity{};
			activity.SetState(state_str.c_str());
			activity.SetDetails(stage_name.c_str());
			std::time_t currentTime;
			std::time(&currentTime);
			activity.GetTimestamps().SetStart((int)currentTime);
			activity.GetAssets().SetLargeImage(ConvertToIconName(stage_map[bgm_play].first).c_str());
			activity.GetAssets().SetLargeText(stage_map[bgm_play].second.c_str());
			activity.GetAssets().SetSmallImage(char_icon_str.c_str());
			// UUUUUUUUUUUUUU
			if (char_name == "CERES FAUNA") {
				std::uniform_int_distribution<int> distribution(1, UUUUUUUChance);
				int random_value = distribution(gen);
				if (random_value == 1) {
					activity.GetAssets().SetSmallText("UUUUUUU");
				} else {
					activity.GetAssets().SetSmallText(GetFormattedCharName(char_name).c_str());
				}
			} else {
				activity.GetAssets().SetSmallText(GetFormattedCharName(char_name).c_str());
			}
			core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
		}
	}
	/*
			HoloHouse Manager Create Event
	*/
	else if (Name == "gml_Object_obj_HoloHouseManager_Create_0") {
		if (result == discord::Result::Ok && current_state != "house") {
			current_state = "house";

			// Get the "charName" instance variable from the PlayerManager object - this is the name of the Holomem we're currently playing as
			std::string char_name(variable_instance_get(Self, "charName").AsString());

			std::string state_str = "Playing " + GetFormattedCharName(char_name);
			std::string char_icon_str = ConvertToIconName(char_name);

			// Create and set a new Discord Activity saying we're in the Holo House and who we're playing
			discord::Activity activity{};
			activity.SetState(state_str.c_str());
			activity.SetDetails("In Holo House");
			std::time_t current_time;
			std::time(&current_time);
			activity.GetTimestamps().SetStart((int)current_time);
			activity.GetAssets().SetLargeImage("holohouse_icon");
			activity.GetAssets().SetLargeText("Main Entrance");
			activity.GetAssets().SetSmallImage(char_icon_str.c_str());
			// UUUUUUUUUUUUUU
			if (char_name == "CERES FAUNA") {
				std::uniform_int_distribution<int> distribution(1, UUUUUUUChance);
				int randomValue = distribution(gen);
				if (randomValue == 1) {
					activity.GetAssets().SetSmallText("UUUUUUU");
				} else {
					activity.GetAssets().SetSmallText(GetFormattedCharName(char_name).c_str());
				}
			} else {
				activity.GetAssets().SetSmallText(GetFormattedCharName(char_name).c_str());
			}
			core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});
		}
	}

	return AURIE_SUCCESS;
}

EXPORTED AurieStatus ModuleInitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	UNREFERENCED_PARAMETER(ModulePath);

	AurieStatus last_status = AURIE_SUCCESS;

	// Gets a handle to the interface exposed by YYTK
	// You can keep this pointer for future use, as it will not change unless YYTK is unloaded.
	last_status = ObGetInterface(
		"YYTK_Main",
		(AurieInterfaceBase*&)(g_ModuleInterface)
	);

	// If we can't get the interface, we fail loading.
	if (!AurieSuccess(last_status))
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;

	Print(CM_LIGHTGREEN, "[DiscordRPC] - Hello from PluginEntry!");

	// Create callback for Frame Events
	last_status = g_ModuleInterface->CreateCallback(
		Module,
		EVENT_FRAME,
		FrameCallback,
		0
	);

	if (!AurieSuccess(last_status)) {
		PrintError(__FILE__, __LINE__, "[DiscordRPC] - Failed to register callback!");
	}

	// Create callback for Object Events
	last_status = g_ModuleInterface->CreateCallback(
		Module,
		EVENT_OBJECT_CALL,
		CodeCallback,
		0
	);

	if (!AurieSuccess(last_status)) {
		PrintError(__FILE__, __LINE__, "[DiscordRPC] - Failed to register callback!");
	}

	/*
			Discord Rich Presence Initialization
	*/
	result = discord::Core::Create(APPLICATION_ID, (uint64_t)discord::CreateFlags::NoRequireDiscord, &core);
	if (result != discord::Result::Ok) {
		PrintError(__FILE__, __LINE__, "Discord initialization failed: %d - is your Discord closed?", static_cast<int>(result));
		return AURIE_SUCCESS;
	}
	core->SetLogHook(discord::LogLevel::Debug, LogProblemsFunction);
	// Map stage names and location names to their respective BGM indices
	stage_map[266] = std::make_pair("Stage 1", "Grassy Plains");
	stage_map[261] = std::make_pair("Stage 2", "Holo Office");
	stage_map[43] = std::make_pair("Stage 3", "Halloween Castle");
	stage_map[44] = std::make_pair("Stage 4", "Gelora Bung Yagoo");
	stage_map[101] = std::make_pair("Stage 1 (Hard)", "Grassy Plains (Night)");
	stage_map[268] = std::make_pair("Stage 2 (Hard)", "Holo Office (Evening)");
	stage_map[237] = std::make_pair("Stage 3 (Hard)", "Halloween Castle (Myth)");
	stage_map[74] = std::make_pair("Time Stage 1", "Concert Stage");

	return AURIE_SUCCESS;
}