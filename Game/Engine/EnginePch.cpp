#include "pch.h"
#include "EnginePch.h"
#include "Engine.h"

unique_ptr<Engine> GEngine = make_unique<Engine>();  // 전역변수 선언. 동적할당 안해서 crash 났었어.


//void HelloEngine() // 이 라이브러리 자체를 위에 있는 클라이언트에서 활용할 수 있는지를 체크해보기 위해 테스트 삼아 만들어본 함수임.  
//{
//
//} 더 이상 활용 안함
