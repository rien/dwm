/* See LICENSE file for copyright and license details. */

#include <stdlib.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "xft:Source Code Pro:size=11", "xft:Consolas:size=11" };
static const char dmenufont[]       = "xft:Source Code Pro:size=11";

/* colorscheme */
static const char col_center_darkest[]   = "#222C00";
static const char col_center_darker[]    = "#516709";
static const char col_center_middle[]    = "#89A236";
static const char col_center_lighter[]   = "#C7DD80";
static const char col_center_lightest[]  = "#EEF9CE";

static const char col_colder_darkest[]   = "#0D2800";
static const char col_colder_darker[]    = "#245D08";
static const char col_colder_middle[]    = "#519331";
static const char col_colder_lighter[]   = "#90C975";
static const char col_colder_lightest[]  = "#D1ECC4";

static const char col_warmer_darkest[]   = "#2E2C00";
static const char col_warmer_darker[]    = "#6C6709";
static const char col_warmer_middle[]    = "#AAA439";
static const char col_warmer_lighter[]   = "#E8E287";
static const char col_warmer_lightest[]  = "#FFFDD3";

static const char *colors[SchemeLast][3] = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_center_lightest, col_center_darkest, col_warmer_darkest },
	[SchemeSel] =  { col_center_middle,   col_center_darkest, col_warmer_middle  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[M]",      monocle }, /* first entry is default */
	{ "[]=",      tile },
	{ "><>",      NULL },    /* no layout function means floating behavior */
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]    = { "dmenu_run",   "-m", dmenumon, "-fn", dmenufont, "-nb", col_colder_darkest, "-nf", col_colder_lightest, "-sb", col_colder_middle, "-sf", col_colder_lightest, NULL };
static const char *termcmd[]     = { "urxvtc",      NULL  };
static const char *abduco_run[]  = { "abduco_run",  NULL  };
static const char *abduco_list[] = { "abduco_list", NULL  };

static const char *sound_toggle[] = { "sound_control.sh", "toggle", NULL };
static const char *sound_up[]     = { "sound_control.sh", "up",     NULL };
static const char *sound_down[]   = { "sound_control.sh", "down",   NULL };

static const char *mpc_next[]   = { "mpc", "-q", "next",   NULL };
static const char *mpc_toggle[] = { "mpc", "-q", "toggle", NULL };
static const char *mpc_prev[]   = { "mpc", "-q", "prev",   NULL };

static const char *brightness_up[]   = { "xbacklight", "-inc", "10", NULL };
static const char *brightness_down[] = { "xbacklight", "-dec", "10", NULL };

static Key keys[] = {
	/* modifier         key             function        argument */
    /* function keys */
	{ 0, 0x1008ff12, spawn, { .v = sound_toggle    } }, /* XF86XK_AudioMute */
	{ 0, 0x1008ff13, spawn, { .v = sound_up        } }, /* XF86XK_AudioRaiseVolume */
	{ 0, 0x1008ff11, spawn, { .v = sound_down      } }, /* XF86XK_AudioLowerVolume */
	{ 0, 0x1008ff17, spawn, { .v = mpc_next        } }, /* XF86XK_AudioNext */
	{ 0, 0x1008ff16, spawn, { .v = mpc_prev        } }, /* XF86XK_AudioPrev */
	{ 0, 0x1008ff14, spawn, { .v = mpc_toggle      } }, /* XF86XK_AudioPlay */
	{ 0, 0x1008ff02, spawn, { .v = brightness_up   } }, /* XF86XK_MonBrightnessUp */
	{ 0, 0x1008ff03, spawn, { .v = brightness_down } }, /* XF86XK_MonBrightnessDown */
      /* 0x1008ff1b = XF86Search */

    /* Other keys (scroll lock is rebound using xcape) */
	{ 0,                XK_Scroll_Lock, spawn,          { .v  = dmenucmd    } },
	{ MODKEY,           XK_s,           spawn,          { .v  = abduco_run  } },
	{ MODKEY,           XK_a,           spawn,          { .v  = abduco_list } },
	{ MODKEY|ShiftMask, XK_Return,      spawn,          { .v  = termcmd     } },
	{ MODKEY,           XK_b,           togglebar,      { 0                 } },
	{ MODKEY,           XK_j,           focusstack,     { .i  = +1          } },
	{ MODKEY,           XK_k,           focusstack,     { .i  = -1          } },
	{ MODKEY,           XK_comma,       incnmaster,     { .i  = +1          } },
	{ MODKEY,           XK_period,      incnmaster,     { .i  = -1          } },
	{ MODKEY,           XK_h,           setmfact,       { .f  = -0.05       } },
	{ MODKEY,           XK_l,           setmfact,       { .f  = +0.05       } },
	{ MODKEY,           XK_Return,      zoom,           { 0                 } },
	{ MODKEY,           XK_Tab,         view,           { 0                 } },
	{ MODKEY|ShiftMask, XK_c,           killclient,     { 0                 } },
	{ MODKEY,           XK_m,           setlayout,      { .v  = &layouts[0] } },
	{ MODKEY,           XK_t,           setlayout,      { .v  = &layouts[1] } },
	{ MODKEY,           XK_f,           setlayout,      { .v  = &layouts[2] } },
	{ MODKEY,           XK_space,       setlayout,      { 0                 } },
	{ MODKEY|ShiftMask, XK_space,       togglefloating, { 0                 } },
	{ MODKEY,           XK_0,           view,           { .ui = ~0          } },
	{ MODKEY|ShiftMask, XK_0,           tag,            { .ui = ~0          } },
	{ MODKEY,           XK_w,           focusmon,       { .i  = -1          } },
	{ MODKEY,           XK_e,           focusmon,       { .i  = +1          } },
	{ MODKEY|ShiftMask, XK_w,           tagmon,         { .i  = -1          } },
	{ MODKEY|ShiftMask, XK_e,           tagmon,         { .i  = +1          } },
	{ MODKEY|ShiftMask, XK_q,           quit,           { 0                 } },

	TAGKEYS(XK_1, 0)
	TAGKEYS(XK_2, 1)
	TAGKEYS(XK_3, 2)
	TAGKEYS(XK_4, 3)
	TAGKEYS(XK_5, 4)
	TAGKEYS(XK_6, 5)
	TAGKEYS(XK_7, 6)
	TAGKEYS(XK_8, 7)
	TAGKEYS(XK_9, 8)
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

