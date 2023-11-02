# HoloCure Discord RPC Mod
A HoloCure mod intended to add Discord Rich Presence functionality to HoloCure, allowing it to display in your Discord activity what you are currently doing in the game (stage, holomem, time elapsed, etc.).

![Example Screenshot](https://i.imgur.com/OCGviId.png)

# Installation

Regarding the YYToolkit Launcher:

It's a launcher used to inject .dlls, so most anti-virus will be quick to flag it with Trojan-like behavior because of a similar use-case. The launcher is entirely open source (as is YYToolkit itself, the backbone of the project), so you're more than welcome to build everything yourself from the source: https://github.com/Archie-osu/YYToolkit/

- Download the two .dll files from the latest release of my [Discord RPC mod](https://github.com/mashirochan/HoloCure-DiscordRPC/releases/latest)
- Download `Launcher.exe` from the [latest release of YYToolkit](https://github.com/Archie-osu/YYToolkit/releases/latest)
  - Place the `Launcher.exe` file anywhere you want for convenient access
- Open the folder your `HoloCure.exe` is in
  - Back up your game and save data while you're here!
  - Delete, rename, or move `steam_api64.dll` and `Steamworks_x64.dll` if you're on Steam
  - Place the `discord_game_sdk.dll` you downloaded earlier here
- Add the path to your `discord_game_sdk.dll` to your Path environment variable
  - Guide can be found [here](https://www.imatest.com/support/docs/23-1/editing-system-environment-variables/#Windows) if needed
- Run the `Launcher.exe`
  - Click "Select" next to the Runner field
    - Select your `HoloCure.exe` (wherever it is)
  - Click "Open plugin folder" near the bottom right
    - This should create and open the `autoexec` folder wherever your `HoloCure.exe` is
    - Move or copy your `discord-rpc-vX.X.X.dll` file into this folder
  - Click "Start process"
    - Hope with all your might that it works!

Not much testing at all has gone into this, so I'm really sorry if this doesn't work. Use at your own risk!

Feel free to ask in the [Reddit thread](https://www.reddit.com/r/holocure/comments/16l5wru/discord_rich_presence_mod_release/) about any issues you have, or also join the [HoloCure Discord server](https://discord.gg/holocure) and look for the HoloCure Code Discussion thread in #holocure-general!

# Troubleshooting

Here are some common problems you could have that are preventing your mod from not working correctly:

### YYToolkit Launcher Hangs on "Waiting for game..."
![Waiting for game...](https://i.imgur.com/DxDjOGz.png)

The most likely scenario for this is that you did not delete, rename, or move the `steam_api64.dll` and `Steamworks_x64.dll` files in whatever directory the `HoloCure.exe` that you want to mod is in.

### Failed to load 'discord-rpc.dll' - the file may not be a plugin
![Failed to load](https://i.imgur.com/B7nN8y2.png)

The most likely scenario for this is that you did not properly set the Path system environment variable to your `discord_game_sdk.dll`. I recommend just placing it in the same directory as your `HoloCure.exe` file, and then following [this guide](https://www.imatest.com/support/docs/23-1/editing-system-environment-variables/#Windows) to set your Path variable.

### Failed to install plugin: discord-rpc.dll
![Failed to install plugin](https://i.imgur.com/fcg1WWe.png)

The most likely scenario for this is that you tried to click "Add plugin" before "Open plugin folder", so the YYToolkit launcher has not created an `autoexec` folder yet. To solve this, either click "Open plugin folder" to create an `autoexec` folder automatically, or create one manually in the same directory as your `HoloCure.exe` file.

### VCRUNTIME140_1.dll was not found
![VCRUNTIME140_1.dll was not found](https://i.imgur.com/6lWzUnp.png)

You need to install `Microsoft Visual C++ 2015-2022 Redistributable (x64)` which can be found [here](https://learn.microsoft.com/en-US/cpp/windows/latest-supported-vc-redist).