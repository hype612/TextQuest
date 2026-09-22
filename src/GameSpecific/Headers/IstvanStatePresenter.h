#ifndef ISTVANSTATEPRESENTER_H
#define ISTVANSTATEPRESENTER_H

#include "IIstvanStateObserver.h"
#include "Rect.h"
#include "UIManager.h"
#include <string>
#include <vector>

class IstvanStatePresenter : public IIstvanStateObserver {
public:
  IstvanStatePresenter(UIManager &ui, UIElementHandle handle);

  void onStateChanged(const std::string &state) override;

  static std::vector<std::string> design(const Rect &area);

private:
  UIManager &_ui;
  UIElementHandle _handle;
};

#endif // ISTVANSTATEPRESENTER_H
