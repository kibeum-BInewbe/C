#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define   Tbl_size  37533	// �� ȸ��� ���� 1.1 �� ����.

typedef struct ty_record {
	char name[300];
	int monincome;  // ����: õ��.
	int link;
} type_record;

type_record Hashtable[Tbl_size];
int LAST = Tbl_size - 1; // ���� ���� ���ڸ��� �ʱ�ȭ.
float average_num_probe;
FILE* fp;

int del_middle(int s, int p, int* chain_split);

int hash(char recname[]) {
	unsigned char u; int HA, j, leng, halfleng;
	long sum = 0;
	int A[300];
	// name �� ���� j �� ���Ͽ� ������ �����Ѵ�:
	leng = strlen(recname);
	for (j = 0; j < leng; j++) {
		u = recname[j];
		A[j] = u;
	}
	halfleng = leng / 2;
	for (j = 0; j < halfleng; j++)
		sum = sum + (A[j] * A[leng - 1 - j] * A[(leng - 1) / 2]);
	if (leng % 2 == 1)
		sum = sum + A[halfleng] * A[halfleng + 1] * A[(leng - 1) / 2];
	HA = sum % Tbl_size;  // HA is a home address given by the hash function.
	return HA;
} // end of hash

// ���ڸ� ã��. �������� LAST �� �̿��Ѵ�.
// LAST ���Ͽ� ���ڸ��� ���� �� �ִ�. �� LAST ���� ū ��ġ���� ���ڸ��� ����.
// ���� �Լ��� LAST ���Ͽ��� ���ڸ��� �ִ� ���� ���� �ּҸ� ã�� �ش�. (LAST �� ����).
int find_empty_location() {
	int curr;
	curr = LAST;
	while (Hashtable[curr].name[0] != '\0')  // curr �� ���ڸ��� �ƴϸ� ������.
		curr -= 1;
	return curr;
} // end of find_empty_location.

// return value: �ִ� ������ ���ڵ���� ������ �� ��. (-1 �̸� �Է� ����).
int insert_rec(type_record rec) {
	int HA, curr, nprove = 0, empty_loc;
	HA = hash(rec.name);
	if (Hashtable[HA].name[0] == '\0') { // Ȩ�ּҰ� ��� ����. ���⿡ �ְ� ����.
		Hashtable[HA] = rec; // HA ��ġ�� link �� �̹� -1 ��.
		return 1; // probe ���� 1.
	}
	else { // Ȩ �ּҿ� �̹� ���ڵ尡 ��� ����.
		curr = HA;
		// ��ũ�� ���������� ���� ����.
		do {
			if (strcmp(Hashtable[curr].name, rec.name) == 0)  // ������ ���� �̹� ��� ����.
			{
				printf("insertion failed since same key already exists.\n");
				return -1;
			}
			nprove += 1;
			if (Hashtable[curr].link == -1)
				break;
			else
				curr = Hashtable[curr].link;
		} while (1);

		// curr �� ������ ���Ҹ� ����Ų��. ���ڸ��� ���� �´�.
		empty_loc = find_empty_location();
		// ���⿡ ���ڵ带 �ְ�, chain�� �����Ѵ�.
		Hashtable[empty_loc] = rec;	// �� ��ġ�� ���ڵ� �ְ�
		Hashtable[curr].link = empty_loc; // ���� ��ũ���� �� ��ġ�� ����

		return nprove;
	} //else.
} // end of insert_rec

// ��ȯ��: -1 �̸� ã�� ����, �ƴϸ� ã�� ��ġ.
// probe: ã�µ� ����� ���ڵ� ���κ� ��.
int retrieve_rec(char* key, int* probe) {
	int curr;
	curr = hash(key);
	if (Hashtable[curr].name[0] == '\0') // home address is empty.
		return -1;

	*probe = 0;
	do {
		*probe += 1;
		if (strcmp(Hashtable[curr].name, key) == 0) {
			return curr;
		}
		else
			curr = Hashtable[curr].link;
	} while (curr != -1);
	// ���⿡ ���� ã�⸦ ������ ����.
	return -1;
} // retrieve_rec 

float compute_average_number_of_probes_per_search() {
	char line[300], * res;
	int nprobe, loc, leng, n_search_done = 0, total_probes = 0;
	float average_probe;

	fp = fopen("Companies_Korean.txt", "r");
	while (1)
	{
		res = fgets(line, 300, fp);
		if (!res)
			break;
		leng = strlen(line);
		line[leng - 1] = '\0';  // ������ newline ���ڸ� �����.
		loc = retrieve_rec(line, &nprobe);
		if (loc != -1) {
			n_search_done += 1;
			total_probes += nprobe;
		}
	}
	average_probe = (float)total_probes/(float)n_search_done;
	fclose(fp);
	return average_probe;
} // end of compute_average_number_of_probes_for_search


// return value: number of relocations of records.
// chain_split: ü�κи� ȸ���� ǥ���� ������ ���� �ּ�.
int del_start(int s, int* chain_split)
{
	int curr, prev, nmove, found = -1, prev_found;

	// s ���� ��ġ���� ü�� ������ Ȩ�ּҰ� s �� �� ������ ���ڵ带 ã�´�.
	curr = Hashtable[s].link;
	prev = s;
	while (curr != -1)
	{
		if (hash(Hashtable[curr].name) == s) {
			prev_found = prev; // ã�� ���� ���� �ּҵ� ����.
			found = curr; // ã�� ��� ����.
		}
		// ���� �ּҷ� �����Ѵ�.
		prev = curr;
		curr = Hashtable[curr].link;

	} //while.

	if (found == -1) { // Ȩ�ּҰ� s �� ���ڵ尡 �߰ߵ��� ����.
		// ��ġ s (�� ü���� ù ����)�� ü�ο��� �����Ѵ�. ���� s �� ���̷��ڵ带 �����ϸ� ��.
		Hashtable[s].name[0] = '\0';
		return 0; // no moves of records have happened.
	}
	else {  // found ���� �߰ߵ�. found �� ���ڵ带 s �� �̵���.
		strcpy(Hashtable[s].name, Hashtable[found].name);
		Hashtable[s].monincome = Hashtable[found].monincome;
		// found �� ���ڵ带 ����� ���ο� ������ �����. �Ʒ� 1 �� ���� �ѹ� �̵��� �ݿ���.
		nmove = 1 + del_middle(found, prev_found, chain_split);
		return nmove;
	}
} // end of del_starat.

// return value: number of relocations of records.
// chain_split: ü�κи� ���θ� ǥ���� ������ ���� �ּ�.
int del_middle(int s, int p, int* chain_split)
{
	int curr, prev, HA_curr, HA_curr_belongs_to_D_s, element_of_D_s, nmove, found, prev_found;

	// s ���� ��ġ���� ü�� ������ Ȩ�ּҰ� s �� �� ������ ���ڵ带 ã�´�.
	curr = Hashtable[s].link;
	prev = s;
	found = -1;
	while (curr != -1)
	{
		if (hash(Hashtable[curr].name) == s) {
			prev_found = prev; // ã�� ���� ���� �ּҵ� ����.
			found = curr; // ã�� ��� ����.
		}
		// ���� �ּҷ� �����Ѵ�.
		prev = curr;
		curr = Hashtable[curr].link;

	} //while.

	if (found == -1) { // ã�� ����. ��ġ s �� ü�ο��� skip �ϰ�, s �� ���ڵ带 �����Ѵ�.
		Hashtable[p].link = Hashtable[s].link;  // skip s.
		Hashtable[s].name[0] = '\0'; // s �� ���� ���ڵ带 �־� ����.
		return 0; // no relocations of records occurred.
	}
	else { // s �� home address �� �ϴ� ���Ұ� s ������ ü�ο� �����Ѵ�.
		// D(s): ��ġ s �� �����Ͽ� ���� ü���� ��� �ּҵ��� ����. 
		//       �̰��� ����� ��ġ s ���� ������ ü���� ������ link �ʵ��� ������̴�(-1�� ����).
		//       s �� �����ں��� �����Ͽ� HA �� D(s) �� ������ �ʴ� ������ ���ڵ带 ã��.
		found = -1; // found will have the address that meets the condition.
		curr = Hashtable[s].link; // curr scans elements of chain after location s.
		prev = s; // this is predecessor of curr.
		while (curr != -1) {
			HA_curr = hash(Hashtable[curr].name); // home address of curr.
			HA_curr_belongs_to_D_s = 0; // �ֱ�ȭ. 0 �� curr �� Ȩ�ּҰ� D(s)�� ������ ������ ����.
			element_of_D_s = s; // element_of_D_s �� D(s) �� ���Ҹ� ��ĵ�� ����.
			do { // scan D(s)
				if (element_of_D_s == HA_curr) {
					HA_curr_belongs_to_D_s = 1;
					break;
				}
				else
					element_of_D_s = Hashtable[element_of_D_s].link;
			} while (element_of_D_s != -1);  // -1 �� curr�� �����ص� ������ �ð��� �� �ɸ���.
			// 
			if (HA_curr_belongs_to_D_s == 0) {  // D(s) �� ������ �ʴ� HA �� ���� ���� �߰�!
				found = curr;
				prev_found = prev;
			}

			prev = curr;
			curr = Hashtable[curr].link;
		}
		// found �� ã�� ����� �ִ�.
		if (found != -1) { // D(s)�� HA�� ������ �ʴ� ���Ұ� ������. 
			strcpy(Hashtable[s].name, Hashtable[found].name);
			Hashtable[s].monincome = Hashtable[found].monincome;
			nmove = 1 + del_middle(found, prev_found, chain_split);
			return nmove;
		}
		else { // s ���� ������ ��� ���Ҵ� home address �� D(s)�� ����.
			// s �� �����ڿ��� ü���� �и��ص� �ȴ�.
			Hashtable[p].link = -1; // ü�� �и�.
			*chain_split += 1; // ü�� �и� Ƚ�� ����.
			nmove = del_start(s, chain_split);  // s�� �ִ� ���ڵ� ����. s �� ü���� ����.
			return nmove;
		}
	}
} // end of del_middle.

// return value: number of record relocations
// parameter chain_split: used for relecting chain splits
int delete_rec(char* dkey, int* chain_split) {
	int found = -1, h, curr, prev, nmove = 0;

	h = hash(dkey);
	if (Hashtable[h].name[0] == '\0')
	{
		printf("There are no records with such key.\n");
		return -1;
	}

	if (strcmp(dkey, Hashtable[h].name) == 0)  // dkey is at its home address.
		nmove = del_start(h, chain_split);
	else {
		// search the record whose key is equal to dkey from the next of address h.
		curr = Hashtable[h].link;
		prev = h;
		found = -1;
		while (curr != -1) {
			if (strcmp(Hashtable[curr].name, dkey) == 0) {
				found = curr;
				break;
			}
			else {
				prev = curr;
				curr = Hashtable[curr].link;
			}
		} //while.

		if (found == -1)
		{
			printf("A record with such key does not exist.\n");
			return -1;
		}
		else
			nmove = del_middle(found, prev, chain_split);
	}//else.

	return nmove;
} // end of delete_rec

void delete_multiple(int num_del_req) {
	char line[300], * res;
	int cnt_lines = 0, num_split = 0, nmove, leng, num_deletion_success = 0, num_relocated_deletions = 0;

	fp = fopen("Companies_Korean.txt", "r");
	while (1)
	{
		res = fgets(line, 300, fp);
		if (!res)
			break;
		cnt_lines += 1;
		leng = strlen(line);
		line[leng - 1] = '\0';  // ������ newline ���ڸ� �����.
		nmove = delete_rec(res, &num_split);
		if (nmove >= 0)
			num_deletion_success += 1;
		if (nmove > 0)
			num_relocated_deletions += nmove;
		if (cnt_lines == num_del_req)
			break;
	}
	printf("num_deletion_success=%d, num_relocated_deletions=%d\n", num_deletion_success, num_relocated_deletions);
	fclose(fp);
	return;
}

int check_num_links(char key[]) {
	int count = 0, curr, flag = -1;
	int HA = hash(key);
	if (Hashtable[HA].name[0] == '\0') {
		printf("There are no records with such key.\n");
		return -1;
	}

	curr = HA;

	// move the curr, and fine the key. if the key doesn't exist in hashtable but has hash links,
	// we have to use flag.
	while (curr != -1) {
		if (strcmp(Hashtable[curr].name, key) == 0) {
			flag = 1;
		}
		count += 1;
		curr = Hashtable[curr].link;
	}
	if (flag != 1) {
		printf("There are no records with such key.\n");
		return -1;
	}
	return count;
}

int main(void)
{
	type_record a_rec;
	int cnt_lines = 0, cnt_insertion = 0, pos, n_probe, i, k, leng, m_income, n_chain_split, n_move, num_deletions_to_do;
	int count;
	char line[300], name[1300], * res;

	// ���̺� �ʱ�ȭ. ��� ��ġ�� ���ڸ��� ��.
	for (i = 0; i < Tbl_size; i++) {
		Hashtable[i].name[0] = '\0';
	}

	// ���Ͽ��� ��� �̸��� �о �ؽ����̺� �ִ´�.
	fp = fopen("Companies_Korean.txt", "r");
	while (1)
	{
		res = fgets(line, 300, fp);
		if (!res)
			break;
		cnt_lines += 1;
		leng = strlen(line);
		line[leng - 1] = '\0';  // ������ newline ���ڸ� �����.
		m_income = rand() % 50000; // ������ õ��
		strcpy(a_rec.name, line);
		a_rec.monincome = m_income;
		a_rec.link = -1;
		n_probe = insert_rec(a_rec);
		if (n_probe != -1)
			cnt_insertion += 1; // ���Լ��� Ƚ�� ����
	}
	fclose(fp);
	printf("num of line=%d,  num of inserted records=%d\n\n", cnt_lines, cnt_insertion);

	// ��ɹ� ����
	while (1) {
		printf("���(i,r,v,d,q,c,e)> ");
		res = gets(line);
		if (!res)
		{
			printf("wrong command.\n");
			continue;
		}
		if (strlen(line) < 1)
			continue;

		if (line[0] == 'e')  // exit command.
			break;

		if (line[0] == 'v')  // a command to verify average number of probes of a search.
		{
			average_num_probe = compute_average_number_of_probes_per_search();
			printf("average number of probes per search = %f\n", average_num_probe);
			continue;
		}

		// read a name.
		k = 2; i = 0;
		while (line[k] != '\0' && line[k] == ' ') k++; // ���鹮�ڴ� �ǳʰ���.
		while (line[k] != '\0') { // read one key which is a company name(including a blank).
			name[i] = line[k];
			k++;  i++;
		}
		name[i] = '\0';

		switch (line[0]) {
		case 'i': // insert command:  i ������ȸ
			strcpy(a_rec.name, name); a_rec.monincome = rand() % 50000; a_rec.link = -1;
			n_probe = insert_rec(a_rec);
			if (n_probe == -1)
				printf("insertion failed by duplicate key.\n");
			else
				printf("insertion succeeded. num probe = %d\n", n_probe);
			break;
		case 'r': // search command: r ������ȸ
			n_probe = 0;
			pos = retrieve_rec(name, &n_probe);
			if (pos == -1)
				printf("No such record exists.\n");
			else
				printf("Search succeeded. loc_found=%d, n_probes=%d\n", pos, n_probe);
			break;
		case 'd':  // deletion command:  d ������ȸ
			n_chain_split = 0;
			n_move = delete_rec(name, &n_chain_split);
			if (n_move >= 0)
				printf("Deletion succeeded. num. relocations=%d, num chain splits=%d\n", n_move, n_chain_split);
			break;
		case 'q':  // multiple deletion command:  q  20000   (deletion of the first 20,000 names in the data file).
			for (k = 0; k < i; k++) { // check if name is a number string.
				if (!isdigit(name[k])) {
					printf("number should be given after q.\n");  break;
				}
			}
			num_deletions_to_do = atoi(name);
			delete_multiple(num_deletions_to_do);
			break;

		case 'c':  // check the number of links related to the input
			count = check_num_links(name);
			if (count != -1) {
				printf("number of links : %d\n", count);
			}
			break;

		default:
			printf("wrong command\n");
		}// switch.
	} // while(1)
	return 0;
} // end of main.

