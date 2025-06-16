#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera
{
    public:
        /** Default constructor */
        camera(glm::vec4 camera_position, glm::vec4 view_vector, glm::vec4 camera_foward_move_vector);
        /** Default destructor */
        virtual ~camera();

        /** Access camera_position
         * \return The current value of camera_position
         */
        glm::vec4 getCameraPosition() { return camera_position; }

        /** Set camera_position
         * \param camera_position New value to set
         */
        void setCameraPosition(glm::vec4 camera_position) { this->camera_position = camera_position; }

        /** Access view_vector
         * \return The current value of view_vector
         */
        glm::vec4 getViewVector() { return view_vector; }

        /** Set view_vector
         * \param view_vector New value to set
         */
        void setViewVector(glm::vec4 view_vector) { this->view_vector = view_vector; }

        /** Access camera_foward_move_vector
         * \return The current value of camera_foward_move_vector
         */
        glm::vec4 getCameraFowardMoveVector() { return camera_foward_move_vector; }

        /** Set camera_foward_move_vector
         * \param camera_foward_move_vector New value to set
         */
        void setCameraFowardMoveVector(glm::vec4 camera_foward_move_vector) { this->camera_foward_move_vector = camera_foward_move_vector; }

        /** Access up_vector
         * \return The current value of up_vector
         */
        glm::vec4 getUpVector() { return up_vector; }

        /** Access w_vector
         * \return The current value of w_vector
         */
        glm::vec4 getCameraWVector() { return w_vector; }

        /** Set w_vector
         * \param w_vector New value to set
         */
        void setCameraWVector(glm::vec4 w_vector) { this->w_vector = w_vector; }

        /** Access u_vector
         * \return The current value of u_vector
         */
        glm::vec4 getCameraUVector() { return u_vector; }

        /** Set u_vector
         * \param u_vector New value to set
         */
        void setCameraUVector(glm::vec4 u_vector) { this->u_vector = u_vector; }

        /** Access camera_speed
         * \return The current value of camera_speed
         */
        float getCameraSpeed() { return camera_speed; }

    protected:

    private:
        glm::vec4 camera_position; //!< Member variable "camera_position"
        glm::vec4 view_vector; //!< Member variable "view_vector"
        glm::vec4 camera_foward_move_vector; //!< Member variable "camera_foward_move_vector"
        const glm::vec4 up_vector = glm::vec4(0.0f,1.0f,0.0f,0.0f); //!< Member variable "up_vector" // Vetor "up" fixado para apontar para o "céu" (eixo Y global)
        glm::vec4 w_vector; //!< Member variable "w_vector"
        glm::vec4 u_vector; //!< Member variable "u_vector"
        const float camera_speed = 0.018f; //!< Member variable "camera_speed"
};

#endif // CAMERA_H
