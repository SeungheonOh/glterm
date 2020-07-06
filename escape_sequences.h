/*
 * Note: most of sequences here is not being used by modern terminal emulation.
 * These implementation, therefore, is not complete
 *
 * @	Reserved for future standardization
 * A	Reserved
 * B	Reserved
 * C	Reserved
 * D IND *	Index, moves down one line same column regardless of NL
 * E NEL *	NEw Line, moves done one line and to first column (CR+LF)
 * F SSA	Start of Selected Area to be sent to auxiliary output device
 * G ESA	End of Selected Area to be sent to auxiliary output device
 * H HTS *	Horizontal Tabulation Set at current position
 * I HTJ	Hor Tab Justify, moves string to next tab position
 * J VTS	Vertical Tabulation Set at current line
 * K PLD	Partial Line Down (subscript)
 * L PLU	Partial Line Up (superscript)
 * M RI  *	Reverse Index, go up one line, reverse scroll if necessary
 * N SS2 *	Single Shift to G2
 * O SS3 *	Single Shift to G3 (VT100 uses this for sending PF keys)
 * P DCS *	Device Control String, terminated by ST (VT125 enters graphics)
 * Q PU1	Private Use 1
 * R PU2	Private Use 2
 * S STS	Set Transmit State
 * T CCH	Cancel CHaracter, ignore previous character
 * U MW	Message Waiting, turns on an indicator on the terminal
 * V SPA	Start of Protected Area
 * W EPA	End of Protected Area
 * X	Reserved for for future standard
 * Y	Reserved
 * Z     *	Reserved, but causes DEC terminals to respond with DA codes
 * [ CSI *	Control Sequence Introducer (described in a seperate table)
 * \ ST  *	String Terminator (VT125 exits graphics)
 * ] OSC	Operating System Command (reprograms intelligent terminal)
 * ^ PM	Privacy Message (password verification), terminated by ST
 * _ APC	Application Program Command (to word processor), term by ST
 */

void IND(struct terminal* t) {
}
void NEL(struct terminal* t) {
}
void SSA(struct terminal* t) {
}
void ESA(struct terminal* t) {
}
void HTS(struct terminal* t) {
}
void HTJ(struct terminal* t) {
}
void VTS(struct terminal* t) {
}
void PLD(struct terminal* t) {
}
void PLU(struct terminal* t) {
}
void RI (struct terminal* t) {
}
void SS2(struct terminal* t) {
}
void SS3(struct terminal* t) {
}
void DCS(struct terminal* t) {
  t->esc.state = STATE_DCS;
}
void PU1(struct terminal* t) {
}
void PU2(struct terminal* t) {
}
void STS(struct terminal* t) {
}
void CCH(struct terminal* t) {
}
void MW	(struct terminal* t) {
}
void SPA(struct terminal* t) {
}
void EPA(struct terminal* t) {
}
void ESC_DA (struct terminal* t) {
}
void CSI(struct terminal* t) {
  t->esc.state = STATE_CSI;
}
void ST (struct terminal* t) {
}
void OSC(struct terminal* t) {
  t->esc.state = STATE_OSC;
}
void PM	(struct terminal* t) {
}
void APC(struct terminal* t) {
}

void (* handle_escape_sequence(char c))(struct terminal*) {
  switch(c) {
    case '@':  return NULL; //Reserved
    case 'A':  return NULL; //Reserved
    case 'B':  return NULL; //Reserved
    case 'C':  return NULL; //Reserved
    case 'D':  return IND; //IND
    case 'E':  return NEL; //NEL
    case 'F':  return SSA; //SSA
    case 'G':  return ESA; //ESA
    case 'H':  return HTS; //HTS
    case 'I':  return HTJ; //HTJ
    case 'J':  return VTS; //VTS
    case 'K':  return PLD; //PLD
    case 'L':  return PLU; //PLU
    case 'M':  return RI;  //RI
    case 'N':  return SS2; //SS2
    case 'O':  return SS3; //SS3
    case 'P':  return DCS; //DCS
    case 'Q':  return PU1; //PU1
    case 'R':  return PU2; //PU2
    case 'S':  return STS; //STS
    case 'T':  return CCH; //CCH
    case 'U':  return MW;  //MW
    case 'V':  return SPA; //SPA
    case 'W':  return EPA; //EPA
    case 'X':  return NULL;//Reserved
    case 'Y':  return NULL;//Reserved
    case 'Z':  return ESC_DA;  //DA
    case '[':  return CSI; //CSI
    case '\\': return ST;  //ST
    case ']':  return OSC; //OSC
    case '^':  return PM;  //PM
    case '_':  return APC; //APC
    default:   return NULL;
  }
}
