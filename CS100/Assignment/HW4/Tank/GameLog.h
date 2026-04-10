#include <stdarg.h>
#include <stdio.h>
#include <time.h>

// define the log file name
#define LOG_FILE "game.log"

/// \brief Initializes the log file at the start of the game.
void InitGameLog(void) {
  FILE *file = fopen(LOG_FILE, "w");
  if (file != NULL) {
    fclose(file);
  } else {
    printf("Unable to create log file!\n");
  }
}

/// \brief Logs a formatted message to the log file with a timestamp.
void GameLog(const char *format, ...) {
  // Open the log file in append mode
  FILE *file = fopen(LOG_FILE, "a");
  if (file == NULL) {
    printf("Unable to open log file!\n");
    return;
  }

  // Get the current time and format it as a string
  time_t now;
  time(&now);
  struct tm *timeinfo = localtime(&now);
  char timeStr[30];
  strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);

  // Write the timestamp to the log file
  fprintf(file, "[%s] ", timeStr);

  // Process the variable arguments and write the formatted message to the log file
  va_list args;
  va_start(args, format);
  vfprintf(file, format, args);
  va_end(args);

  // Add a newline after the log message and close the file
  fprintf(file, "\n");
  fclose(file);
}
