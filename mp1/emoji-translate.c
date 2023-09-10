#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emoji.h"
#include "emoji-translate.h"


void emoji_init(emoji_t *emoji) {
  emoji->emoji = calloc(5, 1);
  emoji->translation = calloc(100, 1);
}

void emoji_add_translation(emoji_t *emoji, const unsigned char *source, const unsigned char *translation) {
  memcpy(emoji->emoji, source, 4);
  memcpy(emoji->translation, translation, strlen(translation));
}

// Translates the emojis contained in the file `fileName`.
const unsigned char *emoji_translate_file_alloc(emoji_t *emoji, const char *fileName) {
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) {
    perror("File open");
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  size_t fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  // Count number of emojis
  int i = 0, num = 0;
  char *tmp = calloc(4, 1);
  while (i <= fsize-4) {
    fread(tmp, 4, 1, fp);
    if (!memcmp(tmp, emoji->emoji, 4)) {
      num++; i+=4;
      // fseek(fp, 0, SEEK_CUR);
    } else {
      i++;
      fseek(fp, -3, SEEK_CUR);
    }
  }
  // fprintf(stderr, "num: %d\n", num);

  // Translate emojis
  i = 0; int j = 0; // i is the index in file and j is the index in string
  unsigned char *string = calloc(fsize+1+num*(strlen(emoji->translation)), 1);
  fseek(fp, 0, SEEK_SET);
  while (i <= fsize-4) {
    fread(tmp, 4, 1, fp);
    if (!memcmp(tmp, emoji->emoji, 4)) {
      memcpy(string+j, emoji->translation, strlen(emoji->translation));
      i += 4; j += strlen(emoji->translation);
    } else {
      memcpy(string+j, tmp, 1);
      i++; j++;
      fseek(fp, -3, SEEK_CUR);
    }
  }

  if (i < fsize) {
    fread(tmp, fsize-i, 1, fp);
    memcpy(string+j, tmp, fsize-i);
  }

  fclose(fp);
  return string;
  // return NULL;
}

void emoji_destroy(emoji_t *emoji) {
  free(emoji->emoji);
  free(emoji->translation);
}
