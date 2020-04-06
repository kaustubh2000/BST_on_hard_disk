#include <stdio.h>
#include <stdlib.h>
#include "bst_structs.h"

void delete (tree_t t, int key, FILE* fp, int pos, int pos2);
void insert_node (tree_t t, node_t n, FILE* fp, int pos);
void inorder (FILE* fp, int pos);
void preorder (FILE* fp, int pos);
void add (FILE* fp, int pos);
int inorder_successor (FILE* fp, int pos, int pos2);

// Function to open the file in the desired mode
FILE* init_tree (const char* filename)
{
  FILE* fp;
	fp = fopen (filename, "r+");
	if(fp == NULL)
	{
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

// function to close the file
void close_tree (FILE *fp)
{
  fclose (fp);
}

// function to insert a node into the BST
void insert_node (tree_t t, node_t n, FILE* fp, int pos)
{
  node_t temp;
  fseek (fp, pos, SEEK_SET);
	fread (&temp, sizeof(node_t), 1, fp);
  if (n.key < temp.key) {   // going to the left sub tree

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
    else {   // recursive call to insert the node to the left sub tree
      fseek (fp, temp.left_offset, SEEK_SET);
      pos = ftell(fp);
      insert_node (t, n, fp, pos);
    }
  }
  else if (n.key > temp.key) {  // goes to right sub tree

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
    else {   // recursive call to insert the node to  the right sub tree
      fseek (fp, temp.right_offset, SEEK_SET);
      pos = ftell(fp);
      insert_node (t, n, fp, pos);
    }
  }
	else if (n.key == temp.key) {    // if there is already a node with the same value
		return;
	}
}

// function which calls insert_node
void insert_key (int key, FILE *fp)
{
  tree_t t;
  node_t n;
  n.left_offset = -1;
  n.right_offset = -1;
  n.key = key;

	fseek (fp, 0, SEEK_SET);
  fread (&t, sizeof(tree_t), 1, fp);
  if (t.root == -1) {   // checling if tree is empty
		if (t.free_head == -1){
			t.root = ftell (fp);
			fseek (fp, 0, SEEK_END);
		}
		else {   // checking if there are any holes in the file
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

// function called to display inorder form of the tree
void inorder (FILE* fp, int pos)
{
  if (pos != -1) {
		node_t temp;
		fseek (fp, pos, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);
		inorder (fp, temp.left_offset);
		printf ("%d ", temp.key);
		inorder (fp, temp.right_offset);
	}
}

// recursively displaying inorder form of the BST
void display_inorder (FILE* fp)
{
  tree_t t;
  fread (&t, sizeof(tree_t), 1, fp);
  fseek (fp, t.root, SEEK_SET);
  inorder (fp, t.root);
	fseek (fp, 0, SEEK_SET);
	printf("\n");
}

// recursively displaying preorder form of the BST
void preorder (FILE* fp, int pos)
{
	if (pos != -1) {
		node_t temp;
		fseek (fp, pos, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);
		printf ("%d ", temp.key);
		preorder (fp, temp.left_offset);
		preorder (fp, temp.right_offset);
	}
}

// function called to display preorder form of the tree
void display_preorder (FILE* fp)
{
  tree_t t;
  fread (&t, sizeof(tree_t), 1, fp);
  fseek (fp, t.root, SEEK_SET);
  preorder (fp, t.root);
	fseek (fp, 0, SEEK_SET);
	printf ("\n");
}

// function to add the offset into the linked list of free nodes
void add (FILE* fp, int pos)
{
	tree_t t;
	fseek (fp, 0, SEEK_SET);
	fread (&t, sizeof(tree_t), 1, fp);
	if (t.free_head == -1) {   // if linked list is empty
		t.free_head = pos;
		node_t temp;
		fseek (fp, pos, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);
		temp.left_offset = temp.right_offset = -1;  // using left offset to link
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

// function called to delete a node
void delete_key (int key, FILE* fp)
{
	tree_t t;
	fread (&t , sizeof(tree_t), 1, fp);
	delete (t, key, fp, t.root, -1);
	fseek (fp, 0, SEEK_SET);
}

// recursive function to delete a node
void delete (tree_t t, int key, FILE* fp, int pos, int pos2)
{
	if (pos != -1)
	{
		node_t temp;
		node_t temp2;
		fseek (fp, pos, SEEK_SET);
		fread (&temp, sizeof(node_t), 1, fp);

		if (key < temp.key) {   // recursively calling delete on left subtree
			delete (t, key, fp, temp.left_offset, pos);
		}
		else if (key > temp.key) {   // recursively calling delete on right subtree
			delete (t, key, fp, temp.right_offset, pos);
		}
		else if (key == temp.key)
		{
			if (pos2 == -1)   // if node to be deleted is the root node
			{
				if (temp.right_offset == -1)  // no right subtree
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
					if (temp2.left_offset == -1)    // no left subtree for the right side of the node
					{
						add (fp, temp.right_offset);
						temp.key = temp2.key;
						temp.right_offset = temp2.right_offset;
						fseek (fp, pos, SEEK_SET);
						fwrite (&temp, sizeof(node_t), 1, fp);
					}
					else    // if there is a left subtree to the right side, call the function inorder_successor
					{
						temp.key = inorder_successor (fp, temp.right_offset, -1);
						fseek (fp, pos, SEEK_SET);
						fwrite (&temp, sizeof(node_t), 1, fp);
					}
				}
				return;
			}
			fseek (fp, pos2, SEEK_SET);    // pos2 is the position of the previous node from the node to be deleted
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
	return;
}

// recursive function to get inorder successor
int inorder_successor (FILE* fp, int pos, int pos2)
{
	node_t temp;
	fseek (fp, pos, SEEK_SET);
	fread (&temp, sizeof(node_t), 1, fp);
	if (temp.left_offset != -1)  // as long as a left sub tree exists, call recursively
	{
		return inorder_successor (fp, temp.left_offset, pos);
	}
	add (fp, pos);
	int key = temp.key;
	node_t temp2;
	fseek (fp, pos2, SEEK_SET);   // previous node to inorder successor
	fread (&temp2, sizeof(node_t), 1, fp);
	temp2.left_offset = temp.right_offset;
	fseek (fp, pos2, SEEK_SET);
	fwrite (&temp2, sizeof(node_t), 1, fp);
	return key;
}
