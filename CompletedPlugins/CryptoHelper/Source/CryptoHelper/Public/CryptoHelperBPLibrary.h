// Copyright RLoris 2021

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#define UI UI_ST
THIRD_PARTY_INCLUDES_START
#include "openssl/evp.h"
THIRD_PARTY_INCLUDES_END
#undef UI
#include "CryptoHelperBPLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu.
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UENUM(BlueprintType)
enum class EDigestAlgorithm : uint8 {
	MDC2				UMETA(DisplayName = "MDC2"),
	MD4					UMETA(DisplayName = "MD4"),
	MD5					UMETA(DisplayName = "MD5"),
	MD5_SHA1			UMETA(DisplayName = "MD5/SHA1"),
	SHA1				UMETA(DisplayName = "SHA1"),
	SHA224				UMETA(DisplayName = "SHA224"),
	SHA256				UMETA(DisplayName = "SHA256"),
	SHA384				UMETA(DisplayName = "SHA384"),
	SHA512_224			UMETA(DisplayName = "SHA512/224"),
	SHA512_256			UMETA(DisplayName = "SHA512/256"),
	SHA512				UMETA(DisplayName = "SHA512"),
	SHA3_224			UMETA(DisplayName = "SHA3_224"),
	SHA3_256			UMETA(DisplayName = "SHA3_256"),
	SHA3_384			UMETA(DisplayName = "SHA3_384"),
	SHA3_512			UMETA(DisplayName = "SHA3_512"),
	RIPEMD160			UMETA(DisplayName = "RIPEMD160"),
	WHIRLPOOL			UMETA(DisplayName = "WHIRLPOOL"),
	SHAKE128			UMETA(DisplayName = "SHAKE128"),
	SHAKE256			UMETA(DisplayName = "SHAKE256")
};

UENUM(BlueprintType)
enum class ESymmetricAlgorithm : uint8 {
	AES_256_CBC			UMETA(DisplayName = "AES_256_CBC"),
	AES_256_ECB			UMETA(DisplayName = "AES_256_ECB"),
	AES_192_CBC			UMETA(DisplayName = "AES_192_CBC"),
	AES_192_ECB			UMETA(DisplayName = "AES_192_ECB"),
	AES_128_CBC			UMETA(DisplayName = "AES_128_CBC"),
	AES_128_ECB			UMETA(DisplayName = "AES_128_ECB"),
	BLOWFISH_CBC		UMETA(DisplayName = "BLOWFISH_CBC"),
	BLOWFISH_ECB		UMETA(DisplayName = "BLOWFISH_ECB"),
	CAMELLIA_128_CBC	UMETA(DisplayName = "CAMELLIA_128_CBC"),
	CAMELLIA_128_ECB	UMETA(DisplayName = "CAMELLIA_128_ECB"),
	CAMELLIA_192_CBC	UMETA(DisplayName = "CAMELLIA_192_CBC"),
	CAMELLIA_192_ECB	UMETA(DisplayName = "CAMELLIA_192_ECB"),
	CAMELLIA_256_CBC	UMETA(DisplayName = "CAMELLIA_256_CBC"),
	CAMELLIA_256_ECB	UMETA(DisplayName = "CAMELLIA_256_ECB")
};

UENUM(BlueprintType)
enum class EAsymmetricAlgorithm : uint8 {
	RSA					UMETA(DisplayName = "RSA"),
	DSA					UMETA(DisplayName = "DSA")
	// TODO : DSA & ECDSA
};

USTRUCT(BlueprintType)
struct CRYPTOHELPER_API FKeychainOptions {
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadWrite, Category = "Asymmetric")
	EAsymmetricAlgorithm Type = EAsymmetricAlgorithm::RSA;
	UPROPERTY(BlueprintReadWrite, Category = "Asymmetric")
	int32 Size = 2048;
};

USTRUCT(BlueprintType)
struct CRYPTOHELPER_API FKeychain {
	GENERATED_BODY();
public:
	UPROPERTY(BlueprintReadOnly, Category = "Asymmetric")
	FString PublicKey;
	UPROPERTY(BlueprintReadOnly, Category = "Asymmetric")
	FString PrivateKey;
	UPROPERTY(BlueprintReadOnly, Category = "Asymmetric")
	FKeychainOptions Options;
};

UCLASS()
class CRYPTOHELPER_API UCryptoHelperBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
public:
	// todo: CRC32, raw functions
	/* Keccak */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "KeccakFile"), Category = "Misc|Keccak")
	static bool KeccakFile(const FString& Path, FString& Hash, int32 Bits = 256);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "KeccakString"), Category = "Misc|Keccak")
	static bool KeccakString(const FString& Message, FString& Hash, int32 Bits = 256);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "KeccakBytes"), Category = "Misc|Keccak")
	static bool KeccakBytes(const TArray<uint8>& Message, FString& Hash, int32 Bits = 256);
	/* Utility */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "BytesToHex"), Category = "Utility|Hex")
	static FString UBytesToHex(const TArray<uint8>& Bytes);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HexToBytes"), Category = "Utility|Hex")
	static TArray<uint8> UHexToBytes(const FString& Hex);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StringToBytes"), Category = "Utility|UTF")
	static TArray<uint8> UStringToBytes(const FString& UTFStr);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "BytesToString"), Category = "Utility|UTF")
	static FString UBytesToString(const TArray<uint8>& Bytes);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Base64ToBytes"), Category = "Utility|Base64")
	static TArray<uint8> UBase64ToBytes(const FString& Base64);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "BytesToBase64"), Category = "Utility|Base64")
	static FString UBytesToBase64(const TArray<uint8>& Bytes);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StringToBase64"), Category = "Utility|Base64")
	static FString UStringToBase64(const FString& UTFStr);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Base64ToString"), Category = "Utility|Base64")
	static FString UBase64ToString(const FString& Base64);
	/* Digest */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsHashSupported"), Category = "Hash")
	static bool IsDigestSupported(EDigestAlgorithm HashType);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HashString"), Category = "Hash|String")
	static bool DigestString(const FString& Message, FString& Hash, EDigestAlgorithm HashType = EDigestAlgorithm::SHA256);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HashBytes"), Category = "Hash|Bytes")
	static bool DigestBytes(const TArray<uint8>& Message, FString& Hash, EDigestAlgorithm HashType = EDigestAlgorithm::SHA256);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HashFile"), Category = "Hash|File")
	static bool DigestFile(const FString& Path, FString& Hash, EDigestAlgorithm HashType = EDigestAlgorithm::SHA256);
	/* Salt */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RandomDictionaryString"), Category = "Salt|String")
	static bool RandomDictionaryString(FString& Salt, int32 Length = 16, FString Dictionary = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RandomBytes"), Category = "Salt|Bytes")
	static bool RandomBytes(TArray<uint8>& Salt, int32 Length = 16);
	/* HMAC */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HMACFile"), Category = "HMAC|File")
	static bool HMACFile(const FString& Path, const FString& Key, FString& Hash, EDigestAlgorithm HashType = EDigestAlgorithm::SHA256);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HMACString"), Category = "HMAC|String")
	static bool HMACString(const FString& String, const FString& Key, FString& Hash, EDigestAlgorithm HashType = EDigestAlgorithm::SHA256);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HMACBytes"), Category = "HMAC|Bytes")
	static bool HMACBytes(const TArray<uint8>& Bytes, const FString& Key, FString& Hash, EDigestAlgorithm HashType = EDigestAlgorithm::SHA256);
	/* Symmetric encryption */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "IsSymmetricSupported"), Category = "Symmetric|Key")
	static bool IsSymmetricSupported(ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetIVSize"), Category = "Symmetric|Key")
	static int32 SGetIVSize(ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetKeySize"), Category = "Symmetric|Key")
	static int32 SGetKeySize(ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "EncryptString"), Category = "Symmetric|String")
	static bool SEncryptString(const FString& PlainString, const FString& Key, const FString& IV, FString& CipherString, ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DecryptString"), Category = "Symmetric|String")
	static bool SDecryptString(const FString& CipherString, const FString& Key, const FString& IV, FString& PlainString, ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "EncryptBytes"), Category = "Symmetric|Bytes")
	static bool SEncryptBytes(const TArray<uint8>& PlainBytes, const FString& Key, const FString& IV, TArray<uint8>& CipherBytes, ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DecryptBytes"), Category = "Symmetric|Bytes")
	static bool SDecryptBytes(const TArray<uint8>& CipherBytes, const FString& Key, const FString& IV, TArray<uint8>& PlainBytes, ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "EncryptFile"), Category = "Symmetric|File")
	static bool SEncryptFile(const FString& InPath, const FString& OutPath, const FString& Key, const FString& IV, ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DecryptFile"), Category = "Symmetric|File")
	static bool SDecryptFile(const FString& InPath, const FString& OutPath, const FString& Key, const FString& IV, ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	/* Asymmetric encryption */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GenerateKeychain"), Category = "Asymmetric|Key")
	static bool AGenerateKeychain(const FKeychainOptions& Options, FKeychain& Keychain);
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetKeySize"), Category = "Asymmetric|Key")
	static int32 AGetKeySize(const FString Key, bool IsPublicKey, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA);
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetMaxMessageSize"), Category = "Asymmetric|Key")
	static int32 AGetMaxMessageSize(const FString Key, bool IsPublicKey, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA);
	/* Encrypt w public key */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "EncryptString"), Category = "Asymmetric|Encrypt|String")
	static bool AEncryptStringWithPublicKey(const FString& PlainString, const FString& PublicKey, FString& CipherString, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "EncryptBytes"), Category = "Asymmetric|Encrypt|Bytes")
	static bool AEncryptBytesWithPublicKey(const TArray<uint8>& PlainBytes, const FString& PublicKey, TArray<uint8>& CipherBytes, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA);
	/* Decrypt w private key */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DecryptString"), Category = "Asymmetric|Decrypt|String")
	static bool ADecryptStringWithPrivateKey(const FString& CipherString, const FString& PrivateKey, FString& PlainString, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DecryptBytes"), Category = "Asymmetric|Decrypt|Bytes")
	static bool ADecryptBytesWithPrivateKey(const TArray<uint8>& CipherBytes, const FString& PrivateKey, TArray<uint8>& PlainBytes, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA);
	/* Sign & Verify */
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SignString"), Category = "Asymmetric|Sign|String")
	static bool ASignString(const FString& Message, const FString& PrivateKey, FString& Signature, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA, EDigestAlgorithm HashType = EDigestAlgorithm::SHA1);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VerifyString"), Category = "Asymmetric|Verify|String")
	static bool AVerifyString(const FString& Message, const FString& Signature, const FString& PublicKey, bool& IsAuthentic, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA, EDigestAlgorithm HashType = EDigestAlgorithm::SHA1);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SignBytes"), Category = "Asymmetric|Sign|Bytes")
	static bool ASignBytes(const TArray<uint8>& Message, const FString& PrivateKey, FString& Signature, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA, EDigestAlgorithm HashType = EDigestAlgorithm::SHA1);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VerifyBytes"), Category = "Asymmetric|Verify|Bytes")
	static bool AVerifyBytes(const TArray<uint8>& Message, const FString& Signature, const FString& PublicKey, bool& IsAuthentic, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA, EDigestAlgorithm HashType = EDigestAlgorithm::SHA1);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SignFile"), Category = "Asymmetric|Sign|File")
	static bool ASignFile(const FString& Path, const FString& PrivateKey, FString& Signature, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA, EDigestAlgorithm HashType = EDigestAlgorithm::SHA1);
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "VerifyFile"), Category = "Asymmetric|Verify|File")
	static bool AVerifyFile(const FString& Path, const FString& Signature, const FString& PublicKey, bool& IsAuthentic, EAsymmetricAlgorithm KeyType = EAsymmetricAlgorithm::RSA, EDigestAlgorithm HashType = EDigestAlgorithm::SHA1);
private:
	/* EVP API */
	static const EVP_MD* GetDigestEVP(EDigestAlgorithm HashType);
	static const EVP_CIPHER* GetCipherEVP(ESymmetricAlgorithm Type);
	static EVP_PKEY* GetKeyEVP(EAsymmetricAlgorithm Type, const FString& Key, bool IsPrivateKey);
	/* Atomic Hash */
	static FString Digest(const uint8* Data, uint32 Size, EDigestAlgorithm HashType);	
	static FString HashKeccak(const TArray<uint8>& In, uint32 Bits);
	static FString HashKeccakFile(const FString& Path, uint32 Bits);
	/* Atomic HMAC */
	static FString HMAC(const uint8* Data, uint32 Size, const FString& Key, EDigestAlgorithm HashType);
	/* Atomic Symmetric */
	static bool SEncryptDecryptFile(bool Encrypt, const FString& InPath, const FString& OutPath, const FString& Key, const FString& IV, ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	static bool SEncryptDecrypt(bool Encrypt, const TArray<uint8>& In, TArray<uint8>& Out, const FString& Key, const FString& IV, ESymmetricAlgorithm Type = ESymmetricAlgorithm::AES_256_CBC);
	/* Atomic Asymmetric */
	static bool AEncrypt(const TArray<uint8>& In, TArray<uint8>& Out, const FString& PublicKey, EAsymmetricAlgorithm KeyType);
	static bool ADecrypt(const TArray<uint8>& In, TArray<uint8>& Out, const FString& PrivateKey, EAsymmetricAlgorithm KeyType);
	/* Sign & verify */
	static bool ASign(const uint8* Data, uint32 Size, const FString& PrivateKey, FString& Signature, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType);
	static bool AVerify(const uint8* Data, uint32 Size, const FString& PublicKey, const FString& Signature, bool& IsAuthentic, EAsymmetricAlgorithm KeyType, EDigestAlgorithm HashType);
	// unused
	/* RSA Encrypt w private key */
	static bool AEncryptStringWithPrivateKey(const FString& PlainString, const FString& PrivateKey, FString& CipherString);
	static bool AEncryptBytesWithPrivateKey(const TArray<uint8>& PlainBytes, const FString& PrivateKey, TArray<uint8>& CipherBytes);
	/* RSA Decrypt w public key */
	static bool ADecryptStringWithPublicKey(const FString& CipherString, const FString& PublicKey, FString& PlainString);
	static bool ADecryptBytesWithPublicKey(const TArray<uint8>& CipherBytes, const FString& PublicKey, TArray<uint8>& PlainBytes);
};
