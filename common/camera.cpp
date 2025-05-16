#include <common/camera.hpp>

glm::mat4 Camera::calculateViewMatrix(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
    glm::vec3 zAxis = glm::normalize(eye - target);            //Forward
    glm::vec3 xAxis = glm::normalize(glm::cross(up, zAxis));   //Right
    glm::vec3 yAxis = glm::cross(zAxis, xAxis);                //Up

    glm::mat4 view(1.0f);
    view[0][0] = xAxis.x; view[1][0] = xAxis.y; view[2][0] = xAxis.z;
    view[0][1] = yAxis.x; view[1][1] = yAxis.y; view[2][1] = yAxis.z;
    view[0][2] = zAxis.x; view[1][2] = zAxis.y; view[2][2] = zAxis.z;
    view[3][0] = -glm::dot(xAxis, eye);
    view[3][1] = -glm::dot(yAxis, eye);
    view[3][2] = -glm::dot(zAxis, eye);

    return view;
}

glm::mat4 Camera::calculateProjectionMatrix(float fov, float aspect, float near, float far) {
    float tanHalfFov = tan(fov / 2.0f);

    glm::mat4 proj(0.0f);
    proj[0][0] = 1.0f / (aspect * tanHalfFov);
    proj[1][1] = 1.0f / (tanHalfFov);
    proj[2][2] = -(far + near) / (far - near);
    proj[2][3] = -1.0f;
    proj[3][2] = -(2.0f * far * near) / (far - near);

    return proj;
}

Camera::Camera(const glm::vec3 Eye, const glm::vec3 Target)
{
    eye = Eye;
    target = Target;
}

void Camera::calculateMatrices()
{
    view = calculateViewMatrix(eye, eye + front, worldUp);
    projection = calculateProjectionMatrix(fov, aspect, near, far);
}


void Camera::calculateCameraVectors() {
    // Calculate front vector from yaw and pitch angles
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // Calculate right vector
    right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

    // Calculate up vector
    up = glm::normalize(glm::cross(right, front));

    // Constrain pitch to avoid gimbal lock
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
}

void Camera::quaternionCamera()
{
    Quaternion newOrientation(-pitch, yaw);
    orientation = Maths::SLERP(orientation, newOrientation, 0.2f);

    view = orientation.matrix() * Maths::translate(-eye);
    projection = calculateProjectionMatrix(fov, aspect, near, far);

    right = glm::vec3(view[0][0], view[1][0], view[2][0]);
    up = glm::vec3(view[0][1], view[1][1], view[2][1]);
    front = -glm::vec3(view[0][2], view[1][2], view[2][2]);
}

