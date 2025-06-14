#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

struct view_vector_coordinates{
    float x; //!< Member variable "x"
    float y; //!< Member variable "y"
    float z; //!< Member variable "z"
}typedef DIRECAO;

struct camera_angles{
    float phi_angle;
    float theta_angle;
    float distance_from_origin;
}typedef ANGULOS;

class camera
{
    public:
        /** Default constructor */
        camera();
        /** Default destructor */
        virtual ~camera();

        /** Set angulos
         * \param angulos New value to set
         */
        void Setangulos(ANGULOS angulos) { this->angulos.phi_angle = angulos.phi_angle, this->angulos.theta_angle = angulos.theta_angle, this->angulos.distance_from_origin = angulos.distance_from_origin; }

        /** Access angulos
         * \return The current value of angulos
         */
         ANGULOS Getangulos() { return angulos; }

        /** Set direcao
         * \param direcao New value to set
         */
        void Setdirecao(DIRECAO direcao) { this->direcao.x = direcao.x, this->direcao.y = direcao.y, this->direcao.z = direcao.z; }

        /** Access direcao
         * \return The current value of direcao
         */
        DIRECAO Getdirecao() { return direcao; }

        /** Access view_vector
         * \return The current value of view_vector
         */
        glm::vec4 Getview_vector() { return view_vector; }

        /** Set view_vector
         * \param val New value to set
         */
        void Setview_vector(glm::vec4 val) { view_vector = val; }

        /** Access up_vector
         * \return The current value of up_vector
         */
        glm::vec4 Getup_vector() { return up_vector; }

        /** Set up_vector
         * \param val New value to set
         */
        void Setup_vector(glm::vec4 val) { up_vector = val; }

        /** Access w_vector
         * \return The current value of w_vector
         */
        glm::vec4 Getw_vector() { return w_vector; }

        /** Set w_vector
         * \param val New value to set
         */
        void Setw_vector(glm::vec4 val) { w_vector = val; }

        /** Access u_vector
         * \return The current value of u_vector
         */
        glm::vec4 Getu_vector() { return u_vector; }

        /** Set u_vector
         * \param val New value to set
         */
        void Setu_vector(glm::vec4 val) { u_vector = val; }

        /** Access c_position
         * \return The current value of c_position
         */
        glm::vec4 Getc_position() { return c_position; }

        /** Set c_position
         * \param val New value to set
         */
        void Setc_position(glm::vec4 val) { c_position = val; }

        /** Access camera_speed
         * \return The current value of camera_speed
         */
        float Getcamera_speed() { return camera_speed; }

        /** Set camera_speed
         * \param val New value to set
         */
        void Setcamera_speed(float val) { camera_speed = val; }

    protected:

    private:
        ANGULOS angulos;
        DIRECAO direcao;
        glm::vec4 view_vector; //!< Member variable "view_vector"
        glm::vec4 up_vector; //!< Member variable "up_vector"
        glm::vec4 w_vector; //!< Member variable "w_vector"
        glm::vec4 u_vector; //!< Member variable "u_vector"
        glm::vec4 c_position; //!< Member variable "c_position"
        float camera_speed; //!< Member variable "camera_speed"
};

#endif // CAMERA_H
