set(POLYORBIS_HEADERS_
        "Configs/ConfigManager.hpp"
        "Scenes/Scene1.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "Scenes/ManagerUis.hpp"
        "ManagerRender.hpp"
        "Uis/Navigation/ContentExit.hpp"
        "Uis/Navigation/UiNavigation.hpp"
        "Uis/Navigation/UiTab.hpp"
        "Uis/OverlayDebug.hpp"
        "Uis/UiInventory.hpp"
)

set(POLYORBIS_SOURCES_
        "Configs/ConfigManager.cpp"
        "Scenes/Scene1.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Scenes/ManagerUis.cpp"
        "Main.cpp"
        "ManagerRender.cpp"
        "Uis/Navigation/ContentExit.cpp"
        "Uis/Navigation/UiNavigation.cpp"
        "Uis/Navigation/UiTab.cpp"
        "Uis/OverlayDebug.cpp"
        "Uis/UiInventory.cpp"
)

source_group("Header Files" FILES ${POLYORBIS_HEADERS_})
source_group("Source Files" FILES ${POLYORBIS_SOURCES_})

set(POLYORBIS_SOURCES
        ${POLYORBIS_HEADERS_}
        ${POLYORBIS_SOURCES_}
)
