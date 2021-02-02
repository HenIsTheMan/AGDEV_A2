#include "SceneManager.h"

SceneManager::SceneManager():
	im_GameScene(new GameScene()),
	im_GameInCtor(nullptr),
	im_GameInDtor(nullptr),
	im_GameInit(nullptr),
	im_GameFixedUpdate(nullptr),
	im_GameUpdate(nullptr),
	im_GameLateUpdate(nullptr),
	im_GamePreRender(nullptr),
	im_GameRender(nullptr),
	im_GamePostRender(nullptr)
{
}

void SceneManager::Init(){
}

struct CBase
{ 
};

struct CDerived0 : CBase
{
    void MyFunction()
    {
        std::cout << 0 << std::endl;
    }
};

struct CDerived1 : CBase
{
    void MyFunction()
    {
        std::cout << 1 << std::endl;
    }
};


typedef void (CBase::*FunctionPointer)();


int main()
{
    CDerived0* derived0 = new CDerived0();

    FunctionPointer pointer = static_cast<FunctionPointer>(&CDerived0::MyFunction);
    (derived0->*pointer)();

    delete derived0;
}