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

typedef struct { //距离结构体
	data_struct d;//数据节点
	double dist;//距离
} dist_struct;

#define Type dist_struct

/*定义优先队列的数据结构*/
typedef struct HeapStruct {
	int capacity; //最大容量
	int size;//已存大小
	Type *eles;//内容
} PriorityQueue;

//初始化优先队列 ,创建空堆，初始化元素为0
PriorityQueue *init(int max_size) { //这里初始化init定义为指针函数，其返回值是指针，可以直接赋值给*pq
	PriorityQueue *pq = NULL;

	//检查输入大小的合法性
	if (max_size <= 0) {
		printf("max_size is illeagel!\n");
		return NULL;
	}
	pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));

	if (pq == NULL) {
		printf("malloc failed!\n");
		return NULL;
	}
	//下标为0的位置保留，不使用
	pq->eles = (Type *)malloc((max_size + 1) * sizeof(Type));
	if (pq->eles == NULL) {
		printf("eles malloc failed!\n");
		free(pq);
		return NULL;
	}

	//初始化队列成员
	memset(pq->eles, 0, (max_size + 1)*sizeof(Type));
	pq->capacity = max_size;
	pq->size = 0;

	return pq;
}

//判断是否为空
int my_is_empty(PriorityQueue *pq) {
	if (pq == NULL)
		return 0;
	if (pq->size == 0)
		return 1;
	else
		return 0;
}

//判断是否为满
int is_full(PriorityQueue *pq) {
	if (pq == NULL)
		return 0;
	if (pq->size == pq->capacity)
		return 1;
	else
		return 0;
}

//插入
int push(PriorityQueue *pq, Type value) {
	int i = 0;

	//首先判断队列是否为满
	if (is_full(pq)) {
		printf("Queue is Full!\n");
		return 0;
	}

	//将最后位置+1,作为待插入的空穴，不断将空穴与父节点比较，如果比父节点大，则空穴上移
	for (i = pq->size + 1; value.dist > pq->eles[i / 2].dist && i > 1; i /= 2) {
		pq->eles[i] = pq->eles[i / 2];
	}
	pq->eles[i] = value; //将value放到最后的空穴中
	pq->size ++;
	return 1;
}

//查找最值
Type find_max(PriorityQueue *pq) {
	if (my_is_empty(pq)) {
		printf("Queue is empty\n");
	}
	return pq->eles[1];
}

//删除
Type pop(PriorityQueue *pq) {
	int i = 1;
	int maxchild = 0;

	//判断是否为空
	if (my_is_empty(pq)) {
		printf("Queue is empty!\n");
	}
	//先把最后一个元素取出来
	Type last;
	last = pq->eles[pq->size];

	Type max;
	//取得最大值
	max = pq->eles[1];
	pq->size--;
	if (pq->size == 0) { //如果只有一个元素
		return max;
	}

	//将第一个元素作为空穴，并不断下移
	for (i = 1; i * 2 < pq->size; i = maxchild) {
		maxchild = i * 2;
		//选择两个孩子中的最大值
		if (pq->eles[maxchild].dist < pq->eles[maxchild + 1].dist && maxchild != pq->size)
			maxchild += 1;

		//如果last比空穴处的元素小，则空穴还要往下
		if (last.dist < pq->eles[maxchild].dist)
			pq->eles[i] = pq->eles[maxchild];
		else
			break;
	}
	pq->eles[i] = last;
	return max;
}

//摧毁
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
	data_struct split_data;//数据结点
	int split;//分裂维
	struct kd_node *left;//由位于该结点分割超面左子空间内所有数据点构成的kd-tree
	struct kd_node *right;//由位于该结点分割超面右子空间内所有数据点构成的kd-tree
} kd_struct;


//排序函数，快速排序
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
		T = (kd_struct *)malloc(sizeof(kd_struct));//分割节点
		for (int j = 0; j < 62; j++) {
			T->split_data.data[j] = split_data.data[j];
		}
		T->split_data.specie = split_data.specie;
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
	return sqrt(temp);
}

//搜索1近邻
void search_nearest(
    kd_struct *T, int size, data_struct test,

    PriorityQueue *pq) {
	int path_size;//搜索路径内的指针数目
	kd_struct *search_path[size];//搜索路径保存各结点的指针
	kd_struct *psearch = T;
	search_path[0] = psearch; //初始化搜索路径
	path_size = 1;
	while (psearch->left != NULL || psearch->right != NULL) {
		if (test.data[psearch->split] < psearch->split_data.data[psearch->split] && psearch->left != NULL) {
			psearch = psearch->left;//如果小于就进入左子树
		} else if (psearch->right != NULL) {//如果大于就进入右子树
			psearch = psearch->right;
		}
		search_path[path_size++] = psearch; //将经过的分裂结点保存在搜索路径中
	}
	//取出search_path最后一个元素，即叶子结点赋给nearest
	dist_struct near;
	near.dist = compute_distance(search_path[path_size - 1]->split_data, test);
	for (int j = 0; j < 62; j++) {
		near.d.data[j] = search_path[path_size - 1]->split_data.data[j];
	}
	near.d.specie = search_path[path_size - 1]->split_data.specie;
	push(pq, near); //加入优先队列
	path_size--;//search_path的指针数减一

	//回溯搜索路径
	kd_struct *pback;//回溯指针
	while (path_size != 0) {
		pback = search_path[path_size - 1]; //取出search_path最后一个结点赋给pback
		path_size--;//search_path的指针数减一

		if (pback->left == NULL && pback->right == NULL) { //如果pback为叶子结点
			if (!is_full(pq)) {//队列没满就直接加入
				near.d = pback->split_data;
				near.dist = compute_distance(pback->split_data, test);
				push(pq, near);
			} else if (find_max(pq).dist > compute_distance(pback->split_data, test)) {
				//和已有的k近邻的最大距离比较
				near.d = pback->split_data;
				near.dist = compute_distance(pback->split_data, test);
				pop(pq);
				push(pq, near);
			}
		} else { //如果pback为分裂结点
			int s = pback->split;//s是分割的维度
			if (fabs(pback->split_data.data[s] - test.data[s]) < find_max(pq).dist) {
				/*若以查询点为中心的圆(球或超球)，半径为dist的圆
				与分割超平面相交，那么就要跳到另一边的子空间去搜索*/
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
					//若查询点位于pback的左子空间，那么就要跳到右子空间去搜索
					psearch = pback->right;
				else
					psearch = pback->left;
				//若以查询点位于pback的右子空间，那么就要跳到左子空间去搜索
				if (psearch != NULL)
					search_path[path_size++] = psearch; //psearch加入到search_path中
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
	kd_struct *root = NULL;
	root = build_kdtree(train_set, count_train, 62, root);

	double accuracy = 0;
	for (int k = 1; k < 11; k++) {
		for (int i = 0; i < count_test; i++) {
			PriorityQueue *pq = init(k); //init在定义时，定义为指针，因此其返回值是个指针，直接复制给pq
			search_nearest(root, count_train, test_set[i], pq);
			int vote[3] = {0, 0, 0}; //统计k近邻的种类
			while (!my_is_empty(pq)) {
				dist_struct value = pop(pq);
				vote[(int)value.d.specie]++;
			}
			int max = -1;
			int maxcount = 0;
			for (int j = 0; j < 3; j++) { //多数表决
				if (vote[j] > maxcount) {
					maxcount = vote[j];
					max = j;
				}
			}
			if (max == test_set[i].specie) {
				accuracy += 1;
			}
			destroy(pq);//销毁队列
		}
		accuracy /= count_test;
		printf("k值：%d accuracy:%.5f \n", k, accuracy);
	}
	return 0;
}
