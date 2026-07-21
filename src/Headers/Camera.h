#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include <functional>

class Camera {
public:
  Camera(const Transform &follow);
  float fov() const;
  float fovDegrees() const;
  const Transform &follow() const;

  void setFollow(const Transform &new_follow);
  void setFov(float new_fov);
  void setFovDegrees(float new_fovDegrees);

  Camera() = delete;
  Camera(const Camera &g) = delete;
  Camera &operator=(const Camera &) = delete;
  Camera(Camera &) = delete;
  Camera &operator=(Camera &&) = delete;

private:
  std::reference_wrapper<const Transform> _follow;
  float _fov;
};

#endif // _CAMERA_H
