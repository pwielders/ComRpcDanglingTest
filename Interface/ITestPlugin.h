#pragma once

#include "Module.h"

namespace WPEFramework {
namespace Exchange {

    /* @json 1.0.0 */
    struct EXTERNAL ITestPlugin : virtual public Core::IUnknown {
        enum {
            ID = ID_TEST_PLUGIN
        };

        /* @event */
        struct EXTERNAL INotification : virtual public Core::IUnknown {
            enum {
                ID = ID_TEST_PLUGIN_NOTIFICATION
            };

            ~INotification() override = default;

            // @brief A generic example notification clients can subscribe to
            virtual void SomethingHappend() = 0;
        };

        ~ITestPlugin() override = default;

        /* @brief Register for notifications from the sample plugin */
        virtual uint32_t Register(ITestPlugin::INotification* notification) = 0;

        /* @brief Unregister for notifications from the sample plugin */
        virtual uint32_t Unregister(const ITestPlugin::INotification* notification) = 0;

        /* @brief Return a test value */
        virtual Core::hresult Test(string& result /* @out */) = 0;
    };
}
}