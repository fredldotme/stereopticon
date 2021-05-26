/*
 * Copyright © 2021 Alfred Neumayer
 * Copyright © 2016-2020 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * under the terms of the GNU General Public License version 2 or 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by:
 * - Alfred Neumayer <dev.beidl@gmail.com>
 * - Alan Griffiths <alan@octopull.co.uk>
 */

#include <miral/runner.h>
#include <miral/application_authorizer.h>
#include <miral/display_configuration.h>
#include <miral/external_client.h>
#include <miral/command_line_option.h>
#include <miral/keymap.h>
#include <miral/set_window_management_policy.h>
#include <miral/internal_client.h>
#include <miral/wayland_extensions.h>

#include <unistd.h>
#include <atomic>

#include "window_manager.h"

namespace {
struct StereopticonAuthorizer : miral::ApplicationAuthorizer
{
    StereopticonAuthorizer(){}

    virtual bool connection_is_allowed(miral::ApplicationCredentials const& creds) override
    {
        return true;
    }

    virtual bool configure_display_is_allowed(miral::ApplicationCredentials const& /*creds*/) override
    {
        return false;
    }

    virtual bool set_base_display_configuration_is_allowed(miral::ApplicationCredentials const& /*creds*/) override
    {
        return false;
    }

    virtual bool screencast_is_allowed(miral::ApplicationCredentials const& /*creds*/) override
    {
        return true;
    }

    virtual bool prompt_session_is_allowed(miral::ApplicationCredentials const& /*creds*/) override
    {
        return false;
    }

    bool configure_input_is_allowed(miral::ApplicationCredentials const& /*creds*/) override
    {
        return false;
    }

    bool set_base_input_configuration_is_allowed(miral::ApplicationCredentials const& /*creds*/) override
    {
        return false;
    }
};
}

int main(int argc, char const* argv[])
{
    using namespace miral;

    MirRunner runner{argc, argv};

    DisplayConfiguration display_config{runner};

    WaylandExtensions wayland_extensions;

    for (auto const& extension : {"zwp_pointer_constraints_v1", "zwp_relative_pointer_manager_v1"})
        wayland_extensions.enable(extension);

    return runner.run_with(
        {
            display_config,
            display_config.layout_option(),
            set_window_management_policy<KioskWindowManagerPolicy>(),
            SetApplicationAuthorizer<StereopticonAuthorizer>{},
            Keymap{},
            wayland_extensions
        });
}
