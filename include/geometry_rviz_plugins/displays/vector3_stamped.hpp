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

#ifndef GEOMETRY_RVIZ_PLUGINS__DISPLAYS__VECTOR3_STAMPED_HPP_
#define GEOMETRY_RVIZ_PLUGINS__DISPLAYS__VECTOR3_STAMPED_HPP_

#include <memory>

#include <rviz_common/message_filter_display.hpp>

#include <rviz_common/properties/float_property.hpp>
#include <rviz_common/properties/color_property.hpp>
#include <rviz_common/properties/vector_property.hpp>

#include <rviz_rendering/objects/arrow.hpp>

#include <geometry_msgs/msg/vector3_stamped.hpp>

#include <geometry_rviz_plugins/converter/converter.hpp>


namespace geometry_rviz_plugins::displays
{
class Vector3StampedDisplay
  : public
  rviz_common::MessageFilterDisplay<geometry_msgs::msg::Vector3Stamped>
{
  Q_OBJECT

public:
  Vector3StampedDisplay();
  explicit Vector3StampedDisplay(rviz_common::DisplayContext *);
  ~Vector3StampedDisplay() override;

  void reset() override;
  void processMessage(geometry_msgs::msg::Vector3Stamped::ConstSharedPtr) override;

protected:
  void onInitialize() override;

private Q_SLOTS:
  void arrowPropertyCallback();

private:
  const float default_color_alpha_,
    default_shaft_radius_,
    default_head_radius_,
    default_head_scale_;

  std::unique_ptr<rviz_common::properties::ColorProperty> arrow_color_property_;

  std::unique_ptr<rviz_common::properties::FloatProperty> color_alpha_property_,
    shaft_radius_property_,
    head_radius_property_,
    head_scale_property_;

  std::unique_ptr<rviz_common::properties::VectorProperty> position_offset_property_;

  std::unique_ptr<rviz_rendering::Arrow> rviz_arrow_;

  converter::ConvertArrowProperties convert_arrow_properties_;

  void initializeRvizArrow();
  void updateArrowLocalProperties();

  void destroyRenderingObjects();
};
}  // namespace geometry_rviz_plugins::displays
#endif  // GEOMETRY_RVIZ_PLUGINS__DISPLAYS__VECTOR3_STAMPED_HPP_
