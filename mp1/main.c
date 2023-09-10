#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emoji.h"
#include "emoji-translate.h"

int main() {
  // Feel free to delete all of this and use this function in any way you want:
  printf("Currently, `main.c` provides a basic call to your functions.\n");
  printf("- Edit `main.c` to test your functions within the context of `main`.\n");
  printf("- A test suite is provided in `tests` folder.\n");

  // size_t a = "0xf09faba4";
  printf("Your favorite emoji: %s\n", emoji_favorite());  
  printf("Random emoji: %s\n", emoji_random_alloc());

  emoji_t emoji;
  emoji_init(&emoji);

  emoji_add_translation(&emoji, "🧡", "heart");

  // The file on disk contains: "I 🧡💙 Illinois!"
  unsigned char *translation = emoji_translate_file_alloc(&emoji, "tests/txt/simple.txt");

  // Translation Output: "I heart💙 Illinois!"
  printf("%s\n", translation);

  emoji_destroy(&emoji);

  return 0;
}