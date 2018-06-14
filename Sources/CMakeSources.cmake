set(MICROVERSE_HEADERS_
        "Configs/ConfigManager.hpp"
        "Scenes/Scene1.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "Scenes/ManagerUis.hpp"
        "Terrains/LodBehaviour.hpp"
        "Terrains/MaterialTerrain.hpp"
        "Terrains/MeshTerrain.hpp"
        "MainRenderer.hpp"
        "MainUpdater.hpp"
        "MainWorld.hpp"
        "Uis/Navigation/ContentExit.hpp"
        "Uis/Navigation/UiNavigation.hpp"
        "Uis/Navigation/UiTab.hpp"
        "Uis/OverlayDebug.hpp"
        "Uis/UiInventory.hpp"
        "Voxels/MaterialVoxel.hpp"
        "Voxels/VoxelBlock.hpp"
        "Voxels/VoxelChunk.hpp"
        "Waters/MaterialWater.hpp"
        "Waters/MeshWater.hpp"
)

set(MICROVERSE_SOURCES_
        "Configs/ConfigManager.cpp"
        "Scenes/Scene1.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Scenes/ManagerUis.cpp"
        "Main.cpp"
        "MainRenderer.cpp"
        "MainUpdater.cpp"
        "MainWorld.cpp"
        "Terrains/LodBehaviour.cpp"
        "Terrains/MaterialTerrain.cpp"
        "Terrains/MeshTerrain.cpp"
        "Uis/Navigation/ContentExit.cpp"
        "Uis/Navigation/UiNavigation.cpp"
        "Uis/Navigation/UiTab.cpp"
        "Uis/OverlayDebug.cpp"
        "Uis/UiInventory.cpp"
        "Voxels/MaterialVoxel.cpp"
        "Voxels/VoxelBlock.cpp"
        "Voxels/VoxelChunk.cpp"
        "Waters/MaterialWater.cpp"
        "Waters/MeshWater.cpp"
)

source_group("Header Files" FILES ${MICROVERSE_HEADERS_})
source_group("Source Files" FILES ${MICROVERSE_SOURCES_})

set(MICROVERSE_SOURCES
        ${MICROVERSE_HEADERS_}
        ${MICROVERSE_SOURCES_}
)
