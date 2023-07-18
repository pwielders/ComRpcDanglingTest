
#pragma once

#include "Module.h"

#include <interfaces/ITestPlugin.h>

namespace WPEFramework {
namespace Plugin {
    class TestPluginImplementation;

    class TestPlugin : public PluginHost::IPlugin, public PluginHost::JSONRPC {
    private:
        class Notification : public PluginHost::IShell::ICOMLink::INotification {
        public:
            Notification() = delete;
            explicit Notification(TestPlugin* parent)
                : _parent(*parent)
            {
                ASSERT(parent != nullptr);
            }

            ~Notification() = default;
            Notification(const Notification&) = delete;

        public:
            void Dangling(const Core::IUnknown* remote, const uint32_t interfaceId) override
            {
                TRACE(Trace::Information, (_T("Dangling notification")));

                ASSERT(remote != nullptr);

                if (interfaceId == Exchange::ID_TEST_PLUGIN_NOTIFICATION) {
                    const auto revokedInterface = remote->QueryInterface<Exchange::ITestPlugin::INotification>();
                    if (revokedInterface) {
                        _parent.CallbackRevoked(revokedInterface);
                        revokedInterface->Release();
                    }
                }
            }

            void Revoked(const Core::IUnknown* remote, const uint32_t interfaceId) override
            {
                TRACE(Trace::Information, (_T("Revoked notification")));
            }

            BEGIN_INTERFACE_MAP(Notification)
            INTERFACE_ENTRY(PluginHost::IShell::ICOMLink::INotification)
            END_INTERFACE_MAP

        private:
            TestPlugin& _parent;
        };

    public:
        TestPlugin();
        ~TestPlugin() override;

        // Do not allow copy/move constructors
        TestPlugin(const TestPlugin&) = delete;
        TestPlugin& operator=(const TestPlugin&) = delete;

        BEGIN_INTERFACE_MAP(TestPlugin)
        INTERFACE_ENTRY(PluginHost::IPlugin)
        INTERFACE_ENTRY(PluginHost::IDispatcher)
        INTERFACE_AGGREGATE(Exchange::ITestPlugin, _testPlugin)
        END_INTERFACE_MAP

    public:
        // IPlugin methods
        // ======================
        const string Initialize(PluginHost::IShell* service) override;
        void Deinitialize(PluginHost::IShell* service) override;
        string Information() const override;

    private:
        void CallbackRevoked(const Exchange::ITestPlugin::INotification* remote);

    private:
        uint32_t _connectionId;
        PluginHost::IShell* _service;
        Exchange::ITestPlugin* _testPlugin;
        Core::Sink<Notification> _notification;
    };
}
}