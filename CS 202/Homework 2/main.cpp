/**
* Title: Binary Search Trees
* Author: Deniz Tuna Onguner
* ID: 22001788
* Section: 2
* Homework: 2
* Description: Main file to test BSTs
*/

#include "analysis.h"

BST *merge(const BST &tree1, const BST &tree2);

int main() {

    /** Test 1: Create BSTs */
    BST tree1;
    BST tree2;
    BST tree3;
    BST tree4;


    /** Test 2: Insert Items */
    tree1.insertItem(8);
    tree1.insertItem(15);
    tree1.insertItem(10);
    tree1.insertItem(4);
    tree1.insertItem(9);
    tree1.insertItem(6);
    tree1.insertItem(16);
    tree1.insertItem(5);
    tree1.insertItem(7);
    tree1.insertItem(14);

    // Print out the BST to see if the insertions are correct
    int lengthTree1AfterInsertions;
    int *keysTree1AfterInsertions = tree1.inorderTraversal(lengthTree1AfterInsertions);
    printf("[Test 2: Insert Items]:\nLength = %d\nItems: ", lengthTree1AfterInsertions);
    for (int i = 0; i < lengthTree1AfterInsertions; i++)
        printf("%d ", keysTree1AfterInsertions[i]);

    delete[] keysTree1AfterInsertions;


    /** Test 3: Delete Items */
    tree1.deleteItem(7);
    tree1.deleteItem(10);
    tree1.deleteItem(8);
    tree1.deleteItem(9);
    tree1.deleteItem(5);

    // Print out the BST to see if the deletions are correct
    int lengthTree1AfterDeletions;
    int *keysTree1AfterDeletions = tree1.inorderTraversal(lengthTree1AfterDeletions);
    printf("\n\n[Test 3: Delete Items]:\nLength = %d\nItems: ", lengthTree1AfterDeletions);
    for (int i = 0; i < lengthTree1AfterDeletions; i++)
        printf("%d ", keysTree1AfterDeletions[i]);

    delete[] keysTree1AfterDeletions;


    // Note that no additional tests are held for inorderTraversal function since it is used already.
    /** Test 4: Has Sequence */
    tree2.insertItem(8);
    tree2.insertItem(4);
    tree2.insertItem(13);
    tree2.insertItem(3);
    tree2.insertItem(6);
    tree2.insertItem(12);
    tree2.insertItem(15);
    tree2.insertItem(1);
    tree2.insertItem(5);
    tree2.insertItem(10);
    tree2.insertItem(14);
    tree2.insertItem(2);

    printf("\n\n[Test 4: Has Sequence]:\n");

    // Test case 1: hasSequence([1, 2, 3, 4, 5, 6], 6)
    int arr1[] = {1, 2, 3, 4, 5, 6};
    const bool hasSequence1 = tree2.hasSequence(arr1, 6);
    printf(" [sequence found: %s]\n", hasSequence1 ? "true" : "false");

    // Test case 2: hasSequence([10, 12, 13, 15], 4)
    int arr2[] = {10, 12, 13, 15};
    const bool hasSequence2 = tree2.hasSequence(arr2, 4);
    printf(" [sequence found: %s]\n", hasSequence2 ? "true" : "false");

    // Test case 3: hasSequence([10, 11, 12], 3)
    int arr3[] = {10, 11, 12};
    const bool hasSequence3 = tree2.hasSequence(arr3, 3);
    printf(" [sequence found: %s]\n", hasSequence3 ? "true" : "false");


    /** Test 5: Merge */
    tree3.insertItem(21);
    tree3.insertItem(43);
    tree3.insertItem(65);
    tree3.insertItem(87);
    tree3.insertItem(52);
    tree3.insertItem(41);
    tree3.insertItem(73);
    tree3.insertItem(32);
    tree3.insertItem(29);
    tree3.insertItem(61);

    tree4.insertItem(85);
    tree4.insertItem(93);
    tree4.insertItem(8);
    tree4.insertItem(2);
    tree4.insertItem(13);
    tree4.insertItem(42);
    tree4.insertItem(51);
    tree4.insertItem(17);
    tree4.insertItem(99);

    int lengthMergedTree;
    BST *mergedTree = merge(tree3, tree4);
    int *mergedTreeKeys = mergedTree->inorderTraversal(lengthMergedTree);
    printf("\n[Test 5: Merge]:\nLength = %d\nItems: ", lengthMergedTree);
    for (int i = 0; i < lengthMergedTree; i++)
        printf("%d ", mergedTreeKeys[i]);

    delete[] mergedTreeKeys;
    delete mergedTree;


    /** Test 6: Time Analysis */
    printf("\n\n[Test 6: Time Analysis]:\n");
    timeAnalysis();

    return 0;
}

BST *merge(const BST &tree1, const BST &tree2) {

    BST *mergedTree = new BST();

    int lengthTree1, lengthTree2;
    int *keysTree1 = tree1.inorderTraversal(lengthTree1);
    int *keysTree2 = tree2.inorderTraversal(lengthTree2);

    int *mergedKeys = new int[(lengthTree1 + lengthTree2)];

    int i = 0, j = 0, k = 0;
    while (i < lengthTree1)
        mergedKeys[k++] = keysTree1[i++];
    while (j < lengthTree2)
        mergedKeys[k++] = keysTree2[j++];

    for (i = 0; i < (lengthTree1 + lengthTree2); i++)
        mergedTree->insertItem(mergedKeys[i]);

    delete[] keysTree1;
    delete[] keysTree2;
    delete[] mergedKeys;

    return mergedTree;
}
