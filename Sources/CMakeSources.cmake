set(MICROVERSE_HEADERS_
        "Configs/ConfigManager.hpp"
        "Post/Filters/FilterDamage.hpp"
        "MainRenderer.hpp"
        "Planet/Gravity.hpp"
        "Planet/Planet.hpp"
        "MainUpdater.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "Scenes/Scene1.hpp"
        "Uis/Navigation/ContentExit.hpp"
        "Uis/Navigation/UiNavigation.hpp"
        "Uis/Navigation/UiTab.hpp"
        "Uis/OverlayDebug.hpp"
        "Uis/UiInventory.hpp"
        "Planet/QuadtreeChunk.hpp"
        "Planet/MaterialChunk.hpp"
        "Planet/MeshChunk.hpp"
        "Waters/MaterialWater.hpp"
        "Waters/MeshWater.hpp"
        "World/World.hpp"
)

set(MICROVERSE_SOURCES_
        "Microverse.rc"
        "Configs/ConfigManager.cpp"
        "Post/Filters/FilterDamage.cpp"
        "Main.cpp"
        "MainRenderer.cpp"
        "Planet/Gravity.cpp"
        "Planet/Planet.cpp"
        "MainUpdater.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Scenes/Scene1.cpp"
        "Uis/Navigation/ContentExit.cpp"
        "Uis/Navigation/UiNavigation.cpp"
        "Uis/Navigation/UiTab.cpp"
        "Uis/OverlayDebug.cpp"
        "Uis/UiInventory.cpp"
        "Planet/QuadtreeChunk.cpp"
        "Planet/MaterialChunk.cpp"
        "Planet/MeshChunk.cpp"
        "Waters/MaterialWater.cpp"
        "Waters/MeshWater.cpp"
        "World/World.cpp"
)

source_group("Header Files" FILES ${MICROVERSE_HEADERS_})
source_group("Source Files" FILES ${MICROVERSE_SOURCES_})

set(MICROVERSE_SOURCES
        ${MICROVERSE_HEADERS_}
        ${MICROVERSE_SOURCES_}
)
