// Copyright RLoris 2021

#include "CryptoHelperBPLibrary.h"
#include "CryptoHelper.h"
#include "Misc/SecureHash.h"
#include "Misc/Crc.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Base64.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include <string>
#include <sstream>
#include <iomanip>
#include "Keccak/sha3.h"

#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#include "openssl/sha.h"
#include "openssl/md5.h"
#include "openssl/rsa.h"
#include "openssl/pem.h"
#include "openssl/err.h"
#include "openssl/aes.h"
#include "openssl/rand.h"
THIRD_PARTY_INCLUDES_END
#undef UI

UCryptoHelperBPLibrary::UCryptoHelperBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {}

bool UCryptoHelperBPLibrary::DigestString(const FString& String, FString& Hash, EDigestAlgorithm HashType)
{
	std::string msg = std::string(TCHAR_TO_UTF8(*String));
	Hash = Digest((const uint8*)msg.c_str(), msg.size(), HashType);
	return !Hash.IsEmpty();
}

bool UCryptoHelperBPLibrary::DigestBytes(const TArray<uint8>& Bytes, FString& Hash, EDigestAlgorithm HashType)
{
	Hash = Digest(Bytes.GetData(), Bytes.Num(), HashType);
	return !Hash.IsEmpty();
}

bool UCryptoHelperBPLibrary::DigestFile(const FString& Path, FString& Hash, EDigestAlgorithm HashType)
{
	if (!IsDigestSupported(HashType))
		return false;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (file.FileExists(*Path))
	{
		const EVP_MD* evp = GetDigestEVP(HashType);
		if (!evp) return false;

		FArchive* Ar = IFileManager::Get().CreateFileReader(*Path);
		if (!Ar) return false;

		TArray<uint8> Buffer;
		Buffer.SetNum(1024 * 64);

		const int64 Size = Ar->TotalSize();
		int64 Position = 0;

		EVP_MD_CTX* ctx = EVP_MD_CTX_create();
		int ret = EVP_DigestInit(ctx, evp);

		while (Position < Size)
		{
			const auto ReadNum = FMath::Min(Size - Position, (int64)Buffer.Num());
			Ar->Serialize(Buffer.GetData(), ReadNum);
			ret = EVP_DigestUpdate(ctx, Buffer.GetData(), ReadNum);
			Position += ReadNum;
		}
		unsigned int length = 0;
		TArray<uint8> HashBytes;
		HashBytes.SetNum(EVP_MAX_MD_SIZE);
		ret = EVP_DigestFinal(ctx, HashBytes.GetData(), &length);
		EVP_MD_CTX_destroy(ctx);
		delete Ar;
		Hash = ret == 1 ? BytesToHex(HashBytes.GetData(), length) : "";
		return true;
	}
	Hash = FString();
	return false;
}

bool UCryptoHelperBPLibrary::RandomDictionaryString(FString& Salt, int32 Length, FString Dictionary)
{
	if (Dictionary.Len() == 0 || Length < 0)
	{
		return false;
	}
	for (int32 i = 0; i < Length; i++)
	{
		Salt += Dictionary.Mid(FMath::RandHelper(Dictionary.Len()), 1);
	}
	return true;
}

bool UCryptoHelperBPLibrary::RandomBytes(TArray<uint8>& Salt, int32 Length)
{
	if (Length < 0)
	{
		return false;
	}
	if (Length == 0)
	{
		return true;
	}
	Salt.SetNum(Length);
	int res = RAND_bytes(Salt.GetData(), Length);
	return res == 1;
}

bool UCryptoHelperBPLibrary::HMACFile(const FString& Path, const FString& Key, FString& Hash, EDigestAlgorithm HashType)
{
	if (!IsDigestSupported(HashType))
		return false;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (file.FileExists(*Path))
	{
		const EVP_MD* evp = GetDigestEVP(HashType);
		if (!evp) return false;

		FArchive* Ar = IFileManager::Get().CreateFileReader(*Path);
		if (!Ar) return false;

		TArray<uint8> Buffer;
		Buffer.SetNum(1024 * 64);

		const int64 Size = Ar->TotalSize();
		int64 Position = 0;

		std::string key = TCHAR_TO_UTF8(*Key);
		size_t md_len = 0;
		EVP_MD_CTX* mdctx = EVP_MD_CTX_create();
		EVP_PKEY* pkey = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, (const unsigned char*)key.c_str(), key.size());
		int ret = EVP_DigestSignInit(mdctx, NULL, evp, NULL, pkey);

		while (Position < Size)
		{
			const auto ReadNum = FMath::Min(Size - Position, (int64)Buffer.Num());
			Ar->Serialize(Buffer.GetData(), ReadNum);
			ret = EVP_DigestSignUpdate(mdctx, Buffer.GetData(), ReadNum);
			Position += ReadNum;
		}
		unsigned int length = 0;
		TArray<uint8> HashBytes;
		HashBytes.SetNum(EVP_MAX_MD_SIZE);
		EVP_DigestSignFinal(mdctx, HashBytes.GetData(), &md_len);
		HashBytes.SetNum(md_len);
		Hash = ret == 1 ? BytesToHex(HashBytes.GetData(), HashBytes.Num()) : "";
		delete Ar;
		EVP_PKEY_free(pkey);
		return true;
	}
	Hash = FString();
	return false;
}

bool UCryptoHelperBPLibrary::HMACString(const FString& String, const FString& Key, FString& Hash, EDigestAlgorithm HashType)
{
	std::string str = TCHAR_TO_UTF8(*String);
	Hash = HMAC((const uint8*)str.c_str(), str.size(), Key, HashType);
	return !Hash.IsEmpty();
}

bool UCryptoHelperBPLibrary::HMACBytes(const TArray<uint8>& Bytes, const FString& Key, FString& Hash, EDigestAlgorithm HashType)
{
	Hash = HMAC(Bytes.GetData(), Bytes.Num(), Key, HashType);
	return !Hash.IsEmpty();
}

bool UCryptoHelperBPLibrary::IsSymmetricSupported(ESymmetricAlgorithm Type)
{
	return GetCipherEVP(Type) != NULL;
}

int32 UCryptoHelperBPLibrary::SGetIVSize(ESymmetricAlgorithm Type)
{
	const EVP_CIPHER* cipher = GetCipherEVP(Type);
	if (!cipher) return -1;
	return EVP_CIPHER_iv_length(cipher);
}

int32 UCryptoHelperBPLibrary::SGetKeySize(ESymmetricAlgorithm Type)
{
	const EVP_CIPHER* cipher = GetCipherEVP(Type);
	if (!cipher) return -1;
	return EVP_CIPHER_key_length(cipher);
}

bool UCryptoHelperBPLibrary::SEncryptString(const FString& PlainString, const FString& Key, const FString& IV, FString& CipherString, ESymmetricAlgorithm Type)
{
	TArray<uint8> PlainBytes;
	PlainBytes.Append((uint8*)TCHAR_TO_UTF8(PlainString.GetCharArray().GetData()), PlainString.GetCharArray().Num() - 1);
	TArray<uint8> CipherBytes;
	bool Res = SEncryptDecrypt(true, PlainBytes, CipherBytes, Key, IV, Type);
	CipherString = BytesToHex(CipherBytes.GetData(), CipherBytes.Num());
	return Res;
}

bool UCryptoHelperBPLibrary::SDecryptString(const FString& CipherString, const FString& Key, const FString& IV, FString& PlainString, ESymmetricAlgorithm Type)
{
	TArray<uint8> CipherBytes;
	CipherBytes.SetNum(CipherString.Len() / 2);
	HexToBytes(CipherString, CipherBytes.GetData());
	CipherBytes.Shrink();
	TArray<uint8> PlainBytes;
	bool Res = SEncryptDecrypt(false, CipherBytes, PlainBytes, Key, IV, Type);
	PlainBytes.Add(0);
	PlainString = UBytesToString(PlainBytes);
	return Res;
}

bool UCryptoHelperBPLibrary::SEncryptBytes(const TArray<uint8>& PlainBytes, const FString& Key, const FString& IV, TArray<uint8>& CipherBytes, ESymmetricAlgorithm Type)
{
	return SEncryptDecrypt(true, PlainBytes, CipherBytes, Key, IV, Type);
}

bool UCryptoHelperBPLibrary::SDecryptBytes(const TArray<uint8>& CipherBytes, const FString& Key, const FString& IV, TArray<uint8>& PlainBytes, ESymmetricAlgorithm Type)
{
	return SEncryptDecrypt(false, CipherBytes, PlainBytes, Key, IV, Type);
}

bool UCryptoHelperBPLibrary::SEncryptFile(const FString& InPath, const FString& OutPath, const FString& Key, const FString& IV, ESymmetricAlgorithm Type)
{
	return SEncryptDecryptFile(true, InPath, OutPath, Key, IV, Type);
}

bool UCryptoHelperBPLibrary::SDecryptFile(const FString& InPath, const FString& OutPath, const FString& Key, const FString& IV, ESymmetricAlgorithm Type)
{
	return SEncryptDecryptFile(false, InPath, OutPath, Key, IV, Type);
}

bool UCryptoHelperBPLibrary::AGenerateKeychain(const FKeychainOptions& Options, FKeychain& Keychain)
{
	bool Success = false;
	if (Options.Size <= 0)
	{
		return Success;
	}
	int id = 0;
	switch (Options.Type)
	{
		case(EAsymmetricAlgorithm::RSA):
			id = EVP_PKEY_RSA;
		break;
		case(EAsymmetricAlgorithm::DSA):
			id = EVP_PKEY_DSA;
		break;
		default:
			return Success;
		break;
	}
	long privKeySize;
	long pubKeySize;
	char* privKeyData;
	char* pubKeyData;
	BIO* BIOPrivate = NULL;
	BIO* BIOPublic = NULL;
	EVP_PKEY* PKey = NULL;
	EVP_PKEY* PKEYParams = NULL;
	EVP_PKEY_CTX* Ctx = EVP_PKEY_CTX_new_id(id, nullptr);
	if (Options.Type == EAsymmetricAlgorithm::DSA)
	{
		if (!Ctx) goto free_all;
		if (1 != EVP_PKEY_paramgen_init(Ctx)) goto free_all;
		if (1 != EVP_PKEY_CTX_set_dsa_paramgen_bits(Ctx, Options.Size)) goto free_all;
		if (1 != EVP_PKEY_paramgen(Ctx, &PKEYParams)) goto free_all;
		EVP_PKEY_CTX_free(Ctx);
		Ctx = EVP_PKEY_CTX_new(PKEYParams, NULL);
	}
	if (!Ctx) goto free_all;
	if (1 != EVP_PKEY_keygen_init(Ctx)) goto free_all;
	if (Options.Type == EAsymmetricAlgorithm::RSA && 1 != EVP_PKEY_CTX_set_rsa_keygen_bits(Ctx, Options.Size)) goto free_all;
	if (1 != EVP_PKEY_keygen(Ctx, &PKey)) goto free_all;
	BIOPublic = BIO_new(BIO_s_mem());
	if (1 != PEM_write_bio_PUBKEY(BIOPublic, PKey)) goto free_all;
	BIOPrivate = BIO_new(BIO_s_mem());
	if (1 != PEM_write_bio_PrivateKey(BIOPrivate, PKey, 0, 0, 0, 0, 0)) goto free_all;
	privKeySize = BIO_get_mem_data(BIOPrivate, &privKeyData);
	pubKeySize = BIO_get_mem_data(BIOPublic, &pubKeyData);
	Keychain.PrivateKey = FString(std::string(privKeyData, privKeySize).c_str());
	Keychain.PublicKey = FString(std::string(pubKeyData, pubKeySize).c_str());
	Keychain.Options = Options;
	Success = true;
free_all:
	BIO_free_all(BIOPublic);
	BIO_free_all(BIOPrivate);
	EVP_PKEY_CTX_free(Ctx);
	EVP_PKEY_free(PKey);
	EVP_PKEY_free(PKEYParams);
	return Success;
}

bool UCryptoHelperBPLibrary::AEncryptStringWithPublicKey(const FString& PlainString, const FString& PublicKey, FString& CipherString, EAsymmetricAlgorithm KeyType)
{
	if (PublicKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a valid PUBLIC key to encrypt"));
		return false;
	}
	if (KeyType != EAsymmetricAlgorithm::RSA)
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a RSA key to encrypt"));
		return false;
	}
	TArray<uint8> CipherBytes;
	std::string plain = std::string(TCHAR_TO_UTF8(*PlainString));
	TArray<uint8> In;
	In.Append((uint8*) plain.c_str(), plain.length());
	bool Res = AEncrypt(In, CipherBytes, PublicKey, KeyType);
	CipherString = BytesToHex(CipherBytes.GetData(), CipherBytes.Num());
	return Res;
}

bool UCryptoHelperBPLibrary::AEncryptBytesWithPublicKey(const TArray<uint8>& PlainBytes, const FString& PublicKey, TArray<uint8>& CipherBytes, EAsymmetricAlgorithm KeyType)
{
	if (PublicKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a valid PUBLIC key to encrypt"));
		return false;
	}
	if (KeyType != EAsymmetricAlgorithm::RSA)
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a RSA key to encrypt"));
		return false;
	}
	return AEncrypt(PlainBytes, CipherBytes, PublicKey, KeyType);
}

bool UCryptoHelperBPLibrary::AEncryptStringWithPrivateKey(const FString& PlainString, const FString& PrivateKey, FString& CipherString)
{
	int len = -1;
	int length = 0;
	TArray<uint8> CipherBytes;
	std::string prikey = std::string(TCHAR_TO_UTF8(*PrivateKey));
	BIO* bio = BIO_new_mem_buf((void*)prikey.c_str(), -1);
	RSA* rsaPrivateKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
	std::string plain = std::string(TCHAR_TO_UTF8(*PlainString));
	if (!rsaPrivateKey) goto free_all;
	length = RSA_size(rsaPrivateKey);
	CipherBytes.SetNum(length);
	len = RSA_private_encrypt(plain.length(), (const unsigned char*)plain.c_str(), (unsigned char*)CipherBytes.GetData(), rsaPrivateKey, RSA_PKCS1_PADDING);
	CipherString = BytesToHex(CipherBytes.GetData(), CipherBytes.Num());
free_all:
	BIO_free(bio);
	RSA_free(rsaPrivateKey);
	return (len != -1);
}

bool UCryptoHelperBPLibrary::AEncryptBytesWithPrivateKey(const TArray<uint8>& PlainBytes, const FString& PrivateKey, TArray<uint8>& CipherBytes)
{
	int len = -1;
	int length = 0;
	std::string prikey = std::string(TCHAR_TO_UTF8(*PrivateKey));
	BIO* bio = BIO_new_mem_buf((void*)prikey.c_str(), -1);
	RSA* rsaPrivateKey = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
	if (!rsaPrivateKey) goto free_all;
	length = RSA_size(rsaPrivateKey);
	CipherBytes.SetNum(length);
	len = RSA_private_encrypt(PlainBytes.Num(), (const unsigned char*)PlainBytes.GetData(), (unsigned char*)CipherBytes.GetData(), rsaPrivateKey, RSA_PKCS1_PADDING);
free_all:
	BIO_free(bio);
	RSA_free(rsaPrivateKey);
	return (len != -1);
}

bool UCryptoHelperBPLibrary::ADecryptStringWithPrivateKey(const FString& CipherString, const FString& PrivateKey, FString& PlainString, EAsymmetricAlgorithm KeyType)
{
	if (PrivateKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a valid PRIVATE key to decrypt"));
		return false;
	}
	if (KeyType != EAsymmetricAlgorithm::RSA)
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a RSA key to decrypt"));
		return false;
	}
	TArray<uint8> CipherBytes, PlainBytes;
	CipherBytes.SetNum(CipherString.Len() / 2);
	HexToBytes(CipherString, CipherBytes.GetData());
	bool Res = ADecrypt(CipherBytes, PlainBytes, PrivateKey, KeyType);
	PlainBytes.Add(0);
	PlainString = UBytesToString(PlainBytes); // FString(UTF8_TO_TCHAR(PlainBytes.GetData()));
	return Res;
}

bool UCryptoHelperBPLibrary::ADecryptBytesWithPrivateKey(const TArray<uint8>& CipherBytes, const FString& PrivateKey, TArray<uint8>& PlainBytes, EAsymmetricAlgorithm KeyType)
{
	if (PrivateKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a valid PRIVATE key to decrypt"));
		return false;
	}
	if (KeyType != EAsymmetricAlgorithm::RSA)
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a RSA key to decrypt"));
		return false;
	}
	return ADecrypt(CipherBytes, PlainBytes, PrivateKey, KeyType);
}

bool UCryptoHelperBPLibrary::ADecryptStringWithPublicKey(const FString& CipherString, const FString& PublicKey, FString& PlainString)
{
	TArray<uint8> CipherBytes, PlainBytes;
	CipherBytes.SetNum(CipherString.Len() / 2);
	HexToBytes(CipherString, CipherBytes.GetData());
	bool Res = ADecryptBytesWithPublicKey(CipherBytes, PublicKey, PlainBytes);
	PlainString = UBytesToString(PlainBytes); // FString(UTF8_TO_TCHAR(PlainBytes.GetData()));
	return Res;
}

bool UCryptoHelperBPLibrary::ADecryptBytesWithPublicKey(const TArray<uint8>& CipherBytes, const FString& PublicKey, TArray<uint8>& PlainBytes)
{
	int len = -1;
	int length = 0;
	std::string pubkey = std::string(TCHAR_TO_UTF8(*PublicKey));
	BIO* bio = BIO_new_mem_buf((void*)pubkey.c_str(), -1);
	RSA* rsaPublicKey = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL);
	if (!rsaPublicKey) goto free_all;
	length = RSA_size(rsaPublicKey);
	PlainBytes.SetNum(length);
	len = RSA_public_decrypt(length, (const unsigned char*)CipherBytes.GetData(), (unsigned char*)PlainBytes.GetData(), rsaPublicKey, RSA_PKCS1_PADDING);
	if (len > -1) PlainBytes.SetNum(len);
free_all:
	BIO_free(bio);
	RSA_free(rsaPublicKey);
	return (len != -1);
}

bool UCryptoHelperBPLibrary::ASign(const uint8* Data, uint32 Size, const FString& PrivateKey, FString& Signature, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType)
{
	if (!IsDigestSupported(HashType))
		return false;
	if (PrivateKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a valid PRIVATE key to sign"));
		return false;
	}
	int ret = -1;
	int length = 0;
	const EVP_MD* evp = NULL;
	TArray<uint8> SignatureBytes;
	EVP_PKEY* pkey = GetKeyEVP(KeyType, PrivateKey, true);
	EVP_MD_CTX* signCtx = NULL;
	if (!pkey) goto free_all;
	length = EVP_PKEY_size(pkey);
	evp = GetDigestEVP(HashType);
	if (!evp) goto free_all;
	signCtx = EVP_MD_CTX_create();
	ret = EVP_DigestSignInit(signCtx, NULL, evp, NULL, pkey);
	ret = EVP_DigestSignUpdate(signCtx, Data, Size);
	size_t siglen;
	ret = EVP_DigestSignFinal(signCtx, NULL, &siglen);
	SignatureBytes.SetNum(siglen);
	ret = EVP_DigestSignFinal(signCtx, SignatureBytes.GetData(), &siglen);
	Signature = BytesToHex(SignatureBytes.GetData(), siglen);
free_all:
	EVP_MD_CTX_destroy(signCtx);
	EVP_PKEY_free(pkey);
	return (ret == 1);
}

bool UCryptoHelperBPLibrary::AVerify(const uint8* Data, uint32 Size, const FString& PublicKey, const FString& Signature, bool& IsAuthentic, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType)
{
	IsAuthentic = false;
	if (!IsDigestSupported(HashType))
		return false;
	if (PublicKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a valid PUBLIC key to verify"));
		return false;
	}
	int authStatus = 0;
	EVP_MD_CTX* verifyCtx = NULL;
	const EVP_MD* evp = NULL;
	EVP_PKEY* pkey = NULL;
	TArray<uint8> SignBytes;
	SignBytes.SetNum(Signature.Len() / 2);
	HexToBytes(Signature, SignBytes.GetData());
	pkey = GetKeyEVP(KeyType, PublicKey, false);
	if (!pkey) goto free_all;
	verifyCtx = EVP_MD_CTX_create();
	evp = GetDigestEVP(HashType);
	if (!evp) goto free_all;
	if (EVP_DigestVerifyInit(verifyCtx, NULL, evp, NULL, pkey) <= 0) goto free_all;
	if (EVP_DigestVerifyUpdate(verifyCtx, Data, Size) <= 0) goto free_all;
	authStatus = EVP_DigestVerifyFinal(verifyCtx, (unsigned char*)SignBytes.GetData(), SignBytes.Num());
	IsAuthentic = authStatus == 1 ? true : false;
free_all:
	EVP_MD_CTX_destroy(verifyCtx);
	EVP_PKEY_free(pkey);
	return (authStatus == 1 || authStatus == 0);
}

bool UCryptoHelperBPLibrary::ASignString(const FString& Content, const FString& PrivateKey, FString& Signature, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType)
{
	std::string msg = std::string(TCHAR_TO_UTF8(*Content));
	return ASign((const uint8*)msg.c_str(), msg.size(), PrivateKey, Signature, KeyType, HashType);
}

bool UCryptoHelperBPLibrary::AVerifyString(const FString& Content, const FString& Signature, const FString& PublicKey, bool& IsAuthentic, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType)
{
	std::string msg = std::string(TCHAR_TO_UTF8(*Content));
	return AVerify((const uint8*)msg.c_str(), msg.size(), PublicKey, Signature, IsAuthentic, KeyType, HashType);
}

bool UCryptoHelperBPLibrary::ASignBytes(const TArray<uint8>& Message, const FString& PrivateKey, FString& Signature, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType)
{
	return ASign(Message.GetData(), Message.Num(), PrivateKey, Signature, KeyType, HashType);
}

bool UCryptoHelperBPLibrary::AVerifyBytes(const TArray<uint8>& Message, const FString& Signature, const FString& PublicKey, bool& IsAuthentic, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType)
{
	return AVerify(Message.GetData(), Message.Num(), PublicKey, Signature, IsAuthentic, KeyType, HashType);
}

bool UCryptoHelperBPLibrary::ASignFile(const FString& Path, const FString& PrivateKey, FString& Signature, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType)
{
	if (!IsDigestSupported(HashType))
		return false;
	if (PrivateKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a valid PRIVATE key to sign"));
		return false;
	}
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (file.FileExists(*Path))
	{
		bool Success = false;
		FArchive* Ar = nullptr;
		EVP_MD_CTX* signCtx = nullptr;
		const EVP_MD* evp = nullptr;

		EVP_PKEY* pkey = GetKeyEVP(KeyType, PrivateKey, true);
		if (!pkey) goto free_all;

		evp = GetDigestEVP(HashType);
		if (!evp) goto free_all;

		Ar = IFileManager::Get().CreateFileReader(*Path);
		if (!Ar) goto free_all;

		{
			int length = EVP_PKEY_size(pkey);

			TArray<uint8> Buffer;
			Buffer.SetNum(1024 * 64);

			const int64 Size = Ar->TotalSize();
			int64 Position = 0;

			signCtx = EVP_MD_CTX_create();
			int ret = EVP_DigestSignInit(signCtx, NULL, evp, NULL, pkey);

			while (Position < Size)
			{
				const auto ReadNum = FMath::Min(Size - Position, (int64)Buffer.Num());
				Ar->Serialize(Buffer.GetData(), ReadNum);
				ret = EVP_DigestSignUpdate(signCtx, Buffer.GetData(), ReadNum);
				Position += ReadNum;
			}

			size_t siglen;
			ret = EVP_DigestSignFinal(signCtx, NULL, &siglen);
			TArray<uint8> SignatureBytes;
			SignatureBytes.SetNum(siglen);
			ret = EVP_DigestSignFinal(signCtx, SignatureBytes.GetData(), &siglen);
			Signature = ret == 1 ? BytesToHex(SignatureBytes.GetData(), siglen) : "";
			Success = true;
		}
		
	free_all:
		EVP_PKEY_free(pkey);
		EVP_MD_CTX_destroy(signCtx);
		delete Ar;
		return Success;
	}
	Signature = FString();
	return false;
}

bool UCryptoHelperBPLibrary::AVerifyFile(const FString& Path, const FString& Signature, const FString& PublicKey, bool& IsAuthentic, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType)
{
	IsAuthentic = false;
	if (!IsDigestSupported(HashType))
		return false;
	if (PublicKey.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Provide a valid PUBLIC key to verify"));
		return false;
	}
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (file.FileExists(*Path))
	{
		int authStatus = -1;

		TArray<uint8> SignBytes;
		SignBytes.SetNum(Signature.Len() / 2);
		HexToBytes(Signature, SignBytes.GetData());

		EVP_MD_CTX* verifyCtx = nullptr;
		FArchive* Ar = nullptr;
		const EVP_MD* evp = nullptr;

		EVP_PKEY* pkey = GetKeyEVP(KeyType, PublicKey, false);
		if (!pkey) goto free_all;

		evp = GetDigestEVP(HashType);
		if (!evp) goto free_all;

		Ar = IFileManager::Get().CreateFileReader(*Path);
		if (!Ar) goto free_all;

		{
			int length = EVP_PKEY_size(pkey);

			TArray<uint8> Buffer;
			Buffer.SetNum(1024 * 64);

			const int64 Size = Ar->TotalSize();
			int64 Position = 0;

			verifyCtx = EVP_MD_CTX_create();
			int ret = EVP_DigestVerifyInit(verifyCtx, NULL, evp, NULL, pkey);

			while (Position < Size)
			{
				const auto ReadNum = FMath::Min(Size - Position, (int64)Buffer.Num());
				Ar->Serialize(Buffer.GetData(), ReadNum);
				ret = EVP_DigestVerifyUpdate(verifyCtx, Buffer.GetData(), ReadNum);
				Position += ReadNum;
			}

			authStatus = EVP_DigestVerifyFinal(verifyCtx, SignBytes.GetData(), SignBytes.Num());
			IsAuthentic = authStatus == 1 ? true : false;
		}

	free_all:
		EVP_MD_CTX_destroy(verifyCtx);
		EVP_PKEY_free(pkey);
		delete Ar;
		return (authStatus == 1 || authStatus == 0);
	}
	return false;
}

int32 UCryptoHelperBPLibrary::AGetKeySize(const FString Key, bool IsPublicKey, EAsymmetricAlgorithm KeyType)
{
	int32 Size = -1;
	if (Key.IsEmpty())
	{
		return Size;
	}
	EVP_PKEY* EVPKey = GetKeyEVP(KeyType, Key, !IsPublicKey);
	if (!EVPKey) goto free_all;
	Size = EVP_PKEY_size(EVPKey);
free_all:
	EVP_PKEY_free(EVPKey);
	return Size;
}

int32 UCryptoHelperBPLibrary::AGetMaxMessageSize(const FString Key, bool IsPublicKey, EAsymmetricAlgorithm KeyType)
{
	if (KeyType != EAsymmetricAlgorithm::RSA)
	{
		return -1;
	}
	int32 KeySize = AGetKeySize(Key, IsPublicKey, KeyType);
	return KeySize == -1 ? KeySize : KeySize - 42; // RSA_PKCS1_PADDING_SIZE
}

bool UCryptoHelperBPLibrary::KeccakFile(const FString& Path, FString& Hash, int32 Bits)
{
	if (Bits != 224 && Bits != 256 && Bits != 384 && Bits != 512)
	{
		return false;
	}
	Hash = HashKeccakFile(Path, Bits);
	return !Hash.IsEmpty();
}

bool UCryptoHelperBPLibrary::KeccakString(const FString& Message, FString& Hash, int32 Bits)
{
	if (Bits != 224 && Bits != 256 && Bits != 384 && Bits != 512)
	{
		return false;
	}
	TArray<uint8> In = UStringToBytes(Message);
	return KeccakBytes(In, Hash, Bits);
}

bool UCryptoHelperBPLibrary::KeccakBytes(const TArray<uint8>& Message, FString& Hash, int32 Bits)
{
	if (Bits != 224 && Bits != 256 && Bits != 384 && Bits != 512)
	{
		return false;
	}
	Hash = HashKeccak(Message, Bits);
	return !Hash.IsEmpty();
}

FString UCryptoHelperBPLibrary::UBytesToHex(const TArray<uint8>& Bytes)
{
	return BytesToHex(Bytes.GetData(), Bytes.Num());
}

TArray<uint8> UCryptoHelperBPLibrary::UHexToBytes(const FString& Hex)
{
	TArray<uint8> Bytes;
	Bytes.SetNum(Hex.Len() / 2);
	int32 Total = HexToBytes(Hex, Bytes.GetData());
	Bytes.SetNum(Total);
	return Bytes;
}

TArray<uint8> UCryptoHelperBPLibrary::UStringToBytes(const FString& Str)
{
	TArray<uint8> Bytes;
	std::string str = std::string(TCHAR_TO_UTF8(*Str));
	Bytes.Append((const uint8*) str.c_str(), str.size());
	return Bytes;
}

FString UCryptoHelperBPLibrary::UBytesToString(const TArray<uint8>& Bytes)
{
	if (Bytes.Last() != 0)
	{
		TArray<uint8> Copy(Bytes);
		Copy.Add(0);
		return FString(UTF8_TO_TCHAR(Copy.GetData()));
	}
	return FString(UTF8_TO_TCHAR(Bytes.GetData()));
}

TArray<uint8> UCryptoHelperBPLibrary::UBase64ToBytes(const FString& Str)
{
	TArray<uint8> Bytes;
	FBase64::Decode(Str, Bytes);
	return Bytes;
}

FString UCryptoHelperBPLibrary::UBytesToBase64(const TArray<uint8>& Bytes)
{
	return FBase64::Encode(Bytes);
}

FString UCryptoHelperBPLibrary::UStringToBase64(const FString& UTFStr)
{
	return FBase64::Encode(UTFStr);
}

FString UCryptoHelperBPLibrary::UBase64ToString(const FString& Base64)
{
	FString UTFStr;
	FBase64::Decode(Base64, UTFStr);
	return UTFStr;
}

bool UCryptoHelperBPLibrary::IsDigestSupported(EDigestAlgorithm HashType)
{
	return GetDigestEVP(HashType) != NULL;
}

const EVP_MD* UCryptoHelperBPLibrary::GetDigestEVP(EDigestAlgorithm HashType)
{
	OpenSSL_add_all_digests();
	const EVP_MD* evp = nullptr;
	switch (HashType)
	{
		case(EDigestAlgorithm::MDC2):
		evp = EVP_get_digestbyname("mdc2"); // EVP_mdc2();
		break;
		case(EDigestAlgorithm::MD4):
		evp = EVP_md4();
		break;
		case(EDigestAlgorithm::MD5):
		evp = EVP_md5();
		break;
		case(EDigestAlgorithm::MD5_SHA1):
		evp = EVP_get_digestbyname("md5-sha1"); //  EVP_md5_sha1(); 
		break;
		case(EDigestAlgorithm::SHA1):
		evp = EVP_sha1();
		break;
		case(EDigestAlgorithm::SHA224):
		evp = EVP_sha224();
		break;
		case(EDigestAlgorithm::SHA256):
		evp = EVP_sha256();
		break;
		case(EDigestAlgorithm::SHA384):
		evp = EVP_sha384();
		break;
		case(EDigestAlgorithm::SHA512_224):
		evp = EVP_get_digestbyname("sha512-224"); //  EVP_sha512_224();
		break;
		case(EDigestAlgorithm::SHA512_256):
		evp = EVP_get_digestbyname("sha512-256"); //  EVP_sha512_256();
		break;
		case(EDigestAlgorithm::SHA3_224):
		evp = EVP_get_digestbyname("sha3-224"); //  EVP_sha3_224();
		break;
		case(EDigestAlgorithm::SHA3_256):
		evp = EVP_get_digestbyname("sha3-256"); //  EVP_sha3_256();
		break;
		case(EDigestAlgorithm::SHA3_384):
		evp = EVP_get_digestbyname("sha3-384"); //  EVP_sha3_384();
		break;
		case(EDigestAlgorithm::SHA3_512):
		evp = EVP_get_digestbyname("sha3-512"); //  EVP_sha3_512();
		break;
		case(EDigestAlgorithm::SHA512):
		evp = EVP_sha512();
		break;
		case(EDigestAlgorithm::RIPEMD160):
		evp = EVP_ripemd160();
		break;
		case(EDigestAlgorithm::WHIRLPOOL):
		evp = EVP_get_digestbyname("whirlpool"); // EVP_whirlpool();
		break;
		case(EDigestAlgorithm::SHAKE128):
		evp = EVP_get_digestbyname("shake128"); // EVP_shake128();
		break;
		case(EDigestAlgorithm::SHAKE256):
		evp = EVP_get_digestbyname("shake256"); // EVP_shake256();
		break;
	}
	return evp;
}

const EVP_CIPHER* UCryptoHelperBPLibrary::GetCipherEVP(ESymmetricAlgorithm Type)
{
	OpenSSL_add_all_ciphers();
	const EVP_CIPHER* evp = nullptr;
	switch (Type)
	{
		case(ESymmetricAlgorithm::AES_256_CBC):
			evp = EVP_aes_256_cbc();
		break;
		case(ESymmetricAlgorithm::AES_256_ECB):
			evp = EVP_aes_256_ecb();
		break;
		case(ESymmetricAlgorithm::AES_128_CBC):
			evp = EVP_aes_128_cbc();
			break;
		case(ESymmetricAlgorithm::AES_128_ECB):
			evp = EVP_aes_128_ecb();
		break;
		case(ESymmetricAlgorithm::AES_192_CBC):
			evp = EVP_aes_192_cbc();
		break;
		case(ESymmetricAlgorithm::AES_192_ECB):
			evp = EVP_aes_192_ecb();
		break;
		case(ESymmetricAlgorithm::BLOWFISH_CBC):
			evp = EVP_bf_cbc();
		break;
		case(ESymmetricAlgorithm::BLOWFISH_ECB):
			evp = EVP_bf_ecb();
		break;
		case(ESymmetricAlgorithm::CAMELLIA_128_CBC):
			evp = EVP_get_cipherbyname("camellia-128-cbc"); // EVP_camellia_128_cbc();
		break;
		case(ESymmetricAlgorithm::CAMELLIA_128_ECB):
			evp = EVP_get_cipherbyname("camellia-128-ecb"); // EVP_camellia_128_ecb();
		break;
		case(ESymmetricAlgorithm::CAMELLIA_192_CBC):
			evp = EVP_get_cipherbyname("camellia-192-cbc"); // EVP_camellia_192_cbc();
		break;
		case(ESymmetricAlgorithm::CAMELLIA_192_ECB):
			evp = EVP_get_cipherbyname("camellia-192-ecb"); // EVP_camellia_192_ecb();
		break;
		case(ESymmetricAlgorithm::CAMELLIA_256_CBC):
			evp = EVP_get_cipherbyname("camellia-256-cbc"); // EVP_camellia_256_cbc();
		break;
		case(ESymmetricAlgorithm::CAMELLIA_256_ECB):
			evp = EVP_get_cipherbyname("camellia-256-ecb"); // EVP_camellia_256_ecb();
		break;
	}
	return evp;
}

EVP_PKEY* UCryptoHelperBPLibrary::GetKeyEVP(EAsymmetricAlgorithm Type, const FString& Key, bool IsPrivateKey)
{
	std::string StrKey = std::string(TCHAR_TO_UTF8(*Key));
	BIO* Bio = BIO_new_mem_buf((void*)StrKey.c_str(), -1);
	EVP_PKEY* PKey = IsPrivateKey ? PEM_read_bio_PrivateKey(Bio, NULL, NULL, NULL) : PEM_read_bio_PUBKEY(Bio, NULL, NULL, NULL);
	int KeyId = EVP_PKEY_id(PKey);
	switch (Type)
	{
		case(EAsymmetricAlgorithm::RSA):
			if (KeyId != EVP_PKEY_RSA)
			{
				EVP_PKEY_free(PKey);
				PKey = nullptr;
			}
		break;
		case(EAsymmetricAlgorithm::DSA):
			if (KeyId != EVP_PKEY_DSA)
			{
				EVP_PKEY_free(PKey);
				PKey = nullptr;
			}
		break;
		default:
			EVP_PKEY_free(PKey);
			PKey = nullptr;
		break;
	}
	BIO_free(Bio);
	return PKey;
}

FString UCryptoHelperBPLibrary::Digest(const uint8* Data, uint32 Size, EDigestAlgorithm HashType)
{
	if (!IsDigestSupported(HashType)) 
		return FString();
	EVP_MD_CTX* ctx = EVP_MD_CTX_create();
	const EVP_MD* evp = GetDigestEVP(HashType);
	int ret = EVP_DigestInit(ctx, evp);
	ret = EVP_DigestUpdate(ctx, Data, Size);
	unsigned int length = 0;
	TArray<uint8> HashBytes;
	HashBytes.SetNum(EVP_MAX_MD_SIZE);
	ret = EVP_DigestFinal(ctx, HashBytes.GetData(), &length);
	EVP_MD_CTX_destroy(ctx);
	return ret == 1 ? BytesToHex(HashBytes.GetData(), length) : "";
}

FString UCryptoHelperBPLibrary::HashKeccak(const TArray<uint8>& In, uint32 Bits)
{
	TArray<uint8> Out;
	Out.SetNum(Bits / 8);
	sha3_return_t Res = sha3_HashBuffer(Bits, SHA3_FLAGS_KECCAK, In.GetData(), In.Num(), Out.GetData(), Out.Num());
	return Res == sha3_return_t::SHA3_RETURN_OK ? UBytesToHex(Out) : "";
}

FString UCryptoHelperBPLibrary::HashKeccakFile(const FString& Path, uint32 Bits)
{
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (file.FileExists(*Path))
	{
		FArchive* Ar = IFileManager::Get().CreateFileReader(*Path);
		if (!Ar) return FString();

		TArray<uint8> Buffer;
		Buffer.SetNum(1024 * 64);

		const int64 Size = Ar->TotalSize();
		int64 Position = 0;

		sha3_context ctx;
		sha3_return_t ret = sha3_Init((void*)&ctx, Bits);
		sha3_SetFlags(&ctx, SHA3_FLAGS_KECCAK);

		while (Position < Size)
		{
			const auto ReadNum = FMath::Min(Size - Position, (int64)Buffer.Num());
			Ar->Serialize(Buffer.GetData(), ReadNum);
			sha3_Update((void*)&ctx, Buffer.GetData(), ReadNum);
			Position += ReadNum;
		}

		TArray<uint8> HashBytes;
		HashBytes.Append((const uint8*)sha3_Finalize((void*)&ctx), Bits / 8);

		delete Ar;
		return (ret == 0 ? UBytesToHex(HashBytes) : "");
	}
	return FString();
}

FString UCryptoHelperBPLibrary::HMAC(const uint8* Data, uint32 Size, const FString& Key, EDigestAlgorithm HashType)
{
	if (!IsDigestSupported(HashType))
		return FString();
	EVP_MD_CTX* mdctx = NULL;
	EVP_PKEY* pkey = NULL;
	std::string key = TCHAR_TO_UTF8(*Key);
	TArray<uint8> Hash;
	Hash.SetNum(EVP_MAX_MD_SIZE);
	size_t md_len = 0;
	const EVP_MD* evp = GetDigestEVP(HashType);
	mdctx = EVP_MD_CTX_create();
	if (!(mdctx)) goto free_all;
	pkey = EVP_PKEY_new_mac_key(EVP_PKEY_HMAC, NULL, (const unsigned char*)key.c_str(), key.size());
	if (!(pkey)) goto free_all;
	if (1 != EVP_DigestSignInit(mdctx, NULL, evp, NULL, pkey)) goto free_all;
	if (1 != EVP_DigestSignUpdate(mdctx, Data, Size)) goto free_all;
	if (1 != EVP_DigestSignFinal(mdctx, Hash.GetData(), &md_len)) goto free_all;
	Hash.SetNum(md_len);
free_all:
	EVP_PKEY_free(pkey);
	return BytesToHex(Hash.GetData(), Hash.Num());
}

bool UCryptoHelperBPLibrary::SEncryptDecryptFile(bool Encrypt, const FString& InPath, const FString& OutPath, const FString& Key, const FString& IV, ESymmetricAlgorithm Type)
{
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if (file.FileExists(*InPath))
	{
		const EVP_CIPHER* cipher = GetCipherEVP(Type);
		if (!cipher) return false;

		int BlockSize = EVP_CIPHER_block_size(cipher);
		int KeySize = EVP_CIPHER_key_length(cipher);
		int IVSize = EVP_CIPHER_iv_length(cipher);

		TArray<uint8> KeyBytes;
		if (Key.GetCharArray().Num() > 0)
			KeyBytes.Append((uint8*)TCHAR_TO_UTF8(Key.GetCharArray().GetData()), Key.GetCharArray().Num() - 1);
		TArray<uint8> IVBytes;
		if (IV.GetCharArray().Num() > 0)
			IVBytes.Append((uint8*)TCHAR_TO_UTF8(IV.GetCharArray().GetData()), IV.GetCharArray().Num() - 1);

		if (KeyBytes.Num() != KeySize || IVBytes.Num() != IVSize) return false;

		FArchive* In = IFileManager::Get().CreateFileReader(*InPath);
		if (!In) return false;

		FArchive* Out = IFileManager::Get().CreateFileWriter(*OutPath);
		if (!Out) return false;

		TArray<uint8> InBuffer;
		InBuffer.SetNum(1024 * 64);

		int OutLength = 0;
		TArray<uint8> OutBuffer;
		OutBuffer.SetNum(1024 * 64 + BlockSize);

		const int64 Size = In->TotalSize();
		int64 Position = 0;

		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		EVP_CipherInit_ex(ctx, cipher, NULL, KeyBytes.GetData(), IVBytes.GetData(), Encrypt);

		while (Position < Size)
		{
			const auto ReadNum = FMath::Min(Size - Position, (int64)InBuffer.Num());
			In->Serialize(InBuffer.GetData(), ReadNum);

			OutLength = 0;
			EVP_CipherUpdate(ctx, OutBuffer.GetData(), &OutLength, InBuffer.GetData(), ReadNum);
			Out->Serialize(OutBuffer.GetData(), OutLength);

			Position += ReadNum;
		}
		OutLength = 0;
		EVP_CipherFinal_ex(ctx, OutBuffer.GetData(), &OutLength);
		Out->Serialize(OutBuffer.GetData(), OutLength);
		Out->Close();
		In->Close();

		EVP_CIPHER_CTX_free(ctx);
		delete In;
		delete Out;
		return true;
	}
	return false;
}
bool UCryptoHelperBPLibrary::SEncryptDecrypt(bool Encrypt, const TArray<uint8>& In, TArray<uint8>& Out, const FString& Key, const FString& IV, ESymmetricAlgorithm Type)
{
	bool Res = false;
	EVP_CIPHER_CTX* ctx;
	int len;
	int ciphertext_len;
	TArray<uint8> KeyBytes;
	if (Key.GetCharArray().Num() > 0)
		KeyBytes.Append((uint8*)TCHAR_TO_UTF8(Key.GetCharArray().GetData()), Key.GetCharArray().Num() - 1);
	TArray<uint8> IVBytes;
	if (IV.GetCharArray().Num() > 0)
		IVBytes.Append((uint8*)TCHAR_TO_UTF8(IV.GetCharArray().GetData()), IV.GetCharArray().Num() - 1);
	const EVP_CIPHER* cipher = GetCipherEVP(Type);
	if (!cipher) return false;
	int BlockSize = EVP_CIPHER_block_size(cipher);
	int KeySize = EVP_CIPHER_key_length(cipher);
	int IVSize = EVP_CIPHER_iv_length(cipher);
	Out.SetNum(Encrypt ? (In.Num() + BlockSize) : In.Num()); // AES_BLOCK_SIZE
	ctx = EVP_CIPHER_CTX_new();
	if (!(ctx)) goto free_all;
	if (KeyBytes.Num() != KeySize || IVBytes.Num() != IVSize) goto free_all; // AES_BLOCK_SIZE
	if (1 != EVP_CipherInit_ex(ctx, cipher, NULL, KeyBytes.GetData(), IVBytes.GetData(), Encrypt)) goto free_all;
	if (1 != EVP_CipherUpdate(ctx, Out.GetData(), &len, In.GetData(), In.Num())) goto free_all;
	ciphertext_len = len;
	if (1 != EVP_CipherFinal_ex(ctx, Out.GetData() + len, &len)) goto free_all;
	ciphertext_len += len;
	Out.SetNum(ciphertext_len);
	Res = true;
free_all:
	EVP_CIPHER_CTX_free(ctx);
	return Res;
}

bool UCryptoHelperBPLibrary::AEncrypt(const TArray<uint8>& In, TArray<uint8>& Out, const FString& PublicKey, EAsymmetricAlgorithm KeyType)
{
	bool Success = false;
	size_t OutLen;
	EVP_PKEY* Key = GetKeyEVP(KeyType, PublicKey, false);
	EVP_PKEY_CTX* Ctx = EVP_PKEY_CTX_new(Key, NULL);
	if (!Ctx) goto free_all;
	if (EVP_PKEY_encrypt_init(Ctx) <= 0) goto free_all;
	if (KeyType == EAsymmetricAlgorithm::RSA && EVP_PKEY_CTX_set_rsa_padding(Ctx, RSA_PKCS1_OAEP_PADDING) <= 0) goto free_all;
	if (EVP_PKEY_encrypt(Ctx, NULL, &OutLen, In.GetData(), In.Num()) <= 0) goto free_all;
	Out.SetNum(OutLen);
	if (EVP_PKEY_encrypt(Ctx, Out.GetData(), &OutLen, In.GetData(), In.Num()) <= 0) goto free_all;
	Success = true;
free_all:
	// free
	EVP_PKEY_CTX_free(Ctx);
	EVP_PKEY_free(Key);
	return Success;
}

bool UCryptoHelperBPLibrary::ADecrypt(const TArray<uint8>& In, TArray<uint8>& Out, const FString& PrivateKey, EAsymmetricAlgorithm KeyType)
{
	bool Success = false;
	size_t OutLen;
	EVP_PKEY* Key = GetKeyEVP(KeyType, PrivateKey, true);
	EVP_PKEY_CTX* Ctx = EVP_PKEY_CTX_new(Key, NULL);
	if (!Ctx) goto free_all;
	if (EVP_PKEY_decrypt_init(Ctx) <= 0) goto free_all;
	if (KeyType == EAsymmetricAlgorithm::RSA && EVP_PKEY_CTX_set_rsa_padding(Ctx, RSA_PKCS1_OAEP_PADDING) <= 0) goto free_all;
	if (EVP_PKEY_decrypt(Ctx, NULL, &OutLen, In.GetData(), In.Num()) <= 0) goto free_all;
	Out.SetNum(OutLen);
	if (EVP_PKEY_decrypt(Ctx, Out.GetData(), &OutLen, In.GetData(), In.Num()) <= 0) goto free_all;
	Success = true;
free_all:
	// free
	EVP_PKEY_CTX_free(Ctx);
	EVP_PKEY_free(Key);
	return Success;
}



