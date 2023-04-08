# mcpelauncher-patches

This is a mod for mcpelauncher-manifest that has additional symbol patches.

Symbols patched: 

`_ZNK11AppPlatform19supportsFilePickingEv` - Allows for file import/export  
`_ZNK11AppPlatform17supportsScriptingEv` - Allows for scripting in older versions 
`_ZNK11AppPlatform25getPlatformUIScalingRulesEv` - Makes default GUI smaller to match windows  
`_ZNK11AppPlatform23getMaxSimRadiusInChunksEv` - Makes the max simulation distance 12 chunks to match windows  
`_ZNK11AppPlatform10getEditionEv` - Returns win10, allowing for world export  
`_ZNK11AppPlatform27getDefaultNetworkMaxPlayersEv` - Allows for up to 8 players by default to match windows  

## Fullscreen
To get the fullscreen toggle to show in video settings, you will have to use the Fullscreen option resource pack (`Fullscreen option.mcpack`). You can still toggle fullscreen using F11 without the resource pack.


## Building
`PATH_TO_NDK="/path/to/ndk"`
- x86

  `cmake -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86 -DCMAKE_CXX_FLAGS=-fvisibility=hidden ..`
- x86_64

  `cmake -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=x86_64 -DCMAKE_CXX_FLAGS=-fvisibility=hidden ..`
- armeabi-v7a

  `cmake -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=armeabi-v7a -DCMAKE_CXX_FLAGS=-fvisibility=hidden ..`
- arm64-v8a

  `cmake -DCMAKE_TOOLCHAIN_FILE=$PATH_TO_NDK/build/cmake/android.toolchain.cmake -DANDROID_ABI=arm64-v8a -DCMAKE_CXX_FLAGS=-fvisibility=hidden ..`
