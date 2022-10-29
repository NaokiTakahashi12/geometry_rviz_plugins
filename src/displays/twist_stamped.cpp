
#include <geometry_rviz_plugins/displays/twist_stamped.hpp>

namespace geometry_rviz_plugins::displays
{
    TwistStampedDisplay::TwistStampedDisplay()
    : default_linear_color_alpha_(1.0),
      default_linear_shaft_radius_(0.05),
      default_linear_head_radius_(0.1),
      default_linear_head_scale_(0.4),
      default_linear_arrow_scale_(1.0),
      default_angular_color_alpha_(1.0),
      default_angular_shaft_radius_(0.05),
      default_angular_head_radius_(0.1),
      default_angular_head_scale_(0.4),
      default_angular_arrow_scale_(1.0)
    {
        linear_color_property_.reset
        (
            new rviz_common::properties::ColorProperty
            (
                "Linear Arrow Color",
                QColor(150, 200, 150),
                "Color to draw the twist linear vector arrow.",
                this
            )
        );
        linear_color_alpha_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Linear Color Alpha",
                default_linear_color_alpha_,
                "Twist linear arrow transparency.",
                this
            )
        );
        linear_color_alpha_property_->setMin(0);
        linear_color_alpha_property_->setMax(1);

        linear_shaft_radius_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Linear Shaft Radius",
                default_linear_shaft_radius_,
                "Shaft radius of the linear arrow.",
                this
            )
        );
        linear_shaft_radius_property_->setMin(0);

        linear_head_radius_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Linear Head Radius",
                default_linear_head_radius_,
                "Head radius of the linear arrow.",
                this
            )
        );
        linear_head_radius_property_->setMin(0);

        linear_head_scale_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Linear Head Scale",
                default_linear_head_scale_,
                "Head length scale of the twist linear arrow.",
                this
            )
        );
        linear_head_scale_property_->setMin(0);
        linear_head_scale_property_->setMax(1);

        linear_arrow_scale_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Linear Arrow Scale",
                default_linear_arrow_scale_,
                "Arrow scale of the twist linear.",
                this
            )
        );
        linear_arrow_scale_property_->setMin(0);

        angular_color_property_.reset
        (
            new rviz_common::properties::ColorProperty
            (
                "Angular Arrow Color",
                QColor(100, 100, 200),
                "Color to draw the twist angular vector arrow.",
                this
            )
        );
        angular_color_alpha_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Angular Color Alpha",
                default_angular_color_alpha_,
                "Twist angular arrow transparency.",
                this
            )
        );
        angular_color_alpha_property_->setMin(0);
        angular_color_alpha_property_->setMax(1);

        angular_shaft_radius_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Angular Shaft Radius",
                default_angular_shaft_radius_,
                "Shaft radius of the angular arrow.",
                this
            )
        );
        angular_shaft_radius_property_->setMin(0);

        angular_head_radius_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Angular Head Radius",
                default_angular_head_radius_,
                "Head radius of the angular arrow.",
                this
            )
        );
        angular_head_radius_property_->setMin(0);

        angular_head_scale_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Angular Head Scale",
                default_angular_head_scale_,
                "Head length scale of the twist angular arrow.",
                this
            )
        );
        angular_head_scale_property_->setMin(0);
        angular_head_scale_property_->setMax(1);

        angular_arrow_scale_property_.reset
        (
            new rviz_common::properties::FloatProperty
            (
                "Angular Arrow Scale",
                default_angular_arrow_scale_,
                "Arrow scale of the twist angular.",
                this
            )
        );
        angular_arrow_scale_property_->setMin(0);
    }

    TwistStampedDisplay::TwistStampedDisplay(rviz_common::DisplayContext *context)
    : TwistStampedDisplay()
    {
        this->context_ = context;
        this->scene_manager_ = context->getSceneManager();
        this->scene_node_ = this->scene_manager_->getRootSceneNode()->createChildSceneNode();

        initializeRenderingObjects();
    }

    TwistStampedDisplay::~TwistStampedDisplay()
    {
        destroyRenderingObjects();
    }

    void TwistStampedDisplay::reset()
    {
        destroyRenderingObjects();

        initializeRenderingObjects();

        MFDClass::reset();
    }

    void TwistStampedDisplay::processMessage(geometry_msgs::msg::TwistStamped::ConstSharedPtr msg)
    {
        Ogre::Vector3 ogre_position;
        Ogre::Quaternion ogre_quaternion;

        const bool is_transformable_frame = this->context_->getFrameManager()->getTransform
        (
            msg->header,
            ogre_position,
            ogre_quaternion
        );
        
        if(!is_transformable_frame)
        {
            this->setMissingTransformToFixedFrame(msg->header.frame_id);
            return;
        }
        this->setTransformOk();

        updateTwistRendering(msg, ogre_position, ogre_quaternion);

        this->context_->queueRender();
    }

    void TwistStampedDisplay::onInitialize()
    {
        MFDClass::onInitialize();
    }

    void TwistStampedDisplay::updateTwistRendering
    (
        const geometry_msgs::msg::TwistStamped::ConstSharedPtr msg,
        const Ogre::Vector3 &ogre_position,
        const Ogre::Quaternion &ogre_quaternion
    )
    {
        updateArrowObject
        (
            *rviz_linear_arrow_,
            msg->twist.linear,
            ogre_position,
            ogre_quaternion,
            linear_arrow_scale_property_->getFloat(),
            linear_head_scale_property_->getFloat(),
            linear_head_radius_property_->getFloat(),
            linear_shaft_radius_property_->getFloat()
        );

        const QColor linear_arrow_color =  linear_color_property_->getColor();

        rviz_linear_arrow_->setColor
        (
            linear_arrow_color.redF(),
            linear_arrow_color.greenF(),
            linear_arrow_color.blueF(),
            linear_color_alpha_property_->getFloat()
        );

        updateArrowObject
        (
            *rviz_angular_arrow_,
            msg->twist.angular,
            ogre_position,
            ogre_quaternion,
            angular_arrow_scale_property_->getFloat(),
            angular_head_scale_property_->getFloat(),
            angular_head_radius_property_->getFloat(),
            angular_shaft_radius_property_->getFloat()
        );

        const QColor angular_arrow_color = angular_color_property_->getColor();

        rviz_angular_arrow_->setColor
        (
            angular_arrow_color.redF(),
            angular_arrow_color.greenF(),
            angular_arrow_color.blueF(),
            angular_color_alpha_property_->getFloat()
        );
    }

    void TwistStampedDisplay::updateArrowObject
    (
        rviz_rendering::Arrow &arrow_object,
        const geometry_msgs::msg::Vector3 &vector_msg,
        const Ogre::Vector3 &ogre_position,
        const Ogre::Quaternion &ogre_quaternion,
        const float arrow_scale,
        const float head_scale,
        const float head_radius,
        const float shaft_radius
    )
    {
        const float vector_norm = arrow_scale * std::sqrt
        (
            std::pow(vector_msg.x, 2) +
            std::pow(vector_msg.y, 2) +
            std::pow(vector_msg.z, 2)
        );
        const float arrow_head_length = vector_norm * head_scale;
        const float arrow_shaft_length = vector_norm - arrow_head_length;

        const auto ogre_vector3 = Ogre::Vector3
        (
            vector_msg.x,
            vector_msg.y,
            vector_msg.z
        );

        arrow_object.set
        (
            arrow_shaft_length,
            shaft_radius,
            arrow_head_length,
            head_radius
        );
        arrow_object.setPosition
        (
            ogre_position
        );
        arrow_object.setDirection
        (
            ogre_quaternion * ogre_vector3
        );
    }

    void TwistStampedDisplay::initializeRenderingObjects()
    {
        rviz_linear_arrow_ = std::make_unique<rviz_rendering::Arrow>
        (
            this->scene_manager_,
            this->scene_node_
        );
        rviz_linear_arrow_->set(0, 0, 0, 0);

        rviz_angular_arrow_ = std::make_unique<rviz_rendering::Arrow>
        (
            this->scene_manager_,
            this->scene_node_
        );
        rviz_angular_arrow_->set(0, 0, 0, 0);
    }

    void TwistStampedDisplay::destroyRenderingObjects()
    {
        if(rviz_linear_arrow_)
        {
            rviz_linear_arrow_.reset();
        }
        
        if(rviz_angular_arrow_)
        {
            rviz_angular_arrow_.reset();
        }
    }
}

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(geometry_rviz_plugins::displays::TwistStampedDisplay, rviz_common::Display)
