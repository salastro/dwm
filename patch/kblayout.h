/* Keyboard layout switch on modifier release (without other keys pressed)
 *
 * Configuration (define in config.h before including this):
 *
 *   kblayout_enabled   - set to 1 to enable, 0 to disable
 *   kblayout_key1      - first modifier keysym (e.g., XK_Alt_L)
 *   kblayout_key2      - second modifier keysym (e.g., XK_Shift_L)
 *   kblayout_cmd       - command to run on trigger (use SHCMD() macro)
 *
 * Example config.h:
 *   static const int kblayout_enabled = 1;
 *   static const KeySym kblayout_key1 = XK_Alt_L;
 *   static const KeySym kblayout_key2 = XK_Shift_L;
 *   static const char *kblayout_cmd[] = { "toggle-keyboard-layout.sh", NULL };
 */

static int xi_opcode;
static int kblayout_key1_pressed;
static int kblayout_key2_pressed;
static int kblayout_other_key_pressed;

static void setupkblayout(void);
static void handlekblayoutevent(XEvent *e);
static void triggerkblayoutswitch(void);
