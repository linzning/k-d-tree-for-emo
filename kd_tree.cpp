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

typedef struct kd_node {
	data_struct split_data;//���ݽ��
	int split;//����ά
	struct kd_node *left;//��λ�ڸý��ָ�����ӿռ����������ݵ㹹�ɵ�kd-tree
	struct kd_node *right;//��λ�ڸý��ָ�����ӿռ����������ݵ㹹�ɵ�kd-tree
} kd_struct;


/*//��������
int cmp1( const void *a, const void *b ) {
	return (*(data_struct *)a).x > (*(data_struct *)b).x ? 1 : -1;
}

//��������
int cmp2( const void *a, const void *b ) {
	return (*(data_struct *)a).y > (*(data_struct *)b).y ? 1 : -1;
}*/


//����������������
void my_qsort(data_struct *arr, int begin, int end, int k) {
	//data���������飬k���ݵ�kά��������
	//222222�ṹ�帴��
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
	/*if ((*split) == 0)
		qsort(data_temp, size, sizeof(data_temp[0]), cmp1);
	else
		qsort(data_temp, size, sizeof(data_temp[0]), cmp2);*/
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
	/*if (a.x == b.x && a.y == b.y)
		return 1;
	else
		return 0;*/
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

		/*if (split == 0) { //xά
			for (i = 0; i < size; ++i) {
				if (!equal(data_set[i], split_data) && data_set[i].x <= split_data.x) { //�ȷ��ѽ��С
					data_left[sizeleft].x = data_set[i].x;
					data_left[sizeleft].y = data_set[i].y;
					sizeleft++;//λ�ڷ��ѽ������ӿռ�Ľ����
				} else if (!equal(data_set[i], split_data) && data_set[i].x > split_data.x) { //�ȷ��ѽ���
					data_right[sizeright].x = data_set[i].x;
					data_right[sizeright].y = data_set[i].y;
					sizeright++;//λ�ڷ��ѽ������ӿռ�Ľ����
				}
			}
		} else { //yά
			for (i = 0; i < size; ++i) {
				if (!equal(data_set[i], split_data) && data_set[i].y <= split_data.y) {
					data_left[sizeleft].x = data_set[i].x;
					data_left[sizeleft].y = data_set[i].y;
					sizeleft++;
				} else if (!equal(data_set[i], split_data) && data_set[i].y > split_data.y) {
					data_right[sizeright].x = data_set[i].x;
					data_right[sizeright].y = data_set[i].y;
					sizeright++;
				}
			}
		}*/
		T = (kd_struct *)malloc(sizeof(kd_struct));//�ָ�ڵ�
		for (int j = 0; j < 62; j++) {
			T->split_data.data[j] = split_data.data[j];
		}
		T->split_data.specie = split_data.specie;
		/*T->split_data.x = split_data.x;
		T->split_data.y = split_data.y;*/
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
	//double tmp = pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0);
	return sqrt(temp);
}

//����1����
void search_nearest(kd_struct *T, int size, data_struct test, data_struct *nearest_point, double *distance) {
	int path_size;//����·���ڵ�ָ����Ŀ
	kd_struct *search_path[size];//����·�����������ָ��
	kd_struct *psearch = T;
	data_struct nearest;//����ڵĽ��
	double dist;//��ѯ���������ڽ��ľ���
	search_path[0] = psearch; //��ʼ������·��
	path_size = 1;
	while (psearch->left != NULL || psearch->right != NULL) {
		if (test.data[psearch->split] < psearch->split_data.data[psearch->split] && psearch->left != NULL) {
			psearch = psearch->left;
		} else if (psearch->right != NULL) {
			psearch = psearch->right;
		}
		/*if (psearch->split == 0) {
			if (test.x <= psearch->split_data.x) //���С�ھͽ���������
				psearch = psearch->left;
			else
				psearch = psearch->right;
		} else {
			if (test.y <= psearch->split_data.y) //���С�ھͽ���������
				psearch = psearch->left;
			else
				psearch = psearch->right;
		}*/
		search_path[path_size++] = psearch; //�������ķ��ѽ�㱣��������·����
	}
	//ȡ��search_path���һ��Ԫ�أ���Ҷ�ӽ�㸳��nearest
	//nearest = search_path[path_size - 1]->split_data;
	for (int j = 0; j < 62; j++) {
		nearest.data[j] = search_path[path_size - 1]->split_data.data[j];
	}
	nearest.specie = search_path[path_size - 1]->split_data.specie;
	/*nearest.x = search_path[path_size - 1]->split_data.x;
	nearest.y = search_path[path_size - 1]->split_data.y;*/
	path_size--;//search_path��ָ������һ
	dist = compute_distance(nearest, test); //�������Ҷ�ӽ��ľ�����Ϊ��ʼ����

	//��������·��
	kd_struct *pback;
	while (path_size != 0) {
		pback = search_path[path_size - 1]; //ȡ��search_path���һ����㸳��pback
		path_size--;//search_path��ָ������һ

		if (pback->left == NULL && pback->right == NULL) { //���pbackΪҶ�ӽ��
			if (dist > compute_distance(pback->split_data, test)) {
				nearest = pback->split_data;
				dist = compute_distance(pback->split_data, test);
			}
		} else { //���pbackΪ���ѽ��
			int s = pback->split;//s�Ƿָ��ά��
			if (fabs(pback->split_data.data[s] - test.data[s]) < dist) {
				/*���Բ�ѯ��Ϊ���ĵ�Բ(�����)���뾶Ϊdist��Բ
				��ָƽ���ཻ����ô��Ҫ������һ�ߵ��ӿռ�ȥ����*/
				if (dist > compute_distance(pback->split_data, test)) {
					nearest = pback->split_data;
					dist = compute_distance(pback->split_data, test);
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
			/*
			if (s == 0) { //xά
				if (fabs(pback->split_data.x - test.x) <
				        dist) { //���Բ�ѯ��Ϊ���ĵ�Բ(�����)���뾶Ϊdist��Բ��ָƽ���ཻ����ô��Ҫ������һ�ߵ��ӿռ�ȥ����
					if (dist > compute_distance(pback->split_data, test)) {
						nearest = pback->split_data;
						dist = compute_distance(pback->split_data, test);
					}
					if (test.x <= pback->split_data.x) //����ѯ��λ��pback�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����
						psearch = pback->right;
					else
						psearch = pback->left; //���Բ�ѯ��λ��pback�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����
					if (psearch != NULL)
						search_path[path_size++] = psearch; //psearch���뵽search_path��
				}
			} else { //yά
				if (fabs(pback->split_data.y - test.y) <
				        dist) { //���Բ�ѯ��Ϊ���ĵ�Բ(�����)���뾶Ϊdist��Բ��ָƽ���ཻ����ô��Ҫ������һ�ߵ��ӿռ�ȥ����
					if (dist > compute_distance(pback->split_data, test)) {
						nearest = pback->split_data;
						dist = compute_distance(pback->split_data, test);
					}
					if (test.y <= pback->split_data.y) //����ѯ��λ��pback�����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����
						psearch = pback->right;
					else
						psearch = pback->left; //����ѯ��λ��pback�ĵ����ӿռ䣬��ô��Ҫ�������ӿռ�ȥ����
					if (psearch != NULL)
						search_path[path_size++] = psearch; //psearch���뵽search_path��
				}
			}*/
		}
	}
	for (int j = 0; j < 62; j++) {
		nearest_point->data[j] = nearest.data[j];
	}
	nearest_point->specie = nearest.specie;
	/*(*nearest_point).x = nearest.x; //�����
	(*nearest_point).y = nearest.y;*/
	*distance = dist; //����
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
	/*for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 62; j++) {
			printf("%lf\n", test_set[i].data[j]);
		}
	}*/
	data_struct nearest_point;
	double distance;
	kd_struct *root = NULL;
	root = build_kdtree(train_set, count_train, 62, root);

	/*printf("%d %lf\n", root->split, root->split_data.data[0]);
	printf("%d %lf\n", root->left->split, root->left->split_data.data[0]);
	printf("%d %lf\n", root->right->split, root->right->split_data.data[0]);*/
	double accuracy = 0;
	for (int i = 0; i < count_test; i++) {
		search_nearest(root, count_train, test_set[i], &nearest_point, &distance);
		if ((int)(nearest_point.specie) == (int)(test_set[i].specie)) {
			accuracy += 1;
		}
	}
	accuracy /= count_test;
	printf("accuracy:%.5f \n", accuracy);
	return 0;
}
