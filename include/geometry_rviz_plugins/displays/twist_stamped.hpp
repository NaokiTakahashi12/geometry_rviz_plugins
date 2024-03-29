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

#ifndef GEOMETRY_RVIZ_PLUGINS__DISPLAYS__TWIST_STAMPED_HPP_
#define GEOMETRY_RVIZ_PLUGINS__DISPLAYS__TWIST_STAMPED_HPP_

#include <memory>

#include <rviz_common/message_filter_display.hpp>

#include <rviz_common/properties/float_property.hpp>
#include <rviz_common/properties/color_property.hpp>
#include <rviz_common/properties/vector_property.hpp>

#include <rviz_rendering/objects/arrow.hpp>

#include <geometry_msgs/msg/vector3_stamped.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>

#include <geometry_rviz_plugins/converter/converter.hpp>


namespace geometry_rviz_plugins::displays
{
class TwistStampedDisplay
  : public
  rviz_common::MessageFilterDisplay<geometry_msgs::msg::TwistStamped>
{
  Q_OBJECT

public:
  TwistStampedDisplay();
  explicit TwistStampedDisplay(rviz_common::DisplayContext *);
  ~TwistStampedDisplay() override;

  void reset() override;
  void processMessage(geometry_msgs::msg::TwistStamped::ConstSharedPtr) override;

protected:
  void onInitialize() override;

private Q_SLOTS:
  void linearPropertyCallback();
  void angularPropertyCallback();

private:
  const float default_linear_color_alpha_,
    default_linear_shaft_radius_,
    default_linear_head_radius_,
    default_linear_head_scale_,
    default_linear_arrow_scale_;

  const float default_angular_color_alpha_,
    default_angular_shaft_radius_,
    default_angular_head_radius_,
    default_angular_head_scale_,
    default_angular_arrow_scale_;

  converter::ConvertArrowProperties linear_arrow_properties_,
    angular_arrow_properties_;

  std::unique_ptr<rviz_common::properties::ColorProperty> linear_color_property_;
  std::unique_ptr<rviz_common::properties::FloatProperty> linear_color_alpha_property_,
    linear_shaft_radius_property_,
    linear_head_radius_property_,
    linear_head_scale_property_,
    linear_arrow_scale_property_;

  std::unique_ptr<rviz_common::properties::ColorProperty> angular_color_property_;
  std::unique_ptr<rviz_common::properties::FloatProperty> angular_color_alpha_property_,
    angular_shaft_radius_property_,
    angular_head_radius_property_,
    angular_head_scale_property_,
    angular_arrow_scale_property_;

  std::unique_ptr<rviz_rendering::Arrow> rviz_linear_arrow_,
    rviz_angular_arrow_;

  void updateTwistRendering(
    const geometry_msgs::msg::TwistStamped::ConstSharedPtr msg,
    const Ogre::Vector3 & ogre_position,
    const Ogre::Quaternion & ogre_quaternion
  );

  void updateLinearArrowLocalProperties();
  void updateAngularArrowLocalProperties();
  void initializeRenderingObjects();
  void destroyRenderingObjects();
};
}  // namespace geometry_rviz_plugins::displays
#endif  // GEOMETRY_RVIZ_PLUGINS__DISPLAYS__TWIST_STAMPED_HPP_
