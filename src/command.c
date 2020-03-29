#include "command.h"

void
export_known_commands(add_command_func add_cmd)
{
    (*add_cmd)("echo", &echo_command);
    (*add_cmd)("add", &add_software_command);
    (*add_cmd)("list", &list_software_command);
    (*add_cmd)("install", &install_software_command);
    (*add_cmd)("usage", &show_usage_command);
}
