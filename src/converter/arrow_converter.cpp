// Copyright (c) 2022 Naoki Takahashi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
