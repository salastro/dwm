/* Keyboard layout switch on Alt+Shift release (without other keys pressed) */

static int xi_opcode;
static int kblayout_alt_l_pressed;
static int kblayout_shift_l_pressed;
static int kblayout_other_key_pressed;

static void setupkblayout(void);
static void handlekblayoutevent(XEvent *e);
static void triggerkblayoutswitch(void);
