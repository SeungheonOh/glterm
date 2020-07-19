#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

enum PARSER_STATE {
  GROUND,
  ESCAPE,
  ESCAPE_INTERMEDIATE,
  CSI_ENTRY,
  CSI_PARAM,
  CSI_INTERMEDIATE,
  CSI_IGNORE,
  OSC_STRING,
  SOS_PM_APC_STRING,
  DCS_ENTRY,
  DCS_PARAM,
  DCS_INTERMEDIATE,
  DCS_IGNORE,
  DCS_PASSTHROUGH,
};

char* parser_state_name(enum PARSER_STATE s) {
  switch(s) {
    case GROUND:              return "ground";
    case ESCAPE:              return "escape";
    case ESCAPE_INTERMEDIATE: return "escape_intermediate";
    case CSI_ENTRY:           return "csi_entry";
    case CSI_PARAM:           return "csi_param";
    case CSI_INTERMEDIATE:    return "csi_intermediate";
    case CSI_IGNORE:          return "csi_ignore";
    case OSC_STRING:          return "osc_string";
    case SOS_PM_APC_STRING:   return "sos_pm_apc_string";
    case DCS_ENTRY:           return "dcs_entry";
    case DCS_PARAM:           return "dcs_param";
    case DCS_INTERMEDIATE:    return "dcs_intermediate";
    case DCS_IGNORE:          return "dcs_ignore";
    case DCS_PASSTHROUGH:     return "dcs_passthrough";
    default:                  return ""; // Impossible
  }
}

struct parser {
  enum PARSER_STATE state;
  char* buffer;
};

void parser_ignore(struct parser* p, uint8_t c) {
  // Ignore aka do nothing
}
void parser_print(struct parser* p, uint8_t c) {
  printf("print: %c\n", c);
}
void parser_execute(struct parser* p, uint8_t c) {
  printf("execute: %d\n", c);
}
void parser_clear(struct parser* p, uint8_t c) {
}
void parser_collect(struct parser* p, uint8_t c) {
}
void parser_param(struct parser* p, uint8_t c) {
}
void parser_esc_dispatch(struct parser* p, uint8_t c) {
}
void parser_csi_dispatch(struct parser* p, uint8_t c) {
  printf("csi_dispatch, endchar: %c\n", c);
}
void parser_hook(struct parser* p, uint8_t c) {
}
void parser_put(struct parser* p, uint8_t c) {
}
void parser_unhook(struct parser* p, uint8_t c) {
}
void parser_osc_start(struct parser* p, uint8_t c) {
}
void parser_osc_put(struct parser* p, uint8_t c) {
}
void parser_osc_end(struct parser* p, uint8_t c) {
}

bool handle_anywhere(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x80 ... 0x8f:
    case 0x91 ... 0x97:
    case 0x99:
    case 0x9a:
    case 0x18:
    case 0x1a:
      parser_execute(p, c);
      p->state = GROUND;
      break;

    case 0x9c:
      break;

    case 0x1b:
      parser_clear(p, c);
      p->state = ESCAPE;
      break;

    case 0x98:
    case 0x9e:
    case 0x9f:
      p->state = SOS_PM_APC_STRING;
      break;

    case 0x90:
      parser_clear(p, c);
      p->state = DCS_ENTRY;
      break;

    case 0x9b:
      parser_clear(p, c);
      p->state = CSI_ENTRY;
      break;

    case 0x9d:
      parser_osc_start(p, c);
      p->state = OSC_STRING;
      break;

    default:
      return false;
  }
}
void handle_ground(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
      parser_execute(p, c);
      break;
    case 0x20 ... 0x7f:
      parser_print(p, c);
      break;
    default: break;
  }
}
void handle_escape(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
      parser_execute(p, c);
      break;
    case 0x7f:
      parser_ignore(p, c);
      break;

    case 0x30 ... 0x4f:
    case 0x51 ... 0x57:
    case 0x60 ... 0x7e:
    case 0x5a:
    case 0x59:
    case 0x5c:
      parser_esc_dispatch(p, c);
      p->state = GROUND;
      break;

      //

    case 0x20 ... 0x2f:
      parser_collect(p, c);
      p->state = ESCAPE_INTERMEDIATE;
      break;

    case 0x50: 
      p->state = DCS_ENTRY; 
      break;
    case 0x5d: 
      p->state = OSC_STRING; 
      break;
    case 0x5b: 
      p->state = CSI_ENTRY; 
      break;
    case 0x58:
    case 0x5e:
    case 0x5f:
      p->state = SOS_PM_APC_STRING;
      break;
    default: break;
  }
}
void handle_escape_intermediate(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
      parser_execute(p, c);
      break;
    
    case 0x20 ... 0x2f:
      parser_collect(p, c);
      break;

    case 0x7f:
      parser_ignore(p, c);
      break;

      //

    case 0x30 ... 0x7e:
      parser_esc_dispatch(p, c);
      p->state = GROUND;
      break;
  }
}
void handle_csi_entry(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
      parser_execute(p, c);
      break;

    case 0x7f:
      parser_ignore(p, c);
      break;

     //

    case 0x40 ... 0x7e:
      parser_csi_dispatch(p, c);
      p->state = GROUND;
      break;

    case 0x20 ... 0x2f:
      parser_collect(p, c);
      break;

    case 0x3a:
      p->state = CSI_IGNORE;
      break;

    case 0x3c ... 0x3f:
      parser_collect(p, c);
      p->state = CSI_PARAM;
      break;

    case 0x30 ... 0x39:
    case 0x3b:
      parser_param(p, c);
      p->state = CSI_PARAM;
      break;
  }
}
void handle_csi_param(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
      parser_execute(p, c);
      break;

    case 0x30 ... 0x39:
    case 0x3b:
      parser_param(p, c);
      break;

    case 0x7f:
      parser_ignore(p, c);
      break;

      //

    case 0x3c ... 0x3f:
    case 0x3a:
      p->state = CSI_IGNORE;
      break;

    case 0x20 ... 0x2f:
      parser_collect(p, c);
      p->state = CSI_INTERMEDIATE;
      break;

    case 0x40 ... 0x7e:
      parser_csi_dispatch(p, c);
      p->state = GROUND;
      break;
  }
}
void handle_csi_intermediate(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
      parser_execute(p, c);
      break;

    case 0x20 ... 0x2f:
      parser_collect(p, c);
      break;

    case 0x7f:
      parser_ignore(p, c);
      break;

      //

    case 0x30 ... 0x3f:
      p->state = CSI_IGNORE;
      break;

    case 0x40 ... 0x7e:
      parser_csi_dispatch(p, c);
      p->state = GROUND;
      break;
  }
}
void handle_csi_ignore(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
      parser_execute(p, c);
      break;

    case 0x20 ... 0x3f:
    case 0x7f:
      parser_ignore(p, c);
      break;

    case 0x40 ... 0x7e:
      p->state = GROUND;
      break;
  }
}
void handle_osc_string(struct parser* p, uint8_t c) {
  //TODO
  parser_osc_start(p, c);
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
      parser_ignore(p, c);
      break;

    case 0x20 ... 0x7f:
      parser_osc_put(p, c);
      break;

      //

    case 0x9c:
      parser_osc_end(p, c);
      p->state = GROUND;
      break;
  }
}
void handle_sos_pm_apc_string(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x20 ... 0x7f:
    case 0x19:
      parser_ignore(p, c);
      break;

    case 0x9c:
      p->state = GROUND;
      break;
  }
}
void handle_dcs_entry(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
    case 0x7f:
      parser_ignore(p, c);
      break;

      //

    case 0x20 ... 0x2f:
      parser_collect(p, c);
      p->state = DCS_INTERMEDIATE;
      break;

    case 0x3a:
      p->state = DCS_IGNORE;
      break;

    case 0x30 ... 0x39:
    case 0x3b:
      parser_param(p, c);
      p->state = DCS_PARAM;
      break;

    case 0x3c ... 0x3f:
      parser_collect(p, c);
      p->state = DCS_PARAM;
      break;

    case 0x40 ... 0x7e:
      parser_hook(p, c);
      p->state = DCS_PASSTHROUGH;
      break;
  }
}
void handle_dcs_param(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
    case 0x7f:
      parser_ignore(p, c);
      break;

    case 0x30 ... 0x39:
      parser_param(p, c);
      break;

      //

    case 0x3c ... 0x3f:
    case 0x3a:
      p->state = DCS_IGNORE;
      break;

    case 0x20 ... 0x2f:
      parser_collect(p, c);
      p->state = DCS_INTERMEDIATE;
      break;

    case 0x40 ... 0x7e:
      parser_hook(p, c);
      p->state = DCS_PASSTHROUGH;
      break;
  }
}
void handle_dcs_intermediate(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x19:
    case 0x7f:
      parser_ignore(p, c);
      break;

    case 0x20 ... 0x2f:
      parser_collect(p, c);
      break;

      //

    case 0x30 ... 0x3f:
      p->state = DCS_IGNORE;
      break;

    case 0x40 ... 0x7e:
      parser_hook(p, c);
      p->state = DCS_PASSTHROUGH;
      break;
  }
}
void handle_dcs_ignore(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x20 ... 0x7f:
    case 0x19:
      parser_ignore(p, c);
      break;

    case 0x9c:
      p->state = GROUND;
      break;
  }
}
void handle_dcs_passthrough(struct parser* p, uint8_t c) {
  switch(c) {
    case 0x00 ... 0x17:
    case 0x1c ... 0x1f:
    case 0x20 ... 0x7e:
    case 0x19:
      parser_put(p, c);
      break;

    case 0x7f:
      parser_ignore(p, c);
      break;

    case 0x9c:
      parser_unhook(p, c);
      p->state = GROUND;
      break;
  }
}

void parse(struct parser* p, uint8_t c) {
  if(handle_anywhere(p, c)) return;
  switch(p->state) {
    case GROUND: handle_ground(p, c); break;
    case ESCAPE: handle_escape(p, c); break;
    case ESCAPE_INTERMEDIATE: handle_escape_intermediate(p, c); break;
    case CSI_ENTRY: handle_csi_entry(p, c); break;
    case CSI_PARAM: handle_csi_param(p, c); break;
    case CSI_INTERMEDIATE: handle_csi_intermediate(p, c); break;
    case CSI_IGNORE: handle_csi_ignore(p, c); break;
    case OSC_STRING: handle_osc_string(p, c); break;
    case SOS_PM_APC_STRING: handle_sos_pm_apc_string(p, c); break;
    case DCS_ENTRY: handle_dcs_entry(p, c); break;
    case DCS_PARAM: handle_dcs_param(p, c); break;
    case DCS_INTERMEDIATE: handle_dcs_intermediate(p, c); break;
    case DCS_IGNORE: handle_dcs_ignore(p, c); break;
    case DCS_PASSTHROUGH: handle_dcs_passthrough(p, c); break;
    default: break; // Impossible
  }
}

void main() {
  struct parser p;
  p.state = GROUND;

  char str[] = "\033[\033[cc";

  for(int i = 0; i < strlen(str); i++) {
    char c = str[i];
    printf("state: %s\n", parser_state_name(p.state));
    parse(&p, c);
  }
}
