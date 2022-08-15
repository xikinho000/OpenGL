#include <GL/gl.h>
#include <windows.h>
#include <math.h>
#include "camera.h"

struct SCamera camera = {0,0, 1.7,0,0};


void CameraApply()
{
    glRotatef(-camera.Xrot, 1,0,0);
    glRotatef(-camera.Zrot, 0,0,1);
    glTranslatef(-camera.x, -camera.y, -camera.z);
};

void CameraRotation(float xAngle, float zAngle)
{
    camera.Zrot += zAngle;
    if(camera.Zrot < 0) camera.Zrot += 360;
    if(camera.Zrot > 360) camera.Zrot -= 360;
    camera.Xrot += xAngle;
    if(camera.Xrot < 0 )camera.Xrot = 0;
    if(camera.Xrot > 180 )camera.Xrot = 180;
}

void CameraAutoMoveByMouse(int centerX, int centerY, float speed)
{
    POINT cur;
    POINT base = {centerX,centerY};
    GetCursorPos(&cur);
    CameraRotation((base.y - cur.y) * speed, (base.x - cur.x) *speed );
    SetCursorPos(base.x, base.y);

}


void CameraMoveDirection(int forward, int right, float speed)
{
    float ugol = -camera.Zrot / 180 * M_PI;
    if(forward > 0)
        ugol += right > 0 ? M_PI_4 : (right < 0 ? -M_PI_4 : 0);
    if(forward < 0)
        ugol += M_PI + (right > 0 ? - M_PI_4 : (right < 0 ? M_PI_4 : 0));
    if(forward == 0){
        ugol += right > 0 ? M_PI_2 : -M_PI_2;
        if(right == 0 ) speed = 0;
    }
    if( speed != 0 ){
        camera.x += sin(ugol) * speed;
        camera.y += cos(ugol) * speed;
    }
}
