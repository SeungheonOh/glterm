#include <fcntl.h>
#include <unistd.h>
#include <pty.h>
#include <pwd.h>
#include <termios.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void sigchld(int a) {
	exit(0);
}

bool run_shell(char* fallback_shell) {
  char* shell;
  const struct passwd *pw;

  if((pw = getpwuid(getuid())) == NULL) {
    // TODO
    log_error("%s: error", __func__);
  }

  // try to look up $SHELL, if failed use default login shell
  if((shell = getenv("SHELL")) == NULL) 
    shell = (pw->pw_shell[0]) ? pw->pw_shell : fallback_shell;

  unsetenv("COLUMNS");
	unsetenv("LINES");
	unsetenv("TERMCAP");
	setenv("LOGNAME", pw->pw_name, 1);
	setenv("USER", pw->pw_name, 1);
	setenv("SHELL", shell, 1);
	setenv("HOME", pw->pw_dir, 1);
	setenv("TERM", "xterm", 1);

  execvp(shell, (char*[]){NULL});
}

bool init_pseudo(struct terminal* term, unsigned int cols, unsigned int rows) {
  int m, pid;
  switch(pid = forkpty(&m, NULL, NULL, &(struct winsize) {.ws_row = rows, .ws_col = cols})) {
  case -1:
    log_error("%s: Failed to fork terminal %s", __func__, strerror(errno));
    return false;
  case 0:
    //setsid();
    run_shell("/bin/bash");
    log_error("THIS SHOULD NEVER BE PRINTED");
    break;
  default:
    term->pid = pid;
    term->fd_master = m;
    signal(SIGCHLD, sigchld);
    break;
  }

  // Set nonblocking io mode
  fcntl(term->fd_master, F_SETFL, fcntl(term->fd_master, F_GETFL, 0) | O_NONBLOCK);
  return true;
}

bool kill_pseudo(struct terminal* term) {
  kill(term->pid, SIGTERM);
  close(term->fd_master);
  waitpid(term->pid, NULL, 0);
}

int pseudo_listen(struct terminal* term, char* buf, size_t bufsize) {
  int n;
  if((n = read(term->fd_master, buf, bufsize)) < 0 && errno != 11) {
    // errno 11 is expected because of nonblocking io
    log_error("%s: failed to read terminal fd : %s", __func__, strerror(errno));
  }
  return n;
}

bool pseudo_write(struct terminal* term, char* buf, size_t bufsize) {
  if(write(term->fd_master, buf, bufsize) < 0) {
    log_error("%s: failed to write into terminal fd : %s", __func__, strerror(errno));
    return false;
  }
  return true;
}

bool pseudo_write_byte(struct terminal* term, char b) {
  if(write(term->fd_master, &b, 1) < 0) {
    log_error("%s: failed to write into terminal fd : %s", __func__, strerror(errno));
    return false;
  }
  return true;
}
