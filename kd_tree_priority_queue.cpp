#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;


typedef struct { //����ά��
	double data[62];//����
	double specie;//���
} data_struct;

typedef struct { //����ṹ��
	data_struct d;//���ݽڵ�
	double dist;//����
} dist_struct;

#define Type dist_struct

/*�������ȶ��е����ݽṹ*/
typedef struct HeapStruct {
	int capacity; //�������
	int size;//�Ѵ��С
	Type *eles;//����
} PriorityQueue;

//��ʼ�����ȶ��� ,�����նѣ���ʼ��Ԫ��Ϊ0
PriorityQueue *init(int max_size) { //�����ʼ��init����Ϊָ�뺯�����䷵��ֵ��ָ�룬����ֱ�Ӹ�ֵ��*pq
	PriorityQueue *pq = NULL;

	//��������С�ĺϷ���
	if (max_size <= 0) {
		printf("max_size is illeagel!\n");
		return NULL;
	}
	pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));

	if (pq == NULL) {
		printf("malloc failed!\n");
		return NULL;
	}
	//�±�Ϊ0��λ�ñ�������ʹ��
	pq->eles = (Type *)malloc((max_size + 1) * sizeof(Type));
	if (pq->eles == NULL) {
		printf("eles malloc failed!\n");
		free(pq);
		return NULL;
	}

	//��ʼ�����г�Ա
	memset(pq->eles, 0, (max_size + 1)*sizeof(Type));
	pq->capacity = max_size;
	pq->size = 0;

	return pq;
}

//�ж��Ƿ�Ϊ��
int my_is_empty(PriorityQueue *pq) {
	if (pq == NULL)
		return 0;
	if (pq->size == 0)
		return 1;
	else
		return 0;
}

//�ж��Ƿ�Ϊ��
int is_full(PriorityQueue *pq) {
	if (pq == NULL)
		return 0;
	if (pq->size == pq->capacity)
		return 1;
	else
		return 0;
}

//����
int push(PriorityQueue *pq, Type value) {
	int i = 0;

	//�����ж϶����Ƿ�Ϊ��
	if (is_full(pq)) {
		printf("Queue is Full!\n");
		return 0;
	}

	//�����λ��+1,��Ϊ������Ŀ�Ѩ�����Ͻ���Ѩ�븸�ڵ�Ƚϣ�����ȸ��ڵ�����Ѩ����
	for (i = pq->size + 1; value.dist > pq->eles[i / 2].dist && i > 1; i /= 2) {
		pq->eles[i] = pq->eles[i / 2];
	}
	pq->eles[i] = value; //��value�ŵ����Ŀ�Ѩ��
	pq->size ++;
	return 1;
}

//������ֵ
Type find_max(PriorityQueue *pq) {
	if (my_is_empty(pq)) {
		printf("Queue is empty\n");
	}
	return pq->eles[1];
}

//ɾ��
Type pop(PriorityQueue *pq) {
	int i = 1;
	int maxchild = 0;

	//�ж��Ƿ�Ϊ��
	if (my_is_empty(pq)) {
		printf("Queue is empty!\n");
	}
	//�Ȱ����һ��Ԫ��ȡ����
	Type last;
	last = pq->eles[pq->size];

	Type max;
	//ȡ�����ֵ
	max = pq->eles[1];
	pq->size--;
	if (pq->size == 0) { //���ֻ��һ��Ԫ��
		return max;
	}

	//����һ��Ԫ����Ϊ��Ѩ������������
	for (i = 1; i * 2 < pq->size; i = maxchild) {
		maxchild = i * 2;
		//ѡ�����������е����ֵ
		if (pq->eles[maxchild].dist < pq->eles[maxchild + 1].dist && maxchild != pq->size)
			maxchild += 1;

		//���last�ȿ�Ѩ����Ԫ��С�����Ѩ��Ҫ����
		if (last.dist < pq->eles[maxchild].dist)
			pq->eles[i] = pq->eles[maxchild];
		else
			break;
	}
	pq->eles[i] = last;
	return max;
}

//�ݻ�
int destroy(PriorityQueue *pq) {
	if (pq == NULL) {
		return 0;
	}
	free(pq->eles);
	pq->eles = NULL;
	free(pq);
	pq = NULL;
	return 1;
}

typedef struct kd_node {
	data_struct split_data;//���ݽ��
	int split;//����ά
	struct kd_node *left;//��λ�ڸý��ָ�����ӿռ����������ݵ㹹�ɵ�kd-tree
	struct kd_node *right;//��λ�ڸý��ָ�����ӿռ����������ݵ㹹�ɵ�kd-tree
} kd_struct;


//����������������
void my_qsort(data_struct *arr, int begin, int end, int k) {
	if (begin > end)
		return;
	data_struct tmp_struct = arr[begin];
	double tmp = arr[begin].data[k];
	int i = begin;
	int j = end;
	while (i != j) {
		while (arr[j].data[k] >= tmp && j > i)
			j--;
		while (arr[i].data[k] <= tmp && j > i)
			i++;
		if (j > i) {
			data_struct t = arr[i];
			arr[i] = arr[j];
			arr[j] = t;
		}
	}
	arr[begin] = arr[i];
	arr[i] = tmp_struct;
	my_qsort(arr, begin, i - 1, k);
	my_qsort(arr, i + 1, end, k);
}


//�������ά�ͷ��ѽ��
void choose_split(data_struct data_set[], int size, int dimension, int *split, data_struct *split_data) {
	int i;
	data_struct *data_temp;
	data_temp = (data_struct *)malloc(size * sizeof(data_struct));
	for (i = 0; i < size; i++)//���ݿ�������ʱ����
		data_temp[i] = data_set[i];
	static int count = 0; //��Ϊ��̬��ֻ��ʼ��һ��
	*split = (count++) % dimension; //����ά
	my_qsort(data_temp, 0, size - 1, *split);
	*split_data = data_temp[(size - 1) / 2]; //���ѽ��������λ
}

//�ж��������ݵ��Ƿ����
int equal(data_struct a, data_struct b) {
	int ans = 1; //1�������
	for (int i = 0; i < 62; i++) {
		if (a.data[i] != b.data[i]) {
			ans = 0;
			break;
		}
	}
	if (a.specie != b.specie) {
		ans = 0;
	}
	return ans;
}

//����KD��
kd_struct *build_kdtree(data_struct data_set[], int size, int dimension, kd_struct *T) {
	if (size == 0)
		return NULL;//�ݹ����
	else {
		int sizeleft = 0, sizeright = 0;
		int i, split;//�ָ�ά
		data_struct split_data;//�ָ����ݽڵ�
		choose_split(data_set, size, dimension, &split, &split_data);
		data_struct data_right[size];
		data_struct data_left[size];

		for (i = 0; i < size; i++) {
			if (!equal(data_set[i], split_data) && data_set[i].data[split] <= split_data.data[split]) { //�ȷָ�ڵ�С
				for (int j = 0; j < 62; j++) {
					data_left[sizeleft].data[j] = data_set[i].data[j];
				}
				data_left[sizeleft].specie = data_set[i].specie;
				sizeleft++;
			} else if (!equal(data_set[i], split_data) && data_set[i].data[split] > split_data.data[split]) { //�ȷָ�ڵ��
				for (int j = 0; j < 62; j++) {
					data_right[sizeright].data[j] = data_set[i].data[j];
				}
				data_right[sizeright].specie = data_set[i].specie;
				sizeright++;
			}
		}
		T = (kd_struct *)malloc(sizeof(kd_struct));//�ָ�ڵ�
		for (int j = 0; j < 62; j++) {
			T->split_data.data[j] = split_data.data[j];
		}
		T->split_data.specie = split_data.specie;
		T->split = split;
		T->left = build_kdtree(data_left, sizeleft, dimension, T->left); //���ӿռ�
		T->right = build_kdtree(data_right, sizeright, dimension, T->right); //���ӿռ�
		return T;//����ָ��
	}
}

//����ŷ�Ͼ���
double compute_distance(data_struct a, data_struct b) {
	double temp = 0;
	for (int i = 0; i < 62; i++) {
		temp += pow(a.data[i] - b.data[i], 2.0);
	}
	return sqrt(temp);
}

//����1����
void search_nearest(
    kd_struct *T, int size, data_struct test,

    PriorityQueue *pq) {
	int path_size;//����·���ڵ�ָ����Ŀ
	kd_struct *search_path[size];//����·�����������ָ��
	kd_struct *psearch = T;
	search_path[0] = psearch; //��ʼ������·��
	path_size = 1;
	while (psearch->left != NULL || psearch->right != NULL) {
		if (test.data[psearch->split] < psearch->split_data.data[psearch->split] && psearch->left != NULL) {
			psearch = psearch->left;//���С�ھͽ���������
		} else if (psearch->right != NULL) {//������ھͽ���������
			psearch = psearch->right;
		}
		search_path[path_size++] = psearch; //�������ķ��ѽ�㱣��������·����
	}
	//ȡ��search_path���һ��Ԫ�أ���Ҷ�ӽ�㸳��nearest
	dist_struct near;
	near.dist = compute_distance(search_path[path_size - 1]->split_data, test);
	for (int j = 0; j < 62; j++) {
		near.d.data[j] = search_path[path_size - 1]->split_data.data[j];
	}
	near.d.specie = search_path[path_size - 1]->split_data.specie;
	push(pq, near); //�������ȶ���
	path_size--;//search_path��ָ������һ

	//��������·��
	kd_struct *pback;//����ָ��
	while (path_size != 0) {
		pback = search_path[path_size - 1]; //ȡ��search_path���һ����㸳��pback
		path_size--;//search_path��ָ������һ

		if (pback->left == NULL && pback->right == NULL) { //���pbackΪҶ�ӽ��
			if (!is_full(pq)) {//����û����ֱ�Ӽ���
				near.d = pback->split_data;
				near.dist = compute_distance(pback->split_data, test);
				push(pq, near);
			} else if (find_max(pq).dist > compute_distance(pback->split_data, test)) {
				//�����е�k���ڵ�������Ƚ�
				near.d = pback->split_data;
				near.dist = compute_distance(pback->split_data, test);
				pop(pq);
				push(pq, near);
			}
		} else { //���pbackΪ���ѽ��
			int s = pback->split;//s�Ƿָ��ά��
			if (fabs(pback->split_data.data[s] - test.data[s]) < find_max(pq).dist) {
				/*���Բ�ѯ��Ϊ���ĵ�Բ(�����)���뾶Ϊdist��Բ
				��ָƽ���ཻ����ô��Ҫ������һ�ߵ��ӿռ�ȥ����*/
				if (!is_full(pq)) {
					near.d = pback->split_data;
					near.dist = compute_distance(pback->split_data, test);
					push(pq, near);
				} else if (find_max(pq).dist > compute_distance(pback->split_data, test)) {
					near.d = pback->split_data;
					near.dist = compute_distance(pback->split_data, test);
					pop(pq);
					push(pq, near);
				}
				if (test.data[s] <= pback->split_data.data[s])
					//����ѯ��λ��pback�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����
					psearch = pback->right;
				else
					psearch = pback->left;
				//���Բ�ѯ��λ��pback�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����
				if (psearch != NULL)
					search_path[path_size++] = psearch; //psearch���뵽search_path��
			}
		}
	}
}

int main() {
	data_struct train_set[350];
	data_struct test_set[150];
	int count_train = 0;
	int count_test = 0;
	FILE *fp = fopen("train_emtion.txt", "r");

	if (fp == NULL) {
		printf("�ļ���ʱ��������");
		return -1;
	}

	//��ȡ
	while (!feof(fp)) {
		for (int i = 0; i < 62; i++) {
			fscanf(fp, "%lf ", &train_set[count_train].data[i]);
		}
		fscanf(fp, "%lf ", &train_set[count_train].specie);
		count_train++;
	}
	fclose(fp);

	//���Լ�
	fp = fopen("test_emtion.txt", "r");

	if (fp == NULL) {
		printf("�ļ���ʱ��������");
		return -1;
	}

	//��ȡ
	while (!feof(fp)) {
		for (int i = 0; i < 62; i++) {
			fscanf(fp, "%lf ", &test_set[count_test].data[i]);
		}
		fscanf(fp, "%lf ", &test_set[count_test].specie);
		count_test++;
	}
	//Ԥ����
	double sum[62] = {0};
	double average[62] = {0};
	double deviation[62] = {0};
	for (int j = 0; j < 62; j++) {

		for (int i = 0; i < count_train; i++) {
			sum[j] += train_set[i].data[j];
		}
	}
	//��ֵ
	for (int i = 0; i < 62; i++) {
		average[i] = sum[i] / count_train;
	}
	double SUM[62] = {0};
	for (int j = 0; j < 62; j++) {
		for (int i = 0; i < count_train; i++)
			SUM[j] += pow(train_set[i].data[j] - average[j], 2) / (count_train - 1);
	}
	//��׼��
	for (int i = 0; i < 62; i++) {
		deviation[i] = pow(SUM[i], 0.5);
	}
	//ѵ������׼��
	for (int i = 0; i < count_train; i++) {
		for (int j = 0; j < 62; j++)
			train_set[i].data[j] = (train_set[i].data[j] - average[j]) / deviation[j];
	}
	//���Լ���׼�����ֿ�����
	for (int i = 0; i < count_test; i++) {

		for (int j = 0; j < 62; j++) {
			test_set[i].data[j] = (test_set[i].data[j] - average[j]) / deviation[j];
		}
	}
	kd_struct *root = NULL;
	root = build_kdtree(train_set, count_train, 62, root);

	double accuracy = 0;
	for (int k = 1; k < 11; k++) {
		for (int i = 0; i < count_test; i++) {
			PriorityQueue *pq = init(k); //init�ڶ���ʱ������Ϊָ�룬����䷵��ֵ�Ǹ�ָ�룬ֱ�Ӹ��Ƹ�pq
			search_nearest(root, count_train, test_set[i], pq);
			int vote[3] = {0, 0, 0}; //ͳ��k���ڵ�����
			while (!my_is_empty(pq)) {
				dist_struct value = pop(pq);
				vote[(int)value.d.specie]++;
			}
			int max = -1;
			int maxcount = 0;
			for (int j = 0; j < 3; j++) { //�������
				if (vote[j] > maxcount) {
					maxcount = vote[j];
					max = j;
				}
			}
			if (max == test_set[i].specie) {
				accuracy += 1;
			}
			destroy(pq);//���ٶ���
		}
		accuracy /= count_test;
		printf("kֵ��%d accuracy:%.5f \n", k, accuracy);
	}
	return 0;
}
