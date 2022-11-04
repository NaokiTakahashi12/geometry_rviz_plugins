
#pragma once

#include <rviz_rendering/objects/arrow.hpp>

#include <geometry_msgs/msg/vector3_stamped.hpp>

#include "convert_arrow_properties.hpp"

namespace geometry_rviz_plugins::converter
{
    void rvizArrowConverter
    (
        rviz_rendering::Arrow &,
        const geometry_msgs::msg::Vector3 &,
        const Ogre::Quaternion &,
        const ConvertArrowProperties &
    );
}
