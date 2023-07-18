
#include "Module.h"

#include <interfaces/ITestPlugin.h>

namespace WPEFramework {
namespace Plugin {
    class TestPluginImplementation : public Exchange::ITestPlugin {

    public:
        TestPluginImplementation() = default;
        ~TestPluginImplementation() = default;

        // Do not allow copy/move constructors
        TestPluginImplementation(const TestPluginImplementation&) = delete;
        TestPluginImplementation& operator=(const TestPluginImplementation&) = delete;

        BEGIN_INTERFACE_MAP(TestPluginImplementation)
        INTERFACE_ENTRY(Exchange::ITestPlugin)
        END_INTERFACE_MAP

    public:
        Core::hresult Test(string& result /* @out */) override
        {
            result = "Hello World";

            // Fire the notification for all connected clients
            _adminLock.Lock();
            for (auto& notification : _notificationCallbacks) {
                notification->SomethingHappend();
            }
            _adminLock.Unlock();

            return Core::ERROR_NONE;
        }

        uint32_t Register(Exchange::ITestPlugin::INotification* notification) override
        {
            _adminLock.Lock();

            // Make sure we can't register the same notification callback multiple times
            if (std::find(_notificationCallbacks.begin(), _notificationCallbacks.end(), notification) == _notificationCallbacks.end()) {
                _notificationCallbacks.emplace_back(notification);
                notification->AddRef();
            }

            _adminLock.Unlock();

            return Core::ERROR_NONE;
        }

        uint32_t Unregister(const Exchange::ITestPlugin::INotification* notification) override
        {
            _adminLock.Lock();

            auto itr = std::find(_notificationCallbacks.begin(), _notificationCallbacks.end(), notification);
            if (itr != _notificationCallbacks.end()) {
                (*itr)->Release();
                _notificationCallbacks.erase(itr);
            }

            _adminLock.Unlock();

            return Core::ERROR_NONE;
        }

    private:
        Core::CriticalSection _adminLock;
        std::list<Exchange::ITestPlugin::INotification*> _notificationCallbacks;
    };

    SERVICE_REGISTRATION(TestPluginImplementation, 1, 0);
}
}