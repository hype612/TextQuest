#ifndef NCURSESINPUTHANDLER_H_
#define NCURSESINPUTHANDLER_H_


#if (defined (LINUX) || defined (__linux__))
#include "./IInputHandler.h"
#include <ncurses.h>


class NCursesInputHandler : public IInputHandler {
public:
    NCursesInputHandler(Player& player);
    void Init() override;
    void ReceiveMovementInput(float delta) override;
private:
};

#endif // OS Check end
#endif // NCURSESINPUTHANDLER_H_
