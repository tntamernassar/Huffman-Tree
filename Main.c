#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>

typedef struct HuffmanTree {
	int freq;
	char ch;
	struct HuffmanTree* parent;
	struct HuffmanTree* left;
	struct HuffmanTree* right;
} HuffmanTree;

typedef struct LinkedListNode {
	HuffmanTree *tree;
	struct LinkedListNode *next;
} LinkedListNode;

typedef struct LinkedList {
	int size;
	struct LinkedListNode* head;
} LinkedList;

typedef struct decodableNode {
	char c;
	struct decodableNode *next;
} decodableNode;

typedef struct decodableLinkedList {
	struct decodableNode* last;
	struct decodableNode* head;
	int size;
} decodableLinkedList;



HuffmanTree* memory_tree = NULL;


/*init HuffmanTree */
HuffmanTree* initHFT() {
	return (HuffmanTree*)malloc(sizeof(HuffmanTree));
}

/*init LinkedListNode */
LinkedListNode* initLLN() {
	return (LinkedListNode*)malloc(sizeof(LinkedListNode));
}

LinkedList* initLL() {
	return (LinkedList*)malloc(sizeof(LinkedList));
}

decodableNode* initDN() {
	return (decodableNode*)malloc(sizeof(decodableNode));
}

decodableLinkedList* initDLL() {
	return (decodableLinkedList*)malloc(sizeof(decodableLinkedList));
}


/*return 1 if t1>t2 , 0 if t1 = t2 , -1 else*/
int compareTrees(HuffmanTree* t1, HuffmanTree* t2) {


	if (t1->freq > t2->freq)
		return 1;
	else if (t1->freq < t2->freq)
		return -1;
	else
		return ((unsigned char)t1->ch - (unsigned char)t2->ch);

}

/*search for node instance in list*/
LinkedListNode* search(LinkedList* list, LinkedListNode* node) {
	LinkedListNode* head = list->head;

	while (head)
	{
		if ((unsigned char)head->tree->ch - (unsigned char)node->tree->ch == 0)
			return head;

		head = head->next;
	}

	return NULL;
}

void insertDecodable(decodableLinkedList* list, decodableNode* n) {
	if (list->size == 0) {
		list->last = n;
		list->head = n;
	}
	else {
		list->last->next = n;
		list->last = n;
	}

	list->size++;

}

/*insert toAdd to the list , if exist increase it's freq*/
void insert(LinkedList* list, LinkedListNode* toAdd) {
	LinkedListNode* exist = search(list, toAdd);
	LinkedListNode* curr;
	/*add current character freq*/
	if (exist != NULL) {
		exist->tree->freq++;
		return;
	}
	/*empty list , 'toAdd' should be in the head of the list*/
	if (list->size == 0) {
		list->head = toAdd;
		list->size++;
		return;
	}
	curr = list->head;
	/*check if toAdd should be the first element becuase we skip it in the while*/
	if (compareTrees(list->head->tree, toAdd->tree) >= 0) {
		toAdd->next = list->head;
		list->head = toAdd;
		list->size++;
		return;
	}
	/*reach the place that 'toSAdd' should be in*/
	while (curr->next && compareTrees(curr->next->tree, toAdd->tree) < 0)
		curr = curr->next;

	/*not in the end of the list*/
	if (curr->next) {
		toAdd->next = curr->next;
		curr->next = toAdd;
	}
	else {
		/*in the end of the list*/
		curr->next = toAdd;
	}
	/*increase list size*/
	list->size++;
}

/*insert with huffman insertion method ( */
void HUFF_insert(LinkedList* list, LinkedListNode* toAdd) {
	LinkedListNode* curr;
	/*empty list , 'toAdd' is the first element*/
	if (list->size == 0) {
		list->head = toAdd;
		list->size++;
		return;
	}
	curr = list->head;
	/*check if toAdd should be the first element becuase we skip it in the while*/
	if (compareTrees(list->head->tree, toAdd->tree) > 0) {
		toAdd->next = list->head;
		list->head = toAdd;
		list->size++;
		return;
	}
	/*reach 'toAdd' correct place*/
	while (curr->next && compareTrees(curr->next->tree, toAdd->tree) <0)
		curr = curr->next;
	/*not the end of the list*/
	if (curr->next) {
		toAdd->next = curr->next;
		curr->next = toAdd;
	}
	else {
		/*end of the list*/
		curr->next = toAdd;
	}
	list->size++;
}
/*remove the first element in list*/
LinkedListNode* remove_first(LinkedList* list) {
	LinkedListNode* first;
	/*empty size*/
	if (list->size == 0)
		return NULL;
	first = list->head;
	list->head = first->next;
	list->size--;
	return first;
}
/*create a tree from list with the algorithm given*/
HuffmanTree* create_tree_from_list(LinkedList* list) {
	HuffmanTree* res;
	HuffmanTree* tree;
	/*while the list has more than 1 element*/
	while (list->size > 1) {
		LinkedListNode* node; /*result node*/
							  /*new huffman tree node*/
		tree = initHFT();
		tree->left = remove_first(list)->tree;
		tree->freq = tree->left->freq;
		tree->ch = tree->left->ch;
		tree->left->parent = tree;
		/*check if there is elements left in the list*/
		tree->right = remove_first(list)->tree;
		if (tree->right) {
			tree->freq = tree->freq + tree->right->freq;
			tree->right->parent = tree;
		}
		/*add the created node to the list*/
		node = initLLN();
		node->tree = tree;
		node->next = NULL;
		HUFF_insert(list, node);
	}
	res = remove_first(list)->tree;
	res->parent = NULL;
	return res;
}
/*search for the leaf with the same char*/
HuffmanTree* search_tree(HuffmanTree* t, char c) {
	HuffmanTree* res;
	/*leaf and same char*/
	if (!t->left && !t->right && (unsigned char)t->ch == (unsigned char)c)
		return t;
	res = NULL;
	if (t->left)
		res = search_tree(t->left, c);
	if (!res && t->right)
		res = search_tree(t->right, c);
	return res;
}
/*create huffman from text*/
HuffmanTree* createHuffmanTree(char* s) {
	int i;
	LinkedListNode* node;
	LinkedList* l;
	LinkedListNode* curr;
	/*initialize list*/
	LinkedList* list = initLL();
	list->size = 0;
	list->head = NULL;
	/*create node for each character in the string*/
	i = 0;
	while (s[i - 1] != '@') {
		char ch = s[i];
		/*create a new tree*/
		HuffmanTree* tree = initHFT();
		tree->ch = ch;
		tree->freq = 1;
		tree->parent = NULL;
		tree->right = NULL;
		tree->left = NULL;
		/*create a new node for the list*/
		node = initLLN();
		node->tree = tree;
		node->next = NULL;
		insert(list, node);
		i++;
	}
	/*initilize the main list*/
	l = initLL();
	l->head = 0;
	l->size = 0;
	/*order the nodes by there importance*/
	curr = list->head;
	while (curr) {
		LinkedListNode* new_node = initLLN();
		new_node->next = NULL;
		HuffmanTree* t = initHFT();
		t->ch = curr->tree->ch;
		t->freq = curr->tree->freq;
		t->left = NULL;
		t->right = NULL;
		new_node->tree = t;
		insert(l, new_node);
		LinkedListNode* c = curr;
		curr = c->next;
		/*free the unneeded node from the first lise */
		free(c->tree->left);
		free(c->tree->right);
		free(c->tree->parent);
		free(c->tree);
		free(c);
	}
	/*free the first list*/
	free(list);
	/*create the tree*/
	return create_tree_from_list(l);
}
/*for menu*/
void print_memory_status() {
	if (memory_tree) {
		printf("*** Huffman Tree in-memory ***\n");
	}
	else {
		printf("--- No Huffman Tree ---\n");
	}
}
void deserialize_HUFF(LinkedList* list) {
	HuffmanTree* t;
	char str[15];
	char ch;
	int freq;
	gets(str);

	/*Empty string*/
	if (str[0] == '\0') {
		return;
	}

	ch = str[0];
	char sfreq[13];

	/*get frequancy from the given string*/
	int i = 2;
	while (str[i] != '\0') {
		sfreq[i - 2] = str[i];
		i++;
	}

	freq = atoi(sfreq);

	/*add the given tree to the list*/
	t = initHFT();
	t->ch = ch;
	t->freq = freq;
	t->right = NULL;
	t->left = NULL;

	LinkedListNode* n = initLLN();
	n->next = NULL;
	n->tree = t;

	insert(list, n);
	deserialize_HUFF(list);

}
/*return : c*2^power , while c = 1 or 0*/
int b_power(char c, int power) {
	int res = 1;
	if (c == '0') {
		return 0;
	}
	else {

		while (power != 0) {
			res = res * 2;
			power--;
		}

	}

	return res;

}
void print_ASCII(char* bits) {

	int ASCII = 0;

	ASCII = ASCII + b_power(bits[0], 7);
	ASCII = ASCII + b_power(bits[1], 6);
	ASCII = ASCII + b_power(bits[2], 5);
	ASCII = ASCII + b_power(bits[3], 4);
	ASCII = ASCII + b_power(bits[4], 3);
	ASCII = ASCII + b_power(bits[5], 2);
	ASCII = ASCII + b_power(bits[6], 1);
	ASCII = ASCII + b_power(bits[7], 0);
	printf("%c", (char)ASCII);

}
/*encode character with the tree and set the result to 'result' starting from 'start'*/
int encode(HuffmanTree* tree, char character, int start, char* result) {
	HuffmanTree* curr = search_tree(tree, character);
	int i = 0;
	int j;
	char s[10000];
	curr = curr->parent;
	while (curr) {
		if (character == curr->left->ch) {
			s[i] = '0';
		}
		else {
			s[i] = '1';
		}

		character = curr->ch;
		i++;
		curr = curr->parent;
	}
	s[i] = '\0';
	i--;
	j = 0;
	while (i >= 0) {

		result[j + start] = s[i];
		j++;
		i--;
	}
	return j;
}
/*bit encode 'dec'*/
void bit_encode(char* dec, int j) {
	int i = 0;
	int counter = 0;
	char* bits = (char*)malloc(8);

	while (i < j + 1) {
		if (counter == 8) {
			print_ASCII(bits);
			counter = 0;
		}
		bits[counter] = dec[i];
		i++;
		counter++;
	}
}

void decode(HuffmanTree *t, char* bits) {
	int i = 0;
	int flag = 0;
	HuffmanTree* root = t;
	while (flag == 0) {

		if (!t->right && !t->left) {/*reached a leaf*/
			if (t->ch == '@') /*reached '@' and we should stop here */
				flag = 1;

			printf("%c", t->ch);
			t = root;
		}

		/*if 0 check left*/
		if (bits[i] == '0')
			t = t->left;
		else /*if 1 check right*/
			t = t->right;

		i++;
	}

}

void decode_list(HuffmanTree *t, decodableLinkedList* list) {

	decodableNode* curr = list->head;
	int flag = 0;
	HuffmanTree* root = t;
	while (flag == 0) {

		if (!t->right && !t->left) {/*reached a leaf*/
			if (t->ch == '@') /*reached '@' and we should stop here */
				flag = 1;

			printf("%c", t->ch);
			t = root;
		}

		/*if 0 check left*/
		if (curr->c == '0')
			t = t->left;
		else /*if 1 check right*/
			t = t->right;

		curr = curr->next;

	}

}
void decToBinary(decodableLinkedList* list, char ch)
{

	int n = (unsigned char)ch;
	int i = 0;
	int j;
	int c;
	char s[10000];

	/*devide by 2 each time (convert to binary)*/
	while (n > 0) {
		int c = n % 2;
		char r;

		if (c == 1)
			r = '1';
		else
			r = '0';

		n = n / 2;

		s[i] = r;
		i++;
	}


	/*add zeros*/
	while (i % 8 != 0) {
		s[i] = '0';
		i++;
	}
	i--;
	/*reverse result*/
	char res[10000];
	res[i + 1] = '\0';
	j = 0;
	while (i >= 0) {
		res[j] = s[i];
		i--;
		j++;
	}


	res[j] = '\0';


	for (c = 0; c < j; c++) {
		decodableNode* n = initDN();
		n->c = res[c];
		insertDecodable(list, n);
	}

}

void inOrder(HuffmanTree* t) {
	/*leaf reached , should print it's value*/
	if (!t->left && !t->right) {
		printf("%c:%d\n", t->ch, t->freq);
		return;
	}

	if (t->left)
		inOrder(t->left);
	if (t->right)
		inOrder(t->right);
}

/*iterate through huffman tree nodes with inorder and free each node*/
void free_memory(HuffmanTree* t) {
	if (!t->left && !t->right) {
		free(t);
		return;
	}

	if (t->left)
		free_memory(t->left);
	if (t->right)
		free_memory(t->right);

	free(t);
}

void second_menu() {
	int choise;
	char st[10000];

	print_memory_status();
	printf("1. Encode text using the Huffman Tree\n");
	printf("2. Bit-encode text using the Huffman Tree\n");
	printf("3. Decode using the Huffman Tree\n");
	printf("4. Bit-decode using the Huffman Tree\n");
	printf("5. Serialize the Huffman Tree\n");
	printf("6. Free the in-memory Huffman Tree\n");
	printf("Enter you choice: ");
	gets(st);
	choise = atoi(st);

	if (choise == 1) {
		char str[10000];
		gets(str);
		int i = 0;
		int j = 0;
		char* dec = (char*)malloc(80000);
		while (str[i] != '\0') {

			j += encode(memory_tree, str[i], j, dec);

			i++;
		}

		i = 0;
		while (i < j) {
			printf("%c", dec[i]);
			i++;
		}

		while (j % 8 != 0) {
			printf("0");
			j++;
		}

		printf("\n");
		second_menu();
	}
	else if (choise == 2) {

		char str[10000];
		gets(str);

		int i = 0;
		int j = 0;
		char* dec = (char*)malloc(80000);
		while (str[i] != '\0') {

			j += encode(memory_tree, str[i], j, dec);

			i++;
		}
		while (j % 8 != 0) {
			dec[j] = '0';
			j++;
		}
		dec[j] = '\0';
		bit_encode(dec, j);
		printf("\n");
		second_menu();
	}
	else if (choise == 3) {

		char str[10000];
		gets(str);
		decode(memory_tree, str);
		printf("\n");
		second_menu();
	}
	else if (choise == 4) {
		decodableLinkedList* l = initDLL();
		l->last = NULL;
		l->head = NULL;
		l->size = 0;

		char str[10000];
		gets(str);
		int i = 0;
		while (str[i] != '\0') {
			decToBinary(l, str[i]);
			i++;
		}

		decodableNode* curr = l->head;
		while (curr) {

			curr = curr->next;
		}

		decode_list(memory_tree, l);
		printf("\n");
		second_menu();
	}
	else if (choise == 5) {
		inOrder(memory_tree);
		printf("\n");
		second_menu();
	}
	else if (choise == 6) {
		free_memory(memory_tree);
		memory_tree = NULL;
		return;
	}
	else {
		second_menu();
	}
}
int first_menu() {
	int choise;
	char st[10000];

	print_memory_status();
	printf("1. Create a Huffman Tree from text\n");
	printf("2. Deserialize a Huffman Tree\n");
	printf("3. Exit\n");
	printf("Enter you choice: ");

	gets(st);
	choise = atoi(st);


	if (choise == 1) {
		char str[10000];
		gets(str);
		memory_tree = createHuffmanTree(str);
		second_menu();
	}
	else if (choise == 2) {
		LinkedList* list = initLL();
		list->head = NULL;
		list->size = 0;
		deserialize_HUFF(list);
		memory_tree = create_tree_from_list(list);
		second_menu();
	}
	else if (choise == 3) {
		return 0;
	}
	else {
		return first_menu();
	}

	return 1;
}

int main() {

	int r = first_menu();
	if (r != 0)
		main();
	else
		return 0;

}