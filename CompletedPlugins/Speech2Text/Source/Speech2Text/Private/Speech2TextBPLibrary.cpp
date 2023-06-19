// Copyright Epic Games, Inc. All Rights Reserved.

#include "Speech2TextBPLibrary.h"
#include "Speech2Text.h"

#include <ctime>
#include <string>


USpeech2TextBPLibrary::USpeech2TextBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

std::string USpeech2TextBPLibrary::urlEncode(std::string str) {
	std::string new_str = "";
	char c;
	int ic;
	const char* chars = str.c_str();
	char bufHex[10];
	int len = strlen(chars);

	for (int i = 0; i < len; i++) {
		c = chars[i];
		ic = c;
		// uncomment this if you want to encode spaces with +
		/*if (c==' ') new_str += '+';
		else */if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~' || c == '=') new_str += c;
		else {
			sprintf(bufHex, "%X", c);
			if (ic < 16)
				new_str += "%0";
			else
				new_str += "%";
			new_str += bufHex;
		}
	}
	return new_str;
}

float USpeech2TextBPLibrary::Speech2TextInitial(const FString& appID, const FString& appKey, FString& request)
{
	std::time_t current_time;
	std::time(&current_time);
	std::string base_url = "ws://rtasr.xfyun.cn/v1/ws";
	std::string timeStamp = std::to_string(current_time);
	std::string baseString = "";
	baseString.append(TCHAR_TO_UTF8(*appID));
	baseString.append(timeStamp);

	printf("%s\n", baseString.c_str());

	baseString = md5(baseString);

	printf("%s\n", baseString.c_str());

	uint8_t* t_key = (uint8_t*)TCHAR_TO_UTF8(*appKey);
	uint8_t* t_data = (uint8_t*)baseString.c_str();
	uint8_t signa[256] = { 0 };
	size_t t_len = sizeof(signa);
	hmac_sha1(t_key, appKey.Len(), t_data, baseString.length(), signa, &t_len);

	for (int t_i = 0; t_i < t_len; t_i++)
		printf("%x ", signa[t_i]);

	std::string signaEncoded = urlEncode(base64_encode(signa, t_len));

	std::string r = "";
	r.append(base_url);
	r.append("?appid=");
	r.append(TCHAR_TO_UTF8(*appID));
	r.append("&ts=");
	r.append(timeStamp);
	r.append("&signa=");
	r.append(signaEncoded);

	request = UTF8_TO_TCHAR(r.c_str());

	return 0;
}

float USpeech2TextBPLibrary::Speech2TextBytesDivide(const TArray<uint8>& Bytes, TArray<FString>& Strings, int& ChunkCount) {
	ChunkCount = Bytes.Num() / 1280 + (Bytes.Num() % 1280 == 0 ? 0 : 1);

	char** bytesOri = (char**)malloc(ChunkCount);
	for (int l = 0; l < ChunkCount; l++) {
		bytesOri[l] = (char*)malloc(1280);
		for (int j = 0; j < 1280; j++) {
			if (l * 1280 + j < Bytes.Num())
				bytesOri[l][j] = Bytes[l * 1280 + j];
			else
				bytesOri[l][j] = 0;
		}
		Strings.Add(bytesOri[l]);
	}

	//int finalBytes = (Bytes.Num() % 1280 == 0 ? 1280 : Bytes.Num() % 1280);
	//char* bytesOriFinal = (char*)malloc(finalBytes);
	//for (int j = 0; j < finalBytes; j++) {
	//	bytesOriFinal[j] = Bytes[ChunkCount - 1 * 1280 + j];
	//}
	//Strings.Add(UTF8_TO_TCHAR(bytesOriFinal));

	return 0;
}

TArray<uint8> USpeech2TextBPLibrary::Speech2TextChannelByebye(const TArray<uint8>& Stereo, TArray<uint8>& Mono)
{
	int outputIndex = 0;
	for (int n = 0; n < Stereo.Num(); n += 4)
	{
		// copy in the first 16 bit sample
		/*output[outputIndex++] = input[n];
		output[outputIndex++] = input[n + 1];*/
		Mono.Add(Stereo[n]);
		Mono.Add(Stereo[n+1]);
	}

	TArray<uint8> Resault;
	if (Stereo.Num() > 4 && Stereo.Num() % 4 == 0)
	{
		auto singleNum = Stereo.Num() / 2;
		Resault.SetNum(singleNum);

		auto data_0 = (const uint16*)Stereo.GetData();
		auto data_1 = (uint16*)Resault.GetData();

		auto loopSize = singleNum / 2;

		for (auto i = 0; i < loopSize; ++i)
		{
			data_1[i] = data_0[2 * i] > data_0[2 * i + 1] ? data_0[2 * i] : data_0[2 * i + 1];
		}
	}

	return Resault;
}

void USpeech2TextBPLibrary::Speech2TextSampleByebye(const TArray<uint8>& S441, TArray<uint8>& S160)
{
    int outputIndex = 0;
    for (int n = 0; n < S441.Num(); n += 6)
    {
        S160.Add(S441[n]);
        S160.Add(S441[n+1]);
    }
}
