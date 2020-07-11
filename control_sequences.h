/*
100 40 @   ICH - Insert CHaracter
101 41 A * CUU - CUrsor Up
102 42 B * CUD - CUrsor Down
103 43 C * CUF - CUrsor Forward
104 44 D * CUB - CUrsor Backward
105 45 E   CNL - Cursor to Next Line
106 46 F   CPL - Cursor to Previous Line
107 47 G   CHA - Cursor Horizontal position Absolute
110 48 H * CUP - CUrsor Position
111 49 I   CHT - Cursor Horizontal Tabulation
112 4A J * ED  - Erase in Display (cursor does not move)
113 4B K * EL  - Erase in Line (cursor does not move)
114 4C L * IL  - Insert Line, current line moves down (VT102 series)
115 4D M * DL  - Delete Line, lines below current move up (VT102 series)
116 4E N   EF  - Erase in Field (as bounded by protected fields)
117 4F O   EA  - Erase in qualified Area (defined by DAQ)
120 50 P * DCH - Delete Character, from current position to end of field
121 51 Q   SEM - Set Editing extent Mode (limits ICH and DCH)
122 52 R * CPR - Cursor Position Report (from terminal to host)
123 53 S   SU  - Scroll up, entire display is moved up, new lines at bottom
124 54 T   SD  - Scroll down, new lines inserted at top of screen
125 55 U   NP  - Next Page (if terminal has more than 1 page of memory)
126 56 V   PP  - Previous Page (if terminal remembers lines scrolled off top)
127 57 W   CTC - Cursor Tabulation Control
130 58 X   ECH - Erase CHaracter
131 59 Y   CVT - Cursor Vertical Tab
132 5A Z   CBT - Cursor Back Tab
133 5B [  Reserved for future standardization     left bracket
134 5C \  Reserved                                reverse slant
135 5D ]  Reserved                                right bracket
136 5E ^  Reserved                                circumflex
137 5F _  Reserved                                underscore
140 60 ` * HPA - Horizontal Position Absolute (depends on PUM)
141 61 a * HPR - Horizontal Position Relative (depends on PUM)
142 62 b   REP - REPeat previous displayable character
143 63 c * DA  - Device Attributes
144 64 d * VPA - Vertical Position Absolute (depends on PUM)
145 65 e * VPR - Vertical Position Relative (depends on PUM)
146 66 f * HVP - Horizontal and Vertical Position (depends on PUM)
147 67 g * TBC - Tabulation Clear
150 68 h * SM  - Set Mode (. means permanently set on VT100)
151 69 i * MC  - Media Copy (printer port on VT102)
152 6A j  Reserved for future standardization
153 6B k  Reserved for future standardization
154 6C l * RM  - Reset Mode (. means permanently reset on VT100)
155 6D m * SGR - Set Graphics Rendition (affects character attributes)
156 6E n * DSR - Device Status Report
157 6F o   DAQ - Define Area Qualification starting at current position
 */

void ICH(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * ICH - Insert CHaracter
   *  [10@ = Make room for 10 characters at current position
   */
  log_debug("ICH");
}
void CUU(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CUU - CUrsor Up
   *  [A = Move up one line, stop at top of screen, [9A = move up 9
   */
  log_debug("CUU");
  terminal_move_cursor(t, 0, (argc >0)?atoi(argv[0]):1);
}
void CUD(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /* CUD - CUrsor Down
   *  [B = Move down one line, stop at bottom of screen
   */
  log_debug("CUD");
  terminal_move_cursor(t, 0, -((argc >0)?atoi(argv[0]):1));
}
void CUF(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CUF - CUrsor Forward
   *  [C = Move forward one position, stop at right edge of screen
   */
  log_debug("CUF");
  terminal_move_cursor(t, (argc >0)?atoi(argv[0]):1, 0);
}
void CUB(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CUB - CUrsor Backward
   *  [D = Same as BackSpace, stop at left edge of screen
   */
  log_debug("CUB");
  terminal_move_cursor(t, -((argc >0)?atoi(argv[0]):1), 0);
}
void CNL(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CNL - Cursor to Next Line
   *  [5E = Move to first position of 5th line down
   */
  log_debug("CNL");
  if(argc < 1) return;
  t->cursor.x = 0;
  terminal_move_cursor(t, 0, atoi(argv[0]));
}
void CPL(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CPL - Cursor to Previous Line
   *  [5F = Move to first position of 5th line previous
   */
  log_debug("CPL");
  if(argc < 1) return;
  t->cursor.x = 0;
  terminal_move_cursor(t, 0, -atoi(argv[0]));
}
void CHA(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CHA - Cursor Horizontal position Absolute
   *  [40G = Move to column 40 of current line
   */
  log_debug("CHA");
  if(argc < 1) return;
  terminal_move_cursor(t, atoi(argv[0]), t->cursor.y);
}
void CUP(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CUP - CUrsor Position
   *  [H = Home, [24;80H = Row 24, Column 80
   */
  log_debug("CUP");
  if(argc == 0)
    terminal_set_cursor(t, 0, 0);
  else if(argc == 2)
    terminal_set_cursor(t, atoi(argv[1]), atoi(argv[0]));
}
void CHT(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * TODO
   * CHT - Cursor Horizontal Tabulation
   *  [I = Same as HT (Control-I), [3I = Go forward 3 tabs
   */
  log_debug("CHT");
}
void ED (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * ED  - Erase in Display (cursor does not move)
   *  [J = [0J = Erase from current position to end (inclusive)
   *  [1J = Erase from beginning to current position (inclusive)
   *  [2J = Erase entire display
   *  [?0J = Selective erase in display ([?1J, [?2J similar)
   *
   *  ignore ?s
   */
  log_debug("ED ");

  if(argc == 0 || atoi(argv[0]) == 0) {
    for(int i = t->cursor.x + t->cursor.y * t->cols; i < t->cols * t->rows; i++)
      t->screen[i].d = false;
  } else if(atoi(argv[0]) == 1) {
    for(int i = 0; i < t->cursor.x + t->cursor.y * t->cols; i++)
      t->screen[i].d = false;
  } else if(atoi(argv[0]) == 2) {
    terminal_clear(t);
  }
}
void EL (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * EL  - Erase in Line (cursor does not move)
   *  [K = [0K = Erase from current position to end (inclusive)
   *  [1K = Erase from beginning to current position
   *  [2K = Erase entire current line
   *  [?0K = Selective erase to end of line ([?1K, [?2K similar)
   */
  log_debug("EL ");
  if(argc == 0 || atoi(argv[0]) == 0) {
    for(int i = t->cursor.x; i < t->cols; i++)
      t->screen[i + t->cursor.y * t->cols].d = false;
  } else if(atoi(argv[0]) == 1) {
    for(int i = 0; i < t->cursor.x; i++)
      t->screen[i + t->cursor.y * t->cols].d = false;
  } else if(atoi(argv[0]) == 2) {
    for(int i = 0; i < t->cols; i++)
      t->screen[i + t->cursor.y * t->cols].d = false;
  }
}
void IL (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * IL  - Insert Line, current line moves down (VT102 series)
   *  [3L = Insert 3 lines if currently in scrolling region
   */
  log_debug("IL ");
  if(argc < 0) return;
  if(t->cursor.y + atoi(argv[0]) > t->rows) return;
  for(int i = 0; i < t->cursor.x; i++) {
    t->screen[i + (t->cursor.y + atoi(argv[0])) * t->cols] = t->screen[i + t->cursor.y * t->cols];
    t->screen[i + t->cursor.y * t->cols].d = false;
  }
}
void DL (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * DL  - Delete Line, lines below current move up (VT102 series)
   *  [2M = Delete 2 lines if currently in scrolling region
   */
  log_debug("DL ");
}
void EF (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * EF  - Erase in Field (as bounded by protected fields)
   *  [0N, [1N, [2N act like [L but within currend field
   */
  log_debug("EF ");
}
void EA (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * EA  - Erase in qualified Area (defined by DAQ)
   *  [0O, [1O, [2O act like [J but within current area
   *  Just use ED
   */
  log_debug("EA ");
  ED(t, dec, argc, argv, intermed);
}
void DCH(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * DCH - Delete Character, from current position to end of field
   *  [4P = Delete 4 characters, VT102 series
   *
   *  xterm and urxvt ignore it
   *  https://vt100.net/docs/vt510-rm/DCH.html
   */
  log_debug("DCH");
  int d = ((argc > 0)?atoi(argv[0]):1);
  for(int i = t->cursor.x; i < t->cols; i++) {
    if(i + d < t->cols)t->screen[i + t->cursor.y * t->cols] = t->screen[i + d + t->cursor.y * t->cols];
    else t->screen[(i + t->cursor.x) + t->cursor.y * t->cols].d = false;
  }
}
void SEM(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * SEM - Set Editing extent Mode (limits ICH and DCH)
   *  [0Q = [Q = Insert/delete character affects rest of display
   *  [1Q = ICH/DCH affect the current line only
   *  [2Q = ICH/DCH affect current field (between tab stops) only
   *  [3Q = ICH/DCH affect qualified area (between protected fields)
   */
  log_debug("SEM");
}
void CPR(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CPR - Cursor Position Report (from terminal to host)
   *  [24;80R = Cursor is positioned at line 24 column 80
   */
  log_debug("CPR");
}
void SU (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * SU  - Scroll up, entire display is moved up, new lines at bottom
   *  [3S = Move everything up 3 lines, bring in 3 new lines
   */
  log_debug("SU ");
}
void SD (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * SD  - Scroll down, new lines inserted at top of screen
   *  [4T = Scroll down 4, bring previous lines back into view
   */
  log_debug("SD ");
}
void NP (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * NP  - Next Page (if terminal has more than 1 page of memory)
   *  [2U = Scroll forward 2 pages
   */
  log_debug("NP ");
}
void PP (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * PP  - Previous Page (if terminal remembers lines scrolled off top)
   *  [1V = Scroll backward 1 page
   */
  log_debug("PP ");
}
void CTC(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CTC - Cursor Tabulation Control
   *  [0W = Set horizontal tab for current line at current position
   *  [1W = Set vertical tab stop for current line of current page
   *  [2W = Clear horiz tab stop at current position of current line
   *  [3W = Clear vert tab stop at current line of current page
   *  [4W = Clear all horiz tab stops on current line only
   *  [5W = Clear all horiz tab stops for the entire terminal
   *  [6W = Clear all vert tabs stops for the entire terminal
   */
  log_debug("CTC");
}
void ECH(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * ECH - Erase CHaracter
   *  [4X = Change next 4 characters to "erased" state
   */
  log_debug("ECH");
}
void CVT(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CVT - Cursor Vertical Tab
   *  [2Y = Move forward to 2nd following vertical tab stop
   */
  log_debug("CVT");
}
void CBT(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * CBT - Cursor Back Tab
   *  [3Z = Move backwards to 3rd previous horizontal tab stop
   */
  log_debug("CBT");
}
void HPA(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * HPA - Horizontal Position Absolute (depends on PUM)
   *  [720` = Move to 720 decipoints (1 inch) from left margin
   *  [80` = Move to column 80 on LA120
   */
  log_debug("HPA");
}
void HPR(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * HPR - Horizontal Position Relative (depends on PUM)
   *  [360a = Move 360 decipoints (1/2 inch) from current position
   *  [40a = Move 40 columns to right of current position on LA120
   */
  log_debug("HPR");
}
void REP(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * REP - REPeat previous displayable character
   *  [80b = Repeat character 80 times
   */
  log_debug("REP");
}
void DA (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * DA  - Device Attributes
   *  [c = Terminal will identify itself
   *  [?1;2c = Terminal is saying it is a VT100 with AVO
   *  [>0c = Secondary DA request (distinguishes VT240 from VT220)
   */
  log_debug("DA ");
}
void VPA(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * VPA - Vertical Position Absolute (depends on PUM)
   *  [90d = Move to 90 decipoints (1/8 inch) from top margin
   *  [10d = Move to line 10 if before that else line 10 next page
   */
  log_debug("VPA");
}
void VPR(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * VPR - Vertical Position Relative (depends on PUM)
   *  [720e = Move 720 decipoints (1 inch) down from current position
   *  [6e = Advance 6 lines forward on LA120
   */
  log_debug("VPR");
}
void HVP(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * HVP - Horizontal and Vertical Position (depends on PUM)
   *  [720,1440f = Move to 1 inch down and 2 inches over (decipoints)
   *  [24;80f = Move to row 24 column 80 if PUM is set to character
   */
  log_debug("HVP");
}
void TBC(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * TBC - Tabulation Clear
   *  [0g = Clear horizontal tab stop at current position
   *  [1g = Clear vertical tab stop at current line (LA120)
   *  [2g = Clear all horizontal tab stops on current line only LA120
   *  [3g = Clear all horizontal tab stops in the terminal
   */
  log_debug("TBC");
}
void SM (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * SM  - Set Mode (. means permanently set on VT100)
   *  [0h = Error, this command is ignored
   *  [1h = GATM - Guarded Area Transmit Mode, send all (VT132)
   *  [2h = KAM - Keyboard Action Mode, disable keyboard input
   *  [3h = CRM - Control Representation Mode, show all control chars
   *  [4h = IRM - Insertion/Replacement Mode, set insert mode (VT102)
   *  [5h = SRTM - Status Report Transfer Mode, report after DCS
   *  [6h = ERM - ERasure Mode, erase protected and unprotected
   *  [7h = VEM - Vertical Editing Mode, IL/DL affect previous lines
   *  [8h, [9h are reserved
   *  [10h = HEM - Horizontal Editing mode, ICH/DCH/IRM go backwards
   *  [11h = PUM - Positioning Unit Mode, use decipoints for HVP/etc
   *  [12h = SRM - Send Receive Mode, transmit without local echo
   *  [13h = FEAM - Format Effector Action Mode, FE's are stored
   *  [14h = FETM - Format Effector Transfer Mode, send only if stored
   *  [15h = MATM - Multiple Area Transfer Mode, send all areas
   *  [16h = TTM - Transmit Termination Mode, send scrolling region
   *  [17h = SATM - Send Area Transmit Mode, send entire buffer
   *  [18h = TSM - Tabulation Stop Mode, lines are independent
   *  [19h = EBM - Editing Boundry Mode, all of memory affected
   *  [20h = LNM - Linefeed Newline Mode, LF interpreted as CR LF
   *  [?1h = DECCKM - Cursor Keys Mode, send ESC O A for cursor up
   *  [?2h = DECANM - ANSI Mode, use ESC < to switch VT52 to ANSI
   *  [?3h = DECCOLM - COLumn mode, 132 characters per line
   *  [?4h = DECSCLM - SCrolL Mode, smooth scrolling
   *  [?5h = DECSCNM - SCreeN Mode, black on white background
   *  [?6h = DECOM - Origin Mode, line 1 is relative to scroll region
   *  [?7h = DECAWM - AutoWrap Mode, start newline after column 80
   *  [?8h = DECARM - Auto Repeat Mode, key will autorepeat
   *  [?9h = DECINLM - INterLace Mode, interlaced for taking photos
   *  [?10h = DECEDM - EDit Mode, VT132 is in EDIT mode
   *  [?11h = DECLTM - Line Transmit Mode, ignore TTM, send line
   */
  log_debug("SM ");
}
void MC (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * MC  - Media Copy (printer port on VT102)
   *  [0i = Send contents of text screen to printer
   *  [1i = Fill screen from auxiliary input (printer's keyboard)
   *  [2i = Send screen to secondary output device
   *  [3i = Fill screen from secondary input device
   *  [4i = Turn on copying received data to primary output (VT125)
   *  [4i = Received data goes to VT102 screen, not to its printer
   *  [5i = Turn off copying received data to primary output (VT125)
   *  [5i = Received data goes to VT102's printer, not its screen
   *  [6i = Turn off copying received data to secondary output (VT125)
   *  [7i = Turn on copying received data to secondary output (VT125)
   *  [?0i = Graphics screen dump goes to graphics printer VT125,VT240
   *  [?1i = Print cursor line, terminated by CR LF
   *  [?2i = Graphics screen dump goes to host computer VT125,VT240
   *  [?4i = Disable auto print
   *  [?5i = Auto print, send a line at a time when linefeed received
   */
  log_debug("MC ");
}
void RM (struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * RM  - Reset Mode (. means permanently reset on VT100)
   *  [1l = GATM - Transmit only unprotected characters (VT132)
   *  [2l = KAM - Enable input from keyboard
   *  [3l = CRM - Control characters are not displayable characters
   *  [4l = IRM - Reset to replacement mode (VT102)
   *  [5l = SRTM - Report only on command (DSR)
   *  [6l = ERM - Erase only unprotected fields
   *  [7l = VEM - IL/DL affect lines after current line
   *  [8l, [9l are reserved
   *  [10l = HEM - ICH and IRM shove characters forward, DCH pulls
   *  [11l = PUM - Use character positions for HPA/HPR/VPA/VPR/HVP
   *  [12l = SRM - Local echo - input from keyboard sent to screen
   *  [13l = FEAM - HPA/VPA/SGR/etc are acted upon when received
   *  [14l = FETM - Format Effectors are sent to the printer
   *  [15l = MATM - Send only current area if SATM is reset
   *  [16l = TTM - Transmit partial page, up to cursor position
   *  [17l = SATM - Transmit areas bounded by SSA/ESA/DAQ
   *  [18l = TSM - Setting a tab stop on one line affects all lines
   *  [19l = EBM - Insert does not overflow to next page
   *  [20l = LNM - Linefeed does not change horizontal position
   *  [?1l = DECCKM - Cursor keys send ANSI cursor position commands
   *  [?2l = DECANM - Use VT52 emulation instead of ANSI mode
   *  [?3l = DECCOLM - 80 characters per line (erases screen)
   *  [?4l = DECSCLM - Jump scrolling
   *  [?5l = DECSCNM - Normal screen (white on black background)
   *  [?6l = DECOM - Line numbers are independent of scrolling region
   *  [?7l = DECAWM - Cursor remains at end of line after column 80
   *  [?8l = DECARM - Keys do not repeat when held down
   *  [?9l = DECINLM - Display is not interlaced to avoid flicker
   *  [?10l = DECEDM - VT132 transmits all key presses
   *  [?11l = DECLTM - Send page or partial page depending on TTM
   *  [?12l = ?
   *  [?13l = DECSCFDM - Don't suppress trailing spaces on transmit
   *  [?14l = DECTEM - ENTER sends ESC S (STS) a request to send
   *  [?15l = ?
   *  [?16l = DECEKEM - EDIT key transmits either $[10h or $[10l
   *  [?17l = ?
   *  [?18l = DECPFF - Don't send a formfeed after printing screen
   *  [?19l = DECPEXT - Print only the lines within the scroll region
   *  [?20l = OV0 - Space is destructive, replace not overstrike, GIGI
   *  [?21l = BA0 - No BASIC, GIGI is On-Line or Local
   *  [?22l = BA0 - No BASIC, GIGI is On-Line or Local
   *  [?23l = PK0 - Ignore reprogramming on GIGI keypad and cursors
   *  [?24l = AH0 - No auto-hardcopy when GIGI screen erased
   *  [?29l = Allow all character pitches on the LA100
   *  [?38l = DECTEK - Ignore TEKtronix graphics commands
   */
  log_debug("RM ");
}
void SGR(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * SGR - Set Graphics Rendition (affects character attributes)
   *  [0m = Clear all special attributes
   *  [1m = Bold or increased intensity
   *  [2m = Dim or secondary color on GIGI  (superscript on XXXXXX)
   *  [3m = Italic                          (subscript on XXXXXX)
   *  [4m = Underscore, [0;4m = Clear, then set underline only
   *  [5m = Slow blink
   *  [6m = Fast blink                      (overscore on XXXXXX)
   *  [7m = Negative image, [0;1;7m = Bold + Inverse
   *  [8m = Concealed (do not display character echoed locally)
   *  [9m = Reserved for future standardization
   *  [10m = Select primary font (LA100)
   *  [11m - [19m = Selete alternate font (LA100 has 11 thru 14)
   *  [20m = FRAKTUR (whatever that means)
   *  [22m = Cancel bold or dim attribute only (VT220)
   *  [24m = Cancel underline attribute only (VT220)
   *  [25m = Cancel fast or slow blink attribute only (VT220)
   *  [27m = Cancel negative image attribute only (VT220)
   *  [30m = Write with black,   [40m = Set background to black (GIGI)
   *  [31m = Write with red,     [41m = Set background to red
   *  [32m = Write with green,   [42m = Set background to green
   *  [33m = Write with yellow,  [43m = Set background to yellow
   *  [34m = Write with blue,    [44m = Set background to blue
   *  [35m = Write with magenta, [45m = Set background to magenta
   *  [36m = Write with cyan,    [46m = Set background to cyan
   *  [37m = Write with white,   [47m = Set background to white
   *  [38m, [39m, [48m, [49m are reserved
   */
  log_debug("SGR");
}
void DSR(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * DSR - Device Status Report
   *  0n = Terminal is ready, no malfunctions detected
   *  1n = Terminal is busy, retry later
   *  2n = Terminal is busy, it will send DSR when ready
   *  3n = Malfunction, please try again
   *  4n = Malfunction, terminal will send DSR when ready
   *  5n = Command to terminal to report its status
   *  6n = Command to terminal requesting cursor position (CPR)
   *  ?15n = Command to terminal requesting printer status, returns
   *  ?10n = OK, [?11n = not OK, [?13n = no printer.
   *  ?25n = "Are User Defined Keys Locked?" (VT220)
   */
  log_debug("DSR");
}
void DAQ(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  /*
   * DAQ - Define Area Qualification starting at current position
   *  [0o = Accept all input, transmit on request
   *  [1o = Protected and guarded, accept no input, do not transmit
   *  [2o = Accept any printing character in this field
   *  [3o = Numeric only field
   *  [4o = Alphabetic (A-Z and a-z) only
   *  [5o = Right justify in area
   *  [3;6o = Zero fill in area
   *  [7o = Set horizontal tab stop, this is the start of the field
   *  [8o = Protected and unguarded, accept no input, do transmit
   *  [9o = Space fill in area
   */
  log_debug("DAQ");
}

void test(struct terminal* t, bool dec, int argc, char** argv, char* intermed) {
  log_debug("dec: %d", dec);
  log_debug("argc: %d", argc);
  for(int i = 0; i < argc; i++) log_debug("arg%d: %s", i+1, argv[i]);
  log_debug("intermediate: %s", intermed);
}

void (*handle_control_sequence(char c))(struct terminal*, bool, int, char**, char*) {
  switch(c) {
    case '@':  return ICH;
    case 'A':  return CUU;
    case 'B':  return CUD;
    case 'C':  return CUF;
    case 'D':  return CUB;
    case 'E':  return CNL;
    case 'F':  return CPL;
    case 'G':  return CHA;
    case 'H':  return CUP;
    case 'I':  return CHT;
    case 'J':  return ED;
    case 'K':  return EL;
    case 'L':  return IL;
    case 'M':  return DL;
    case 'N':  return EF;
    case 'O':  return EA;
    case 'P':  return DCH;
    case 'Q':  return SEM;
    case 'R':  return CPR;
    case 'S':  return SU;
    case 'T':  return SD;
    case 'U':  return NP;
    case 'V':  return PP;
    case 'W':  return CTC;
    case 'X':  return ECH;
    case 'Y':  return CVT;
    case 'Z':  return CBT;
    case '[':  return NULL;
    case '\\': return NULL;
    case ']':  return NULL;
    case '^':  return NULL;
    case '_':  return NULL;
    case '`':  return HPA;
    case 'a':  return HPR;
    case 'b':  return REP;
    case 'c':  return DA;
    case 'd':  return VPA;
    case 'e':  return VPR;
    case 'f':  return HVP;
    case 'g':  return TBC;
    case 'h':  return SM;
    case 'i':  return MC;
    case 'j':  return NULL;
    case 'k':  return NULL;
    case 'l':  return RM;
    case 'm':  return SGR;
    case 'n':  return DSR;
    case 'o':  return DAQ;
    default:   return NULL;
  }
}
