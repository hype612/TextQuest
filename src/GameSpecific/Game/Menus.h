#ifndef MENUS_H
#define MENUS_H

#include "Core.h"
#include "Difficulty.h"
#include "IInputHandler.h"
#include "UIManager.h"
#include <optional>

class Menus {
public:
  enum class MenuChoice { NEW_GAME, LAST_SEED, EXIT };

  Menus(GameEngine &ge, UIManager &ui, IInputHandler &input);

  // Shows the start menu. `savedSeed` enables the "replay last seed" option.
  MenuChoice showStart(std::optional<unsigned int> savedSeed);
  // Shows the difficulty picker. Returns nullopt if the player quit.
  std::optional<Difficulty> showDifficulty();
  // Shows the "you won / you died" box. It stays up until hideEnd().
  void showEnd(bool won);
  // Removes the end box if it is showing. Safe to call when it is not.
  void hideEnd();

  Menus(const Menus &) = delete;
  Menus &operator=(const Menus &) = delete;
  Menus(Menus &&) = delete;
  Menus &operator=(Menus &&) = delete;

private:
  // A bordered box of the given size, centered on the screen. The writable
  // area is the inside of the border.
  UIElement makeBox(unsigned int w, unsigned int h) const;

  GameEngine &_ge;
  UIManager &_ui;
  IInputHandler &_input;
  std::optional<UIElementHandle> _endBox;
};

#endif // MENUS_H
