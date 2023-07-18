#include "TestPlugin.h"

#include <interfaces/json/JTestPlugin.h>

namespace WPEFramework {
namespace Plugin {

    static Metadata<TestPlugin> metadata(
        // Version
        1, 0, 0,
        // Preconditions
        {},
        // Terminations
        {},
        // Controls
        {});

    TestPlugin::TestPlugin()
        : _connectionId(0)
        , _service(nullptr)
        , _testPlugin(nullptr)
        , _notification(this)
    {
    }

    TestPlugin::~TestPlugin()
    {
    }

    const string TestPlugin::Initialize(PluginHost::IShell* service)
    {
        ASSERT(_service == nullptr);
        ASSERT(_connectionId == 0);

        string result = {};

        _service = service;
        _service->AddRef();

        // Register for COM-RPC connection/disconnection notifications
        _service->Register(&_notification);

        // Instantiate the ITestPlugin interface (which could spawn the OOP side if running in OOP mode)
        // Store connection ID in _connectionId
        _testPlugin = _service->Root<Exchange::ITestPlugin>(_connectionId, 2000, _T("TestPluginImplementation"));

        if (!_testPlugin) {
            // Error occurred, return non-empty string
            result = "Failed to create ITestPlugin";
        } else {
            // Register JSON-RPC
            Exchange::JTestPlugin::Register(*this, _testPlugin);
        }

        return result;
    }

    void TestPlugin::Deinitialize(PluginHost::IShell* service)
    {
        if (service != nullptr) {
            ASSERT(_service == service);

            // Unsubscribe from connection notification first to prevent any false-positives
            _service->Unregister(&_notification);

            if (_testPlugin != nullptr) {
                Exchange::JTestPlugin::Unregister(*this);

                _testPlugin->Release();
                _testPlugin == nullptr;
            }

            _service->Release();
            _service = nullptr;
            _connectionId = 0;
        }
    }

    string TestPlugin::Information() const
    {
        // No additional info to report
        return string();
    }

    void TestPlugin::CallbackRevoked(const Exchange::ITestPlugin::INotification* remote)
    {
        // Unregister the notification
        TRACE(Trace::Information, (_T("Revoking callback due to dead client")));
        _testPlugin->Unregister(remote);
    }
}
}