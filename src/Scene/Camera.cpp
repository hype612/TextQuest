#include "../Headers/Camera.h"
#include "../Headers/Logger.h"

Camera::Camera(const Transform &follow) : _follow(follow) {}
float Camera::fov() const { return _fov; }
float Camera::fovDegrees() const {
  return (_fov * 180 / std::numbers::pi_v<float>);
}
const Transform &Camera::follow() const { return _follow; }
void Camera::setFollow(const Transform &new_follow) { _follow = new_follow; }
void Camera::setFov(float new_fov) {
  if (new_fov > 2.f * std::numbers::pi_v<float> ||
      new_fov < -2.f * std::numbers::pi_v<float>) {
    Logger::GetInstance()->log(
        "setFov was called with " + std::to_string(new_fov) +
            ". This is either degress or non-normalized radians.",
        LogType::CORE, LogLevel::WARNING);
  }
  _fov = new_fov;
}
void Camera::setFovDegrees(float new_fovDegrees) {
  if (new_fovDegrees < 2.f * std::numbers::pi_v<float> ||
      new_fovDegrees > -2.f * std::numbers::pi_v<float>) {
    Logger::GetInstance()->log("setFovDegrees was called with " +
                                   std::to_string(new_fovDegrees) +
                                   ". This may be in radians.",
                               LogType::CORE, LogLevel::WARNING);
  }
  _fov = new_fovDegrees * std::numbers::pi_v<float> / 180;
}
