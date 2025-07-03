#include "event_work.hpp"

#include <chrono>
#include <thread>

#include "utility.hpp"

constexpr size_t BUF_SIZE = 512;
static char buf[BUF_SIZE];

void exitWork()
{
    int rc = GHM.end();
    if (rc == RC_SUCCESS)
        printf("Success to exit!\n");
    else
        printf("Failed to exit! Error: %s.\n", RCMSG(rc));
}

void addHotkeyWork()
{
    printf("Please input the string of the hotkey you want to add. (e.g. Ctrl+C)\n");
    int ret = scanf("%s", buf);
    KeyCombination kc(buf);

    if (!kc.isValid())
    {
        printf("The hotkey [%s] given is invalid!\n", KCSTR(kc));
        return;
    }

    if (GHM.has(kc))
    {
        printf("The hotkey [%s] given is exists!\n", KCSTR(kc));
        return;
    }

    printf("Please input the text it be print when the hotkey be triggered.\n");
    ret = scanf("%s", buf);

    std::string str(buf);
    int rc = GHM.add(kc, [=]() { printf("%s\n", str.c_str()); });
    if (rc == RC_SUCCESS)
        printf("Success to add the hotkey [%s]!\n", KCSTR(kc));
    else
        printf("Failed to add the hotkey [%s]! Error: %s.\n", KCSTR(kc), RCMSG(rc));
}

void removeHotkeyWork()
{
    printf("Please input the string of the hotkey you want to remove. (e.g. Ctrl+C)\n");
    int ret = scanf("%s", buf);
    KeyCombination kc(buf);

    if (!GHM.has(kc))
    {
        printf("The hotkey [%s] given not exists!\n", KCSTR(kc));
        return;
    }

    if (isUnchangedKeyCombination(kc))
    {
        printf("The hotkey [%s] is can't be remove!\n", KCSTR(kc));
        return;
    }

    int rc = GHM.remove(kc);
    if (rc == RC_SUCCESS)
        printf("Success to remove the hotkey [%s]!\n", KCSTR(kc));
    else
        printf("Failed to remove the hotkey [%s]! Error: %s.\n", KCSTR(kc), RCMSG(rc));
}

void replaceHotkeyWork()
{
    printf("Please input the string of the old hotkey you want to replace. (e.g. Ctrl+C)\n");
    int ret = scanf("%s", buf);
    KeyCombination oldKc(buf);

    if (!GHM.has(oldKc))
    {
        printf("The old hotkey [%s] given not exists!\n", KCSTR(oldKc));
        return;
    }

    if (isUnchangedKeyCombination(oldKc))
    {
        printf("The hotkey [%s] is can't be replace!\n", KCSTR(oldKc));
        return;
    }

    printf("Please input the string of the new hotkey you want to add. (e.g. Ctrl+Shift+C)\n");
    ret = scanf("%s", buf);
    KeyCombination newKc(buf);

    if (!newKc.isValid())
    {
        printf("The new hotkey [%s] given is invalid!\n", KCSTR(newKc));
        return;
    }

    if (GHM.has(newKc))
    {
        printf("The newKc hotkey [%s] given is exists!\n", KCSTR(newKc));
        return;
    }

    int rc = GHM.replace(oldKc, newKc);
    if (rc == RC_SUCCESS)
        printf("Success to replace the hotkey [%s] to hotkey [%s]!\n", KCSTR(oldKc), KCSTR(newKc));
    else
        printf("Failed to replace the hotkey [%s] to hotkey [%s]! Error: %s.\n",
            KCSTR(oldKc), KCSTR(newKc), RCMSG(rc));
}

void setHotkeyAutoRepeatWork()
{
    printf("Please input the string of the hotkey you want to set is auto repeat. (e.g. Ctrl+C)\n");
    int ret = scanf("%s", buf);
    KeyCombination kc(buf);

    if (GHM.has(kc))
    {
        printf("The hotkey [%s] given is exists!\n", KCSTR(kc));
        return;
    }

    bool autoRepeat = false;
    printf("Please input the \"Y/N (Yes/No)\" to set whether the hotkey is auto repeat.\n");
    ret = scanf("%s", buf);
    while (true)
    {
        if (isEqualStr(buf, "yes") || isEqualStr(buf, "y"))
        {
            autoRepeat = true;
            break;
        }
        else if (isEqualStr(buf, "no") || isEqualStr(buf, "n"))
        {
            autoRepeat = false;
            break;
        }
        else
        {
            printf("Invalid input, please retry.\n");
            ret = scanf("%s", buf);
        }
    }

    int rc = GHM.setAutoRepeat(kc, autoRepeat);
    if (rc == RC_SUCCESS)
        printf("Success to set the hotkey [%s] to %s!\n",
            KCSTR(kc), autoRepeat ? "auto repeat" : "no auto repeat");
    else
        printf("Success to set the hotkey [%s] to %s! Error: %s.\n",
            KCSTR(kc), autoRepeat ? "auto repeat" : "no auto repeat", RCMSG(rc));
}

void simpleWork()
{
    printf("This is a simple work. Just print a message.\n");
}

// In seconds.
static int heavyWorkLevel = 10;

void heavyWork()
{
    printf(
"This is a heavy work. It will take a long time [current level: %d] to finish.\n\
(print a message every 1 second, the level indicates how long it will take.)\n", heavyWorkLevel);
    int level = heavyWorkLevel;
    std::thread th = std::thread([=]() {
        int i = 0;
        while (i < level)
        {
            printf("Heavy work: %d\n", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            i++;
        }
        printf("Heavy work: %d\n", i);
        printf("Heavy work finished!\n");
    });
    th.detach();
}

void setHeavyWorkLevelWork()
{
    printf("Please input the level of the heavy work.\n");
    int level = 0;
    int ret = scanf("%d", &level);
    if (level < 1)
    {
        printf("Invalid input, please retry.\n");
        ret = scanf("%d", &level);
    }
    heavyWorkLevel = level;
}
