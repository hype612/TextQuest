#include "DualsenseInputHandler.h"
#include "MoveDirection.h"
#include <array>
#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <libudev.h>
#include <linux/input-event-codes.h>
#include <stdexcept>
#include <unistd.h>

void DualsenseInputHandler::Init() {
  _dualsense_devs = {nullptr, nullptr, nullptr};
  size_t ds_idx = 0;
  udev *dualsense_udev = udev_new();
  if (!dualsense_udev) {
    throw(std::runtime_error("failed to create udev"));
  }
  udev_enumerate *query = udev_enumerate_new(dualsense_udev);
  if (!query) {
    udev_unref(dualsense_udev);
    throw(std::runtime_error("failed to create udev_enumerate"));
  }
  udev_enumerate_add_match_subsystem(query, "input");

  if (udev_enumerate_scan_devices(query) < 0) {
    throw(std::runtime_error("something fed up on scan_devices"));
  } else {
    udev_list_entry *entry = udev_enumerate_get_list_entry(query);
    udev_list_entry_foreach(entry, udev_enumerate_get_list_entry(query)) {
      udev_device *dev = udev_device_new_from_syspath(
          dualsense_udev, udev_list_entry_get_name(entry));
      const char *devnode = udev_device_get_devnode(dev);
      if (!devnode) {
        udev_device_unref(dev);
        continue;
      }

      // check if it is the mouse or the js
      const char *dev_name = udev_device_get_sysname(dev);
      if (!dev_name || strncmp(dev_name, "event", 5) != 0) {
        udev_device_unref(dev);
        continue;
      }

      udev_device *prnt =
          udev_device_get_parent_with_subsystem_devtype(dev, "input", nullptr);
      if (!prnt) {
        udev_device_unref(dev);
        continue;
      }

      const char *dev_vendor = udev_device_get_sysattr_value(prnt, "id/vendor");
      const char *dev_product =
          udev_device_get_sysattr_value(prnt, "id/product");
      if (!dev_vendor || !dev_product) {
        udev_device_unref(dev);
        continue;
      }

      if (!strcmp(dev_vendor, _ds_vendor_id) &&
          !strcmp(dev_product, _ds_product_id)) {
        libevdev *in = capture_dualsense(devnode);
        if (in && ds_idx < 3) {
          _dualsense_devs[ds_idx] = in;
          ds_idx++;
        }
      }
      udev_device_unref(dev);
    }
  }

  // actual query loop
  _gamepad = nullptr;
  for (size_t i = 0; i < 3; i++) {
    if (_dualsense_devs[i] == nullptr) {
      continue;
    }
    if (libevdev_has_event_code(_dualsense_devs[i], EV_KEY, BTN_SOUTH)) {
      _gamepad = _dualsense_devs[i];
      break;
    }
  }

  if (!_gamepad) {
    throw(std::runtime_error("gamepad not found"));
  }
  _ls_min_x = libevdev_get_abs_minimum(_gamepad, ABS_X);
  _ls_max_x = libevdev_get_abs_maximum(_gamepad, ABS_X);
  _ls_min_y = libevdev_get_abs_minimum(_gamepad, ABS_Y);
  _ls_max_y = libevdev_get_abs_maximum(_gamepad, ABS_Y);
  _rs_min_x = libevdev_get_abs_minimum(_gamepad, ABS_RX);
  _rs_max_x = libevdev_get_abs_maximum(_gamepad, ABS_RX);

  udev_enumerate_unref(query);
  udev_unref(dualsense_udev);
}

void DualsenseInputHandler::ReceiveInput() {
  input_event e;
  _optionsPressed = false;
  while (0 == libevdev_next_event(_gamepad, LIBEVDEV_READ_FLAG_NORMAL, &e)) {
    // options button (BTN_START on the DualSense), press only
    if (e.type == EV_KEY && e.code == BTN_START && e.value == 1) {
      _optionsPressed = true;
    }
    query_facebuttons(e);
    query_dpadbuttons(e);
    query_trigs_and_shoulders(e);
    query_analogs(e);
  }
}

void DualsenseInputHandler::query_facebuttons(const input_event &e) {

  // leftside buttons - pressed
  if (e.code == BTN_NORTH && e.value == 1) {
    _triangle_pressed = true;
  }
  if (e.code == BTN_EAST && e.value == 1) {
    _circle_pressed = true;
  }
  if (e.code == BTN_WEST && e.value == 1) {
  }
  if (e.code == BTN_SOUTH && e.value == 1) {
    _cross_pressed = true;
  }

  // leftside buttons - release
  if (e.code == BTN_NORTH && e.value == 0) {
    _triangle_pressed = false;
  }
  if (e.code == BTN_EAST && e.value == 0) {
    _circle_pressed = false;
  }
  if (e.code == BTN_WEST && e.value == 0) {
  }
  if (e.code == BTN_SOUTH && e.value == 0) {
    _cross_pressed = false;
  }
}
void DualsenseInputHandler::query_dpadbuttons(const input_event &e) {
  if (e.type != EV_ABS) {
    return;
  }
  // dpad buttons - press
  if (e.code == ABS_HAT0Y && e.value == 1) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] = 1.f;
  }
  if (e.code == ABS_HAT0Y && e.value == -1) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] = 1.f;
  }
  if (e.code == ABS_HAT0X && e.value == -1) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] = 1.f;
  }
  if (e.code == ABS_HAT0X && e.value == 1) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = 1.f;
  }

  // dpad buttons - release
  if (e.code == ABS_HAT0Y && e.value == 0) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] = 0.f;
    _mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] = 0.f;
  }
  if (e.code == ABS_HAT0X && e.value == 0) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] = 0.f;
    _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = 0.f;
  }
}
void DualsenseInputHandler::query_trigs_and_shoulders(const input_event &e) {
  // trigs and shuulders - press
  if (e.code == BTN_TL && e.value == 1) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::SHOOT)] = 1.f;
  }
  if (e.code == BTN_TR && e.value == 1) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::SHOOT)] = 1.f;
  }
  // trigs and shuulders - release
  if (e.code == BTN_TL && e.value == 0) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::SHOOT)] = 0.f;
  }
  if (e.code == BTN_TR && e.value == 0) {
    _mvmtKeyStates[static_cast<int>(MoveDirection::SHOOT)] = 0.f;
  }
}
void DualsenseInputHandler::query_analogs(const input_event &e) {
  // codes are only unique per type: SYN_REPORT is code 0, same as ABS_X
  if (e.type != EV_ABS) {
    return;
  }
  if (e.code == ABS_X) {
    float x_axis_l = normalize_analog_input(ABS_X, e.value);
    if (x_axis_l > 0.f) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = x_axis_l;
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] = 0.f;
    } else {
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_RIGHT)] = 0.f;
      _mvmtKeyStates[static_cast<int>(MoveDirection::STRAFE_LEFT)] = -x_axis_l;
    }
  }
  if (e.code == ABS_Y) {
    float y_axis_l = normalize_analog_input(ABS_Y, e.value);
    if (y_axis_l > 0.f) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] = y_axis_l;
      _mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] = 0.f;
    } else {
      _mvmtKeyStates[static_cast<int>(MoveDirection::BACKWARD)] = 0.f;
      _mvmtKeyStates[static_cast<int>(MoveDirection::FORWARD)] = -y_axis_l;
    }
  }
  if (e.code == ABS_RX) {
    float x_axis_r = normalize_analog_input(ABS_RX, e.value);
    if (x_axis_r > 0.f) {
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] = x_axis_r;
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] = 0.f;
    } else {
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_RIGHT)] = 0.f;
      _mvmtKeyStates[static_cast<int>(MoveDirection::TURN_LEFT)] = -x_axis_r;
    }
  }
}

float DualsenseInputHandler::normalize_analog_input(int axisCode,
                                                    int rawInput) {
  float out = 0.f;
  switch (axisCode) {
  case ABS_X:
    out = 2.f * (rawInput - _ls_min_x) / (_ls_max_x - _ls_min_x) - 1;
    break;
  case ABS_Y:
    // up is negative in evdev, query_analogs relies on that
    out = 2.f * (rawInput - _ls_min_y) / (_ls_max_y - _ls_min_y) - 1;
    break;
  case ABS_RX:
    out = 2.f * (rawInput - _rs_min_x) / (_rs_max_x - _rs_min_x) - 1;
    break;
  }

  if (out < 0.15f && out > -0.15f)
    return 0.f;
  return out;
}

float DualsenseInputHandler::keyDown(MoveDirection dir) const {
  return _mvmtKeyStates[static_cast<int>(dir)];
}
// fallback way to query raw key states w/o
// routing through enums
bool DualsenseInputHandler::rawKeyPressed(uint32_t key) const {
  if (key == _keyOne && _triangle_pressed) {
    return true;
  }
  if (key == _keyTwo && _circle_pressed) {
    return true;
  }
  if (key == _keyThree && _cross_pressed) {
    return true;
  }
  // options button acts as the keyboard's x
  if (key == _keyX && _optionsPressed) {
    return true;
  }
  return false;
}

DualsenseInputHandler::~DualsenseInputHandler() {
  // Cleanup
  for (size_t i = 0; i < 3; i++) {
    if (_dualsense_devs[i]) {
      close(libevdev_get_fd(_dualsense_devs[i]));
      libevdev_free(_dualsense_devs[i]);
    }
  }
}

libevdev *DualsenseInputHandler::capture_dualsense(const char *fpath) {
  int fd = open(fpath, O_RDONLY | O_NONBLOCK);
  libevdev *out = nullptr;
  if (libevdev_new_from_fd(fd, &out) == 0) {
    return out;
  } else {
    close(fd);
    return nullptr;
  }
}
