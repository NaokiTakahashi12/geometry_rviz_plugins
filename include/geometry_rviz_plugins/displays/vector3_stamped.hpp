
#pragma once

#include <rviz_common/message_filter_display.hpp>

#include <memory>

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

        public :
            Vector3StampedDisplay();
            explicit Vector3StampedDisplay(rviz_common::DisplayContext *);
            ~Vector3StampedDisplay() override;

            void reset() override;
            void processMessage(geometry_msgs::msg::Vector3Stamped::ConstSharedPtr) override;

        protected :
            void onInitialize() override;

        private Q_SLOTS:
            void arrowPropertyCallback();

        private :
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
}

