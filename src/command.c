#include "command.h"

void
export_known_commands(add_command_func add_cmd)
{
    (*add_cmd)("echo", &echo_command);
}