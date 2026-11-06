#include "GameHud.h"
#include <vector>

GameHud::GameHud(UIManager &ui, vec2i scr, SceneBuilder::BuiltScene &built,
                 int playerMaxHp) {
  // Health bar
  Rect hbArea{1, scr.y - 3, 50, 3};
  Rect hbWritable{1, 1, 48, 1};
  UIElementHandle hbHandle = ui.addElement(
      UIElement(hbArea, hbWritable, HealthBarPresenter::design(hbArea)));
  _healthBar = std::make_unique<HealthBarPresenter>(ui, hbHandle, playerMaxHp);

  // Demon state debug display
  Rect stateArea{2, scr.y - 7, 30, 3};
  Rect stateWritable{1, 1, 28, 1};
  UIElementHandle stateHandle = ui.addElement(UIElement(
      stateArea, stateWritable, IstvanStatePresenter::design(stateArea)));
  // Hidden until Istvan first notices the player (see IstvanStatePresenter).
  ui.setVisible(stateHandle, false);
  _istvanState = std::make_unique<IstvanStatePresenter>(ui, stateHandle);
  if (built.istvanCtrl)
    built.istvanCtrl->setStateObserver(_istvanState.get());

  // Minimap, top-right corner
  const int mmW = 25, mmH = 13;
  Rect mmArea{scr.x - mmW - 1, 1, mmW, mmH};
  Rect mmWritable{1, 1, mmW - 2, mmH - 2};
  UIElementHandle mmHandle = ui.addElement(
      UIElement(mmArea, mmWritable, MinimapPresenter::design(mmArea)));
  _minimap = std::make_unique<MinimapPresenter>(
      ui, mmHandle, *built.scene, built.scene->entityAtId(0).transform());

  // Damage feedback: the player has a single health observer, so fan out to
  // the bar and the flash.
  _damageFlash = std::make_unique<DamageFlashPresenter>(
      ui, std::vector<UIElementHandle>{hbHandle, stateHandle, mmHandle},
      playerMaxHp);
  _healthFanOut = std::make_unique<HealthObserverFanOut>();
  _healthFanOut->add(_healthBar.get());
  _healthFanOut->add(_damageFlash.get());
  built.playerCtrl->setHealthObserver(_healthFanOut.get());

  // Weapon viewmodel, bottom-centre
  UIElementHandle vmHandle = ui.addElement(
      ViewmodelPresenter::makeElement(ViewmodelPresenter::area(scr.x, scr.y)));
  _viewmodel = std::make_unique<ViewmodelPresenter>(ui, vmHandle);
  built.playerCtrl->setShootObserver(_viewmodel.get());
}

void GameHud::update(float dt) {
  _minimap->update();
  _viewmodel->update(dt);
}
