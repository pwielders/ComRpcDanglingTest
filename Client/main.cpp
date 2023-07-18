#ifndef MODULE_NAME
#define MODULE_NAME ComrpcExample
#endif

#include <com/com.h>
#include <core/core.h>
#include <plugins/plugins.h>

#include <interfaces/ITestPlugin.h>

using namespace WPEFramework;

class NotificationHandler : public Exchange::ITestPlugin::INotification {
    void SomethingHappend() override
    {
        printf("Something happened\n");
    }

    BEGIN_INTERFACE_MAP(NotificationHandler)
    INTERFACE_ENTRY(Exchange::ITestPlugin::INotification)
    END_INTERFACE_MAP
};

int main(int argc, char const* argv[])
{
    {
        RPC::SmartInterfaceType<Exchange::ITestPlugin> link;
        Core::Sink<NotificationHandler> notificationHandler;

        auto success = link.Open(RPC::CommunicationTimeOut, link.Connector(), "TestPlugin");
        if (success == Core::ERROR_NONE && link.IsOperational()) {
            auto testPlugin = link.Interface();
            testPlugin->Register(&notificationHandler);

            // Invoke method
            string result;
            success = testPlugin->Test(result);
            printf("Got result: %s (%s)\n", result.c_str(), Core::ErrorToString(success));

            raise(SIGSEGV);

            testPlugin->Unregister(&notificationHandler);
            testPlugin->Release();
        }

        link.Close(Core::infinite);
    }

    Core::Singleton::Dispose();

    return 0;
}