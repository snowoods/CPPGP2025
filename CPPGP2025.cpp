// CPPGP2025.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include "ZVector3.h"
#include "ZMatrix.h"

int main()
{
    ZVector3 v1(1, 2, 3);
    std::cout << v1 << std::endl;

    ZVector3 v2(4, -5, 6);
    std::cout << v2.Cross(v1) << std::endl;

    std::cout << ZVector3::Dot(v1, v2) << std::endl;
    std::cout << v1.radBetween(v1, v2) << std::endl;
    std::cout << v1.degBetween(v1, v2) << std::endl;

    // Matrix 테스트
    ZVector3 localPoint(1.0, 1.0, 1.0);
    std::cout << "Local Space Point: " << localPoint << std::endl;

    // Scale 모든 축으로 2배 확대
    ZMatrix matScale = ZMatrix::CreateScale(2.0, 2.0, 2.0);

    // Rotation : Y축 90도 회전
    const double PI = 3.1415926535897932;
    ZMatrix matRotation = ZMatrix::CreateRotationY(PI / 2.0);

    // Translation : 월드 공간의 (5, 6, 7) 위치로 이동
    ZMatrix matTranslation = ZMatrix::CreateTranslation(5.0, 6.0, 7.0);

    // 변환을 순차적으로 적용
    ZVector3 pointAfterScale = localPoint.Transform(matScale);
    std::cout << pointAfterScale << std::endl;

    ZVector3 pointAfterRotation = pointAfterScale.Transform(matRotation);
    std::cout << pointAfterRotation << std::endl;

    ZVector3 finalWorldPoint = pointAfterRotation.Transform(matTranslation);
    std::cout << finalWorldPoint << std::endl;

    // 한방에 월드 매트릭스
    ZMatrix worldMatrix = matScale * matRotation * matTranslation;
    ZVector3 finalWorldPointByMatrix = localPoint.Transform(worldMatrix);
    std::cout << finalWorldPointByMatrix << std::endl;

}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
