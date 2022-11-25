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

#include <geometry_rviz_plugins/displays/vector3_stamped.hpp>

#include <rviz_common/msg_conversions.hpp>
#include <pluginlib/class_list_macros.hpp>

#include <cmath>

#include <memory>
#include <chrono>


namespace geometry_rviz_plugins::displays
{
Vector3StampedDisplay::Vector3StampedDisplay()
: default_color_alpha_(1.0),
  default_shaft_radius_(0.05),
  default_head_radius_(0.1),
  default_head_scale_(0.2)
{
  arrow_color_property_.reset(
    new rviz_common::properties::ColorProperty(
      "Color",
      QColor(200, 200, 200),
      "Color to draw the vector arrow.",
      this
    )
  );

  color_alpha_property_.reset(
    new rviz_common::properties::FloatProperty(
      "Alpha",
      default_color_alpha_,
      "Vector transparency.",
      this
    )
  );
  color_alpha_property_->setMin(0);
  color_alpha_property_->setMax(1);

  shaft_radius_property_.reset(
    new rviz_common::properties::FloatProperty(
      "Shaft Radius",
      default_shaft_radius_,
      "Shaft radius of the vector.",
      this,
      SLOT(arrowPropertyCallback())
    )
  );
  shaft_radius_property_->setMin(0);

  head_radius_property_.reset(
    new rviz_common::properties::FloatProperty(
      "Head Radius",
      default_head_radius_,
      "Head radius of the vector.",
      this,
      SLOT(arrowPropertyCallback())
    )
  );
  head_radius_property_->setMin(0);

  head_scale_property_.reset(
    new rviz_common::properties::FloatProperty(
      "Head Scale",
      default_head_scale_,
      "Head length scale of the vector.",
      this,
      SLOT(arrowPropertyCallback())
    )
  );
  head_scale_property_->setMin(0);
  head_scale_property_->setMax(1);

  position_offset_property_.reset(
    new rviz_common::properties::VectorProperty(
      "Offset the vector from the origin of the reference frame.",
      Ogre::Vector3::ZERO,
      "",
      this,
      SLOT(arrowPropertyCallback())
    )
  );
}

Vector3StampedDisplay::Vector3StampedDisplay(rviz_common::DisplayContext * context)
: Vector3StampedDisplay()
{
  this->context_ = context;
  this->scene_manager_ = context->getSceneManager();
  this->scene_node_ = this->scene_manager_->getRootSceneNode()->createChildSceneNode();

  updateArrowLocalProperties();

  initializeRvizArrow();
}

Vector3StampedDisplay::~Vector3StampedDisplay()
{
  destroyRenderingObjects();
}

void Vector3StampedDisplay::reset()
{
  destroyRenderingObjects();

  updateArrowLocalProperties();

  initializeRvizArrow();

  MFDClass::reset();
}

void Vector3StampedDisplay::processMessage(geometry_msgs::msg::Vector3Stamped::ConstSharedPtr msg)
{
  const Ogre::Vector3 offset_vector = position_offset_property_->getVector();

  Ogre::Vector3 position;
  Ogre::Quaternion quaternion;

  const bool is_transformable_frame = this->context_->getFrameManager()->getTransform(
    msg->header,
    position,
    quaternion
  );

  if (!is_transformable_frame) {
    this->setMissingTransformToFixedFrame(msg->header.frame_id);
    return;
  }
  this->setTransformOk();

  position.x += offset_vector.x;
  position.y += offset_vector.y;
  position.z += offset_vector.z;

  converter::rvizArrowConverter(
    *rviz_arrow_,
    msg->vector,
    quaternion,
    convert_arrow_properties_
  );
  rviz_arrow_->setPosition(
    position
  );

  const QColor arrow_color = arrow_color_property_->getColor();

  rviz_arrow_->setColor(
    arrow_color.redF(),
    arrow_color.greenF(),
    arrow_color.blueF(),
    color_alpha_property_->getFloat()
  );

  this->context_->queueRender();
}

void Vector3StampedDisplay::onInitialize()
{
  MFDClass::onInitialize();
}

void Vector3StampedDisplay::arrowPropertyCallback()
{
  updateArrowLocalProperties();
}

void Vector3StampedDisplay::initializeRvizArrow()
{
  rviz_arrow_ = std::make_unique<rviz_rendering::Arrow>(
    this->scene_manager_,
    this->scene_node_
  );
  rviz_arrow_->set(0, 0, 0, 0);
}

void Vector3StampedDisplay::updateArrowLocalProperties()
{
  convert_arrow_properties_.arrow_scale = 1;
  convert_arrow_properties_.head_scale = head_scale_property_->getFloat();
  convert_arrow_properties_.head_radius = head_radius_property_->getFloat();
  convert_arrow_properties_.shaft_radius = shaft_radius_property_->getFloat();
}

void Vector3StampedDisplay::destroyRenderingObjects()
{
  rviz_arrow_.reset();
}
}  // namespace geometry_rviz_plugins::displays

PLUGINLIB_EXPORT_CLASS(geometry_rviz_plugins::displays::Vector3StampedDisplay, rviz_common::Display)
