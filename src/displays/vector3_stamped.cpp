
#include <geometry_rviz_plugins/displays/vector3_stamped.hpp>

#include <cmath>

#include <chrono>
#include <iostream>

#include <rviz_common/msg_conversions.hpp>


namespace geometry_rviz_plugins::displays
{
    Vector3StampedDisplay::Vector3StampedDisplay()
    : default_color_alpha_(1.0),
      default_shaft_radius_(0.05),
      default_head_radius_(0.1),
      default_head_scale_(0.2)
    {
        arrow_color_property_.reset
        (
            new rviz_common::properties::ColorProperty
            (
                "Color",
                QColor(200, 200, 200),
                "Color to draw the vector arrow.",
                this
            )
        );

        color_alpha_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Alpha",
                default_color_alpha_,
                "Vector transparency.",
                this
            )
        );
        color_alpha_property_->setMin(0);
        color_alpha_property_->setMax(1);

        shaft_radius_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Shaft Radius",
                default_shaft_radius_,
                "Shaft radius of the vector.",
                this
            )
        );
        shaft_radius_property_->setMin(0);
        shaft_radius_property_->setMax(1);

        head_radius_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Head Radius",
                default_head_radius_,
                "Head radius of the vector.",
                this
            )
        );
        head_radius_property_->setMin(0);
        head_radius_property_->setMax(1);

        head_scale_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Head Scale",
                default_head_scale_,
                "Head length scale of the vector.",
                this
            )
        );
        head_scale_property_->setMin(0);
        head_scale_property_->setMax(1);

        position_offsed_property_.reset
        (
            new rviz_common::properties::VectorProperty
            (
                "Offset the vector from the origin of the reference frame.",
                Ogre::Vector3::ZERO,
                "",
                this
            )
        );
    }

    Vector3StampedDisplay::Vector3StampedDisplay(rviz_common::DisplayContext *context)
    : Vector3StampedDisplay()
    {
        this->context_ = context;
        this->scene_manager_ = context->getSceneManager();
        this->scene_node_ = this->scene_manager_->getRootSceneNode()->createChildSceneNode();

        initializeRvizArrow();
    }

    Vector3StampedDisplay::~Vector3StampedDisplay()
    {
        destroyRenderingObjects();
        destroyProperties();
    }

    void Vector3StampedDisplay::reset()
    {
        destroyRenderingObjects();

        initializeRvizArrow();

        MFDClass::reset();
    }

    void Vector3StampedDisplay::processMessage(geometry_msgs::msg::Vector3Stamped::ConstSharedPtr msg)
    {
        const float shaft_radius = shaft_radius_property_->getFloat(),
                    head_radius = head_radius_property_->getFloat(),
                    head_scale = head_scale_property_->getFloat();

        Ogre::Vector3 position;
        Ogre::Quaternion quaternion;

        const bool is_transformable_frame = this->context_->getFrameManager()->getTransform(msg->header, position, quaternion);

        if(! is_transformable_frame)
        {
            this->setMissingTransformToFixedFrame(msg->header.frame_id);
            return;
        }
        this->setTransformOk();

        const float vector_norm = std::sqrt
        (
          std::pow(msg->vector.x, 2) +
          std::pow(msg->vector.y, 2) +
          std::pow(msg->vector.z, 2)
        );
        const float arrow_head_length = vector_norm * head_scale;
        const float arrow_shaft_length = vector_norm - arrow_head_length;

        const QColor arrow_color = arrow_color_property_->getColor();
        //arrow_color.a = color_alpha_property_->getFloat();

        Ogre::Vector3 ogre_vector3;
        ogre_vector3 = Ogre::Vector3
        (
            msg->vector.x,
            msg->vector.y,
            msg->vector.z
        );

        rviz_arrow_->set
        (
            arrow_shaft_length,
            shaft_radius,
            arrow_head_length,
            head_radius
        );
        rviz_arrow_->setColor
        (
            arrow_color.redF(),
            arrow_color.greenF(),
            arrow_color.blueF(),
            color_alpha_property_->getFloat()
        );
        rviz_arrow_->setPosition
        (
            position
        );
        rviz_arrow_->setDirection
        (
            quaternion * ogre_vector3
        );

        this->context_->queueRender();
    }

    void Vector3StampedDisplay::onInitialize()
    {
        MFDClass::onInitialize();
    }

    void Vector3StampedDisplay::initializeRvizArrow()
    {
        rviz_arrow_ = std::make_unique<rviz_rendering::Arrow>
        (
            this->scene_manager_,
            this->scene_node_
        );
        rviz_arrow_->set(0, 0, 0, 0);
    }

    void Vector3StampedDisplay::destroyRenderingObjects()
    {
        rviz_arrow_.reset();
    }

    void Vector3StampedDisplay::destroyProperties()
    {
        shaft_radius_property_.reset();
        head_radius_property_.reset();
        head_scale_property_.reset();
    }
}

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(geometry_rviz_plugins::displays::Vector3StampedDisplay, rviz_common::Display)

