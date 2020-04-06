# BST_on_hard_disk
Writing a BST (Binary Search Tree) on the hard disk

This code is used to store a BST into a file i.e. the hard disk itself, instead of the heap of the program. Offsets are used to get the position of subtrees.
Function to open the file: init_file ("<file_name>.dat")
Function to insert and delete: insert_key / delete_key ( <key>, <file_pointer> )
Function to display the inorder or preorder form of the BST: display_inorder / display_preorder ( <file_pointer> )

Client code should contain the functions above if you want to test this code.

To compile: gcc implementation.c <client>.c -o <output_file>.out
This will run the client code against the file which will implement the BST in the file. You can use the three client files already provided if you want to.
