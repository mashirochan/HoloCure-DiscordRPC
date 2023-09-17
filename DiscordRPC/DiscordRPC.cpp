#define YYSDK_PLUGIN
#include "DiscordRPC.hpp"	// Include our header
#include "ModInfo.h"
#include "DiscordSDK/discord.h"
#include <Windows.h>    // Include Windows's mess.
#include <vector>       // Include the STL vector.
#include <unordered_map>
#include <functional>

static struct Version {
	int major = VERSION_MAJOR;
	int minor = VERSION_MINOR;
	int build = VERSION_BUILD;
} version;

static struct Mod {
	Version version;
	const char* name = MOD_NAME;
} mod;

//// Discord SDK Vars
constexpr auto APPLICATION_ID = 1151654619244666951;
discord::Core* core{};
discord::Result result;
void LogProblemsFunction(discord::LogLevel level, std::string message) {
	PrintError(__FILE__, __LINE__, "Discord:%d - %s", level, message.c_str());
}

// CallBuiltIn is way too slow to use per frame. Need to investigate if there's a better way to call in built functions.

// We save the CodeCallbackHandler attributes here, so we can unregister the callback in the unload routine.
static CallbackAttributes_t* g_pFrameCallbackAttributes = nullptr;
static CallbackAttributes_t* g_pCodeCallbackAttributes = nullptr;
static uint32_t FrameNumber = 0;
static const char* playStr = "Play Modded!";
RefString tempVar = RefString(playStr, strlen(playStr), false);
static bool versionTextChanged = false;

static std::unordered_map<int, const char*> codeIndexToName;
static std::unordered_map<int, std::function<void(YYTKCodeEvent* pCodeEvent, CInstance* Self, CInstance* Other, CCode* Code, RValue* Res, int Flags)>> codeFuncTable;

// This callback is registered on EVT_PRESENT and EVT_ENDSCENE, so it gets called every frame on DX9 / DX11 games.
YYTKStatus FrameCallback(YYTKEventBase* pEvent, void* OptionalArgument) {
	FrameNumber++;

	::core->RunCallbacks();

	// Tell the core the handler was successful.
	return YYTK_OK;
}

// This callback is registered on EVT_CODE_EXECUTE, so it gets called every game function call.
YYTKStatus CodeCallback(YYTKEventBase* pEvent, void* OptionalArgument) {
	YYTKCodeEvent* pCodeEvent = dynamic_cast<decltype(pCodeEvent)>(pEvent);

	std::tuple<CInstance*, CInstance*, CCode*, RValue*, int> args = pCodeEvent->Arguments();

	CInstance* Self = std::get<0>(args);
	CInstance* Other = std::get<1>(args);
	CCode* Code = std::get<2>(args);
	RValue* Res = std::get<3>(args);
	int			Flags = std::get<4>(args);

	if (!Code->i_pName) {
		return YYTK_INVALIDARG;
	}

	if (codeFuncTable.count(Code->i_CodeIndex) != 0) {
		codeFuncTable[Code->i_CodeIndex](pCodeEvent, Self, Other, Code, Res, Flags);
	} else // Haven't cached the function in the table yet. Run the if statements and assign the function to the code index
	{
		codeIndexToName[Code->i_CodeIndex] = Code->i_pName;
		if (_strcmpi(Code->i_pName, "gml_Object_obj_TitleScreen_Create_0") == 0) {
			auto TitleScreen_Create_0 = [](YYTKCodeEvent* pCodeEvent, CInstance* Self, CInstance* Other, CCode* Code, RValue* Res, int Flags) {
				if (versionTextChanged == false) {
					YYRValue yyrv_version;
					CallBuiltin(yyrv_version, "variable_global_get", Self, Other, { "version" });
					std::string moddedVerStr = yyrv_version.operator std::string() + " (Modded)";
					CallBuiltin(yyrv_version, "variable_global_set", Self, Other, { "version", moddedVerStr.c_str() });
					versionTextChanged = true;
				}

				discord::Activity activity{};
				activity.SetName("HoloCure - Save the Fans!");
				activity.SetState("Playing Fauna");
				activity.SetDetails("Stage 3 (Hard)");
				activity.GetTimestamps().SetStart(1694842092);
				activity.GetAssets().SetLargeImage("stage_3_hard_icon");
				activity.GetAssets().SetLargeText("Halloween Castle (Myth)");
				activity.GetAssets().SetSmallImage("ceres_fauna_icon");
				activity.GetAssets().SetSmallText("UUUUUUU");
				core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {});

				pCodeEvent->Call(Self, Other, Code, Res, Flags);
			};
			TitleScreen_Create_0(pCodeEvent, Self, Other, Code, Res, Flags);
			codeFuncTable[Code->i_CodeIndex] = TitleScreen_Create_0;
		} else if (_strcmpi(Code->i_pName, "gml_Object_obj_TextController_Create_0") == 0) {
			auto TextController_Create_0 = [](YYTKCodeEvent* pCodeEvent, CInstance* Self, CInstance* Other, CCode* Code, RValue* Res, int Flags) {
				YYRValue Result;
				pCodeEvent->Call(Self, Other, Code, Res, Flags);
				CallBuiltin(Result, "variable_global_get", Self, Other, { "TextContainer" });
				YYRValue tempResult;
				CallBuiltin(tempResult, "struct_get", Self, Other, { Result, "titleButtons" });
				YYRValue tempResultOne;
				CallBuiltin(tempResultOne, "struct_get", Self, Other, { tempResult, "eng" });

				tempResultOne.RefArray->m_Array[0].String = &tempVar;
				};
			TextController_Create_0(pCodeEvent, Self, Other, Code, Res, Flags);
			codeFuncTable[Code->i_CodeIndex] = TextController_Create_0;
		} else {
			auto UnmodifiedFunc = [](YYTKCodeEvent* pCodeEvent, CInstance* Self, CInstance* Other, CCode* Code, RValue* Res, int Flags) {
				pCodeEvent->Call(Self, Other, Code, Res, Flags);
				};
			UnmodifiedFunc(pCodeEvent, Self, Other, Code, Res, Flags);
			codeFuncTable[Code->i_CodeIndex] = UnmodifiedFunc;
		}
	}
	// Tell the core the handler was successful.
	return YYTK_OK;
}

// Create an entry routine - it must be named exactly this, and must accept these exact arguments.
// It must also be declared DllExport (notice how the other functions are not).
DllExport YYTKStatus PluginEntry(YYTKPlugin* PluginObject) {

	// Set the unload routine
	PluginObject->PluginUnload = PluginUnload;

	// Print a message to the console
	PrintMessage(CLR_DEFAULT, "[%s v%d.%d.%d] - Hello from PluginEntry!", mod.name, mod.version.major, mod.version.minor, mod.version.build);

	PluginAttributes_t* PluginAttributes = nullptr;

	// Get the attributes for the plugin - this is an opaque structure, as it may change without any warning.
	// If Status == YYTK_OK (0), then PluginAttributes is guaranteed to be valid (non-null).
	if (YYTKStatus Status = PmGetPluginAttributes(PluginObject, PluginAttributes)) {
		PrintError(__FILE__, __LINE__, "[%s v%d.%d.%d] - PmGetPluginAttributes failed with 0x%x", mod.name, mod.version.major, mod.version.minor, mod.version.build, Status);
		return YYTK_FAIL;
	}

	// Register a callback for frame events
	YYTKStatus Status = PmCreateCallback(
		PluginAttributes,					// Plugin Attributes
		g_pFrameCallbackAttributes,				// (out) Callback Attributes
		FrameCallback,						// The function to register as a callback
		static_cast<EventType>(EVT_PRESENT | EVT_ENDSCENE), // Which events trigger this callback
		nullptr								// The optional argument to pass to the function
	);

	if (Status) {
		PrintError(__FILE__, __LINE__, "[%s v%d.%d.%d] - PmCreateCallback failed with 0x%x", mod.name, mod.version.major, mod.version.minor, mod.version.build, Status);
		return YYTK_FAIL;
	}

	// Register a callback for frame events
	Status = PmCreateCallback(
		PluginAttributes,					// Plugin Attributes
		g_pCodeCallbackAttributes,			// (out) Callback Attributes
		CodeCallback,						// The function to register as a callback
		static_cast<EventType>(EVT_CODE_EXECUTE), // Which events trigger this callback
		nullptr								// The optional argument to pass to the function
	);

	if (Status) {
		PrintError(__FILE__, __LINE__, "[%s v%d.%d.%d] - PmCreateCallback failed with 0x%x", mod.name, mod.version.major, mod.version.minor, mod.version.build, Status);
		return YYTK_FAIL;
	}

	/*
								Discord Rich Presence Initialization
	*/
	result = discord::Core::Create(APPLICATION_ID, DiscordCreateFlags_Default, &core);
	if (result != discord::Result::Ok) {
		PrintError(__FILE__, __LINE__, "Discord initialization failed: %s", result);
		return YYTK_FAIL;
	}
	
	core->SetLogHook(discord::LogLevel::Debug, LogProblemsFunction);

	// Off it goes to the core.
	return YYTK_OK;
}

// The routine that gets called on plugin unload.
// Registered in PluginEntry - you should use this to release resources.
YYTKStatus PluginUnload() {
	YYTKStatus Removal = PmRemoveCallback(g_pFrameCallbackAttributes);

	// If we didn't succeed in removing the callback.
	if (Removal != YYTK_OK) {
		PrintError(__FILE__, __LINE__, "[%s v%d.%d.%d] PmRemoveCallback failed with 0x%x", mod.name, mod.version.major, mod.version.minor, mod.version.build, Removal);
	}

	Removal = PmRemoveCallback(g_pCodeCallbackAttributes);

	// If we didn't succeed in removing the callback.
	if (Removal != YYTK_OK) {
		PrintError(__FILE__, __LINE__, "[%s v%d.%d.%d] PmRemoveCallback failed with 0x%x", mod.name, mod.version.major, mod.version.minor, mod.version.build, Removal);
	}

	PrintMessage(CLR_DEFAULT, "[%s v%d.%d.%d] - Goodbye!", mod.name, mod.version.major, mod.version.minor, mod.version.build);

	return YYTK_OK;
}

// Boilerplate setup for a Windows DLL, can just return TRUE.
// This has to be here or else you get linker errors (unless you disable the main method)
BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	return 1;
}