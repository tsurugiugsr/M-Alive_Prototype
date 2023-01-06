// Copyright Epic Games, Inc. All Rights Reserved.

#include "Speech2TextBPLibrary.h"
#include "Speech2Text.h"

#include <ctime>
#include <string>


USpeech2TextBPLibrary::USpeech2TextBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

float USpeech2TextBPLibrary::Speech2TextInitial(const FString& appID, const FString& appKey)
{
	std::time_t current_time;
	std::time(&current_time);
	std::string base_url = "ws://rtasr.xfyun.cn/v1/ws";
	std::string timeStamp = std::to_string(current_time);
	std::string baseString = "";
	baseString.append(TCHAR_TO_UTF8(*appID));
	baseString.append(timeStamp);
	baseString = md5(baseString);
	std::string signa = "";

	uint8_t t_key[] = { 'k','e','y' };
	uint8_t t_data[] = { 'h','e','l','l','o' };
	uint8_t t_result[256] = { 0 };
	size_t t_len = sizeof(t_result);
	hmac_sha1(t_key, 3, t_data, 5, t_result, &t_len);
	int t_i;
	for (t_i = 0; t_i < t_len; t_i++)
		printf("%x ", t_result[t_i]);

	return 0;
}

