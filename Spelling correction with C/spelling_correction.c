#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node* nodeptr;
typedef struct node {
	char ch; // character in this node
	long int bp;  // a byte position in the master file
	nodeptr right;  // a horizontal pointer
	nodeptr below;  // a vertical poiner
} nodetype;

typedef struct record {
	char word[50];
	int  widx;
	int  wfreq;
} ty_rec;

typedef struct correct_result {
	char word[100]; // the result of correction
	int ns, nd, ni, nt; // number of substitution, deletion, insertion, transposistion
	float  penalty; // penalty score
} ty_correction;

ty_correction cwords[50000];
int nres = 0;

int found_perfect_match = 0;

nodeptr ROOT_TRIE = NULL; // Pointer to ROOT_TRIE node of the total trie.

//void retrieve_trie(nodeptr root_sub_trie, char key[], char corr_key[], int ki, int ci, int n_s, int n_d, int n_i, int n_t);
void find_with_spell_correction(nodeptr root_sub_trie, char key[], char corr_key[], int ki, int ci, int n_s, int n_d, int n_i, int n_t);

// Input :
//      (1) ROOT_TRIE of trie (�̰��� �Ķ��Ÿ�� �Ѱ����� ���� �ƴϰ� �Լ� ���ۺκп��� *curr = ROOT_TRIE ���忡 
//          ���� ��Ʈ���� ã�� �����ϵ��� ���� �ǹ�)
//      (2) a string of key (ã�� Ű�� ���ڿ�)
// Output : 
//    (1) integer return value : -1: ã�Ⱑ ������ ���;  0 �̻� ����:  ã�Ⱑ ������ ��� ������ ������ index.
//    (2) *curr : a ptr to end marker node if found
//               (if not found, a ptr to a node at which search stops.)
//    (3) *prev : a ptr to a previous node of *curr
//    (Note: there are several cases in relation of *curr and *prev.)

int search_trie(char key[], nodeptr* curr, nodeptr* prev, int *val_i)
{
	int i = 0;
	*prev = NULL;
	*curr = ROOT_TRIE;
	if (ROOT_TRIE == NULL) return -1;

	do {
		while (key[i] != '\0' && (*curr)->ch == key[i]) {
			(*prev) = (*curr);
			i++;
			(*curr) = (*curr)->below;
		}

		if ((*curr)->ch == '\0' && key[i] == '\0') {
			return i;
		}

		while (*curr != NULL && (*curr)->ch < key[i]) {
			(*prev) = (*curr);
			(*curr) = (*curr)->right;
		}

		if ((*curr) == NULL || (*curr)->ch > key[i]) {
			*val_i = i;  
			return -1;
		}
	} while (1);
} // search_trie

// hang down the nodes for the remaining characters of key during the insertion.
// trie nodes for key[i], key[i+1], ..... are attached vertically to the node  pointed to by tn .
// return pointer to the last node.

nodeptr hang_down(char key[], nodeptr tn, int i) {
	nodeptr newn;
	while (key[i] != '\0') {
		newn = malloc(sizeof(nodetype));
		newn->ch = key[i];
		newn->right = NULL;
		i++;
		tn->below = newn;
		tn = newn;
	}
	newn = malloc(sizeof(nodetype));
	newn->ch = '\0';
	//newn->bp = Byte_Position_Of_Master_File; �����Լ����� last->bp�� ���������� �����ϰ� ���� ���� hang down���� newn�� return ���شٸ�? last = newn�� ��, insert_trie�� ���ٰ� �ű⼭�� �ٽ� newn�� return �ǵ��� �������
	newn->right = NULL;
	newn->below = NULL;
	tn->below = newn;
	return newn;
} // hang_down

float get_penalty(int n_s, int  n_d, int  n_i, int  n_t) {
	return n_s * 1.1 + n_d * 1.3 + n_i * 1.6 + n_t * 1.9;
}

//  insert a key into the trie.  
//  it returns NULL: insertion failure; pointer to the last node: insertion success.
nodeptr insert_trie(char key[50]) {
	int res, i = 0;
	nodeptr node_found, tn;
	nodeptr curr, prev;
	
	res = search_trie(key, &curr, &prev, &i);
	if (res > 0) return;
	if (ROOT_TRIE == NULL) {
		tn = malloc(sizeof(nodetype));
		tn->ch = key[0];
		ROOT_TRIE = tn;
		tn->right = NULL;
		return hang_down(key, tn, 1);
		
	}
	else if (prev == NULL) {
		tn = malloc(sizeof(nodetype));
		tn->ch = key[i];
		tn->right = ROOT_TRIE;
		ROOT_TRIE = tn;
		return hang_down(key, tn, i + 1);
		
	}
	else if (prev->below == curr) {
		tn = malloc(sizeof(nodetype));
		tn->ch = key[i];
		prev->below = tn;
		tn->right = curr;
		return hang_down(key, tn, i + 1);
		
	}
	else if (prev->right == curr && curr != NULL) {
		tn = malloc(sizeof(nodetype));
		tn->ch = key[i];
		prev->right = tn;
		tn->right = curr;
		return hang_down(key, tn, i + 1);
	}
	else if (prev->right == curr && curr == NULL) {
		tn = malloc(sizeof(nodetype));
		tn->ch = key[i];
		prev->right = tn;
		tn->right = NULL;
		return hang_down(key, tn, i + 1);
		
	}
	else {
		printf("logic error in insert_trie.");
		return;
	}

} // function  insert_trie.

void  transposition(nodeptr root_sub_trie, char key[], char  corr_key[], int ki, int ci, int n_s, int n_d, int n_i, int n_t)
{
	nodeptr  p, s;
	int temp;
	char  new_corr_key[100];
	strcpy(new_corr_key, corr_key); //   corr_key string is copied to another space to protect corr_key from later change. 
	p = root_sub_trie;
	do {
		if (p->ch == key[ki + 1]) {
			s = p->below;
			do {
				if (s->ch == key[ki]) {
					new_corr_key[ci] = key[ki + 1];
					new_corr_key[ci + 1] = key[ki]; ci++; new_corr_key[ci + 1] = '\0'; ki++;
					find_with_spell_correction(s->below, key, new_corr_key, ki + 1, ci + 1, n_s, n_d, n_i, n_t + 1);
					break;
				}
				else s = s->right;
			} while (s);
			break;
		}
		else p = p->right;
	} while (p);
	return;
} // transposition 

//     we suppose that character  key[ki] is one altered from any one in nodes of first level of sub-trie.
void  substitution(nodeptr root_sub_trie, char key[], char  corr_key[], int ki, int ci, int n_s, int n_d, int n_i, int n_t)
{
	nodeptr np, start_ptr;
	int temp;
	char  new_corr_key[100];
	strcpy(new_corr_key, corr_key); // ���ο� ������ ���ڿ� ������ ����� ���ݱ����� ����� �����Ѵ�.
	np = root_sub_trie;
	do {
		start_ptr = np->below;
		new_corr_key[ci] = np->ch;
		new_corr_key[ci + 1] = '\0';
		if (start_ptr) {
			find_with_spell_correction(start_ptr, key, new_corr_key, ki + 1, ci + 1, n_s + 1, n_d, n_i, n_t);
		}
		np = np->right;
	} while (np);
	return;
} // substitution 

void  deletion(nodeptr root_sub_trie, char key[], char  corr_key[], int ki, int ci, int n_s, int n_d, int n_i, int n_t)
{
	nodeptr np, start_ptr;
	int temp;
	char  new_corr_key[100];
	strcpy(new_corr_key, corr_key); //  corr_key string is copied to another space to protect corr_key from later change. 
	np = root_sub_trie;
	do {
		start_ptr = np->below;
		new_corr_key[ci] = np->ch;
		new_corr_key[ci + 1] = '\0';
		if (start_ptr) {
			find_with_spell_correction(start_ptr, key, new_corr_key, ki, ci + 1, n_s, n_d+1, n_i, n_t);
		}
		np = np->right;
	} while (np);
	return;
} // deletion 

void  insertion(nodeptr root_sub_trie, char key[], char  corr_key[], int ki, int ci, int n_s, int n_d, int n_i, int n_t)
{
	nodeptr  start_ptr;
	int temp;
	char  new_corr_key[100];
	strcpy(new_corr_key, corr_key); //   corr_key string is copied to another space to protect corr_key from later change. 
	start_ptr = root_sub_trie;
	ki++;
	// ki+1 ������ suffix �� ������ partial trie ���� ã�� ������ �Ѵ�.
	if (start_ptr)
		find_with_spell_correction(start_ptr, key, new_corr_key, ki, ci, n_s, n_d, n_i + 1, n_t);
	return;
} // insertion 


void find_with_spell_correction(nodeptr root_sub_trie, char key[], char corr_key[], int ki, int ci, int n_s, int n_d, int n_i, int n_t) {
	nodeptr curr = root_sub_trie;
	char  new_corr_key[100], c;
	if (!curr)	return; // impossible situation. So no result is registered. 

	if (curr->ch == '\0' && key[ki] == '\0') { // a result found. So register the result.
		strcpy(cwords[nres].word, corr_key); cwords[nres].penalty = get_penalty(n_s, n_d, n_i, n_t);
		cwords[nres].ns = n_s; cwords[nres].nd = n_d; cwords[nres].ni = n_i; cwords[nres].nt = n_t;
		if (get_penalty(n_s, n_d, n_i, n_t) == 0) found_perfect_match = 1;
		nres++;
		if (nres >= 50000) { printf("too many possible corrections.\n"); c = getchar(); }
		return;
	}

	// First we try normal processing. So we try to consume key[ki] normally by trying matching.
	while (curr && curr->ch < key[ki])	curr = curr->right;
	if (curr && curr->ch == key[ki]) {  // matching key[ki] is OK.
		strcpy(new_corr_key, corr_key);	new_corr_key[ci] = key[ki]; new_corr_key[ci + 1] = '\0';
		find_with_spell_correction(curr->below, key, new_corr_key, ki+1, ci+1, n_s, n_d, n_i, n_t);
	}

	// Secondly, we assume all possible errors at ki.
	if (!found_perfect_match && n_s + n_d + n_i + n_t <= 2) {  // If 3 errors found, no more attempts will be made.
		// We assume all possible situations.
		if (ki <= strlen(key) - 1)
			substitution(root_sub_trie, key, corr_key, ki, ci, n_s, n_d, n_i, n_t);
		if (ki <= strlen(key))
			deletion(root_sub_trie, key, corr_key, ki, ci, n_s, n_d, n_i, n_t);
		if (ki <= strlen(key) - 1)
			insertion(root_sub_trie, key, corr_key, ki, ci, n_s, n_d, n_i, n_t);
		if (ki <= strlen(key) - 2)  // At least we need to have two or more letters left for transposition error processing.
			transposition(root_sub_trie, key, corr_key, ki, ci, n_s, n_d, n_i, n_t);
	}
	return;
} // end find_with_spell_correction  

void sort_results(ty_correction cwords[]) {
	int i, j, midx;
	float minval;
	ty_correction temp;
	for (i = 0; i < nres - 1; i++) {
		minval = cwords[i].penalty;
		midx = i;
		for (j = i + 1; j < nres; j++) {
			if (cwords[j].penalty < minval) { minval = cwords[j].penalty; midx = j; }
		}
		temp = cwords[i];
		cwords[i] = cwords[midx];
		cwords[midx] = temp;
	}
}

int printed(char printed_words[][100], char aword[], int total) {
	int i, found = 0;
	for (i = 0; i < total; i++) {
		if (strcmp(printed_words[i], aword) == 0)
			return 1;
	}
	return 0; // not found.
}

void output_top_corrections() {
	char printed_words[500][100];
	int  i, n_printed = 0, num_show = 10;
	for (i = 0; i < nres; i++) {
		if (!printed(printed_words, cwords[i].word, n_printed)) { // if not printed before, print it.
			printf("<%2d>: %s penalty:%7.3f, substi:%d,  delet:%d,  inser:%d,  transp:%d\n", n_printed, cwords[i].word, cwords[i].penalty, cwords[i].ns, cwords[i].nd, cwords[i].ni, cwords[i].nt);
			strcpy(printed_words[n_printed], cwords[i].word); // record a word printed now.
			n_printed++;
			if (n_printed >= num_show)
				break;
		}
	}
}

void main() {
	int i;
	char c, command, word[200];
	int insertion_cnt, res, widx, wcnt;
	ty_rec record;
	nodeptr last, curr, prev;
	int ki = 0, ci = 0; // ki: ���� ó���� key �� ���� �ε���;    ci: ������ ���� corr_key �� �ε���.

	FILE* fpr, * fpw, * fpr1;

	fpr = fopen("Corpus_dictionary_AP_Penn_RARE.txt", "r");
	if (!fpr) {
		printf("file open error of file Corpus_dictionary_AP_Penn_RARE.txt.\n");
		c = getchar();
	}

	fpw = fopen("records.bin", "wb");
	if (!fpw) {
		printf("file open error of file records.bin for writing.\n");
		c = getchar();
	}

	// initialize correction penalties.
	for (i = 0; i < 60; i++) {
		cwords[i].penalty = 0;
	}

	insertion_cnt = 0;

	while (1) {
		res = fscanf(fpr, "%s %d %d", word, &widx, &wcnt);
		if (res != 3) {
			//printf("file fscanf error.\n");
			//c = getchar();
			break;
		}

		// prepare a record in a variable
		if (strlen(word) > 49) {  // too long to store in a record.
			printf("Too long word. Word =%s \n", word);
			continue; // ignore such words.
		}

		strcpy(record.word, word);
		record.widx = widx;
		record.wfreq = wcnt;

		last = insert_trie(word);
		if (!last) {
			//printf ("word insertion error. word = %s\n", word) ;
			//c = getchar () ;
		}
		else {
			insertion_cnt++;
			last->bp = ftell(fpw); // store bypt position of the data file.
			fwrite(&record, sizeof(ty_rec), 1, fpw); // store the record in the data file.
			fflush(fpw);
		}

		if (insertion_cnt % 5000 == 0) {
			printf("insertion count = %d\n", insertion_cnt);
		}
	} // while

	printf("Finishing insertion. \n");
	fclose(fpw);
	fclose(fpr);

	char  key[50], corr_key[50];

	do {
		nres = 0;
		printf("\nTYPE A KEY: ");
		gets(key); sscanf(key, "%s", key);
		corr_key[0] = '\0';
		found_perfect_match = 0;
		ki = 0, ci = 0;
		find_with_spell_correction(ROOT_TRIE, key, corr_key, ki, ci, 0, 0, 0, 0);
		sort_results(cwords);
		printf("total number of corrected results = %d\n\n", nres);
		output_top_corrections();
	} while (1);
}

