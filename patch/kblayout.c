/* Keyboard layout switch on modifier release (without other keys pressed)
 *
 * This implements the "switch on release only if no other key was pressed" behavior
 * that allows modifier+<key> combinations to work in applications without
 * triggering a layout switch.
 */

#include <X11/extensions/XInput2.h>

void setupkblayout(void)
{
    if (!kblayout_enabled)
        return;

    int event, error;
    XIEventMask evmask;
    unsigned char mask[(XI_LASTEVENT + 7) / 8] = {0};

    /* Check for XInput2 extension */
    if (!XQueryExtension(dpy, "XInputExtension", &xi_opcode, &event, &error))
    {
        fprintf(stderr, "dwm: XInput extension not available, keyboard layout switching disabled\n");
        return;
    }

    /* Check XInput2 version */
    int major = 2, minor = 0;
    if (XIQueryVersion(dpy, &major, &minor) == BadRequest)
    {
        fprintf(stderr, "dwm: XInput2 not available, keyboard layout switching disabled\n");
        return;
    }

    /* Select for raw key events on root window */
    XISetMask(mask, XI_RawKeyPress);
    XISetMask(mask, XI_RawKeyRelease);

    evmask.deviceid = XIAllMasterDevices;
    evmask.mask_len = sizeof(mask);
    evmask.mask = mask;

    XISelectEvents(dpy, root, &evmask, 1);
}

void triggerkblayoutswitch(void)
{
    const Arg a = {.v = kblayout_cmd};
    spawn(&a);
}

void handlekblayoutevent(XEvent *e)
{
    if (!kblayout_enabled)
        return;

    XGenericEventCookie *cookie = &e->xcookie;

    if (e->type != GenericEvent || cookie->extension != xi_opcode)
        return;

    if (!XGetEventData(dpy, cookie))
        return;

    XIRawEvent *raw = cookie->data;
    KeySym keysym = XkbKeycodeToKeysym(dpy, raw->detail, 0, 0);

    if (cookie->evtype == XI_RawKeyPress)
    {
        if (keysym == kblayout_key1)
        {
            kblayout_key1_pressed = 1;
        }
        else if (keysym == kblayout_key2)
        {
            kblayout_key2_pressed = 1;
        }
        else
        {
            /* Any other key pressed while both modifiers held */
            if (kblayout_key1_pressed && kblayout_key2_pressed)
            {
                kblayout_other_key_pressed = 1;
            }
        }
    }
    else if (cookie->evtype == XI_RawKeyRelease)
    {
        if (keysym == kblayout_key1)
        {
            /* Check if we should trigger layout switch */
            if (kblayout_key1_pressed && kblayout_key2_pressed && !kblayout_other_key_pressed)
            {
                triggerkblayoutswitch();
            }
            kblayout_key1_pressed = 0;
            if (!kblayout_key2_pressed)
            {
                kblayout_other_key_pressed = 0;
            }
        }
        else if (keysym == kblayout_key2)
        {
            /* Check if we should trigger layout switch */
            if (kblayout_key1_pressed && kblayout_key2_pressed && !kblayout_other_key_pressed)
            {
                triggerkblayoutswitch();
            }
            kblayout_key2_pressed = 0;
            if (!kblayout_key1_pressed)
            {
                kblayout_other_key_pressed = 0;
            }
        }
    }

    XFreeEventData(dpy, cookie);
}
