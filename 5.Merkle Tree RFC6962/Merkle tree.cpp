#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
using namespace std;
typedef unsigned int uint;
typedef struct Merkle_Tree_Node
{
	struct Merkle_Tree_Node* left_child;//����
	struct Merkle_Tree_Node* right_child;//�Һ���
	struct Merkle_Tree_Node* father_node;//���ڵ�
	unsigned int level;//���ڵ����
	unsigned int hash_value;//hashֵ
	char* str;//�����Ҷ�ӽڵ㣬��û��str��ֵΪ��Ӧ�ַ���
}Merkle_Tree;
Merkle_Tree* correct_leaf;
int neibor_counter = 0;
#define Init_new_node(root, tree_level) {	\
	root = (Merkle_Tree *)malloc(sizeof(Merkle_Tree)); \
	root->left_child = NULL; \
	root->right_child = NULL; \
	root->father_node = NULL; \
	root->level = (unsigned int)tree_level; \
	root->hash_value = 0;	\
	root->str = NULL; \
	}
unsigned int sha256_string(char* str1, char* str2)//��ʱ����sha256��ϣ����
{
	unsigned int magic_number = 12345, hash = 0;
	if (str1 != NULL)
	{
		while (*str1 != '\0' && *str1 != 0)
		{
			hash = hash * magic_number + *str1;
			str1++;
		}
	}
	if (str2 != NULL)
	{
		while (*str2 != '\0' && *str2 != 0)
		{
			hash = hash * magic_number + *str2;
			str2++;
		}
	}
	return hash & 0xFFFFFFFF;

}
unsigned int sha256_int(unsigned int num1, unsigned int num2)//��ʱ����sha256��ϣ����
{
	unsigned int magic_num = 12345, hash = 0;
	hash = num1 + num2;
	hash = hash * magic_num;
	return hash ^ 0xFFFFFFFF;
}

Merkle_Tree* get_leaf_node(Merkle_Tree* root)//ȡ�����һ��Ҷ�ӽڵ�
{
	Merkle_Tree* ptr = root;
	Merkle_Tree* temp;
	if (ptr->left_child == NULL && ptr->right_child == NULL)//���Һ��Ӷ��գ���Ҷ�ӽڵ㣬����֮
	{
		return ptr;
	}
	else if (ptr->right_child == NULL && ptr->left_child != NULL)//�ҿ��󲻿գ��������ֱ��Ҷ�ӽڵ�
	{
		return get_leaf_node(ptr->left_child);
	}
	else if (ptr->right_child != NULL)//�Ҳ��գ������ұ�ֱ��Ҷ�ӽڵ�
	{
		return get_leaf_node(ptr->right_child);
	}
}
Merkle_Tree* get_correct_insert_node(Merkle_Tree* leaf_node)//����Ҷ�ӽڵ㣬���һ���������ͷ�ڵ�
{
	Merkle_Tree* ptr = leaf_node->father_node;//Ҷ�ӽڵ㲻��Ҫ���룬���ҵ��丸�ڵ�
	while (ptr->left_child != NULL && ptr->right_child != NULL && ptr->father_node != NULL)
	{
		ptr = ptr->father_node;//������ڵ����Һ��Ӷ����ˣ�˵������Ҫ����ڵ㣬���ϲ�ѯֱ��������Ҫ��Ϊֹ
	}
	if (ptr->father_node == NULL && ptr->left_child != NULL && ptr->right_child != NULL)
	{
		return NULL;//û�и��ڵ㵫�����Һ��Ӷ����ˣ�˵��Ŀǰ����������������Ҫ���룬���ؿ�ֵ
	}
	else
	{
		return ptr;//���򣬵�ǰ�ڵ����Ҫ����
	}
}

Merkle_Tree* Build_Merkle_Tree(Merkle_Tree* root, char** input_list, int length)//������ڵ㣬�����б��ַ������б����б���
{
	Merkle_Tree* new_node, * temp, * ptr;
	int count;
	if (length == 0)
	{
		cout << "Build Complete!" << endl;
		return root;
	}
	else
	{
		Init_new_node(new_node, 0);//�½�Ҷ�ӽڵ�
		new_node->str = (char*)malloc(strlen(*input_list) + 1);//Ҷ�ӽڵ��ַ�������ռ䣬+1����Ϊ������

		for (int i = 0; i < strlen(*input_list); i++)
		{
			new_node->str[i] = (*input_list)[i];//�ַ�����ֵ
		}
		new_node->str[strlen(*input_list)] = '\0';//��ֵ������
		if (root == NULL)//root��˵����ǰû��������Ҫ�½�ͷ�ڵ�
		{
			Init_new_node(root, 1);
			root->left_child = new_node;
			new_node->father_node = root;
			root->hash_value = sha256_string(root->left_child->str, root->right_child == NULL ? NULL : root->right_child->str);
			root = Build_Merkle_Tree(root, input_list + 1, length - 1);//�ݹ�����½ڵ㣬input_list+1����һ���ַ�����length-1�������������һ��
		}
		else//root��Ϊ�գ���ǰ����
		{
			//ptr������Ҷ�ӽڵ�
			ptr = get_correct_insert_node(get_leaf_node(root));//ptr�Ǵ�����ڵ�Ľڵ�

			if (ptr != NULL)//ptr��Ϊ�գ���ʾ�Ѿ��ҵ���Ҫ����ڵ�Ľڵ�
			{
				if (ptr->left_child->left_child == NULL && ptr->right_child == NULL)//ptr���ӽڵ���ӽڵ��ǿգ�˵��ptr���ӽڵ���Ҷ�ӽڵ㣬ֱ�Ӳ����Һ��Ӽ���
				{
					ptr->right_child = new_node;
					new_node->father_node = ptr;
					ptr->hash_value = sha256_string(ptr->left_child->str, ptr->right_child == NULL ? NULL : ptr->right_child->str);
				}
				else//˵������Ҷ�ӽڵ�ĸ��ڵ㣬�����е��м�ڵ�,��Ӧ�ø��м�ڵ��½��ӽڵ�
				{
					count = ptr->level - 1;//���µ�ǰ���
					Init_new_node(temp, count);
					ptr->right_child = temp;
					temp->father_node = ptr;
					ptr = ptr->right_child;
					count = ptr->level - 1;//����Ϊ��ǰ���-1
					while (count > 0)//���ݵ�ǰ����½�����
					{
						Init_new_node(temp, count);
						ptr->left_child = temp;
						temp->father_node = ptr;
						ptr = ptr->left_child;
						count = count - 1;
					}
					ptr->left_child = new_node;//����count=1������Ҷ�ӽڵ�ĸ��ڵ���һ�㣬��Ҷ�ӽڵ����
					new_node->father_node = ptr;
					ptr->hash_value = sha256_string(ptr->left_child->str, ptr->right_child == NULL ? NULL : ptr->right_child->str);
				}
			}
			else//��ǰ���ؿ�ֵ��˵���������Ѿ����ˣ�Ҫ����ͷ�ڵ㣬���ҲҪ����
			{

				temp = root;
				Init_new_node(root, temp->level + 1);//������ͷ�ڵ�
				root->left_child = temp;
				temp->father_node = root;

				Init_new_node(temp, root->level - 1);//������ͷ�ڵ��µ��Һ���
				root->right_child = temp;
				temp->father_node = root;
				ptr = root->right_child;
				count = ptr->level - 1;

				while (count > 0)//�洢����������ȣ�����ͬ����ȵ�������
				{
					Init_new_node(temp, count);
					ptr->left_child = temp;
					temp->father_node = ptr;
					ptr = ptr->left_child;
					count = count - 1;
				}
				ptr->left_child = new_node;//�ص���Ҷ�ӽڵ�ĸ��ڵ�,��Ҷ�ӽڵ㸳ֵ
				new_node->father_node = ptr;
				ptr->hash_value = sha256_string(ptr->left_child->str, ptr->right_child == NULL ? NULL : ptr->right_child->str);

			}
			//����Ҷ�ӽڵ��Ѿ��ɹ���ӣ�����pָ��ǰ�²����Ҷ�ӽڵ�ĸ��ڵ㣬��p���¶��ϸ���hashֵ
			if (ptr != root)
			{
				ptr = ptr->father_node;
				while (ptr != root)
				{
					ptr->hash_value = sha256_int(ptr->left_child->hash_value, ptr->right_child == NULL ? 0 : ptr->right_child->hash_value);
					ptr = ptr->father_node;
				}
				ptr->hash_value = sha256_int(ptr->left_child->hash_value, ptr->right_child == NULL ? 0 : ptr->right_child->hash_value);
			}
			root = Build_Merkle_Tree(root, input_list + 1, length - 1);
		}
	}
}

bool compare_str(char* s1, char* s2)
{
	int len1 = strlen(s1);
	int len2 = strlen(s2);
	if (len1 != len2)
	{
		return 0;
	}
	else
	{
		for (int i = 0; i < len1; i++)
		{
			if (*(s1 + i) != *(s2 + i))
			{
				return 0;
			}
		}
		return 1;
	}
}
void Merkle_Proof(Merkle_Tree* mt1, Merkle_Tree* mt2, bool result)//Merkleһ����֤������������merkle������
{
	if (mt1 != NULL && mt2 != NULL)
	{
		if (mt1->hash_value != mt2->hash_value)
		{
			if (mt1->left_child != NULL && mt2->left_child != NULL && mt1->left_child->hash_value != mt2->left_child->hash_value)
			{
				Merkle_Proof(mt1->left_child, mt2->left_child, result);
			}
			else if (mt1->right_child != NULL && mt2->right_child != NULL && mt1->right_child->hash_value != mt2->right_child->hash_value)
			{
				Merkle_Proof(mt1->right_child, mt2->right_child, result);
			}
			else if (mt1->left_child->left_child == NULL && mt1->left_child->right_child == NULL && mt1->right_child->left_child == NULL && mt1->right_child->right_child == NULL && mt2->left_child->left_child == NULL && mt2->left_child->right_child == NULL && mt2->right_child->left_child == NULL && mt2->right_child->right_child == NULL)
			{
				if (!compare_str(mt1->left_child->str, mt2->left_child->str))
				{
					printf("Verify failed,the words that differ are: %s and %s\n\n\n", mt1->left_child->str, mt2->left_child->str);
				}
				else if (!compare_str(mt1->right_child->str, mt2->right_child->str))
				{
					printf("Verify failed,the words that differ are:%s and %s\n\n\n", mt1->right_child->str, mt2->right_child->str);
				}
			}
		}
		else
		{
			printf("verify pass!");
			result = 1;
			return;
		}
	}
}
void Print_Merkle_Tree(Merkle_Tree* mt, int high)
{
	Merkle_Tree* p = mt;
	int i;

	if (p == NULL)
		return;

	if (p->left_child == NULL && p->right_child == NULL) {
		for (i = 0; i < high - p->level; i++)
			printf("           ");

		printf("------>%s\n", p->str);
	}
	else {
		Print_Merkle_Tree(mt->left_child, high);
		printf("\n");
		for (i = 0; i < high - p->level; i++)
			printf("           ");

		printf("------>%-6d\n", p->hash_value);

		Print_Merkle_Tree(mt->right_child, high);
	}
}
void Get_Node(Merkle_Tree* root, char* input)//ȡ�ö�Ӧstr��input��Ҷ�ӽڵ�
{
	if (root != NULL)
	{
		if (root->str != NULL)
		{
			if (compare_str(input, root->str))
			{
				correct_leaf = root;
				return;
			}
			else
			{
				return;
			}
		}
		else
		{
			Get_Node(root->left_child, input);
			Get_Node(root->right_child, input);
		}

	}
	return;
}
bool is_root(Merkle_Tree* node)//�ж��Ƿ��Ǹ��ڵ�
{
	if (node->father_node == NULL && node->left_child != NULL && node->right_child != NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
Merkle_Tree* Get_Neibor_Node(Merkle_Tree* node)//ȡ�ñ��ڵ�����ڽڵ㣬���ýڵ��Ǹ��ڵ��򷵻ؿ�ָ��
{
	if (is_root(node))
	{
		return NULL;
	}
	else
	{
		Merkle_Tree* temp_node = node;
		node = node->father_node;
		if (node->left_child == temp_node)
			return node->right_child;
		else
			return node->left_child;
	}
}
void Merkle_audit_path(Merkle_Tree* root, Merkle_Tree* leaf_node)
{
	if (leaf_node == NULL)
		return;
	else
	{
		leaf_node = Get_Neibor_Node(leaf_node);
		if (leaf_node == NULL)
		{
			printf("%d  ", root->hash_value);
			return;
		}
		if (leaf_node->str == NULL)
			printf("%d  ------>  ", leaf_node->hash_value);
		else if (leaf_node->str != NULL)
			printf("%s  ------>  ", leaf_node->str);
		leaf_node = leaf_node->father_node;
		Merkle_audit_path(root, leaf_node);
	}
}
int main()
{
	char* str_arr[8] = { (char*)"Hi",(char*)",",(char*)"I",(char*)"am",(char*)"Twilight",(char*)"!",(char*)"happy",(char*)"Hi" };
	char* str_arr1[8] = { (char*)"Hi",(char*)",",(char*)"I",(char*)"am",(char*)"Rarity",(char*)"!",(char*)"happy",(char*)"Hi" };
	int num;
	int num1;
	Merkle_Tree* mt = NULL;
	Merkle_Tree* mt11 = NULL;
	Merkle_Tree* mt2 = NULL;
	Merkle_Tree* leaf = NULL;
	bool result = 0;
	mt = Build_Merkle_Tree(mt, str_arr, 8);
	char* test_input = (char*)"Twilight";
	mt11 = Build_Merkle_Tree(mt11, str_arr1, 8);
	Merkle_Proof(mt, mt11, result);
	Print_Merkle_Tree(mt11, mt11->level);
	Print_Merkle_Tree(mt, mt->level);
	Get_Node(mt, test_input);
	printf("Merkle���·���ǣ�");
	Merkle_audit_path(mt, correct_leaf);
}