#ifndef WINDOWSINPUTHANDLER_H_
#define WINDOWSINPUTHANDLER_H_


#if (defined (_WIN32) || defined (_WIN64))

#include <Windows.h>
#include "./IInputHandler.h"

class WindowsInputHandler : public IInputHandler {
public:
    WindowsInputHandler(Player& player);
    void Init() override;
    void ReceiveMovementInput(float delta) override;
private:
};


#endif // OS Checker end
#endif // WINDOWSINPUTHANDLER_H_
