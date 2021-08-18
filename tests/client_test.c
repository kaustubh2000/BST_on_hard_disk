#include <stdio.h>
#include "bst_structs.h"

int main()
{
  FILE* fp = init_tree ("test.dat");
  #if 0
  insert_key (15, fp);
  display_inorder (fp);
  delete_key (10, fp);
  display_inorder (fp);
  delete_key (5, fp);
  display_inorder (fp);
  delete_key (15, fp);
  display_inorder (fp);
  insert_key (10, fp);
  display_inorder (fp);
  insert_key (5, fp);
  display_inorder (fp);
  insert_key (15, fp);
  display_inorder (fp);
  #endif

  #if 1
  for (int i=1, j=100; i<100, j>0; i++, j--) {
    if (i >= j) break;
    insert_key (i, fp);
    insert_key (j, fp);
  }
  display_inorder (fp);
//  disp_freelist (fp);
  for (int i=1, j=100; i<100, j>1; i++, j--) {
    if (i >= j) break;
    delete_key (i, fp);
    delete_key (j, fp);
  }
  display_inorder (fp);
//  disp_freelist (fp);
  for (int i=1, j=100; i<100, j>0; i++, j--) {
    if (i >= j) break;
    insert_key (i, fp);
    insert_key (j, fp);
  }
  printf ("INORDER:\n");
  display_inorder (fp);
//  disp_freelist (fp);
  #endif
  printf ("\n");
  // delete_key (3, fp);
  // insert_key (3, fp);
  // display_inorder (fp);
  // printf ("-------\n");
  // display_preorder (fp);
  close_tree (fp);
}
