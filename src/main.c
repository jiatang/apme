#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "console.h"
#include "aion.h"
#include "cmd.h"
#include "chatlog.h"
#include "util.h"
#include "help.h"
#include "event.h"

bool aptrack_prompt(char *prompt, char *answer)
{
    char line[64];

    fprintf(stdout, "\n%s > ", prompt); fflush(stdout);

    if (fgets(line, sizeof(line), stdin) == NULL)
    {
        line[0] = '\0';
    }

    fprintf(stdout, "\n"); fflush(stdout);

    util_chomp(line);

    if (strcasecmp(answer, line) == 0) 
    {
        return true;
    }

    return false;
}

void aptrack_chatlog_check(void)
{
    bool chatlog_enabled;
    bool enable_ok;

    /* Check if the chatlog feature is enabled in AION */
    if (!aion_chatlog_is_enabled(&chatlog_enabled))
    {
        aptrack_prompt("Unable to determine if the CHATLOG is enabled. Not all features might be available. Press RETURN to contunue.", "");
        return;
    }

    if (chatlog_enabled)
    {
        con_printf("CHATLOG is enabled.\n");
        return;
    }

    /* Do the warn dialog and enable chatlog stuff */
    printf("%s\n", help_chatlog_warning);
    enable_ok = aptrack_prompt("Type ACCEPT to enable the chatlog, or ENTER to continue",
                               "accept");
    if (!enable_ok)
    {
        aptrack_prompt("The CHATLOG was not enabled by user request, press ENTER to continue", "");
        return;
    }

    if (!aion_chatlog_enable())
    {
        printf("%s\n", help_chatlog_enable_error);
        aptrack_prompt("Press ENTER to continue", "");
        return;
    }

    printf("%s\n", help_chatlog_enabled);
    aptrack_prompt("Press ENTER to continue", "");
}

void aptrack_event_handler(enum event_type ev)
{
    switch (ev)
    {
        case EVENT_AION_GROUP_UPDATE:
            break;

        case EVENT_AION_AP_UPDATE:
            break;

        case EVENT_AION_INVENTORY_FULL:
            break;

        case EVENT_AION_LOOT_RIGHTS:
            break;
    }
}

bool aptrack_init(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    con_init();

    if (!aion_init())
    {
        con_printf("Unable to initialize the Aion subsystem.\n");
        return false; 
    }

    if (!chatlog_init())
    {
        con_printf("Error initializing the Chatlog parser.\n");
        return false;
    }

    event_register(aptrack_event_handler);

    return true;
}

void aptrack_periodic()
{
    cmd_poll();
    chatlog_poll();
}

int main(int argc, char *argv[])
{
    /* Do the chatlog enable/disable stuff, warn user... */
    aptrack_chatlog_check();

    /* Initialize sub-systems */
    if (!aptrack_init(argc, argv))
    {
        return 1;
    }

    /* Main processing loop */
    for (;;)
    {
        aptrack_periodic();
        usleep(300);
    }

#ifdef SYS_WINDOWS
    /* On windows, pause before exiting */
    {
        char buf[16];
        printf("Press ENTER to exit.\n");
        fgets(buf, sizeof(buf), stdin);
    }
#endif

    /* And we disappear into the void */
    return 0;
}

