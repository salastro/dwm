/* Keyboard layout switch on Alt+Shift release (without other keys pressed)
 *
 * This implements the "switch on release only if no other key was pressed" behavior
 * that allows Alt+Shift+<key> combinations to work in applications without
 * triggering a layout switch.
 */

#include <X11/extensions/XInput2.h>

void setupkblayout(void)
{
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
    const Arg a = SHCMD("toggle-keyboard-layout.sh");
    spawn(&a);
}

void handlekblayoutevent(XEvent *e)
{
    XGenericEventCookie *cookie = &e->xcookie;

    if (e->type != GenericEvent || cookie->extension != xi_opcode)
        return;

    if (!XGetEventData(dpy, cookie))
        return;

    XIRawEvent *raw = cookie->data;
    KeySym keysym = XkbKeycodeToKeysym(dpy, raw->detail, 0, 0);

    if (cookie->evtype == XI_RawKeyPress)
    {
        if (keysym == XK_Alt_L)
        {
            kblayout_alt_l_pressed = 1;
        }
        else if (keysym == XK_Shift_L)
        {
            kblayout_shift_l_pressed = 1;
        }
        else
        {
            /* Any other key pressed while Alt+Shift held */
            if (kblayout_alt_l_pressed && kblayout_shift_l_pressed)
            {
                kblayout_other_key_pressed = 1;
            }
        }
    }
    else if (cookie->evtype == XI_RawKeyRelease)
    {
        if (keysym == XK_Alt_L)
        {
            /* Check if we should trigger layout switch */
            if (kblayout_alt_l_pressed && kblayout_shift_l_pressed && !kblayout_other_key_pressed)
            {
                triggerkblayoutswitch();
            }
            kblayout_alt_l_pressed = 0;
            if (!kblayout_shift_l_pressed)
            {
                kblayout_other_key_pressed = 0;
            }
        }
        else if (keysym == XK_Shift_L)
        {
            /* Check if we should trigger layout switch */
            if (kblayout_alt_l_pressed && kblayout_shift_l_pressed && !kblayout_other_key_pressed)
            {
                triggerkblayoutswitch();
            }
            kblayout_shift_l_pressed = 0;
            if (!kblayout_alt_l_pressed)
            {
                kblayout_other_key_pressed = 0;
            }
        }
    }

    XFreeEventData(dpy, cookie);
}
