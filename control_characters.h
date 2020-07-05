/*
 * @ NUL:  Null filler, terminal should ignore this character
 * A SOH:  Start of Header
 * B STX:  Start of Text, implied end of header
 * C ETX:  End of Text, causes some terminal to respond with ACK or NAK
 * D EOT:  End of Transmission
 * E ENQ:  Enquiry, causes terminal to send ANSWER-BACK ID
 * F ACK:  Acknowledge, usually sent by terminal in response to ETX
 * G BEL:  Bell, triggers the bell, buzzer, or beeper on the terminal
 * H BS:   Backspace, can be used to define overstruck characters
 * I HT:   Horizontal Tabulation, move to next predetermined position
 * J LF:   Linefeed, move to same position on next line (see also NL)
 * K VT:   Vertical Tabulation, move to next predetermined line
 * L FF:   Form Feed, move to next form or page
 * M CR:   Carriage Return, move to first character of current line
 * N SO:   Shift Out, switch to G1 (other half of character set)
 * O SI:   Shift In, switch to G0 (normal half of character set)
 * P DLE:  Data Link Escape, interpret next control character specially
 * Q XON:  (DC1) Terminal is allowed to resume transmitting
 * R DC2:  Device Control 2, causes ASR-33 to activate paper-tape reader
 * S XOFF: (DC2) Terminal must pause and refrain from transmitting
 * T DC4:  Device Control 4, causes ASR-33 to deactivate paper-tape reader
 * U NAK:  Negative Acknowledge, used sometimes with ETX and ACK
 * V SYN:  Synchronous Idle, used to maintain timing in Sync communication
 * W ETB:  End of Transmission block
 * X CAN:  Cancel (makes VT100 abort current escape sequence if any)
 * Y EM:   End of Medium
 * Z SUB:  Substitute (VT100 uses this to display parity errors)
 * [ ESC:  Prefix to an ESCape sequence
 * \ FS:   File Separator
 * ] GS:   Group Separator
 * ^ RS:   Record Separator (sent by VT132 in block-transfer mode)
 * _ US:   Unit Separator
 */
void NUL(struct terminal* t) {
  log_debug("NUL control code");
}
void SOH(struct terminal* t) {
  log_debug("SOH control code");
}
void STX(struct terminal* t) {
  log_debug("STX control code");
}
void ETX(struct terminal* t) {
  log_debug("ETX control code");
}
void EOT(struct terminal* t) {
  log_debug("EOT control code");
}
void ENQ(struct terminal* t) {
  log_debug("ENQ control code");
}
void ACK(struct terminal* t) {
  log_debug("ACK control code");
}
void BEL(struct terminal* t) {
  log_debug("BEL control code");
}
void BS (struct terminal* t) {
  log_debug("BS  control code");
  t->cursor.x--;
  terminal_erase_cell(t, t->cursor.x, t->cursor.y);
}
void HT (struct terminal* t) {
  log_debug("HT  control code");
}
void LF (struct terminal* t) {
  log_debug("LF  control code");
  terminal_cursor_next_line(t);
}
void VT (struct terminal* t) {
  log_debug("VT  control code");
}
void FF (struct terminal* t) {
  log_debug("FF  control code");
}
void CR (struct terminal* t) {
  log_debug("CR  control code");
}
void SO (struct terminal* t) {
  log_debug("SO  control code");
}
void SI (struct terminal* t) {
  log_debug("SI  control code");
}
void DLE(struct terminal* t) {
  log_debug("DLE control code");
}
void XON(struct terminal* t) {
  log_debug("XON control code");
}
void DC2(struct terminal* t) {
  log_debug("DC2 control code");
}
void XOFF(struct terminal* t) {
  log_debug("XOFF control code");
}
void DC4(struct terminal* t) {
  log_debug("DC4 control code");
}
void NAK(struct terminal* t) {
  log_debug("NAK control code");
}
void SYN(struct terminal* t) {
  log_debug("SYN control code");
}
void ETB(struct terminal* t) {
  log_debug("ETB control code");
}
void CAN(struct terminal* t) {
  log_debug("CAN control code");
}
void EM (struct terminal* t) {
  log_debug("EM  control code");
}
void SUB(struct terminal* t) {
  log_debug("SUB control code");
}
void ESC(struct terminal* t) {
  log_debug("ESC control code");
  t->esc.state = STATE_ESC;
}
void FS (struct terminal* t) {
  log_debug("FS  control code");
}
void GS (struct terminal* t) {
  log_debug("GS  control code");
}
void RS (struct terminal* t) {
  log_debug("RS  control code");
}
void US (struct terminal* t) {
  log_debug("US  control code");
}

void (*handle_control_character(char c))(struct terminal* t) {
  switch(c) {
    case 0x00: return NUL; //NUL
    case 0x01: return SOH; //SOH
    case 0x02: return STX; //STX
    case 0x03: return ETX; //ETX
    case 0x04: return EOT; //EOT
    case 0x05: return ENQ; //ENQ
    case 0x06: return ACK; //ACK
    case 0x07: return BEL; //BEL:
    case 0x08: return BS;  //BS
    case 0x09: return HT;  //HT
    case 0x0A: return LF;  //LF
    case 0x0B: return VT;  //VT
    case 0x0C: return FF;  //FF
    case 0x0D: return CR;  //CR
    case 0x0E: return SO;  //SO
    case 0x0F: return SI;  //SI
    case 0x10: return DLE; //DLE
    case 0x11: return XON; //XON
    case 0x12: return DC2; //DC2
    case 0x13: return XOFF;//XOFF
    case 0x14: return DC4; //DC4
    case 0x15: return NAK; //NAK
    case 0x16: return SYN; //SYN
    case 0x17: return ETB; //ETB
    case 0x18: return CAN; //CAN
    case 0x19: return EM;  //EM 
    case 0x1A: return SUB; //SUB
    case 0x1B: return ESC; //ESC
    case 0x1C: return FS;  //FS
    case 0x1D: return GS;  //GS 
    case 0x1E: return RS;  //RS
    case 0x1F: return US;  //US
    default:   return NULL;
  }
}


