#pragma once

// This file holds all the identifiers (uint32_t) used to identify an interface. From this
// identifier, the comrpc framework can find the proper proxy/stub in case of communicating
// over a process boundary.
// Some users do not "fully" rebuild the system in case of changes. If this means that the
// Proxy/Stub code is not always rebuild in case of new releases, the identifier associated
// with an interface becomes as important as the interface syntax and as interfaces are not
// allowed to be changed, the ID associated with the interface should also not be changed
// and thus should be "fixed".

// So if you extend this file by defining a new interface ID make sure it is defined (has
// an actual value) and once the enum label has a value, never change it again.

// As some interfaces might be grouped, the first ID of the group is assigned a value, the
// other interfaces belonging to this group use the enum value of label that has an assigned
// value and just increment that label by the proper amount.

// Using this system, all interfaces will have an assigned number. If numbers overlap, the
// compiler, your best friend, will start complaining. Time to reassign the value, before we
// deploy.

// NOTE: Default the gap between each group of interface is 16. If you need more and the new
//       addition is add the end, write a comment with your interface that you might need more
//       than 16 interface in that group so that the next ID is indeed elevated (and rounded
//       up to a multiple of 16) if the next entry is made in the future.

#include "Module.h"

namespace WPEFramework {

namespace Exchange {

    enum IDS : uint32_t {
        ID_TEST_PLUGIN                               = Core::IUnknown::ID_OFFSET_CUSTOM,
        ID_TEST_PLUGIN_NOTIFICATION                  = ID_TEST_PLUGIN + 1
    };
}
}
