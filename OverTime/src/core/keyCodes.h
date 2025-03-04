#pragma once 
#ifndef KEYCODES_H
#define KEYCODES_H

//keycodes from glfw3.h transcripted to enum
enum keyCodes {
	OT_KEY_SPACE = 32,

	OT_KEY_APOSTROPHE = 39,  /* ' */

	OT_KEY_COMMA = 44, /* , */
	OT_KEY_MINUS,  /* - */
	OT_KEY_PERIOD,  /* . */
	OT_KEY_SLASH,   /* / */
	OT_KEY_0,
	OT_KEY_1,
	OT_KEY_2,
	OT_KEY_3,
	OT_KEY_4,
	OT_KEY_5,
	OT_KEY_6,
	OT_KEY_7,
	OT_KEY_8,
	OT_KEY_9,

	OT_KEY_SEMICOLON = 59, /* ; */

	OT_KEY_EQUAL = 61,  /* = */

	OT_KEY_A = 65,
	OT_KEY_B,
	OT_KEY_C,
	OT_KEY_D,
	OT_KEY_E,
	OT_KEY_F,
	OT_KEY_G,
	OT_KEY_H,
	OT_KEY_I,
	OT_KEY_J,
	OT_KEY_K,
	OT_KEY_L,
	OT_KEY_M,
	OT_KEY_N,
	OT_KEY_O,
	OT_KEY_P,
	OT_KEY_Q,
	OT_KEY_R,
	OT_KEY_S,
	OT_KEY_T,
	OT_KEY_U,
	OT_KEY_V,
	OT_KEY_W,
	OT_KEY_X,
	OT_KEY_Y,
	OT_KEY_Z,
	OT_KEY_LEFT_BRACKET,  /* [ */
	OT_KEY_BACKSLASH,  /* \ */
	OT_KEY_RIGHT_BRACKET,  /* ] */

	OT_KEY_GRAVE_ACCENT = 96,  /* ` */

	OT_KEY_WORLD_1 = 161, /* non-US #1 */
	OT_KEY_WORLD_2 = 162, /* non-US #2 */

	/* Function keys */
	OT_KEY_ESCAPE = 256,
	OT_KEY_ENTER,
	OT_KEY_TAB,
	OT_KEY_BACKSPACE,
	OT_KEY_INSERT,
	OT_KEY_DELETE,
	OT_KEY_RIGHT,
	OT_KEY_LEFT,
	OT_KEY_DOWN,
	OT_KEY_UP,
	OT_KEY_PAGE_UP,
	OT_KEY_PAGE_DOWN,
	OT_KEY_HOME,
	OT_KEY_END,

	OT_KEY_CAPS_LOCK = 280,
	OT_KEY_SCROLL_LOCK,
	OT_KEY_NUM_LOCK,
	OT_KEY_PRINT_SCREEN,
	OT_KEY_PAUSE,

	OT_KEY_F1 = 290,
	OT_KEY_F2,
	OT_KEY_F3,
	OT_KEY_F4,
	OT_KEY_F5,
	OT_KEY_F6,
	OT_KEY_F7,
	OT_KEY_F8,
	OT_KEY_F9,
	OT_KEY_F10,
	OT_KEY_F11,
	OT_KEY_F12,
	OT_KEY_F13,
	OT_KEY_F14,
	OT_KEY_F15,
	OT_KEY_F16,
	OT_KEY_F17,
	OT_KEY_F18,
	OT_KEY_F19,
	OT_KEY_F20,
	OT_KEY_F21,
	OT_KEY_F22,
	OT_KEY_F23,
	OT_KEY_F24,
	OT_KEY_F25,

	OT_KEY_KP_0 = 320,
	OT_KEY_KP_1,
	OT_KEY_KP_2,
	OT_KEY_KP_3,
	OT_KEY_KP_4,
	OT_KEY_KP_5,
	OT_KEY_KP_6,
	OT_KEY_KP_7,
	OT_KEY_KP_8,
	OT_KEY_KP_9,
	OT_KEY_KP_DECIMAL,
	OT_KEY_KP_DIVIDE,
	OT_KEY_KP_MULTIPLY,
	OT_KEY_KP_SUBTRACT,
	OT_KEY_KP_ADD,
	OT_KEY_KP_ENTER,
	OT_KEY_KP_EQUAL,

	OT_KEY_LEFT_SHIFT = 340,
	OT_KEY_LEFT_CONTROL,
	OT_KEY_LEFT_ALT,
	OT_KEY_LEFT_SUPER,
	OT_KEY_RIGHT_SHIFT,
	OT_KEY_RIGHT_CONTROL,
	OT_KEY_RIGHT_ALT,
	OT_KEY_RIGHT_SUPER,
	OT_KEY_MENU
};


//OG stolen glfw3.h keycodes
/*#define OT_KEY_SPACE              32

#define OT_KEY_APOSTROPHE         39 

#define OT_KEY_COMMA              44  // ,
#define OT_KEY_MINUS              45  // - 
#define OT_KEY_PERIOD             46 // . 
#define OT_KEY_SLASH              47 // /
#define OT_KEY_0                  48
#define OT_KEY_1                  49
#define OT_KEY_2                  50
#define OT_KEY_3                  51
#define OT_KEY_4                  52
#define OT_KEY_5                  53
#define OT_KEY_6                  54
#define OT_KEY_7                  55
#define OT_KEY_8                  56
#define OT_KEY_9                  57

#define OT_KEY_SEMICOLON          59  // ; 

#define OT_KEY_EQUAL              61  // = 

#define OT_KEY_A                  65
#define OT_KEY_B                  66
#define OT_KEY_C                  67
#define OT_KEY_D                  68
#define OT_KEY_E                  69
#define OT_KEY_F                  70
#define OT_KEY_G                  71
#define OT_KEY_H                  72
#define OT_KEY_I                  73
#define OT_KEY_J                  74
#define OT_KEY_K                  75
#define OT_KEY_L                  76
#define OT_KEY_M                  77
#define OT_KEY_N                  78
#define OT_KEY_O                  79
#define OT_KEY_P                  80
#define OT_KEY_Q                  81
#define OT_KEY_R                  82
#define OT_KEY_S                  83
#define OT_KEY_T                  84
#define OT_KEY_U                  85
#define OT_KEY_V                  86
#define OT_KEY_W                  87
#define OT_KEY_X                  88
#define OT_KEY_Y                  89
#define OT_KEY_Z                  90
#define OT_KEY_LEFT_BRACKET       91  // [ 
#define OT_KEY_BACKSLASH          92  // \ 
#define OT_KEY_RIGHT_BRACKET      93  // ] 

#define OT_KEY_GRAVE_ACCENT       96  // ` 

#define OT_KEY_WORLD_1            161 // non-US #1
#define OT_KEY_WORLD_2            162 // non-US #2 

// Function keys 
#define OT_KEY_ESCAPE             256
#define OT_KEY_ENTER              257
#define OT_KEY_TAB                258
#define OT_KEY_BACKSPACE          259
#define OT_KEY_INSERT             260
#define OT_KEY_DELETE             261
#define OT_KEY_RIGHT              262
#define OT_KEY_LEFT               263
#define OT_KEY_DOWN               264
#define OT_KEY_UP                 265
#define OT_KEY_PAGE_UP            266
#define OT_KEY_PAGE_DOWN          267
#define OT_KEY_HOME               268
#define OT_KEY_END                269

#define OT_KEY_CAPS_LOCK          280
#define OT_KEY_SCROLL_LOCK        281
#define OT_KEY_NUM_LOCK           282
#define OT_KEY_PRINT_SCREEN       283
#define OT_KEY_PAUSE              284

#define OT_KEY_F1                 290
#define OT_KEY_F2                 291
#define OT_KEY_F3                 292
#define OT_KEY_F4                 293
#define OT_KEY_F5                 294
#define OT_KEY_F6                 295
#define OT_KEY_F7                 296
#define OT_KEY_F8                 297
#define OT_KEY_F9                 298
#define OT_KEY_F10                299
#define OT_KEY_F11                300
#define OT_KEY_F12                301
#define OT_KEY_F13                302
#define OT_KEY_F14                303
#define OT_KEY_F15                304
#define OT_KEY_F16                305
#define OT_KEY_F17                306
#define OT_KEY_F18                307
#define OT_KEY_F19                308
#define OT_KEY_F20                309
#define OT_KEY_F21                310
#define OT_KEY_F22                311
#define OT_KEY_F23                312
#define OT_KEY_F24                313
#define OT_KEY_F25                314

#define OT_KEY_KP_0               320
#define OT_KEY_KP_1               321
#define OT_KEY_KP_2               322
#define OT_KEY_KP_3               323
#define OT_KEY_KP_4               324
#define OT_KEY_KP_5               325
#define OT_KEY_KP_6               326
#define OT_KEY_KP_7               327
#define OT_KEY_KP_8               328
#define OT_KEY_KP_9               329
#define OT_KEY_KP_DECIMAL         330
#define OT_KEY_KP_DIVIDE          331
#define OT_KEY_KP_MULTIPLY        332
#define OT_KEY_KP_SUBTRACT        333
#define OT_KEY_KP_ADD             334
#define OT_KEY_KP_ENTER           335
#define OT_KEY_KP_EQUAL           336

#define OT_KEY_LEFT_SHIFT         340
#define OT_KEY_LEFT_CONTROL       341
#define OT_KEY_LEFT_ALT           342
#define OT_KEY_LEFT_SUPER         343
#define OT_KEY_RIGHT_SHIFT        344
#define OT_KEY_RIGHT_CONTROL      345
#define OT_KEY_RIGHT_ALT          346
#define OT_KEY_RIGHT_SUPER        347
#define OT_KEY_MENU               348 */
#endif