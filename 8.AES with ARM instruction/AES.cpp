#include<iostream>
using namespace std;

const unsigned char S[256] = {
0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

const uint32_t rcon[10] = {
		0x01000000UL, 0x02000000UL, 0x04000000UL, 0x08000000UL, 0x10000000UL,
		0x20000000UL, 0x40000000UL, 0x80000000UL, 0x1B000000UL, 0x36000000UL
};

const uint8_t rcon1[10] = {
		0x01, 0x02, 0x04, 0x08, 0x10,
		0x20, 0x40, 0x80, 0x1B, 0x36
};

const uint8_t mixcolumns[4][4] = {
0x02,0x03,0x01,0x01,
0x01,0x02,0x03,0x01,
0x01,0x01,0x02,0x03,
0x03,0x01,0x01,0x02
};

static void SubBytes(uint8_t plaintext[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			plaintext[i][j] = S[plaintext[i][j]];
}

static void ShiftRows(uint8_t plaintext[4][4])
{
	uint8_t temp[4][4];
	for (int i = 0; i < 4; i++)
	{
		temp[i][0] = plaintext[i][i % 4];
		temp[i][1] = plaintext[i][(1 + i) % 4];
		temp[i][2] = plaintext[i][(2 + i) % 4];
		temp[i][3] = plaintext[i][(3 + i) % 4];
	}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			plaintext[i][j] = temp[i][j];
}

static uint8_t GaloisField(uint8_t mixcolumn, uint8_t plaintext)
{
	if (mixcolumn == 1)
		return plaintext;
	if (mixcolumn == 2)
	{
		if (plaintext >= 0x80)
			return (plaintext << 1) ^ 0x1B;
		else
			return (plaintext << 1);
	}
	if (mixcolumn == 3)
		return GaloisField(2, plaintext) ^ GaloisField(1, plaintext);
	else
		return 0;
}

static void MixColumns(uint8_t plaintext[4][4])
{
	uint8_t temptext[4][4] = { 0 };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
			{
				if (k == 0)
					temptext[i][j] = GaloisField(mixcolumns[i][k], plaintext[k][j]);
				else
					temptext[i][j] = temptext[i][j] ^ GaloisField(mixcolumns[i][k], plaintext[k][j]);
			}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			plaintext[i][j] = temptext[i][j];
}

static void KeyExpansion(uint8_t key[4][4], uint8_t roundkey[4][44])
{
	for (int j = 0; j < 44; j++)
	{
		if (j < 4)
			for (int i = 0; i < 4; i++)
				roundkey[i][j] = key[i][j];
		else
		{
			if (j % 4 != 0)
				for (int i = 0; i < 4; i++)
					roundkey[i][j] = roundkey[i][j - 4] ^ roundkey[i][j - 1];
			else
			{
				for (int i = 0; i < 4; i++)
				{
					if (i == 0)
						roundkey[i][j] = roundkey[i][j - 4] ^ S[roundkey[(i + 1) % 4][j - 1]] ^ rcon1[(j / 4) - 1];
					else
						roundkey[i][j] = roundkey[i][j - 4] ^ S[roundkey[(i + 1) % 4][j - 1]];
				}

			}
		}
	}
}

static void AddRoundKey(uint8_t plaintext[4][4], uint8_t roundkey[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			plaintext[i][j] ^= roundkey[i][j];
}

static void AES(const char* plaintext, uint8_t* ciphertext, bool output)
{
	//plaintext = "2021004600460000";
	uint8_t plaintext_operating[4][4] = {
	plaintext[0],plaintext[4],plaintext[8],plaintext[12],
	plaintext[1],plaintext[5],plaintext[9],plaintext[13],
	plaintext[2],plaintext[6],plaintext[10],plaintext[14],
	plaintext[3],plaintext[7],plaintext[11],plaintext[15]
	};
	uint8_t key[4][4] = {
	plaintext[0],plaintext[4],plaintext[8],plaintext[12],
	plaintext[1],plaintext[5],plaintext[9],plaintext[13],
	plaintext[2],plaintext[6],plaintext[10],plaintext[14],
	plaintext[3],plaintext[7],plaintext[11],plaintext[15]
	};
	uint8_t roundkey[4][44] = {};
	KeyExpansion(key, roundkey);
	uint8_t Roundkey[10][4][4] = {};
	for (int j = 4; j < 44; j += 4)
	{
		for (int i = 0; i < 4; i++)
		{
			Roundkey[j / 4 - 1][i][j % 4] = roundkey[i][j];
			Roundkey[j / 4 - 1][i][(j + 1) % 4] = roundkey[i][j + 1];
			Roundkey[j / 4 - 1][i][(j + 2) % 4] = roundkey[i][j + 2];
			Roundkey[j / 4 - 1][i][(j + 3) % 4] = roundkey[i][j + 3];
			if (output) {
				cout << "第" << j / 4 << "轮密钥：" << endl;
				printf("%02X ", roundkey[i][j]);
				printf("%02X ", roundkey[i][j + 1]);
				printf("%02X ", roundkey[i][j + 2]);
				printf("%02X\n", roundkey[i][j + 3]);
			}
		}
	}
	AddRoundKey(plaintext_operating, key);
	for (int i = 0; i < 9; i++)
	{
		SubBytes(plaintext_operating);
		ShiftRows(plaintext_operating);
		MixColumns(plaintext_operating);
		AddRoundKey(plaintext_operating, Roundkey[i]);
	}
	SubBytes(plaintext_operating);
	ShiftRows(plaintext_operating);
	AddRoundKey(plaintext_operating, Roundkey[9]);

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			ciphertext[i * 4 + j] = plaintext_operating[i][j];
}

int main() {
	const char* plaintext = "2021004600080000";
	cout << "明文：" << endl;
	for (int j = 0; j < 16; j++)
		printf("%02X ", plaintext[j]);
	printf("\n");
	uint8_t ciphertext[16];
	AES(plaintext, ciphertext,0);
	cout << "密文：";
	for (int j = 0; j < 16; j++)
		printf("%02X ", ciphertext[j]);
}