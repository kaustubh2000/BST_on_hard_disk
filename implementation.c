#include <stdio.h>
#include <stdlib.h>
#include "bst_structs.h"

void delete (tree_t t, int key, FILE* fp, int pos, int pos2);
int inorder_successor (FILE* fp, int pos, int pos2);
void disp_freelist (FILE* fp);

void disp_freelist (FILE* fp)
{
	tree_t t;
	fseek (fp, 0, SEEK_SET);
	fread (&t, sizeof(tree_t), 1, fp);
	int val = t.free_head;
	printf ("FREE LIST:\n");
	while (val != -1)
	{
		printf ("%d -> ", val);
		node_t temp;
		fseek (fp, val, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);
		val = temp.left_offset;
	}
	printf ("%d\n", val);
	fseek (fp, 0, SEEK_SET);
}

FILE* init_tree (const char* filename)
{
  FILE* fp;
	fp = fopen (filename, "r+");
	if(fp == NULL)
	{
		// printf("opening file failed for r+\n");
		fp = fopen(filename, "w+");
		if(fp == NULL)
		{
			printf("fatal: cannot open file\n");
			exit(1);
		}
		tree_t t;
	  t.root = -1;
	  t.free_head = -1;
	  fwrite (&t, sizeof(tree_t), 1, fp);
		fseek (fp, 0, SEEK_SET);
	  return fp;
	}
	return fp;
}

void close_tree (FILE *fp)
{
  fclose (fp);
}

void insert_node (tree_t t, node_t n, FILE* fp, int pos)
{
	// count++; if (count >= 10) return;
  node_t temp;
  fseek (fp, pos, SEEK_SET);
	fread (&temp, sizeof(node_t), 1, fp);
	// printf ("NEW -- Key: %d ; Left offset: %d ; Right offset: %d\n", n.key, n.left_offset, n.right_offset);
  if (n.key < temp.key) {

    if (temp.left_offset == -1) {

			if (t.free_head == -1) {

				fseek (fp, 0, SEEK_END);
				temp.left_offset = ftell (fp);
				fwrite (&n, sizeof(node_t), 1, fp);
	      fseek (fp, pos, SEEK_SET);
	      fwrite (&temp, sizeof(node_t), 1, fp);
				fseek (fp, 0, SEEK_END);
				return;

			}

			else {

				fseek (fp, t.free_head, SEEK_SET);
				node_t temp2;
				fread (&temp2, sizeof(node_t), 1, fp);
				fseek (fp, t.free_head, SEEK_SET);
				temp.left_offset = ftell (fp);
				t.free_head = temp2.left_offset;
				fwrite (&n, sizeof(node_t), 1, fp);
				fseek (fp, 0, SEEK_SET);
				fwrite (&t, sizeof(tree_t), 1, fp);
				fseek (fp, pos, SEEK_SET);
				fwrite (&temp, sizeof(node_t), 1, fp);
				return;

			}
    }
    else {

      fseek (fp, temp.left_offset, SEEK_SET);
      pos = ftell(fp);
      insert_node (t, n, fp, pos);

    }
  }
  else if (n.key > temp.key) {

    if (temp.right_offset == -1) {

			if (t.free_head == -1) {

				fseek (fp, 0, SEEK_END);
				temp.right_offset = ftell(fp);
				fwrite (&n, sizeof(node_t),1, fp);
	      fseek (fp, pos, SEEK_SET);
	      fwrite (&temp, sizeof(node_t), 1, fp);
				fseek (fp, 0, SEEK_END);
	      return;
    	}

			else {

				fseek (fp, t.free_head, SEEK_SET);
				node_t temp2;
				fread (&temp2, sizeof(node_t), 1, fp);
				fseek (fp, t.free_head, SEEK_SET);
				temp.right_offset = ftell (fp);
				t.free_head = temp2.left_offset;
				fwrite (&n, sizeof(node_t), 1, fp);
				fseek (fp, 0, SEEK_SET);
				fwrite (&t, sizeof(tree_t), 1, fp);
				fseek (fp, pos, SEEK_SET);
				fwrite (&temp, sizeof(node_t), 1, fp);
				return;

			}
		}
    else {
      fseek (fp, temp.right_offset, SEEK_SET);
      pos = ftell(fp);
			// printf ("Key: %d ; Left offset: %d ; Right offset: %d\n", temp.key, temp.left_offset, temp.right_offset);
      insert_node (t, n, fp, pos);
    }
  }
	else if (n.key == temp.key) {
		printf ("Equal not allowed -- key -- %d\n", n.key);
		return;
	}
}

void insert_key (int key, FILE *fp)
{
  tree_t t;
  node_t n;
  n.left_offset = -1;
  n.right_offset = -1;
  n.key = key;

	fseek (fp, 0, SEEK_SET);
  fread (&t, sizeof(tree_t), 1, fp);
  if (t.root == -1) {
		if (t.free_head == -1){
			t.root = ftell (fp);
			fseek (fp, 0, SEEK_END);
		}
		else {
			t.root = t.free_head;
			fseek (fp, t.free_head, SEEK_SET);
			node_t temp;
			fread (&temp, sizeof(node_t), 1, fp);
			fseek (fp, t.free_head, SEEK_SET);
			t.free_head = temp.left_offset;
		}
			fwrite (&n, sizeof(node_t), 1, fp);
			fseek (fp, 0, SEEK_SET);
			fwrite (&t, sizeof(tree_t), 1, fp);
			fseek (fp, 0, SEEK_SET);
	    return;
	}
  insert_node (t, n, fp, t.root);
	fseek (fp, 0, SEEK_SET);
}

void inorder (FILE* fp, int pos)
{
  if (pos != -1) {
		node_t temp;
		fseek (fp, pos, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);
		inorder (fp, temp.left_offset);
		printf ("Key: %d ; Left offset: %d ; Right offset: %d Pos: %d\n", temp.key, temp.left_offset, temp.right_offset, pos);
		// printf ("%d ", temp.key);
		inorder (fp, temp.right_offset);
	}
}

void display_inorder (FILE* fp)
{
  tree_t t;
  fread (&t, sizeof(tree_t), 1, fp);
	// printf ("\nfree head is: %d\n", t.free_head);
  fseek (fp, t.root, SEEK_SET);
  inorder (fp, t.root);
	fseek (fp, 0, SEEK_SET);
	printf("\n");
}

void preorder (FILE* fp, int pos)
{
	if (pos != -1) {
		node_t temp;
		fseek (fp, pos, SEEK_SET);
		// printf ("curr pos: %d\n", pos);
		fread (&temp, sizeof(node_t), 1, fp);
		// printf ("Key: %d ; Left offset: %d ; Right offset: %d Pos: %d\n", temp.key, temp.left_offset, temp.right_offset, pos);
		printf ("%d ", temp.key);
		preorder (fp, temp.left_offset);
		preorder (fp, temp.right_offset);
	}
}

void display_preorder (FILE* fp)
{
  tree_t t;
  fread (&t, sizeof(tree_t), 1, fp);
  fseek (fp, t.root, SEEK_SET);
	// printf ("\nfree head is: %d\n", t.free_head);
  preorder (fp, t.root);
	fseek (fp, 0, SEEK_SET);
	printf ("\n");
}

void add (FILE* fp, int pos)
{
	tree_t t;
	fseek (fp, 0, SEEK_SET);
	fread (&t, sizeof(tree_t), 1, fp);
	if (t.free_head == -1) {
		t.free_head = pos;
		node_t temp;
		fseek (fp, pos, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);
		temp.left_offset = temp.right_offset = -1;
		fseek (fp, pos, SEEK_SET);
		fwrite (&temp, sizeof(node_t), 1, fp);
		fseek (fp, 0, SEEK_SET);
		fwrite (&t, sizeof(tree_t), 1, fp);
	}
	else {
		fseek (fp, t.free_head, SEEK_SET);
		node_t temp;
		fread (&temp, sizeof(node_t), 1, fp);
		while (temp.left_offset != -1) {
			fseek (fp, temp.left_offset, SEEK_SET);
			fread (&temp, sizeof(node_t), 1, fp);
		}
		temp.left_offset = pos;
		fseek (fp, -sizeof(node_t), SEEK_CUR);
		fwrite (&temp, sizeof(node_t), 1, fp);
		fseek (fp, pos, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);
		temp.left_offset = -1;
		fseek (fp, pos, SEEK_SET);
		fwrite (&temp, sizeof(node_t), 1, fp);
	}
	fseek (fp, 0, SEEK_SET);
}

void delete_key (int key, FILE* fp)
{
	tree_t t;
	fread (&t , sizeof(tree_t), 1, fp);
	delete (t, key, fp, t.root, -1);
	fseek (fp, 0, SEEK_SET);
}

void delete (tree_t t, int key, FILE* fp, int pos, int pos2)
{
	if (pos != -1)
	{
		node_t temp;
		node_t temp2;
		fseek (fp, pos, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);

		if (key < temp.key) {
			delete (t, key, fp, temp.left_offset, pos);
		}
		else if (key > temp.key) {
			delete (t, key, fp, temp.right_offset, pos);
		}
		else if (key == temp.key)
		{
			if (pos2 == -1)
			{
				if (temp.right_offset == -1)
				{
					add (fp, t.root);
					fseek (fp, 0, SEEK_SET);
					fread (&t, sizeof(tree_t), 1, fp);
					t.root = temp.left_offset;
					fseek (fp, 0, SEEK_SET);
					fwrite (&t, sizeof(tree_t), 1, fp);
				}
				else
				{
					fseek (fp, temp.right_offset, SEEK_SET);
					fread (&temp2, sizeof(node_t), 1, fp);
					if (temp2.left_offset == -1)
					{
						add (fp, temp.right_offset);
						temp.key = temp2.key;
						temp.right_offset = temp2.right_offset;
						fseek (fp, pos, SEEK_SET);
						fwrite (&temp, sizeof(node_t), 1, fp);
					}
					else
					{
						temp.key = inorder_successor (fp, temp.right_offset, -1);
						fseek (fp, pos, SEEK_SET);
						fwrite (&temp, sizeof(node_t), 1, fp);
					}
				}
				return;
			}
			fseek (fp, pos2, SEEK_SET);
			fread (&temp2, sizeof(node_t), 1, fp);
			if (temp.key < temp2.key) {

				if (temp.right_offset == -1) {

					add (fp, temp2.left_offset);
					temp2.left_offset = temp.left_offset;
					fseek (fp, pos2, SEEK_SET);
					fwrite (&temp2, sizeof(node_t), 1, fp);

				}
				else {

					fseek (fp, temp.right_offset, SEEK_SET);
					node_t temp3;
					fread (&temp3, sizeof(node_t), 1, fp);

					if (temp3.left_offset == -1)
					{
						add (fp, temp.right_offset);
						temp.key = temp3.key;
						temp.right_offset = temp3.right_offset;
						fseek (fp, pos, SEEK_SET);
						fwrite (&temp, sizeof(node_t), 1, fp);
					}

					else
					{
						temp.key = inorder_successor (fp, temp.right_offset, -1);
						fseek (fp, pos, SEEK_SET);
						fwrite (&temp, sizeof(node_t), 1, fp);
					}

				}
			}
			else if (temp.key > temp2.key) {

				if (temp.right_offset == -1) {

					add (fp, temp2.right_offset);
					temp2.right_offset = temp.left_offset;
					fseek (fp, pos2, SEEK_SET);
					fwrite (&temp2, sizeof(node_t), 1, fp);

				}
				else {

					fseek (fp, temp.right_offset, SEEK_SET);
					node_t temp3;
					fread (&temp3, sizeof(node_t), 1, fp);

					if (temp3.left_offset == -1)
					{
						add (fp, temp.right_offset);
						temp.key = temp3.key;
						temp.right_offset = temp3.right_offset;
						fseek (fp, pos, SEEK_SET);
						fwrite (&temp, sizeof(node_t), 1, fp);
					}
					else
					{
						temp.key = inorder_successor (fp, temp.right_offset, -1);
						fseek (fp, pos, SEEK_SET);
						fwrite (&temp, sizeof(node_t), 1, fp);
					}

				}
			}
		}
		return;
	}
	printf ("Data not found.\n");
	return;
}

int inorder_successor (FILE* fp, int pos, int pos2)
{
	node_t temp;
	fseek (fp, pos, SEEK_SET);
	fread (&temp, sizeof(node_t), 1, fp);
	if (temp.left_offset != -1)
	{
		return inorder_successor (fp, temp.left_offset, pos);
	}
	add (fp, pos);
	int key = temp.key;
	node_t temp2;
	fseek (fp, pos2, SEEK_SET);
	fread (&temp2, sizeof(node_t), 1, fp);
	temp2.left_offset = temp.right_offset;
	fseek (fp, pos2, SEEK_SET);
	fwrite (&temp2, sizeof(node_t), 1, fp);
	return key;

}
