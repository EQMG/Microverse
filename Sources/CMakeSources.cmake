set(MICROVERSE_HEADERS_
        "Configs/ConfigManager.hpp"
        "Post/Filters/FilterDamage.hpp"
        "MainRenderer.hpp"
        "MainUpdater.hpp"
        "MainWorld.hpp"
        "Scenes/FpsCamera.hpp"
        "Scenes/FpsPlayer.hpp"
        "Scenes/Scene1.hpp"
        "Scenes/Uis/Navigation/ContentExit.hpp"
        "Scenes/Uis/Navigation/UiNavigation.hpp"
        "Scenes/Uis/Navigation/UiTab.hpp"
        "Scenes/Uis/OverlayDebug.hpp"
        "Scenes/Uis/UiInventory.hpp"
        "Terrains/LodBehaviour.hpp"
        "Terrains/MaterialTerrain.hpp"
        "Terrains/MeshTerrain.hpp"
        "Voxels/MaterialVoxel.hpp"
        "Voxels/VoxelBlock.hpp"
        "Voxels/VoxelChunk.hpp"
        "Waters/MaterialWater.hpp"
        "Waters/MeshWater.hpp"
)

set(MICROVERSE_SOURCES_
        "Configs/ConfigManager.cpp"
        "Post/Filters/FilterDamage.cpp"
        "Main.cpp"
        "MainRenderer.cpp"
        "MainUpdater.cpp"
        "MainWorld.cpp"
        "Scenes/FpsCamera.cpp"
        "Scenes/FpsPlayer.cpp"
        "Scenes/Scene1.cpp"
        "Scenes/Uis/Navigation/ContentExit.cpp"
        "Scenes/Uis/Navigation/UiNavigation.cpp"
        "Scenes/Uis/Navigation/UiTab.cpp"
        "Scenes/Uis/OverlayDebug.cpp"
        "Scenes/Uis/UiInventory.cpp"
        "Terrains/LodBehaviour.cpp"
        "Terrains/MaterialTerrain.cpp"
        "Terrains/MeshTerrain.cpp"
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
