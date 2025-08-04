/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "librewolf"

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "Amiri:size=10", "NotoColorEmoji:pixelsize=10:antialias=true:autohint=true"  };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,          1 << 8,      0,           0,          0,         -1 },
	{ TERMCLASS,  NULL,       NULL,       	 0,           0,           1,          0,         -1 },
	{ NULL,       NULL,       "Event Tester", 0,          0,           0,          1,         -1 },
	{ TERMCLASS,  "floatterm", NULL,       	 0,           1,           1,          0,         -1 },
	{ TERMCLASS,  "bg",        NULL,       	 1 << 7,      0,           1,          0,         -1 },
	{ TERMCLASS,  "spterm",    NULL,       	 SPTAG(0),    1,           1,          0,         -1 },
	{ TERMCLASS,  "spcalc",    NULL,       	 SPTAG(1),    1,           1,          0,         -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",	tile },	                /* Default: Master on left, slaves on right */
	{ "TTT",	bstack },               /* Master on top, slaves on bottom */

	{ "[@]",	spiral },               /* Fibonacci spiral */
	{ "[\\]",	dwindle },              /* Decreasing in size right and leftward */

	{ "[D]",	deck },	                /* Master on left, slaves in monocle-like mode on right */
	{ "[M]",	monocle },              /* All windows on top of eachother */

	{ "|M|",	centeredmaster },               /* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },       /* Same but master floats */

	{ "><>",	NULL },	                /* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	44,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	45,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  55,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, 49, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, 38,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, 52,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, 53,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		STRING,	&normbordercolor },
		{ "color8",		STRING,	&selbordercolor },
		{ "color0",		STRING,	&normbgcolor },
		{ "color4",		STRING,	&normfgcolor },
		{ "color0",		STRING,	&selfgcolor },
		{ "color4",		STRING,	&selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static const Key keys[] = {
	/* modifier                     key            function                argument */
	STACKKEYS(MODKEY,                              focus)
	STACKKEYS(MODKEY|ShiftMask,                    push)
	/* { MODKEY|ShiftMask,		XK_Escape,     spawn,	               SHCMD("") }, */
	{ MODKEY,			49,      spawn,	               {.v = (const char*[]){ "dmenuunicode", NULL } } },
	/* { MODKEY|ShiftMask,		49,      togglescratch,	       SHCMD("") }, */
	TAGKEYS(			10,          0)
	TAGKEYS(			11,          1)
	TAGKEYS(			12,          2)
	TAGKEYS(			13,          3)
	TAGKEYS(			14,          4)
	TAGKEYS(			15,          5)
	TAGKEYS(			16,          6)
	TAGKEYS(			17,          7)
	TAGKEYS(			18,          8)
	{ MODKEY,			19,	       view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,		19,	       tag,                    {.ui = ~0 } },
	{ MODKEY,			20,      spawn,                  SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%-; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		20,      spawn,                  SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%-; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			21,      spawn,                  SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 5%+; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		21,      spawn,                  SHCMD("wpctl set-volume @DEFAULT_AUDIO_SINK@ 15%+; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			22,  spawn,                  {.v = (const char*[]){ "sysact", NULL } } },
	{ MODKEY|ShiftMask,		22,  spawn,                  {.v = (const char*[]){ "sysact", NULL } } },

	{ MODKEY,			23,        view,                   {0} },
	/* { MODKEY|ShiftMask,		23,	       spawn,                  SHCMD("") }, */
	{ MODKEY,			24,          killclient,             {0} },
	{ MODKEY|ShiftMask,		24,          spawn,                  {.v = (const char*[]){ "sysact", NULL } } },
	{ MODKEY,			25,          spawn,                  {.v = (const char*[]){ BROWSER, NULL } } },
	{ MODKEY|ShiftMask,		25,          spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "nmtui", NULL } } },
	{ MODKEY,			26,          spawn,                  SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook 2>/dev/null") },
	{ MODKEY|ShiftMask,		26,          spawn,                  SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
	{ MODKEY,			27,          spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "lfub", NULL } } },
	{ MODKEY|ShiftMask,		27,          spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
	{ MODKEY,			28,          setlayout,              {.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		28,          setlayout,              {.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			29,          setlayout,              {.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,		29,          setlayout,              {.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,			30,          setlayout,              {.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,		30,          setlayout,              {.v = &layouts[5]} }, /* monocle */
	{ MODKEY,			31,          setlayout,              {.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		31,          setlayout,              {.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,			32,          incnmaster,             {.i = +1 } },
	{ MODKEY|ShiftMask,		32,          incnmaster,             {.i = -1 } },
	{ MODKEY,			33,          spawn,                  {.v = (const char*[]){ "mpc", "toggle", NULL } } },
	{ MODKEY|ShiftMask,		33,          spawn,                  SHCMD("mpc pause; pauseallmpv") },
	{ MODKEY,			34, spawn,                 {.v = (const char*[]){ "mpc", "seek", "-10", NULL } } },
	{ MODKEY|ShiftMask,		34, spawn,                 {.v = (const char*[]){ "mpc", "seek", "-60", NULL } } },
	{ MODKEY,			35, spawn,                {.v = (const char*[]){ "mpc", "seek", "+10", NULL } } },
	{ MODKEY|ShiftMask,		35, spawn,                {.v = (const char*[]){ "mpc", "seek", "+60", NULL } } },
	{ MODKEY,			51,  view,                   {0} },
	/* { MODKEY|ShiftMask,		51,  spawn,                  SHCMD("") }, */

	{ MODKEY|ControlMask,		43,	       spawn,		       SHCMD("xrandr -o left") },
	{ MODKEY|ControlMask,		44,	       spawn,		       SHCMD("xrandr -o normal") },
	{ MODKEY|ControlMask,		45,	       spawn,		       SHCMD("xrandr -o inverted") },
	{ MODKEY|ControlMask,		46,	       spawn,		       SHCMD("xrandr -o right") },

	{ MODKEY,			38,          togglegaps,             {0} },
	{ MODKEY|ShiftMask,		38,          defaultgaps,            {0} },
	{ MODKEY,			39,          togglesticky,           {0} },
	/* { MODKEY|ShiftMask,		39,          spawn,                  SHCMD("") }, */
	{ MODKEY,			40,          spawn,                  {.v = (const char*[]){ "dmenu_run", NULL } } },
	{ MODKEY|ShiftMask,		40,          spawn,                  {.v = (const char*[]){ "passmenu", NULL } } },
	{ MODKEY,			41,          togglefullscr,          {0} },
	{ MODKEY|ShiftMask,		41,          setlayout,              {.v = &layouts[8]} },
	{ MODKEY,			42,          shiftview,              { .i = -1 } },
	{ MODKEY|ShiftMask,		42,          shifttag,               { .i = -1 } },
	{ MODKEY,			43,          setmfact,               {.f = -0.05} },
	/* J and K are automatically bound above in STACKEYS */
	{ MODKEY,			46,          setmfact,               {.f = +0.05} },
	{ MODKEY,			47,  shiftview,              { .i = 1 } },
	{ MODKEY|ShiftMask,		47,  shifttag,               { .i = 1 } },
	{ MODKEY,			48, togglescratch,          {.ui = 1} },
	/* { MODKEY|ShiftMask,		48, spawn,                  SHCMD("") }, */
	{ MODKEY|ShiftMask,		48, togglesmartgaps,        {0} },
	{ MODKEY,			36,     spawn,                  {.v = termcmd } },
	{ MODKEY|ShiftMask,		36,     togglescratch,          {.ui = 0} },

	{ MODKEY,			52,          incrgaps,               {.i = +3 } },
	/* { MODKEY|ShiftMask,		52,          spawn,                  SHCMD("") }, */
	{ MODKEY,			53,          incrgaps,               {.i = -3 } },
	/* { MODKEY|ShiftMask,		53,          spawn,                  SHCMD("") }, */
	{ MODKEY,			54,          spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "profanity", NULL } } },
	/* { MODKEY|ShiftMask,		54,          spawn,                  SHCMD("") }, */
	/* V is automatically bound above in STACKKEYS */
	{ MODKEY,			56,          togglebar,              {0} },
	{ MODKEY|ShiftMask,		56,          spawn,                  SHCMD("bookmarkthis") },
	{ MODKEY,			57,          spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "nvim", "-c", "VimwikiIndex", NULL } } },
	{ MODKEY|ShiftMask,		57,          spawn,                  SHCMD(TERMINAL " -e newsboat ; pkill -RTMIN+6 dwmblocks") },
	{ MODKEY,			58,          spawn,                  {.v = (const char*[]){ TERMINAL, "-e", "ncmpcpp", NULL } } },
	{ MODKEY|ShiftMask,		58,          spawn,                  SHCMD("wpctl set-mute @DEFAULT_AUDIO_SINK@ toggle; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			59,      spawn,                  {.v = (const char*[]){ "mpc", "prev", NULL } } },
	{ MODKEY|ShiftMask,		59,      spawn,                  {.v = (const char*[]){ "mpc", "seek", "0%", NULL } } },
	{ MODKEY,			60,     spawn,                  {.v = (const char*[]){ "mpc", "next", NULL } } },
	{ MODKEY|ShiftMask,		60,     spawn,                  {.v = (const char*[]){ "mpc", "repeat", NULL } } },

	{ MODKEY,			113,       focusmon,               {.i = -1 } },
	{ MODKEY|ShiftMask,		113,       tagmon,                 {.i = -1 } },
	{ MODKEY,			114,      focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,		114,      tagmon,                 {.i = +1 } },

	{ MODKEY,			112,    shiftview,              { .i = -1 } },
	{ MODKEY|ShiftMask,		112,    shifttag,               { .i = -1 } },
	{ MODKEY,			117,  shiftview,              { .i = +1 } },
	{ MODKEY|ShiftMask,		117,  shifttag,               { .i = +1 } },
	{ MODKEY,			118,     spawn,                  SHCMD("xdotool type $(grep -v '^#' ~/.local/share/larbs/snippets/urls | dmenu -i -l 33 | cut -d' ' -f1)") },
	{ MODKEY|ShiftMask,		118,     spawn,          	       SHCMD("snippet") },

	{ MODKEY,			67,         spawn,                  SHCMD("guide") },
	{ MODKEY,			68,         spawn,                  {.v = (const char*[]){ "tutorialvids", NULL } } },
	{ MODKEY,			69,         spawn,                  {.v = (const char*[]){ "displayselect", NULL } } },
	{ MODKEY,			70,         spawn,                  SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			71,         xrdb,                   {.v = NULL } },
	{ MODKEY,			72,         spawn,                  {.v = (const char*[]){ "torwrap", NULL } } },
	{ MODKEY,			73,         spawn,                  {.v = (const char*[]){ "td-toggle", NULL } } },
	{ MODKEY,			74,         spawn,                  {.v = (const char*[]){ "mailsync", NULL } } },
	{ MODKEY,			75,         spawn,                  {.v = (const char*[]){ "mounter", NULL } } },
	{ MODKEY,			76,        spawn,                  {.v = (const char*[]){ "unmounter", NULL } } },
	{ MODKEY,			95,        spawn,                  SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") },
	{ MODKEY,			96,        spawn,                  SHCMD("remaps") },
	{ MODKEY,			65,      zoom,                   {0} },
	{ MODKEY|ShiftMask,		65,      togglefloating,         {0} },

	{ 0,				107,      spawn,                  SHCMD("maim pic-full-$(date '+%y%m%d-%H%M-%S').png") },
	{ ShiftMask,			107,      spawn,                  {.v = (const char*[]){ "maimpick", NULL } } },
	{ MODKEY,			107,      spawn,		       {.v = (const char*[]){ "dmenurecord", NULL } } },
	{ MODKEY|ShiftMask,		107,      spawn,                  {.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
	{ MODKEY,			119,     spawn,                  {.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
	{ MODKEY|ControlMask,		107,      spawn,                  SHCMD("killall screenkey || screenkey &") },


	/* { MODKEY|Mod4Mask,           43,          incrgaps,               {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,           46,          incrgaps,               {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask, 43,          incrogaps,              {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask, 46,          incrogaps,              {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask, 43,        incrigaps,              {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask, 46,        incrigaps,              {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask, XK_0,          defaultgaps,            {0} }, */
	/* { MODKEY,                    29,          incrihgaps,             {.i = +1 } }, */
	/* { MODKEY,                    32,          incrihgaps,             {.i = -1 } }, */
	/* { MODKEY|ControlMask,        29,          incrivgaps,             {.i = +1 } }, */
	/* { MODKEY|ControlMask,        32,          incrivgaps,             {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,           29,          incrohgaps,             {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,           32,          incrohgaps,             {.i = -1 } }, */
	/* { MODKEY|ShiftMask,          29,          incrovgaps,             {.i = +1 } }, */
	/* { MODKEY|ShiftMask,          32,          incrovgaps,             {.i = -1 } }, */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask           button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,                   Button2,        zoom,           {0} },
	{ ClkStatusText,        0,                   Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,                   Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,                   Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,                   Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,                   Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,           Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,           Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,              Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,              Button2,        defaultgaps,    {0} },
	{ ClkClientWin,         MODKEY,              Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		     Button4,	     incrgaps,       {.i = +1} },
	{ ClkClientWin,		MODKEY,		     Button5,	     incrgaps,       {.i = -1} },
	{ ClkTagBar,            0,                   Button1,        view,           {0} },
	{ ClkTagBar,            0,                   Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,              Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,              Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		     Button4,	     shiftview,      {.i = -1} },
	{ ClkTagBar,		0,		     Button5,	     shiftview,      {.i = 1} },
	{ ClkRootWin,		0,		     Button2,	     togglebar,      {0} },
};

