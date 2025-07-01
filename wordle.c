#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CGREEN "\033[32m"
#define CYELLOW "\033[33m"
#define CGRAY "\033[90m"
#define CRESET "\033[0m"

char wordle_answers[][6] = {
  "about", "after", "again", "being", "board", "black", "books", "could", "group", "great",
  "hotel", "house", "image", "items", "learn", "local", "music", "never", "other", "phone",
  "place", "press", "price", "right", "small", "state", "store", "their", "these", "those",
  "three", "total", "under", "video", "where", "which", "while", "world", "years", "found",
  "might", "still", "every", "power", "since", "guide", "white", "times", "sites", "level",
  "hours", "title", "shall", "class", "money", "visit", "tools", "reply", "value", "stock",
  "point", "sales", "large", "table", "start", "model", "human", "movie", "march", "yahoo",
  "going", "study", "staff", "april", "users", "topic", "below", "early", "above", "basis",
  "clear", "check", "event", "field", "green", "index", "input", "issue", "japan", "light",
  "major", "maker", "march", "maybe", "north", "order", "parts", "quick", "rates", "reach"
};

void w_getchar(int fd) {
  char c;
  read(fd, &c, 1);
}

int w_format(char *res, const char *prefix, const char *text, const char *suffix) {
  int prefix_len = strlen(prefix);
  int suffix_len = strlen(suffix);
  int text_len = strlen(text);

  memcpy(res, prefix, prefix_len);
  res += prefix_len;

  memcpy(res, text, text_len);
  res += text_len;

  memcpy(res, suffix, suffix_len);
  res += suffix_len;

  *res = '\0'; 

  return prefix_len + text_len + suffix_len;
}

int main() {
  srand(time(NULL));

  int count = sizeof(wordle_answers) / sizeof(wordle_answers[0]);
  char *rand_word = wordle_answers[rand() % count];

  write(STDOUT_FILENO, "CWORDLE:\n", 9);

  while (1) {
    char guess[6] = {0};
    ssize_t n = read(STDIN_FILENO, guess, sizeof(guess) - 1);
    if (n == -1) {
      write(STDOUT_FILENO, "INTERNAL GAME ERROR\n", 20);
      return 0;
    }
    
    if (n > 0 && guess[n - 1] == '\n') {
      guess[n - 1] = '\0';
    } 
    else {
      guess[n] = '\0';
      char c;
      while (read(STDIN_FILENO, &c, 1) > 0 && c != '\n');
    }

    int rand_len = strlen(rand_word);
    if (strlen(guess) != rand_len) {
      write(STDOUT_FILENO, "retry bud\n", 10);
      continue;
    }

    char line_buf[80];
    int offset = 0;

    for (int i = 0; i < rand_len; i++) {
      int len = 0;

      if (guess[i] == rand_word[i]) {
        len = w_format(line_buf + offset, CGREEN, "#", CRESET);
      }
      else if (strchr(rand_word, guess[i]) != NULL) {
        len = w_format(line_buf + offset, CYELLOW, "#", CRESET);
      }
      else len = w_format(line_buf + offset, CGRAY, "#", CRESET);
  
      offset += len;
    }

    line_buf[offset++] = '\n';
    write(STDOUT_FILENO, line_buf, offset);

    if (memcmp(guess, rand_word, rand_len) == 0) {
      write(STDOUT_FILENO, "you guessed it!", 15);
      
      w_getchar(STDIN_FILENO);

      break;
    }
  }

  return 0;
}
