#include "precomp.h"

namespace RaytracingRenderer {
    //Constructor with custom camera position
    Camera::Camera(float3 cameraPos, float fov, float aspect_ratio)
    {
        this->cameraPos = cameraPos;
        this->fov = fov;
        this->aspect_ratio = aspect_ratio;
        updateViewport();
    }

    Camera::Camera() {
        this->cameraPos = float3(0.f, 0.f, -1.f);
        updateViewport();
	}

    //Update camera view matrix with new values (used after view is moved)
    void Camera::updateCameraVectors()
    {
        //Update camera view angle
        cameraLook = normalize(getDirection(yaw, pitch));
        cameraRight = normalize(cross(up, cameraLook));
        cameraUp = cross(cameraLook, cameraRight);
    }

	// If the cameraPos point was moved, this function can update the viewport accordingly.
    void Camera::updateViewport()
    {
        updateCameraVectors();

        // Screen center : 𝐶 = 𝐸 + 𝑑𝑉, with screen distance 𝑑. Change FOV by altering 𝑑;
        float3 screen_center = cameraPos + fov * cameraLook;

        // Make sure the viewport size lines up with the screen resolution.
        float viewport_height = 2.f;
        float viewport_width = aspect_ratio * viewport_height;

        // Divide by 2 to traverse half the width/height away from the center.
        //Recalculate horz/vert with new camera axis
        float3 horizontal = cameraRight * viewport_width / 2;
        float3 vertical = cameraUp * viewport_height / 2;

        // Screen corners : 𝑃0 = 𝐶 + (−1, −1, 0), 𝑃1 = 𝐶 + (1, −1, 0), 𝑃2 = 𝐶 + (−1, 1, 0)
        screen_p0 = screen_center - horizontal + vertical;
        screen_p1 = screen_center + horizontal + vertical;
        screen_p2 = screen_center - horizontal - vertical;
    }

    //Update camera position on WASD keypress, modulated by cameraSpeed variable
    void Camera::keyHandler(int key)
    {
        const float cameraSpeed = 0.5f;
        switch (key)
        {
        case 87: //W
            //cout << "BUTTON PUSHED ";
            cameraPos += cameraLook * cameraSpeed;
            break;
        case 83: //S
            cameraPos -= cameraLook * cameraSpeed;
            break;
        case 65: //A
            cameraPos -= cameraRight * cameraSpeed;
            break;
        case 68: //D
            cameraPos += cameraRight * cameraSpeed;
            break;
        }
        updateViewport();
    }

    //Update camera view angle on mouse movement + mouse button one pressed
    void Camera::mouseHandler(int x, int y)
    {
        //Prevent look from stuttering when mouse leaves the window
        if (firstMouse)
        {
            lastX = float(x);
            lastY = float(y);
            firstMouse = false;
        }

        //Update last known mouse coordinates, get the offset from new coordinates
        float xoffset = x - lastX;
        float yoffset = lastY - y;
        lastX = float(x);
        lastY = float(y);

        //Adjust for mouse sensitivity
        float sensitivity = 0.3f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        //Adjust pitch + yaw
        yaw += xoffset;
        pitch -= yoffset;

        //Lock camera's Y-axis between (-45, 45) degrees to prevent full rolling
        if (pitch > 45.0f)
            pitch = 45.0f;
        if (pitch < -45.0f)
            pitch = -45.0f;
        
        updateViewport();
    }

    //Calculates new camera look vector
    float3 Camera::getDirection(float yaw, float pitch)
    {			
        yaw *= (PI / 180);
        pitch *= (PI / 180);

        float3 direction;
        direction.x = cos((yaw)) * cos((pitch));
        direction.y = sin((pitch));
        direction.z = sin((yaw)) * cos((pitch));


        return direction;
    }
}