/*
Definiert die Terminal ESC Sequencen
* Author: Michael Albert
* ANSI ESC Sequenes
http://de.wikipedia.org/wiki/American_Standard_Code_for_Information_Interchange
http://en.wikipedia.org/wiki/ANSI_escape_code
http://ascii-table.com/ansi-escape-sequences-vt-100.php
*/


#define TERM_RESET 				"\x1b[0m"
#define TERM_BOLD				"\x1b[1m"
#define TERM_UNDERLINE_SINGLE	"\x1b[4m"
#define TERM_BLINK_SLOW			"\x1b[5m"
#define TERM_BLINK_RAPID		"\x1b[6m"
#define TERM_BLINK_OFF			"\x1b[25m"
#define TERM_SWAP_FG_BG			"\x1b[7m"
#define TERM_UNDERLINE_NONE		"\x1b[24m"


#define TERM_FG_BLACK		"\x1b[0;30m"
#define TERM_FG_RED			"\x1b[0;31m"
#define TERM_FG_GREEN		"\x1b[0;32m"
#define TERM_FG_YELLOW		"\x1b[0;33m"
#define TERM_FG_BLUE		"\x1b[0;34m"
#define TERM_FG_MAGENTA		"\x1b[0;35m"
#define TERM_FG_CYAN		"\x1b[0;36m"
#define TERM_FG_WHITE 		"\x1b[0;37m"
#define TERM_FG_RESET		"\x1b[0;39m"

#define TERM_FG_BLACK_BRIGHT	"\x1b[30;1m"
#define TERM_FG_RED_BRIGHT 		"\x1b[31;1m"
#define TERM_FG_GREEN_BRIGHT	"\x1b[32;1m"
#define TERM_FG_YELLOW_BRIGHT	"\x1b[33;1m"
#define TERM_FG_BLUE_BRIGHT		"\x1b[34;1m"
#define TERM_FG_MAGENTA_BRIGHT	"\x1b[35;1m"
#define TERM_FG_CYAN_BRIGHT		"\x1b[36;1m"
#define TERM_FG_WHITE_BRIGHT	"\x1b[37;1m"
