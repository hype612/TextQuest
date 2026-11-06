#include "Menus.h"
#include <string>
#include <vector>

Menus::Menus(GameEngine &ge, UIManager &ui, IInputHandler &input)
    : _ge(ge), _ui(ui), _input(input) {}

UIElement Menus::makeBox(unsigned int w, unsigned int h) const {
  vec2i scr = _ge.screenSize();
  Rect area{(scr.x - static_cast<int>(w)) / 2,
            (scr.y - static_cast<int>(h)) / 2, w, h};
  Rect writable{1, 1, w - 2, h - 2};
  std::vector<std::string> design;
  design.push_back(std::string(w, '='));
  for (unsigned int i = 0; i < h - 2; i++)
    design.push_back("|" + std::string(w - 2, ' ') + "|");
  design.push_back(std::string(w, '='));
  return UIElement(area, writable, design);
}

Menus::MenuChoice Menus::showStart(std::optional<unsigned int> savedSeed) {
  std::string lastSeedLine =
      savedSeed ? "  2) Replay last seed: " + std::to_string(*savedSeed)
                : "  2) Replay last seed (none saved)";
  UIElementHandle handle = _ui.addElement(makeBox(36, 7));
  if (UIElement *el = _ui.elementAt(handle))
    el->updateContent({"  TEXTQUEST", "", "  1) New game (random seed)",
                       lastSeedLine, "  3) Exit"});

  MenuChoice choice = MenuChoice::EXIT;
  _ge.runMenu([&]() {
    if (_input.rawKeyPressed('1')) {
      choice = MenuChoice::NEW_GAME;
      return false;
    }
    if (savedSeed && _input.rawKeyPressed('2')) {
      choice = MenuChoice::LAST_SEED;
      return false;
    }
    if (_input.rawKeyPressed('3') || _input.rawKeyPressed('x'))
      return false;
    return true;
  });
  _ui.removeElement(handle);
  return choice;
}

std::optional<Difficulty> Menus::showDifficulty() {
  UIElementHandle handle = _ui.addElement(makeBox(36, 7));
  if (UIElement *el = _ui.elementAt(handle))
    el->updateContent(
        {"  SELECT DIFFICULTY", "", "  1) Easy", "  2) Normal", "  3) Hard"});

  std::optional<Difficulty> picked;
  _ge.runMenu([&]() {
    if (_input.rawKeyPressed('1')) {
      picked = EASY;
      return false;
    }
    if (_input.rawKeyPressed('2')) {
      picked = NORMAL;
      return false;
    }
    if (_input.rawKeyPressed('3')) {
      picked = HARD;
      return false;
    }
    if (_input.rawKeyPressed('x'))
      return false;
    return true;
  });
  _ui.removeElement(handle);
  return picked;
}

void Menus::showEnd(bool won) {
  hideEnd();
  // 3 writable rows: the result, a blank row and the key hint.
  UIElementHandle handle = _ui.addElement(makeBox(30, 5));
  UIElement *el = _ui.elementAt(handle);
  if (!el)
    return;
  _endBox = handle;
  const int writableWidth = static_cast<int>(el->writableArea().width);
  auto centered = [writableWidth](const std::string &line) {
    int pad = (writableWidth - static_cast<int>(line.size())) / 2;
    return std::string(pad > 0 ? pad : 0, ' ') + line;
  };
  el->updateContent({centered(won ? "YOU WON" : "YOU DIED"), "",
                     centered("r = retry    x = quit")});
}

void Menus::hideEnd() {
  if (!_endBox)
    return;
  _ui.removeElement(*_endBox);
  _endBox.reset();
}
