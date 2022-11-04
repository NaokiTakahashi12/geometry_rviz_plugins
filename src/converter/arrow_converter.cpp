
#include <geometry_rviz_plugins/converter/arrow_converter.hpp>

#include <cmath>

namespace geometry_rviz_plugins::converter
{
    void rvizArrowConverter
    (
        rviz_rendering::Arrow &rviz_arrow,
        const geometry_msgs::msg::Vector3 &msg,
        const Ogre::Quaternion &quaternion,
        const ConvertArrowProperties &convert_arrow_properties
    ) {
        const float vector_norm = convert_arrow_properties.arrow_scale * std::sqrt
        (
            std::pow(msg.x, 2) +
            std::pow(msg.y, 2) +
            std::pow(msg.z, 2)
        );
        const float arrow_head_length = vector_norm * convert_arrow_properties.head_scale;
        const float arrow_shaft_length = vector_norm - arrow_head_length;

        const Ogre::Vector3 vector = Ogre::Vector3
        (
            msg.x,
            msg.y,
            msg.z
        );

        rviz_arrow.set
        (
            arrow_shaft_length,
            convert_arrow_properties.shaft_radius,
            arrow_head_length,
            convert_arrow_properties.head_radius
        );
        rviz_arrow.setDirection
        (
            quaternion * vector
        );
    }
}
