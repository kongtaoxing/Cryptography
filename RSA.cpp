#include <stdlib.h>
#include <stdio.h>

#define LTM_DESC
#include <tomcrypt.h>

int main(void)
{
	int err, hash_idx, prng_idx, res, padding, inpadding;
	unsigned long l1, l2, i;
	unsigned char pt[17], pt2[17], out[1024], out2[1024], out3[1024];
	rsa_key key;
	printf("RSA加解密算法加密机制\n");

	while (true) {
		printf("\n请输入文本串 s (16字节):\n");
		scanf("%s", &pt);
		printf("请选择加密机制:\n");
		printf("\t1.RSAES-PKCS1-V1_5\n");
		printf("\t2.RSAES_OAEP\n");
		scanf("%d", &inpadding);
		if (inpadding == 1)
			padding = LTC_LTC_PKCS_1_V1_5;
		else
			padding = LTC_LTC_PKCS_1_OAEP;
		/* register prng/hash */
		if (register_prng(&sprng_desc) == -1) {
			printf("Error registering sprng");
			return EXIT_FAILURE;
		}
		/* register a math library (in this case TomsFastMath)*/
		ltc_mp = ltm_desc;
		if (register_hash(&sha1_desc) == -1) {
			printf("Error registering sha1");
			return EXIT_FAILURE;
		}
		hash_idx = find_hash("sha1");
		prng_idx = find_prng("sprng");
		/* make an RSA-1024 key */
		if ((err = rsa_make_key(NULL, /* PRNG state */
			prng_idx, /* PRNG idx */
			1024 / 8, /* 1024-bit key */
			65537, /* we like e=65537 */
			&key) /* where to store the key */
			) != CRYPT_OK) {
			printf("rsa_make_key %s", error_to_string(err));
			return EXIT_FAILURE;
		}

		/* fill in pt[] with a key we want to send ... */
		l1 = sizeof(out);
		if ((err = rsa_encrypt_key_ex(pt, /* data we wish to encrypt */
			16, /* data is 16 bytes long */
			out, /* where to store ciphertext */
			&l1, /* length of ciphertext */
			(unsigned char*)"TestApp", /* our lparam for this program */
			7, /* lparam is 7 bytes long */
			NULL, /* PRNG state */
			prng_idx, /* prng idx */
			hash_idx, /* hash idx */
			padding,
			&key) /* our RSA key */
			) != CRYPT_OK) {
			printf("rsa_encrypt_key %s", error_to_string(err));
			return EXIT_FAILURE;
		}
		printf("\n加密后的密文 s' :\n");
		for (int i = 0; i <= 127; i++)
			printf("%02x", out[i]);
		printf("\n");
		/* now let’s decrypt the encrypted key */
		l2 = sizeof(pt2);
		if ((err = rsa_decrypt_key_ex(out, /* encrypted data */
			l1, /* length of ciphertext */
			pt2, /* where to put plaintext */
			&l2, /* plaintext length */
			(unsigned char*)"TestApp", /* lparam for this program */
			7, /* lparam is 7 bytes long */
			hash_idx, /* hash idx */
			padding,
			&res, /* validity of data */
			&key) /* our RSA key */
			) != CRYPT_OK) {
			printf("rsa_decrypt_key %s", error_to_string(err));
			return EXIT_FAILURE;
		}
		printf("将s'解密后的明文 s''：");           //%s\n",pt2
		for (i = 0; i < 16; i++) {

			printf("%c", pt2[i]);
		}
		printf("\n");
		printf("原输入的明文 s ：");           //%s\n",pt2
		for (i = 0; i < 16; i++) {

			printf("%c", pt[i]);
		}
		printf("\n");


		if ((err = rsa_encrypt_key_ex(pt, /* data we wish to encrypt */
			16, /* data is 16 bytes long */
			out2, /* where to store ciphertext */
			&l1, /* length of ciphertext */
			(unsigned char*)"TestApp", /* our lparam for this program */
			7, /* lparam is 7 bytes long */
			NULL, /* PRNG state */
			prng_idx, /* prng idx */
			hash_idx, /* hash idx */
			padding,
			&key) /* our RSA key */
			) != CRYPT_OK) {
			printf("rsa_encrypt_key %s", error_to_string(err));
			return EXIT_FAILURE;
		}

		printf("\n用相同密钥对同一个明文加密后的密文s'2\n:");
		for (int i = 0; i <= 127; i++)
			printf("%02x", out2[i]);
		printf("\n");
		l2 = sizeof(pt2);
		if ((err = rsa_decrypt_key_ex(out2, /* encrypted data */
			l1, /* length of ciphertext */
			pt2, /* where to put plaintext */
			&l2, /* plaintext length */
			(unsigned char*)"TestApp", /* lparam for this program */
			7, /* lparam is 7 bytes long */
			hash_idx, /* hash idx */
			padding,
			&res, /* validity of data */
			&key) /* our RSA key */
			) != CRYPT_OK) {
			printf("rsa_decrypt_key %s", error_to_string(err));
			return EXIT_FAILURE;
		}
		printf("将s'2解密后的明文：");
		for (i = 0; i < 16; i++) {

			printf("%c", pt2[i]);
		}
		printf("\n");
		if ((err = rsa_encrypt_key_ex(pt, /* data we wish to encrypt */
			16, /* data is 16 bytes long */
			out3, /* where to store ciphertext */
			&l1, /* length of ciphertext */
			(unsigned char*)"TestApp", /* our lparam for this program */
			7, /* lparam is 7 bytes long */
			NULL, /* PRNG state */
			prng_idx, /* prng idx */
			hash_idx, /* hash idx */
			padding,
			&key) /* our RSA key */
			) != CRYPT_OK) {
			printf("rsa_encrypt_key %s", error_to_string(err));
			return EXIT_FAILURE;
		}
		printf("\n用相同密钥对同一个明文加密后的密文s'3:\n");
		for (int i = 0; i <= 127; i++)
			printf("%02x", out3[i]);
		printf("\n");
		l2 = sizeof(pt2);
		if ((err = rsa_decrypt_key_ex(out3, /* encrypted data */
			l1, /* length of ciphertext */
			pt2, /* where to put plaintext */
			&l2, /* plaintext length */
			(unsigned char*)"TestApp", /* lparam for this program */
			7, /* lparam is 7 bytes long */
			hash_idx, /* hash idx */
			padding,
			&res, /* validity of data */
			&key) /* our RSA key */
			) != CRYPT_OK) {
			printf("rsa_decrypt_key %s", error_to_string(err));
			return EXIT_FAILURE;
		}
		printf("将s'3解密后的明文：");
		for (i = 0; i < 16; i++) {

			printf("%c", pt2[i]);
		}
		printf("\n");
	}
	return 0;
}
