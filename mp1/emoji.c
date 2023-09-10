#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <time.h>


// Return your favorite emoji.  Do not allocate new memory.
// (This should **really** be your favorite emoji, we plan to use this later in the semester. :))
char *emoji_favorite() {
  return "😅";
  return "😆";
}


// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.  You should
// consider everything in the ranges starting from (and including) U+1F000 up to (and including) U+1FAFF.
int emoji_count(const unsigned char *utf8str) {
  int count = 0, i = 0;
  while (i <= strlen(utf8str)-4) {
    if (memcmp(utf8str+i, "\U0001F000", 4)>=0 && memcmp(utf8str+i, "\U0001FAFF", 4)<=0) {
      count++;
      i+=4;
    } else {
      i++;
    }
  }
  return count;
  // return 0;
}


// Return a random emoji stored in new heap memory you have allocated.  Make sure what
// you return is a valid C-string that contains only one random emoji.
char *emoji_random_alloc() {
  char *emoji = calloc(5, 1);
  char *emoji_string = calloc(9, 1);
  char *emoji_string_tmp = calloc(3, 1);
  int i; char * pEnd;

  FILE *fp = fopen("/dev/urandom", "r");
  if (fp == NULL) {
    perror("File open");
    return NULL;
  }
  size_t r;
  if (fread(&r, sizeof(r), 1, fp) != 1) {
    perror("fread");
    return NULL;
  }
  if (fclose(fp) == -1) {
    perror("fclose");
    return NULL;
  }

  // fprintf(stderr, "The random number is %x\n", r);
  r = r % (0xf09fabbf-0xf09f8080) + 0xf09f8080;
  // r = 0xf09faba4;
  // fprintf(stderr, "The emoji is %x\n", r);

  sprintf(emoji_string, "%x", r);
  memcpy(emoji_string_tmp, emoji_string, 2);
  i = strtol(emoji_string_tmp, &pEnd, 16);
  emoji[0] = i;
  memcpy(emoji_string_tmp, emoji_string+2, 2);
  i = strtol(emoji_string_tmp, &pEnd, 16);
  emoji[1] = i;
  memcpy(emoji_string_tmp, emoji_string+4, 2);
  i = strtol(emoji_string_tmp, &pEnd, 16);
  emoji[2] = i;
  memcpy(emoji_string_tmp, emoji_string+6, 2);
  i = strtol(emoji_string_tmp, &pEnd, 16);
  emoji[3] = i;

  free(emoji_string);
  free(emoji_string_tmp);
  return emoji;
  // return NULL;
}


// Modify the UTF-8 string `utf8str` to invert the FIRST character (which may be up to 4 bytes)
// in the string if it the first character is an emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into ANY non-smiling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(unsigned char *utf8str) {
  if (!memcmp(utf8str, "😊", 4)) memcpy(utf8str, "😢", 4);
  if (!memcmp(utf8str, "🥳", 4)) memcpy(utf8str, "😖", 4);
  if (!memcmp(utf8str, "😏", 4)) memcpy(utf8str, "😟", 4);
  if (!memcmp(utf8str, "🤔", 4)) memcpy(utf8str, "🫢", 4);
  if (!memcmp(utf8str, "😵‍💫", 4)) memcpy(utf8str, "😑", 4);
  if (!memcmp(utf8str, "😬", 4)) memcpy(utf8str, "😦", 4);
}


// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling your
// `emoji_invertChar` function on each character.
void emoji_invertAll(unsigned char *utf8str) {
  int i = 0;
  while (i <= strlen(utf8str)-4) {
    if (memcmp(utf8str+i, "\U0001F000", 4)>=0 && memcmp(utf8str+i, "\U0001FAFF", 4)<=0) {
      emoji_invertChar(utf8str+i);
      i+=4;
    } else {
      i++;
    }
  }
}


// Reads the full contents of the file `fileName, inverts all emojis, and
// returns a newly allocated string with the inverted file's content.
unsigned char *emoji_invertFile_alloc(const char *fileName) {
  FILE *fp = fopen(fileName, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: Cannot open file %s\n", fileName);
    return NULL;
  }
  fseek(fp, 0, SEEK_END);
  size_t fsize = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  unsigned char *string = calloc(fsize+1, 1);
  fread(string, fsize, 1, fp);
  fclose(fp);
  emoji_invertAll(string);
  return string;
  // return NULL;
}
