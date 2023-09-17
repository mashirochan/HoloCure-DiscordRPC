# HoloCure-DiscordRPC
A HoloCure mod intended to add Discord Rich Presence functionality to HoloCure, allowing it to display in your Discord activity what you are currently doing in the game (stage, holomem, time elapsed, etc.).

![Example Screenshot](https://i.imgur.com/OCGviId.png)

# Installation

Regarding the YYToolkit Launcher:

It's a launcher used to inject .dlls, so most anti-virus will be quick to flag it with Trojan-like behavior because of a similar use-case. The launcher is entirely open soruce (as is YYToolkit itself, the backbone of the project), so you're more than welcome to build everything yourself from the source: https://github.com/Archie-osu/YYToolkit/

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

Feel free to ask in the [Reddit thread](https://www.reddit.com/r/holocure/comments/16ktvh4/discord_rich_presence_mod_release/) about any issues you have!
