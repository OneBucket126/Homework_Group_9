#include"sm3.h"
uint8_t* padding(uint8_t* input, unsigned long long length)//length��λ�Ǳ���,��������uint8_t�������ʽ�����padding�õ�uint8_t����
{
	int k = 1;
	int total_length = (length / 512 + 1) * 512;//������س���
	int byte_length = length / 8;//���ǰ�ֽڳ���
	int total_byte_length = total_length / 8;//�����ֽڳ���
	for (k; k < total_length; k++)
	{
		if (((length + 1 + k) % 512) == 448)
			break;
	}
	uint8_t* output = (uint8_t*)malloc((total_length / 8) * sizeof(uint8_t));
	for (int i = 0; i < byte_length; i++)//����output��ǰ������inputһ��
	{
		output[i] = input[i];
	}

	output[byte_length] = 0x80;//��ֵһ��1��7����

	int outfix = byte_length + 1 + (k - 7) / 8;//ʣ�µ�0�ĸ����ǣ�k-7��/8
	for (int i = byte_length + 1; i < outfix; i++)//Ϊ��Щ��0
	{
		output[i] = 0x00;
	}

	uint8_t* ptr = (uint8_t*)&length;
	for (int i = 0; i < 8; i++)
	{
		uint8_t temp = *ptr;
		output[total_byte_length - i - 1] = temp;
		ptr = ptr + 1;
	}
	after_padding_length = total_byte_length;
	return output;
}
void message_extension(uint8_t* input, uint32_t* W, uint32_t* W_1)//W��w0-w67��W_1��w0'-w63' uint32ֻ֧�����볤����512bit����,W��W_1����֮ǰ�����ȷ���68��64��uint32_t�ռ�
{
	int word_length = after_padding_length / 16;//��Ϣ�ִ�С,Ҳ��һ����Ϣ�ֺ��е�uint8_t������
	for (int i = 0; i < 16; i++)
	{
		W[i] = MERAGE4(input[i * 4 + 0], input[i * 4 + 1], input[i * 4 + 2], input[i * 4 + 3]);
	}
	for (int i = 16; i < 68; i++)//W�ļ��� ��68��
	{
		uint32_t Wj_16 = W[i - 16];
		uint32_t Wj_9 = W[i - 9];
		uint32_t Wj_3 = W[i - 3];
		uint32_t Wj_13 = W[i - 13];
		uint32_t Wj_6 = W[i - 6];
		uint32_t round_shift1 = round_shift_left(Wj_3, 15);
		uint32_t round_shift2 = round_shift_left(Wj_13, 7);
		uint32_t P1_input = Wj_16 ^ Wj_9 ^ round_shift1;
		uint32_t P1_result = P1(P1_input);
		W[i] = P1_result ^ round_shift2 ^ Wj_6;
	}
	for (int i = 0; i < 64; i++)//w'�ļ��㣬64��
	{
		W_1[i] = W[i] ^ W[i + 4];
	}
}
void CF(uint32_t* Vi, uint8_t* Bi, uint32_t* W, uint32_t* W_1, unsigned long long input_length)
{
	uint8_t* padding_result = (uint8_t*)malloc(64 * sizeof(uint8_t));
	padding_result = padding(Bi, input_length);
	message_extension(padding_result, W, W_1);
	free(padding_result);

	uint32_t A, B, C, D, E, F, G, H, SS1, SS2, TT1, TT2, shift_left_Ti, FF_result, GG_result;//shift_left_Ti��Ti��������ѭ����λ�Ľ��
	A = IV[0], B = IV[1], C = IV[2], D = IV[3], E = IV[4], F = IV[5], G = IV[6], H = IV[7];
	for (int i = 0; i < 64; i++)
	{
		uint32_t A_round_shift_left_12 = round_shift_left(A, 12);
		unsigned int j_shift_left = i % 32;
		if (i <= 15)
			shift_left_Ti = round_shift_left(Tj015, j_shift_left);
		else
			shift_left_Ti = round_shift_left(Tj1663, j_shift_left);

		SS1 = (A_round_shift_left_12 + E + shift_left_Ti) % MODNUM;
		SS1 = round_shift_left(SS1, 7);
		SS2 = SS1 ^ A_round_shift_left_12;
		if (i <= 15)
		{
			FF_result = FFj015(A, B, C);
			GG_result = GGj015(E, F, G);
			TT1 = (FF_result + D + SS2 + W_1[i]) % MODNUM;
			TT2 = (GG_result + H + SS1 + W[i]) % MODNUM;
		}

		else
		{
			FF_result = FFj1663(A, B, C);
			GG_result = GGj1663(E, F, G);
			TT1 = (FF_result + D + SS2 + W_1[i]) % MODNUM;
			TT2 = (GG_result + H + SS1 + W[i]) % MODNUM;
		}
		D = C;
		C = round_shift_left(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = round_shift_left(F, 19);
		F = E;
		E = P0(TT2);
	}

	SM3_hash_result[0] = A ^ IV[0];
	SM3_hash_result[1] = B ^ IV[1];
	SM3_hash_result[2] = C ^ IV[2];
	SM3_hash_result[3] = D ^ IV[3];
	SM3_hash_result[4] = E ^ IV[4];
	SM3_hash_result[5] = F ^ IV[5];
	SM3_hash_result[6] = G ^ IV[6];
	SM3_hash_result[7] = H ^ IV[7];
}
void CF_length_extension_attack(uint32_t* Vi, uint8_t* after_padding_input, uint32_t* W, uint32_t* W_1)
{
	message_extension(after_padding_input, W, W_1);
	uint32_t A, B, C, D, E, F, G, H, SS1, SS2, TT1, TT2, shift_left_Ti, FF_result, GG_result;//shift_left_Ti��Ti��������ѭ����λ�Ľ��
	A = IV[0], B = IV[1], C = IV[2], D = IV[3], E = IV[4], F = IV[5], G = IV[6], H = IV[7];
	for (int i = 0; i < 64; i++)
	{
		uint32_t A_round_shift_left_12 = round_shift_left(A, 12);
		unsigned int j_shift_left = i % 32;
		if (i <= 15)
			shift_left_Ti = round_shift_left(Tj015, j_shift_left);
		else
			shift_left_Ti = round_shift_left(Tj1663, j_shift_left);

		SS1 = (A_round_shift_left_12 + E + shift_left_Ti) % MODNUM;
		SS1 = round_shift_left(SS1, 7);
		SS2 = SS1 ^ A_round_shift_left_12;
		if (i <= 15)
		{
			FF_result = FFj015(A, B, C);
			GG_result = GGj015(E, F, G);
			TT1 = (FF_result + D + SS2 + W_1[i]) % MODNUM;
			TT2 = (GG_result + H + SS1 + W[i]) % MODNUM;
		}

		else
		{
			FF_result = FFj1663(A, B, C);
			GG_result = GGj1663(E, F, G);
			TT1 = (FF_result + D + SS2 + W_1[i]) % MODNUM;
			TT2 = (GG_result + H + SS1 + W[i]) % MODNUM;
		}
		D = C;
		C = round_shift_left(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = round_shift_left(F, 19);
		F = E;
		E = P0(TT2);
	}
	SM3_hash_result[0] = A ^ IV[0];
	SM3_hash_result[1] = B ^ IV[1];
	SM3_hash_result[2] = C ^ IV[2];
	SM3_hash_result[3] = D ^ IV[3];
	SM3_hash_result[4] = E ^ IV[4];
	SM3_hash_result[5] = F ^ IV[5];
	SM3_hash_result[6] = G ^ IV[6];
	SM3_hash_result[7] = H ^ IV[7];
}
void sm3(uint8_t* input, unsigned long long length)//sm3��ϣ�����������֧��512�������ڣ���uint8������ʽ����,�ڶ����������������ݳ��ȣ���λ��bit,����������ɺ�ȫ�ֱ���SM3_hash_result�洢����������ʽ�ǳ���Ϊ8��uint32����
{
	uint8_t* padding_result = (uint8_t*)malloc(64 * sizeof(uint8_t));
	padding_result = padding(input, length);
	uint32_t* W = (uint32_t*)malloc(68 * sizeof(uint32_t));//��Wi����'
	uint32_t* W_1 = (uint32_t*)malloc(64 * sizeof(uint32_t));//Wi'
	CF(IV, input, W, W_1, length);
	free(W);
	free(W_1);
}
void print_sm3_result()
{
	for (int i = 0; i < 8; i++)
	{
		printf("%x ", SM3_hash_result[i]);
	}
}
void length_extension_attack(uint32_t* known_hash_result, uint8_t* append, unsigned long long append_length, uint32_t* W, uint32_t* W_1)
{
	CF(known_hash_result, append, W, W_1, append_length);
}
bool check_attack_result(uint32_t* known_hash_result, uint8_t* append, unsigned long long append_length, uint32_t* W, uint32_t* W_1, uint8_t* after_padding_input)//W��W_1����֮ǰ�����ȷ���68��64��uint32_t�ռ�
{
	bool check_result = 1;
	length_extension_attack(known_hash_result, append, append_length, W, W_1);
	uint32_t length_extension_attack_result[8];
	for (int i = 0; i < 8; i++)
	{
		length_extension_attack_result[i] = SM3_hash_result[i];//������չ�����Ľ��
	}
	//-------------------------------------------����hash----------------------------------------	
	CF_length_extension_attack(IV, after_padding_input, W, W_1);
	CF(IV, append, W, W_1, 8);//������ϣ�����Ľ��
	//-------------------------------------------����hash----------------------------------------	
	for (int i = 0; i < 8; i++)//�����һ����
	{
		if (length_extension_attack_result[i] != SM3_hash_result[i])
		{
			return CHECK_FAIL;
		}
	}
	return CHECK_PASS;
}
string strRand(int length) {			// length: �����ַ����ĳ���
	char tmp;							// tmp: �ݴ�һ�������
	string buffer;						// buffer: ���淵��ֵ

	// ���������бȽ���Ҫ:
	random_device rd;					// ����һ�� std::random_device ���� rd
	default_random_engine random(rd());	// �� rd ��ʼ��һ������������� random

	for (int i = 0; i < length; i++) {
		tmp = random() % 36;	// ���һ��С�� 36 ��������0-9��A-Z �� 36 ���ַ�
		if (tmp < 10) {			// ��������С�� 10���任��һ�����������ֵ� ASCII
			tmp += '0';
		}
		else {				// ���򣬱任��һ����д��ĸ�� ASCII
			tmp -= 10;
			tmp += 'A';
		}
		buffer += tmp;
	}
	return buffer;
}
bool GetBitStatu(int num, int pos)
{
	if (num & (1 << pos)) //��λ��֮��Ľ����0
		return 1;
	else
		return 0;
}
void build_tree(normal_node* root, uint32_t input, string sm3_input)//��������������������·���洢hash�����Ҷ�ӽڵ�洢hash���룬��hash�����hash����洢������
{
	normal_node* ptr = root;
	int bit_status;
	for (int i = 0; i < 32; i++)
	{
		bit_status = GetBitStatu(input, i);
		if (bit_status == 1)
		{
			if (ptr->left_child != NULL)
				ptr = ptr->left_child;
			else
			{
				normal_node* child_node = new normal_node;
				child_node->left_child = nullptr;
				child_node->right_child = nullptr;
				ptr->left_child = child_node;
				ptr = ptr->left_child;
			}
		}
		else
		{
			if (ptr->right_child != NULL)
				ptr = ptr->right_child;
			else
			{
				normal_node* child_node = new normal_node;
				child_node->left_child = nullptr;
				child_node->right_child = nullptr;
				ptr->right_child = child_node;
				ptr = ptr->right_child;
			}
		}
	}
	ptr->sm3_input = sm3_input;
}
bool search_tree(uint32_t hash_result, normal_node* root)//�������ӿڣ������ϣ��������ڵ㣬���ظý���Ƿ�������
{
	int bit_status;
	normal_node* ptr = root;
	for (int i = 0; i < 32; i++)
	{
		bit_status = GetBitStatu(hash_result, i);
		if (bit_status == 1)
		{
			if (ptr->left_child == NULL)
				return RESULT_NOT_FOUND;
			else
				ptr = ptr->left_child;
		}
		else
		{
			if (ptr->right_child == NULL)
				return RESULT_NOT_FOUND;
			else
				ptr = ptr->right_child;
		}
	}
	return RESULT_FOUND;
}
string get_hash_input_from_root(normal_node* root, uint32_t hash_result)//����hash�����root�ڵ㣬�����в�ѯhashֵ��Ӧ��ԭ��
{
	bool bit_status;
	normal_node* ptr = root;
	for (int i = 0; i < 32; i++)
	{
		bit_status = GetBitStatu(hash_result, i);
		if (bit_status == 1)
		{
			if (ptr->left_child == NULL)
				return "nod found";
			else
				ptr = ptr->left_child;
		}
		else
		{
			if (ptr->right_child == NULL)
				return "not found";
			else
				ptr = ptr->right_child;
		}
	}
	return ptr->sm3_input;
}
bool birthday_attack(normal_node* root)
{
	unsigned int length;
	char* hash_input;
	for (int i = 0; i < 65535; i++)
	{
		length = rand() % 56;
		string temp = strRand(length);
		hash_input = (char*)temp.c_str();
		uint8_t* sm3_input = (uint8_t*)malloc(length * sizeof(uint8_t));
		for (int i = 0; i < length; i++)
		{
			sm3_input[i] = hash_input[i];
		}
		sm3(sm3_input, length * 8);
		free(sm3_input);
		uint32_t reduce_result = SM3_hash_result[0];
		bool result = search_tree(reduce_result, root);
		if (result == RESULT_FOUND)
		{
			return RESULT_FOUND;//�ҵ���ײ
		}
		build_tree(root, reduce_result, temp);
	}
}
int main()
{
	normal_node* root = new normal_node;
	root->left_child = nullptr;
	root->right_child = nullptr;
	bool collison_result = birthday_attack(root);
	if (collison_result == RESULT_FOUND)
	{
		printf("%s", "collision found!!!");
	}
}
