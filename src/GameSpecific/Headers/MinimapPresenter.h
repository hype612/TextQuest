#ifndef MINIMAPPRESENTER_H
#define MINIMAPPRESENTER_H

#include "Rect.h"
#include "SceneManager.h"
#include "Transform.h"
#include "UIManager.h"
#include <string>
#include <vector>

// Shows a window of the map centered on the player: '#' walls, 'e' living
// enemies, and one of ^ v < > for the player, pointing where they face.
// Polled once per frame through update().
class MinimapPresenter {
public:
  MinimapPresenter(UIManager &ui, UIElementHandle handle,
                   const SceneManager &scene, const Transform &player);

  void update();

  static std::vector<std::string> design(const Rect &area);

private:
  char playerArrow() const;

  UIManager &_ui;
  UIElementHandle _handle;
  const SceneManager &_scene;
  const Transform &_player;
  std::vector<std::string> _last;
};

#endif // MINIMAPPRESENTER_H
