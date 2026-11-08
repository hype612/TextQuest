#ifndef DUALSENSEINPUTHANDLER_H
#define DUALSENSEINPUTHANDLER_H

#include "IInputHandler.h"

#include "MoveDirection.h"
#include <array>
#include <cstdint>
#include <fcntl.h>
#include <libevdev/libevdev.h>
#include <libudev.h>
#include <linux/input-event-codes.h>
#include <unistd.h>

class DualsenseInputHandler : public IInputHandler {
public:
  void Init() override;
  void ReceiveInput() override;
  float keyDown(MoveDirection dir) const override;
  // fallback way to query raw key states w/o
  // routing through enums
  bool rawKeyPressed(uint32_t key) const override;
  ~DualsenseInputHandler();

private:
  libevdev *capture_dualsense(const char *fpath);
  void query_facebuttons(const input_event &e);
  void query_dpadbuttons(const input_event &e);
  void query_trigs_and_shoulders(const input_event &e);
  void query_analogs(const input_event &e);

  float normalize_analog_input(int axisCode, int rawInput);

  libevdev *_gamepad = nullptr;
  std::array<float, moveDirectionCount + 1> _mvmtKeyStates{};
  std::array<libevdev *, 3> _dualsense_devs{};

  // notcurses key ids (Unicode codepoints) that rawKeyPressed understands
  const uint32_t _keyOne = '1';
  const uint32_t _keyTwo = '2';
  const uint32_t _keyThree = '3';
  const uint32_t _keyX = 'x';

  // true only for the frame in which the options button went down
  bool _optionsPressed = false;

  // dualsense identifiers
  const char *_ds_vendor_id = "054c";
  const char *_ds_product_id = "0ce6";

  bool _triangle_pressed = false;
  bool _circle_pressed = false;
  bool _cross_pressed = false;

  // analog abs info
  int _ls_min_x;
  int _ls_min_y;
  int _ls_max_x;
  int _ls_max_y;

  // we do not care about right stick y
  int _rs_min_x;
  int _rs_max_x;

  // deadzones
  int _ls_dz_x;
  int _ls_dz_y;
  int _rs_dz_x;
};

#endif // DUALSENSEINPUTHANDLER_H
