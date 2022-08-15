#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

struct SCamera{
    float x,y,z;
    float Xrot, Yrot, Zrot;
}camera;

void CameraApply();

void CameraRotation(float xAngle, float zAngle);

void CameraAutoMoveByMouse(int centerX, int centerY, float speed);

void CameraMoveDirection(int forward, int right, float speed);

#endif // CAMERA_H_INCLUDED
