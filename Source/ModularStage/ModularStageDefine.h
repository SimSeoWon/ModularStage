#pragma once

#include "CoreMinimal.h"

/**
 * ModularStage 프로젝트 전역에서 사용하는 상수 및 정의
 */

namespace ModularStageDefine
{
    /** 육각형 타일 기본 설정 */
    namespace HexGrid
    {
        // 타일의 중심에서 모서리까지의 거리 (Outer Radius)
        const float TILE_SIZE = 200.0f;
        
        // 육각형의 기하학적 상수 (sqrt(3) / 2)
        const float SQRT_3_OVER_2 = 0.8660254f;
        
        // 타일 간의 가로/세로 간격 (Pointy Top 기준)
        const float HORIZONTAL_SPACING = TILE_SIZE * 1.7320508f; // SQRT(3) * TILE_SIZE
        const float VERTICAL_SPACING = TILE_SIZE * 1.5f;
        
        // 기본 레이어 높이 간격
        const float LAYER_HEIGHT = 50.0f;

        // 클러스터 타일 수 (중심 1 + 주변 18 = 총 19개, 2겹)
        const int32 CLUSTER_TILE_COUNT = 19;
        const int32 CENTER_TILE_INDEX = 0;
    }

    /** 미션 시스템 관련 정의 */
    namespace Mission
    {
        const int32 MAX_TASK_COUNT = 100;
        const float DEFAULT_SPAWN_DELAY = 0.5f;
    }
}

/** 프로젝트 내에서 자주 사용되는 유틸리티 매크로 */
#define GET_HEX_SPACING_H ModularStageDefine::HexGrid::HORIZONTAL_SPACING
#define GET_HEX_SPACING_V ModularStageDefine::HexGrid::VERTICAL_SPACING
