#include <stdio.h>
#include "bst_structs.h"

int main()
{
  FILE* fp = init_tree ("test.dat");
  // delete_key(12, fp);
  insert_key (4, fp);
  insert_key (6, fp);
  insert_key (5, fp);
  insert_key (7, fp);
  insert_key (8, fp);
  insert_key (9, fp);
  display_inorder (fp);
//  disp_freelist (fp);
  delete_key (7, fp);
  disp_freelist (fp);
  insert_key (13, fp);
//  disp_freelist (fp);
  printf("\n");
  printf ("INORDER\n");
  display_inorder (fp);
  // printf ("PREORDER\n");
  // display_preorder (fp);
//  disp_freelist (fp);
  close_tree (fp);
}
