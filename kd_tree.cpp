#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
using namespace std;


typedef struct { //数据维度
	double data[62];//特征
	double specie;//类别
} data_struct;

typedef struct kd_node {
	data_struct split_data;//数据结点
	int split;//分裂维
	struct kd_node *left;//由位于该结点分割超面左子空间内所有数据点构成的kd-tree
	struct kd_node *right;//由位于该结点分割超面右子空间内所有数据点构成的kd-tree
} kd_struct;


/*//用于排序
int cmp1( const void *a, const void *b ) {
	return (*(data_struct *)a).x > (*(data_struct *)b).x ? 1 : -1;
}

//用于排序
int cmp2( const void *a, const void *b ) {
	return (*(data_struct *)a).y > (*(data_struct *)b).y ? 1 : -1;
}*/


//排序函数，快速排序
void my_qsort(data_struct *arr, int begin, int end, int k) {
	//data待排序数组，k根据第k维数据排序
	//222222结构体复制
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


//计算分裂维和分裂结点
void choose_split(data_struct data_set[], int size, int dimension, int *split, data_struct *split_data) {
	int i;
	data_struct *data_temp;
	data_temp = (data_struct *)malloc(size * sizeof(data_struct));
	for (i = 0; i < size; i++)//数据拷贝到临时数组
		data_temp[i] = data_set[i];
	static int count = 0; //设为静态，只初始化一次
	*split = (count++) % dimension; //分裂维
	my_qsort(data_temp, 0, size - 1, *split);
	/*if ((*split) == 0)
		qsort(data_temp, size, sizeof(data_temp[0]), cmp1);
	else
		qsort(data_temp, size, sizeof(data_temp[0]), cmp2);*/
	*split_data = data_temp[(size - 1) / 2]; //分裂结点排在中位
}

//判断两个数据点是否相等
int equal(data_struct a, data_struct b) {
	int ans = 1; //1代表相等
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

//建立KD树
kd_struct *build_kdtree(data_struct data_set[], int size, int dimension, kd_struct *T) {
	if (size == 0)
		return NULL;//递归出口
	else {
		int sizeleft = 0, sizeright = 0;
		int i, split;//分割维
		data_struct split_data;//分割数据节点
		choose_split(data_set, size, dimension, &split, &split_data);
		data_struct data_right[size];
		data_struct data_left[size];

		for (i = 0; i < size; i++) {
			if (!equal(data_set[i], split_data) && data_set[i].data[split] <= split_data.data[split]) { //比分割节点小
				for (int j = 0; j < 62; j++) {
					data_left[sizeleft].data[j] = data_set[i].data[j];
				}
				data_left[sizeleft].specie = data_set[i].specie;
				sizeleft++;
			} else if (!equal(data_set[i], split_data) && data_set[i].data[split] > split_data.data[split]) { //比分割节点大
				for (int j = 0; j < 62; j++) {
					data_right[sizeright].data[j] = data_set[i].data[j];
				}
				data_right[sizeright].specie = data_set[i].specie;
				sizeright++;
			}
		}

		/*if (split == 0) { //x维
			for (i = 0; i < size; ++i) {
				if (!equal(data_set[i], split_data) && data_set[i].x <= split_data.x) { //比分裂结点小
					data_left[sizeleft].x = data_set[i].x;
					data_left[sizeleft].y = data_set[i].y;
					sizeleft++;//位于分裂结点的左子空间的结点数
				} else if (!equal(data_set[i], split_data) && data_set[i].x > split_data.x) { //比分裂结点大
					data_right[sizeright].x = data_set[i].x;
					data_right[sizeright].y = data_set[i].y;
					sizeright++;//位于分裂结点的右子空间的结点数
				}
			}
		} else { //y维
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
		T = (kd_struct *)malloc(sizeof(kd_struct));//分割节点
		for (int j = 0; j < 62; j++) {
			T->split_data.data[j] = split_data.data[j];
		}
		T->split_data.specie = split_data.specie;
		/*T->split_data.x = split_data.x;
		T->split_data.y = split_data.y;*/
		T->split = split;
		T->left = build_kdtree(data_left, sizeleft, dimension, T->left); //左子空间
		T->right = build_kdtree(data_right, sizeright, dimension, T->right); //右子空间
		return T;//返回指针
	}
}

//计算欧氏距离
double compute_distance(data_struct a, data_struct b) {
	double temp = 0;
	for (int i = 0; i < 62; i++) {
		temp += pow(a.data[i] - b.data[i], 2.0);
	}
	//double tmp = pow(a.x - b.x, 2.0) + pow(a.y - b.y, 2.0);
	return sqrt(temp);
}

//搜索1近邻
void search_nearest(kd_struct *T, int size, data_struct test, data_struct *nearest_point, double *distance) {
	int path_size;//搜索路径内的指针数目
	kd_struct *search_path[size];//搜索路径保存各结点的指针
	kd_struct *psearch = T;
	data_struct nearest;//最近邻的结点
	double dist;//查询结点与最近邻结点的距离
	search_path[0] = psearch; //初始化搜索路径
	path_size = 1;
	while (psearch->left != NULL || psearch->right != NULL) {
		if (test.data[psearch->split] < psearch->split_data.data[psearch->split] && psearch->left != NULL) {
			psearch = psearch->left;
		} else if (psearch->right != NULL) {
			psearch = psearch->right;
		}
		/*if (psearch->split == 0) {
			if (test.x <= psearch->split_data.x) //如果小于就进入左子树
				psearch = psearch->left;
			else
				psearch = psearch->right;
		} else {
			if (test.y <= psearch->split_data.y) //如果小于就进入右子树
				psearch = psearch->left;
			else
				psearch = psearch->right;
		}*/
		search_path[path_size++] = psearch; //将经过的分裂结点保存在搜索路径中
	}
	//取出search_path最后一个元素，即叶子结点赋给nearest
	//nearest = search_path[path_size - 1]->split_data;
	for (int j = 0; j < 62; j++) {
		nearest.data[j] = search_path[path_size - 1]->split_data.data[j];
	}
	nearest.specie = search_path[path_size - 1]->split_data.specie;
	/*nearest.x = search_path[path_size - 1]->split_data.x;
	nearest.y = search_path[path_size - 1]->split_data.y;*/
	path_size--;//search_path的指针数减一
	dist = compute_distance(nearest, test); //计算与该叶子结点的距离作为初始距离

	//回溯搜索路径
	kd_struct *pback;
	while (path_size != 0) {
		pback = search_path[path_size - 1]; //取出search_path最后一个结点赋给pback
		path_size--;//search_path的指针数减一

		if (pback->left == NULL && pback->right == NULL) { //如果pback为叶子结点
			if (dist > compute_distance(pback->split_data, test)) {
				nearest = pback->split_data;
				dist = compute_distance(pback->split_data, test);
			}
		} else { //如果pback为分裂结点
			int s = pback->split;//s是分割的维度
			if (fabs(pback->split_data.data[s] - test.data[s]) < dist) {
				/*若以查询点为中心的圆(球或超球)，半径为dist的圆
				与分割超平面相交，那么就要跳到另一边的子空间去搜索*/
				if (dist > compute_distance(pback->split_data, test)) {
					nearest = pback->split_data;
					dist = compute_distance(pback->split_data, test);
				}
				if (test.data[s] <= pback->split_data.data[s])
					//若查询点位于pback的左子空间，那么就要跳到右子空间去搜索
					psearch = pback->right;
				else
					psearch = pback->left;
				//若以查询点位于pback的右子空间，那么就要跳到左子空间去搜索
				if (psearch != NULL)
					search_path[path_size++] = psearch; //psearch加入到search_path中
			}
			/*
			if (s == 0) { //x维
				if (fabs(pback->split_data.x - test.x) <
				        dist) { //若以查询点为中心的圆(球或超球)，半径为dist的圆与分割超平面相交，那么就要跳到另一边的子空间去搜索
					if (dist > compute_distance(pback->split_data, test)) {
						nearest = pback->split_data;
						dist = compute_distance(pback->split_data, test);
					}
					if (test.x <= pback->split_data.x) //若查询点位于pback的左子空间，那么就要跳到右子空间去搜索
						psearch = pback->right;
					else
						psearch = pback->left; //若以查询点位于pback的右子空间，那么就要跳到左子空间去搜索
					if (psearch != NULL)
						search_path[path_size++] = psearch; //psearch加入到search_path中
				}
			} else { //y维
				if (fabs(pback->split_data.y - test.y) <
				        dist) { //若以查询点为中心的圆(球或超球)，半径为dist的圆与分割超平面相交，那么就要跳到另一边的子空间去搜索
					if (dist > compute_distance(pback->split_data, test)) {
						nearest = pback->split_data;
						dist = compute_distance(pback->split_data, test);
					}
					if (test.y <= pback->split_data.y) //若查询点位于pback的左子空间，那么就要跳到右子空间去搜索
						psearch = pback->right;
					else
						psearch = pback->left; //若查询点位于pback的的右子空间，那么就要跳到左子空间去搜索
					if (psearch != NULL)
						search_path[path_size++] = psearch; //psearch加入到search_path中
				}
			}*/
		}
	}
	for (int j = 0; j < 62; j++) {
		nearest_point->data[j] = nearest.data[j];
	}
	nearest_point->specie = nearest.specie;
	/*(*nearest_point).x = nearest.x; //最近邻
	(*nearest_point).y = nearest.y;*/
	*distance = dist; //距离
}

int main() {
	data_struct train_set[350];
	data_struct test_set[150];
	int count_train = 0;
	int count_test = 0;
	FILE *fp = fopen("train_emtion.txt", "r");

	if (fp == NULL) {
		printf("文件打开时发生错误");
		return -1;
	}

	//读取
	while (!feof(fp)) {
		for (int i = 0; i < 62; i++) {
			fscanf(fp, "%lf ", &train_set[count_train].data[i]);
		}
		fscanf(fp, "%lf ", &train_set[count_train].specie);
		count_train++;
	}
	fclose(fp);

	//测试集
	fp = fopen("test_emtion.txt", "r");

	if (fp == NULL) {
		printf("文件打开时发生错误");
		return -1;
	}

	//读取
	while (!feof(fp)) {
		for (int i = 0; i < 62; i++) {
			fscanf(fp, "%lf ", &test_set[count_test].data[i]);
		}
		fscanf(fp, "%lf ", &test_set[count_test].specie);
		count_test++;
	}
	//预处理
	double sum[62] = {0};
	double average[62] = {0};
	double deviation[62] = {0};
	for (int j = 0; j < 62; j++) {

		for (int i = 0; i < count_train; i++) {
			sum[j] += train_set[i].data[j];
		}
	}
	//均值
	for (int i = 0; i < 62; i++) {
		average[i] = sum[i] / count_train;
	}
	double SUM[62] = {0};
	for (int j = 0; j < 62; j++) {
		for (int i = 0; i < count_train; i++)
			SUM[j] += pow(train_set[i].data[j] - average[j], 2) / (count_train - 1);
	}
	//标准差
	for (int i = 0; i < 62; i++) {
		deviation[i] = pow(SUM[i], 0.5);
	}
	//训练集标准化
	for (int i = 0; i < count_train; i++) {
		for (int j = 0; j < 62; j++)
			train_set[i].data[j] = (train_set[i].data[j] - average[j]) / deviation[j];
	}
	//测试集标准化，分开处理
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
