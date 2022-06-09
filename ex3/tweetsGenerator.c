#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000

#define BASE 10
#define E 5
#define D 4

typedef struct WordStruct {
    char *word;
    int times_in_file;
    struct WordProbability *prob_list;
    int prob_len;
    //... Add your own fields here
} WordStruct;

typedef struct WordProbability {
    struct WordStruct *word_struct_ptr;
    int times_after;
    float parameter;
    int top;
    int bottom;
    //... Add your own fields here
} WordProbability;

/************ LINKED LIST ************/
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
} LinkList;

/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkList *link_list, WordStruct *data)
{
  Node *new_node = malloc (sizeof (Node));
  if (new_node == NULL)
    {
      return 1;
    }
  *new_node = (Node) {data, NULL};

  if (link_list->first == NULL)
    {
      link_list->first = new_node;
      link_list->last = new_node;
    }
  else
    {
      link_list->last->next = new_node;
      link_list->last = new_node;
    }

  link_list->size++;
  return 0;
}

// functions I have added

//void print_info (WordStruct *word)
//{
//  printf ("the word is - %s, it appears %d times in the file\n",
//          word->word, word->times_in_file);
//  printf ("the word has %d different words after it:\n", word->prob_len);
//
//  for (int j = 0; j < word->prob_len; ++j)
//    {
//      printf ("the word %s, appears after %d, bottom: %d, top %d\n",
//              word->prob_list[j].word_struct_ptr->word,
//              word->prob_list[j].times_after,
//              word->prob_list[j].bottom, word->prob_list[j].top);
//
//    }
//}

/**
 * Deletes the '\n' character from words if they are in end of line.
 * @param word to check
 * @return void
 */
void delete_newline (char *word)
{
  int length = strlen (word);
  if (word[length - 1] == '\n')
    {
      word[length - 1] = '\0';
    }
}

/**
 * // returns the position of the word in the prob_list,
  // if it's not there returns -1.
 * @param probability list, word to check
 * @return position of the word in the prob_list
 */
int is_word_in_prob_list(int len, WordProbability *prob_list,
                         WordStruct *second)
{
  int i;
  for (i = 0; i < len; i++)
    {
      if (strcmp (prob_list[i].word_struct_ptr->word, second->word) == 0)
        {
          return i;
        }
    }
  return -1; //word not in prob list

}

/**
 * Checks if the word is in the dictionary.
 * @param word to check
 * @return data of the word-struct, or NULL
 */
WordStruct *is_word_in_dic (char *word, LinkList *dictionary)
{
  Node *ptr = dictionary->first;
  while (ptr != NULL)
    {
      if (strcmp (word, ptr->data->word) == 0)
        {
          return ptr->data;// the word is already in dic
        }
      ptr = ptr->next;
    }
  return NULL; //then the word is not in the dic
}

/**
 * Checks is the word is end of sentence.
 * @param word to check
 * @return 1 if it is, 0 if not
 */
int is_end_of_sentence (struct WordStruct *word)
{
  int length = strlen (word->word);
  if (word->word[length - 1] == '.')
    {
      return 1; //is end of sentence
    }
  else
    {
      return 0; // not end of sentence
    }
}

/*************************************/

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number (int max_number)
{
  int num = rand () % max_number;
  return num;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word (LinkList *dictionary)
{
  while (1)
    {
      int num = get_random_number (dictionary->size);
      Node *temp = dictionary->first;

      for (int i = 0; i < num; i++)
        {
          temp = temp->next;
        }
      if (is_end_of_sentence (temp->data) == 0)
        { //if its not end of sentence, return it
          return temp->data;
        }
    }
  return NULL;
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word (WordStruct *word_struct_ptr)
{
  int times = word_struct_ptr->times_in_file;
  int prob_len = word_struct_ptr->prob_len;
  int i;
  for (i = 0; i < prob_len; i++)
    {
      if (i == 0)
        {
          word_struct_ptr->prob_list[i].bottom = 0;
          word_struct_ptr->prob_list[i].top =
              (word_struct_ptr->prob_list[i].times_after) - 1;
        }
      else if (i == (prob_len - 1))
        {
          word_struct_ptr->prob_list[i].top = times - 1;
          word_struct_ptr->prob_list[i].bottom =
              times - word_struct_ptr->prob_list[i].times_after;
        }
      else
        {
          word_struct_ptr->prob_list[i].bottom =
              word_struct_ptr->prob_list[i - 1].top + 1;
          word_struct_ptr->prob_list[i].top =
              word_struct_ptr->prob_list[i].bottom +
              word_struct_ptr->prob_list[i].times_after - 1;
        }
    }
  int ran = get_random_number (times);
  int j;
  for (j = 0; j < prob_len; j++)
    {
      if (word_struct_ptr->prob_list[j].bottom <= ran
      && ran <= word_struct_ptr->prob_list[j].top)
        {
          return word_struct_ptr->prob_list[j].word_struct_ptr;
        }
    }
  return NULL;
}


/**
 * Receive dictionary, generate and print to
 * stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence (LinkList *dictionary)
{
  int amount = 1;
  WordStruct *curr = get_first_random_word (dictionary);
  printf ("%s", curr->word);
  while (amount < MAX_WORDS_IN_SENTENCE_GENERATION && curr->prob_list != NULL)
    {
      curr = get_next_random_word (curr);
      printf (" %s", curr->word);
      amount++;
    }
  printf ("\n");
  return amount;
}


/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list (WordStruct *first_word,
                                  WordStruct *second_word)
{
  if (first_word->prob_len == 0)
    { //if prob_list is empty, add second word
      first_word->prob_list =
          malloc (sizeof (WordProbability));//allocate prob_list
      if (first_word->prob_list == NULL)
        {
          printf ("Allocation failure: WordStruct -> prob_list");
          exit (EXIT_FAILURE);
        }
      first_word->prob_list[0].word_struct_ptr = second_word;
      first_word->prob_list[0].times_after = 1;
      first_word->prob_len++;
      return 1;
    }
  else
    { //if prob_list is not empty, check if the second word is in prob_list
      int res = is_word_in_prob_list
          (first_word->prob_len, first_word->prob_list, second_word);
      if (res != -1)
        { //if it is, add one to times after
          first_word->prob_list[res].times_after++;
          return 0;
        }
      else
        { // if it's not, add it to the prob_list
          first_word->prob_list = realloc (first_word->prob_list,
                                           (sizeof (WordProbability))
                                           * (first_word->prob_len + 1));
          if (first_word->prob_list == NULL)
            {
              printf ("Allocation failure: reallocation for prob_list");
              exit (EXIT_FAILURE);
            }
          first_word->prob_list[first_word->prob_len]
          .word_struct_ptr = second_word;
          first_word->prob_list[first_word->prob_len]
          .times_after = 1;
          first_word->prob_len++;
          return 1;
        }
    }
}


/**
 * allocating memory for and creates a new word-struct.
 * @param None
 * @return the new word-struct
 */
WordStruct *make_new_wordstruct(void)
{
  WordStruct *word_struct = malloc (sizeof (WordStruct));//allocate wordStruct
  if (word_struct == NULL)
    {
      printf ("Allocation failure: WordStruct");
      exit (EXIT_FAILURE);
    }
  word_struct->word =
      malloc (sizeof (char) * MAX_WORD_LENGTH);//allocate word
  if (word_struct->word == NULL)
    {
      printf ("Allocation failure: WordStruct -> word");
      exit (EXIT_FAILURE);
    }
  return word_struct;
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary (FILE *fp, int words_to_read, LinkList *dictionary)
{
  char line[MAX_SENTENCE_LENGTH];
  WordStruct *prev_word = NULL;
  WordStruct *temp = NULL;
  int is_end;
  char *token;
  int i = 0;
  while (fgets (line, MAX_SENTENCE_LENGTH, fp)
  != NULL && (i < words_to_read || words_to_read == 0))
    {
      token = strtok (line, " ");
      while (token != NULL && (i < words_to_read || words_to_read == 0))
        {
          delete_newline (token); //delete new line
          temp = is_word_in_dic (token, dictionary);
          if (temp == NULL) //if the word is not in the dictionary
            {
              WordStruct *word_struct = make_new_wordstruct();
              strcpy (word_struct->word, token);
              word_struct->times_in_file = 1;
              word_struct->prob_len = 0;
              add (dictionary, word_struct);
              temp = word_struct;
            }
          else
            { //if the word is already in the dictionary
              temp->times_in_file++;
            }
          if (i > 0)
            {
              is_end = is_end_of_sentence (prev_word);
              //check if end of sentence
              if (is_end == 1)
                {
                  prev_word->prob_list = NULL;
                }
              else
                {
                  add_word_to_probability_list (prev_word, temp);
                }
            }
          i++;
          prev_word = temp;
          token = strtok (NULL, " ");
        }
    }
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary (LinkList *dictionary)
{
  Node *temp = dictionary->first;
  while (temp != NULL)
    {
      Node *next = temp->next;
      free (temp->data->word);
      if (temp->data->prob_list != NULL)
        {
          free (temp->data->prob_list);
        }
      free (temp->data);
      free (temp);
      temp = next;
      dictionary->size--;
    }
  free (dictionary);
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */


int main (int argc, char *argv[])
{
  if (argc < D || argc > E)
    {
      printf ("Usage: enter 3 or 4 variables");
      exit (EXIT_FAILURE);
    }
  char *ptr1;
  int seed = strtol (argv[1], &ptr1, BASE);
  char *ptr2;
  int num_of_tweets = strtol (argv[2], &ptr2, BASE);
  char *ptr4;
  int words_to_read;
  if (argc == E)
    {
      words_to_read = strtol (argv[4], &ptr4, BASE);
    }
  else
    {
      words_to_read = 0;
    }
  srand (seed);
  FILE *words_file = fopen (argv[3], "r");
  if (words_file == NULL)
    {
      printf ("Error: couldn't open file");
      exit (EXIT_FAILURE);
    }
  LinkList *words_dictionary = malloc (sizeof (LinkList));
  if (words_dictionary == NULL)
    {
      printf ("Allocation failure: words_dictionary");
      exit (EXIT_FAILURE);
    }
  words_dictionary->first = NULL;
  words_dictionary->last = NULL;
  words_dictionary->size = 0;

  fill_dictionary (words_file, words_to_read, words_dictionary);

  int i;
  for (i = 1; i <= num_of_tweets; i++)
    {
      printf ("Tweet %d: ", i);
      generate_sentence (words_dictionary);
    }

  free_dictionary (words_dictionary);
  fclose (words_file);

  return 0;
}
