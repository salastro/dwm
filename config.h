/* See LICENSE file for copyright and license details. */
/* appearance */
#include <X11/X.h>
#include <X11/Xutil.h>
#include <string.h>

/* Keyboard layout switching on modifier release
 * Triggers only when both keys are pressed and released without any other key */
static const int kblayout_enabled = 1;			/* 0 to disable */
static const KeySym kblayout_key1 = XK_Alt_L;	/* first modifier */
static const KeySym kblayout_key2 = XK_Shift_L; /* second modifier */
static const char *kblayout_cmd[] = {"toggle-keyboard-layout.sh", NULL};

static const unsigned int borderpx = 3; /* border pixel of windows */
/* This allows the bar border size to be explicitly set separately from borderpx.
 * If left as 0 then it will default to the borderpx value of the monitor and will
 * automatically update with setborderpx. */
static const unsigned int barborderpx = 0; /* border pixel of bar */
static const unsigned int snap = 32;	   /* snap pixel */
static const int swallowfloating = 1;	   /* 1 means swallow floating windows by default */
static const unsigned int gappih = 0;	   /* horiz inner gap between windows */
static const unsigned int gappiv = 0;	   /* vert inner gap between windows */
static const unsigned int gappoh = 0;	   /* horiz outer gap between windows and screen edge */
static const unsigned int gappov = 0;	   /* vert outer gap between windows and screen edge */
static const int smartgaps_fact = 0;	   /* gap factor when there is only one client; 0 = no gaps, 3 = 3x outer gaps */
static const int showbar = 1;			   /* 0 means no bar */
static const int topbar = 1;			   /* 0 means bottom bar */
#define ICONSIZE 17.5					   /* icon size */
#define ICONSPACING 5					   /* space between icon and title */
/* Status is to be shown on: -1 (all monitors), 0 (a specific monitor by index), 'A' (active monitor) */
static const int statusmon = 'A';
static const unsigned int systrayspacing = 10; /* systray spacing */
static const int showsystray = 1;			   /* 0 means no systray */

#define FLEXWINTITLE_BORDERS 0	   // 0 = off, 1 = on
#define FLEXWINTITLE_FLOATWEIGHT 0 // floating window title weight, set to 0 to not show floating windows

/* Indicators: see patch/bar_indicators.h for options */
// static int tagindicatortype              = INDICATOR_NONE;
static int tiledindicatortype = INDICATOR_NONE;
static int floatindicatortype = INDICATOR_NONE;
static const int quit_empty_window_count = 1; /* only allow dwm to quit if no (<= count) windows are open */
static const char statussep = ';';			  /* separator between status bars */
static void (*bartabmonfns[])(Monitor *) = {monocle /* , customlayoutfn */};
static const char *fonts[] = {"monospace:size=12", "fontawesome:size=13", "JoyPixels:pixelsize=14"};
static const char dmenufont[] = "monospace:size=12";

static char c000000[] = "#000000"; // placeholder value

static char normfgcolor[] = "#bbbbbb";
static char normbgcolor[] = "#222222";
static char normbordercolor[] = "#444444";
static char normfloatcolor[] = "#db8fd9";

static char selfgcolor[] = "#eeeeee";
static char selbgcolor[] = "#005577";
static char selbordercolor[] = "#005577";
static char selfloatcolor[] = "#005577";

static char titlenormfgcolor[] = "#bbbbbb";
static char titlenormbgcolor[] = "#222222";
static char titlenormbordercolor[] = "#444444";
static char titlenormfloatcolor[] = "#db8fd9";

static char titleselfgcolor[] = "#eeeeee";
static char titleselbgcolor[] = "#005577";
static char titleselbordercolor[] = "#005577";
static char titleselfloatcolor[] = "#005577";

static char tagsnormfgcolor[] = "#bbbbbb";
static char tagsnormbgcolor[] = "#222222";
static char tagsnormbordercolor[] = "#444444";
static char tagsnormfloatcolor[] = "#db8fd9";

static char tagsselfgcolor[] = "#eeeeee";
static char tagsselbgcolor[] = "#005577";
static char tagsselbordercolor[] = "#005577";
static char tagsselfloatcolor[] = "#005577";

static char hidnormfgcolor[] = "#005577";
static char hidselfgcolor[] = "#227799";
static char hidnormbgcolor[] = "#222222";
static char hidselbgcolor[] = "#222222";

static char urgfgcolor[] = "#bbbbbb";
static char urgbgcolor[] = "#222222";
static char urgbordercolor[] = "#ff0000";
static char urgfloatcolor[] = "#db8fd9";

static char normTTBbgcolor[] = "#330000";
static char normLTRbgcolor[] = "#330033";
static char normMONObgcolor[] = "#000033";
static char normGRIDbgcolor[] = "#003300";
static char normGRD1bgcolor[] = "#003300";
static char normGRD2bgcolor[] = "#003300";
static char normGRDMbgcolor[] = "#506600";
static char normHGRDbgcolor[] = "#b96600";
static char normDWDLbgcolor[] = "#003333";
static char normSPRLbgcolor[] = "#333300";
static char normfloatbgcolor[] = "#115577";
static char actTTBbgcolor[] = "#440000";
static char actLTRbgcolor[] = "#440044";
static char actMONObgcolor[] = "#000044";
static char actGRIDbgcolor[] = "#004400";
static char actGRD1bgcolor[] = "#004400";
static char actGRD2bgcolor[] = "#004400";
static char actGRDMbgcolor[] = "#507711";
static char actHGRDbgcolor[] = "#b97711";
static char actDWDLbgcolor[] = "#004444";
static char actSPRLbgcolor[] = "#444400";
static char actfloatbgcolor[] = "#116688";
static char selTTBbgcolor[] = "#550000";
static char selLTRbgcolor[] = "#550055";
static char selMONObgcolor[] = "#212171";
static char selGRIDbgcolor[] = "#005500";
static char selGRD1bgcolor[] = "#005500";
static char selGRD2bgcolor[] = "#005500";
static char selGRDMbgcolor[] = "#508822";
static char selHGRDbgcolor[] = "#b98822";
static char selDWDLbgcolor[] = "#005555";
static char selSPRLbgcolor[] = "#555500";
static char selfloatbgcolor[] = "#117799";

static const unsigned int baralpha = 0xD9;
static const unsigned int borderalpha = baralpha;
static const unsigned int alphas[][3] = {
	/*                       fg      bg        border     */
	[SchemeNorm] = {OPAQUE, baralpha, borderalpha},
	[SchemeSel] = {OPAQUE, baralpha, borderalpha},
	[SchemeTitleNorm] = {OPAQUE, baralpha, borderalpha},
	[SchemeTitleSel] = {OPAQUE, baralpha, borderalpha},
	[SchemeTagsNorm] = {OPAQUE, baralpha, borderalpha},
	[SchemeTagsSel] = {OPAQUE, baralpha, borderalpha},
	[SchemeHidNorm] = {OPAQUE, baralpha, borderalpha},
	[SchemeHidSel] = {OPAQUE, baralpha, borderalpha},
	[SchemeUrg] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActTTB] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActLTR] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActMONO] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActGRID] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActGRD1] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActGRD2] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActGRDM] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActHGRD] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActDWDL] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActSPRL] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexActFloat] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaTTB] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaLTR] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaMONO] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaGRID] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaGRD1] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaGRD2] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaGRDM] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaHGRD] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaDWDL] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaSPRL] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexInaFloat] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelTTB] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelLTR] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelMONO] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelGRID] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelGRD1] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelGRD2] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelGRDM] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelHGRD] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelDWDL] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelSPRL] = {OPAQUE, baralpha, borderalpha},
	[SchemeFlexSelFloat] = {OPAQUE, baralpha, borderalpha},
};

static char *colors[][ColCount] = {
	/*                       fg                bg                border                float */
	[SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor, normfloatcolor},
	[SchemeSel] = {selfgcolor, selbgcolor, selbordercolor, selfloatcolor},
	[SchemeTitleNorm] = {titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor},
	[SchemeTitleSel] = {titleselfgcolor, titleselbgcolor, titleselbordercolor, titleselfloatcolor},
	[SchemeTagsNorm] = {tagsnormfgcolor, tagsnormbgcolor, tagsnormbordercolor, tagsnormfloatcolor},
	[SchemeTagsSel] = {tagsselfgcolor, tagsselbgcolor, tagsselbordercolor, tagsselfloatcolor},
	[SchemeHidNorm] = {hidnormfgcolor, hidnormbgcolor, c000000, c000000},
	[SchemeHidSel] = {hidselfgcolor, hidselbgcolor, c000000, c000000},
	[SchemeUrg] = {urgfgcolor, urgbgcolor, urgbordercolor, urgfloatcolor},
	[SchemeFlexActTTB] = {titleselfgcolor, actTTBbgcolor, actTTBbgcolor, c000000},
	[SchemeFlexActLTR] = {titleselfgcolor, actLTRbgcolor, actLTRbgcolor, c000000},
	[SchemeFlexActMONO] = {titleselfgcolor, actMONObgcolor, actMONObgcolor, c000000},
	[SchemeFlexActGRID] = {titleselfgcolor, actGRIDbgcolor, actGRIDbgcolor, c000000},
	[SchemeFlexActGRD1] = {titleselfgcolor, actGRD1bgcolor, actGRD1bgcolor, c000000},
	[SchemeFlexActGRD2] = {titleselfgcolor, actGRD2bgcolor, actGRD2bgcolor, c000000},
	[SchemeFlexActGRDM] = {titleselfgcolor, actGRDMbgcolor, actGRDMbgcolor, c000000},
	[SchemeFlexActHGRD] = {titleselfgcolor, actHGRDbgcolor, actHGRDbgcolor, c000000},
	[SchemeFlexActDWDL] = {titleselfgcolor, actDWDLbgcolor, actDWDLbgcolor, c000000},
	[SchemeFlexActSPRL] = {titleselfgcolor, actSPRLbgcolor, actSPRLbgcolor, c000000},
	[SchemeFlexActFloat] = {titleselfgcolor, actfloatbgcolor, actfloatbgcolor, c000000},
	[SchemeFlexInaTTB] = {titlenormfgcolor, normTTBbgcolor, normTTBbgcolor, c000000},
	[SchemeFlexInaLTR] = {titlenormfgcolor, normLTRbgcolor, normLTRbgcolor, c000000},
	[SchemeFlexInaMONO] = {titlenormfgcolor, normMONObgcolor, normMONObgcolor, c000000},
	[SchemeFlexInaGRID] = {titlenormfgcolor, normGRIDbgcolor, normGRIDbgcolor, c000000},
	[SchemeFlexInaGRD1] = {titlenormfgcolor, normGRD1bgcolor, normGRD1bgcolor, c000000},
	[SchemeFlexInaGRD2] = {titlenormfgcolor, normGRD2bgcolor, normGRD2bgcolor, c000000},
	[SchemeFlexInaGRDM] = {titlenormfgcolor, normGRDMbgcolor, normGRDMbgcolor, c000000},
	[SchemeFlexInaHGRD] = {titlenormfgcolor, normHGRDbgcolor, normHGRDbgcolor, c000000},
	[SchemeFlexInaDWDL] = {titlenormfgcolor, normDWDLbgcolor, normDWDLbgcolor, c000000},
	[SchemeFlexInaSPRL] = {titlenormfgcolor, normSPRLbgcolor, normSPRLbgcolor, c000000},
	[SchemeFlexInaFloat] = {titlenormfgcolor, normfloatbgcolor, normfloatbgcolor, c000000},
	[SchemeFlexSelTTB] = {titleselfgcolor, selTTBbgcolor, selTTBbgcolor, c000000},
	[SchemeFlexSelLTR] = {titleselfgcolor, selLTRbgcolor, selLTRbgcolor, c000000},
	[SchemeFlexSelMONO] = {titleselfgcolor, selMONObgcolor, selMONObgcolor, c000000},
	[SchemeFlexSelGRID] = {titleselfgcolor, selGRIDbgcolor, selGRIDbgcolor, c000000},
	[SchemeFlexSelGRD1] = {titleselfgcolor, selGRD1bgcolor, selGRD1bgcolor, c000000},
	[SchemeFlexSelGRD2] = {titleselfgcolor, selGRD2bgcolor, selGRD2bgcolor, c000000},
	[SchemeFlexSelGRDM] = {titleselfgcolor, selGRDMbgcolor, selGRDMbgcolor, c000000},
	[SchemeFlexSelHGRD] = {titleselfgcolor, selHGRDbgcolor, selHGRDbgcolor, c000000},
	[SchemeFlexSelDWDL] = {titleselfgcolor, selDWDLbgcolor, selDWDLbgcolor, c000000},
	[SchemeFlexSelSPRL] = {titleselfgcolor, selSPRLbgcolor, selSPRLbgcolor, c000000},
	[SchemeFlexSelFloat] = {titleselfgcolor, selfloatbgcolor, selfloatbgcolor, c000000},
};

static char *statuscolors[][ColCount] = {
	/*                       fg                bg                border                float */
	[SchemeNorm] = {normfgcolor, normbgcolor, normbordercolor, normfloatcolor},
	[SchemeSel] = {selfgcolor, selbgcolor, selbordercolor, selfloatcolor},
	[SchemeTitleNorm] = {titlenormfgcolor, titlenormbgcolor, titlenormbordercolor, titlenormfloatcolor},
	[SchemeTitleSel] = {titleselfgcolor, titleselbgcolor, titleselbordercolor, titleselfloatcolor},
	[SchemeTagsNorm] = {tagsnormfgcolor, tagsnormbgcolor, tagsnormbordercolor, tagsnormfloatcolor},
	[SchemeTagsSel] = {tagsselfgcolor, tagsselbgcolor, tagsselbordercolor, tagsselfloatcolor},
	[SchemeHidNorm] = {hidnormfgcolor, hidnormbgcolor, c000000, c000000},
	[SchemeHidSel] = {hidselfgcolor, hidselbgcolor, c000000, c000000},
	[SchemeUrg] = {urgfgcolor, urgbgcolor, urgbordercolor, urgfloatcolor},
};

static const char *const autostart[] = {
	"cbatticon", NULL,
	"blueman-applet", NULL,
	"birdtray", NULL,
	"clipmenud", NULL,
	"darkman", "run", NULL,
	"dwmblocks", NULL,
	"numlockx", NULL,
	"picom", NULL,
	"sxhkd", NULL,
	"xss-lock", "slock", NULL,
	"xhost", "+", "local:", NULL,
	NULL /* terminate */
};

const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34", NULL};
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm", spcmd1},
};

/* Tags
 * In a traditional dwm the number of tags in use can be changed simply by changing the number
 * of strings in the tags array. This build does things a bit different which has some added
 * benefits. If you need to change the number of tags here then change the NUMTAGS macro in dwm.c.
 *
 * Examples:
 *
 *  1) static char *tagicons[][NUMTAGS*2] = {
 *         [DEFAULT_TAGS] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F", "G", "H", "I" },
 *     }
 *
 *  2) static char *tagicons[][1] = {
 *         [DEFAULT_TAGS] = { "•" },
 *     }
 *
 * The first example would result in the tags on the first monitor to be 1 through 9, while the
 * tags for the second monitor would be named A through I. A third monitor would start again at
 * 1 through 9 while the tags on a fourth monitor would also be named A through I. Note the tags
 * count of NUMTAGS*2 in the array initialiser which defines how many tag text / icon exists in
 * the array. This can be changed to *3 to add separate icons for a third monitor.
 *
 * For the second example each tag would be represented as a bullet point. Both cases work the
 * same from a technical standpoint - the icon index is derived from the tag index and the monitor
 * index. If the icon index is is greater than the number of tag icons then it will wrap around
 * until it an icon matches. Similarly if there are two tag icons then it would alternate between
 * them. This works seamlessly with alternative tags and alttagsdecoration patches.
 */
static char *tagicons[][NUMTAGS] =
	{
		[DEFAULT_TAGS] = {"🖥️", "🌐", "📝", "💬", "5", "6", "7", "8", "9"},
		[ALTERNATIVE_TAGS] = {"A", "B", "C", "D", "E", "F", "G", "H", "I"},
		[ALT_TAGS_DECORATION] = {"<1>", "<2>", "<3>", "<4>", "<5>", "<6>", "<7>", "<8>", "<9>"},
};

/* grid of tags */
#define SWITCHTAG_UP 1 << 0
#define SWITCHTAG_DOWN 1 << 1
#define SWITCHTAG_LEFT 1 << 2
#define SWITCHTAG_RIGHT 1 << 3
#define SWITCHTAG_TOGGLETAG 1 << 4
#define SWITCHTAG_TAG 1 << 5
#define SWITCHTAG_VIEW 1 << 6
#define SWITCHTAG_TOGGLEVIEW 1 << 7

static const int tagrows = 2;

/* There are two options when it comes to per-client rules:
 *  - a typical struct table or
 *  - using the RULE macro
 *
 * A traditional struct table looks like this:
 *    // class      instance  title  wintype  tags mask  isfloating  monitor
 *    { "Gimp",     NULL,     NULL,  NULL,    1 << 4,    0,          -1 },
 *    { "Firefox",  NULL,     NULL,  NULL,    1 << 7,    0,          -1 },
 *
 * The RULE macro has the default values set for each field allowing you to only
 * specify the values that are relevant for your rule, e.g.
 *
 *    RULE(.class = "Gimp", .tags = 1 << 4)
 *    RULE(.class = "Firefox", .tags = 1 << 7)
 *
 * Refer to the Rule struct definition for the list of available fields depending on
 * the patches you enable.
 */
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 *	WM_WINDOW_ROLE(STRING) = role
	 *	_NET_WM_WINDOW_TYPE(ATOM) = wintype
	 */

	// == Tag 1 Desktop ==
	RULE(.class = "zoom", .isfloating = 1)
		RULE(.class = "Mathpix Snipping Tool", .isfloating = 1)
			RULE(.class = "MATLAB R2024b - academic use", .noswallow = 1, .isfloating = 1)
				RULE(.class = "St", .isterminal = 1, .noswallow = 1)
					RULE(.class = "Alacritty", .isterminal = 1, .noswallow = 1)
						RULE(.class = "kitty", .isterminal = 1, .noswallow = 1)
							RULE(.class = "XTerm", .isterminal = 1, .noswallow = 1)
								RULE(.title = "Event Tester", .noswallow = 1)
									RULE(.instance = "spterm", .tags = SPTAG(0), .isfloating = 1)
										RULE(.instance = "matplotlib", .noswallow = 1, .isfloating = 1)
											RULE(.class = "Matplotlib", .noswallow = 1, .isfloating = 1)
												RULE(.class = "Pavucontrol", .isfloating = 1)
													RULE(.class = "Blueman-manager", .isfloating = 1)
														RULE(.class = "Nm-connection-editor", .isfloating = 1)
															RULE(.class = "Arandr", .isfloating = 1)
																RULE(.class = "Lxappearance", .isfloating = 1)

	// == Tag 2 Browser ==
	RULE(.title = "Zen Browser", .tags = 1 << 1)
		RULE(.class = "firefox", .tags = 1 << 1)
			RULE(.class = "Firefox", .tags = 1 << 1)
				RULE(.class = "Chromium", .tags = 1 << 1)
					RULE(.class = "Google-chrome", .tags = 1 << 1)
						RULE(.class = "Brave-browser", .tags = 1 << 1)
							RULE(.class = "qutebrowser", .tags = 1 << 1)

	// == Tag 3 Workspace ==
	RULE(.instance = "libreoffice", .tags = 1 << 2)
		RULE(.class = "libreoffice-startcenter", .tags = 1 << 2)
			RULE(.class = "Soffice", .tags = 1 << 2)
				RULE(.instance = "com.github.xournalpp.xournalpp", .tags = 1 << 2)
					RULE(.class = "Zathura", .tags = 1 << 2)
						RULE(.class = "Evince", .tags = 1 << 2)
							RULE(.class = "okular", .tags = 1 << 2)
								RULE(.class = "Obsidian", .tags = 1 << 2)
									RULE(.class = "notion-app", .tags = 1 << 2)
										RULE(.class = "logseq", .tags = 1 << 2)

	// == Tag 4 Communication ==
	RULE(.class = "discord", .tags = 1 << 3)
		RULE(.class = "Slack", .tags = 1 << 3)
			RULE(.class = "TelegramDesktop", .tags = 1 << 3)
				RULE(.class = "Telegram", .tags = 1 << 3)
					RULE(.class = "Signal", .tags = 1 << 3)
						RULE(.class = "Element", .tags = 1 << 3)
							RULE(.class = "whatsapp-for-linux", .tags = 1 << 3)
								RULE(.class = "Microsoft Teams - Preview", .tags = 1 << 3)
									RULE(.class = "teams-for-linux", .tags = 1 << 3)
										RULE(.class = "Thunderbird", .tags = 1 << 3)

	// At the end to overwrite previous rules
	RULE(.wintype = WTYPE "DIALOG", .isfloating = 1)
		RULE(.wintype = WTYPE "UTILITY", .isfloating = 1)
			RULE(.wintype = WTYPE "TOOLBAR", .isfloating = 1)
				RULE(.wintype = WTYPE "SPLASH", .isfloating = 1)};

static const MonitorRule monrules[] = {
	/* monitor  tag   layout  mfact  nmaster  showbar  topbar */
	// {  1,       -1,   2,      -1,    -1,      -1,      -1     }, // use a different layout for the second monitor
	{-1, 2, 2, -1, -1, -1, -1},
	{-1, 3, 2, -1, -1, -1, -1},
	{-1, 4, 2, -1, -1, -1, -1},
	{-1, 7, 3, -1, -1, -1, -1},
	{-1, 8, 5, -1, -1, -1, -1},
	{-1, 9, 4, -1, -1, -1, -1},
	{-1, -1, 0, -1, -1, -1, -1}, // default
};

/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
	/* monitor   bar    alignment         widthfunc                 drawfunc                clickfunc                hoverfunc                name */
	{0, 0, BAR_ALIGN_RIGHT, width_systray, draw_systray, click_systray, NULL, "systray"},
	{-1, 0, BAR_ALIGN_LEFT, width_taggrid, draw_taggrid, click_taggrid, NULL, "taggrid"},
	{-1, 0, BAR_ALIGN_LEFT, width_seltagicon, draw_seltagicon, click_seltagicon, NULL, "seltagicon"},
	{-1, 0, BAR_ALIGN_LEFT, width_ltsymbol, draw_ltsymbol, click_ltsymbol, NULL, "layout"},
	{statusmon, 0, BAR_ALIGN_RIGHT, width_pwrl_status, draw_pwrl_status, click_pwrl_status, NULL, "powerline_status"},
	{-1, 0, BAR_ALIGN_NONE, width_flexwintitle, draw_flexwintitle, click_flexwintitle, NULL, "flexwintitle"},
	{statusmon, 1, BAR_ALIGN_LEFT, width_pwrl_status_es, draw_pwrl_status_es, click_pwrl_status, NULL, "powerline_status"},
	{statusmon, 1, BAR_ALIGN_RIGHT, width_pwrl_status_rs, draw_pwrl_status_rs, click_pwrl_status, NULL, "powerline_status"},
	{-1, 1, BAR_ALIGN_CENTER, width_wintitle_floating, draw_wintitle_floating, click_wintitle_floating, NULL, "wintitle_floating"},
};

/* layout(s) */
static const float mfact = 0.55;	 /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;		 /* number of clients in master area */
static const int resizehints = 0;	 /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int decorhints = 1;	 /* 1 means respect decoration hints */

/* mouse scroll resize */
static const int scrollsensetivity = 30; /* 1 means resize window by 1 pixel for each scroll event */
/* resizemousescroll direction argument list */
static const int scrollargs[][2] = {
	/* width change         height change */
	{+scrollsensetivity, 0},
	{-scrollsensetivity, 0},
	{0, +scrollsensetivity},
	{0, -scrollsensetivity},
};

static const Layout layouts[] = {
	/* symbol     arrange function */
	{"[]=", tile}, /* first entry is default */
	{"><>", NULL}, /* no layout function means floating behavior */
	{"[M]", monocle},
	{"TTT", bstack},
	{"|M|", centeredmaster},
	{"[D]", deck},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                          \
	{MODKEY, KEY, comboview, {.ui = 1 << TAG}},                    \
		{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
		{MODKEY | ShiftMask, KEY, combotag, {.ui = 1 << TAG}},     \
		{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                           \
	{                                                        \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

static const char *termcmd[] = {"st", NULL};

static const Key keys[] = {
	/* modifier                     key            function                argument */
	{MODKEY, XK_b, togglebar, {0}},
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},
	{MODKEY, XK_s, swapfocus, {.i = -1}},
	{MODKEY | ControlMask, XK_j, pushdown, {0}},
	{MODKEY | ControlMask, XK_k, pushup, {0}},
	{MODKEY, XK_i, incnmaster, {.i = +1}},
	{MODKEY | ShiftMask, XK_i, incnmaster, {.i = -1}},
	{MODKEY, XK_h, setmfact, {.f = -0.05}},
	{MODKEY, XK_l, setmfact, {.f = +0.05}},
	{MODKEY | ShiftMask, XK_j, setcfact, {.f = -0.25}},
	{MODKEY | ShiftMask, XK_k, setcfact, {.f = +0.25}},
	{MODKEY | ShiftMask, XK_o, setcfact, {0}},
	{MODKEY | Mod1Mask, XK_j, moveresize, {.v = "0x 25y 0w 0h"}},
	{MODKEY | Mod1Mask, XK_k, moveresize, {.v = "0x -25y 0w 0h"}},
	{MODKEY | Mod1Mask, XK_l, moveresize, {.v = "25x 0y 0w 0h"}},
	{MODKEY | Mod1Mask, XK_h, moveresize, {.v = "-25x 0y 0w 0h"}},
	{MODKEY | Mod1Mask | ShiftMask, XK_j, moveresize, {.v = "0x 0y 0w 25h"}},
	{MODKEY | Mod1Mask | ShiftMask, XK_k, moveresize, {.v = "0x 0y 0w -25h"}},
	{MODKEY | Mod1Mask | ShiftMask, XK_l, moveresize, {.v = "0x 0y 25w 0h"}},
	{MODKEY | Mod1Mask | ShiftMask, XK_h, moveresize, {.v = "0x 0y -25w 0h"}},
	{MODKEY, XK_space, zoom, {0}},
	// { MODKEY|Mod4Mask,              XK_u,          incrgaps,               {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_u,          incrgaps,               {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_i,          incrigaps,              {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_i,          incrigaps,              {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_o,          incrogaps,              {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_o,          incrogaps,              {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_6,          incrihgaps,             {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_6,          incrihgaps,             {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_7,          incrivgaps,             {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_7,          incrivgaps,             {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_8,          incrohgaps,             {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_8,          incrohgaps,             {.i = -1 } },
	// { MODKEY|Mod4Mask,              XK_9,          incrovgaps,             {.i = +1 } },
	// { MODKEY|Mod4Mask|ShiftMask,    XK_9,          incrovgaps,             {.i = -1 } },
	{MODKEY | Mod1Mask, XK_0, togglegaps, {0}},
	// { MODKEY|Mod4Mask|ShiftMask,    XK_0,          defaultgaps,            {0} },
	{MODKEY, XK_Tab, view, {0}},
	{Mod1Mask, XK_Tab, shiftviewclients, {.i = +1}},
	{Mod1Mask | ShiftMask, XK_Tab, shiftviewclients, {.i = -1}},
	{MODKEY, XK_z, showhideclient, {0}},
	{MODKEY | ShiftMask, XK_z, showlasthidden, {0}},
	{MODKEY, XK_c, killclient, {0}},
	{MODKEY | ShiftMask, XK_x, killunsel, {0}},
	{MODKEY | ShiftMask, XK_q, quit, {1}},
	{MODKEY | ControlMask | ShiftMask, XK_q, quit, {0}},
	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
	{MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
	{MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
	{MODKEY | ShiftMask, XK_t, setlayout, {.v = &layouts[3]}},
	{MODKEY | ShiftMask, XK_m, setlayout, {.v = &layouts[4]}},
	{MODKEY | ShiftMask, XK_d, setlayout, {.v = &layouts[5]}},
	{MODKEY | ShiftMask, XK_space, setlayout, {0}},
	{MODKEY, XK_bracketleft, togglefloating, {0}},
	{MODKEY | ShiftMask, XK_Return, togglescratch, {.ui = 0}},
	{MODKEY | ControlMask, XK_w, setscratch, {.ui = 0}},
	{MODKEY | ShiftMask, XK_w, removescratch, {.ui = 0}},
	{MODKEY | ControlMask, XK_s, togglesticky, {0}},
	{MODKEY, XK_0, view, {.ui = ~SPTAGMASK}},
	{MODKEY | ShiftMask, XK_0, tag, {.ui = ~SPTAGMASK}},
	// { MODKEY,                       XK_comma,      focusmon,               {.i = -1 } },
	// { MODKEY,                       XK_period,     focusmon,               {.i = +1 } },
	// { MODKEY|ShiftMask,             XK_comma,      tagmon,                 {.i = -1 } },
	// { MODKEY|ShiftMask,             XK_period,     tagmon,                 {.i = +1 } },
	// { MODKEY|ControlMask,           XK_Up,         switchtag,              { .ui = SWITCHTAG_UP    | SWITCHTAG_VIEW } },
	// { MODKEY|ControlMask,           XK_Down,       switchtag,              { .ui = SWITCHTAG_DOWN  | SWITCHTAG_VIEW } },
	// { MODKEY|ControlMask,           XK_Right,      switchtag,              { .ui = SWITCHTAG_RIGHT | SWITCHTAG_VIEW } }, // note keybinding conflict with placedir
	// { MODKEY|ControlMask,           XK_Left,       switchtag,              { .ui = SWITCHTAG_LEFT  | SWITCHTAG_VIEW } }, // note keybinding conflict with placedir
	// { MODKEY|Mod4Mask,              XK_Up,         switchtag,              { .ui = SWITCHTAG_UP    | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	// { MODKEY|Mod4Mask,              XK_Down,       switchtag,              { .ui = SWITCHTAG_DOWN  | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	// { MODKEY|Mod4Mask,              XK_Right,      switchtag,              { .ui = SWITCHTAG_RIGHT | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	// { MODKEY|Mod4Mask,              XK_Left,       switchtag,              { .ui = SWITCHTAG_LEFT  | SWITCHTAG_TAG | SWITCHTAG_VIEW } },
	{MODKEY, XK_KP_7, explace, {.ui = EX_NW}}, /* XK_KP_Home,  */
	{MODKEY, XK_KP_8, explace, {.ui = EX_N}},  /* XK_KP_Up,    */
	{MODKEY, XK_KP_9, explace, {.ui = EX_NE}}, /* XK_KP_Prior, */
	{MODKEY, XK_KP_4, explace, {.ui = EX_W}},  /* XK_KP_Left,  */
	{MODKEY, XK_KP_5, explace, {.ui = EX_C}},  /* XK_KP_Begin, */
	{MODKEY, XK_KP_6, explace, {.ui = EX_E}},  /* XK_KP_Right, */
	{MODKEY, XK_KP_1, explace, {.ui = EX_SW}}, /* XK_KP_End,   */
	{MODKEY, XK_KP_2, explace, {.ui = EX_S}},  /* XK_KP_Down,  */
	{MODKEY, XK_KP_3, explace, {.ui = EX_SE}}, /* XK_KP_Next,  */

	{MODKEY | ShiftMask, XK_KP_8, exresize, {.v = (int[]){0, 25}}},					 /* XK_KP_Up,    */
	{MODKEY | ShiftMask, XK_KP_2, exresize, {.v = (int[]){0, -25}}},				 /* XK_KP_Down,  */
	{MODKEY | ShiftMask, XK_KP_6, exresize, {.v = (int[]){25, 0}}},					 /* XK_KP_Right, */
	{MODKEY | ShiftMask, XK_KP_4, exresize, {.v = (int[]){-25, 0}}},				 /* XK_KP_Left,  */
	{MODKEY | ShiftMask, XK_KP_5, exresize, {.v = (int[]){25, 25}}},				 /* XK_KP_Begin, */
	{MODKEY | ShiftMask | ControlMask, XK_KP_5, exresize, {.v = (int[]){-25, -25}}}, /* XK_KP_Begin, */

	{MODKEY | ControlMask, XK_KP_6, togglehorizontalexpand, {.i = +1}}, /* XK_KP_Right, */
	{MODKEY | ControlMask, XK_KP_3, togglehorizontalexpand, {.i = 0}},	/* XK_KP_Next,  */
	{MODKEY | ControlMask, XK_KP_4, togglehorizontalexpand, {.i = -1}}, /* XK_KP_Left,  */
	{MODKEY | ControlMask, XK_KP_8, toggleverticalexpand, {.i = +1}},	/* XK_KP_Up,    */
	{MODKEY | ControlMask, XK_KP_1, toggleverticalexpand, {.i = 0}},	/* XK_KP_End,   */
	{MODKEY | ControlMask, XK_KP_2, toggleverticalexpand, {.i = -1}},	/* XK_KP_Down,  */
	{MODKEY | ControlMask, XK_KP_9, togglemaximize, {.i = -1}},			/* XK_KP_Prior, */
	{MODKEY | ControlMask, XK_KP_7, togglemaximize, {.i = +1}},			/* XK_KP_Home,  */
	{MODKEY | ControlMask, XK_KP_5, togglemaximize, {.i = 0}},			/* XK_KP_Begin, */
	{MODKEY | ControlMask, XK_minus, setborderpx, {.i = -1}},
	{MODKEY | ControlMask | ShiftMask, XK_equal, setborderpx, {.i = +1}},
	{MODKEY | ControlMask, XK_numbersign, setborderpx, {.i = 0}},
	{MODKEY | ControlMask, XK_comma, cyclelayout, {.i = -1}},
	{MODKEY | ControlMask, XK_period, cyclelayout, {.i = +1}},
	TAGKEYS(XK_1, 0)
		TAGKEYS(XK_2, 1)
			TAGKEYS(XK_3, 2)
				TAGKEYS(XK_4, 3)
					TAGKEYS(XK_5, 4)
						TAGKEYS(XK_6, 5)
							TAGKEYS(XK_7, 6)
								TAGKEYS(XK_8, 7)
									TAGKEYS(XK_9, 8)};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask           button          function        argument */
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ClkWinTitle, 0, Button1, togglewin, {0}},
	{ClkWinTitle, 0, Button3, showhideclient, {0}},
	{ClkWinTitle, 0, Button2, zoom, {0}},
	{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
	/* placemouse options, choose which feels more natural:
	 *    0 - tiled position is relative to mouse cursor
	 *    1 - tiled postiion is relative to window center
	 *    2 - mouse pointer warps to window center
	 *
	 * The moveorplace uses movemouse or placemouse depending on the floating state
	 * of the selected client. Set up individual keybindings for the two if you want
	 * to control these separately (i.e. to retain the feature to move a tiled window
	 * into a floating position).
	 */
	{ClkClientWin, MODKEY, Button1, moveorplace, {.i = 1}},
	{ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ClkClientWin, MODKEY, Button4, resizemousescroll, {.v = &scrollargs[0]}},
	{ClkClientWin, MODKEY, Button5, resizemousescroll, {.v = &scrollargs[1]}},
	{ClkClientWin, MODKEY, Button6, resizemousescroll, {.v = &scrollargs[2]}},
	{ClkClientWin, MODKEY, Button7, resizemousescroll, {.v = &scrollargs[3]}},
	{ClkClientWin, MODKEY | ControlMask, Button1, dragcfact, {0}},
	{ClkClientWin, MODKEY | ShiftMask, Button1, dragmfact, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	{ClkTagBar, MODKEY, Button1, tag, {0}},
	{ClkTagBar, MODKEY, Button3, toggletag, {0}},
};
