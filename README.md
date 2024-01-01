# HoloCure Discord RPC Mod
A HoloCure mod intended to add Discord Rich Presence functionality to HoloCure, allowing it to display in your Discord activity what you are currently doing in the game (stage, holomem, time elapsed, etc.).

![Example Screenshot](https://i.imgur.com/OCGviId.png)

# Features

- Title Screen Display
- Current Stage
- Current Game Mode
- Current Holomem
- Time Elapsed
- Holo House
- Stage Icons
- Holomem Icons
- Config File System

# Installation

- Download the two .dll files from the latest release of my [Discord RPC mod](https://github.com/mashirochan/HoloCure-DiscordRPC/releases/latest)
- Download `AurieManager.exe` from the [latest release of Aurie](https://github.com/AurieFramework/Aurie/releases/latest)
- (OPTIONAL) Open up `%localappdata%\HoloCure` in your File Explorer
  - Back up your save data!
- Run the `AurieManager.exe`
  - Click "Add Game" next to the game drop-down
    - Select your `HoloCure.exe` file
  - Click "Install Aurie" in the bottom right
  - Click "Add Mods" on the right
    - Select your `DiscordRPC.dll` file
  - Move or copy `discord_game_sdk.dll` to `<game-folder>\mods\Native\` (where `AurieCore.dll` is)
  - Click "Play Game"
    - Hope with all your might that it works!

Not much testing at all has gone into this, so I'm really sorry if this doesn't work. Use at your own risk!

Feel free to ask in the [Reddit thread](https://www.reddit.com/r/holocure/comments/16l5wru/discord_rich_presence_mod_release/) about any issues you have, or also join the [HoloCure Discord server](https://discord.gg/holocure) and look for the HoloCure Code Discussion thread in #holocure-general!

# Troubleshooting

Here are some common problems you could have that are preventing your mod from not working correctly:

### VCRUNTIME140_1.dll was not found
![VCRUNTIME140_1.dll was not found](https://i.imgur.com/6lWzUnp.png)

You need to install `Microsoft Visual C++ 2015-2022 Redistributable (x64)` which can be found [here](https://learn.microsoft.com/en-US/cpp/windows/latest-supported-vc-redist).

### "Play Game" Does Not Open Game

Depending on when you are reading this, there is currently a bug in Aurie Manager where the "Play Game" button is a bit finicky.. You should be able to just start the game through Steam or however you usually start it up. Odds are that this will be fixed very soon and this troubleshooting will become irrelevant!
